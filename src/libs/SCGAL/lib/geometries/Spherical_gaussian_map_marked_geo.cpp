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
// $Id: Spherical_gaussian_map_marked_geo.cpp 7805 2009-07-19 22:21:38Z efif $
// $Revision: 7805 $
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

#include "SCGAL/Spherical_gaussian_map_marked_geo.hpp"
#include "SCGAL/Exact_coord_array.hpp"

#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Cylinder.hpp"
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

SGAL_BEGIN_NAMESPACE

std::string
Spherical_gaussian_map_marked_geo::s_tag("SphericalGaussianMapMarked");
Container_proto* Spherical_gaussian_map_marked_geo::s_prototype = NULL;

/*! Default values */

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
const Int Spherical_gaussian_map_marked_geo::s_def_aos_marked_edge_count(1);
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

/*! Constructor */
Spherical_gaussian_map_marked_geo::
Spherical_gaussian_map_marked_geo(Boolean proto) :
  Spherical_gaussian_map_base_geo(proto),
  m_own_sgm(SGAL_FALSE),
  m_sgm(NULL),
  m_minkowski_sum(SGAL_FALSE),
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

  m_vertices_renderer(NULL),
  m_colored_vertices_renderer(NULL),
  m_edges_renderer(NULL),
  m_colored_edges_renderer(NULL),
  m_inflated_line_edges_renderer(NULL),
  m_inflated_strip_edges_renderer(NULL),
  m_inflated_tube_edges_renderer(NULL),

  m_marked_vertices_renderer(NULL),
  m_colored_marked_vertices_renderer(NULL),
  m_marked_edges_renderer(NULL),
  m_colored_marked_edges_renderer(NULL)
{
  if (!proto) create_renderers();
}

/*! Copy Constructor */
Spherical_gaussian_map_marked_geo::
Spherical_gaussian_map_marked_geo(const Spherical_gaussian_map_marked_geo & gm)
{
  // Not implemented yet!
  SGAL_assertion(0);
}

/*! Destructor */
Spherical_gaussian_map_marked_geo::~Spherical_gaussian_map_marked_geo()
{
  m_sgm_nodes.clear();
  if (m_sgm && m_own_sgm) delete m_sgm;
}

/*! Clean the data structure */
void Spherical_gaussian_map_marked_geo::clean()
{
  if (!m_sgm) {
    m_sgm = new Sgm;
    m_own_sgm = SGAL_TRUE;
  }
  Mesh_set::clean();
  if (m_minkowski_sum) {
    clock_t start_time = clock();
    Sgm_node_iter ni = m_sgm_nodes.begin();
    Spherical_gaussian_map_marked_geo* geo1 = *ni++;
    Spherical_gaussian_map_marked_geo* geo2 = *ni;
    m_sgm->minkowski_sum(*(geo1->get_sgm()), *(geo2->get_sgm()));
    clock_t end_time = clock();
    m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  } else if (m_coord_array) {
    clock_t start_time = clock();
    Sgm_initializer sgm_initializer(*m_sgm);
    Sgm_geo_initializer_visitor visitor;
    sgm_initializer.set_marked_vertex_index(m_marked_vertex_index);
    sgm_initializer.set_marked_edge_index(m_marked_edge_index);
    sgm_initializer.set_marked_facet_index(m_marked_facet_index);

    Exact_coord_array* exact_coord_array =
      dynamic_cast<Exact_coord_array *>(m_coord_array);
    if (exact_coord_array && (exact_coord_array->size() > 0)) {
      // std::cout << "Spherical_gaussian_map_marked_geo::exact" << std::endl;
      sgm_initializer(exact_coord_array->begin(),
                      exact_coord_array->end(),
                      exact_coord_array->size(),
                      m_coord_indices.begin(), m_coord_indices.end(),
                      m_num_primitives, &visitor);
    } else {
      // std::cout << "Spherical_gaussian_map_marked_geo::inexact" << std::endl;
      sgm_initializer(m_coord_array->begin(), m_coord_array->end(),
                      m_coord_array->size(),
                      m_coord_indices.begin(), m_coord_indices.end(),
                      m_num_primitives, &visitor);
    }
    clock_t end_time = clock();
    m_time = static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
  }
  update_facets();

#if 0
  // Mark one vertex in the corresponding aos faces
  m_sgm.reset_faces();                // reset the "visited" flag
  Arrangement & arr = m_sgm.arrangement(0);
  Sgm_face_iterator fi = arr.faces_begin();
  m_sgm.process_boundary_faces(fi, Face_set_marked_op());
#endif

  Uint num_vertices = m_sgm->number_of_vertices();
  Uint num_edges = m_sgm->number_of_edges();
  Uint num_facets = m_sgm->number_of_facets();
  if (m_marked_vertex_index >= num_vertices)
    m_marked_vertex_index = num_vertices;
  if (m_marked_edge_index >= num_edges) m_marked_edge_index = num_edges;
  if (m_marked_facet_index >= num_facets) m_marked_facet_index = num_facets;
}

/*! Clear the internal representation and auxiliary data structures */
void Spherical_gaussian_map_marked_geo::clear()
{
  Spherical_gaussian_map_base_geo::clear();
  if (m_sgm) m_sgm->clear();
}

/*! */
void Spherical_gaussian_map_marked_geo::cull(Cull_context & cull_context) {}

/*! */
void Spherical_gaussian_map_marked_geo::isect(Isect_action* action)
{
  Context* context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);

  isect_primary();
    
  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

/*! \brief sets the attributes of the object extracted from an input file */
void Spherical_gaussian_map_marked_geo::set_attributes(Element* elem)
{
  Spherical_gaussian_map_base_geo::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);

    // AOS marked vertex attributes:
    if (name == "aosMarkedVertexStyle") {
      m_aos_marked_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
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
      m_aos_marked_edge_style = Edge_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeCount") {
      m_aos_marked_edge_count = boost::lexical_cast<Int>(value);
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

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); mcai++)
  {
    const std::string & name = elem->get_name(mcai);
    Cont_list & cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      set_minkowski_sum(true);
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Container* cont = *ci;
        Spherical_gaussian_map_marked_geo* sgm =
          dynamic_cast<Spherical_gaussian_map_marked_geo*>(cont);
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

/*! */
void Spherical_gaussian_map_marked_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Spherical_gaussian_map_base_geo::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the object fields to the prototype
  MF_container* field = new MF_container(GEOMETRIES, "geometries",
                                          get_member_offset(&m_sgm_nodes));
  s_prototype->add_field_info(field);

  Execution_function exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_marked_geo::
                                    increase_vertex_index);
  SF_bool* bool_field =
    new SF_bool(INCREASE_VERTEX_INDEX, "increaseVertexIndex",
                get_member_offset(&m_increase_vertex_index),
                exec_func);
  s_prototype->add_field_info(bool_field);
  exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_marked_geo::
                                    increase_edge_index);
  bool_field = new SF_bool(INCREASE_EDGE_INDEX, "increaseEdgeIndex",
                           get_member_offset(&m_increase_edge_index),
                           exec_func);
  s_prototype->add_field_info(bool_field);
  exec_func =
    static_cast<Execution_function>(&Spherical_gaussian_map_marked_geo::
                                    increase_facet_index);
  bool_field = new SF_bool(INCREASE_FACET_INDEX, "increaseFacetIndex",
                           get_member_offset(&m_increase_facet_index),
                           exec_func);
  s_prototype->add_field_info(bool_field);
}

/*! */
void Spherical_gaussian_map_marked_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Spherical_gaussian_map_marked_geo::get_prototype() 
{  
  if (!s_prototype) Spherical_gaussian_map_marked_geo::init_prototype();
  return s_prototype;
}

/*! Draw the polyhedron directly from the gaussian map representation */
void Spherical_gaussian_map_marked_geo::draw_primal(Draw_action* action)
{
  TRACE_MSG(Trace::GAUSSIAN_MAP, "draw_primal()\n");

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
  
  Context* context = action->get_context();
  glColor3fv((float*)&m_marked_facet_color);
  
  glFrontFace((is_ccw()) ? GL_CW : GL_CCW);

  Sgm_vertex_const_iterator vit;
  for (vit = m_sgm->vertices_begin(); vit != m_sgm->vertices_end(); ++vit) {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3) continue;
    
    if (m_draw_marked_facet && vit->marked())
      context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
          
    glBegin(GL_POLYGON);
    const Vector3f & normal = vit->get_rendered_normal();
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

/*! \brief draws the primal marked vertex. */
void Spherical_gaussian_map_marked_geo::
draw_primal_marked_vertex(Draw_action* action)
{
  Sphere vertex_geom;
  vertex_geom.set_radius(m_marked_vertex_radius);

  Context* context = action->get_context();
  
  Sgm_face_const_iterator fi;
  for (fi = m_sgm->faces_begin(); fi != m_sgm->faces_end(); ++fi) {
    if (!fi->marked()) continue;
    
    Vector3f vec = to_vector3f(fi->point());

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

/*! Draw the primal marked edge */
void Spherical_gaussian_map_marked_geo::
draw_primal_marked_edge(Draw_action* action)
{
  Cylinder edge_geom;
  edge_geom.set_radius(m_marked_edge_radius);

  Context* context = action->get_context();
  
  Sgm_halfedge_const_iterator hi;
  for (hi = m_sgm->halfedges_begin(); hi != m_sgm->halfedges_end(); ++hi) {
    if (!hi->marked()) continue;
    
    Sgm_face_const_handle face1 = hi->face();
    Sgm_face_const_handle face2 = hi->twin()->face();

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
    Matrix4f mat;
    trans.get_matrix(mat);
        
    glPushMatrix();
    glMultMatrixf((float *)&mat);
    glColor3fv((float*)&m_marked_edge_color);
    context->draw_material_mode_enable(Gfx::COLOR_MATERIAL);
    edge_geom.draw(action);
    context->draw_material_mode_enable(Gfx::NO_COLOR_MATERIAL);
    glPopMatrix();
    return;
  }
}

/*! */
void Spherical_gaussian_map_marked_geo::isect_primary()
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

/*! Print statistics. */
void Spherical_gaussian_map_marked_geo::print_stat()
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
    glColor3fv((float *)&m_marked_vertex_color);
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

/*! \brief draws the marked (primal) vertex. */
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
    } else {
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
  
/*! \brief draws the arrangement vertices. */
void Spherical_gaussian_map_marked_geo::draw_aos_vertices(Draw_action* action)
{
  Sgm_vertex_const_iterator vi;
  for (vi = m_sgm->vertices_begin(); vi != m_sgm->vertices_end(); ++vi) {
    if (m_draw_marked_facet && vi->marked()) continue;
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    draw_aos_vertex(action, center);
  }
}

/*! \brief draws the arrangement vertices. */
void Spherical_gaussian_map_marked_geo::draw_aos_edges(Draw_action* action)
{
  Sgm_edge_const_iterator hei;
  for (hei = m_sgm->edges_begin(); hei != m_sgm->edges_end(); ++hei) {
    if (get_draw_marked_edge() && hei->marked()) continue;
    const X_monotone_curve_2 & curve = hei->curve();
    Vector3f src = to_vector3f(curve.source());
    Vector3f trg = to_vector3f(curve.target());
    src.normalize();
    trg.normalize();
    draw_aos_edge(action, src, trg);
  }
}

/*! \biref increases the vertex index. */
void Spherical_gaussian_map_marked_geo::
increase_vertex_index(Field_info* field_info)
{
  m_marked_vertex_index++;
  if (m_marked_vertex_index == m_sgm->number_of_vertices())
    m_marked_vertex_index = 0;
  clear();
}

/*! \biref increases the face index. */
void Spherical_gaussian_map_marked_geo::
increase_edge_index(Field_info* field_info)
{
  m_marked_edge_index++;
  if (m_marked_edge_index == m_sgm->number_of_edges())
    m_marked_edge_index = 0;
  clear();
}

/*! \biref increases the face index */
void Spherical_gaussian_map_marked_geo::
increase_facet_index(Field_info* field_info)
{
  m_marked_facet_index++;
  if (m_marked_facet_index == m_sgm->number_of_facets())
    m_marked_facet_index = 0;
  clear();
}

/*! \brief renders the marked_vertices with color */
void Spherical_gaussian_map_marked_geo::Marked_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_facet_color);
  Sgm_vertex_const_iterator vi;
  for (vi = m_geo.m_sgm->vertices_begin(); vi != m_geo.m_sgm->vertices_end();
       ++vi)
  {
    if (!vi->marked()) continue;
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    m_geo.draw_aos_marked_vertex(action, center);
  }
}

/*! \brief renders the marked_vertices with color */
void Spherical_gaussian_map_marked_geo::Colored_marked_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_facet_color);
  Marked_vertices_renderer::operator()(action);
}

/*! \brief renders the edges with color */
void Spherical_gaussian_map_marked_geo::Colored_edges_renderer::
operator()(Draw_action* action)
{
  Vector3f src;
  Vector3f trg;
  Sgm_edge_const_iterator hei;
  for (hei = m_geo.m_sgm->edges_begin(); hei != m_geo.m_sgm->edges_end(); ++hei)
  {
    if (m_geo.get_draw_marked_edge() && hei->marked()) continue;
    
    const X_monotone_curve_2 & curve = hei->curve();
    src = to_vector3f(curve.source());
    trg = to_vector3f(curve.target());
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
    m_geo.draw_aos_edge(action, src, trg);
  }
}

/*! \brief renders the marked_edges. */
void Spherical_gaussian_map_marked_geo::Marked_edges_renderer::
operator()(Draw_action* action)
{
  Vector3f src;
  Vector3f trg;
  Sgm_edge_const_iterator hei;
  for (hei = m_geo.m_sgm->edges_begin(); hei != m_geo.m_sgm->edges_end(); ++hei)
  {
    if (!hei->marked()) continue;
    const X_monotone_curve_2 & curve = hei->curve();
    src = to_vector3f(curve.source());
    trg = to_vector3f(curve.target());
    src.normalize();
    trg.normalize();
    m_geo.draw_aos_marked_edge(action, src, trg);
  }
}

/*! \brief renders the marked_edges with color. */
void Spherical_gaussian_map_marked_geo::Colored_marked_edges_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_edge_color);
  Marked_edges_renderer::operator()(action);
}

/*! \brief renders the marked primal vertex. */
void Spherical_gaussian_map_marked_geo::Marked_face_renderer::
operator()(Draw_action* action)
{
  const Vector3f & color = m_geo.get_marked_vertex_color();
  glColor4f(color[0], color[1], color[2],
            1 - m_geo.get_aos_marked_face_transparency());
  m_geo.draw_aos_marked_face(action);
}

/*! \brief creates the renderers */
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

/*! \brief destroys the renderers. */
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

/*! \brief cleans the renderer. */
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

/*! \brief draws an arrangement on sphere marked vertex */
void Spherical_gaussian_map_marked_geo::
draw_aos_marked_vertex(Draw_action* action, Vector3f & center)
{
  draw_vertex_on_sphere(action, center,
                        m_aos_marked_vertex_style,
                        m_aos_marked_vertex_radius,
                        m_aos_delta_angle);
}

/*! \brief draws an arrangement on sphere marked edge. */
void Spherical_gaussian_map_marked_geo::
draw_aos_marked_edge(Draw_action* action,
                     Vector3f & source, Vector3f & target)
{
  draw_edge_on_sphere(action, source, target,
                      m_aos_marked_edge_style,
                      m_aos_marked_edge_count,
                      m_aos_marked_edge_directed,
                      m_aos_marked_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

/*! Set the source gausian maps of the minkowski sum. */
void Spherical_gaussian_map_marked_geo::
insert_sgm(Spherical_gaussian_map_marked_geo* sgm)
{
  m_sgm_nodes.push_back(sgm);
  Observer observer(this, get_field_info(GEOMETRIES));  
  sgm->register_observer(observer);
  m_dirty_sphere_bound = true;
}

/*! \brief obrains a reference to the cubical Gaussian map. */
Spherical_gaussian_map_marked* Spherical_gaussian_map_marked_geo::get_sgm()
{
  if (m_dirty) clean();
  return m_sgm;
}

/*! \brief sets the Gaussian map. */
void Spherical_gaussian_map_marked_geo::
set_sgm(Spherical_gaussian_map_marked* sgm)
{
  m_dirty = false;
  m_dirty_sphere_bound = true;
  m_sgm = sgm;
}
  
SGAL_END_NAMESPACE
