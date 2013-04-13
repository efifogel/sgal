// Copyright (c) 2004, 2005, 2006 Israel.
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
// $Id: Spherical_gaussian_map_geo.cpp 14183 2012-11-21 22:58:50Z efif $
// $Revision: 14183 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#pragma warning(disable: 4503)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)
#endif

#include <time.h>
#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"

#include "SCGAL/Spherical_gaussian_map_geo.hpp"
#include "SCGAL/Exact_coord_array.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Spherical_gaussian_map_geo::s_tag = "SphericalGaussianMap";
Container_proto* Spherical_gaussian_map_geo::s_prototype = NULL;

/*! Default values */
REGISTER_TO_FACTORY(Spherical_gaussian_map_geo,
                    "Spherical_gaussian_map_geo");

/*! Constructor. */
Spherical_gaussian_map_geo::Spherical_gaussian_map_geo(Boolean proto) :
  Spherical_gaussian_map_base_geo(proto),
  m_owned_sgm(false),
  m_sgm(NULL),
  m_minkowski_sum(false),
  m_vertices_renderer(NULL),
  m_colored_vertices_renderer(NULL),
  m_edges_renderer(NULL),
  m_colored_edges_renderer(NULL),
  m_inflated_line_edges_renderer(NULL),
  m_inflated_strip_edges_renderer(NULL),
  m_inflated_tube_edges_renderer(NULL)
{
  if (!proto) create_renderers();
}

/*! Copy Constructor. */
Spherical_gaussian_map_geo::
Spherical_gaussian_map_geo(const Spherical_gaussian_map_geo& gm)
{ SGAL_assertion(0); }

/*! Destructor. */
Spherical_gaussian_map_geo::~Spherical_gaussian_map_geo()
{
  m_sgm_nodes.clear();
  if (m_owned_sgm) {
    if (m_sgm) {
      delete m_sgm;
      m_sgm = NULL;
    }
    m_owned_sgm = false;
  }
}

/*! \brief cleans the data structure. */
void Spherical_gaussian_map_geo::clean()
{
  if (!m_sgm) {
    m_sgm = new Sgm;
    SGAL_assertion(m_sgm);
    m_owned_sgm = true;
  }
  if (Mesh_set::is_dirty()) Mesh_set::clean();
  if (m_minkowski_sum) {
    clock_t start_time = clock();
    Sgm_node_iter  ni = m_sgm_nodes.begin();
    Shared_spherical_gaussian_map_geo geo1 = *ni++;
    Shared_spherical_gaussian_map_geo geo2 = *ni;
    m_sgm->minkowski_sum(*(geo1->get_sgm()), *(geo2->get_sgm()));
    clock_t end_time = clock();
    m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  }
  else if (m_coord_array) {
    clock_t start_time = clock();
    Sgm_initializer sgm_initializer(*m_sgm);

    boost::shared_ptr<Exact_coord_array>  exact_coord_array =
      boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array);
    if (exact_coord_array && (exact_coord_array->size() > 0)) {
      // std::cout << "Spherical_gaussian_map_geo::exact" << std::endl;
      sgm_initializer(exact_coord_array->begin(),
                      exact_coord_array->end(),
                      exact_coord_array->size(),
                      m_coord_indices.begin(), m_coord_indices.end(),
                      m_num_primitives);
    }
    else {
      // std::cout << "Spherical_gaussian_map_geo::inexact" << std::endl;
      sgm_initializer(m_coord_array->begin(), m_coord_array->end(),
                      m_coord_array->size(),
                      m_coord_indices.begin(), m_coord_indices.end(),
                      m_num_primitives);
    }
    clock_t end_time = clock();
    m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  }
  update_facets();
}

/*! \brief clears the internal representation and auxiliary data structures. */
void Spherical_gaussian_map_geo::clear()
{
  Spherical_gaussian_map_base_geo::clear();
  if (m_sgm) m_sgm->clear();
}

/*! \brief */
void Spherical_gaussian_map_geo::cull(Cull_context& cull_context) {}

/*! \brief */
void Spherical_gaussian_map_geo::isect(Isect_action* action)
{
  Context* context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);
  isect_primary();
  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

/*! \brief sets the attributes of the object extracted from an input file. */
void Spherical_gaussian_map_geo::set_attributes(Element* elem)
{
  Spherical_gaussian_map_base_geo::set_attributes(elem);

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      set_minkowski_sum(true);
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        Shared_spherical_gaussian_map_geo sgm =
          boost::dynamic_pointer_cast<Spherical_gaussian_map_geo>(cont);
        if (sgm) insert_sgm(sgm);
        else {
          std::cerr << "Invalid " << s_tag << " geometry nodes!"
                    << std::endl;
        }
      }
      elem->mark_delete(mcai);
    }
    continue;
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief */
void Spherical_gaussian_map_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Spherical_gaussian_map_base_geo::get_prototype());
  MF_container* field = new MF_container(GEOMETRIES, "geometries",
                                         get_member_offset(&m_sgm_nodes));
  s_prototype->add_field_info(field);
}

/*! \brief */
void Spherical_gaussian_map_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Spherical_gaussian_map_geo::get_prototype() 
{  
  if (!s_prototype) Spherical_gaussian_map_geo::init_prototype();
  return s_prototype;
}

/*! \brief draws the polyhedron directly from the gaussian map representation.
 */
void Spherical_gaussian_map_geo::draw_primal(Draw_action* action)
{
  SGAL_TRACE_MSG(Trace::GAUSSIAN_MAP, "draw_primal()\n");

#if 0
  {
    std::cout << "Faces:" << std::endl;
    Sgm_face_const_iterator fit;
    for (fit = m_sgm->faces_begin(); fit != m_sgm->faces_end(); ++fit) {
      std::cout << fit->point() << std::endl;

      std::cout << "  Outer CCB:" << std::endl;
      Sgm::Outer_ccb_const_iterator oit;
      for (oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end(); ++oit) {
        Sgm::Halfedge_const_iterator first = *oit;
        Sgm::Halfedge_const_iterator curr = first;
        do {
          std::cout << "  " << curr->curve() << std::endl;
          curr = curr->next();
        } while (curr != first);
      }

      std::cout << "  Inner CCB:" << std::endl;
      Sgm::Inner_ccb_const_iterator iit;
      for (iit = fit->inner_ccbs_begin(); iit != fit->inner_ccbs_end(); ++iit) {
        Sgm::Halfedge_const_iterator first = *iit;
        Sgm::Halfedge_const_iterator curr = first;
        do {
          std::cout << "  " << curr->curve() << std::endl;
          curr = curr->next();
        } while (curr != first);
      }
    }
    std::cout << std::endl;
  }
#endif
    
  glFrontFace((is_ccw()) ? GL_CW : GL_CCW);

  Sgm_vertex_const_iterator vit;
  for (vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit) {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;
    glBegin(GL_POLYGON);
    const Vector3f& normal = vit->get_rendered_normal();
    glNormal3fv((float*)&normal);

    Sgm_halfedge_around_vertex_const_circulator hec(vit->incident_halfedges());
    Sgm_halfedge_around_vertex_const_circulator begin_hec = hec;
    do {
      Vector3f vec = to_vector3f((*hec).face()->point());
      glVertex3fv((float*)&vec);
      ++hec;
    } while (hec != begin_hec);
    glEnd();
  }
  glFrontFace(GL_CCW);
  glColor3f(1.0f, 1.0f, 1.0f);
}

/*! \brief */
void Spherical_gaussian_map_geo::isect_primary()
{
  Sgm_vertex_const_iterator vit;
  for (vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit) {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;

    glBegin(GL_POLYGON);
    Sgm_halfedge_around_vertex_const_circulator hec(vit->incident_halfedges());
    Sgm_halfedge_around_vertex_const_circulator begin_hec = hec;
    do {
      Vector3f vec = to_vector3f((*hec).face()->point());
      glVertex3fv((float*)&vec);
      ++hec;
    } while (hec != begin_hec);
    glEnd();
  }
}

/*! \brief prints statistics. */
void Spherical_gaussian_map_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty()) clean();

  if (m_minkowski_sum)
    std::cout << "Minkowski sum took " << m_time << " seconds.\n";
  else
    std::cout << "Construction took " << m_time << " seconds." << std::endl;
  m_sgm->print_stat();
}

/*! \brief draws the arrangement on sphere opaque */
void Spherical_gaussian_map_geo::draw_aos_opaque(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_cull_face(Gfx::BACK_CULL);
  context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
  
  // Draw the sphere:
  if (m_draw_aos_surface) {
    glStencilFunc(GL_EQUAL, 0, 0xFFFFFFFFL);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glColor3fv((float*)&m_aos_surface_color);
    context->draw_light_model_sides(Gfx::ONE_SIDE);
    if ((m_aos_vertex_style != Vertex_shape::BALL) ||
        (m_aos_edge_style != Edge_shape::TUBE)) {
      float min_range = 0;
      if (m_aos_vertex_style != Vertex_shape::BALL) min_range += .05f;
      if (m_aos_edge_style != Edge_shape::TUBE) min_range += .05f;
      glDepthRange(min_range, 1);
    }
    m_aos_sphere.draw(action);
    if ((m_aos_vertex_style != Vertex_shape::BALL) ||
        (m_aos_edge_style != Edge_shape::TUBE))
      glDepthRange(0, 1);
  }
  glDisable(GL_STENCIL_TEST);  

  // Draw the edges:
  glColor3fv((float*)&m_aos_edge_colors[0][0]);
  if (m_aos_edge_style == Edge_shape::LINE) {
    context->draw_light_enable(false);
    context->draw_line_width(m_aos_edge_line_width);
  }
  if (m_aos_edge_style != Edge_shape::TUBE) glDepthRange(0.05f, 1);
  
  (*m_colored_edges_renderer)(action);
  if (m_aos_edge_style == Edge_shape::LINE) {
    context->draw_line_width(1.0f);
    context->draw_light_enable(true);  
  }
  if (m_aos_edge_style != Edge_shape::TUBE) glDepthRange(0, 1);
  
  // Draw the vertices:
  glColor3fv((float*)&m_aos_vertex_color);
  if (m_aos_vertex_style == Vertex_shape::POINT) {
    context->draw_light_enable(false);  
    context->draw_point_size(m_aos_vertex_point_size);
    glIsEnabled(GL_POINT_SMOOTH);
  }
  (*m_colored_vertices_renderer)(action);
  if (m_aos_vertex_style == Vertex_shape::POINT) {
    context->draw_point_size(1.0f);
    context->draw_light_enable(true);  
  }

  context->draw_cull_face(Gfx::NO_CULL);
  context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/*! \brief draws the arrangement vertices */
void Spherical_gaussian_map_geo::draw_aos_vertices(Draw_action* action)
{
  Sgm_vertex_const_iterator vi;
  for (vi = m_sgm->vertices_begin(); vi != m_sgm->vertices_end(); ++vi) {
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    if (vi->point().is_no_boundary()) draw_aos_vertex(action, center);
    else draw_aos_boundary_vertex(action, center);
  }
}

/*! \brief draws the arrangement on sphere edges */
void Spherical_gaussian_map_geo::draw_aos_edges(Draw_action* action)
{
  Sgm_edge_const_iterator hei;
  for (hei = m_sgm->edges_begin(); hei != m_sgm->edges_end(); ++hei) {
    const Sgm_x_monotone_curve_2& curve = hei->curve();
    Vector3f src = to_vector3f(curve.source());
    Vector3f trg = to_vector3f(curve.target());
    Vector3f normal = to_vector3f(curve.target());
    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg, normal);
  }
}

/*! \brief creates the renderers. */
void Spherical_gaussian_map_geo::create_renderers()
{
  m_vertices_renderer = new Vertices_renderer(*this);
  m_colored_vertices_renderer = new Colored_vertices_renderer(*this);

  m_edges_renderer = new Edges_renderer(*this);
  m_colored_edges_renderer = new Colored_edges_renderer(*this);
  
  m_inflated_line_edges_renderer = new Inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer = new Inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer = new Inflated_tube_edges_renderer(*this);
}

/*! \brief destroys the renderers. */
void Spherical_gaussian_map_geo::destroy_renderers()
{
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;

  if (m_edges_renderer) delete m_edges_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;

  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;
}

/*! \brief cleans the renderer. */
void Spherical_gaussian_map_geo::clean_renderer()
{
  Spherical_gaussian_map_base_geo::clean_renderer();
  
  if (m_draw_aos_haloed) {
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

  // Edges:
  switch (get_aos_edge_style()) {
   case Edge_shape::STRIP:
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::FRONTFACING);
    break;

   case Edge_shape::LINE:
   case Edge_shape::TUBE:
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::OTHER);
    break;

   default: break;
  }

  // Vertices:
  switch (get_aos_vertex_style()) {
   case Vertex_shape::DISC:
    m_renderer.push_back(m_colored_vertices_renderer,
                      Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_vertices_renderer,
                      Arrangement_renderer::FRONTFACING);
    break;

   case Vertex_shape::POINT:
   case Vertex_shape::RING:
   case Vertex_shape::BALL:
    m_renderer.push_back(m_colored_vertices_renderer,
                      Arrangement_renderer::OTHER);
    break;

   default: break;
  }

  // Depth:  
  if (is_aos_edge_enabled())
    m_renderer.push_back(m_edges_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_vertices_renderer, Arrangement_renderer::DEPTH);
}

/*! \brief sets the source gausian maps of the minkowski sum. */
void Spherical_gaussian_map_geo::
insert_sgm(Shared_spherical_gaussian_map_geo sgm)
{
  m_sgm_nodes.push_back(sgm);
  Observer observer(this, get_field_info(GEOMETRIES));
  sgm->register_observer(observer);
  m_dirty_sphere_bound = true;
}

/*! \brief obrains the Gaussian map. */
Spherical_gaussian_map* Spherical_gaussian_map_geo::get_sgm()
{
  if (m_dirty) clean();
  return m_sgm;
}

/*! \brief sets the Gaussian map. */
void Spherical_gaussian_map_geo::set_sgm(Spherical_gaussian_map* sgm)
{
  m_dirty = false;
  m_sgm = sgm;
}
  
SGAL_END_NAMESPACE
