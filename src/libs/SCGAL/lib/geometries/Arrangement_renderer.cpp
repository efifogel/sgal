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
// $Id: Arrangement_renderer.cpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A renderer of arrangements
 */

#include <cmath>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Rotation.hpp"

#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

/*! Vertex shape styles */
const char* Arrangement_renderer::Vertex_shape::s_styles[] = {
  "none", "point", "disc", "ring", "ball"
};

/*! Edge shape styles */
const char* Arrangement_renderer::Edge_shape::s_styles[] = {
  "line", "strip", "tube"
};

/*! Halftone stipple pattern for backfacing elements */
Ubyte Arrangement_renderer::s_halftone[] = {
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55
};

/*! Constructor */
Arrangement_renderer::Arrangement_renderer() : m_stencil_ref(0) {}

/*! Destructor */
Arrangement_renderer::~Arrangement_renderer() { clear(); }

/*! \brief clears all renderer lists */
void Arrangement_renderer::clear()
{
  m_surface_renderers.clear();
  m_inflated_renderers.clear();
  m_backfacing_renderers.clear();
  m_frontfacing_renderers.clear();
  m_other_renderers.clear();
  m_depth_renderers.clear();
}

/*! \brief clears a given renderer list. */
void Arrangement_renderer::clear(Renderer_type type)
{
  switch (type) {
   case SURFACE: m_surface_renderers.clear(); break;
   case INFLATED: m_inflated_renderers.clear(); break;
   case BACKFACING: m_backfacing_renderers.clear(); break;
   case FRONTFACING: m_frontfacing_renderers.clear(); break;
   case OTHER: m_other_renderers.clear(); break;
   case DEPTH: m_depth_renderers.clear(); break;
  }
}

/*! \brief inserts a renderer to the back a given renderer list. */
void Arrangement_renderer::push_back(Renderer* renderer, Renderer_type type)
{
  switch (type) {
   case SURFACE: m_surface_renderers.push_back(renderer); break;
   case INFLATED: m_inflated_renderers.push_back(renderer); break;
   case BACKFACING: m_backfacing_renderers.push_back(renderer); break;
   case FRONTFACING: m_frontfacing_renderers.push_back(renderer); break;
   case OTHER: m_other_renderers.push_back(renderer); break;
   case DEPTH: m_depth_renderers.push_back(renderer); break;
  }
}

/*! \brief inserts a renderer to the front a given renderer list. */
void Arrangement_renderer::push_front(Renderer* renderer, Renderer_type type)
{
  switch (type) {
   case SURFACE: m_surface_renderers.push_front(renderer); break;
   case INFLATED: m_inflated_renderers.push_front(renderer); break;
   case BACKFACING: m_frontfacing_renderers.push_front(renderer); break;
   case FRONTFACING: m_frontfacing_renderers.push_front(renderer); break;
   case OTHER: m_other_renderers.push_front(renderer); break;
   case DEPTH: m_depth_renderers.push_front(renderer); break;
  }
}

/*! \brief renders all. */
void Arrangement_renderer::operator()(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
  Renderer_iter it;
  
  Boolean depth_mask = context->get_depth_mask();

  /*! Phase 1: Surface & faces */
  // Update only the color buffer:
  context->draw_depth_mask(false);
  context->draw_transp_enable(true);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL, m_stencil_ref, 0xFFFFFFFFL);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
  for (it = m_surface_renderers.begin(); it != m_surface_renderers.end(); ++it)
  {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  context->draw_transp_enable(false);
  context->draw_depth_mask(true);
  glDisable(GL_STENCIL_TEST);
  
  /*! Phase 2: Inflated */
  context->draw_light_enable(false);  
  context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
  for (it = m_inflated_renderers.begin();
       it != m_inflated_renderers.end(); ++it)
  {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  context->draw_light_enable(true);  
  context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));    

  /*! Phase 3: Backfacing */
  context->draw_depth_mask(false);
  context->draw_cull_face(Gfx::FRONT_CULL);
  context->draw_light_model_sides(Gfx::TWO_SIDE);
  glEnable(GL_POLYGON_STIPPLE);
  glPolygonStipple(s_halftone);
  for (it = m_backfacing_renderers.begin();
       it != m_backfacing_renderers.end(); ++it)
  {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  glDisable(GL_POLYGON_STIPPLE);  

  /*! Phase 4: frontfacing */
  context->draw_light_model_sides(Gfx::ONE_SIDE);
  context->draw_cull_face(Gfx::BACK_CULL);
  context->draw_depth_func(Gfx::ALWAYS_DFUNC);
  context->draw_depth_mask(depth_mask);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL, m_stencil_ref + 1, 0xFFFFFFFFL);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);  
  for (it = m_frontfacing_renderers.begin();
       it != m_frontfacing_renderers.end(); ++it)
  {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  glDisable(GL_STENCIL_TEST);
  
  /*! Phase 5: */
  context->draw_depth_func(Gfx::LEQUAL_DFUNC);
  for (it = m_other_renderers.begin(); it != m_other_renderers.end(); ++it) {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  context->draw_depth_func(Gfx::LESS_DFUNC);

  /*! Phase 6: */
  context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
  for (it = m_depth_renderers.begin(); it != m_depth_renderers.end(); ++it) {
    Renderer & renderer = *(*it);
    renderer(action);
  }
  context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));    

  context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
  glColor4f(1, 1, 1, 1);
}

/*! \brief translates the vertex shape string to an enumeration. */
Arrangement_renderer::Vertex_shape::Style
Arrangement_renderer::Vertex_shape::style(const std::string & style)
{
  for (unsigned int i = 0; i < NUM_STYLES; ++i) {
    if (style.compare(s_styles[i]) == 0)
      return static_cast<Vertex_shape::Style>(i);
  }
  return BALL;
}

/*! \brief translates the edge shape style string to an enumeration. */
Arrangement_renderer::Edge_shape::Style
Arrangement_renderer::Edge_shape::style(const std::string & style)
{
  for (unsigned int i = 0; i < NUM_STYLES; ++i) {
    if (style.compare(s_styles[i]) == 0)
      return static_cast<Edge_shape::Style>(i);
  }
  return TUBE;
}

SGAL_END_NAMESPACE
