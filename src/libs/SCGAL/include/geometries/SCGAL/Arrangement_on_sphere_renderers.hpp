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

/*! \file
 * Renderers of arrangement on sphere
 */

#ifndef SCGAL_ARRANGEMENT_ON_SPHERE_RENDERERS_HPP
#define SCGAL_ARRANGEMENT_ON_SPHERE_RENDERERS_HPP

#include <list>
#if (defined _MSC_VER)
#include <windows.h>
#endif
#include "GL/gl.h"

#include "SGAL/basic.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Draw_action.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

/*! A function object that renders the sphere surface */
class SGAL_SCGAL_DECL Sphere_renderer : public Arrangement_renderer::Renderer {
public:
  /*! Constrcutor */
  Sphere_renderer();

  /*! Drawer operator */
  virtual void operator()(Draw_action* action);

protected:
  /*! The arrangement geometry */
  Sphere m_sphere;
};

/*! A function object that renders the sphere surface with color */
class SGAL_SCGAL_DECL Colored_sphere_renderer : public Sphere_renderer {
public:
  /*! Constructor */
  Colored_sphere_renderer(const Vector4f& color);

  /*! Set the color
   * \param color the new color
   */
  void set_color(const Vector3f& color)
  {
    m_color[0] = color[0];
    m_color[1] = color[1];
    m_color[2] = color[2];
  }

  /*! Set the transparency
   * \param transparency the transparency
   */
  void set_transparency(Float transparency)
  { m_color[3] = 1.0f - transparency; }

  /*! Drawer operator */
  virtual void operator()(Draw_action* action);

protected:
  /*! The arrangement geometry */
  Vector4f m_color;
};

/*! A function object that renders the sphere surface only into the stencil
 * buffer
 */
class SGAL_SCGAL_DECL Stencil_sphere_renderer : public Sphere_renderer {
public:
  /*! Drawer operator */
  virtual void operator()(Draw_action* action);
};

/*! Draw an arrangement on sphere vertex as a disc
 * \param action
 * \param center the vertex center
 * \param radius the vertex radius
 */
void draw_disc_vertex_on_sphere(Draw_action* action, Vector3f& center,
                                Float radius, Float delta_angle);

/*! Draw an arrangement on sphere vertex
 * \param action
 * \param center the vertex center
 * \param style the vertex shape style
 * \param radius the vertex radius
 * \param delta_angle
 */
void draw_vertex_on_sphere(Draw_action* action, Vector3f& center,
                           Arrangement_renderer::Vertex_shape::Style style,
                           Float radius, Float delta_angle);

/*! Draw an arrangement on sphere edge as a strip
 * \param action
 * \param source the edge source
 * \param target the edge target
 * \param normal the normal to the plane containing the edge
 * \param radius the edge radius
 */
void draw_strip_edge_on_sphere(Draw_action* action,
                               Vector3f& src, Vector3f& trg, Vector3f& normal,
                               Float radius, Float delta_angle);

/*! Draw an arrangement on sphere edge as a strip
 * \param action
 * \param source the edge source
 * \param target the edge target
 * \param normal the normal to the plane containing the edge
 * \param count the number of parallel graphs
 * \param directed indicated whether the edge is drawn with arrows
 * \param radius the edge radius
 */
void draw_strip_edge_on_sphere(Draw_action* action,
                               Vector3f& src, Vector3f& trg, Vector3f& normal,
                               Uint count, Boolean directed, Float radius,
                               Float delta_angle,
                               Float src_vertex_radius,
                               Float trg_vertex_radius);

/*! Draw an arrangement on sphere edge
 * \param action
 * \param source the edge source
 * \param target the edge target
 * \param normal the normal to the plane containing the edge
 * \param shape the edge shape
 * \param radius the edge radius
 * \param delta_angle
 */
void draw_edge_on_sphere(Draw_action* action,
                         Vector3f& source, Vector3f& target, Vector3f& normal,
                         Arrangement_renderer::Edge_shape::Style style,
                         Uint count, Boolean directed, Float radius,
                         Float delta_angle,
                         Float src_vertex_radius, Float trg_vertex_radius);

/*! Draw a triangular patch on the sphere
 * \param vec1
 * \param vec2
 * \param vec3
 * \param level
 */
void draw_triangular_patch(const Vector3f& vec1, const Vector3f& vec2,
                           const Vector3f& vec3, Uint level);

/*! Draw an arrangement on sphere face
 * \param action
 * \param begin the begining iterator of a range of points on the boundary.
 * \param end the past-beyond iterator of a range of points on the boundary.
 */
void draw_aos_convex_face(Draw_action* action,
                          std::list<Vector3f>::const_iterator begin,
                          std::list<Vector3f>::const_iterator end,
                          const Vector3f& mid);

/*! Draw an arrangement on sphere face
 * \param action
 * \param begin the begining iterator of a range of points on the boundary.
 * \param end the past-beyond iterator of a range of points on the boundary.
 */
void draw_aos_convex_face(Draw_action* action,
                          std::list<Vector3f>::const_iterator begin,
                          std::list<Vector3f>::const_iterator end);

SGAL_END_NAMESPACE

#endif
