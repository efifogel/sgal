// Copyright (c) 2014 Israel.
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
 * Renderers of arrangement on torus
 */

#ifndef SCGAL_ARRANGEMENT_ON_TORUS_RENDERERS_HPP
#define SCGAL_ARRANGEMENT_ON_TORUS_RENDERERS_HPP

#include <list>
#if (defined _MSC_VER)
#include <windows.h>
#endif
#include "GL/gl.h"

#include "SGAL/basic.hpp"
#include "SGAL/Torus.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Draw_action.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

/*! A function object that renders the torus surface. */
class SGAL_SCGAL_DECL Torus_renderer : public Arrangement_renderer::Renderer {
public:
  /*! Constrcutor.
   */
  Torus_renderer();

  /*! Drawing operator.
   */
  virtual void operator()(Draw_action* action);

protected:
  /*! The arrangement geometry. */
  Torus m_torus;
};

/*! A function object that renders the torus surface with color */
class SGAL_SCGAL_DECL Colored_torus_renderer : public Torus_renderer {
public:
  /*! Constructor.
   */
  Colored_torus_renderer(const Vector4f& color);

  /*! Set the color.
   * \param color (in) the new color.
   */
  void set_color(const Vector3f& color)
  {
    m_color[0] = color[0];
    m_color[1] = color[1];
    m_color[2] = color[2];
  }

  /*! Set the transparency.
   * \param transparency (in) the transparency.
   */
  void set_transparency(Float transparency)
  { m_color[3] = 1.0f - transparency; }

  /*! Drawing operator. */
  virtual void operator()(Draw_action* action);

protected:
  /*! The arrangement geometry. */
  Vector4f m_color;
};

/*! A function object that renders the torus surface only into the stencil
 * buffer.
 */
class SGAL_SCGAL_DECL Stencil_torus_renderer : public Torus_renderer {
public:
  /*! Drawing operator.
   */
  virtual void operator()(Draw_action* action);
};

/*! Draw an arrangement on torus vertex as a disc.
 * \param action (in)
 * \param center (in) the vertex center
 * \param radius (in) the vertex radius
 */
void draw_disc_vertex_on_torus(Draw_action* action, Vector2f& center,
                               Float radius, Float delta_angle);

/*! Draw an arrangement on torus vertex.
 * \param action (in).
 * \param center (in) the vertex center.
 * \param style (in) the vertex shape style.
 * \param radius (in) the vertex radius.
 * \param delta_angle (in).
 */
void draw_vertex_on_torus(Draw_action* action, Vector2f& center,
                          Arrangement_renderer::Vertex_shape::Style style,
                          Float radius, Float delta_angle);

/*! Draw an arrangement on torus edge as a strip.
 * \param action (in).
 * \param source (in) the edge source.
 * \param target (in) the edge target.
 * \param normal (in) the normal to the plane containing the edge.
 * \param radius (in) the edge radius.
 */
void draw_strip_edge_on_torus(Draw_action* action,
                              Vector2f& src, Vector2f& trg,
                              Float radius, Float delta_angle);

/*! Draw an arrangement on torus edge as a strip.
 * \param action (in).
 * \param source (in) the edge source.
 * \param target (in) the edge target.
 * \param normal (in) the normal to the plane containing the edge.
 * \param count (in) the number of parallel graphs.
 * \param directed (in) indicated whether the edge is drawn with arrows.
 * \param radius (in) the edge radius.
 */
void draw_strip_edge_on_torus(Draw_action* action,
                              Vector2f& src, Vector2f& trg,
                              Uint count, Boolean directed, Float radius,
                              Float delta_angle,
                              Float src_vertex_radius,
                              Float trg_vertex_radius);

/*! Draw an arrangement on torus edge.
 * \param action (in).
 * \param source (in) the edge source.
 * \param target (in) the edge target.
 * \param normal (in) the normal to the plane containing the edge.
 * \param shape (in) the edge shape.
 * \param radius (in) the edge radius.
 * \param delta_angle (in).
 */
void draw_edge_on_torus(Draw_action* action,
                        Vector2f& source, Vector2f& target,
                        Arrangement_renderer::Edge_shape::Style style,
                        Uint count, Boolean directed, Float radius,
                        Float delta_angle,
                        Float src_vertex_radius, Float trg_vertex_radius);

/*! Draw a triangular patch on the torus.
 * \param vec1 (in).
 * \param vec2 (in).
 * \param vec3 (in).
 * \param level (in).
 */
void draw_triangular_patch(const Vector2f& vec1, const Vector2f& vec2,
                           const Vector2f& vec3, Uint level);

/*! Draw an arrangement on torus face.
 * \param action (in).
 * \param begin (in)the begining iterator of a range of points on the boundary.
 * \param end (in) the past-the-end iterator of a range of points on the
 *        boundary.
 */
void draw_aos_convex_face(Draw_action* action,
                          std::list<Vector2f>::const_iterator begin,
                          std::list<Vector2f>::const_iterator end,
                          const Vector2f& mid);

/*! Draw an arrangement on torus face.
 * \param action (in).
 * \param begin (in)the begining iterator of a range of points on the boundary.
 * \param end (in) the past-the-end iterator of a range of points on the
 *        boundary.
 */
void draw_aos_convex_face(Draw_action* action,
                          std::list<Vector2f>::const_iterator begin,
                          std::list<Vector2f>::const_iterator end);

SGAL_END_NAMESPACE

#endif
