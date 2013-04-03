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
// $Id: Arrangement_on_sphere_renderers.cpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146)
#pragma warning( disable : 4244)
#pragma warning( disable : 4390)
#pragma warning( disable : 4503)
#pragma warning( disable : 4800)
#pragma warning( disable : 4996)
#endif

/*! \file
 * Renderers of arrangement on sphere
 */

#include <math.h>
#if (defined _MSC_VER)
#include <windows.h>
#endif
#include "GL/gl.h"

#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Extrusion.hpp"

#include "SGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Sphere_renderer::Sphere_renderer()
{
  m_sphere.set_stacks(64);
  m_sphere.set_slices(64);
}

/*! \brief draws the sphere */
void Sphere_renderer::operator()(Draw_action* action)
{ m_sphere.draw(action); }

/*! Constructor */
Colored_sphere_renderer::
Colored_sphere_renderer(const Vector4f& color) : m_color(color) {}

/*! \brief draws the sphere */
void Colored_sphere_renderer::operator()(Draw_action* action)
{
  glColor4fv((float*)&m_color);
  Sphere_renderer::operator()(action);
}

/*! \brief draws the sphere */
void Stencil_sphere_renderer::operator()(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
  Sphere_renderer::operator()(action);
  context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));
}

/*! \brief draws an arrangement on sphere vertex as a disc */
void draw_disc_vertex_on_sphere(Draw_action* action, Vector3f& center,
                                Float radius, Float delta_angle)
{
  glBegin(GL_TRIANGLE_FAN);
  glNormal3fv((float*)&center);
  glVertex3fv((float*)&center);

  // Find a vertex on the perimeter fan:
  Vector3f vec;
  if ((abs(center[0]) <= SGAL_EPSILON) && (abs(center[1]) <= SGAL_EPSILON))
    vec.set(0, -center[2], center[1]);
  else vec.set(-center[1], center[0], 0);
  vec.normalize();
  vec.scale(radius);
  vec.add(center);
  vec.normalize();
      
  glVertex3fv((float*)&vec);
  Rotation rot(center, 0);
  float angle;
  for (angle = delta_angle; angle < SGAL_TWO_PI; angle += delta_angle) {
    Vector3f tmp;
    rot.set_angle(angle);
    rot.rotate(vec, tmp);
    glVertex3fv((float*)&tmp);
  }
  glVertex3fv((float*)&vec);
  glEnd();
}

/*! \brief draws an arrangement on sphere vertex */
void draw_vertex_on_sphere(Draw_action* action, Vector3f& center,
                           Arrangement_renderer::Vertex_shape::Style style,
                           Float radius, Float delta_angle)
{
  typedef Arrangement_renderer::Vertex_shape    Vertex_shape;
  
  switch (style) {
   case Vertex_shape::NONE: break;
    
   case Vertex_shape::BALL:                 // draw a ball
    {
     Sphere sphere;
     sphere.set_center(center);
     sphere.set_radius(radius);
     sphere.draw(action);
    }
    break;
    
   case Vertex_shape::POINT:                // draw a point
    glBegin(GL_POINTS);
    glVertex3fv((float*)&center);
    glEnd();
    break;

   case Vertex_shape::DISC:
    draw_disc_vertex_on_sphere(action, center, radius, delta_angle);      
    break;

   case Vertex_shape::RING:
    {
     glBegin(GL_LINE_LOOP);

     Vector3f vec;
     if ((abs(center[0]) <= SGAL_EPSILON) && (abs(center[1]) <= SGAL_EPSILON))
       vec.set(0, -center[2], center[1]);
     else vec.set(-center[1], center[0], 0);
     vec.normalize();
     vec.scale(radius);
     vec.add(center);

     vec.normalize();
     Rotation rot(center, 0);
     float angle;
     for (angle = delta_angle; angle < SGAL_TWO_PI; angle += delta_angle) {
       Vector3f tmp;
       rot.set_angle(angle);
       rot.rotate(vec, tmp);
       glVertex3fv((float*)&tmp);
     }
     // glVertex3fv((float*)&vec);
     glEnd();
    }
    break;
    
   default: break;
  }
}

/*! \brief draws an arrangement on sphere edge as a strip */
void draw_strip_edge_on_sphere(Draw_action* action,
                               Vector3f& source, Vector3f& target,
                               Vector3f& normal,
                               Float radius, Float delta_angle)
{
  Rotation rot;
  Vector3f diff;
  diff.add(target, source);
  if ((abs(diff[0]) < SGAL_EPSILON) && (abs(diff[1]) < SGAL_EPSILON) &&
      (abs(diff[2]) < SGAL_EPSILON))
    rot.set(normal, SGAL_PI);
  else
    rot.make(source, target);
  float angle = rot.get_angle();
  
  glBegin(GL_QUAD_STRIP);
  Vector3f vec1 = source;
  Vector3f dif;
  for (float tmp_angle = delta_angle; tmp_angle < angle;
       tmp_angle += delta_angle)
  {
    rot.set_angle(tmp_angle);
    Vector3f vec2;
    rot.rotate(source, vec2);
    dif.sub(vec2, vec1);
    Vector3f vec3;
    vec3.cross(vec1, dif);
    vec3.normalize();
    
    Vector3f c1;
    c1.scale(radius, vec3);
    c1.add(vec1);
    Vector3f c2;
    c2.scale(-radius, vec3);
    c2.add(vec1);
    glNormal3fv((float*)&vec1);
    glVertex3fv((float*)&c1);
    glVertex3fv((float*)&c2);
    vec1 = vec2;
  }
  dif.sub(target, vec1);
  Vector3f vec3;
  vec3.cross(vec1, dif);
  vec3.normalize();
  Vector3f c1;
  c1.scale(radius, vec3);
  c1.add(vec1);
  Vector3f c2;
  c2.scale(-radius, vec3);
  c2.add(vec1);
  glNormal3fv((float*)&vec1);
  glVertex3fv((float*)&c1);
  glVertex3fv((float*)&c2);

  c1.scale(radius, vec3);
  c1.add(target);
  c2.scale(-radius, vec3);
  c2.add(target);
  glNormal3fv((float*)&target);
  glVertex3fv((float*)&c1);
  glVertex3fv((float*)&c2);
  
  glEnd();
}

/*! \brief draws an arrangement on sphere edge */
void draw_edge_on_sphere(Draw_action* action,
                         Vector3f& source, Vector3f& target, Vector3f& normal,
                         Arrangement_renderer::Edge_shape::Style style,
                         Uint count, Boolean directed, Float radius,
                         Float delta_angle,
                         Float source_vertex_radius, Float target_vertex_radius)
{
  typedef Arrangement_renderer::Edge_shape              Edge_shape;
  
  Rotation rot;
  Vector3f diff;
  diff.add(target, source);
  if ((abs(diff[0]) < SGAL_EPSILON) && (abs(diff[1]) < SGAL_EPSILON) &&
      (abs(diff[2]) < SGAL_EPSILON))
    rot.set(normal, SGAL_PI);
  else
    rot.make(source, target);
  float angle = rot.get_angle();

  switch (style) {
   case Edge_shape::LINE:
    glBegin(GL_LINE_STRIP);
    glVertex3fv((float*)&source);
    for (float tmp_angle = delta_angle; tmp_angle < angle;
         tmp_angle += delta_angle)
    {
      rot.set_angle(tmp_angle);
      Vector3f tmp;
      rot.rotate(source, tmp);
      glVertex3fv((float*)&tmp);
    }
    glVertex3fv((float*)&target);
    glEnd();
    break;

   case Edge_shape::STRIP:
    draw_strip_edge_on_sphere(action, source, target, normal,
                              count, directed, radius, delta_angle,
                              source_vertex_radius, target_vertex_radius);
    break;
      
   case Edge_shape::TUBE:
    {
     Extrusion tube;
     tube.set_cross_section_radius(radius);
     SGAL::Array<Vector3f>& spine = tube.get_spine();
     spine.resize(static_cast<Uint>(ceil(angle / delta_angle)) + 1);
     unsigned int i = 0;
     spine[i++] = source;
     for (float tmp_angle = delta_angle; tmp_angle < angle;
          tmp_angle += delta_angle)
     {
       rot.set_angle(tmp_angle);
       Vector3f tmp;
       rot.rotate(source, tmp);
       spine[i++] = tmp;
     }
     spine[i] = target;
     tube.draw(action);
    }
    break;
    
   default: break;
  }
}

/*! \brief draws an arrangement on sphere edge */
void draw_strip_edge_on_sphere(Draw_action* action,
                               Vector3f& source, Vector3f& target,
                               Vector3f& normal,
                               Uint count, Boolean directed, Float radius,
                               Float delta_angle,
                               Float source_vertex_radius,
                               Float target_vertex_radius)
{
  Vector3f dif, vec, start_vec, start_source, start_target;
  dif.sub(target, source);
  dif.normalize();
  vec.cross(source, dif);
  vec.normalize();
 
  Float new_radius = radius / (2 * count - 1);
  start_vec.scale(new_radius * 2 * (1 - count), vec);
  start_source.add(source, start_vec);
  start_target.add(target, start_vec);

  Vector3f new_vec, new_source, new_target;
  unsigned int i;
  for (i = 0; i < count; ++i) {
    new_vec.scale(i * 4 * new_radius, vec);
    new_source.add(start_source, new_vec);
    new_target.add(start_target, new_vec);
    draw_strip_edge_on_sphere(action, new_source, new_target, normal,
                              new_radius, delta_angle);
  }

  if (directed) {
    glBegin(GL_TRIANGLES);
    Vector3f a, b, c;
    Vector3f hor_start_source, hor_source, start_ver_source, ver_source;
    hor_start_source.scale(-radius, vec);
    hor_source.scale(-radius * 3, vec);
    start_ver_source.scale(source_vertex_radius, dif);
    ver_source.scale(radius * 6, dif);
    a.add(source, hor_start_source);
    a.add(start_ver_source);
    a.normalize();
    b.add(a, ver_source);
    b.normalize();
    c.add(b, hor_source);
    c.normalize();
    draw_triangular_patch(a, c, b, 4);

    Vector3f hor_start_target, hor_target, start_ver_target, ver_target;
    hor_start_target.scale(radius, vec);
    hor_target.scale(radius * 3, vec);
    start_ver_target.scale(-target_vertex_radius, dif);
    ver_target.scale(radius * -6, dif);
    a.add(target, hor_start_target);
    a.add(start_ver_target);
    a.normalize();
    b.add(a, ver_target);
    b.normalize();
    c.add(b, hor_target);
    c.normalize();
    draw_triangular_patch(a, c, b, 4);
    glEnd();
  }
}

void draw_triangular_patch(const Vector3f& vec1, const Vector3f& vec2,
                           const Vector3f& vec3, Uint level)
{
  if (level == 0) {
    glNormal3fv((float*)&vec1);
    glVertex3fv((float*)&vec1);
    glNormal3fv((float*)&vec2);
    glVertex3fv((float*)&vec2);
    glNormal3fv((float*)&vec3);
    glVertex3fv((float*)&vec3);
    return;
  }

  Vector3f vec12;
  vec12.add(vec1, vec2);
  vec12.scale(0.5f);
  vec12.normalize();

  Vector3f vec23;
  vec23.add(vec2, vec3);
  vec23.scale(0.5f);
  vec23.normalize();

  Vector3f vec31;
  vec31.add(vec3, vec1);
  vec31.scale(0.5f);
  vec31.normalize();

  --level;
  draw_triangular_patch(vec1, vec12, vec31, level);
  draw_triangular_patch(vec12, vec2, vec23, level);
  draw_triangular_patch(vec23, vec3, vec31, level);
  draw_triangular_patch(vec12, vec23, vec31, level);
}

/*! \brief draws an arrangement on sphere face */
void draw_aos_convex_face(Draw_action* action,
                   std::list<Vector3f>::const_iterator begin,
                   std::list<Vector3f>::const_iterator end,
                   const Vector3f& mid)
{
  glBegin(GL_TRIANGLES);
  std::list<Vector3f>::const_iterator it1 = begin;
  std::list<Vector3f>::const_iterator next = it1;
  while (it1 != end) {
    ++next;
    std::list<Vector3f>::const_iterator it2 = (next == end) ? begin : next;
    const Vector3f& vec1 = (*it1);
    const Vector3f& vec2 = (*it2);
    draw_triangular_patch(vec1, vec2, mid, 4);
    it1 = next;
  }
  glEnd();
}

/*! \brief draws an arrangement on sphere face */
void draw_aos_convex_face(Draw_action* action,
                   std::list<Vector3f>::const_iterator begin,
                   std::list<Vector3f>::const_iterator end)

{
  // Compute the mid point:
  Vector3f mid;
  std::list<Vector3f>::const_iterator it;
  Vector3f v0, v1, d0, normal;
  for (it = begin; it != end; ++it) {
    const Vector3f& vec = (*it);
    mid.add(vec);
  }
  SGAL_assertion(std::distance(begin, end) != 0);
  
  mid.scale(1.0f / std::distance(begin, end));
  mid.normalize();
  draw_aos_convex_face(action, begin, end, mid);
}

SGAL_END_NAMESPACE
