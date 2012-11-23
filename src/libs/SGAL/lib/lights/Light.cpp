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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <algorithm>

#include "SGAL/Light.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto * Light::s_prototype = NULL;

/*! defaults values for Light's members.*/
const Boolean Light::m_def_is_on(SGAL_TRUE);
const Float Light::m_def_intensity(1);
const Float Light::m_def_ambient_intensity(1);
const Vector3f Light::m_def_color(1, 1, 1);

/*! constructor */
Light::Light(Boolean proto) :
  Node(proto), 
  m_id(-1),
  m_is_on(m_def_is_on),
  m_intensity(m_def_intensity),
  m_ambient_intensity(m_def_ambient_intensity),
  m_ref(0)
{
  m_color = Vector3f(m_def_color);
}

/*! distructor */
Light::~Light() {}

/*! turn the light on/off */
void Light::set_on(Boolean is_on) 
{ 
  m_is_on = is_on;
} 

/*! return the status of the light */
Boolean Light::get_on() 
{ 
  return m_is_on; 
}

/*! set the light's intensity */
void Light::set_intensity(Float intensity)
{
  m_intensity = intensity;
}

/*! get the light's intensity */
Float Light::get_intensity()
{
  return m_intensity;
}

/*! set the ambient intensity of the light */
void Light::set_ambient_intensity(Float ambient_intensity)
{
  m_ambient_intensity = ambient_intensity;
}

/*! get the ambient intensity of the light */
Float Light::get_ambient_intensity()
{
  return m_ambient_intensity;
}

/*! set the light's color. It is used for diffuse and specular components */
void Light::set_color(const Vector3f & color)
{
  m_color = color;
}

/*! get the light's color */
void Light::get_color(Vector3f & color)
{
  color = m_color;
}

/*! initializes the node prototype */
void Light::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_bool(ON, "on",
                                          get_member_offset(&m_is_on),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_vector3f(COLOR, "color",
                                              get_member_offset(&m_color),
                                              exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_float(INTENSITY, "intensity",
                                           get_member_offset(&m_intensity),
                                           exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->
    add_field_info(new SF_float(AMBIENT_INTENSITY,"ambient_intensity",
                                get_member_offset(&m_ambient_intensity),
                                exec_func));
}

/*! Delete the node prototype */
void Light::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */
Container_proto * Light::get_prototype() 
{  
  if (!s_prototype) Light::init_prototype();
  return s_prototype;
}

/*!
 */
void Light::cull(Cull_context & cull_context)
{
  cull_context.add_light(this);
}

/*! draw the light */
Action::Trav_directive Light::draw(Draw_action * da) 
{
  if (da == 0 || da->get_context() == 0) return Action::TRAV_STOP;

  int already_defined;
  Matrix4f mat;
  m_id = da->get_context()->get_light_target(this, mat, already_defined);
  if (m_id == already_defined) return Action::TRAV_CONT;
  
  int light_id  = GL_LIGHT0 + m_id;

  Vector3f ambient;
  ambient.scale(m_ambient_intensity, m_color);
  glLightfv(light_id, GL_AMBIENT, (float *) &ambient);
  Vector3f diffuse;
  diffuse.scale(m_intensity, m_color);
  glLightfv(light_id, GL_DIFFUSE, (float *) &diffuse);
  glLightfv(light_id, GL_SPECULAR, (float *) &diffuse);
  return Action::TRAV_CONT;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Light::set_attributes(Element * elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "on") {
      set_on(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "color") {
      Vector3f col(value);
      set_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "intensity") {
      set_intensity(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "ambientIntensity") {
      set_ambient_intensity(atoff(value.c_str()));
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */  
void Light::add_to_scene(Scene_graph * sg)
{
  sg->set_have_lights(SGAL_TRUE);
}

#if 0
/*! get a list of attributes (called in the save process) */
Attribute_list Light::get_attributes()
{ 
  Attribute_list attrs; 
  Attribue attrib;
  char buf[32];
  Vector3f col;

  attrs = Node::get_attributes();

  if (m_is_on != m_def_is_on) {
    attrib.first = "on";
    std::string val = get_on() ? "TRUE" : "FALSE";
    attrib.second = val;
    attrs.push_back(attrib);
  }
  if (m_color != m_def_color) {
    attrib.first = "color";
    get_color(col);
    attrib.second = col.get_text();
    attrs.push_back(attrib);
  }
  if (m_intensity != m_defIntensity) {
    attrib.first = "intensity";
    sprintf(buf, "%g",   get_intensity());
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if(m_ambient_intensity != m_defAmbientIntensity) {
    attrib.first = "ambient_intensity";
    sprintf(buf, "%g",   get_ambient_intensity());
    attrib.second = buf;
    attrs.push_back(attrib);
  }

  return attrs; 
}

/*! Add a child object to the scene 
 * @param sg (in) a reference to the scene graph
 * @param parentName (in) the name of the parent object.
 */
void Light::add_to_scene(Scene_graph * sg, XML_entity * parent) 
{ 
  Scene_config * scene_config = sg->get_scene_config();

  if (scene_config && 
      parent == (XML_entity*)sg->get_navigation_root() && 
      scene_config->AreGlobalLightsStationary())
  {
    parent = (XML_entity*)sg->get_root();
  }  
  Node::add_to_scene(sg, parent);

  Group * group = dynamic_cast<Group *>(parent);
  if (group) group->SetHasLight();
}
#endif

SGAL_END_NAMESPACE
