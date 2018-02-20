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
 * A renderer of arrangements
 */

#ifndef SCGAL_ARRANGEMENT_RENDERER_HPP
#define SCGAL_ARRANGEMENT_RENDERER_HPP

#include <list>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Vector3f;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SCGAL_DECL Arrangement_renderer {
public:
  /*! A function object that renders something */
  class SGAL_SCGAL_DECL Renderer {
  public:
    /*! Render */
    virtual void operator()(Draw_action* action) = 0;

    /*! Destructor */
    virtual ~Renderer() {}
  };

protected:
  typedef std::list<Renderer*>                          Renderer_list;
  typedef Renderer_list::iterator                       Renderer_iter;

  /*! A list of renderers that render the embedding surface or faces. */
  Renderer_list m_surface_renderers;

  /*! A list of renderers that render the frontfacing features inflated */
  Renderer_list m_inflated_renderers;

  /*! A list of renderers that render the backfacing features */
  Renderer_list m_backfacing_renderers;

  /*! A list of renderers that render the frontfacing features */
  Renderer_list m_frontfacing_renderers;

  /*! A list of renderers that render all other features */
  Renderer_list m_other_renderers;

  /*! A list of renderers that draw into the depth buffer only */
  Renderer_list m_depth_renderers;

  /*! Stencil reference value */
  Int32 m_stencil_ref;

  /*! Halftone stipple pattern for backfacing elements */
  static Ubyte s_halftone[];

public:
  enum Renderer_type {
    SURFACE,
    INFLATED,
    BACKFACING,
    FRONTFACING,
    OTHER,
    DEPTH
  };

  struct Vertex_shape {
  public:
    enum Style { NONE, POINT, DISC, RING, BALL, NUM_STYLES };

    /*! Translate the vertex-shape style string to an enumeration
     * \param style the edge-shape style
     * \return the enumeration of style. If there is no match, then BALL
     */
    static Style style(const std::string& style);

  private:
    /*! Vertex styles */
    static const char* s_styles[];
  };

  /*! Edge shape enumerations and names */
  struct Edge_shape {
  public:
    enum Style { LINE, STRIP, TUBE, NUM_STYLES };

    /*! Translate the edge-shape style string to an enumeration
     * \param style the edge-shape style
     * \return the enumeration of style. If there is no match, then TUBE
     */
    static Style style(const std::string& style);

  private:
    /*! Edge styles */
    static const char* s_styles[];
  };

  /*! Constructor */
  Arrangement_renderer();

  /*! Destructor */
  ~Arrangement_renderer();

  /*! Insert a renderer to the back a given renderer list */
  void push_back(Renderer* renderer, Renderer_type type);

  /*! Insert a renderer to the front a given renderer list */
  void push_front(Renderer* renderer, Renderer_type type);

  /*! Clear all renderer lists */
  void clear();

  /*! Clear a given renderer list */
  void clear(Renderer_type type);

  /*! Render all */
  void operator()(Draw_action* action);

  /*! Set the stencil reference value */
  void set_stencil_ref(Int32 stencil_ref) { m_stencil_ref = stencil_ref; }
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
