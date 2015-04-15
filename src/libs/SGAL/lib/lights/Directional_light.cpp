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

#include "SGAL/Directional_light.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Directional_light::s_tag = "DirectionalLight";
Container_proto * Directional_light::s_prototype(NULL);

/*! The default direction of the light */
const Vector3f Directional_light::s_def_direction(0, 0, -1);

REGISTER_TO_FACTORY(Directional_light, "Directional_light");

//! \brief constructs.
Directional_light::Directional_light(Boolean proto) :
  Light(proto),
  m_direction(s_def_direction)
{}

//! \brief destructs.
Directional_light::~Directional_light() {}

//! \brief draws the light.
Action::Trav_directive Directional_light::draw(Draw_action* draw_action)
{
  if (!m_is_on) return Action::TRAV_CONT;

  Light::draw(draw_action);
  int light_id  = GL_LIGHT0 + m_id;
  Vector4f dir(m_direction);
  dir.negate();
  glLightfv(light_id, GL_POSITION, (float *)&dir);

  return Action::TRAV_CONT;
}

//! \brief initializes the node prototype.
void Directional_light::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Light::get_prototype());

  // Add the field-info records to the prototype:
  // direction
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Vector3f_handle_function direction_func =
    static_cast<Vector3f_handle_function>
    (&Directional_light::direction_handle);
  s_prototype->add_field_info(new SF_vector3f(DIRECTION,
                                              "direction",
                                              RULE_EXPOSED_FIELD,
                                              direction_func,
                                              exec_func));
}

//! \brief deletes the node prototype.
void Directional_light::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */
Container_proto* Directional_light::get_prototype()
{
  if (!s_prototype) Directional_light::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Directional_light::set_attributes(Element* elem)
{
  Light::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "direction") {
      Vector3f dir(value);
      set_direction(dir);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/** get a list of attributes (called in the save process) */
Attribute_list Directional_light::get_attributes()
{
  Attribute_list attribs;
  attribs = Light::get_attributes();
  Attribue attrib;
  Vector3f dir;

  if(m_direction != m_defDirection) {
    attrib.first = "direction";
    get_direction(dir);
    attrib.second = dir.get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

SGAL_END_NAMESPACE
