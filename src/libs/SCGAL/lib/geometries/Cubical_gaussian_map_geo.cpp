// Copyright (c) 2004 Israel.
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
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <time.h>
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
#include "SGAL/Extrusion.hpp"

#include "SCGAL/Cubical_gaussian_map_geo.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

std::string Cubical_gaussian_map_geo::s_tag = "CubicalGaussianMap";
SGAL::Container_proto* Cubical_gaussian_map_geo::s_prototype(nullptr);

const bool Cubical_gaussian_map_geo::s_def_draw_aos(false);
const bool Cubical_gaussian_map_geo::s_def_draw_aos_opaque(false);
const bool Cubical_gaussian_map_geo::s_def_draw_aos_haloed(true);
const bool Cubical_gaussian_map_geo::s_def_draw_aos_unfolded(false);

const bool Cubical_gaussian_map_geo::s_def_draw_aos_surface(true);
const Vector3f
Cubical_gaussian_map_geo::s_def_aos_surface_color(0.3f, 0.3f, 0.3f);

// Dual vertex attributes:
const Cubical_gaussian_map_geo::Vertex_style
  Cubical_gaussian_map_geo::s_def_aos_vertex_style(Vertex_shape::DISC);
const Float Cubical_gaussian_map_geo::s_def_aos_vertex_radius(0.06f);
const Float Cubical_gaussian_map_geo::s_def_aos_vertex_point_size(2);
const Vector3f Cubical_gaussian_map_geo::s_def_aos_vertex_color(0.5f, 0, 0);

// Dual nonreal vertex attributes:
const Cubical_gaussian_map_geo::Vertex_style
  Cubical_gaussian_map_geo::
  s_def_aos_nonreal_vertex_style(Vertex_shape::DISC);
const Float Cubical_gaussian_map_geo::s_def_aos_nonreal_vertex_radius(0.03f);
const Float Cubical_gaussian_map_geo::s_def_aos_nonreal_vertex_point_size(1);
const Vector3f Cubical_gaussian_map_geo::
s_def_aos_nonreal_vertex_color(0, 0, 0);

// Dual dual edge attributes:
const Boolean Cubical_gaussian_map_geo::s_def_aos_edge_enabled(true);
const Cubical_gaussian_map_geo::Edge_style
  Cubical_gaussian_map_geo::s_def_aos_edge_style(Edge_shape::TUBE);
const Uint Cubical_gaussian_map_geo::s_def_aos_edge_count(1);
const Boolean Cubical_gaussian_map_geo::s_def_aos_edge_directed(false);
const Float Cubical_gaussian_map_geo::s_def_aos_edge_radius(.03f);
const Float Cubical_gaussian_map_geo::s_def_aos_edge_line_width(1);
const Vector3f Cubical_gaussian_map_geo::s_def_aos_edge_color(1, 1, 1);

// Dual nonreal-edge attributes:
const Boolean Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_enabled(true);
const Cubical_gaussian_map_geo::Edge_style
  Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_style(Edge_shape::TUBE);
const Uint Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_count(1);
const Boolean Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_directed(false);
const Float Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_radius(.015f);
const Float Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_line_width(1);
const Vector3f Cubical_gaussian_map_geo::s_def_aos_nonreal_edge_color(0, 0, 0);

//
const Float Cubical_gaussian_map_geo::s_def_aos_delta_angle(0.1f);

const float Cubical_gaussian_map_geo::s_def_marked_vertex_radius(0.1f);
const float Cubical_gaussian_map_geo::s_def_marked_edge_radius(0.1f);

const Cubical_gaussian_map_geo::Vertex_style
  Cubical_gaussian_map_geo::
  s_def_aos_marked_vertex_style(Vertex_shape::DISC);
const Float Cubical_gaussian_map_geo::s_def_aos_marked_vertex_radius(0.03f);
const Float Cubical_gaussian_map_geo::s_def_aos_marked_vertex_point_size(1);

const bool Cubical_gaussian_map_geo::s_def_draw_marked_vertex(false);
const bool Cubical_gaussian_map_geo::s_def_draw_marked_edge(false);
const bool Cubical_gaussian_map_geo::s_def_draw_marked_facet(false);
const Vector3f Cubical_gaussian_map_geo::s_def_marked_vertex_color(0.5f, 0, 0);
const Vector3f Cubical_gaussian_map_geo::s_def_marked_edge_color(0, 0.5f, 0);
const Vector3f Cubical_gaussian_map_geo::s_def_marked_facet_color(0, 0, 0.5f);

REGISTER_TO_FACTORY(Cubical_gaussian_map_geo, "Cubical_gaussian_map_geo");

//! \brief constructor.
Cubical_gaussian_map_geo::Cubical_gaussian_map_geo(Boolean proto) :
  Mesh_set(proto),
  m_dirty_cgm(true),
  m_draw_aos(s_def_draw_aos), m_draw_primal(!s_def_draw_aos),
  m_draw_aos_opaque(s_def_draw_aos_opaque),
  m_draw_aos_haloed(s_def_draw_aos_haloed),
  m_draw_aos_unfolded(s_def_draw_aos_unfolded),
  m_draw_aos_surface(s_def_draw_aos_surface),
  m_aos_surface_color(s_def_aos_surface_color),
  m_aos_vertex_style(s_def_aos_vertex_style),
  m_aos_vertex_radius(s_def_aos_vertex_radius),
  m_aos_vertex_point_size(s_def_aos_vertex_point_size),
  m_aos_vertex_color(s_def_aos_vertex_color),
  m_aos_nonreal_vertex_style(s_def_aos_nonreal_vertex_style),
  m_aos_nonreal_vertex_radius(s_def_aos_nonreal_vertex_radius),
  m_aos_nonreal_vertex_point_size(s_def_aos_nonreal_vertex_point_size),
  m_aos_nonreal_vertex_color(s_def_aos_nonreal_vertex_color),
  // Edge
  m_aos_edge_enabled(s_def_aos_edge_enabled),
  m_aos_edge_style(s_def_aos_edge_style),
  m_aos_edge_count(s_def_aos_edge_count),
  m_aos_edge_directed(s_def_aos_edge_directed),
  m_aos_edge_radius(s_def_aos_edge_radius),
  m_aos_edge_line_width(s_def_aos_edge_line_width),
  // Nonreal edge
  m_aos_nonreal_edge_enabled(s_def_aos_nonreal_edge_enabled),
  m_aos_nonreal_edge_style(s_def_aos_nonreal_edge_style),
  m_aos_nonreal_edge_count(s_def_aos_nonreal_edge_count),
  m_aos_nonreal_edge_directed(s_def_aos_nonreal_edge_directed),
  m_aos_nonreal_edge_radius(s_def_aos_nonreal_edge_radius),
  m_aos_nonreal_edge_line_width(s_def_aos_nonreal_edge_line_width),
  m_aos_nonreal_edge_color(s_def_aos_nonreal_edge_color),
  m_aos_delta_angle(s_def_aos_delta_angle),
  m_aos_marked_vertex_style(s_def_aos_marked_vertex_style),
  m_aos_marked_vertex_radius(s_def_aos_marked_vertex_radius),
  m_aos_marked_vertex_point_size(s_def_aos_marked_vertex_point_size),
  m_draw_marked_vertex(s_def_draw_marked_vertex),
  m_draw_marked_edge(s_def_draw_marked_edge),
  m_draw_marked_facet(s_def_draw_marked_facet),
  m_marked_vertex_color(s_def_marked_vertex_color),
  m_marked_edge_color(s_def_marked_edge_color),
  m_marked_facet_color(s_def_marked_facet_color),
  m_translated(false),
  m_rotated(false),
  m_minkowski_sum(false),
  m_vertex_geom(nullptr),
  m_edge_geom(nullptr),
  m_marked_vertex_radius(s_def_marked_vertex_radius),
  m_marked_edge_radius(s_def_marked_edge_radius),
  m_marked_vertex_index(0),
  m_marked_edge_index(0),
  m_marked_facet_index(0),
  m_renderer_dirty(true),
  m_surface_renderer(nullptr),
  m_colored_surface_renderer(nullptr),
  m_stencil_surface_renderer(nullptr),
  m_vertices_renderer(nullptr),
  m_colored_vertices_renderer(nullptr),
  m_nonreal_vertices_renderer(nullptr),
  m_colored_nonreal_vertices_renderer(nullptr),
  m_marked_vertices_renderer(nullptr),
  m_colored_marked_vertices_renderer(nullptr),
  m_edges_renderer(nullptr),
  m_colored_edges_renderer(nullptr),
  m_inflated_line_edges_renderer(nullptr),
  m_inflated_strip_edges_renderer(nullptr),
  m_inflated_tube_edges_renderer(nullptr)
{
  if (!proto) {
    create_renderers();
    m_aos_edge_color[0] = m_aos_edge_color[1] = s_def_aos_edge_color;
  }
}

//! \brief copy Constructor.
Cubical_gaussian_map_geo::
Cubical_gaussian_map_geo(const Cubical_gaussian_map_geo& gaussian_map)
{
  // Not implemented yet!
  SGAL_assertion(0);
}

//! \brief destructor.
Cubical_gaussian_map_geo::~Cubical_gaussian_map_geo()
{
  m_cgm_nodes.clear();
  clear_cgm();
  if (m_vertex_geom) {
    delete m_vertex_geom;
    m_vertex_geom = nullptr;
  }
  if (m_edge_geom) {
    delete m_edge_geom;
    m_edge_geom = nullptr;
  }
}

//! \brief cleans the data structure.
void Cubical_gaussian_map_geo::clean_cgm()
{
  m_dirty_cgm = false;

  if (m_minkowski_sum) {
    Cgm_node_iter ni = m_cgm_nodes.begin();
    Shared_cubical_gaussian_map_geo geo1 = *ni++;
    Shared_cubical_gaussian_map_geo geo2 = *ni;
    if (geo1->is_dirty_cgm()) geo1->clean_cgm();
    if (geo2->is_dirty_cgm()) geo2->clean_cgm();
    clock_t start_time = clock();
    m_cgm.minkowski_sum(geo1->get_cgm(), geo2->get_cgm());
    clock_t end_time = clock();
    m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  }
  else {
    clock_t start_time = clock();
    auto exact_coord_array =
      boost::dynamic_pointer_cast<Exact_coord_array_3d>(get_coord_array());
    if (exact_coord_array) {
      if (!exact_coord_array->empty()) {
        typedef boost::shared_ptr<Exact_coord_array_3d>
          Shared_exact_coord_array_3d;
        Cleaner_visitor<Shared_exact_coord_array_3d>
          cleaner_visitor(this, exact_coord_array);
        const auto& indices = get_facet_coord_indices();
        boost::apply_visitor(cleaner_visitor, indices);
      }
    }
    else {
      auto coord_array =
        boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
      if (coord_array) {
        if (!coord_array->empty()) {
          typedef boost::shared_ptr<Coord_array_3d>     Shared_coord_array_3d;
          Cleaner_visitor<Shared_coord_array_3d>
            cleaner_visitor(this, coord_array);
          const auto& indices = get_facet_coord_indices();
          boost::apply_visitor(cleaner_visitor, indices);
        }
      }
      else SGAL_error();
    }
    clock_t end_time = clock();
    m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  }

  // Update the normals of all facets in the corresponding dual vertices
  process_facets();

#if 0
  // Mark one vertex in the corresponding dual faces
  m_cgm.reset_faces();                // reset the "visited" flag
  Arrangement& arr = m_cgm.arrangement(0);
  Arr_face_iterator fi = arr.faces_begin();
  fi++;                         // skip the unbounded face
  m_cgm.process_boundary_faces(fi, Face_set_marked_op());
#endif

  Uint num_vertices = m_cgm.number_of_vertices();
  Uint num_edges = m_cgm.number_of_edges();
  Uint num_facets = m_cgm.number_of_facets();
  if (m_marked_vertex_index >= num_vertices)
    m_marked_vertex_index = num_vertices;
  if (m_marked_edge_index >= num_edges) m_marked_edge_index = num_edges;
  if (m_marked_facet_index >= num_facets) m_marked_facet_index = num_facets;
}

//! \brief obrains a reference to the cubical Gaussian map.
Cubical_gaussian_map_geo::Polyhedral_cgm*
Cubical_gaussian_map_geo::get_cgm()
{
  if (is_dirty_cgm()) clean_cgm();
  return &m_cgm;
}

//! \brief sets the cubical Gaussian map.
void Cubical_gaussian_map_geo::set_cgm(const Polyhedral_cgm& cgm)
{
  m_dirty_cgm = false;
  m_cgm = cgm;
}

//! \brief clears the internal representation and auxiliary data structures.
void Cubical_gaussian_map_geo::clear_cgm()
{
  m_dirty_cgm = true;
  m_cgm.clear();
}

//! \brief draws the geometry for selection.
void Cubical_gaussian_map_geo::isect(SGAL::Isect_action* action)
{
  Context* context = action->get_context();
  if (!m_is_solid && context) context->draw_cull_face(Gfx::NO_CULL);

  isect_primary();

  if (!m_is_solid  && context) context->draw_cull_face(Gfx::BACK_CULL);
}

//! \brief cleans the bounding sphere of the cubical Gaussian map.
void Cubical_gaussian_map_geo::clean_bounding_sphere()
{
#define SQRT_3          1.732f

  if (m_bb_is_pre_set) {
    m_dirty_bounding_sphere = false;
    return;
  }

  if (is_dirty_cgm()) clean_cgm();
  if (m_draw_aos) {
    if (!m_draw_aos_unfolded) {
      m_bounding_sphere.set_center(Vector3f(0, 0, 0));
      m_bounding_sphere.set_radius(SQRT_3);
    }
    else {
      m_bounding_sphere.set_center(Vector3f(5, 4, 0));
      m_bounding_sphere.set_radius(4);
    }
  }
  else {
    Inexact_sphere_vector spheres;

    //! \todo this should change with dual-face iterator
    Uint points_size = 0;
    for (Uint i = 0; i < Polyhedral_cgm::NUM_FACES; ++i) {
      const Arrangement& arr = m_cgm.arrangement(i);
      Uint num_faces = arr.number_of_faces();
      points_size += num_faces - 1;
    }
    spheres.resize(points_size);
    Extract_dual_approximate_sphere extract;
    Uint j = 0;
    for (Uint i = 0; i < Polyhedral_cgm::NUM_FACES; ++i) {
      const Arrangement& arr = m_cgm.arrangement(i);
      Uint num_faces = arr.number_of_faces();
      Arr_face_const_iterator fi = arr.faces_begin();
      fi++;           // skip the unbounded face
      std::transform(fi, arr.faces_end(), &spheres[j], extract);
      j += num_faces - 1;
    }

    if (!spheres.empty()) {
      Min_sphere min_sphere(spheres.begin(), spheres.end());
      Vector3f center_vec;
      std::copy(min_sphere.center_cartesian_begin(),
                min_sphere.center_cartesian_end(),
                &center_vec[0]);
      m_bounding_sphere.set_center(center_vec);
      m_bounding_sphere.set_radius(min_sphere.radius());
    }
  }
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of this object.
void Cubical_gaussian_map_geo::set_attributes(SGAL::Element* elem)
{
  Mesh_set::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "drawDual") {
      m_draw_aos = compare_to_true(value);
      m_draw_primal = !m_draw_aos;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualOpaque") {
      m_draw_aos_opaque = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualHaloed") {
      m_draw_aos_haloed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualUnfolded") {
      m_draw_aos_unfolded = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDualCube") {
      m_draw_aos_surface = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualCubeColor") {
      Vector3f col(value);
      set_aos_surface_color(col);
      elem->mark_delete(ai);
      continue;
    }

    // Dual vertex attributes:
    if (name == "dualVertexStyle") {
      m_aos_vertex_style = Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualVertexRadius") {
      m_aos_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualVertexPointSize") {
      m_aos_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualVertexColor") {
      Vector3f col(value);
      set_aos_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }

    // Dual nonreal vertex attributes:
    if (name == "dualNonrealVertexStyle") {
      m_aos_nonreal_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualNonrealVertexRadius") {
      m_aos_nonreal_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualNonrealVertexPointSize") {
      m_aos_nonreal_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualNonrealVertexColor") {
      Vector3f col(value);
      set_aos_nonreal_vertex_color(col);
      elem->mark_delete(ai);
      continue;
    }

    // Edge attributes:
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
    if (name == "dualLineWidth") {
      sscanf(value.c_str(), "%f", &m_aos_edge_line_width);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualLineColor") {
      Vector3f col(value);
      set_aos_edge_color(col, 0);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualLineColor1") {
      Vector3f col(value);
      set_aos_edge_color(col, 0);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualLineColor2") {
      Vector3f col(value);
      set_aos_edge_color(col, 1);
      elem->mark_delete(ai);
      continue;
    }

    // Dual marked Vertex attributes:
    if (name == "dualMarkedVertexStyle") {
      m_aos_marked_vertex_style =
        Vertex_shape::style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualMarkedVertexRadius") {
      m_aos_marked_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "dualMarkedVertexPointSize") {
      m_aos_marked_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    // Marked primal features:
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
    if (name == "markedFaceColor") {
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
    if (name == "dualVertexRadius") {
      m_aos_vertex_radius = boost::lexical_cast<Float>(value);
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
        Shared_cubical_gaussian_map_geo cgm =
          boost::dynamic_pointer_cast<Cubical_gaussian_map_geo>(cont);
        if (cgm) insert_cgm(cgm);
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
void Cubical_gaussian_map_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new SGAL::Container_proto(Mesh_set::get_prototype());

  // Add the object fields to the prototype
  // drawDual
  auto exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::draw_changed);
  auto draw_aos_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_aos_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL,
                                          "drawDual",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_func, exec_func));

  // drawDualUnfolded
  auto draw_aos_unfolded_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_aos_unfolded_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL_UNFOLDED,
                                          "drawDualUnfolded",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_unfolded_func, exec_func));

  // drawDualOpaque
  auto draw_aos_opaque_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_aos_opaque_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL_OPAQUE,
                                          "drawDualOpaque",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_opaque_func));

  // drawDualHaloed
  auto draw_aos_haloed_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_aos_haloed_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL_HALOED,
                                          "drawDualHaloed",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_haloed_func));

  // drawDualCube
  auto draw_aos_surface_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_aos_surface_handle);
  s_prototype->add_field_info(new SF_bool(DRAW_DUAL_CUBE,
                                          "drawDualCube",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_surface_func));

  // dualLineWidth
  auto aos_edge_line_width_func =
    static_cast<Float_handle_function>
    (&Cubical_gaussian_map_geo::aos_edge_line_width_handle);
  s_prototype->add_field_info(new SF_float(DUAL_EDGE_LINE_WIDTH,
                                           "dualLineWidth",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           aos_edge_line_width_func));

  // translated
  exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::coord_changed);
  auto translated_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::translated_handle);
  s_prototype->add_field_info(new SF_bool(TRANSLATED,
                                          "translated",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          translated_func, exec_func));

  // rotated
  auto rotated_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::rotated_handle);
  s_prototype->add_field_info(new SF_bool(ROTATED, "rotated",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          rotated_func, exec_func));

  // trueDrawPrimal
  auto draw_primal_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::draw_primal_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_PRIMAL,
                                          "trueDrawPrimal",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_primal_func));

  // trueDrawDual
//   Boolean_handle_function draw_aos_func =
//     static_cast<Boolean_handle_function>
//     (&Cubical_gaussian_map_geo::draw_aos_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_DUAL,
                                          "trueDrawDual",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          draw_aos_func));

  // trueDrawDualUnfolded
//   Boolean_handle_function draw_aos_unfolded_func =
//     static_cast<Boolean_handle_function>
//     (&Cubical_gaussian_map_geo::draw_aos_unfolded_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_DRAW_DUAL_UNFOLDED,
                                          "trueDrawDualUnfolded",
                                          Field_info::RULE_EXPOSED_FIELD,
                                           draw_aos_unfolded_func));

  // export
  exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::output);
  auto export_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::export_handle);
  s_prototype->add_field_info(new SF_bool(EXPORT, "export",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          export_func,
                                          exec_func));

  // increaseVertexIndex
  exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::
                                    increase_vertex_index);
  auto increase_vertex_index_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::increase_vertex_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_VERTEX_INDEX,
                                          "increaseVertexIndex",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          increase_vertex_index_func,
                                          exec_func));

  // increaseEdgeIndex
  exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::
                                    increase_edge_index);
  auto increase_edge_index_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::increase_edge_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_EDGE_INDEX,
                                          "increaseEdgeIndex",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          increase_edge_index_func,
                                          exec_func));

  // increaseFacetIndex
  exec_func =
    static_cast<Execution_function>(&Cubical_gaussian_map_geo::
                                    increase_facet_index);
  auto increase_facet_index_func =
    static_cast<Boolean_handle_function>
    (&Cubical_gaussian_map_geo::increase_facet_index_handle);
  s_prototype->add_field_info(new SF_bool(INCREASE_FACET_INDEX,
                                          "increaseFacetIndex",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          increase_facet_index_func,
                                          exec_func));

  // dualLineColor1
  auto aos_edge_color0_func =
    static_cast<Vector3f_handle_function>
    (&Cubical_gaussian_map_geo::aos_edge_color1_handle);
  s_prototype->add_field_info(new SF_vector3f(DUAL_EDGE_COLOR1,
                                              "dualLineColor1",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              aos_edge_color0_func));

  // dualLineColor2
  auto aos_edge_color1_func =
    static_cast<Vector3f_handle_function>
    (&Cubical_gaussian_map_geo::aos_edge_color2_handle);
  s_prototype->add_field_info(new SF_vector3f(DUAL_EDGE_COLOR2,
                                              "dualLineColor2",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              aos_edge_color1_func));

  // geometries
  auto cgm_nodes_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Cubical_gaussian_map_geo::cgm_nodes_handle);
  s_prototype->add_field_info(new MF_shared_container(GEOMETRIES,
                                                      "geometries",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      cgm_nodes_func));
}

//! \brief
void Cubical_gaussian_map_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief
SGAL::Container_proto* Cubical_gaussian_map_geo::get_prototype()
{
  if (!s_prototype) Cubical_gaussian_map_geo::init_prototype();
  return s_prototype;
}

//! \brief raises the flag that indicates that the sphere bound changed.
void Cubical_gaussian_map_geo::draw_changed(const Field_info* /* field_info */)
{
  m_draw_primal = !m_draw_aos;
  m_dirty_bounding_sphere = true;

  if (m_draw_aos) {
    Field* field = get_field(TRUE_DRAW_DUAL);
    if (field) field->cascade();

    if (m_draw_aos_unfolded) {
      Field* field = get_field(TRUE_DRAW_DUAL_UNFOLDED);
      if (field) field->cascade();
    }
  }
  else {
    Field* field = get_field(TRUE_DRAW_PRIMAL);
    if (field) field->cascade();
  }
}

//! \brief draws the internal representation.
void Cubical_gaussian_map_geo::draw_geometry(SGAL::Draw_action* action)
{
  if (!m_draw_aos) {
    draw_primal(action);
    return;
  }

  if (m_draw_aos_unfolded) {
    draw_aos_unfolded(action);
    return;
  }

  if (m_renderer_dirty) clean_renderer();
  if (m_draw_aos_opaque) draw_aos_opaque(action);
  else m_renderer(action);
}

//! \brief draws the polyhedron directly from the gaussian map representation.
void Cubical_gaussian_map_geo::draw_primal(Draw_action* action)
{
  SGAL_TRACE_MSG(SGAL::Trace::CUBICAL_GAUSSIAN_MAP, "draw_primal()\n");

  Context* context = action->get_context();
  glColor3fv((float*)&m_marked_facet_color);

  glFrontFace((is_ccw()) ? GL_CW : GL_CCW);

  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; ++i) {
    const Arrangement& arr = m_cgm.arrangement(i);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (!vit->is_real()) continue;

      if (m_draw_marked_facet && vit->marked())
        context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);

      glBegin(GL_POLYGON);
      const Vector3f& normal = vit->get_normal();
      glNormal3fv((float*)&normal);

      Halfedge_around_vertex_const_circulator hec(vit->incident_halfedges());
      Halfedge_around_vertex_const_circulator begin_hec = hec;
      Point_to_vector3f convert;
      do {
        const Point_3& p = (*hec).face()->point();
        Vector3f vec = convert(p);
        glVertex3fv((float*)&vec);
        ++hec;
      } while (hec != begin_hec);
      glEnd();

      if (m_draw_marked_facet && vit->marked())
        context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
    }
  }
  glFrontFace(GL_CCW);
  glColor3f(1.0f, 1.0f, 1.0f);

  if (m_draw_marked_vertex) draw_primal_marked_vertex(action);
  if (m_draw_marked_edge) draw_primal_marked_edge(action);
}

//! \brief draws the primal marked vertex.
void Cubical_gaussian_map_geo::draw_primal_marked_vertex(Draw_action* action)
{
  if (!m_vertex_geom) m_vertex_geom = new Sphere();
  m_vertex_geom->set_radius(m_marked_vertex_radius);

  Context* context = action->get_context();

  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; i++) {
    const Arrangement& arr = m_cgm.arrangement(i);
    Arr_face_const_iterator fi = arr.faces_begin();
    // Skip the unbounded face
    for (++fi; fi != arr.faces_end(); ++fi) {
      if (fi->marked()) {
        const Point_3& p = fi->point();
        Point_to_vector3f convert;
        Vector3f vec = convert(p);

        glPushMatrix();
        glTranslatef(vec[0], vec[1], vec[2]);
        glColor3fv((float*)&m_marked_vertex_color);
        context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
        m_vertex_geom->draw(action);
        context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
        glPopMatrix();
        return;
      }
    }
  }
}

//! \brief draws the primal marked edge.
void Cubical_gaussian_map_geo::draw_primal_marked_edge(Draw_action* action)
{
  if (!m_edge_geom) m_edge_geom = new Cylinder();
  m_edge_geom->set_radius(m_marked_edge_radius);

  Context* context = action->get_context();

  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; i++) {
    const Arrangement& arr = m_cgm.arrangement(i);
    Arr_halfedge_const_iterator hi;
    for (hi = arr.halfedges_begin(); hi != arr.halfedges_end(); ++hi) {
      if (hi->marked()) {
        Arr_face_const_handle face1 = hi->face();
        Arr_face_const_handle face2 = hi->twin()->face();

        const Point_3& point1 = face1->point();
        const Point_3& point2 = face2->point();

        Point_to_vector3f convert;
        Vector3f vec1 = convert(point1);
        Vector3f vec2 = convert(point2);

        Vector3f dst;
        dst.sub(vec2, vec1);
        Vector3f src(0,1,0);
        Rotation rot;
        rot.make(src, dst);

        Vector3f translation;
        translation.add(vec1, vec2);
        translation.scale(0.5f);

        float dist = vec1.distance(vec2);
        m_edge_geom->set_height(dist);

        Transform trans;
        trans.set_translation(translation);
        trans.set_rotation(rot);
        const auto& mat = trans.get_matrix();

        glPushMatrix();
        glMultMatrixf((float*)&mat);
        glColor3fv((float*)&m_marked_edge_color);
        context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
        m_edge_geom->draw(action);
        context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
        glPopMatrix();
        return;
      }
    }
  }
}

//! \brief
void Cubical_gaussian_map_geo::isect_primary()
{
  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; i++) {
    const Arrangement& arr = m_cgm.arrangement(i);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (!vit->is_real()) continue;

      glBegin(GL_POLYGON);
      Halfedge_around_vertex_const_circulator hec(vit->incident_halfedges());
      Arr_halfedge_around_vertex_const_circulator begin_hec = hec;
      Point_to_vector3f convert;
      do {
        const Point_3& p = (*hec).face()->point();
        Vector3f vec = convert(p);
        glVertex3fv((float*)&vec);
        ++hec;
      } while (hec != begin_hec);
      glEnd();
    }
  }
}

//! \brief draws the dual representation opaque.
void Cubical_gaussian_map_geo::draw_aos_opaque(Draw_action* action)
{
  SGAL::Context* context = action->get_context();

  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; i++) {
    // Draw without updating the depth buffer:
    context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
    context->draw_depth_mask(false);
    glColor3fv((float*)&m_aos_surface_color);
    draw_cube_face(i);
    if (m_draw_marked_vertex) {
      glColor3fv((float*)&m_marked_vertex_color);
      draw_aos_marked_face(i);
    }

    if (m_draw_marked_edge) {
      glColor3fv((float*)&m_marked_edge_color);
      draw_aos_marked_edge(i);
    }

    if (m_draw_marked_facet) {
      glColor3fv((float*)&m_marked_facet_color);
      draw_aos_marked_vertex(i);
    }

    // Draw the projection:
    context->draw_light_enable(false);
    glColor3fv((float*)&m_aos_edge_color[0]);
    draw_projection(action, i, m_aos_edge_line_width);

    // Update the depth buffer only:
    context->draw_depth_mask(true);
    context->draw_color_mask(Vector4ub(0x0, 0x0, 0x0, 0x0));
    draw_cube_face(i);
    context->draw_light_enable(true);
    context->draw_color_mask(Vector4ub(0xff, 0xff, 0xff, 0xff));
  }

  context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  context->draw_line_width(1.0f);
}

//! \brief draws the dual marked vertex.
void Cubical_gaussian_map_geo::draw_aos_marked_vertex(unsigned int id)
{
  float radius = m_aos_marked_vertex_radius;
  const Arrangement& arr = m_cgm.arrangement(id);
  // Skip the unbounded face
  for (auto vi = arr.vertices_begin(); vi != arr.vertices_end(); ++vi) {
    if (!vi->marked()) continue;

    const Point_2& point = vi->point();
    float x = (float) CGAL::to_double(point.x());
    float y = (float) CGAL::to_double(point.y());

    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id >= 3) std::swap(j, k);

    // draw a disk:
    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f vec;
    vec[i] = normal[i];

    glBegin(GL_POLYGON);
    glNormal3fv((float*)&normal);
    float angle = 0;
    float delta = SGAL_TWO_PI / 16;
    for (unsigned int i = 0; i < 16; ++i) {
      vec[k] = x + radius * cosf(angle);
      vec[j] = y + radius * sinf(angle);
      angle += delta;
      glVertex3fv((float*)&vec);
    }

    glEnd();
  }
}

//! \brief draws the dual representation of the polyhedron in 2D.
void Cubical_gaussian_map_geo::draw_aos_unfolded(SGAL::Draw_action* action)
{
  if (is_dirty_cgm()) clean_cgm();

  SGAL::Context* context = action->get_context();

  context->draw_depth_enable(false);
  context->draw_light_enable(false);

  // SGAL::Vector2f trans[] = {{1,1}, {2,0}, {0,-2}, {2,2}, {2,0}, {-4,2}};
  SGAL::Vector2f trans[6];
  trans[0] = Vector2f(1, 3);
  trans[1] = Vector2f(3, 3);
  trans[2] = Vector2f(3, 5);
  trans[3] = Vector2f(5, 3);
  trans[4] = Vector2f(7, 3);
  trans[5] = Vector2f(3, 1);

  SGAL::Vector2f scale[6];
  scale[0] = Vector2f(-1, -1);
  scale[1] = Vector2f(1, -1);
  scale[2] = Vector2f(1, 1);
  scale[3] = Vector2f(1, -1);
  scale[4] = Vector2f(-1, -1);
  scale[5] = Vector2f(1, -1);

#if 0
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
#endif

  float edge_line_width = m_aos_edge_line_width * 2;

  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; i++) {
    const Arrangement& arr = m_cgm.arrangement(i);

    glPushMatrix();
    glTranslatef(trans[i][0], trans[i][1], 0);

    // Draw the background:
    if (m_draw_aos_surface) {
      glColor3fv((float*)&m_aos_surface_color);
      glBegin(GL_POLYGON);
      Vector2f vec(-1, -1);
      glVertex2fv((float*)&vec);
      vec[0] = 1;  glVertex2fv((float*)&vec);
      vec[1] = 1;  glVertex2fv((float*)&vec);
      vec[0] = -1; glVertex2fv((float*)&vec);
      glEnd();
    }

    // Draw the curves:
    glScalef(scale[i][0], scale[i][1], 1);
    Vector2f src;
    Vector2f trg;

    for (auto hei = arr.edges_begin(); hei != arr.edges_end(); ++hei) {
      float line_width = (hei->is_real()) ?
        edge_line_width : m_aos_edge_line_width;
      context->draw_line_width(line_width);

      unsigned int j = ((i == 0) || (i == 2) || (i == 4)) ? 1 : 0;
      unsigned int k = ((i == 0) || (i == 2) || (i == 4)) ? 0 : 1;

      const X_monotone_curve_2& curve = hei->curve();
      const Point_2& source = curve.source();
      const Point_2& target = curve.target();

      src[j] = (float) CGAL::to_double(source.x());
      src[k] = (float) CGAL::to_double(source.y());

      trg[j] = (float) CGAL::to_double(target.x());
      trg[k] = (float) CGAL::to_double(target.y());

      glBegin(GL_LINES);
      Uint num = 0;
      Vector3f color;
      for (Uint id = 0; id < 2; ++id) {
        if (hei->is_arr(id)) {
          color.add(m_aos_edge_color[id]);
          ++num;
        }
      }
      if (num != 0) color.scale(1.0f / num);
      else color = m_aos_edge_color[0];
      glColor3fv((float*) &color);
      glVertex2fv((float*)&src);
      glVertex2fv((float*)&trg);
      glEnd();
    }

    // Restore the modelview matrix:
    glPopMatrix();
  }

  // Restore default:
  context->draw_light_enable(true);
  context->draw_depth_enable(true);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  context->draw_line_width(1.0f);
}

/*! \brief draws the face of the unit cube. */
void Cubical_gaussian_map_geo::draw_cube_face(unsigned int id)
{
  unsigned int i = (id + 0) % 3;
  unsigned int j = (id + 1) % 3;
  unsigned int k = (id + 2) % 3;
  if (id >= 3) std::swap(j, k);
  glBegin(GL_POLYGON);
  Vector3f normal;
  normal[i] = (id < 3) ? -1.0f : 1.0f;
  normal[j] = normal[k] = 0;
  glNormal3fv((float*)&normal);
  Vector3f vec;
  vec[i] = normal[i];
  vec[j] = vec[k] = -1;
  glVertex3fv((float*)&vec);
  vec[k] = 1;  glVertex3fv((float*)&vec);
  vec[j] = 1;  glVertex3fv((float*)&vec);
  vec[k] = -1; glVertex3fv((float*)&vec);
  glEnd();
}

//! \brief draws the dual marked face.
void Cubical_gaussian_map_geo::draw_aos_marked_face(unsigned int id)
{
  const Arrangement& arr = m_cgm.arrangement(id);
  Arr_face_const_iterator fi = arr.faces_begin();
  // Skip the unbounded face
  for (++fi; fi != arr.faces_end(); ++fi) {
    if (!fi->marked()) continue;

    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id >= 3) std::swap(j, k);
    glBegin(GL_POLYGON);
    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;
    glNormal3fv((float*)&normal);
    Vector3f vec;
    vec[i] = normal[i];
    Arr_ccb_halfedge_const_circulator hec = fi->outer_ccb();
    Arr_ccb_halfedge_const_circulator hec_begin = hec;
    do {
      const Point_2& p = hec->source()->point();
      vec[k] = (float) CGAL::to_double(p.x());
      vec[j] = (float) CGAL::to_double(p.y());
      glVertex3fv((float*)&vec);
      hec++;
    } while(hec != hec_begin);
    glEnd();
  }
}

//! \brief draws the dual marked edge.
void Cubical_gaussian_map_geo::draw_aos_marked_edge(unsigned int id)
{
  float radius = m_aos_edge_radius;
  unsigned int i = (id + 0) % 3;
  unsigned int j = (id + 1) % 3;
  unsigned int k = (id + 2) % 3;
  if (id < 3) std::swap(j, k);
  Vector3f src;
  Vector3f trg;
  src[i] = trg[i] = (id < 3) ? -1.0f : 1.0f;
  const Arrangement& arr = m_cgm.arrangement(id);
  for (auto ei = arr.edges_begin(); ei != arr.edges_end(); ++ei) {
    if (!ei->marked()) continue;

    Arr_vertex_const_handle source_vertex = ei->source();
    Arr_vertex_const_handle target_vertex = ei->target();

    const Point_2& source_point = source_vertex->point();
    const Point_2& target_point = target_vertex->point();

    src[j] = (float) CGAL::to_double(source_point.x());
    src[k] = (float) CGAL::to_double(source_point.y());

    trg[j] = (float) CGAL::to_double(target_point.x());
    trg[k] = (float) CGAL::to_double(target_point.y());

    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f dir;
    dir.sub(trg, src);
    dir.normalize();

    Vector3f perp;
    perp.cross(dir, normal);

    Vector3f ext(perp);
    ext.scale(radius);

    Vector3f vec0(src), vec1(trg), vec2(trg), vec3(src);
    if (true || source_vertex->get_location() == Arr_vertex::Interior) {
      vec0.add(ext);
      vec3.sub(ext);
    }
    else if (source_vertex->get_location() == Arr_vertex::Corner) {
      Vector3f par0;
      par0[j] = -src[k];
      float a0 = radius / par0.dot(perp);
      vec0[j] -= a0 * src[k];

      Vector3f par3;
      par3[k] = -src[j];
      float a3 = radius / par3.dot(perp);
      vec3[k] -= a3 * src[j];
    } else {
      if (src[j] == 1 || src[j] == -1) std::swap(j, k);
      Vector3f par;
      par[j] = -src[k];
      float a = radius / par.dot(perp);
      vec0[j] -= a * src[k];
      vec3[j] += a * src[k];
    }

    if (true || target_vertex->get_location() == Arr_vertex::Interior) {
      vec1.add(ext);
      vec2.sub(ext);
    } else if (target_vertex->get_location() == Arr_vertex::Corner) {
      Vector3f par1;
      par1[j] = -src[k];
      float a1 = radius / par1.dot(perp);
      vec1[j] -= a1 * src[k];

      Vector3f par2;
      par2[k] = -src[j];
      float a2 = radius / par2.dot(perp);
      vec2[k] -= a2 * src[j];
    } else {
      if (trg[j] == 1 || trg[j] == -1) std::swap(j, k);
      Vector3f par;
      par[j] = -trg[k];
      float a = radius / par.dot(perp);
      vec1[j] -= a * trg[k];
      vec2[j] += a * trg[k];
    }
    glBegin(GL_QUADS);
    glNormal3fv((float*)&normal);
    glVertex3fv((float*)&vec0);
    glVertex3fv((float*)&vec1);
    glVertex3fv((float*)&vec2);
    glVertex3fv((float*)&vec3);
    glEnd();
  }
}

//! \brief draws a dual vertex.
void Cubical_gaussian_map_geo::draw_aos_vertex(Draw_action* action,
                                               const Vector3f& center,
                                               const Vector3f& normal)
{
  draw_vertex_on_cube(action, center, normal,
                      m_aos_vertex_style, m_aos_vertex_radius,
                      m_aos_delta_angle);
}

//! \brief draws a dual nonreal vertex.
void Cubical_gaussian_map_geo::draw_aos_nonreal_vertex(Draw_action* action,
                                                       const Vector3f& center,
                                                       const Vector3f& normal)
{
  draw_vertex_on_cube(action, center, normal,
                      m_aos_nonreal_vertex_style, m_aos_nonreal_vertex_radius,
                      m_aos_delta_angle);
}

//! \brief draws a dual marked_vertex.
void Cubical_gaussian_map_geo::draw_aos_marked_vertex(Draw_action* action,
                                                      const Vector3f& center,
                                                      const Vector3f& normal)
{
  draw_vertex_on_cube(action, center, normal,
                      m_aos_marked_vertex_style, m_aos_marked_vertex_radius,
                      m_aos_delta_angle);
}

//! \brief draws a dual edge.
void Cubical_gaussian_map_geo::draw_aos_edge(Draw_action* action,
                                             const Vector3f& src,
                                             const Vector3f& trg,
                                             const Vector3f& normal)
{
  draw_edge_on_cube(action, src, trg, normal, m_aos_edge_style,
                    m_aos_edge_radius);
}

//! \brief draws a dual nonreal-edge.
void Cubical_gaussian_map_geo::draw_aos_nonreal_edge(Draw_action* action,
                                                     const Vector3f& src,
                                                     const Vector3f& trg,
                                                     const Vector3f& normal)
{
  draw_edge_on_cube(action, src, trg, normal, m_aos_nonreal_edge_style,
                    m_aos_nonreal_edge_radius);
}

//! \brief draws the dual vertices.
void Cubical_gaussian_map_geo::draw_aos_vertices(Draw_action* action)
{
  for (unsigned int id = 0; id < Polyhedral_cgm::NUM_FACES; ++id) {
    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id < 3) std::swap(j, k);

    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f center;
    center[i] = (id < 3) ? -1.0f : 1.0f;

    const Arrangement& arr = m_cgm.arrangement(id);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (!vit->is_real() || vit->marked()) continue;

      const Point_2& point = vit->point();
      center[j] = static_cast<float>(CGAL::to_double(point.x()));
      center[k] = static_cast<float>(CGAL::to_double(point.y()));

      draw_aos_vertex(action, center, normal);
    }
  }
}

//! \brief draws the dual nonreal vertices.
void Cubical_gaussian_map_geo::draw_aos_nonreal_vertices(Draw_action* action)
{
  for (unsigned int id = 0; id < Polyhedral_cgm::NUM_FACES; ++id) {
    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id < 3) std::swap(j, k);

    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f center;
    center[i] = (id < 3) ? -1.0f : 1.0f;

    const Arrangement& arr = m_cgm.arrangement(id);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (vit->is_real()) continue;

      const Point_2& point = vit->point();
      center[j] = static_cast<float>(CGAL::to_double(point.x()));
      center[k] = static_cast<float>(CGAL::to_double(point.y()));

      draw_aos_nonreal_vertex(action, center, normal);
    }
  }
}

//! \brief draws the dual marked vertices.
void Cubical_gaussian_map_geo::draw_aos_marked_vertices(Draw_action* action)
{
  for (unsigned int id = 0; id < Polyhedral_cgm::NUM_FACES; ++id) {
    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id < 3) std::swap(j, k);

    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f center;
    center[i] = (id < 3) ? -1.0f : 1.0f;

    const Arrangement& arr = m_cgm.arrangement(id);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (!vit->marked()) continue;

      const Point_2& point = vit->point();
      center[j] = (float) CGAL::to_double(point.x());
      center[k] = (float) CGAL::to_double(point.y());

      // if (!vit->is_real()) continue;
      draw_aos_marked_vertex(action, center, normal);
    }
  }
}

//! \brief draws the dual edges.
void Cubical_gaussian_map_geo::draw_aos_edges(Draw_action* action)
{
  for (unsigned int id = 0; id < Polyhedral_cgm::NUM_FACES; ++id) {
    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id < 3) std::swap(j, k);
    Vector3f src;
    Vector3f trg;
    src[i] = trg[i] = (id < 3) ? -1.0f : 1.0f;
    const Arrangement& arr = m_cgm.arrangement(id);
    for (auto hei = arr.edges_begin(); hei != arr.edges_end(); ++hei) {
      const X_monotone_curve_2& curve = hei->curve();
      const Point_2& source = curve.source();
      const Point_2& target = curve.target();

      src[j] = (float) CGAL::to_double(source.x());
      src[k] = (float) CGAL::to_double(source.y());

      trg[j] = (float) CGAL::to_double(target.x());
      trg[k] = (float) CGAL::to_double(target.y());

      Vector3f normal;
      normal[i] = (id < 3) ? -1.0f : 1.0f;
      normal[j] = normal[k] = 0;

      draw_aos_edge(action, src, trg, normal);
      if (hei->is_real())
        draw_aos_edge(action, src, trg, normal);
      else
        draw_aos_nonreal_edge(action, src, trg, normal);
    }
  }
}

//! \brief draws the planar map associated with a face of the unit cube.
void Cubical_gaussian_map_geo::draw_projection(SGAL::Draw_action* action,
                                               unsigned int id,
                                               float non_edge_line_width)
{
  SGAL::Context* context = action->get_context();
  float edge_line_width = m_aos_edge_line_width;
  unsigned int i = (id + 0) % 3;
  unsigned int j = (id + 1) % 3;
  unsigned int k = (id + 2) % 3;
  if (id < 3) std::swap(j, k);
  Vector3f src;
  Vector3f trg;
  src[i] = trg[i] = (id < 3) ? -1.0f : 1.0f;
  const Arrangement& arr = m_cgm.arrangement(id);
  for (auto hei = arr.edges_begin(); hei != arr.edges_end(); ++hei) {
    if (m_draw_marked_edge && hei->marked()) continue;

    const X_monotone_curve_2& curve = hei->curve();
    const Point_2& source = curve.source();
    const Point_2& target = curve.target();

    src[j] = (float) CGAL::to_double(source.x());
    src[k] = (float) CGAL::to_double(source.y());

    trg[j] = (float) CGAL::to_double(target.x());
    trg[k] = (float) CGAL::to_double(target.y());

    float line_width = (hei->is_real()) ?
      edge_line_width :  non_edge_line_width;
    context->draw_line_width(line_width);

    if (m_minkowski_sum) {
      Uint num = 0;
      Vector3f color;
      for (Uint id = 0; id < 2; ++id) {
        if (hei->is_arr(id)) {
          color.add(m_aos_edge_color[id]);
          ++num;
        }
      }
      SGAL_assertion(num != 0);
      color.scale(1.0f / num);
      glColor3fv((float*) &color);
    }

    glBegin(GL_LINES);
    glVertex3fv((float*)&src);
    glVertex3fv((float*)&trg);
    glEnd();
  }
}

/*! Print statistics */
void Cubical_gaussian_map_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (is_dirty_cgm()) clean_cgm();

  if (m_minkowski_sum)
    std::cout << "Minkowski sum took " << m_time << " seconds.\n";
  else
    std::cout << "Construction took " << m_time << " seconds." << std::endl;
  m_cgm.print_stat();
}

/*! \brief computes the planes and the normals of the dual facets and store
 * them at the vertices of the planar map.
 */
void Cubical_gaussian_map_geo::process_facets()
{
  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; ++i) {
    Arrangement& arr = m_cgm.arrangement(i);
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
      if (!vit->is_real()) continue;
      Vector_3 exact_normal = m_cgm.calculate_normal(i, vit->point());
      float x = (float) CGAL::to_double(exact_normal.x());
      float y = (float) CGAL::to_double(exact_normal.y());
      float z = (float) CGAL::to_double(exact_normal.z());
      SGAL::Vector3f normal(x,y,z);
      normal.normalize();
      vit->set_normal(normal);
    }
  }
}

//! \brief sets the source gausian maps of the minkowski sum.
void Cubical_gaussian_map_geo::insert_cgm(Shared_cubical_gaussian_map_geo cgm)
{
  m_cgm_nodes.push_back(cgm);
  Observer observer(this, get_field_info(GEOMETRIES));
  cgm->register_observer(observer);
  m_dirty_bounding_sphere = true;
}

//! \brief increases the vertex index.
void Cubical_gaussian_map_geo::
increase_vertex_index(const Field_info* /* field_info */)
{
  m_marked_vertex_index++;
  if (m_marked_vertex_index == m_cgm.number_of_vertices())
    m_marked_vertex_index = 0;
  clear_cgm();
}

//! \brief increases the face index.
void Cubical_gaussian_map_geo::
increase_edge_index(const Field_info* /* field_info */)
{
  m_marked_edge_index++;
  if (m_marked_edge_index == m_cgm.number_of_edges())
    m_marked_edge_index = 0;
  clear_cgm();
}

//! \brief increases the face index.
void Cubical_gaussian_map_geo::
increase_facet_index(const Field_info* /* field_info */)
{
  m_marked_facet_index++;
  if (m_marked_facet_index == m_cgm.number_of_facets())
    m_marked_facet_index = 0;
  clear_cgm();
}

//! \brief creates the renderers.
void Cubical_gaussian_map_geo::create_renderers()
{
  m_aos_marked_face_renderer = new Marked_face_renderer(*this);

  m_surface_renderer = new Surface_renderer();
  Vector4f color;
  color[0] = m_aos_surface_color[0];
  color[1] = m_aos_surface_color[1];
  color[2] = m_aos_surface_color[2];
  color[3] = 0.5f;
  m_colored_surface_renderer = new Colored_surface_renderer(color);
  m_stencil_surface_renderer = new Stencil_surface_renderer();

  m_vertices_renderer = new Cgm_vertices_renderer(*this);
  m_colored_vertices_renderer = new Colored_vertices_renderer(*this);
  m_nonreal_vertices_renderer = new Nonreal_vertices_renderer(*this);
  m_colored_nonreal_vertices_renderer =
    new Colored_nonreal_vertices_renderer(*this);
  m_marked_vertices_renderer = new Marked_vertices_renderer(*this);
  m_colored_marked_vertices_renderer =
    new Colored_marked_vertices_renderer(*this);

  m_edges_renderer = new Cgm_edges_renderer(*this);
  m_inflated_line_edges_renderer = new Cgm_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer = new Cgm_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer = new Cgm_inflated_tube_edges_renderer(*this);
  m_colored_edges_renderer = new Colored_edges_renderer(*this);
}

//! \brief destroys the renderers.
void Cubical_gaussian_map_geo::destroy_renderers()
{
  if (m_aos_marked_face_renderer) delete m_aos_marked_face_renderer;
  if (m_surface_renderer) delete m_surface_renderer;
  if (m_colored_surface_renderer) delete m_colored_surface_renderer;
  if (m_stencil_surface_renderer) delete m_stencil_surface_renderer;

  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;

  if (m_nonreal_vertices_renderer) delete m_nonreal_vertices_renderer;
  if (m_colored_nonreal_vertices_renderer)
    delete m_colored_nonreal_vertices_renderer;
  if (m_marked_vertices_renderer) delete m_marked_vertices_renderer;
  if (m_colored_marked_vertices_renderer)
    delete m_colored_marked_vertices_renderer;

  if (m_edges_renderer) delete m_edges_renderer;
  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;
}

//! \brief cleans the renderer.
void Cubical_gaussian_map_geo::clean_renderer()
{
  if (m_draw_marked_vertex)
    m_renderer.push_back(m_aos_marked_face_renderer,
                         Arrangement_renderer::SURFACE);
  if (get_draw_aos_surface())
    m_renderer.push_back(m_colored_surface_renderer,
                         Arrangement_renderer::SURFACE);
  else m_renderer.push_back(m_stencil_surface_renderer,
                            Arrangement_renderer::SURFACE);

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

  if (get_aos_edge_style() == Edge_shape::STRIP) {
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::FRONTFACING);
  } else {
    m_renderer.push_back(m_colored_edges_renderer,
                         Arrangement_renderer::OTHER);
  }

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

   default:
    break;
  }

  switch (get_aos_nonreal_vertex_style()) {
   case Vertex_shape::DISC:
    m_renderer.push_back(m_colored_nonreal_vertices_renderer,
                         Arrangement_renderer::BACKFACING);
    m_renderer.push_back(m_colored_nonreal_vertices_renderer,
                         Arrangement_renderer::FRONTFACING);
    break;

   case Vertex_shape::POINT:
   case Vertex_shape::RING:
   case Vertex_shape::BALL:
    m_renderer.push_back(m_colored_nonreal_vertices_renderer,
                         Arrangement_renderer::OTHER);
    break;

   default:
    break;
  }

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

   default:
    break;
  }

  if (get_draw_aos_surface())
    m_renderer.push_back(m_surface_renderer, Arrangement_renderer::DEPTH);
  if (is_aos_edge_enabled())
    m_renderer.push_back(m_edges_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_vertices_renderer, Arrangement_renderer::DEPTH);
  if (get_aos_nonreal_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_nonreal_vertices_renderer,
                         Arrangement_renderer::DEPTH);
  if (get_aos_marked_vertex_style() != Vertex_shape::NONE)
    m_renderer.push_back(m_marked_vertices_renderer,
                         Arrangement_renderer::DEPTH);

  m_renderer_dirty = false;
}

//! \brief renders the vertices with color.
void Cubical_gaussian_map_geo::Colored_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.get_aos_vertex_color());
  Base::operator()(action);
}

//! \brief renders the marked vertices.
void Cubical_gaussian_map_geo::Marked_vertices_renderer::
operator()(Draw_action* action)
{ m_geo.draw_aos_marked_vertices(action); }

//! \brief renders the marked vertices with color.
void Cubical_gaussian_map_geo::Colored_marked_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_marked_facet_color);
  Marked_vertices_renderer::operator()(action);
}

//! \brief renders the nonreal vertices.
void Cubical_gaussian_map_geo::Nonreal_vertices_renderer::
operator()(Draw_action* action)
{ m_geo.draw_aos_nonreal_vertices(action); }

//! \brief renders the nonreal vertices with color.
void Cubical_gaussian_map_geo::Colored_nonreal_vertices_renderer::
operator()(Draw_action* action)
{
  glColor3fv((float*)&m_geo.m_aos_nonreal_vertex_color);
  Nonreal_vertices_renderer::operator()(action);
}

//! \brief renders the edges with color.
void Cubical_gaussian_map_geo::Colored_edges_renderer::
operator()(Draw_action* action)
{
  for (unsigned int id = 0; id < Polyhedral_cgm::NUM_FACES; ++id) {
    unsigned int i = (id + 0) % 3;
    unsigned int j = (id + 1) % 3;
    unsigned int k = (id + 2) % 3;
    if (id < 3) std::swap(j, k);

    Vector3f normal;
    normal[i] = (id < 3) ? -1.0f : 1.0f;
    normal[j] = normal[k] = 0;

    Vector3f src;
    Vector3f trg;
    src[i] = trg[i] = (id < 3) ? -1.0f : 1.0f;
    const Arrangement& arr = m_geo.m_cgm.arrangement(id);
    for (auto hei = arr.edges_begin(); hei != arr.edges_end(); ++hei) {
      const X_monotone_curve_2& curve = hei->curve();
      const Point_2& source = curve.source();
      const Point_2& target = curve.target();

      src[j] = (float) CGAL::to_double(source.x());
      src[k] = (float) CGAL::to_double(source.y());

      trg[j] = (float) CGAL::to_double(target.x());
      trg[k] = (float) CGAL::to_double(target.y());

      if (!hei->is_real()) {
        glColor3fv((float*) &m_geo.get_aos_nonreal_edge_color());
        m_geo.draw_aos_nonreal_edge(action, src, trg, normal);
        continue;
      }

      if (m_geo.m_minkowski_sum) {
        Uint num = 0;
        Vector3f color;
        for (Uint id = 0; id < 2; ++id) {
          if (hei->is_arr(id)) {
            color.add(m_geo.get_aos_edge_color(id));
            ++num;
          }
        }
        SGAL_assertion(num != 0);
        color.scale(1.0f / num);
        glColor3fv((float*) &color);
      }
      else if (hei->marked()) glColor3fv((float*) &m_geo.m_marked_edge_color);
      else glColor3fv((float*) &m_geo.get_aos_edge_color());

      m_geo.draw_aos_edge(action, src, trg, normal);
    }
  }
}

//! \brief renders the marked primal vertex.
void Cubical_gaussian_map_geo::Marked_face_renderer::
operator()(Draw_action* /* action */)
{
  const Vector3f& color = m_geo.get_marked_vertex_color();
  glColor4f(color[0], color[1], color[2], 0.6f);
  for (unsigned int i = 0; i < Polyhedral_cgm::NUM_FACES; ++i)
    m_geo.draw_aos_marked_face(i);
}

//! \brief processes change of points.
void Cubical_gaussian_map_geo::field_changed(const Field_info* field_info)
{
  clear_cgm();
  Mesh_set::field_changed(field_info);
}

//! \brief processes change of coordinate field.
void Cubical_gaussian_map_geo::coord_changed(const Field_info* field_info)
{
  clear_cgm();
  Mesh_set::coord_changed(field_info);
}

//! \brief draws the geometry.
void Cubical_gaussian_map_geo::draw(Draw_action* action)
{
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (is_dirty_facet_normal_indices()) clean_facet_normal_indices();
  if (is_dirty_facet_color_indices()) clean_facet_color_indices();
  if (is_dirty_facet_tex_coord_indices()) clean_facet_tex_coord_indices();
  if (is_dirty_cgm()) clean_cgm();
  if (is_cgm_empty()) return;

  draw_mesh(action);
}

SGAL_END_NAMESPACE
