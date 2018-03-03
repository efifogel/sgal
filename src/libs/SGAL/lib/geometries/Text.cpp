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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "SGAL/Text.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Font_style.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Context.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Text::s_tag = "Text";
Container_proto* Text::s_prototype(nullptr);

REGISTER_TO_FACTORY(Text, "Text");

//! \brief constructor.
Text::Text(Boolean proto) : Geometry(proto) {}

//! \brief destructor.
Text::~Text() {}

//! \brief draw the node.
void Text::draw(Draw_action* draw_action)
{
  Context* context = draw_action->get_context();
  if (!context) return;

  if (m_string.empty()) return;
  if (!m_font_style) {
    m_default_font_style = Shared_font_style(new Font_style());
    set_font_style(m_default_font_style);
  }

  int pass_no = draw_action->get_pass_no();
  if (pass_no == 0) {
    draw_action->set_second_pass_required(true);
    return;
  }

  Gfx::Cull_face cull_face = context->get_cull_face();
  context->draw_cull_face(Gfx::NO_CULL);
  m_font_style->draw_string(context, m_string);
  context->draw_cull_face(cull_face);
}

//! \brief initializes the node prototype.
void Text::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // string
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto string_func = static_cast<String_handle_function>(&Text::string_handle);
  s_prototype->add_field_info(new SF_string(STRING, "string",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            string_func,
                                            exec_func));
}

//! \brief deletes the prototype.
void Text::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Text::get_prototype()
{
  if (!s_prototype) Text::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Text::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "string") {
      m_string = value;
      elem->mark_delete(ai);
      continue;               // Advance to next attribute
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

  // remove all the marked attributes
  elem->delete_marked();
}

//! \brief sets the font style.
void Text::set_font_style(Shared_font_style font_style)
{
  m_font_style = font_style;
  m_dirty_bounding_sphere = true;
}

//! \brief cleans the sphere bound of the text.
void Text::clean_bounding_sphere()
{
  if (!m_font_style) {
    m_default_font_style = Shared_font_style(new Font_style());
    set_font_style(m_default_font_style);
  }

  Float width, height;
  m_font_style->get_string_size(m_string, width, height);
  //! \todo Assume the origin is in the center
  Float radius =  (width < height) ? height : width;
  Vector3f center(0,0,0);
  m_bounding_sphere.set_center(center);
  m_bounding_sphere.set_radius(radius);
  m_dirty_bounding_sphere = false;
}

SGAL_END_NAMESPACE
