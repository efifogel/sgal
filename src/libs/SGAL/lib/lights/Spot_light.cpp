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

#include "SGAL/Spot_light.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto * Spot_light::s_prototype = NULL;
const std::string Spot_light::s_tag = "SpotLight";

// Default values:
Vector3f Spot_light::s_def_direction(0, 0, -1);
Vector3f Spot_light::s_def_attenuation(1, 0, 0);
Vector3f Spot_light::s_def_location(0, 0, 0);
float Spot_light::s_def_beam_width = 90;
float Spot_light::s_def_cutoff_angle = 45.0f;
float Spot_light::s_def_radius = 100;

REGISTER_TO_FACTORY(Spot_light, "Spot_light");

/*! Constructor */
Spot_light::Spot_light(Boolean proto) :
  Light(proto),
  m_attenuation(s_def_attenuation),
  m_location(s_def_location),
  m_radius(s_def_radius),
  m_direction(s_def_direction),
  m_beam_width(s_def_beam_width),
  m_cutoff_angle(s_def_cutoff_angle)
{}

/*! Destructor */
Spot_light::~Spot_light() {}

/*! sets the direction of the light */
void Spot_light::set_direction(const Vector3f & direction)
{
  m_direction = direction;
}

/*! obtains the direction of the light */
void Spot_light::get_direction(Vector3f & direction)
{
  direction = m_direction;
}

/*! sets the beam_width (iner cone angel) of the light */
void Spot_light::set_beam_width(const Float & beam_width)
{
  m_beam_width = beam_width * 180.0f / SGAL_PI;
}

/*! sets the beam_width (iner cone angle) of the light */
Float Spot_light::get_beam_width()
{
  return m_beam_width / 180.0f * SGAL_PI;
}

/*! set the cutOffAngle (outer cone angle) of the light */
void Spot_light::set_cutoff_angle(const Float & cutoff_angle)
{
  m_cutoff_angle = cutoff_angle * 180.0f / SGAL_PI;
  if (m_cutoff_angle > 90) {
    m_cutoff_angle = 90;
  }
}

/*! set the cutOffAngle (outer cone angle) of the light */
Float Spot_light::get_cutoff_angle()
{
  return m_cutoff_angle / 180.0f * SGAL_PI;
}

/*!  exponent = It's an OpenGL factor (0 - 128) determined the intensity of
 * the iner cone (beam_width angle) light. To translate the vrml attribute, we
 * considered the beam_width as follow:
 * m_beam_width = ((128-exponent)*m_cutOffAngle) / 128
 */
float Spot_light::get_exponent()
{
  float exponent = (m_cutoff_angle > m_beam_width) ?
    (128 - (m_beam_width / m_cutoff_angle) * 128.0f) : 0;
  return exponent;
}

/*! sets the attenuation factor of the light */
void Spot_light::set_attenuation(const Vector3f & attenuation)
{
  m_attenuation = attenuation;
}

/*! sets the attenuation factor of the light */
void Spot_light::get_attenuation(Vector3f & attenuation)
{
  attenuation = m_attenuation;
}

/*! sets the location of the light */
void Spot_light::set_location(const Vector3f & location)
{
  m_location = location;
}

/*! sets the location of the light */
void Spot_light::get_location(Vector3f & location)
{
  location = m_location;
}

/*! sets the radius */
void Spot_light::set_radius(const Float & radius)
{
  m_radius = radius;
}

/*! obtains the radius of the light */
Float Spot_light::get_radius()
{
  return m_radius;
}

/*! draws the light */
Action::Trav_directive Spot_light::draw(Draw_action * draw_action)
{
  if (!m_is_on) return Action::TRAV_CONT;
  
  Light::draw(draw_action);
  int lightId  = GL_LIGHT0 + m_id;

  float exponent = get_exponent();

  Vector4f dir(m_direction);
  Vector4f loc(m_location);
  loc[3] = 1.0f;

  glLightfv(lightId, GL_POSITION,        (float *) &loc);
  glLightfv(lightId, GL_SPOT_DIRECTION,  (float *) &dir);
  glLightf(lightId, GL_SPOT_CUTOFF,     (float) m_cutoff_angle);

  glLightf(lightId, GL_CONSTANT_ATTENUATION,  m_attenuation[0]);
  glLightf(lightId, GL_LINEAR_ATTENUATION,    m_attenuation[1]);
  glLightf(lightId, GL_QUADRATIC_ATTENUATION, m_attenuation[2]);

  glLightf (lightId, GL_SPOT_EXPONENT, exponent);

  return Action::TRAV_CONT;
}

/*! initializes the node prototype */
void Spot_light::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Light::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->
    add_field_info(new SF_vector3f(LOCATION, "location",
                                   get_member_offset(&m_location),
                                   exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->
    add_field_info(new SF_vector3f(DIRECTION, "direction",
                                   get_member_offset(&m_direction),
                                   exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->
    add_field_info(new SF_vector3f(ATTENUATION, "attenuation",
                                   get_member_offset(&m_attenuation),
                                   exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                               get_member_offset(&m_radius),
                                               exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_float(BEAMWIDTH, "beamwidth",
                                               get_member_offset(&m_beam_width),
                                               exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_float(CUTOFFANGLE, "cutoffangle",
                                           get_member_offset(&m_cutoff_angle),
                                           exec_func));
}

/*! Delete the node prototype */
void Spot_light::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */
Container_proto * Spot_light::get_prototype() 
{  
  if (!s_prototype) Spot_light::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file */
void Spot_light::set_attributes(Element * elem)
{  
  Light::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "direction") {
      Vector3f dir(value);
      set_direction(dir);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "beamWidth") {
      set_beam_width(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "cutOffAngle") {
      set_cutoff_angle(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "attenuation") {
      Vector3f dir(value);
      set_attenuation(dir);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "location") {
      Vector3f dir(value);
      set_location(dir);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "radius") {
      set_radius(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! get a list of attributes (called in the save process) */
Attribute_list Spot_light::get_attributes()
{ 
  Attribute_list attrs; 
  attrs = Light::get_attributes();
  Attribue attrib;
  char buf[32];
  Vector3f att;

  if (m_direction != m_defDirection) {
    attrib.first = "direction";
    get_direction(att);
    attrib.second = att.get_text();
    attrs.push_back(attrib);
  }
  if (m_beam_width != m_def_beam_width) {
    attrib.first = "beam_width";
    sprintf(buf, "%f",   get_beam_width());
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_cutOffAngle != m_defCutOffAngle) {
    attrib.first = "cutOffAngle";
    sprintf(buf, "%f",   get_cut_off_angle());
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_attenuation != m_def_attenuation) {
    attrib.first = "attenuation";
    get_attenuation(att);
    attrib.second = att.get_text();
    attrs.push_back(attrib);
  }
  if (m_location != m_def_location) {
    attrib.first = "location";
    GetLocation(att);
    attrib.second = att.get_text();
    attrs.push_back(attrib);
  }
  if (m_radius != m_defRadius) {
    attrib.first = "radius";
    sprintf(buf, "%f", get_radius());
    attrib.second = buf;
    attrs.push_back(attrib);
  }

  return attrs; 
}
#endif

SGAL_END_NAMESPACE
