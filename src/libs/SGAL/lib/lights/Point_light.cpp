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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Point_light.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

std::string Point_light::s_tag = "PointLight";

REGISTER_TO_FACTORY(Point_light, "Point_light");

/*! Constructor */
Point_light::Point_light(Boolean proto) : Spot_light(proto) {}

/*! Destructor */
Point_light::~Point_light() {}

/*! Draws the light */
Action::Trav_directive Point_light::draw(Draw_action* draw_action)
{
  if (!m_is_on) return Action::TRAV_CONT;

  Light::draw(draw_action);
  int lightId = GL_LIGHT0 + m_id;

  Vector4f loc(m_location);
  loc[3] = 1.0f;

  glLightfv(lightId, GL_POSITION, (float *)&loc);
  glLightf(lightId, GL_SPOT_CUTOFF, 180.0f);

  glLightf(lightId, GL_CONSTANT_ATTENUATION,  m_attenuation[0]);
  glLightf(lightId, GL_LINEAR_ATTENUATION,    m_attenuation[1]);
  glLightf(lightId, GL_QUADRATIC_ATTENUATION, m_attenuation[2]);

  return Action::TRAV_CONT;
}

/*! \brief sets the attributes of this container. */
void Point_light::set_attributes(Element* elem)
{
  Spot_light::set_attributes(elem);
}

#if 0
/*! obtains a list of attributes (called in the save process) */
Attribute_list Point_light::get_attributes() {

  Attribute_list attribs;
  attribs = Spot_light::get_attributes();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
