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

SGAL_BEGIN_NAMESPACE

const std::string Font_style::s_tag = "FontStyle";
Container_proto* Font_style::s_prototype(nullptr);

REGISTER_TO_FACTORY(Font_style, "Font_style");

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
  m_font(nullptr)
{
}

//! \brief Destructor
Font_style::~Font_style()
{
  if (m_font) {
    delete m_font;
    m_font = nullptr;
  }
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
}

//! \breif initializes the prototype.
void Font_style::init_prototype()
{
  if (s_prototype != nullptr) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  // family
  Execution_function exec_func =
    static_cast<Execution_function>(&Font_style::on_field_change);
  Uint_handle_function family_func =
    reinterpret_cast<Uint_handle_function>(&Font_style::family_handle);
  s_prototype->add_field_info(new SF_uint(FAMILY, "family",
                                          RULE_EXPOSED_FIELD,
                                          family_func,
                                          exec_func));

  // horizontal
  Boolean_handle_function horizontal_func =
    static_cast<Boolean_handle_function>(&Font_style::horizontal_handle);
  s_prototype->add_field_info(new SF_bool(HORIZONTAL, "horizontal",
                                          RULE_EXPOSED_FIELD,
                                          horizontal_func, exec_func));

  // justify
  Uint_array_handle_function justify_func =
    static_cast<Uint_array_handle_function>(&Font_style::justify_handle);
  s_prototype->add_field_info(new MF_uint(JUSTIFY, "justify",
                                          RULE_EXPOSED_FIELD,
                                          justify_func,
                                          exec_func));

  // language
  String_handle_function language_func =
    static_cast<String_handle_function>(&Font_style::language_handle);
  s_prototype->add_field_info(new SF_string(LANGUAGE, "language",
                                            RULE_EXPOSED_FIELD,
                                            language_func, exec_func));

  // leftToRight
  Boolean_handle_function l2r_func =
    static_cast<Boolean_handle_function>(&Font_style::l2r_handle);
  s_prototype->add_field_info(new SF_bool(LEFT_TO_RIGHT, "leftToRight",
                                          RULE_EXPOSED_FIELD,
                                          l2r_func, exec_func));

  // size
  Float_handle_function size_func =
    static_cast<Float_handle_function>(&Font_style::size_handle);
  s_prototype->add_field_info(new SF_float(SIZE, "size",
                                           RULE_EXPOSED_FIELD,
                                           size_func,
                                           exec_func));

  // spacing
  Float_handle_function spacing_func =
    static_cast<Float_handle_function>(&Font_style::spacing_handle);
  s_prototype->add_field_info(new SF_float(SPACING, "spacing",
                                           RULE_EXPOSED_FIELD,
                                           spacing_func,
                                           exec_func));

  // style
  Uint_handle_function style_func =
    reinterpret_cast<Uint_handle_function>(&Font_style::style_handle);
  s_prototype->add_field_info(new SF_uint(STYLE, "style",
                                          RULE_EXPOSED_FIELD,
                                          style_func,
                                          exec_func));

  // topToBottom
  Boolean_handle_function t2b_func =
    static_cast<Boolean_handle_function>(&Font_style::t2b_handle);
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

SGAL_END_NAMESPACE
