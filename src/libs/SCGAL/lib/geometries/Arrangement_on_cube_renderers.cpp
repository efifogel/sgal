// Copyright (c) 2007 Israel.
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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include "GL/gl.h"

#include "SGAL/basic.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Context.hpp"

#include "SCGAL/Arrangement_on_cube_renderers.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief draws a cube */
void Cube_renderer::operator()(Draw_action * action) { m_cube.draw(action); }

/*! Constructor */
Colored_cube_renderer::Colored_cube_renderer(const Vector4f & color) :
  m_color(color) {}

/*! \brief draws a cube */
void Colored_cube_renderer::operator()(Draw_action * action)
{
  glColor4fv((float*)&m_color);
  Cube_renderer::operator()(action);
}

/*! \brief draws a cube */
void Stencil_cube_renderer::operator()(Draw_action * action)
{
  Context * context = action->get_context();
  context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
  Cube_renderer::operator()(action);
  context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));
}

/*! \brief draws an arrangement on cube vertex */
void draw_vertex_on_cube(Draw_action * action,
                         const Vector3f & center, const Vector3f & normal,
                         Arrangement_renderer::Vertex_shape::Style style,
                         Float radius, Float delta_angle)
{
  typedef Arrangement_renderer::Vertex_shape      Vertex_shape;

  switch (style) {
   case Vertex_shape::NONE: break;

   case Vertex_shape::BALL:       // draw a ball
    {
     Sphere sphere;
     sphere.set_center(center);
     sphere.set_radius(radius);
     sphere.draw(action);
    }
    break;

   case Vertex_shape::POINT:      // draw a point
    glBegin(GL_POINTS);
    glVertex3fv((float*)&center);
    glEnd();
    break;

   case Vertex_shape::DISC:
    {
     Vector3f vec;
     if (normal[0] == -1) vec.set(0, 0, radius);
     else if (normal[0] == 1) vec.set(0, radius, 0);
     else if (normal[1] == -1) vec.set(radius, 0, 0);
     else if (normal[1] == 1) vec.set(0, 0, radius);
     else if (normal[2] == -1) vec.set(0, radius, 0);
     else if (normal[2] == 1) vec.set(radius, 0, 0);

     Rotation rot(normal, 0);

     glBegin(GL_TRIANGLE_FAN);
     glNormal3fv((float*)&normal);
     glVertex3fv((float*)&center);
     float angle;
     for (angle = 0; angle < SGAL_TWO_PI; angle += delta_angle) {
       rot.set_angle(angle);
       Vector3f tmp;
       rot.rotate(vec, tmp);
       tmp.add(center);
       glVertex3fv((float*)&tmp);
     }
     vec.add(center);
     glVertex3fv((float*)&vec);
     glEnd();
    }
    break;

   case Vertex_shape::RING:
    {
     Vector3f vec;
     if (normal[0] == -1) vec.set(0, 0, radius);
     else if (normal[0] == 1) vec.set(0, radius, 0);
     else if (normal[1] == -1) vec.set(radius, 0, 0);
     else if (normal[1] == 1) vec.set(0, 0, radius);
     else if (normal[2] == -1) vec.set(0, radius, 0);
     else if (normal[2] == 1) vec.set(radius, 0, 0);

     Rotation rot(normal, 0);

     glBegin(GL_LINE_LOOP);
     float angle;
     for (angle = delta_angle; angle < SGAL_TWO_PI; angle += delta_angle) {
       rot.set_angle(angle);
       Vector3f tmp;
       rot.rotate(vec, tmp);
       tmp.add(center);
       glVertex3fv((float*)&tmp);
     }
     glEnd();
    }
    break;

   default: break;
  }
}

/*! \brief draws an arrangement on cube edge */
void draw_edge_on_cube(Draw_action * action,
                       const Vector3f & src, const Vector3f & trg,
                       const Vector3f & normal,
                       Arrangement_renderer::Edge_shape::Style style,
                       Float radius)
{
  typedef Arrangement_renderer::Edge_shape      Edge_shape;

  Vector3f dif, cross, offset, tmp;

  switch (style) {
   case Edge_shape::LINE:
    glBegin(GL_LINES);
    glVertex3fv((float*)&src);
    glVertex3fv((float*)&trg);
    glEnd();
    break;

   case Edge_shape::STRIP:
    dif.sub(trg, src);
    cross.cross(normal, dif);
    cross.normalize();
    offset.scale(radius, cross);

    glBegin(GL_POLYGON);
    glNormal3fv((float*)&normal);
    tmp.add(src, offset);
    glVertex3fv((float*)&tmp);
    tmp.sub(src, offset);
    glVertex3fv((float*)&tmp);
    tmp.sub(trg, offset);
    glVertex3fv((float*)&tmp);
    tmp.add(trg, offset);
    glVertex3fv((float*)&tmp);
    glEnd();
    break;

   case Edge_shape::TUBE:
    {
      Extrusion tube;
      tube.set_cross_section_radius(radius);
      std::vector<Vector3f>& spine = tube.get_spine();
      spine.resize(2);
      spine[0] = src;
      spine[1] = trg;
      tube.draw(action);
    }
    break;

   default: break;
  }
}

SGAL_END_NAMESPACE
