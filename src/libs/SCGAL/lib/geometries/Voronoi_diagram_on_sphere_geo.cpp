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

#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Voronoi_diagram_on_sphere_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"

SGAL_BEGIN_NAMESPACE

std::string Voronoi_diagram_on_sphere_geo::s_tag = "VoronoiDiagramOnSphere";
Container_proto* Voronoi_diagram_on_sphere_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Voronoi_diagram_on_sphere_geo,
                    "Voronoi_diagram_on_sphere_geo");

/*! Default values */
const Voronoi_diagram_on_sphere_geo::Site_style
  Voronoi_diagram_on_sphere_geo::s_def_site_style(Site_shape::BALL);
const Float Voronoi_diagram_on_sphere_geo::s_def_site_radius(0.06f);
const Float Voronoi_diagram_on_sphere_geo::s_def_site_point_size(1);
const Float Voronoi_diagram_on_sphere_geo::s_def_site_delta_angle(0.1f);

//! \brief constructs.
Voronoi_diagram_on_sphere_geo::
Voronoi_diagram_on_sphere_geo(Boolean proto) :
  Geodesic_voronoi_on_sphere_geo(proto),
  m_owned_vos(false),
  m_vos(nullptr),
  m_site_style(s_def_site_style),
  m_site_radius(s_def_site_radius),
  m_site_point_size(s_def_site_point_size),
  m_site_delta_angle(s_def_site_delta_angle)
{ if (!proto) create_renderers(); }

//! \brief destructs.
Voronoi_diagram_on_sphere_geo::~Voronoi_diagram_on_sphere_geo()
{
  clear();
  if (m_site_renderer) delete m_site_renderer;
  if (m_site_other_renderer) delete m_site_other_renderer;
  if (m_owned_vos) {
    if (m_vos) {
      delete m_vos;
      m_vos = nullptr;
    }
    m_owned_vos = false;
  }
}

//! \brief initializes the container prototype.
void Voronoi_diagram_on_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Geodesic_voronoi_on_sphere_geo::get_prototype());

  // siteStyleId
  Execution_function exec_func = static_cast<Execution_function>
    (&Arrangement_on_surface_geo::renderer_changed);
  Uint_handle_function site_style_func =
    reinterpret_cast<Uint_handle_function>
    (&Voronoi_diagram_on_sphere_geo::site_style_handle);
  s_prototype->add_field_info(new SF_uint(SITE_STYLE_ID, "siteStyleId",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          site_style_func, exec_func));
}

//! \brief deletes the container prototype.
void Voronoi_diagram_on_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Voronoi_diagram_on_sphere_geo::get_prototype()
{
  if (!s_prototype) Voronoi_diagram_on_sphere_geo::init_prototype();
  return s_prototype;
}

//! \brief sets the ellpsoid attributes.
 void Voronoi_diagram_on_sphere_geo::set_attributes(Element* elem)
{
  Geodesic_voronoi_on_sphere_geo::set_attributes(elem);

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);

    if (name == "siteStyle") {
      m_site_style = Site_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteRadius") {
      m_site_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "sitePointSize") {
      m_site_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "coord") {
      Shared_coord_array coord_array =
        boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief cleans the representation.
void Voronoi_diagram_on_sphere_geo::clean()
{
  Geodesic_voronoi_on_sphere_geo::clean();

  if (!m_vos) {
    m_vos = new Voronoi_on_sphere;
    SGAL_assertion(m_vos);
    m_owned_vos = true;
  }
  auto exact_coord_array =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  if (exact_coord_array && (exact_coord_array->size() > 0)) {
    Epec_kernel kernel;

    if (this->m_site_indices.size() != 0) {
      // Compute the voronoi:
      std::vector<Site> points;
      points.resize(this->m_site_indices.size());
      Uint i = 0;
      for (auto it = this->m_site_indices.begin();
           it != this->m_site_indices.end(); ++it)
      {
        Epec_point_3& p = (*exact_coord_array)[*it];
        Epec_vector_3 v = kernel.construct_vector_3_object()(CGAL::ORIGIN, p);
        Epec_direction_3 d = kernel.construct_direction_3_object()(v);
        points[i++] = d;
      }
      CGAL::voronoi_2(points.begin(), points.end(), *(this->m_vos));
      points.clear();
    }
  }
}

//! \brief clears the internal representation and auxiliary data structures.
void Voronoi_diagram_on_sphere_geo::clear()
{
  this->m_dirty = true;
  if (m_vos) m_vos->clear();
}

//! \brief
void Voronoi_diagram_on_sphere_geo::cull(Cull_context& cull_context) {}

//! \brief draws the arrangement on sphere opaque.
void Voronoi_diagram_on_sphere_geo::draw_opaque(Draw_action* action)
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
  if (m_aos_edge_style == Edge_shape::STRIP) draw_aos_edges(action);
  if (m_aos_vertex_style == Vertex_shape::DISC)
    draw_aos_vertices(action);
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
  } else if (m_aos_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(false);
    context->draw_line_width(m_aos_vertex_point_size);
    glIsEnabled(GL_LINE_SMOOTH);
  }
  (*m_colored_vertices_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::POINT) {
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  } else if (m_aos_vertex_style == Vertex_shape::RING) {
    context->draw_light_enable(true);
    context->draw_line_width(1.0);
  }

  // Draw the usual way:
  if (m_aos_edge_style == Edge_shape::TUBE)
    (*m_colored_edges_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::BALL)
    (*m_colored_vertices_renderer)(action);

  if (m_draw_sites) draw_sites(action);
  context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
}

//! \brief draws a site.
void Voronoi_diagram_on_sphere_geo::draw_site(Draw_action* action,
                                              Epec_point_3& point)
{
  float x = static_cast<float>(CGAL::to_double(point.x()));
  float y = static_cast<float>(CGAL::to_double(point.y()));
  float z = static_cast<float>(CGAL::to_double(point.z()));
  Vector3f center(x, y, z);
  center.normalize();
  draw_vertex_on_sphere(action, center, m_site_style, m_site_radius,
                        m_site_delta_angle);
}

//! \brief draws the sites.
void Voronoi_diagram_on_sphere_geo::draw_sites(Draw_action* action)
{
  glColor3fv((float*)&m_site_color[0]);

  auto exact_coord_array =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  if (exact_coord_array && (exact_coord_array->size() > 0)) {
    std::vector<Uint>::iterator it;
    for (it = this->m_site_indices.begin();
         it != this->m_site_indices.end(); ++it)
    {
      Epec_point_3& point = (*exact_coord_array)[*it];
      draw_site(action, point);
    }
  }
}

//! \brief cleans the renderer.
void Voronoi_diagram_on_sphere_geo::clean_renderer()
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

  if (get_draw_aos_surface())
    m_renderer.push_back(m_surface_renderer, Arrangement_renderer::DEPTH);
  if (is_aos_edge_enabled())
    m_renderer.push_back(m_edges_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_vertices_renderer, Arrangement_renderer::DEPTH);

  if (!get_draw_sites()) return;
  if (get_site_style() == Vertex_shape::DISC) {
    m_renderer.push_back(m_site_renderer, Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_site_renderer, Arrangement_renderer::FRONTFACING);
  } else {
    m_renderer.push_back(m_site_other_renderer, Arrangement_renderer::OTHER);
  }

  m_renderer_dirty = false;
}

//! \brief draws the sites.
void Voronoi_diagram_on_sphere_geo::Site_other_renderer::
operator()(Draw_action* action)
{
  Context* context = action->get_context();
  if (m_geo.get_site_style() == Site_shape::POINT) {
    context->draw_light_enable(false);
    context->draw_point_size(m_geo.get_site_point_size());
  }
  m_geo.draw_sites(action);
  if (m_geo.get_site_style() == Site_shape::POINT) {
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);
  }
}

//! \brief creates the renderers.
void Voronoi_diagram_on_sphere_geo::create_renderers()
{
  m_edges_renderer = new Voronoi_edges_renderer(*this);
  m_vertices_renderer = new Voronoi_vertices_renderer(*this);
  m_colored_edges_renderer = new Voronoi_colored_edges_renderer(*this);
  m_colored_vertices_renderer = new Voronoi_colored_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Voronoi_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Voronoi_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Voronoi_ring_colored_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Voronoi_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Voronoi_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Voronoi_inflated_tube_edges_renderer(*this);

  m_site_renderer = new Site_renderer(*this);
  m_site_other_renderer = new Site_other_renderer(*this);
}

//! \brief destroys the renderers.
void Voronoi_diagram_on_sphere_geo::destroy_renderers()
{
  if (m_edges_renderer) delete m_edges_renderer;
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;
  if (m_line_colored_edges_renderer) delete m_line_colored_edges_renderer;
  if (m_point_colored_vertices_renderer)
    delete m_point_colored_vertices_renderer;
  if (m_ring_colored_vertices_renderer)
    delete m_ring_colored_vertices_renderer;
  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;
}

SGAL_END_NAMESPACE
