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
#include "SGAL/Rotation.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Cylinder.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Utilities.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Spherical_gaussian_map_marked_geo.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"
#include "SCGAL/compute_planes.hpp"

SGAL_BEGIN_NAMESPACE

const std::string
Spherical_gaussian_map_marked_geo::s_tag("SphericalGaussianMapMarked");
Container_proto* Spherical_gaussian_map_marked_geo::s_prototype(nullptr);

// Default values

// Default marked vertex attributes:
const Spherical_gaussian_map_marked_geo::Vertex_style
  Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_vertex_style(Vertex_shape::DISC);
const Float Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_vertex_radius(.06f);
const Float Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_vertex_point_size(1);

// Default aos marked edge attributes:
const Boolean Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_edge_enabled(true);
const Spherical_gaussian_map_marked_geo::Edge_style
  Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_edge_style(Edge_shape::TUBE);
const Uint Spherical_gaussian_map_marked_geo::s_def_aos_marked_edge_count(1);
const Boolean Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_edge_directed(false);
const Float Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_edge_radius(.03f);
const Float Spherical_gaussian_map_marked_geo::
  s_def_aos_marked_edge_line_width(1);

// Default aos marked face attributes:
const Float Spherical_gaussian_map_marked_geo::
s_def_aos_marked_face_transparency(.4f);

// Feature attributes:
const bool Spherical_gaussian_map_marked_geo::s_def_draw_marked_vertex(false);
const bool Spherical_gaussian_map_marked_geo::s_def_draw_marked_edge(false);
const bool Spherical_gaussian_map_marked_geo::s_def_draw_marked_facet(false);

const Float Spherical_gaussian_map_marked_geo::s_def_marked_vertex_radius(0.1f);
const Float Spherical_gaussian_map_marked_geo::s_def_marked_edge_radius(0.1f);

const Vector3f Spherical_gaussian_map_marked_geo::
s_def_marked_vertex_color(0.5f, 0, 0);
const Vector3f Spherical_gaussian_map_marked_geo::
s_def_marked_edge_color(0, 0.5f, 0);
const Vector3f Spherical_gaussian_map_marked_geo::
s_def_marked_facet_color(0, 0, 0.5f);

REGISTER_TO_FACTORY(Spherical_gaussian_map_marked_geo,
                    "Spherical_gaussian_map_marked_geo");

//! \brief constructor.
Spherical_gaussian_map_marked_geo::
Spherical_gaussian_map_marked_geo(Boolean proto) :
  Spherical_gaussian_map_base_geo(proto),
  m_owned_sgm(false),
  m_sgm(nullptr),
  m_minkowski_sum(false),
  m_aos_marked_vertex_style(s_def_aos_marked_vertex_style),
  m_aos_marked_vertex_radius(s_def_aos_marked_vertex_radius),
  m_aos_marked_vertex_point_size(s_def_aos_marked_vertex_point_size),
  m_aos_marked_edge_enabled(s_def_aos_marked_edge_enabled),
  m_aos_marked_edge_style(s_def_aos_marked_edge_style),
  m_aos_marked_edge_count(s_def_aos_marked_edge_count),
  m_aos_marked_edge_directed(s_def_aos_marked_edge_directed),
  m_aos_marked_edge_radius(s_def_aos_marked_edge_radius),
  m_aos_marked_edge_line_width(s_def_aos_marked_edge_line_width),
  m_aos_marked_face_transparency(s_def_aos_marked_face_transparency),
  m_draw_marked_vertex(s_def_draw_marked_vertex),
  m_marked_vertex_index(0),
  m_marked_vertex_radius(s_def_marked_vertex_radius),
  m_marked_vertex_color(s_def_marked_vertex_color),

  m_draw_marked_edge(s_def_draw_marked_edge),
  m_marked_edge_index(0),
  m_marked_edge_radius(s_def_marked_edge_radius),
  m_marked_edge_color(s_def_marked_edge_color),

  m_draw_marked_facet(s_def_draw_marked_facet),
  m_marked_facet_color(s_def_marked_facet_color),
  m_marked_facet_index(0),

  m_vertices_renderer(nullptr),
  m_colored_vertices_renderer(nullptr),
  m_edges_renderer(nullptr),
  m_colored_edges_renderer(nullptr),
  m_inflated_line_edges_renderer(nullptr),
  m_inflated_strip_edges_renderer(nullptr),
  m_inflated_tube_edges_renderer(nullptr),

  m_marked_vertices_renderer(nullptr),
  m_colored_marked_vertices_renderer(nullptr),
  m_marked_edges_renderer(nullptr),
  m_colored_marked_edges_renderer(nullptr)
{
  if (proto) return;
  create_renderers();
}

//! \brief copy Constructor.
Spherical_gaussian_map_marked_geo::
Spherical_gaussian_map_marked_geo(const Spherical_gaussian_map_marked_geo& gm)
{
  // Not implemented yet!
  SGAL_assertion(0);
}

//! \brief destructor.
Spherical_gaussian_map_marked_geo::~Spherical_gaussian_map_marked_geo()
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
void Spherical_gaussian_map_marked_geo::clean_sgm()
{
  if (!m_sgm) {
    m_sgm = new Sgm;
    SGAL_assertion(m_sgm);
    m_owned_sgm = true;
  }

  clock_t start_time = clock();
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
  }
  else if (m_minkowski_sum) {
    auto ni = m_sgm_nodes.begin();
    auto geo1 = *ni++;
    auto geo2 = *ni;
    m_sgm->minkowski_sum(*(geo1->get_sgm()), *(geo2->get_sgm()));
  }
  // else if (m_polyhedron) {
  //   /*! This is a suspicious option, cause an external object, namely the
  //    * polyhedron, is altered.
  //    */
  //   typedef boost::is_same<Polyhedron::Plane_3, Epec_plane_3>
  //     Polyhedron_has_plane;

  //   //! \todo Use an existing kernel.
  //   Epec_kernel kernel;

  //   compute_planes(kernel, *m_polyhedron, Polyhedron_has_plane());
  //   merge_coplanar_facets(kernel, *m_polyhedron, Polyhedron_has_plane());
  //   CGAL::Arr_polyhedral_sgm_initializer<Sgm, Polyhedron,
  //                                        Sgm_geo_initializer_visitor>
  //     initializer(*m_sgm);
  //   initializer.set_marked_vertex_index(m_marked_vertex_index);
  //   initializer.set_marked_edge_index(m_marked_edge_index);
  //   initializer.set_marked_facet_index(m_marked_facet_index);
  //   Sgm_geo_initializer_visitor visitor;
  //   init_polyhedron(*m_polyhedron, initializer, visitor);
  // }
  else {
    CGAL::Arr_polyhedral_sgm_initializer<Sgm, Polyhedron,
                                         Sgm_geo_initializer_visitor>
      initializer(*m_sgm);
    initializer.set_marked_vertex_index(m_marked_vertex_index);
    initializer.set_marked_edge_index(m_marked_edge_index);
    initializer.set_marked_facet_index(m_marked_facet_index);
    Sgm_geo_initializer_visitor visitor;
    init(initializer, visitor);
  }
  clock_t end_time = clock();
  m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;

  update_facets();

#if 0
  // Mark one vertex in the corresponding aos faces
  m_sgm.reset_faces();                // reset the "visited" flag
  Arrangement& arr = m_sgm.arrangement(0);
  Sgm_face_iterator fi = arr.faces_begin();
  m_sgm.process_boundary_faces(fi, Face_set_marked_op());
#endif

  auto num_vertices = m_sgm->number_of_vertices();
  auto num_edges = m_sgm->number_of_edges();
  auto num_facets = m_sgm->number_of_facets();
  if (m_marked_vertex_index >= num_vertices)
    m_marked_vertex_index = num_vertices;
  if (m_marked_edge_index >= num_edges) m_marked_edge_index = num_edges;
  if (m_marked_facet_index >= num_facets) m_marked_facet_index = num_facets;

  m_dirty_sgm = false;
}

//! \brief clears the internal representation and auxiliary data structures.
void Spherical_gaussian_map_marked_geo::clear_sgm()
{
  m_dirty_sgm = true;
  if (m_sgm) m_sgm->clear();
}

//! \brief sets the attributes of the object extracted from an input file.
void Spherical_gaussian_map_marked_geo::set_attributes(Element* elem)
{
  Spherical_gaussian_map_base_geo::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    // AOS marked vertex attributes:
    if (name == "aosMarkedVertexStyle") {
      m_aos_marked_vertex_style = Vertex_shape::style(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedVertexRadius") {
      m_aos_marked_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedPointSize") {
      m_aos_marked_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    // AOS marked edge attributes:
    if (name == "aosMarkedEdgeEnabled") {
      m_aos_marked_edge_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
     if (name == "aosMarkedEdgeStyle") {
      m_aos_marked_edge_style = Edge_shape::style(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeCount") {
      m_aos_marked_edge_count = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeDirected") {
      m_aos_marked_edge_directed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeRadius") {
      m_aos_marked_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosMarkedEdgeLineWidth") {
      m_aos_marked_edge_line_width = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosMarkedFaceTransparency") {
      m_aos_marked_face_transparency = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "drawMarkedVertex") {
      m_draw_marked_vertex = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawMarkedEdge") {
      m_draw_marked_edge = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawMarkedFacet") {
      m_draw_marked_facet = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "markedVertexRadius") {
      m_marked_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedEdgeRadius") {
      m_marked_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedVertexColor") {
      Vector3f col(value);
      set_marked_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedEdgeColor") {
      Vector3f col(value);
      set_marked_edge_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedFacetColor") {
      Vector3f col(value);
      set_marked_facet_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedVertexIndex") {
      set_marked_vertex_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedEdgeIndex") {
      set_marked_edge_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "markedFacetIndex") {
      set_marked_facet_index(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); mcai++)
  {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      set_minkowski_sum(true);
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        Shared_spherical_gaussian_map_marked_geo sgm =
          boost::dynamic_pointer_cast<Spherical_gaussian_map_marked_geo>(cont);
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
void Spherical_gaussian_map_marked_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Spherical_gaussian_map_base_geo::get_prototype());

  // Add the object fields to the prototype
  Shared_container_array_handle_function sgm_nodes_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Spherical_gaussian_map_marked_geo::sgm_nodes_handle);
  s_prototype->add_field_info(new MF_shared_container(GEOMETRIES, "geometries",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      sgm_nodes_func));

  Execution_function exec_func =
    static_cast<Execution_function>
    (&Spherical_gaussian_map_marked_geo::increase_vertex_index);
  Boolean_handle_function increase_vertex_index_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_marked_geo::increase_vertex_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_VERTEX_INDEX,
                                          "increaseVertexIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          increase_vertex_index_func,
                                          exec_func));

  exec_func = static_cast<Execution_function>
    (&Spherical_gaussian_map_marked_geo::increase_edge_index);
  Boolean_handle_function increase_edge_index_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_marked_geo::increase_edge_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_EDGE_INDEX,
                                          "increaseEdgeIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          increase_edge_index_func,
                                          exec_func));

  exec_func = static_cast<Execution_function>
    (&Spherical_gaussian_map_marked_geo::increase_facet_index);
  Boolean_handle_function increase_facet_index_func =
    static_cast<Boolean_handle_function>
    (&Spherical_gaussian_map_marked_geo::increase_facet_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_FACET_INDEX,
                                          "increaseFacetIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          increase_facet_index_func,
                                          exec_func));
}

//! \brief
void Spherical_gaussian_map_marked_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief
Container_proto* Spherical_gaussian_map_marked_geo::get_prototype()
{
  if (!s_prototype) Spherical_gaussian_map_marked_geo::init_prototype();
  return s_prototype;
}

//! \brief draws the polyhedron directly from the gaussian map representation.
void Spherical_gaussian_map_marked_geo::draw_primal(Draw_action* action)
{
#if 0
  {
    std::cout << "Faces:" << std::endl;
    for (auto fit = m_sgm->faces_begin(); fit != m_sgm->faces_end(); ++fit) {
      std::cout << fit->point() << std::endl;

      std::cout << "  Outer CCB:" << std::endl;
      for (auto oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end();
           ++oit)
      {
        Sgm::Halfedge_const_iterator first = *oit;
        Sgm::Halfedge_const_iterator curr = first;
        do {
          std::cout << "  " << curr->curve() << std::endl;
          curr = curr->next();
        } while (curr != first);
      }

      std::cout << "  Inner CCB:" << std::endl;
      for (auto iit = fit->inner_ccbs_begin(); iit != fit->inner_ccbs_end();
           ++iit)
      {
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

  Context* context = action->get_context();
  glColor3fv((float*)&m_marked_facet_color);

  glFrontFace((is_ccw()) ? GL_CW : GL_CCW);

  for (auto vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end();
       ++vit)
  {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;

    if (m_draw_marked_facet && vit->marked())
      context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);

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

    if (m_draw_marked_facet && vit->marked())
      context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
  }
  glFrontFace(GL_CCW);
  glColor3f(1.0f, 1.0f, 1.0f);

  if (m_draw_marked_vertex) draw_primal_marked_vertex(action);
  if (m_draw_marked_edge) draw_primal_marked_edge(action);
}

//! \brief draws the primal marked vertex.
void Spherical_gaussian_map_marked_geo::
draw_primal_marked_vertex(Draw_action* action)
{
  Sphere vertex_geom;
  vertex_geom.set_radius(m_marked_vertex_radius);

  auto* context = action->get_context();

  for (auto fi = m_sgm->faces_begin(); fi != m_sgm->faces_end(); ++fi) {
    if (!fi->marked()) continue;

    auto vec = to_vector3f(fi->point());

    glPushMatrix();
    glTranslatef(vec[0], vec[1], vec[2]);
    glColor3fv((float*)&m_marked_vertex_color);
    context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
    vertex_geom.draw(action);
    context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
    glPopMatrix();
    return;
  }
}

//! \brief draws the primal marked edge.
void Spherical_gaussian_map_marked_geo::
draw_primal_marked_edge(Draw_action* action)
{
  Cylinder edge_geom;
  edge_geom.set_radius(m_marked_edge_radius);

  Context* context = action->get_context();

  for (auto hi = m_sgm->halfedges_begin(); hi != m_sgm->halfedges_end(); ++hi) {
    if (!hi->marked()) continue;

    auto face1 = hi->face();
    auto face2 = hi->twin()->face();

    Vector3f vec1 = to_vector3f(face1->point());
    Vector3f vec2 = to_vector3f(face2->point());

    Vector3f dst;
    dst.sub(vec2, vec1);
    Vector3f src(0,1,0);
    Rotation rot;
    rot.make(src, dst);

    Vector3f translation;
    translation.add(vec1, vec2);
    translation.scale(0.5f);

    float dist = vec1.distance(vec2);
    edge_geom.set_height(dist);

    Transform trans;
    trans.set_translation(translation);
    trans.set_rotation(rot);
    const auto& mat = trans.get_matrix();

    glPushMatrix();
    glMultMatrixf((float*)&mat);
    glColor3fv((float*)&m_marked_edge_color);
    context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
    edge_geom.draw(action);
    context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
    glPopMatrix();
    return;
  }
}

//! \brief
void Spherical_gaussian_map_marked_geo::isect_primary()
{
  for (auto vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit)
  {
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

//! \brief prints statistics.
void Spherical_gaussian_map_marked_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (m_dirty_sgm) clean_sgm();

  if (m_minkowski_sum)
    std::cout << "Minkowski sum took " << m_time << " seconds.\n";
  else
    std::cout << "Construction took " << m_time << " seconds." << std::endl;
  m_sgm->print_stat();
}

//! \brief draws the arrangement on sphere opaque.
void Spherical_gaussian_map_marked_geo::draw_aos_opaque(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_cull_face(Gfx::BACK_CULL);
  context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);

  // Draw marked (primal) vertex:
  glEnable(GL_STENCIL_TEST);
  if (m_draw_marked_vertex) {
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFFL);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glColor3fv((float*)&m_marked_vertex_color);
    draw_aos_marked_face(action);
  }

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

  // Draw the marked edges:
  if (m_draw_marked_edge) {
    glColor3fv((float*)&m_marked_edge_color);
    if (m_aos_marked_edge_style == Edge_shape::LINE) {
      context->draw_light_enable(false);
      context->draw_line_width(m_aos_marked_edge_line_width);
    }
    if (m_aos_marked_edge_style != Edge_shape::TUBE)
      glDepthRange(0.05f, 1);

    (*m_colored_marked_edges_renderer)(action);
    if (m_aos_marked_edge_style == Edge_shape::LINE) {
      context->draw_line_width(1.0f);
      context->draw_light_enable(true);
    }
    if (m_aos_marked_edge_style != Edge_shape::TUBE)
      glDepthRange(0, 1);
  }

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

  // Draw the marked vertices:
  if (m_draw_marked_facet) {
    glColor3fv((float*)&m_marked_facet_color);
    if (m_aos_marked_vertex_style == Vertex_shape::POINT) {
      context->draw_light_enable(false);
      context->draw_point_size(m_aos_marked_vertex_point_size);
      glIsEnabled(GL_POINT_SMOOTH);
    }
    (*m_colored_marked_vertices_renderer)(action);
    if (m_aos_marked_vertex_style == Vertex_shape::POINT) {
      context->draw_point_size(1.0f);
      context->draw_light_enable(true);
    }
  }

  context->draw_cull_face(Gfx::NO_CULL);
  context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//! \brief draws the marked (primal) vertex.
void Spherical_gaussian_map_marked_geo::
draw_aos_marked_face(Draw_action* action)
{
  Sgm_face_const_iterator fi;
  for (fi = m_sgm->faces_begin(); fi != m_sgm->faces_end(); ++fi) {
    if (!fi->marked()) continue;

    // Collect the points on the boundary:
    std::list<Vector3f> points;

    // Only one outer ccb or one inner ccb exclusively:
    Sgm_halfedge_const_iterator first;
    if (fi->number_of_outer_ccbs() == 1) {
      Sgm::Outer_ccb_const_iterator oit = fi->outer_ccbs_begin();
      first = *oit;
    }
    else {
      SGAL_assertion(fi->number_of_inner_ccbs() == 1);
      Sgm::Inner_ccb_const_iterator iit = fi->inner_ccbs_begin();
      first = *iit;
    }

    Sgm_halfedge_const_iterator curr = first;
    do {
      Vector3f vec = to_vector3f(curr->target()->point());
      vec.normalize();
      points.push_back(vec);
      curr = curr->next();
    } while (curr != first);
    SGAL_assertion(points.size() >= 3);
    draw_aos_convex_face(action, points.begin(), points.end());
    points.clear();
  }
}

//! \brief draws the arrangement vertices.
void Spherical_gaussian_map_marked_geo::draw_aos_vertices(Draw_action* action)
{
  for (auto vi = m_sgm->vertices_begin(); vi != m_sgm->vertices_end(); ++vi) {
    if (m_draw_marked_facet && vi->marked()) continue;
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    if (vi->point().is_no_boundary() || (vi->degree() > 2))
      draw_aos_vertex(action, center);
    else draw_aos_boundary_vertex(action, center);
  }
}

//! \brief draws the arrangement vertices.
void Spherical_gaussian_map_marked_geo::draw_aos_edges(Draw_action* action)
{
  for (auto hei = m_sgm->edges_begin(); hei != m_sgm->edges_end(); ++hei) {
    if (get_draw_marked_edge() && hei->marked()) continue;
    const X_monotone_curve_2& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg, normal);
  }
}

//! \biref increases the vertex index.
void Spherical_gaussian_map_marked_geo::
increase_vertex_index(const Field_info* /* field_info */)
{
  m_marked_vertex_index++;
  if (m_marked_vertex_index == m_sgm->number_of_vertices())
    m_marked_vertex_index = 0;
  clear_sgm();
}

//! \biref increases the face index.
void Spherical_gaussian_map_marked_geo::
increase_edge_index(const Field_info* /* field_info */)
{
  m_marked_edge_index++;
  if (m_marked_edge_index == m_sgm->number_of_edges())
    m_marked_edge_index = 0;
  clear_sgm();
}

//! \biref increases the face index.
void Spherical_gaussian_map_marked_geo::
increase_facet_index(const Field_info* field_info)
{
  m_marked_facet_index++;
  if (m_marked_facet_index == m_sgm->number_of_facets())
    m_marked_facet_index = 0;
  clear_sgm();
}

//! \brief renders the marked_vertices with color.
void Spherical_gaussian_map_marked_geo::Marked_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_facet_color);
  for (auto vi = m_geo.m_sgm->vertices_begin();
       vi != m_geo.m_sgm->vertices_end(); ++vi)
  {
    if (!vi->marked()) continue;
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    m_geo.draw_aos_marked_vertex(action, center);
  }
}

//! \brief renders the marked_vertices with color.
void Spherical_gaussian_map_marked_geo::Colored_marked_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_facet_color);
  Marked_vertices_renderer::operator()(action);
}

//! \brief renders the edges with color.
void Spherical_gaussian_map_marked_geo::Colored_edges_renderer::
operator()(Draw_action* action)
{
  for (auto hei = m_geo.m_sgm->edges_begin(); hei != m_geo.m_sgm->edges_end();
       ++hei)
  {
    if (m_geo.get_draw_marked_edge() && hei->marked()) continue;

    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    if (m_geo.m_minkowski_sum) {
      Uint num = 0;
      Vector3f color;
      for (Uint id = 0; id < 2; ++id) {
        if (hei->is_arr(id)) {
          color.add(m_geo.m_aos_edge_colors[id]);
          ++num;
        }
      }
      SGAL_assertion(num != 0);
      color.scale(1.0f / num);
      glColor3fv((float*) &color);
    }
    else glColor3fv((float*) &m_geo.m_aos_edge_colors[0]);
    m_geo.draw_aos_edge(action, src, trg, normal);
  }
}

//! \brief renders the marked_edges.
void Spherical_gaussian_map_marked_geo::Marked_edges_renderer::
operator()(Draw_action* action)
{
  for (auto hei = m_geo.m_sgm->edges_begin(); hei != m_geo.m_sgm->edges_end();
       ++hei)
  {
    if (!hei->marked()) continue;
    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();
    m_geo.draw_aos_marked_edge(action, src, trg, normal);
  }
}

//! \brief renders the marked_edges with color.
void Spherical_gaussian_map_marked_geo::Colored_marked_edges_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_edge_color);
  Marked_edges_renderer::operator()(action);
}

//! \brief renders the marked primal vertex.
void Spherical_gaussian_map_marked_geo::Marked_face_renderer::
operator()(Draw_action* action)
{
  const Vector3f& color = m_geo.get_marked_vertex_color();
  glColor4f(color[0], color[1], color[2],
            1 - m_geo.get_aos_marked_face_transparency());
  m_geo.draw_aos_marked_face(action);
}

//! \brief creates the renderers.
void Spherical_gaussian_map_marked_geo::create_renderers()
{
  m_vertices_renderer = new Vertices_renderer(*this);
  m_colored_vertices_renderer = new Colored_vertices_renderer(*this);

  m_edges_renderer = new Edges_renderer(*this);
  m_colored_edges_renderer = new Colored_edges_renderer(*this);

  m_inflated_line_edges_renderer = new Inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer = new Inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer = new Inflated_tube_edges_renderer(*this);

  m_marked_face_renderer = new Marked_face_renderer(*this);

  m_marked_vertices_renderer = new Marked_vertices_renderer(*this);
  m_colored_marked_vertices_renderer =
    new Colored_marked_vertices_renderer(*this);

  m_marked_edges_renderer = new Marked_edges_renderer(*this);
  m_colored_marked_edges_renderer = new Colored_marked_edges_renderer(*this);
}

//! \brief destroys the renderers.
void Spherical_gaussian_map_marked_geo::destroy_renderers()
{
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;

  if (m_edges_renderer) delete m_edges_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;

  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;

  if (m_marked_face_renderer) delete m_marked_face_renderer;
  if (m_marked_vertices_renderer) delete m_marked_vertices_renderer;
  if (m_colored_marked_vertices_renderer)
    delete m_colored_marked_vertices_renderer;

  if (m_marked_edges_renderer) delete m_marked_edges_renderer;
  if (m_colored_marked_edges_renderer)
    delete m_colored_marked_edges_renderer;
}

//! \brief cleans the renderer.
void Spherical_gaussian_map_marked_geo::clean_renderer()
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

  // Surface and faces:
  if (m_draw_marked_vertex)
    m_renderer.push_front(m_marked_face_renderer,
                         Arrangement_renderer::SURFACE);

  // Marked edges:
  if (m_draw_marked_edge) {
    switch (get_aos_marked_edge_style()) {
     case Edge_shape::STRIP:
      m_renderer.push_back(m_colored_marked_edges_renderer,
                           Arrangement_renderer::BACKFACING);
      m_renderer.push_back(m_colored_marked_edges_renderer,
                           Arrangement_renderer::FRONTFACING);
      break;

     case Edge_shape::LINE:
     case Edge_shape::TUBE:
      m_renderer.push_back(m_colored_marked_edges_renderer,
                           Arrangement_renderer::OTHER);
      break;

     default: break;
    }
  }

  // Marked vertices:
  if (m_draw_marked_facet) {
    switch (get_aos_marked_vertex_style()) {
     case Vertex_shape::DISC:
      m_renderer.push_back(m_colored_marked_vertices_renderer,
                           Arrangement_renderer::BACKFACING);
      m_renderer.push_back(m_colored_marked_vertices_renderer,
                           Arrangement_renderer::FRONTFACING);
      break;

     case Vertex_shape::POINT:
     case Vertex_shape::RING:
     case Vertex_shape::BALL:
      m_renderer.push_back(m_colored_marked_vertices_renderer,
                           Arrangement_renderer::OTHER);
      break;

     default: break;
    }
  }

  // Depth:
  if (is_aos_edge_enabled())
    m_renderer.push_back(m_edges_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_vertices_renderer, Arrangement_renderer::DEPTH);
  if (m_draw_marked_facet &&
      (get_aos_marked_vertex_style() != Vertex_shape::NONE))
    m_renderer.push_back(m_marked_vertices_renderer,
                         Arrangement_renderer::DEPTH);
  if (m_draw_marked_edge && is_aos_marked_edge_enabled())
    m_renderer.push_back(m_marked_edges_renderer, Arrangement_renderer::DEPTH);
}

//! \brief draws an arrangement on sphere marked vertex.
void Spherical_gaussian_map_marked_geo::
draw_aos_marked_vertex(Draw_action* action, Vector3f& center)
{
  draw_vertex_on_sphere(action, center,
                        m_aos_marked_vertex_style,
                        m_aos_marked_vertex_radius,
                        m_aos_delta_angle);
}

//! \brief draws an arrangement on sphere marked edge.
void Spherical_gaussian_map_marked_geo::
draw_aos_marked_edge(Draw_action* action,
                     Vector3f& source, Vector3f& target, Vector3f& normal)
{
  draw_edge_on_sphere(action, source, target, normal,
                      m_aos_marked_edge_style,
                      m_aos_marked_edge_count,
                      m_aos_marked_edge_directed,
                      m_aos_marked_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

//! \brief sets the source gausian maps of the minkowski sum.
void Spherical_gaussian_map_marked_geo::
insert_sgm(Shared_spherical_gaussian_map_marked_geo sgm)
{
  m_sgm_nodes.push_back(sgm);
  Observer observer(this, get_field_info(GEOMETRIES));
  sgm->register_observer(observer);
  m_dirty_bounding_sphere = true;
}

//! \brief obrains a reference to the cubical Gaussian map.
Spherical_gaussian_map_marked* Spherical_gaussian_map_marked_geo::get_sgm()
{
  if (m_dirty_sgm) clean_sgm();
  return m_sgm;
}

//! \brief sets the Gaussian map.
void Spherical_gaussian_map_marked_geo::
set_sgm(Spherical_gaussian_map_marked* sgm)
{
  m_dirty_sgm = false;
  m_dirty_bounding_sphere = true;
  m_sgm = sgm;
}

SGAL_END_NAMESPACE
