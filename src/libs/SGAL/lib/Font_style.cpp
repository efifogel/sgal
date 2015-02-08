// Copyright (c) 2004 Israel.
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

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MODULE_H
#include FT_STROKER_H
  //#include FT_SYNTHESIS_H
  //#include FT_LCD_FILTER_H
  //#include FT_CFF_DRIVER_H
  //#include FT_TRUETYPE_DRIVER_H
  //#include FT_INTERNAL_OBJECTS_H
  //#include FT_INTERNAL_DRIVER_H
}

#include "SGAL/basic.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Text.hpp"
#include "SGAL/Font_style.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Texture_font.hpp"
#include "SGAL/Imagemagick_font.hpp"
#include "SGAL/Font_outliner.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Font_style::s_tag = "FontStyle";
Container_proto* Font_style::s_prototype(nullptr);

REGISTER_TO_FACTORY(Font_style, "Font_style");

//! The freetype root library object.
FT_Library Font_style::s_ft_library;

//! The truetype driver.
FT_Driver Font_style::s_ft_driver;

// Default values
const Font_style::Family Font_style::s_def_family(FAMILY_SERIF);
const Boolean Font_style::s_def_horizontal(true);
const Uint_array Font_style::s_def_justify =
  boost::assign::list_of(static_cast<Uint>(JUSTIFY_BEGIN))
  (static_cast<Uint>(JUSTIFY_FIRST));
const std::string Font_style::s_def_language("");
const Boolean Font_style::s_def_left_to_right(true);
const Float Font_style::s_def_size(1.0f);
const Float Font_style::s_def_spacing(1.0f);
const Font_style::Style Font_style::s_def_style(STYLE_PLAIN);
const Boolean Font_style::s_def_top_to_bottom(true);

const Char* Font_style::s_family_names[] =
  { "SERIF", "SANS", "TYPEWRITER" };

const Char* Font_style::s_justify_names[] =
  { "BEGIN", "FIRST", "MIDDLE", "END" };

const Char* Font_style::s_style_names[] =
  { "PLAIN", "BOLD", "ITALIC", "BOLDITALIC" };

//! \brief constructor
Font_style::Font_style(Boolean proto) :
  Node(proto),
  m_family(s_def_family),
  m_horizontal(s_def_horizontal),
  m_justify(s_def_justify),
  m_language(s_def_language),
  m_left_to_right(s_def_left_to_right),
  m_size(s_def_size),
  m_spacing(s_def_spacing),
  m_style(s_def_style),
  m_top_to_bottom(s_def_top_to_bottom),
  m_dirty(true),
  m_font(nullptr),
  m_dirty_face(true)
{
  if (!proto) return;

  FT_Error err = FT_Init_FreeType(&s_ft_library);
  if (err) {
    std::cerr << "Failed to initialize FreeType library!" << std::endl;
    throw;
  }

  s_ft_driver = (FT_Driver) FT_Get_Module(s_ft_library, "truetype");
  if (!s_ft_driver) {
    std::cerr << "Failed to find the TrueType driver in FreeType 2!"
              << std::endl;
    throw;
  }
}

//! \brief Destructor
Font_style::~Font_style()
{
  if (m_font) {
    delete m_font;
    m_font = nullptr;
  }
  FT_Done_Face(m_face);
  // FT_Done_FreeType(s_ft_library);

  m_dirty_face = true;
}

//! \brief draws the node while traversing the scene graph.
inline Action::Trav_directive Font_style::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

//! \brief sets the flag that indicates whether the font is accentuated.
void Font_style::set_bold(Boolean b)
{
  m_bold = b;
  m_dirty = true;
}

/*! \brief set the flag that indicates whether the font is italicized
 * (slanted).
 */
void Font_style::set_italic(Boolean i)
{
  m_italic = i;
  m_dirty = true;
}

//! \brief sets the flag that indicates whether the font is antialiased font.
void Font_style::set_antialias(Boolean a)
{
  m_antialias = a;
  m_dirty = true;
}

//! \breif
void Font_style::on_field_change(const Field_info* /* field_info */)
{
  m_dirty = true;
  set_rendering_required();

  m_dirty_face = true;
  m_triangulations.clear();
}

//! \breif initializes the prototype.
void Font_style::init_prototype()
{
  if (s_prototype != nullptr) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  auto exec_func = static_cast<Execution_function>(&Font_style::on_field_change);

  // family
  auto family_func =
    reinterpret_cast<Uint_handle_function>(&Font_style::family_handle);
  s_prototype->add_field_info(new SF_uint(FAMILY, "family",
                                          RULE_EXPOSED_FIELD,
                                          family_func,
                                          exec_func));

  // horizontal
  auto horizontal_func =
    static_cast<Boolean_handle_function>(&Font_style::horizontal_handle);
  s_prototype->add_field_info(new SF_bool(HORIZONTAL, "horizontal",
                                          RULE_EXPOSED_FIELD,
                                          horizontal_func, exec_func));

  // justify
  auto justify_func =
    static_cast<Uint_array_handle_function>(&Font_style::justify_handle);
  s_prototype->add_field_info(new MF_uint(JUSTIFY, "justify",
                                          RULE_EXPOSED_FIELD,
                                          justify_func,
                                          exec_func));

  // language
  auto language_func =
    static_cast<String_handle_function>(&Font_style::language_handle);
  s_prototype->add_field_info(new SF_string(LANGUAGE, "language",
                                            RULE_EXPOSED_FIELD,
                                            language_func, exec_func));

  // leftToRight
  auto l2r_func = static_cast<Boolean_handle_function>(&Font_style::l2r_handle);
  s_prototype->add_field_info(new SF_bool(LEFT_TO_RIGHT, "leftToRight",
                                          RULE_EXPOSED_FIELD,
                                          l2r_func, exec_func));

  // size
  auto size_func = static_cast<Float_handle_function>(&Font_style::size_handle);
  s_prototype->add_field_info(new SF_float(SIZE, "size",
                                           RULE_EXPOSED_FIELD,
                                           size_func,
                                           exec_func));

  // spacing
  auto spacing_func =
    static_cast<Float_handle_function>(&Font_style::spacing_handle);
  s_prototype->add_field_info(new SF_float(SPACING, "spacing",
                                           RULE_EXPOSED_FIELD,
                                           spacing_func,
                                           exec_func));

  // style
  auto style_func =
    reinterpret_cast<Uint_handle_function>(&Font_style::style_handle);
  s_prototype->add_field_info(new SF_uint(STYLE, "style",
                                          RULE_EXPOSED_FIELD,
                                          style_func,
                                          exec_func));

  // topToBottom
  auto t2b_func = static_cast<Boolean_handle_function>(&Font_style::t2b_handle);
  s_prototype->add_field_info(new SF_bool(TOP_TO_BOTTOM, "topToBottom",
                                          RULE_EXPOSED_FIELD,
                                          t2b_func, exec_func));
}

//! \brief deletes the prototype.
void Font_style::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Font_style::get_prototype()
{
  if (!s_prototype)  Font_style::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Font_style::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "family") {
      Uint num = sizeof(s_family_names) / sizeof(char*);
      const char** found = std::find(s_family_names, &s_family_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_family_names;
      if (index < num) set_family(static_cast<Family>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "horizontal") {
      set_horizontal(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "justify") {
      Uint num_values = get_num_tokens(value);
      m_justify.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_justify[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "language") {
      set_language(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "leftToRight")  {
      set_left_to_right(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "size") {
      set_size(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "spacing") {
      set_spacing(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "style") {
      Uint num = sizeof(s_style_names) / sizeof(char*);
      const char** found = std::find(s_style_names, &s_style_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_style_names;
      if (index < num) set_style(static_cast<Style>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "topToBottom") {
      set_top_to_bottom(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Font_style::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  Vector3f vec;
  Rotation rot;

  attribs = Node::get_attributes();

  if (!m_family.empty()) {
    attrib.first = "family";
    attrib.second = m_family;
    attribs.push_back(attrib);
  }
  if (!m_style.empty() && !m_style.equal("PLAIN")) {
    attrib.first = "style";
    attrib.second = m_style;
    attribs.push_back(attrib);
  }
  if (!m_horizontal) {
    attrib.first = "horizontal";
    attrib.second = (m_horizontal)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }
  if (!m_justify.empty() && !m_justify.equal("PLAIN")) {
    attrib.first = "justify";
    attrib.second = m_justify;
    attribs.push_back(attrib);
  }
  if (!m_language.empty()) {
    attrib.first = "language";
    attrib.second = m_language;
    attribs.push_back(attrib);
  }
  if (!m_left_to_right) {
    attrib.first = "leftToRight";
    attrib.second = (m_left_to_right)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }
  if (m_spacing != 1) {
    attrib.first = "spacing";
    attrib.second = String::fromFloat(m_spacing,2);
    attribs.push_back(attrib);
  }
  if (m_size != 1) {
    attrib.first = "size";
    attrib.second = String::fromFloat(m_size,2);
    attribs.push_back(attrib);
  }
  if (!m_top2Bottom) {
    attrib.first = "topToBottom";
    attrib.second = (m_top2Bottom)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief cleans the face.
void Font_style::clean_face()
{
  const char* file_name ="/usr/share/fonts/truetype/msttcorefonts/Verdana.ttf";
  FT_Error err = FT_New_Face(s_ft_library, file_name, 0, &m_face);
  if (err) {
    std::cerr << "Failed to open input font file!" << std::endl;
    throw;
  }

  // Find driver and check format
  if (m_face->driver != s_ft_driver) {
    err = FT_Err_Invalid_File_Format;
    std::cerr << "is not a TrueType font!" << std::endl;
    throw;
  }

  m_dirty_face = false;
}

//! \brief obtains the font.
Font* Font_style::get_font()
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  return m_font;
}

//! obtains constructs and initializes the font.
void Font_style::create_font()
{
  m_font = new Imagemagick_font(m_font_name, m_antialias,
                                m_left_to_right, m_top_to_bottom);
  m_font->init();
  m_dirty = false;
}

//! Draw one string.
void Font_style::draw_string(Context* context, const std::string& str)
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  m_font->draw_string(context, str, m_size);
}

//! Obtain the width and height of the string.
void Font_style::get_string_size(const std::string& str,
                                 Float& width, Float& height)
{
  Uint w, h;
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  m_font->get_string_size(str, w, h);
  height = m_size;
  width = height * w / h;
}

//! \brief sets the font family.
void Font_style::set_family(Family family) { m_family = family; }

/*! \brief sets the flag that indicates whether the text advances horizontally
 * in its major direction.
 */
void Font_style::set_horizontal(Boolean flag) { m_horizontal = flag; }

/*! \brief sets the alignment of the above text layout relative to the origin
 * of the object coordinate system.
 */
void Font_style::set_justify(const Uint_array& justify)
{ m_justify = justify; }

//! \brief sets the context of the language for the text string.
void Font_style::set_language(const std::string& labguage)
{ m_language = labguage; }

/*! \brief sets the flag that indicates whether the text advances from left to
 * right.
 */
void Font_style::set_left_to_right(Boolean flag) { m_left_to_right = flag; }

//! \brief sets the nominal height (in object space units) of glyphs rendered.
void Font_style::set_size(Float size) { m_size = size; }

//! \brief sets the line spacing between adjacent lines of text.
void Font_style::set_spacing(Float spacing) { m_spacing = spacing; }

//! \brief sets the font style, e.g., bold.
void Font_style::set_style(Style style) {m_style = style; }

/*! \brief sets the flag that indicates whether the text advances from top to
 * bottom.
 */
void Font_style::set_top_to_bottom(Boolean flag) { m_top_to_bottom = flag; }

//! \brief processes 'move to' instructions.
int Font_style::move_to(FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b0(to->x, to->y);
  font_outliner->move_to(b0);
  return 0;
}

//! \brief processes 'line to' instructions.
int Font_style::line_to(FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(to->x, to->y);
  font_outliner->line_to(b1);
  return 0;
}

//! processes 'conic to' instructions.
int Font_style::conic_to(FT_Vector* control, FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(control->x, control->y);
  Vector2f b2(to->x, to->y);
  font_outliner->conic_to(b1, b2);
  return 0;
}

//! processes 'cubic to' instructions.
int Font_style::cubic_to(FT_Vector* control1, FT_Vector* control2,
                         FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(control1->x, control1->y);
  Vector2f b2(control2->x, control2->y);
  Vector2f b3(to->x, to->y);
  font_outliner->cubic_to(b1, b2, b3);
  return 0;
}

//! \brief computes the outlines.
void Font_style::compute_outlines(char c, Outlines& outlines)
{
  if (m_dirty_face) clean_face();

  // Load glyph
  FT_Glyph glyph;
  FT_Error err =
    FT_Load_Char(m_face, c, FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE);
  if (err) {
    std::cerr << "FT_Load_Glyph: error!" << std::endl;
  }

  // FT_Get_Glyph(face->glyph, &glyph);
  FT_Outline outline = m_face->glyph->outline;

  if (m_face->glyph->format != ft_glyph_format_outline) {
    std::cerr << "Not an outline font!" << std::endl;
  }

  FT_Outline_Funcs funcs;
  funcs.shift = 0;
  funcs.delta = 0;
  funcs.move_to = (FT_Outline_MoveTo_Func)&move_to;
  funcs.line_to = (FT_Outline_LineTo_Func)&line_to;
  funcs.conic_to = (FT_Outline_ConicTo_Func)&conic_to;
  funcs.cubic_to = (FT_Outline_CubicTo_Func)&cubic_to;
  // trace outline of the glyph
  Font_outliner outliner(outlines);
  err = FT_Outline_Decompose(&outline, &funcs, &outliner);
  if (err) {
    std::cerr << "Failed to decompose!" << std::endl;
    throw;
  }

  // Remove last point from every outline, as it is a replicate of the first
  for (auto oit = outlines.begin(); oit != outlines.end(); ++oit)
    oit->pop_back();
}

//! Insert an outline into a triangulation.
Uint Font_style::insert_outline(Triangulation& tri, const Outline& outline,
                                Uint k) const
{
  auto pit = outline.begin();
  Triangulation::Point p((*pit)[0], (*pit)[1]);
  Triangulation::Vertex_handle start = tri.insert(p);
  start->info() = k++;
  Triangulation::Vertex_handle prev = start;
  for (++pit; pit != outline.end(); ++pit) {
    Triangulation::Point p((*pit)[0], (*pit)[1]);
    Triangulation::Vertex_handle next = tri.insert(p);
    next->info() = k++;
    tri.insert_constraint(prev, next);
    prev = next;
  }
  tri.insert_constraint(prev, start);
  return k;
}

//! \brief marks
void Font_style::mark_domains(Triangulation& tri,
                              typename Triangulation::Face_handle start,
                              int index,
                              std::list<typename Triangulation::Edge>& border)
  const
{
  if (start->info().nesting_level != -1) return;
  std::list<typename Triangulation::Face_handle> queue;
  queue.push_back(start);
  while (! queue.empty()) {
    typename Triangulation::Face_handle fh = queue.front();
    queue.pop_front();
    if (fh->info().nesting_level == -1) {
      fh->info().nesting_level = index;
      for (int i = 0; i < 3; i++) {
        typename Triangulation::Edge e(fh,i);
        typename Triangulation::Face_handle n = fh->neighbor(i);
        if (n->info().nesting_level == -1) {
          if (tri.is_constrained(e)) border.push_back(e);
          else queue.push_back(n);
        }
      }
    }
  }
}

//! \brief marks facets in a triangulation that are inside the domain.
// Explores set of facets connected with non constrained edges,
// and attribute to each such set a nesting level.
// We start from facets incident to the infinite vertex, with a nesting
// level of 0. Then we recursively consider the non-explored facets incident
// to constrained edges bounding the former set and increase the nesting
// level by 1.
// Facets in the domain are those with an odd nesting level.
void Font_style::mark_domains(Triangulation& tri) const
{
  typename Triangulation::All_faces_iterator it;
  for (it = tri.all_faces_begin(); it != tri.all_faces_end(); ++it)
    it->info().nesting_level = -1;

  std::list<typename Triangulation::Edge> border;
  mark_domains(tri, tri.infinite_face(), 0, border);
  while (! border.empty()) {
    typename Triangulation::Edge e = border.front();
    border.pop_front();
    typename Triangulation::Face_handle n = e.first->neighbor(e.second);
    if (n->info().nesting_level == -1)
      mark_domains(tri, n, e.first->info().nesting_level+1, border);
  }
}

//! \brief computes the glyph of a character.
const Font_style::Triangulation& Font_style::compute_glyph(char c)
{
  Outlines outlines;
  compute_outlines(c, outlines);

  // Compute the triangulations
  auto it = m_triangulations.find(c);
  if (it != m_triangulations.end()) return it->second;

  auto& tri = m_triangulations[c];
  Uint k(0);
  for (auto oit = outlines.begin(); oit != outlines.end(); ++oit) {
    const auto& outline = *oit;
    k = insert_outline(tri, outline, k);
  }
  mark_domains(tri);  // mark facets that are inside the domain
  return tri;
}

SGAL_END_NAMESPACE
