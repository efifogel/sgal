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

#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <algorithm>

#include "SGAL/Light.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Light::s_prototype(nullptr);

//! defaults values for Light's members.
const Boolean Light::m_def_is_on(true);
const Float Light::m_def_intensity(1);
const Float Light::m_def_ambient_intensity(1);
const Vector3f Light::m_def_color(1, 1, 1);

//! \brief constructor
Light::Light(Boolean proto) :
  Node(proto),
  m_id(-1),
  m_is_on(m_def_is_on),
  m_intensity(m_def_intensity),
  m_ambient_intensity(m_def_ambient_intensity),
  m_ref(0)
{ m_color = Vector3f(m_def_color); }

//! \brief distructor
Light::~Light() {}

//! \brief turns the light on and off.
void Light::set_on(Boolean is_on) { m_is_on = is_on; }

//! \brief determines whether the light is on.
Boolean Light::get_on() { return m_is_on; }

//! \brief sets the light intensity.
void Light::set_intensity(Float intensity) { m_intensity = intensity; }

//! \brief obtains the light intensity.
Float Light::get_intensity() { return m_intensity; }

//! \brief sets the ambient intensity of the light.
void Light::set_ambient_intensity(Float ambient_intensity)
{ m_ambient_intensity = ambient_intensity; }

//! \brief obtains the ambient intensity of the light.
Float Light::get_ambient_intensity() { return m_ambient_intensity; }

/*! \brief sets the light color. It is used for diffuse and specular components.
 */
void Light::set_color(const Vector3f& color) { m_color = color; }

//! \brief obtains the light color.
void Light::get_color(Vector3f& color) { color = m_color; }

//! \brief initializes the node prototype.
void Light::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  // on
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function is_on_func =
    static_cast<Boolean_handle_function>(&Light::is_on_handle);
  s_prototype->add_field_info(new SF_bool(ON, "on",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_on_func, exec_func));

  // color
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Vector3f_handle_function color_func =
    static_cast<Vector3f_handle_function>(&Light::color_handle);
  s_prototype->add_field_info(new SF_vector3f(COLOR, "color",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              color_func,
                                              exec_func));

  // intensity
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function intensity_func =
    static_cast<Float_handle_function>(&Light::intensity_handle);
  s_prototype->add_field_info(new SF_float(INTENSITY, "intensity",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           intensity_func, exec_func));

  // ambientIntensity
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function ambient_intensity_func =
    static_cast<Float_handle_function>(&Light::ambient_intensity_handle);
  s_prototype->add_field_info(new SF_float(AMBIENT_INTENSITY,
                                           "ambientIntensity",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           ambient_intensity_func,
                                           exec_func));
}

//! \brief deletes the node prototype.
void Light::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Light::get_prototype()
{
  if (!s_prototype) Light::init_prototype();
  return s_prototype;
}

//! \brief
void Light::cull(Cull_context& cull_context)
{ cull_context.add_light(this); }

//! \brief draws the light.
Action::Trav_directive Light::draw(Draw_action* da)
{
  if ((da == 0) || (da->get_context() == 0)) return Action::TRAV_STOP;

  Int32 already_defined;
  Matrix4f mat;
  m_id = da->get_context()->get_light_target(this, mat, already_defined);
  if (m_id == already_defined) return Action::TRAV_CONT;

  Int32 light_id  = GL_LIGHT0 + m_id;

  Vector3f ambient;
  ambient.scale(m_ambient_intensity, m_color);
  glLightfv(light_id, GL_AMBIENT, (float *) &ambient);
  Vector3f diffuse;
  diffuse.scale(m_intensity, m_color);
  glLightfv(light_id, GL_DIFFUSE, (float *) &diffuse);
  glLightfv(light_id, GL_SPECULAR, (float *) &diffuse);
  return Action::TRAV_CONT;
}

//! \brief sets the attributes of this object.
void Light::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
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
      set_intensity(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "ambientIntensity") {
      set_ambient_intensity(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Light::add_to_scene(Scene_graph* sg)
{ sg->set_have_lights(true); }

#if 0
//! get a list of attributes (called in the save process).
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

//! \brief adds the light node to the scene.
void Light::add_to_scene(Scene_graph* sg)
{
  Scene_config* scene_config = sg->get_scene_config();

  if (scene_config &&
      parent == (XML_entity*)sg->get_navigation_root() &&
      scene_config->AreGlobalLightsStationary())
  {
    parent = (XML_entity*)sg->get_root();
  }
  Node::add_to_scene(sg, parent);

  Group* group = dynamic_cast<Group *>(parent);
  if (group) group->SetHasLight();
}
#endif

SGAL_END_NAMESPACE
