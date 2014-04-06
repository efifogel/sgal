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

#if defined(_WIN32)
#pragma warning( disable : 4146)
#pragma warning( disable : 4244)
#pragma warning( disable : 4390)
#pragma warning( disable : 4503)
#pragma warning( disable : 4800)
#pragma warning( disable : 4996)
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Context.hpp"

#include "SCGAL/Arrangement_on_surface_geo.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Arrangement_on_surface_geo::s_prototype(nullptr);

/*! Insertion strategy names */
const char* Arrangement_on_surface_geo::s_insertion_strategy_names[] =
  {"aggregate", "increment"};

/*! Default values */
const Arrangement_on_surface_geo::Insertion_strategy
  Arrangement_on_surface_geo::s_def_insertion_strategy(AGGREGATE);
const Boolean Arrangement_on_surface_geo::s_def_draw_opaque(false);
const Boolean Arrangement_on_surface_geo::s_def_draw_haloed(false);
const Boolean Arrangement_on_surface_geo::s_def_draw_halftone(true);

// Surface:
const Boolean Arrangement_on_surface_geo::s_def_draw_aos_surface(true);
const Vector3f Arrangement_on_surface_geo::
s_def_aos_surface_color(.5f, .5f, .5f);

// Vertex:
const Arrangement_on_surface_geo::Vertex_style
  Arrangement_on_surface_geo::s_def_aos_vertex_style(Vertex_shape::BALL);
const Float Arrangement_on_surface_geo::s_def_aos_vertex_radius(.06f);
const Float Arrangement_on_surface_geo::s_def_aos_vertex_point_size(2);
const Vector3f Arrangement_on_surface_geo::s_def_aos_vertex_color(.5f, 0, 0);

// Isolated Vertex:
const Arrangement_on_surface_geo::Vertex_style
  Arrangement_on_surface_geo::
  s_def_aos_isolated_vertex_style(Vertex_shape::BALL);
const Float Arrangement_on_surface_geo::s_def_aos_isolated_vertex_radius(.06f);
const Float Arrangement_on_surface_geo::s_def_aos_isolated_vertex_point_size(2);
const Vector3f Arrangement_on_surface_geo::
s_def_aos_isolated_vertex_color(.5f, .5f, 0);

// Boundary Vertex:
const Arrangement_on_surface_geo::Vertex_style
  Arrangement_on_surface_geo::
  s_def_aos_boundary_vertex_style(Vertex_shape::BALL);
const Float Arrangement_on_surface_geo::s_def_aos_boundary_vertex_radius(.06f);
const Float Arrangement_on_surface_geo::s_def_aos_boundary_vertex_point_size(2);
const Vector3f Arrangement_on_surface_geo::
s_def_aos_boundary_vertex_color(.5f, .5f, 0);

// Edge
const Boolean Arrangement_on_surface_geo::s_def_aos_edge_enabled(true);
const Arrangement_on_surface_geo::Edge_style
  Arrangement_on_surface_geo::s_def_aos_edge_style(Edge_shape::TUBE);
const Uint Arrangement_on_surface_geo::s_def_aos_edge_count(1);
const Boolean Arrangement_on_surface_geo::s_def_aos_edge_directed(false);
const Float Arrangement_on_surface_geo::s_def_aos_edge_radius(.03f);
const Float Arrangement_on_surface_geo::s_def_aos_edge_line_width(1);
const Float Arrangement_on_surface_geo::s_def_aos_delta_angle(.1f);
const Vector3f Arrangement_on_surface_geo::s_def_aos_edge_color(0, 0, .5f);

//! \brief constructor.
Arrangement_on_surface_geo::
Arrangement_on_surface_geo(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_insertion_strategy(s_def_insertion_strategy),
  m_draw_opaque(s_def_draw_opaque),
  m_draw_haloed(s_def_draw_haloed),
  m_draw_halftone(s_def_draw_halftone),
  // Surface
  m_draw_aos_surface(s_def_draw_aos_surface),
  m_aos_surface_color(s_def_aos_surface_color),
  // Vertex
  m_aos_vertex_style(s_def_aos_vertex_style),
  m_aos_vertex_radius(s_def_aos_vertex_radius),
  m_aos_vertex_point_size(s_def_aos_vertex_point_size),
  m_aos_vertex_color(s_def_aos_vertex_color),
  // Isolated vertex
  m_aos_isolated_vertex_style(s_def_aos_isolated_vertex_style),
  m_aos_isolated_vertex_radius(s_def_aos_isolated_vertex_radius),
  m_aos_isolated_vertex_point_size(s_def_aos_isolated_vertex_point_size),
  m_aos_isolated_vertex_color(s_def_aos_isolated_vertex_color),
  // Boundary vertex
  m_aos_boundary_vertex_style(s_def_aos_boundary_vertex_style),
  m_aos_boundary_vertex_radius(s_def_aos_boundary_vertex_radius),
  m_aos_boundary_vertex_point_size(s_def_aos_boundary_vertex_point_size),
  m_aos_boundary_vertex_color(s_def_aos_boundary_vertex_color),
  // Edge
  m_aos_edge_enabled(s_def_aos_edge_enabled),
  m_aos_edge_style(s_def_aos_edge_style),
  m_aos_edge_count(s_def_aos_edge_count),
  m_aos_edge_directed(s_def_aos_edge_directed),
  m_aos_edge_radius(s_def_aos_edge_radius),
  m_aos_edge_line_width(s_def_aos_edge_line_width),
  m_aos_delta_angle(s_def_aos_delta_angle),
  m_aos_edge_color(s_def_aos_edge_color),
  // Other
  m_renderer_dirty(true),
  m_surface_renderer(nullptr),
  m_colored_surface_renderer(nullptr),
  m_stencil_surface_renderer(nullptr),
  m_edges_renderer(nullptr),
  m_vertices_renderer(nullptr),
  m_isolated_vertices_renderer(nullptr),
  m_colored_edges_renderer(nullptr),
  m_colored_vertices_renderer(nullptr),
  m_colored_isolated_vertices_renderer(nullptr),
  m_line_colored_edges_renderer(nullptr),
  m_point_colored_vertices_renderer(nullptr),
  m_ring_colored_vertices_renderer(nullptr),
  m_point_colored_isolated_vertices_renderer(nullptr),
  m_ring_colored_isolated_vertices_renderer(nullptr),
  m_inflated_line_edges_renderer(nullptr),
  m_inflated_strip_edges_renderer(nullptr),
  m_inflated_tube_edges_renderer(nullptr)
{}

//! \brief destructor.
Arrangement_on_surface_geo::~Arrangement_on_surface_geo() { clear(); }

//! \brief initializes the container prototype.
void Arrangement_on_surface_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Arrangement_on_surface_geo::
                                    renderer_changed);

  // drawOpaque
  Boolean_handle_function draw_opaque_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::draw_opaque_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_OPAQUE,
                                          "drawOpaque",
                                          RULE_EXPOSED_FIELD,
                                          draw_opaque_func,
                                          exec_func));

  // drawHaloed
  Boolean_handle_function draw_haloed_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::draw_haloed_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_HALOED,
                                          "drawHaloed",
                                          RULE_EXPOSED_FIELD,
                                          draw_haloed_func,
                                          exec_func));

  // drawHalftone
  Boolean_handle_function draw_halftone_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::draw_halftone_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_HALFTONE,
                                          "drawHalftone",
                                          RULE_EXPOSED_FIELD,
                                          draw_halftone_func,
                                          exec_func));

  // drawSurface
  Boolean_handle_function draw_aos_surface_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::draw_aos_surface_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_AOS_SURFACE,
                                          "drawSurface",
                                          RULE_EXPOSED_FIELD,
                                          draw_aos_surface_func,
                                          exec_func));

  // aosVertexStyleId
  Uint_handle_function aos_vertex_style_func =
    reinterpret_cast<Uint_handle_function>
    (&Arrangement_on_surface_geo::aos_vertex_style_handle);
  s_prototype->add_field_info(new SF_uint(AOS_VERTEX_STYLE_ID,
                                          "aosVertexStyleId",
                                          RULE_EXPOSED_FIELD,
                                          aos_vertex_style_func,
                                          exec_func));

  Uint_handle_function aos_isolated_vertex_style_func =
    reinterpret_cast<Uint_handle_function>
    (&Arrangement_on_surface_geo::aos_isolated_vertex_style_handle);
  // aosIsolatedVertexStyleId
  s_prototype->add_field_info(new SF_uint(AOS_ISOLATED_VERTEX_STYLE_ID,
                                          "aosIsolatedVertexStyleId",
                                          RULE_EXPOSED_FIELD,
                                          aos_isolated_vertex_style_func,
                                          exec_func));

  // aosIsolatedVertexRadius
  Float_handle_function aos_isolated_vertex_radius_func =
    static_cast<Float_handle_function>
    (&Arrangement_on_surface_geo::aos_isolated_vertex_radius_handle);
  s_prototype->add_field_info(new SF_float(AOS_ISOLATED_VERTEX_RADIUS,
                                           "aosIsolatedVertexRadius",
                                           RULE_EXPOSED_FIELD,
                                           aos_isolated_vertex_radius_func));

  // aosEdgeEnabled
  Boolean_handle_function aos_edge_enabled_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::aos_edge_enabled_handle);
  s_prototype->add_field_info(new SF_bool(AOS_EDGE_ENABLED,
                                          "aosEdgeEnabled",
                                          RULE_EXPOSED_FIELD,
                                          aos_edge_enabled_func,
                                          exec_func));

  // aosEdgeStyleId
  Uint_handle_function aos_edge_style_func =
    reinterpret_cast<Uint_handle_function>
    (&Arrangement_on_surface_geo::aos_edge_style_handle);
  s_prototype->add_field_info(new SF_uint(AOS_EDGE_STYLE_ID,
                                          "aosEdgeStyleId",
                                          RULE_EXPOSED_FIELD,
                                          aos_edge_style_func,
                                          exec_func));

  // aosEdgeCountId
  Uint_handle_function aos_edge_count_func =
    static_cast<Uint_handle_function>
    (&Arrangement_on_surface_geo::aos_edge_count_handle);
  s_prototype->add_field_info(new SF_uint(AOS_EDGE_COUNT_ID,
                                          "aosEdgeCountId",
                                          RULE_EXPOSED_FIELD,
                                          aos_edge_count_func,
                                          exec_func));

  // aosEdgeDirected
  Boolean_handle_function aos_edge_directed_func =
    static_cast<Boolean_handle_function>
    (&Arrangement_on_surface_geo::aos_edge_directed_handle);
  s_prototype->add_field_info(new SF_bool(AOS_EDGE_DIRECTED,
                                          "aosEdgeDirected",
                                          RULE_EXPOSED_FIELD,
                                          aos_edge_directed_func,
                                          exec_func));
}

//! \brief deletes the container prototype.
void Arrangement_on_surface_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Arrangement_on_surface_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_surface_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
void Arrangement_on_surface_geo::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);

    if (name == "insertionStrategy") {
      std::string req_strategy = strip_double_quotes(value);
      for (unsigned int i = 0; i < NUM_INSERTION_STRATEGIES; ++i) {
        const char* strategy = s_insertion_strategy_names[i];
        if (req_strategy.compare(strategy) == 0) {
          m_insertion_strategy = (Insertion_strategy) i;
          break;
        }
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawOpaque") {
      m_draw_opaque = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawHaloed") {
      m_draw_haloed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawHalftone") {
      m_draw_halftone = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawSurface") {
      m_draw_aos_surface = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "surfaceColor") {
      Vector3f col(value);
      m_aos_surface_color = col;
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosVertexStyle") {
      m_aos_vertex_style = Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosVertexRadius") {
      m_aos_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosVertexPointSize") {
      m_aos_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosVertexColor") {
      Vector3f col(value);
      set_aos_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosIsolatedVertexStyle") {
      m_aos_isolated_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosIsolatedVertexRadius") {
      m_aos_isolated_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosIsolatedVertexPointSize") {
      m_aos_isolated_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosIsolatedVertexColor") {
      Vector3f col(value);
      m_aos_isolated_vertex_color = col;
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosBoundaryVertexStyle") {
      m_aos_boundary_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexRadius") {
      m_aos_boundary_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexPointSize") {
      m_aos_boundary_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosBoundaryVertexColor") {
      Vector3f col(value);
      m_aos_boundary_vertex_color = col;
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosEdgeEnabled") {
      m_aos_edge_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeStyle") {
      m_aos_edge_style = Edge_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeCount") {
      m_aos_edge_count = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeDirected") {
      m_aos_edge_directed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeRadius") {
      m_aos_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeLineWidth") {
      m_aos_edge_line_width = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosEdgeColor") {
      Vector3f col(value);
      set_aos_edge_color(col);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief */
void Arrangement_on_surface_geo::cull(Cull_context& /* cull_context */) {}

/*! \brief draws the geometry */
void Arrangement_on_surface_geo::draw(Draw_action* action)
{
  if (is_dirty()) clean();
  // if (is_empty()) return;

  Boolean normalize = has_scale();
  if (normalize) glEnable(GL_NORMALIZE);
  if (m_renderer_dirty) clean_renderer();
  if (m_draw_opaque) draw_opaque(action);
  else m_renderer(action);
  if (normalize) glDisable(GL_NORMALIZE);
}

/*! \brief draws the arrangement on sphere opaque.
 * 1. Draw the geometry once without updating the depth buffer. Draw only
 * the embedding surface, and the vertices and edges rendered with flat
 * polygons embedded on the sphere, as indicated by their shapes.
 * 2. Draw the geometry drawn in (1.) again without updating the color buffer.
 * If vertices or edges are drawn as points or lines, use polygon offset when
 * updating the depth buffer.
 * 3. Draw the vertices and edges rendered with points and lines
 * 4. Draw the vertices and edges rendered the usual way.
 */
void Arrangement_on_surface_geo::draw_opaque(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);

  // Update only the color buffer:
  context->draw_depth_mask(false);

  // Draw the sphere:
  if (m_draw_aos_surface) {
    glColor3fv((float*)&m_aos_surface_color);
    (*m_surface_renderer)(action);
  }

  // Draw the edges and the vertices:
  if (m_aos_edge_style == Edge_shape::STRIP)
    (*m_colored_edges_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::DISC)
    (*m_colored_vertices_renderer)(action);
  if (m_aos_isolated_vertex_style == Vertex_shape::DISC)
    (*m_colored_isolated_vertices_renderer)(action);

  context->draw_depth_mask(true);

  // Update only the depth buffer:
  if ((m_aos_edge_style == Edge_shape::LINE) ||
      (m_aos_vertex_style == Vertex_shape::POINT) ||
      (m_aos_vertex_style == Vertex_shape::RING))
  {
    glPolygonOffset(16, 16);
    glEnable(GL_POLYGON_OFFSET_FILL);
  }
  context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
  (*m_surface_renderer)(action);
  if (m_aos_edge_style == Edge_shape::STRIP)
    draw_aos_edges(action);
  if (m_aos_vertex_style == Vertex_shape::DISC)
    draw_aos_vertices(action);
  if (m_aos_isolated_vertex_style == Vertex_shape::DISC)
    draw_aos_isolated_vertices(action);
  context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));
  if ((m_aos_edge_style == Edge_shape::LINE) ||
      (m_aos_vertex_style == Vertex_shape::POINT) ||
      (m_aos_vertex_style == Vertex_shape::RING))
  {
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0, 0);
  }

  // Draw the edges as lines:
  if (m_aos_edge_style == Edge_shape::LINE) {
    context->draw_light_enable(false);
    context->draw_line_width(m_aos_edge_line_width);
  }
  (*m_colored_edges_renderer)(action);
  if (m_aos_edge_style == Edge_shape::LINE) {
    context->draw_line_width(1.0f);
    context->draw_light_enable(true);
  }

  // Draw the vertices as points or lines:
  if (m_aos_vertex_style == Vertex_shape::POINT) {
    context->draw_light_enable(false);
    context->draw_point_size(m_aos_vertex_point_size);
    glIsEnabled(GL_POINT_SMOOTH);
  }
  else if (m_aos_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(false);
    context->draw_line_width(m_aos_vertex_point_size);
    glIsEnabled(GL_LINE_SMOOTH);
  }
  (*m_colored_vertices_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::POINT) {
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  }
  else if (m_aos_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(true);
    context->draw_line_width(1.0);
  }

  // Draw the isolated vertices as points or lines:
  if (m_aos_isolated_vertex_style == Vertex_shape::POINT) {
    context->draw_light_enable(false);
    context->draw_point_size(m_aos_vertex_point_size);
    glIsEnabled(GL_POINT_SMOOTH);
  }
  else if (m_aos_isolated_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(false);
    context->draw_line_width(m_aos_vertex_point_size);
    glIsEnabled(GL_LINE_SMOOTH);
  }
  if (m_colored_isolated_vertices_renderer)
    (*m_colored_isolated_vertices_renderer)(action);
  if (m_aos_isolated_vertex_style == Vertex_shape::POINT) {
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  }
  else if (m_aos_isolated_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(true);
    context->draw_line_width(1.0);
  }

  // Draw the usual way:
  if (m_aos_edge_style == Edge_shape::TUBE)
    (*m_colored_edges_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::BALL)
    (*m_colored_vertices_renderer)(action);

  context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
}

//! \brief raises the flag that indicates that the renderer changed.
void Arrangement_on_surface_geo::
renderer_changed(const Field_info* /* field_info */)
{
  m_renderer.clear();
  m_renderer_dirty = true;
}

//! \brief cleans the renderer.
void Arrangement_on_surface_geo::clean_renderer()
{
  if (get_draw_aos_surface())
    m_renderer.push_back(m_colored_surface_renderer,
                         Arrangement_renderer::SURFACE);
  else m_renderer.push_back(m_stencil_surface_renderer,
                            Arrangement_renderer::SURFACE);

  if (m_draw_haloed) {
    if (get_aos_edge_style() == Edge_shape::LINE)
      m_renderer.push_back(m_inflated_line_edges_renderer,
                           Arrangement_renderer::INFLATED);
    else if (get_aos_edge_style() == Edge_shape::STRIP)
      m_renderer.push_back(m_inflated_strip_edges_renderer,
                           Arrangement_renderer::INFLATED);
    else if (get_aos_edge_style() == Edge_shape::TUBE)
      m_renderer.push_back(m_inflated_tube_edges_renderer,
                           Arrangement_renderer::INFLATED);
  }
  if (get_aos_edge_style() == Edge_shape::STRIP) {
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::FRONTFACING);
  } else {
    if (get_aos_edge_style() == Edge_shape::LINE)
      m_renderer.push_back(m_line_colored_edges_renderer,
                           Arrangement_renderer::OTHER);
    else
      m_renderer.push_back(m_colored_edges_renderer,
                           Arrangement_renderer::OTHER);
  }
  if (get_aos_vertex_style() == Vertex_shape::DISC) {
    m_renderer.push_back(m_colored_vertices_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_vertices_renderer,
                         Arrangement_renderer::FRONTFACING);
  } else {
    if (get_aos_vertex_style() == Vertex_shape::POINT)
      m_renderer.push_back(m_point_colored_vertices_renderer,
                           Arrangement_renderer::OTHER);
    else if (get_aos_vertex_style() == Vertex_shape::RING)
      m_renderer.push_back(m_ring_colored_vertices_renderer,
                           Arrangement_renderer::OTHER);
    else
      m_renderer.push_back(m_colored_vertices_renderer,
                           Arrangement_renderer::OTHER);
  }
  if (get_aos_isolated_vertex_style() == Vertex_shape::DISC) {
    m_renderer.push_back(m_colored_isolated_vertices_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_isolated_vertices_renderer,
                         Arrangement_renderer::FRONTFACING);
  } else {
    if (get_aos_isolated_vertex_style() == Vertex_shape::POINT)
      m_renderer.push_back(m_point_colored_isolated_vertices_renderer,
                           Arrangement_renderer::OTHER);
    else if (get_aos_isolated_vertex_style() == Vertex_shape::RING)
      m_renderer.push_back(m_ring_colored_isolated_vertices_renderer,
                           Arrangement_renderer::OTHER);
    else
      m_renderer.push_back(m_colored_isolated_vertices_renderer,
                           Arrangement_renderer::OTHER);
  }

  if (get_draw_aos_surface())
    m_renderer.push_back(m_surface_renderer, Arrangement_renderer::DEPTH);
  if (is_aos_edge_enabled())
    m_renderer.push_back(m_edges_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_vertex_style() != Vertex_shape::NONE) {
    m_renderer.push_back(m_vertices_renderer, Arrangement_renderer::DEPTH);
    m_renderer.push_back(m_isolated_vertices_renderer,
                         Arrangement_renderer::DEPTH);
  }

  m_renderer_dirty = false;
}

SGAL_END_NAMESPACE
