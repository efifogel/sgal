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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#pragma warning(disable: 4503)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)
#endif

#include <time.h>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"

#include "SCGAL/Spherical_gaussian_map_colored_geo.hpp"
#include "SCGAL/Sgm_color_overlay_traits.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"
#include "SCGAL/compute_planes.hpp"

SGAL_BEGIN_NAMESPACE

const std::string
Spherical_gaussian_map_colored_geo::s_tag("SphericalGaussianMapColored");
Container_proto* Spherical_gaussian_map_colored_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Spherical_gaussian_map_colored_geo,
                    "Spherical_gaussian_map_colored_geo");

//! \brief constructor.
Spherical_gaussian_map_colored_geo::
Spherical_gaussian_map_colored_geo(Boolean proto) :
  Spherical_gaussian_map_base_geo(proto),
  m_owned_sgm(false),
  m_sgm(nullptr),
  m_polyhedron(nullptr),
  m_minkowski_sum(false),
  m_vertices_renderer(nullptr),
  m_colored_vertices_renderer(nullptr),
  m_edges_renderer(nullptr),
  m_colored_edges_renderer(nullptr),
  m_inflated_line_edges_renderer(nullptr),
  m_inflated_strip_edges_renderer(nullptr),
  m_inflated_tube_edges_renderer(nullptr)
{
  if (proto) return;
  create_renderers();
}

//! \brief copy constructor. Not implemented yet!
Spherical_gaussian_map_colored_geo::
Spherical_gaussian_map_colored_geo(const Spherical_gaussian_map_colored_geo& gm)
{ SGAL_assertion(0); }

//! \brief Destructor.
Spherical_gaussian_map_colored_geo::~Spherical_gaussian_map_colored_geo()
{
  clear_sgm();
  m_sgm_nodes.clear();
  if (m_owned_sgm) {
    if (m_sgm) {
      delete m_sgm;
      m_sgm = nullptr;
    }
    m_owned_sgm = false;
  }
}

//! \brief cleans the data structure.
void Spherical_gaussian_map_colored_geo::clean_sgm()
{
  if (!m_sgm) {
    m_sgm = new Sgm;
    SGAL_assertion(m_sgm);
    m_owned_sgm = true;
  }

  auto start_time = clock();
  if (is_convex_hull()) {
    Polyhedron polyhedron;
    convex_hull(polyhedron);
    typedef boost::is_same<Polyhedron::Plane_3, Epec_plane_3>
      Polyhedron_has_plane;

    //! \todo Use an existing kernel.
    Epec_kernel kernel;

    compute_planes(kernel, polyhedron, Polyhedron_has_plane());
    merge_coplanar_facets(kernel, polyhedron, Polyhedron_has_plane());

    CGAL::Arr_polyhedral_sgm_initializer<Sgm, Polyhedron,
                                         Sgm_geo_initializer_visitor>
      initializer(*m_sgm);
    Sgm_geo_initializer_visitor visitor;
    init_polyhedron(polyhedron, initializer, visitor);
    set_colors();
  }
  else if (m_minkowski_sum) {
    auto ni = m_sgm_nodes.begin();
    auto geo1 = *ni++;
    auto geo2 = *ni;
    Sgm_color_overlay_traits<Sgm> sgm_overlay;
    m_sgm->minkowski_sum(*(geo1->get_sgm()), *(geo2->get_sgm()), sgm_overlay);
  }
  else if (m_polyhedron) {
    /*! This is a suspicious option, cause an external object, namely the
     * polyhedron, is altered.
     */
    typedef boost::is_same<Polyhedron::Plane_3, Epec_plane_3>
      Polyhedron_has_plane;

    //! \todo Use an existing kernel.
    Epec_kernel kernel;

    compute_planes(kernel, *m_polyhedron, Polyhedron_has_plane());
    merge_coplanar_facets(kernel, *m_polyhedron, Polyhedron_has_plane());
    CGAL::Arr_polyhedral_sgm_initializer<Sgm, Polyhedron,
                                         Sgm_geo_initializer_visitor>
      initializer(*m_sgm);
    Sgm_geo_initializer_visitor visitor;
    init_polyhedron(*m_polyhedron, initializer, visitor);
    set_colors();
  }
  else {
    CGAL::Arr_polyhedral_sgm_initializer<Sgm, Polyhedron,
                                         Sgm_geo_initializer_visitor>
      initializer(*m_sgm);
    Sgm_geo_initializer_visitor visitor;
    init(initializer, visitor);
    set_colors();
  }
  clock_t end_time = clock();
  m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;

  update_facets();

  m_dirty_sgm = false;
}

//! \brief sets colors of all edges.
void Spherical_gaussian_map_colored_geo::set_colors()
{
  for (auto it = m_sgm->halfedges_begin(); it != m_sgm->halfedges_end(); ++it)
    it->set_color(get_aos_edge_color());
}

//! \brief clears the internal representation and auxiliary data structures.
void Spherical_gaussian_map_colored_geo::clear_sgm()
{
  m_dirty_sgm = true;
  if (m_sgm) m_sgm->clear();
}

//! \brief sets the attributes of this object.
void Spherical_gaussian_map_colored_geo::set_attributes(Element* elem)
{
  Spherical_gaussian_map_base_geo::set_attributes(elem);

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      set_minkowski_sum(true);
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        auto cont = *ci;
        auto sgm =
          boost::dynamic_pointer_cast<Spherical_gaussian_map_colored_geo>(cont);
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

//! \brief
void Spherical_gaussian_map_colored_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Spherical_gaussian_map_base_geo::get_prototype());

  // geometries
  Shared_container_array_handle_function sgm_nodes_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Spherical_gaussian_map_colored_geo::sgm_nodes_handle);
  s_prototype->add_field_info(new MF_shared_container(GEOMETRIES, "geometries",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      sgm_nodes_func));
}

//! \brief
void Spherical_gaussian_map_colored_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief
Container_proto* Spherical_gaussian_map_colored_geo::get_prototype()
{
  if (!s_prototype) Spherical_gaussian_map_colored_geo::init_prototype();
  return s_prototype;
}

//! \brief draws the polyhedron directly from the gaussian map representation.
void Spherical_gaussian_map_colored_geo::draw_primal(Draw_action* action)
{
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

  for (auto vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit) {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;

    glBegin(GL_POLYGON);
    const auto& normal = vit->get_rendered_normal();
    glNormal3fv((float*)&normal);

    auto hec(vit->incident_halfedges());
    auto begin_hec = hec;
    do {
      auto vec = to_vector3f((*hec).face()->point());
      glVertex3fv((float*)&vec);
      ++hec;
    } while (hec != begin_hec);
    glEnd();
  }
  glFrontFace(GL_CCW);
  glColor3f(1.0f, 1.0f, 1.0f);
}

//! \brief
void Spherical_gaussian_map_colored_geo::isect_primary()
{
  for (auto vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit) {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;

    glBegin(GL_POLYGON);
    auto hec(vit->incident_halfedges());
    auto begin_hec = hec;
    do {
      auto vec = to_vector3f((*hec).face()->point());
      glVertex3fv((float*)&vec);
      ++hec;
    } while (hec != begin_hec);
    glEnd();
  }
}

//! \brief prints statistics.
void Spherical_gaussian_map_colored_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (m_dirty_sgm) clean_sgm();

  if (m_convex_hull)
    std::cout << "Convex hull took " << m_time << " seconds.\n";
  else if (m_minkowski_sum)
    std::cout << "Minkowski sum took " << m_time << " seconds.\n";
  else
    std::cout << "Construction took " << m_time << " seconds." << std::endl;
  m_sgm->print_stat();
}

//! \brief draws the arrangement on sphere opaque.
void Spherical_gaussian_map_colored_geo::draw_aos_opaque(Draw_action* action)
{
  auto* context = action->get_context();
  context->draw_cull_face(Gfx::BACK_CULL);
  context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);

  glEnable(GL_STENCIL_TEST);

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
  if (m_aos_edge_style != Edge_shape::TUBE) glDepthRange(.05f, 1);

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

//! \brief draws the arrangement vertices.
void Spherical_gaussian_map_colored_geo::draw_aos_vertices(Draw_action* action)
{
  for (auto vi = m_sgm->vertices_begin(); vi != m_sgm->vertices_end(); ++vi) {
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    if (vi->point().is_no_boundary() || (vi->degree() > 2))
      draw_aos_vertex(action, center);
    else draw_aos_boundary_vertex(action, center);
  }
}

//! \brief draws the arrangement vertices.
void Spherical_gaussian_map_colored_geo::draw_aos_edges(Draw_action* action)
{
  for (auto hei = m_sgm->edges_begin(); hei != m_sgm->edges_end(); ++hei) {
    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg, normal);
  }
}

//! \brief renders the edges with color.
void Spherical_gaussian_map_colored_geo::Colored_edges_renderer::
operator()(Draw_action* action)
{
  for (auto hei = m_geo.m_sgm->edges_begin(); hei != m_geo.m_sgm->edges_end();
       ++hei)
  {
    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    const Vector3f& color = hei->color();
    glColor3fv((float*)&color);
    m_geo.draw_aos_edge(action, src, trg, normal);
  }
}

//! \brief creates the renderers.
void Spherical_gaussian_map_colored_geo::create_renderers()
{
  m_vertices_renderer = new Vertices_renderer(*this);
  m_colored_vertices_renderer = new Colored_vertices_renderer(*this);

  m_edges_renderer = new Edges_renderer(*this);
  m_colored_edges_renderer = new Colored_edges_renderer(*this);

  m_inflated_line_edges_renderer = new Inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer = new Inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer = new Inflated_tube_edges_renderer(*this);
}

//! \brief destroys the renderers.
void Spherical_gaussian_map_colored_geo::destroy_renderers()
{
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;

  if (m_edges_renderer) delete m_edges_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;

  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;
}

//! \brief cleans the renderer.
void Spherical_gaussian_map_colored_geo::clean_renderer()
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

//! \brief sets the source gausian maps of the minkowski sum.
void Spherical_gaussian_map_colored_geo::
insert_sgm(Shared_spherical_gaussian_map_colored_geo sgm)
{
  m_sgm_nodes.push_back(sgm);
  Observer observer(this, get_field_info(GEOMETRIES));
  sgm->register_observer(observer);
  m_dirty_bounding_sphere = true;
}

//! \brief sets an intermediate polyhedron.
void Spherical_gaussian_map_colored_geo::set_polyhedron(Polyhedron* polyhedron)
{
  m_polyhedron = polyhedron;
  clear_sgm();
}

//! \brief obrains a reference to the Gaussian map.
Spherical_gaussian_map_colored* Spherical_gaussian_map_colored_geo::get_sgm()
{
  if (m_dirty_sgm) clean_sgm();
  return m_sgm;
}

//! \brief sets the Gaussian map.
void Spherical_gaussian_map_colored_geo::
set_sgm(Spherical_gaussian_map_colored* sgm)
{
  m_dirty_sgm = false;
  m_dirty_bounding_sphere = true;
  m_sgm = sgm;
}

SGAL_END_NAMESPACE
