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
// $Id: Power_diagram_on_sphere_geo.cpp 7729 2009-06-18 08:47:21Z efif $
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
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Torus.hpp"
#include "SGAL/Arc.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"

#include "SCGAL/Power_diagram_on_sphere_geo.hpp"
#include "SCGAL/Exact_plane_array.hpp"

SGAL_BEGIN_NAMESPACE

std::string Power_diagram_on_sphere_geo::s_tag = "PowerDiagramOnSphere";
Container_proto* Power_diagram_on_sphere_geo::s_prototype(NULL);

REGISTER_TO_FACTORY(Power_diagram_on_sphere_geo, "Power_diagram_on_sphere_geo");

/*! Default values */
const Boolean Power_diagram_on_sphere_geo::s_def_site_enabled(true);
const Power_diagram_on_sphere_geo::Site_style
  Power_diagram_on_sphere_geo::s_def_site_style(Edge_shape::LINE);
const Int Power_diagram_on_sphere_geo::s_def_site_count(1);
const Boolean Power_diagram_on_sphere_geo::s_def_site_directed(false);
const Float Power_diagram_on_sphere_geo::s_def_site_radius(.06f);
const Float Power_diagram_on_sphere_geo::s_def_site_line_width(1);
const Float Power_diagram_on_sphere_geo::s_def_site_delta_angle(.1f);

/*! Constructor. */
Power_diagram_on_sphere_geo::
Power_diagram_on_sphere_geo(Boolean proto) :
  Geodesic_voronoi_on_sphere_geo(proto),
  m_owned_vos(false),
  m_vos(NULL),
  m_site_enabled(s_def_site_enabled),
  m_site_style(s_def_site_style),
  m_site_count(s_def_site_count),
  m_site_directed(s_def_site_directed),
  m_site_radius(s_def_site_radius),
  m_site_line_width(s_def_site_line_width),
  m_site_delta_angle(s_def_site_delta_angle)
{ if (!proto) create_renderers(); }

/*! Destructor. */
Power_diagram_on_sphere_geo::~Power_diagram_on_sphere_geo()
{
  clear();
  if (m_inflated_site_renderer) delete m_inflated_site_renderer;
  if (m_site_renderer) delete m_site_renderer;
  if (m_site_other_renderer) delete m_site_other_renderer;
  if (m_owned_vos) {
    if (m_vos) {
      delete m_vos;
      m_vos = NULL;
    }
    m_owned_vos = false;
  }
}

/*! \brief initializes the container prototype. */
void Power_diagram_on_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Geodesic_voronoi_on_sphere_geo::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Arrangement_on_surface_geo::
                                    renderer_changed);

  // siteEnabled
  s_prototype->
    add_field_info(new SF_bool(SITE_ENABLED, "siteEnabled",
                               get_member_offset(&m_site_enabled),
                               exec_func));

  // siteStyleId
  s_prototype->
    add_field_info(new SF_int(SITE_STYLE_ID, "siteStyleId",
                              get_member_offset(&m_site_style),
                              exec_func));

  // siteCountId
  s_prototype->
    add_field_info(new SF_int(SITE_COUNT_ID, "siteCountId",
                              get_member_offset(&m_site_count),
                              exec_func));

  // siteDirected
  s_prototype->
    add_field_info(new SF_bool(SITE_DIRECTED, "siteDirected",
                               get_member_offset(&m_site_directed),
                               exec_func));
}

/*! \brief deletes the container prototype. */
void Power_diagram_on_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Power_diagram_on_sphere_geo::get_prototype()
{
  if (!s_prototype) Power_diagram_on_sphere_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes. */
 void Power_diagram_on_sphere_geo::set_attributes(Element* elem)
{
  Geodesic_voronoi_on_sphere_geo::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "siteEnabled") {
      m_site_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteStyle") {
      m_site_style = Site_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteCount") {
      m_site_count = boost::lexical_cast<Int>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteDirected") {
      m_site_directed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteRadius") {
      m_site_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteLineWidth") {
      m_site_line_width = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Element::Shared_container cont = elem->get_value(cai);
    if (name == "plane") {
      Shared_coeff_array coeff_array =
        boost::dynamic_pointer_cast<Coeff_array>(cont);
      set_coeff_array(coeff_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief cleans the representation. */
void Power_diagram_on_sphere_geo::clean()
{
  Geodesic_voronoi_on_sphere_geo::clean();

  if (!m_vos) {
    m_vos = new Voronoi_on_sphere;
    SGAL_assertion(m_vos);
    m_owned_vos = true;
  }

  Exact_kernel kernel;
  Exact_point_3 origin = kernel.construct_point_3_object() (CGAL::ORIGIN);

  Shared_exact_plane_array exact_coeff_array =
    boost::dynamic_pointer_cast<Exact_plane_array>(m_coeff_array);
  if (exact_coeff_array && (exact_coeff_array->size() > 0)) {

    if (this->m_site_indices.size() != 0) {
      // Compute the voronoi:
      std::vector<Uint>::iterator it;
      std::vector<Site> sites;
      sites.resize(this->m_site_indices.size());
      Uint i = 0;
      for (it = this->m_site_indices.begin();
           it != this->m_site_indices.end(); ++it)
      {
        Exact_plane_3 plane = (*exact_coeff_array)[*it];
        if (kernel.oriented_side_3_object() (plane, origin) ==
            CGAL::ON_NEGATIVE_SIDE)
          plane = kernel.construct_opposite_plane_3_object() (plane);
        sites[i++] = plane;
      }
      CGAL::voronoi_2(sites.begin(), sites.end(), *(this->m_vos));
      sites.clear();
    }
  }
}

/*! \brief clears the internal representation and auxiliary data structures. */
void Power_diagram_on_sphere_geo::clear()
{
  this->m_dirty = true;
  if (m_vos) m_vos->clear();
}

/*! \brief */
void Power_diagram_on_sphere_geo::cull(Cull_context& cull_context) {}

/*! \brief sets the plane coefficients array. */
inline void
Power_diagram_on_sphere_geo::set_coeff_array(Shared_coeff_array coeff_array)
{
  m_coeff_array = coeff_array;
  // No need to recalculate the sphere bound
}

/*! \brief draws the arrangement on sphere opaque. */
void Power_diagram_on_sphere_geo::draw_opaque(Draw_action* action)
{
  Arrangement_on_sphere_base_geo::draw_opaque(action);
  if (m_draw_sites) draw_sites(action);
}

/*! \brief draws a site. */
void Power_diagram_on_sphere_geo::draw_site(Draw_action* action,
                                            Exact_plane_3& plane)
{
  float a = static_cast<float>(CGAL::to_double(plane.a()));
  float b = static_cast<float>(CGAL::to_double(plane.b()));
  float c = static_cast<float>(CGAL::to_double(plane.c()));
  float d = static_cast<float>(CGAL::to_double(plane.d()));

  float length_square = a*a + b*b + c*c;
  float length_reciprocal = 1.0f / sqrtf(length_square);
  float radius = sqrtf(length_square - d*d) * length_reciprocal;
  float x = a * length_reciprocal;
  float y = b * length_reciprocal;
  float z = c * length_reciprocal;
  Vector3f normal(x, y, z);
  Vector3f ref(0, 0, 1);
  Rotation rot;
  rot.make(ref, normal);

  Transform transform;
  transform.set_rotation(rot);
  glPushMatrix();
  glMultMatrixf((float *)&(transform.get_matrix()));

  switch (m_site_style) {
   case Edge_shape::TUBE:                 // draw a tube
    {
     Torus torus;
     torus.set_cross_section_radius(m_site_radius);
     torus.set_spine_radius(radius);
     torus.set_slices(static_cast<Uint>(SGAL_TWO_PI / m_site_delta_angle));
     torus.set_stacks(static_cast<Uint>(SGAL_PI / m_site_delta_angle));

     // Transform the torus:
     Transform transform1;
     transform1.set_translation(.0f, .0f, -d * length_reciprocal);
     transform1.set_rotation(1.0f, .0f, .0f, SGAL_HALF_PI);

     glMultMatrixf((float *)&(transform1.get_matrix()));
     torus.draw(action);
    }
    break;

   case Edge_shape::LINE:                // draw a line
    {
     glBegin(GL_LINE_LOOP);

     // Find a vertex on the perimeter fan:
     Vector3f vec(radius, .0f, .0f);

     Rotation rot2(ref, 0);
     float angle;
     for (angle = 0; angle < SGAL_TWO_PI; angle += m_aos_delta_angle) {
       rot2.set_angle(angle);
       Vector3f tmp;
       rot2.rotate(vec, tmp);
       tmp[2] = -d * length_reciprocal;
       glVertex3fv((float*)&tmp);
     }
     glEnd();
    }
    break;

   case Edge_shape::STRIP:
    {
     Arc arc;
     arc.set_solid(true);
     arc.set_slices(static_cast<Uint>(SGAL_TWO_PI / m_site_delta_angle));
     arc.set_stacks(static_cast<Uint>(SGAL_PI / m_site_delta_angle));

     float angle1 = atan2f(radius, -d * length_reciprocal);
     float angle2 = asinf(m_site_radius);

     arc.set_beta(angle1 - angle2);
     arc.set_delta(2 * angle2);

     // Transform the arc:
     Transform transform1;
     transform1.set_rotation(1.0f, .0f, .0f, SGAL_HALF_PI);

     glMultMatrixf((float *)&(transform1.get_matrix()));
     arc.draw(action);
    }
    break;

   default: break;
  }

  glPopMatrix();
}

/*! \brief draws the sites */
void Power_diagram_on_sphere_geo::draw_sites(Draw_action* action)
{
  glColor3fv((float*)&m_site_color[0]);

  Shared_exact_plane_array exact_coeff_array =
    boost::dynamic_pointer_cast<Exact_plane_array>(m_coeff_array);
  if (exact_coeff_array && (exact_coeff_array->size() > 0)) {
    std::vector<Uint>::iterator it;
    for (it = this->m_site_indices.begin();
         it != this->m_site_indices.end(); ++it)
    {
      Exact_plane_3& plane = (*exact_coeff_array)[*it];
      draw_site(action, plane);
    }
  }
}

/*! \brief draws the arrangement vertices. */
void Power_diagram_on_sphere_geo::draw_aos_vertices(Draw_action* action)
{
  Vos_vertex_const_iterator vi;
  for (vi = m_vos->vertices_begin(); vi != m_vos->vertices_end(); ++vi) {
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    draw_aos_vertex(action, center);
  }
}

/*! \brief draws the arrangement edges. */
void Power_diagram_on_sphere_geo::draw_aos_edges(Draw_action* action)
{
  Vos_edge_const_iterator hei;
  for (hei = m_vos->edges_begin(); hei != m_vos->edges_end(); ++hei) {
    const X_monotone_curve& curve = hei->curve();
    Vector3f src = to_vector3f(curve.source());
    Vector3f trg = to_vector3f(curve.target());
    Vector3f normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg, normal);
  }
}

/*! \brief cleans the renderer */
void Power_diagram_on_sphere_geo::clean_renderer()
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

  if (get_draw_sites()) {
    if (m_draw_haloed)
      m_renderer.push_back(m_inflated_site_renderer,
                           Arrangement_renderer::INFLATED);
    if (get_site_style() == Edge_shape::STRIP) {
      m_renderer.push_back(m_site_renderer, Arrangement_renderer::BACKFACING);
      m_renderer.push_back(m_site_renderer, Arrangement_renderer::FRONTFACING);
    } else {
      m_renderer.push_back(m_site_other_renderer, Arrangement_renderer::OTHER);
    }
  }

  m_renderer_dirty = false;
}

/*! \brief draws the sites. */
void Power_diagram_on_sphere_geo::Inflated_site_renderer::
operator()(Draw_action* action)
{
  Context* context = action->get_context();

  float saved_edge_radius = 0;
  Boolean tube_edge_style = false;
  if (m_geo.get_site_style() == Edge_shape::LINE) {
    context->draw_line_width(m_geo.get_site_line_width() * 8);
  } else {
    saved_edge_radius = m_geo.get_site_radius();
    m_geo.set_site_radius(saved_edge_radius * 2);
    if (m_geo.get_site_style() == Edge_shape::TUBE) {
      tube_edge_style = true;
      m_geo.set_site_style(Edge_shape::STRIP);
    }
  }
  m_geo.draw_sites(action);
  if (m_geo.get_site_style() == Edge_shape::LINE) {
    context->draw_line_width(1);
  } else {
    m_geo.set_site_radius(saved_edge_radius);
    if (tube_edge_style) m_geo.set_site_style(Edge_shape::TUBE);
  }
}

/*! \brief draws the sites. */
void Power_diagram_on_sphere_geo::Site_other_renderer::
operator()(Draw_action* action)
{
  Context* context = action->get_context();
  if (m_geo.get_site_style() == Edge_shape::LINE) {
    context->draw_light_enable(false);
    context->draw_line_width(m_geo.get_site_line_width());
  }
  m_geo.draw_sites(action);
  if (m_geo.get_site_style() == Edge_shape::LINE) {
    context->draw_line_width(1.0f);
    context->draw_light_enable(true);
  }
}

/*! \brief creates the renderers. */
void Power_diagram_on_sphere_geo::create_renderers()
{
  m_edges_renderer = new Pd_edges_renderer(*this);
  m_vertices_renderer = new Pd_vertices_renderer(*this);
  m_colored_edges_renderer = new Pd_colored_edges_renderer(*this);
  m_colored_vertices_renderer = new Pd_colored_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Pd_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Pd_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Pd_ring_colored_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Pd_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Pd_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Pd_inflated_tube_edges_renderer(*this);

  m_inflated_site_renderer = new Inflated_site_renderer(*this);
  m_site_renderer = new Site_renderer(*this);
  m_site_other_renderer = new Site_other_renderer(*this);
}

/*! \brief destroys the renderers. */
void Power_diagram_on_sphere_geo::destroy_renderers()
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
