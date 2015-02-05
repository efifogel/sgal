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
  m_depth(s_def_depth)
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
      for (Uint i = 0; i < num_values; ++i) svalue >> m_normal_indices[i];
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

//! \brief cleans the coordinate array.
void Text_3d::clean_coords()
{
  std::cout << "Text_3d::clean_coords()" << std::endl;
  SGAL_assertion(m_font_style);
  Font_style::Outlines outlines;
  m_font_style->compute_outlines(m_strings.front(), outlines);

  set_primitive_type(PT_TRIANGLES);

  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief cleans the coordinate indices.
void Text_3d::clean_flat_coord_indices()
{
}

//! calculates the default 2D texture-mapping oordinates.
void Text_3d::clean_tex_coord_array_2d()
{
}

//! Generate the texture coordinate indices.
void Text_3d::clean_flat_tex_coord_indices()
{
}

//! \brief sets the text strings.
void Text_3d::set_strings(const String_array& strings)
{
  m_strings = strings;
  structure_changed(get_field_info(STRINGS));
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

SGAL_END_NAMESPACE
