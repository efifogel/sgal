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

#include <CGAL/basic.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

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
  m_dirty_coord_array(false),
  m_time(0)
{
  if (proto) return;
  m_surface.set_mesh_set(this);
  m_dirty = false;
}

//! \brief destructor.
Exact_polyhedron_geo::~Exact_polyhedron_geo() {}

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
}

#if 0
//! \brief cleans the facets.
void Exact_polyhedron_geo::clean_facets()
{
  // Compute the plane equations:
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Plane_equation());

#if 0
  /* Is there a collision?
   * \todo this doeasn't belong here
   */
  CGAL::Oriented_side side = oriented_side(CGAL::ORIGIN);
  std::cout << ((side == CGAL::ON_ORIENTED_BOUNDARY) ? "On boundary" :
                ((side == CGAL::ON_POSITIVE_SIDE) ? "Outside" : "Inside"))
            << std::endl;
#endif

  // Compute the normal used only for drawing the polyhedron
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                Plane_to_normal());

  // Convert the exact points to approximate used for drawing the polyhedron
  std::for_each(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
                Point_to_vector());

  m_dirty_facets = false;
}
#endif

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
Exact_polyhedron& Exact_polyhedron_geo::get_polyhedron()
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
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
  if (m_polyhedron.empty()) return;

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

  auto iit = (!triangles && !quads) ?
    m_coord_indices.begin() : m_flat_coord_indices.begin();
  for (auto fit = m_polyhedron.facets_begin();
       fit != m_polyhedron.facets_end(); ++fit)
  {
    Exact_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
    do {
      *iit++ = hh->vertex()->m_index;
    } while (++hh != fit->facet_begin());
    if (!triangles && !quads) *iit++ = (Uint) -1;
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
#if 0
  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_polyhedron) clean_polyhedron();
    if (m_smooth) calculate_single_normal_per_vertex();
    else if (m_creased) calculate_normal_per_polygon();
    else calculate_multiple_normals_per_vertex();
  }
  else if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle == 0) calculate_normal_per_polygon();
  else SGAL_assertion();
#else
  calculate_normal_per_polygon();
#endif
  m_dirty_normals = false;
  m_normals_cleaned = true;
  m_dirty_normal_buffer = true;
}

SGAL_END_NAMESPACE
