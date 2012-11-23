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
// $Id: Arrangement_on_cube_renderers.hpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Renderers of arrangement parts
 */

#ifndef SGAL_ARRANGEMENT_ON_CUBE_RENDERERS_HPP
#define SGAL_ARRANGEMENT_ON_CUBE_RENDERERS_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Box.hpp"
#include "SGAL/Vector4f.hpp"

#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;

/*! A function object that renders the cube surface */
class Cube_renderer : public Arrangement_renderer::Renderer {
public:
  /*! Constrcutor */
  Cube_renderer() {}
    
  /*! Drawer operator */
  virtual void operator()(Draw_action * action);

protected:
  /*! The arrangement geometry */
  Box m_cube;
};

/*! A function object that renders the cube surface with color */
class Colored_cube_renderer : public Cube_renderer {
public:
  /*! Constructor */
  Colored_cube_renderer(const Vector4f & color);
    
  /*! Set the color
   * \param color the new color
   */
  void set_color(const Vector3f & color)
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
  virtual void operator()(Draw_action * action);

protected:
  /*! The surface color */
  Vector4f m_color;
};

/*! A function object that renders the cube surface only into the stencil
 * buffer
 */
class Stencil_cube_renderer : public Cube_renderer {
public:
  /*! Drawer operator */
  virtual void operator()(Draw_action * action);
};

/*! Draw an arrangement on cube vertex
 * \param action
 * \param center
 * \param normal
 * \param shape
 * \param radius
 */
void draw_vertex_on_cube(Draw_action * action,
                         const Vector3f & center, const Vector3f & normal,
                         Arrangement_renderer::Vertex_shape::Style style,
                         Float radius, Float delta_angle);

/*! Draw an arrangement on cube edge
 * \param action
 * \param source
 * \param target
 * \param normal
 * \param shape
 * \param radius
 */
void draw_edge_on_cube(Draw_action * action,
                       const Vector3f & source, const Vector3f & target,
                       const Vector3f & normal,
                       Arrangement_renderer::Edge_shape::Style style,
                       Float radius);

SGAL_END_NAMESPACE

#endif
