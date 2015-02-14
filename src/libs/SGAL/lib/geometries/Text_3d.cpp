// Copyright (c) 2015 Israel.
// All rights reserved.
//
// This file is part of SGAL; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the
// License. See the file LICENSE.LGPL distributed with SGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the
// software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4996 )
#endif

#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Text_3d.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Font_style.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Text_3d::s_tag = "Text3D";
Container_proto* Text_3d::s_prototype(nullptr);

// Default values:
const Float Text_3d::s_def_max_extent(0);
const Float Text_3d::s_def_depth(0.1f);

REGISTER_TO_FACTORY(Text_3d, "Text_3d");

//! \brief constructor.
Text_3d::Text_3d(Boolean proto) :
  Indexed_face_set(proto),
  m_max_extent(s_def_max_extent),
  m_depth(s_def_depth),
  m_dirty_ucs4_strings(true)
{}

//! \brief destructor.
Text_3d::~Text_3d()
{
  m_strings.clear();
  m_lengths.clear();
}

//! \brief initilalizes the container prototype.
void Text_3d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Indexed_face_set::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func = static_cast<Execution_function>(&Text_3d::structure_changed);

  // string
  auto strings_func =
    static_cast<String_array_handle_function>(&Text_3d::strings_handle);
  s_prototype->add_field_info(new MF_string(STRINGS, "string",
                                            RULE_EXPOSED_FIELD,
                                            strings_func, exec_func));

  // fontStyle
  auto font_style_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Text_3d::font_style_handle);
  s_prototype->add_field_info(new SF_shared_container(FONT_STYLE, "fontStyle",
                                                      RULE_EXPOSED_FIELD,
                                                      font_style_func,
                                                      exec_func));

  // length
  auto lengths_func =
    static_cast<Float_array_handle_function>(&Text_3d::lengths_handle);
  s_prototype->add_field_info(new MF_float(LENGTHS, "length",
                                           RULE_EXPOSED_FIELD,
                                           lengths_func, exec_func));

  // maxExtent
  auto max_extent_func =
    static_cast<Float_handle_function>(&Text_3d::max_extent_handle);
  s_prototype->add_field_info(new SF_float(MAX_EXTENT, "maxExtent",
                                           RULE_EXPOSED_FIELD, max_extent_func,
                                           s_def_max_extent, exec_func));

  // depth
  auto depth_func = static_cast<Float_handle_function>(&Text_3d::depth_handle);
  s_prototype->add_field_info(new SF_float(DEPTH, "depth", RULE_EXPOSED_FIELD,
                                           depth_func, s_def_depth, exec_func));
}

//! \brief deletes the container prototype.
void Text_3d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Text_3d::get_prototype()
{
  if (!s_prototype) Text_3d::init_prototype();
  return s_prototype;
}

//! \brief sets the extrusion attributes.
void Text_3d::set_attributes(Element* elem)
{
  Indexed_face_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "string") {
      auto start = value.find_first_of('\"');
      while (start != std::string::npos) {
        ++start;
        auto end = value.find_first_of('\"', start);
        if (end == std::string::npos) break;
        m_strings.push_back(value.substr(start, end-start));
        ++end;
        if (end == value.size()) break;
        start = value.find_first_of('\"', end);
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "length") {
      auto num_values = get_num_tokens(value);
      m_lengths.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_lengths[i];
      elem->mark_delete(ai);
    }
    if (name == "maxExtent") {
      set_max_extent(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depth") {
      set_depth(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "fontStyle") {
      Shared_font_style font_style =
        boost::dynamic_pointer_cast<Font_style>(cont);
      set_font_style(font_style);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief processes change of structure.
void Text_3d::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_flat_coord_indices();
  field_changed(field_info);
}

//! \brief create a quadrilateral.
size_t Text_3d::create_quad(Uint a, Uint b, Uint c, Uint d, size_t k)
{
  m_flat_coord_indices[k++] = a;
  m_flat_coord_indices[k++] = b;
  m_flat_coord_indices[k++] = c;
  m_flat_coord_indices[k++] = a;
  m_flat_coord_indices[k++] = c;
  m_flat_coord_indices[k++] = d;
  return k;
}

//! \brief cleans the coordinate array.
void Text_3d::clean_coords()
{
  // Compute the outlines
  if (!m_font_style) m_font_style = Shared_font_style(new Font_style());
  SGAL_assertion(m_font_style);

  if (m_dirty_ucs4_strings) clean_ucs4_strings();

  // auto c = m_ucs4_strings.front().front();
  // const auto& tri = m_font_style->compute_glyph_geometry(c);

  ///////////////////////////
  typedef std::list<const Font_style::Triangulation*>  Line_geometry;
  std::vector<Line_geometry> line_geometrys(m_ucs4_strings.size());

  auto lgit = line_geometrys.begin();
  for (auto lit = m_ucs4_strings.begin(); lit != m_ucs4_strings.end(); ++lit) {
    const auto& line = *lit;
    auto& line_geometry = *lgit++;
    auto line_num = std::distance(m_ucs4_strings.begin(), lit);
    // Vector2f pen_start = get_pen_start_for_line(line_num,
    //                                             size,
    //                                             spacing,
    //                                             horizontal,
    //                                             leftToRight,
    //                                             topToBottom);

    // auto_ptr<line_geometry> line_geom(new line_geometry(horizontal,
    //                                                     leftToRight,
    //                                                     topToBottom,
    //                                                     pen_start));
    for (auto cit = line.begin(); cit != line.end(); ++cit) {
      auto c = *cit;
      const auto& tri = m_font_style->compute_glyph_geometry(c);
      line_geometry.push_back(&tri);

      // const auto* glyphGeometry = 0;
      // auto pos = this->glyph_geometry_map.find(glyphIndex);
      // if (pos != this->glyph_geometry_map.end()) glyphGeometry = &pos->second;
      // else {
      //   glyph_geometry_map_t::value_type
      //     value(glyphIndex, glyph_geometry(this->face, glyphIndex, size));
      //   const auto result = this->glyph_geometry_map.insert(value);
      //   assert(result.second);
      //   glyphGeometry = &result.first->second;
      // }
      // assert(glyphGeometry);
      // line_geom->add(*glyphGeometry);
    }

    // Scale to length.
    // auto length = (line_num < m_lengths.size()) ? m_lengths[line_num] : 0.0f;
    // if (length > 0.0f) line_geom->scale(length);

    // lines.push_back(line_geom);
  }

  // auto max_extent = std::max(get_max_extent(), 0.0f);

  ///////////////////////////

  // Compute the number of coordinates
  Uint size(0);
  for (auto tit = line_geometrys.begin(); tit != line_geometrys.end(); ++tit) {
    const auto& line_geometry = *tit;
    for (auto lit = line_geometry.begin(); lit != line_geometry.end(); ++lit) {
      const auto* tri = *lit;
      size += tri->number_of_vertices();
    }
  }

  // Resize the coordinate array
  if (!m_coord_array) m_coord_array.reset(new Coord_array_3d);
  auto coords = boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);
  coords->resize(size + size);          // back and front
  std::cout << "# coords: " << coords->size() << std::endl;

  // Assign the coordinate array
  Float ttt(0.0f);
  Uint k(0);
  for (auto tit = line_geometrys.begin(); tit != line_geometrys.end(); ++tit) {
    const auto& line_geometry = *tit;
    for (auto lit = line_geometry.begin(); lit != line_geometry.end(); ++lit) {
      const auto* tri = *lit;
      size = tri->number_of_vertices();
      for (auto it = tri->finite_vertices_begin();
           it != tri->finite_vertices_end(); ++it)
      {
        auto x = static_cast<Float>(CGAL::to_double(it->point().x()));
        auto y = static_cast<Float>(CGAL::to_double(it->point().y()));
        (*coords)[k].set(x+ttt, y, 0);
        (*coords)[k+size].set(x+ttt, y, m_depth);
        ++k;
      }
      ttt += 1000;
      k += size;
    }
  }

  // Calculate the number of primitives
  Uint num_primitives = 0;
  for (auto tit = line_geometrys.begin(); tit != line_geometrys.end(); ++tit) {
    const auto& line_geometry = *tit;
    for (auto lit = line_geometry.begin(); lit != line_geometry.end(); ++lit) {
      const auto* tri = *lit;
      for (auto it = tri->finite_faces_begin();
           it != tri->finite_faces_end(); ++it)
        if (it->info().in_domain()) num_primitives += 2;

      for (auto it = tri->finite_edges_begin();
           it != tri->finite_edges_end(); ++it)
      {
        auto f1 = it->first;
        auto f2 = f1->neighbor(it->second);
        if ((f1->info().in_domain() && !f2->info().in_domain()) ||
            (f2->info().in_domain() && !f1->info().in_domain()))
          num_primitives += 2;
      }
    }
  }

  // Compute the indices
  m_flat_coord_indices.resize(num_primitives * 3);
  std::cout << "# indices: " << m_flat_coord_indices.size() << std::endl;
  size_t i(0);
  Uint base(0);
  for (auto tit = line_geometrys.begin(); tit != line_geometrys.end(); ++tit) {
    const auto& line_geometry = *tit;
    for (auto lit = line_geometry.begin(); lit != line_geometry.end(); ++lit) {
      const auto* tri = *lit;
      size = tri->number_of_vertices();
      // Set the front indices
      for (auto it = tri->finite_faces_begin(); it != tri->finite_faces_end();
           ++it)
      {
        if (! it->info().in_domain()) continue;
        m_flat_coord_indices[i++] = it->vertex(2)->info() + base;
        m_flat_coord_indices[i++] = it->vertex(1)->info() + base;
        m_flat_coord_indices[i++] = it->vertex(0)->info() + base;
      }
      // Set the back indices
      for (auto it = tri->finite_faces_begin(); it != tri->finite_faces_end();
           ++it)
      {
        if (! it->info().in_domain()) continue;
        m_flat_coord_indices[i++] = it->vertex(0)->info() + base + size;
        m_flat_coord_indices[i++] = it->vertex(1)->info() + base + size;
        m_flat_coord_indices[i++] = it->vertex(2)->info() + base + size;
      }

      // Set the side indices
      for (auto it = tri->finite_edges_begin(); it != tri->finite_edges_end();
           ++it)
      {
        auto f1 = it->first;
        auto f2 = f1->neighbor(it->second);
        if (f1->info().in_domain() && !f2->info().in_domain()) {
          auto a = f1->vertex(f1->ccw(it->second))->info() + base;
          auto b = f1->vertex(f1->cw(it->second))->info() + base;
          auto c = b + size;
          auto d = a + size;
          i = create_quad(a, b, c, d, i);
        }
        else if (f2->info().in_domain() && !f1->info().in_domain()) {
          auto a = f1->vertex(f1->cw(it->second))->info() + base;
          auto b = f1->vertex(f1->ccw(it->second))->info() + base;
          auto c = b + size;
          auto d = a + size;
          i = create_quad(a, b, c, d, i);
        }
      }
      base += 2 * size;
    }
  }

  set_primitive_type(PT_TRIANGLES);
  set_num_primitives(num_primitives);
  set_solid(true);

  coord_content_changed(get_field_info(COORD_ARRAY));

  m_dirty_coord_indices = true;
  m_dirty_flat_coord_indices = false;
  m_coord_indices_flat = true;
}

//! \brief cleans the coordinate indices.
void Text_3d::clean_flat_coord_indices()
{
}

//! calculates the default 2D texture-mapping oordinates.
void Text_3d::clean_tex_coord_array_2d()
{
  m_dirty_tex_coord_array = false;
}

//! Generate the texture coordinate indices.
void Text_3d::clean_flat_tex_coord_indices()
{
  m_dirty_tex_coord_indices = true;
  m_dirty_flat_tex_coord_indices = false;
  m_tex_coord_indices_flat = true;
}

//! \brief sets the text strings.
void Text_3d::set_strings(const String_array& strings)
{
  m_strings = strings;
  structure_changed(get_field_info(STRINGS));
  m_ucs4_strings.clear();
}

//! \brief sets the font style.
void Text_3d::set_font_style(Shared_font_style font_style)
{
  m_font_style = font_style;
  structure_changed(get_field_info(FONT_STYLE));
}

//! \brief sets the lengths of the text strings.
void Text_3d::set_lengths(const Float_array& lengths)
{
  m_lengths = lengths;
  structure_changed(get_field_info(LENGTHS));
}

//! \brief sets the maximum extent of the text strings.
void Text_3d::set_max_extent(Float max_extent)
{
  m_max_extent = max_extent;
  structure_changed(get_field_info(MAX_EXTENT));
}

//! \brief sets the depth of the 3D text.
void Text_3d::set_depth(Float depth)
{
  m_depth = depth;
  structure_changed(get_field_info(DEPTH));
}

//! \brief
std::ptrdiff_t Text_3d::utf8_to_ucs4(const unsigned char* src_orig,
                                     Char32 & dst, size_t len) const
{
  const unsigned char* src = src_orig;
  if (len == 0) return 0;

  unsigned char s = *src++;
  --len;

  Char32 result;
  size_t extra;
  if (!(s & 0x80)) {
    result = s;
    extra = 0;
  }
  else if (!(s & 0x40)) return -1;
  else if (!(s & 0x20)) {
    result = s & 0x1f;
    extra = 1;
  }
  else if (!(s & 0x10)) {
    result = s & 0xf;
    extra = 2;
  }
  else if (!(s & 0x08)) {
    result = s & 0x07;
    extra = 3;
  }
  else if (!(s & 0x04)) {
    result = s & 0x03;
    extra = 4;
  }
  else if ( ! (s & 0x02)) {
    result = s & 0x01;
    extra = 5;
  }
  else return -1;

  if (extra > len) return -1;

  while (extra--) {
    result <<= 6;
    s = *src++;
    if ((s & 0xc0) != 0x80) return -1;

    result |= s & 0x3f;
  }
  dst = result;
  return src - src_orig;
}

//! \brief
Boolean Text_3d::utf8_len(const unsigned char* utf8_str, size_t len,
                          size_t& chars, size_t& max_char_width) const
{
  int n = 0;
  Char32 max = 0;
  while (len) {
    Char32 c;
    const std::ptrdiff_t clen = utf8_to_ucs4(utf8_str, c, len);
    if (clen <= 0) return false;        // malformed UTF8 string
    if (c > max) max = c;
    utf8_str += clen;
    len -= clen;
    ++n;
  }
  chars = n;
  if (max >= 0x10000) max_char_width = 4;
  else if (max > 0x100) max_char_width = 2;
  else max_char_width = 1;
  return true;
}

//! \brief cleans the fixed-length encodings of the strings.
void Text_3d::clean_ucs4_strings()
{
  m_ucs4_strings.clear();
  m_ucs4_strings.resize(m_strings.size());
  auto tit = m_ucs4_strings.begin();
  for (auto it = m_strings.begin(); it != m_strings.end(); ++it, ++tit) {
    const auto& str = *it;
    auto& ucs4_str = *tit;

    // Convert the characters from UTF-8 to UCS-4.
    std::vector<unsigned char> utf8_str(str.begin(), str.end());
    size_t num_chars = 0, max_char_width = 0;
    const bool well_formed =
      utf8_len(&utf8_str[0], utf8_str.size(), num_chars, max_char_width);
    if (well_formed) {
      ucs4_str.resize(num_chars);
      auto utf8_it = utf8_str.begin();
      std::vector<Char32>::iterator ucs4_it = ucs4_str.begin();
      while (utf8_it != utf8_str.end()) {
        const std::ptrdiff_t utf8_bytes =
          utf8_to_ucs4(&*utf8_it, *ucs4_it, utf8_str.end() - utf8_it);
        utf8_it += utf8_bytes;
        ++ucs4_it;
      }
    }
  }
  m_dirty_ucs4_strings = false;
}

SGAL_END_NAMESPACE
