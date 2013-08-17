// Copyright (c) 2004  Israel.
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
// $Source: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <GL/gl.h>
#include <GL/glu.h>

#include "Planar_map_geo.h"
#include "Draw_action.h"
#include "Container_factory.h"
#include "Element.h"

SGAL_BEGIN_NAMESPACE

typedef Element::Str_attr_iter          Str_attr_iter;
typedef Element::Cont_attr_iter         Cont_attr_iter;

REGISTER_TO_FACTORY(Planar_map_geo, "Planar_map_geo");

/*! Constructor */
Planar_map_geo::Planar_map_geo()
{
  m_tag = "sgalPlanarMapSet";
}

/*! Destructor */
Planar_map_geo::~Planar_map_geo() {}

/*! */
void Planar_map_geo::cull(Cull_context & cull_context) {}

/*! draw() draws the planar map
 * \param action action.
 */
void Planar_map_geo::draw(Draw_action * draw_action)
{
  Context * context = draw_action->get_context();
  if (!context) return;

  int pass_no = draw_action->get_pass_no();
  if (pass_no == 0) {
    draw_action->set_second_pass_required(true);
    return;
  }

  Int vx, vy, vw, vh;
  context->get_viewport(vx, vy, vw, vh);

  GLdouble model_matrix[16];
  GLdouble proj_matrix[16];
  GLint viewport[4];
  GLdouble px,py,pz;
  viewport[0] = vx;
  viewport[1] = vy;
  viewport[2] = vw;
  viewport[3] = vh;
  
  glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
  glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);

  // Project vector v(0,0,0) to px,py,pz.
  gluProject(0, 0, 0, model_matrix, proj_matrix, viewport, &px, &py, &pz);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  GLboolean bCull = glIsEnabled(GL_CULL_FACE);
  if (bCull) glDisable(GL_CULL_FACE);

  glOrtho(vx, vx + vw, vy, vy + vh, -1, 1);

  // Draw the planar map:
  
  if (bCull) glEnable(GL_CULL_FACE);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  return;
}

/*! */
void Planar_map_geo::isect(Isect_action * action)
{
}

/*! */
Boolean Planar_map_geo::calculate_sphere_bound()
{
  return false;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Planar_map_geo::set_attributes(Element * elem)
{
  Geometry::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "coordIndex") {
      elem->mark_delete(ai);
    }
  }

  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "coord") {
      elem->mark_delete(cai);      
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
