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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Color_background.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_infos.hpp"
// #include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Color_background::s_tag = "ColorBackground";
Container_proto* Color_background::s_prototype(NULL);

// Default values:
const Vector4f Color_background::m_def_color(1, 1, 1, 0);

REGISTER_TO_FACTORY(Color_background, "Color_background");

/*! Constructor */
Color_background::Color_background(Boolean proto) :
  Background(proto),
  m_color(m_def_color)
{}

/*! Destructor */
Color_background::~Color_background() {}

/*! \brief draws the background. This is done by setting the clear color. */
void Color_background::draw(Draw_action* draw_action)
{
  Context* context = draw_action->get_context();
  if (!context) return;
  Uint which = Gfx::COLOR_CLEAR;
  if (get_clear_depth()) which |= Gfx::DEPTH_CLEAR;
  if (get_clear_stencil()) which |= Gfx::STENCIL_CLEAR;
  context->clear(which, m_color);
}

/*! \brief sets the attributes of this object. */
void Color_background::set_attributes(Element* elem)
{
  Background::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "color") {
      set_color(Vector4f(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene. */
void Color_background::add_to_scene(Scene_graph* sg)
{ Background::add_to_scene(sg); }

#if 0
/*! \brief obtains the attributes. */
Attribute_list Color_background::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  if (m_color != m_def_color) {
    attrib.first = "color";
    attrib.second = get_color().get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

/*! \brief */
void Color_background::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Background::get_prototype());

  // Add the object fields to the prototype
  // color
  s_prototype->add_field_info(new SF_vector4f(COLOR, "color",
                                              get_member_offset(&m_color)));
}

/*! \brief */
void Color_background::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Color_background::get_prototype()
{
  if (s_prototype == NULL) Color_background::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
