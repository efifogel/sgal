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
// $Id: Arrangement_renderers.hpp 7774 2009-07-08 12:42:37Z efif $
// $Revision: 7774 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Renderers of arrangement parts
 */

#ifndef SCGAL_ARRANGEMENT_RENDERERS_HPP
#define SCGAL_ARRANGEMENT_RENDERERS_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include "GL/gl.h"

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Draw_action.hpp"

#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

/*! A function object that renders the edges. */
template <typename T_Geometry>
class Edges_renderer : public Arrangement_renderer::Renderer {
public:
  typedef T_Geometry                                    Geometry;

  /*! Constructor. */
  Edges_renderer(Geometry& geo) : m_geo(geo) {}

  /*! Render the edges. */
  virtual void operator()(Draw_action* action)
  { m_geo.draw_aos_edges(action); }

protected:
  /*! The arrangement geometry. */
  Geometry& m_geo;

private:
  // Assignment operator cannot be generated.
  Edges_renderer& operator=(const Edges_renderer&);

  // In C++11, the following is supported:
  // Edges_renderer& operator=(const Edges_renderer&) = delete;
};

/*! A function object that renders the vertices. */
template <typename T_Geometry>
class Vertices_renderer : public Arrangement_renderer::Renderer {
public:
  typedef T_Geometry                                    Geometry;

  /*! Constructor. */
  Vertices_renderer(Geometry& geo) : m_geo(geo) {}

  /*! Render the vertices. */
  virtual void operator()(Draw_action* action)
  { m_geo.draw_aos_vertices(action); }

protected:
  /*! The arrangement geometry. */
  Geometry& m_geo;

private:
  // Assignment operator cannot be generated.
  Vertices_renderer& operator=(const Vertices_renderer&);

  // In C++11, the following is supported:
  // Vertices_renderer& operator=(const Vertices_renderer&) = delete;
};

/*! A function object that renders the isolated vertices. */
template <typename T_Geometry>
class Isolated_vertices_renderer :
  public Arrangement_renderer::Renderer
{
public:
  typedef T_Geometry                                    Geometry;

  /*! Constructor */
  Isolated_vertices_renderer(Geometry& geo) : m_geo(geo) {}

  /*! Render the isolated vertices */
  virtual void operator()(Draw_action* action)
  { m_geo.draw_aos_isolated_vertices(action); }

protected:
  /*! The arrangement geometry */
  Geometry& m_geo;
};

/*! A function object that renders the edges with color. */
template <typename Edges_renderer>
class Colored_edges_renderer : public Edges_renderer {
public:
  typedef typename Edges_renderer::Geometry          Geometry;

  /*! Constructor. */
  Colored_edges_renderer(Geometry& geo) : Edges_renderer(geo) {}

  /*! Render the edges. */
  virtual void operator()(Draw_action* action)
  {
    const Vector3f& color = this->m_geo.get_aos_edge_color();
    glColor3fv((float*)&color);
    Edges_renderer::operator()(action);
  }
};

/*! A function object that renders the vertices with color */
template <typename Vertices_renderer>
class Colored_vertices_renderer : public Vertices_renderer {
public:
  typedef typename Vertices_renderer::Geometry       Geometry;

  /*! Constructor */
  Colored_vertices_renderer(Geometry& geo) : Vertices_renderer(geo) {}

  /*! Render the vertices. */
  virtual void operator()(Draw_action* action)
  {
    const Vector3f& color = this->m_geo.get_aos_vertex_color();
    glColor3fv((float*)&color);
    Vertices_renderer::operator()(action);
  }
};

/*! A function object that renders the isolated vertices with color. */
template <typename Isolated_vertices_renderer>
class Colored_isolated_vertices_renderer : public Isolated_vertices_renderer {
public:
  typedef typename Isolated_vertices_renderer::Geometry Geometry;

  /*! Constructor. */
  Colored_isolated_vertices_renderer(Geometry& geo) :
    Isolated_vertices_renderer(geo) {}

  /*! Render the isolated vertices. */
  virtual void operator()(Draw_action* action)
  {
    const Vector3f& color = this->m_geo.get_aos_isolated_vertex_color();
    glColor3fv((float*)&color);
    Isolated_vertices_renderer::operator()(action);
  }
};

/*! A function object that draws edges as inflated lines. */
template <typename Edges_renderer>
class Inflated_line_edges_renderer : public Edges_renderer {
public:
  typedef typename Edges_renderer::Geometry     Geometry;

  /*! Constructor */
  Inflated_line_edges_renderer(Geometry& geo) : Edges_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_line_width(this->m_geo.get_aos_edge_line_width() * 8);
    Edges_renderer::operator()(action);
    context->draw_line_width(1.0f);
  }

  private:
    // Assignment operator cannot be generated.
    Inflated_line_edges_renderer&
      operator=(const Inflated_line_edges_renderer&);

    // In C++11, the following is supported:
    // Inflated_line_edges_renderer&
    //   operator=(const Inflated_line_edges_renderer&) = delete;
};

/*! A function object that draws edges as inflated strips. */
template <typename Edges_renderer>
class Inflated_strip_edges_renderer : public Edges_renderer {
public:
  typedef typename Edges_renderer::Geometry     Geometry;

  /*! Constructor. */
  Inflated_strip_edges_renderer(Geometry& geo) : Edges_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    float saved_edge_radius = this->m_geo.get_aos_edge_radius();
    this->m_geo.set_aos_edge_radius(saved_edge_radius * 2);
    Edges_renderer::operator()(action);
    this->m_geo.set_aos_edge_radius(saved_edge_radius);
  }
};

/*! A function object that draws edges as inflated tubes. */
template <typename Edges_renderer>
class Inflated_tube_edges_renderer : public Edges_renderer {
public:
  typedef typename Edges_renderer::Geometry     Geometry;

  /*! Constructor. */
  Inflated_tube_edges_renderer(Geometry& geo) : Edges_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    typedef Arrangement_renderer::Edge_shape    Edge_shape;

    float saved_edge_radius = this->m_geo.get_aos_edge_radius();
    this->m_geo.set_aos_edge_radius(saved_edge_radius * 2);
    this->m_geo.set_aos_edge_style(Edge_shape::STRIP);
    Edges_renderer::operator()(action);
    this->m_geo.set_aos_edge_radius(saved_edge_radius);
    this->m_geo.set_aos_edge_style(Edge_shape::TUBE);
  }
};

/*! A function object that renders the frontfacing  edges inflated. */
template <typename Geometry>
class Inflated_renderer : public Edges_renderer<Geometry> {
public:
  typedef Edges_renderer<Geometry>                   Base_renderer;

  /*! Constructor. */
  Inflated_renderer(Geometry& geo) : Base_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    typedef Arrangement_renderer::Edge_shape    Edge_shape;

    Context* context = action->get_context();

    float saved_edge_radius = 0;
    Boolean tube_edge_shape = false;
    if (this->m_geo.get_aos_edge_shape() == Edge_shape::LINE) {
      context->draw_line_width(this->m_geo.get_aos_edge_line_width() * 8);
    } else {
      saved_edge_radius = this->m_geo.get_aos_edge_radius();
      this->m_geo.set_aos_edge_radius(saved_edge_radius * 2);
      if (this->m_geo.get_aos_edge_shape() == Edge_shape::TUBE) {
        tube_edge_shape = true;
        this->m_geo.set_aos_edge_shape(Edge_shape::STRIP);
      }
    }
    Base_renderer::operator()(action);
    if (this->m_geo.get_aos_edge_shape() == Edge_shape::LINE) {
      context->draw_line_width(1);
    } else {
      this->m_geo.set_aos_edge_radius(saved_edge_radius);
      if (tube_edge_shape)
        this->m_geo.set_aos_edge_shape(Edge_shape::TUBE);
    }
  }
};

/*! A function object that draws edges as lines. */
template <typename Colored_edges_renderer>
class Line_colored_edges_renderer : public Colored_edges_renderer {
public:
  typedef typename Colored_edges_renderer::Geometry     Geometry;

  /*! Constructor */
  Line_colored_edges_renderer(Geometry& geo) : Colored_edges_renderer(geo) {}

  /*! Drawer operator */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_light_enable(false);
    context->draw_line_width(this->m_geo.get_aos_edge_line_width());
    Colored_edges_renderer::operator()(action);
    context->draw_line_width(1.0f);
    context->draw_light_enable(true);
  }
};

/*! A function object that draws vertices as rings. */
template <typename Colored_vertices_renderer>
class Point_colored_vertices_renderer : public Colored_vertices_renderer {
public:
  typedef typename Colored_vertices_renderer::Geometry  Geometry;

  /*! Constructor. */
  Point_colored_vertices_renderer(Geometry& geo) :
    Colored_vertices_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_light_enable(false);
    context->draw_point_size(this->m_geo.get_aos_vertex_point_size());
    Colored_vertices_renderer::operator()(action);
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  }
};

/*! A function object that draws vertices as points. */
template <typename Colored_vertices_renderer>
class Ring_colored_vertices_renderer : public Colored_vertices_renderer {
public:
  typedef typename Colored_vertices_renderer::Geometry  Geometry;

  /*! Constructor */
  Ring_colored_vertices_renderer(Geometry& geo) :
    Colored_vertices_renderer(geo) {}

  /*! Drawer operator */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_light_enable(false);
    context->draw_line_width(this->m_geo.get_aos_vertex_point_size());
    Colored_vertices_renderer::operator()(action);
    context->draw_light_enable(true);
    context->draw_line_width(1.0f);
  }
};

/*! A function object that draws isolated vertices as rings. */
template <typename Colored_isolated_vertices_renderer>
class Point_colored_isolated_vertices_renderer :
  public Colored_isolated_vertices_renderer
{
public:
  typedef typename Colored_isolated_vertices_renderer::Geometry Geometry;

  /*! Constructor. */
  Point_colored_isolated_vertices_renderer(Geometry& geo) :
    Colored_isolated_vertices_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_light_enable(false);
    context->draw_point_size(this->m_geo.get_aos_isolated_vertex_point_size());
    Colored_isolated_vertices_renderer::operator()(action);
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  }
};

/*! A function object that draws isolated vertices as points. */
template <typename Colored_isolated_vertices_renderer>
class Ring_colored_isolated_vertices_renderer :
  public Colored_isolated_vertices_renderer
{
public:
  typedef typename Colored_isolated_vertices_renderer::Geometry Geometry;

  /*! Constructor. */
  Ring_colored_isolated_vertices_renderer(Geometry& geo) :
    Colored_isolated_vertices_renderer(geo) {}

  /*! Drawer operator. */
  virtual void operator()(Draw_action* action)
  {
    Context* context = action->get_context();
    context->draw_light_enable(false);
    context->draw_line_width(this->m_geo.get_aos_isolated_vertex_point_size());
    Colored_isolated_vertices_renderer::operator()(action);
    context->draw_light_enable(true);
    context->draw_line_width(1.0f);
  }
};

SGAL_END_NAMESPACE

#endif
