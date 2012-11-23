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
// $Source$
// $Revision: 13560 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

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

std::string Font_style::s_tag = "FontStyle";
Container_proto* Font_style::s_prototype = NULL;

REGISTER_TO_FACTORY(Font_style, "Font_style");

/*! Constructor */
Font_style::Font_style(Boolean proto) :
  Node(proto),
  m_family("fixedsys"),
  m_horizontal(SGAL_TRUE),
  m_justify("PLAIN"),
  m_language(""),
  m_left_to_right(SGAL_TRUE),
  m_size(1),
  m_spacing(1),
  m_style("PLAIN"),
  m_top_to_bottom(SGAL_TRUE),
  m_dirty(SGAL_TRUE),
  m_font(NULL)
{
  //m_family = "MS Serif"; // SERIF
}

/*! Destructor */
Font_style::~Font_style()
{
  if (m_font) {
    delete m_font;
    m_font = NULL;
  }
}

/*! \brief sets the flag that indicates whether the font is accentuated */
void Font_style::set_bold(Boolean b)
{
  m_bold = b;
  m_dirty = SGAL_TRUE;
}

/*! \brief set the flag that indicates whether the font is italicized
 * (slanted)
 */
void Font_style::set_italic(Boolean i)
{
  m_italic = i;
  m_dirty = SGAL_TRUE;
}
  
/*! \brief sets the flag that indicates whether the font is antialiased font */
void Font_style::set_antialias(Boolean a)
{
  m_antialias = a;
  m_dirty = SGAL_TRUE;
}

/*! \breif sets the flag that indicates the horizontal text direction */
void Font_style::set_left_to_right(Boolean l2r)
{
  m_left_to_right = l2r;
}

/*! \breif sets the flag that indicates the vertical text direction */
void Font_style::set_top_to_bottom(Boolean t2b)
{
  m_top_to_bottom = t2b;
}

/*!
 */
void Font_style::on_field_change(Field_info* /* field_info */)
{
  m_dirty = SGAL_TRUE;
  set_rendering_required();
}

/*! init_prototype() initializes the node prototype */
void Font_style::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype != NULL) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&Font_style::on_field_change);
  s_prototype->add_field_info(new SF_string(FAMILY, "family",
                                            get_member_offset(&m_family),
                                            exec_func));

  s_prototype->add_field_info(new SF_string(STYLE, "style",
                                            get_member_offset(&m_style),
                                            exec_func));

  s_prototype->add_field_info(new SF_bool(HORIZONTAL, "horizontal",
                                          get_member_offset(&m_horizontal),
                                          exec_func));

  s_prototype->add_field_info(new SF_string(JUSTIFY, "justify",
                                            get_member_offset(&m_justify),
                                            exec_func));

  s_prototype->add_field_info(new SF_string(LANGUAGE, "language",
                                            get_member_offset(&m_language),
                                            exec_func));

  s_prototype->add_field_info(new SF_bool(LEFT_TO_RIGHT, "leftToRight",
                                          get_member_offset(&m_left_to_right),
                                          exec_func));

  s_prototype->add_field_info(new SF_float(SIZE, "size",
                                           get_member_offset(&m_size),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(SPACING, "spacing",
                                           get_member_offset(&m_spacing),
                                           exec_func));

  s_prototype->add_field_info(new SF_bool(TOP_TO_BOTTOM, "topToBottom",
                                          get_member_offset(&m_top_to_bottom),
                                          exec_func));
}

/*!
 */
void Font_style::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*!
 */
Container_proto* Font_style::get_prototype() 
{  
  if (!s_prototype)  Font_style::init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Font_style::set_attributes(Element* elem) 
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "family") {
      m_family = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "style") {
      m_style = value;
      if (m_style == "BOLD") m_bold = SGAL_TRUE;
      if (m_style == "ITALIC") m_italic = SGAL_TRUE;
      if (m_style == "BOLDITALIC") {
        m_bold = SGAL_TRUE;
        m_italic = SGAL_TRUE;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "size") {
      m_size = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "horizontal") {
      m_horizontal = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "justify") {
      m_justify = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "language") {
      m_language = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "leftToRight")  {
      m_left_to_right = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "spacing") {
      m_spacing = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "topToBottom") {
      m_top_to_bottom = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
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

/*! \brief obtains the font */
Font* Font_style::get_font()
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  return m_font;
}

/*! obtains constructs and initializes the font */
void Font_style::create_font()
{
  m_font = new Imagemagick_font(m_font_name, m_antialias,
                                m_left_to_right, m_top_to_bottom);
  m_font->init();
  m_dirty = SGAL_FALSE;
}

/*! Draw one string */
void Font_style::draw_string(Context* context, const std::string& str)
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  m_font->draw_string(context, str, m_size);
}

/*! Obtain the width and height of the string */
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

SGAL_END_NAMESPACE
