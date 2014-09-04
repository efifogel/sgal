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
#include <vector>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/calculate_multiple_normals_per_vertex.hpp"

#include "SCGAL/Exact_polyhedron.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_polyhedron_geo::s_tag = "ExactPolyhedron";
Container_proto* Exact_polyhedron_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Exact_polyhedron_geo, "Exact_polyhedron_geo");

//! \brief constructor.
Exact_polyhedron_geo::Exact_polyhedron_geo(Boolean proto) :
  Boundary_set(proto),
  m_convex_hull(false),
  m_dirty_polyhedron(true),
  m_dirty_polyhedron_edges(true),
  m_dirty_polyhedron_facets(true),
  m_dirty_coord_array(false),
  m_time(0)
{
  if (proto) return;
  m_dirty = false;
  m_surface.set_mesh_set(this);
  //! \todo move crease_angle to here.
  set_crease_angle(0);
  set_normal_per_vertex(true);
  set_color_per_vertex(true);
}

//! \brief destructor.
Exact_polyhedron_geo::~Exact_polyhedron_geo() {}

//! Determine whether the representation is empty.
Boolean Exact_polyhedron_geo::is_empty() const
{
  return m_coord_indices.empty() && m_flat_coord_indices.empty() &&
    m_polyhedron.empty();
}

//! \brief computes the convex hull of the coordinate set.
void Exact_polyhedron_geo::convex_hull()
{
  if (!m_coord_array || (m_coord_array->size() == 0)) return;

  boost::shared_ptr<Exact_coord_array_3d> exact_coords =
    boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);
  if (exact_coords) {
    if (exact_coords->size() > 0)
      CGAL::convex_hull_3(exact_coords->begin(), exact_coords->end(),
                          m_polyhedron);

    /* Compute the index of the vertex into the coordinate array and assign it
     * to the polyhedron-vertex record.
     * \todo make more efficient.
     */
    clean_vertices(exact_coords->begin(), exact_coords->end());
  }
  else {
    boost::shared_ptr<Coord_array_3d> coord_array =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coord_array) {
      if (coord_array->size() > 0) {
        std::vector<Exact_point_3> points;
        points.resize(coord_array->size());
        std::transform(coord_array->begin(), coord_array->end(),
                       points.begin(), Vector_to_point());

        // std::copy(points.begin(), points.end(),
        //           std::ostream_iterator<Exact_point_3>(std::cout, "\n"));

        CGAL::convex_hull_3(points.begin(), points.end(), m_polyhedron);

        /* Compute the index of the vertex into the coordinate array and assign
         * it to the polyhedron-vertex record.
         * \todo make more efficient.
         */
        clean_vertices(points.begin(), points.end());
      }
    }
    else SGAL_error();
  }

  m_dirty_polyhedron = false;
  m_dirty_coord_indices = true;
  m_dirty_flat_coord_indices = true;
}

//! \brief cleans the data structure.
void Exact_polyhedron_geo::clean_polyhedron()
{
  if (!m_coord_array || m_coord_array->size() == 0) return;

  clock_t start_time = clock();
  if (m_convex_hull) convex_hull();
  else {
    m_polyhedron.delegate(m_surface);
#if 0
    if (!m_polyhedron.normalized_border_is_valid())
      m_polyhedron.normalize_border();
#else
    m_polyhedron.normalize_border();
#endif
    m_dirty_polyhedron = false;
  }
  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  m_dirty_polyhedron_edges = true;
  m_dirty_polyhedron_facets = true;
}

//! \brief cleans the polyhedron cells.
void Exact_polyhedron_geo::clean_polyhedron_edges()
{
  m_dirty_polyhedron_edges = false;

//   // Compute the normal used only for drawing the polyhedron
//   std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
//                 [](Exact_polyhedron::Facet& facet)
//                 {
//                   Exact_polyhedron::Halfedge_const_handle h = facet.halfedge();
//                   Kernel kernel;
//                   auto normal =
//                     kernel.construct_cross_product_vector_3_object()
//                     (h->next()->vertex()->point() - h->vertex()->point(),
//                      h->next()->next()->vertex()->point() -
//                      h->next()->vertex()->point());
//                   facet.plane() =
//                     kernel.construct_plane_3_object()(h->vertex()->point(), normal);
//                   std::cout << "plane: " << facet.plane() << std::endl;

// //                   const Vector3f& v1 = to_vector3f(h->vertex()->point());
// //                   h = h->next();
// //                   const Vector3f& v2 = to_vector3f(h->vertex()->point());
// //                   h = h->next();
// //                   const Vector3f& v3 = to_vector3f(h->vertex()->point());
// //                   Vector3f vec1, vec2;
// //                   vec1.sub(v2, v1);
// //                   vec2.sub(v3, v2);
// //                   facet.m_normal.cross(vec1, vec2);
//                   facet.m_normal.set(CGAL::to_double(normal.x()),
//                                      CGAL::to_double(normal.y()),
//                                      CGAL::to_double(normal.z()));
//                   facet.m_normal.normalize();
//                 });

  // Clean the halfedges
  Edge_normal_calculator edge_normal_calculator(get_crease_angle());
  edge_normal_calculator =
    std::for_each(m_polyhedron.edges_begin(), m_polyhedron.edges_end(),
                  edge_normal_calculator);
  m_smooth = edge_normal_calculator.m_smooth;
  m_creased = edge_normal_calculator.m_creased;
}

//! \brief cleans the polyhedron facets.
void Exact_polyhedron_geo::clean_polyhedron_facets()
{
  // Compute the plane equations:
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Plane_equation());

  // Compute the normal used only for drawing the polyhedron
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                Plane_to_normal());

  // // Convert the exact points to approximate used for drawing the polyhedron
  // std::for_each(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
  //               Point_to_vector());

  m_dirty_polyhedron_facets = false;
  m_dirty_polyhedron_edges = true;
}

//! \brief clears the internal representation.
void Exact_polyhedron_geo::clear()
{
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
  m_dirty = true;
}

//! \brief
void Exact_polyhedron_geo::cull(Cull_context& /* cull_context */) {}

//! \brief sets the attributes of this object.
void Exact_polyhedron_geo::set_attributes(Element* elem)
{
  Boundary_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "convexHull") {
      set_convex_hull(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the container prototype.
void Exact_polyhedron_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Boundary_set::get_prototype());
}

//! \brief deletes the container prototype.
void Exact_polyhedron_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Exact_polyhedron_geo::get_prototype()
{
  if (!s_prototype) Exact_polyhedron_geo::init_prototype();
  return s_prototype;
}

//! \brief computes the orientation of a point relative to the polyhedron.
CGAL::Oriented_side Exact_polyhedron_geo::oriented_side(const Exact_point_3& p)
{
  for (auto fi = m_polyhedron.facets_begin(); fi != m_polyhedron.facets_end();
       ++fi)
  {
    const Exact_plane_3& plane = fi->plane();
    CGAL::Oriented_side side =  plane.oriented_side(p);
    if (side == CGAL::ON_NEGATIVE_SIDE) continue;
    return side;
  }
  return CGAL::ON_NEGATIVE_SIDE;
}

//! \brief prints statistics.
void Exact_polyhedron_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();

  std::cout << "Primal"
            << ", no. vertices: " << m_polyhedron.size_of_vertices()
            << ",  no. edges: " << m_polyhedron.size_of_halfedges()/2
            << ",  no. facets: " << m_polyhedron.size_of_facets()
            << std::endl;

  std::cout << "Convex hull took " << m_time << " seconds to compute"
            << std::endl;
}

//! \brief obtains the polyhedron data-structure.
const Exact_polyhedron&
Exact_polyhedron_geo::get_polyhedron(Boolean with_planes)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (with_planes) {
    if (m_dirty_polyhedron_facets) clean_polyhedron_facets();
  }
  return m_polyhedron;
}

//! \brief sets the polyhedron data-structure.
void Exact_polyhedron_geo::set_polyhedron(Exact_polyhedron& polyhedron)
{
  m_polyhedron = polyhedron;
  m_dirty_polyhedron = false;
  if (m_coord_array) m_coord_array->resize(0);
  m_dirty_coord_array = true;
  m_dirty = true;
}

/*! \brief Sets the flag that indicates whether to compute the convex hull
 * of the coordinate set.
 */
void Exact_polyhedron_geo::set_convex_hull(Boolean flag)
{
  if (m_convex_hull == flag) return;
  m_convex_hull = flag;
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
  m_dirty = true;
}

//! \brief processes change of field.
void Exact_polyhedron_geo::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case COORD_ARRAY: clear(); break;
   default: break;
  }
  Boundary_set::field_changed(field_info);
}

//! \brief cleans the representation.
void Exact_polyhedron_geo::clean()
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_coord_array) clean_coord_array();
  if (m_dirty_coord_indices || m_dirty_flat_coord_indices)
    clean_coord_indices();

  Boundary_set::clean();
}

//! \brief cleans the coordinates.
void Exact_polyhedron_geo::clean_coord_array()
{
  m_dirty_coord_array = false;
  m_dirty_coord_indices = true;
  m_dirty_flat_coord_indices = true;
  m_dirty_normals = true;
  if (m_polyhedron.empty()) return;

  //! \todo handle the case of Exact_coord_array_3d
  if (!m_coord_array) {
    Uint size = m_polyhedron.size_of_vertices();
    m_coord_array.reset(new Coord_array_3d(size));
    SGAL_assertion(m_coord_array);
  }
  else m_coord_array->resize(m_polyhedron.size_of_vertices());

  boost::shared_ptr<Coord_array_3d> coords =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);

  /* Generate the coordinate array and assign the index into the coordinate
   * array of the vertex to the vertex.
   */
  Uint index = 0;
  auto cit = coords->begin();
  for (auto vit = m_polyhedron.vertices_begin();
       vit != m_polyhedron.vertices_end(); ++vit)
  {
    vit->m_index = index++;
    *cit++ = to_vector3f(vit->point());
  }
}

//! \brief cleans the coordinate indices.
void Exact_polyhedron_geo::clean_coord_indices()
{
  if (m_polyhedron.empty() || !m_coord_array || (m_coord_array->size() == 0)) {
    m_dirty_coord_indices = false;
    m_dirty_flat_coord_indices = false;
    return;
  }

  set_num_primitives(m_polyhedron.size_of_facets());

  bool triangles(true);
  bool quads(true);
  Uint size = 0;
  for (auto fit = m_polyhedron.facets_begin();
       fit != m_polyhedron.facets_end(); ++fit)
  {
    Exact_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
    size_t circ_size = CGAL::circulator_size(hh);
    size += circ_size;
    if (circ_size != 3) triangles = false;
    if (circ_size != 4) quads = false;
  }
  SGAL_assertion(triangles && quads);

  if (!triangles && !quads) {
    set_primitive_type(PT_POLYGONS);
    size += m_polyhedron.size_of_facets();
  }
  else set_primitive_type(quads ? PT_QUADS : PT_TRIANGLES);

  m_flat_coord_indices.resize(size);

  Uint index = 0;
  auto iit = (!triangles && !quads) ?
    m_coord_indices.begin() : m_flat_coord_indices.begin();
  for (auto fit = m_polyhedron.facets_begin();
       fit != m_polyhedron.facets_end(); ++fit)
  {
    Exact_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
    do {
      *iit++ = hh->vertex()->m_index;
      hh->m_index = index++;
    } while (++hh != fit->facet_begin());
    if (!triangles && !quads) {
      *iit++ = (Uint) -1;
      ++index;
    }
  }

  if (!triangles && !quads) {
    m_dirty_coord_indices = false;
    m_dirty_flat_coord_indices = true;
    m_normal_indices_flat = false;
  }
  else {
    m_dirty_coord_indices = true;
    m_dirty_flat_coord_indices = false;
    m_normal_indices_flat = true;
  }
}

//! \brief claculates the normals in case they are invalidated.
void Exact_polyhedron_geo::clean_normals()
{
  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_polyhedron) clean_polyhedron();
    if (m_dirty_polyhedron_facets) clean_polyhedron_facets();
    if (m_dirty_polyhedron_edges) clean_polyhedron_edges();
    if (m_smooth) calculate_single_normal_per_vertex();
    else if (m_creased) calculate_normal_per_polygon();
    else calculate_multiple_normals_per_vertex();
  }
  else if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle == 0) calculate_normal_per_polygon();
  else SGAL_error();
  m_dirty_normals = false;
  m_normals_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief obtains the ith 3D coordinate.
const Vector3f& Exact_polyhedron_geo::get_coord_3d(Uint i) const
{
  boost::shared_ptr<Coord_array_3d> coords =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (coords) return (*coords)[i];

  boost::shared_ptr<Exact_coord_array_3d> exact_coords =
    boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);
  SGAL_assertion(exact_coords);
  return exact_coords->get_inexact_coord(i);
}

//! \brief calculates the sphere bound.
Boolean Exact_polyhedron_geo::clean_sphere_bound()
{
  if (is_dirty()) clean();
  if (!m_dirty_sphere_bound) return false;

  if (!m_bb_is_pre_set && m_coord_array) {
    boost::shared_ptr<Coord_array_3d> coords =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coords) m_sphere_bound.set_around(coords->begin(), coords->end());
    else {
      boost::shared_ptr<Exact_coord_array_3d> exact_coords =
        boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);
      SGAL_assertion(exact_coords);
      const std::vector<Vector3f>& vecs = exact_coords->get_inexact_coords();
      m_sphere_bound.set_around(vecs.begin(), vecs.end());
    }
  }
  m_dirty_sphere_bound = false;
  return true;
}

//! \brief calculates multiple normals per vertex for all vertices.
void Exact_polyhedron_geo::calculate_multiple_normals_per_vertex()
{
  m_flat_normal_indices.resize(m_flat_coord_indices.size());
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array());
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->clear();

  SGAL::calculate_multiple_normals_per_vertex(m_polyhedron,
                                              m_normal_array,
                                              m_flat_normal_indices);
  m_dirty_normal_indices = true;
  m_dirty_flat_normal_indices = false;
}

SGAL_END_NAMESPACE
