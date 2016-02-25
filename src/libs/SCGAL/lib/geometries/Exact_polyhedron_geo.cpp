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
#include <CGAL/convex_hull_3.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/enum.h>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"
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
#include "SGAL/Epec_polyhedron.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

SGAL_BEGIN_NAMESPACE

/*! \todo one option to construct a polyhedron geo is to compute the convex
 * hull of a set of input coordinates. Currently, we use the field "coordArray"
 * as the input. If this option is desired, the polyhedron intermediate
 * structure is constructed. Then, the "coordArray" field is overriden with
 * the coordinates of the convex hull. We need to introduce a new filed, say
 * "coordSet", which should be used as input. This way, the input field is not
 * overriden.
 */

const std::string Exact_polyhedron_geo::s_tag = "ExactPolyhedron";
Container_proto* Exact_polyhedron_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Exact_polyhedron_geo, "Exact_polyhedron_geo");

//! \brief constructs from the prototype.
Exact_polyhedron_geo::Exact_polyhedron_geo(Boolean proto) :
  Boundary_set(proto),
  m_convex_hull(false),
  m_dirty_volume(true),
  m_dirty_surface_area(true),
  m_dirty_coord_array(true),
  m_dirty_polyhedron(true),
  m_dirty_polyhedron_edges(true),
  m_dirty_polyhedron_facets(true),
  m_consistent(true),
  m_has_singular_vertices(false),
  m_time(0)
{
  if (proto) return;
  m_surface.set_mesh_set(this);
  //! \todo move crease_angle to here.
  set_crease_angle(0);
  set_normal_per_vertex(true);
  set_color_per_vertex(true);
}

//! \brief destructs.
Exact_polyhedron_geo::~Exact_polyhedron_geo() {}

//! \brief sets the attributes of this object.
void Exact_polyhedron_geo::set_attributes(Element* elem)
{
  Boundary_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
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

  // volume
  auto volume_func = static_cast<Float_handle_function>
    (&Exact_polyhedron_geo::volume_handle);
  s_prototype->add_field_info(new SF_float(VOLUME, "volume",
                                           Field_info::RULE_OUT, volume_func));

  // surfaceArea
  auto surface_area_func = static_cast<Float_handle_function>
    (&Exact_polyhedron_geo::surface_area_handle);
  s_prototype->add_field_info(new SF_float(SURFACE_AREA, "surfaceArea",
                                           Field_info::RULE_OUT,
                                           surface_area_func));
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

//! \brief computes the convex hull of the coordinate set.
void Exact_polyhedron_geo::convex_hull()
{
  if (!m_coord_array || m_coord_array->empty()) return;

  auto exact_coords =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
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
        std::vector<Epec_point_3> points;
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
  m_dirty_facet_coord_indices = true;
}

//! \brief cleans the coordinate array and the coordinate indices.
void Exact_polyhedron_geo::clean_coords()
{
  m_dirty_coord_array = false;

  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_polyhedron.empty()) return;

  if (!m_coord_array) {
    auto size = m_polyhedron.size_of_vertices();
    m_coord_array.reset(new Coord_array_3d(size));
    SGAL_assertion(m_coord_array);
  }
  else m_coord_array->resize(m_polyhedron.size_of_vertices());

  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (coords) {
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
  else {
    auto exact_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
    if (exact_coords) {
      Uint index = 0;
      auto cit = exact_coords->begin();
      for (auto vit = m_polyhedron.vertices_begin();
           vit != m_polyhedron.vertices_end(); ++vit)
      {
        vit->m_index = index++;
        *cit++ = vit->point();
      }
    }
    else SGAL_error();
  }

  /* Notice that we call the function of the base calss.
   * In general when the coordinates change, we must invalidate the polyhedron
   * to force cleaning of the polyhedron, so that the change to the coordinates
   * is reflected in the polyhedron. However, clean_coords() could have beeen
   * invoked as a consequence to an update of the polyhedron. Naturally, in this
   * case we do not want to invalidate the polyhedron.
   */
  Boundary_set::coord_content_changed(get_field_info(COORD_ARRAY));

  clean_coord_indices();
}

//! \brief clears the coordinates.
void Exact_polyhedron_geo::clear_coord_array()
{
  m_dirty_coord_array = true;
  if (m_coord_array) m_coord_array->clear();
}

//! \brief obtains the coordinate array.
Exact_polyhedron_geo::Shared_coord_array Exact_polyhedron_geo::get_coord_array()
{
  if (is_dirty_coord_array()) clean_coords();
  return m_coord_array;
}

//! \brief cleans the coordinate indices.
void Exact_polyhedron_geo::clean_coord_indices()
{
  if (m_polyhedron.empty()) {
    m_dirty_coord_indices = false;
    m_dirty_facet_coord_indices = false;
    return;
  }

  set_num_primitives(m_polyhedron.size_of_facets());

  bool triangles(true);
  bool quads(true);
  Uint size = 0;
  for (auto fit = m_polyhedron.facets_begin();
       fit != m_polyhedron.facets_end(); ++fit)
  {
    Epec_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
    size_t circ_size = CGAL::circulator_size(hh);
    size += circ_size;
    if (circ_size != 3) triangles = false;
    if (circ_size != 4) quads = false;
  }
  SGAL_assertion(triangles && quads);

  Uint index = 0;
  if (triangles) {
    set_primitive_type(PT_TRIANGLES);
    auto& coord_indices = empty_triangle_coord_indices();
    coord_indices.resize(get_num_primitives());
    size_t i(0);
    for (auto fit = m_polyhedron.facets_begin();
         fit != m_polyhedron.facets_end(); ++fit, ++i)
    {
      Epec_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
      size_t j(0);
      do {
        coord_indices[i][j++] = hh->vertex()->m_index;
        hh->m_index = index++;
      } while (++hh != fit->facet_begin());
    }
  }
  else if (quads) {
    set_primitive_type(PT_QUADS);
    auto& coord_indices = empty_quad_coord_indices();
    coord_indices.resize(get_num_primitives());
    size_t i(0);
    for (auto fit = m_polyhedron.facets_begin();
         fit != m_polyhedron.facets_end(); ++fit, ++i)
    {
      Epec_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
      size_t j(0);
      do {
        coord_indices[i][j++] = hh->vertex()->m_index;
        hh->m_index = index++;
      } while (++hh != fit->facet_begin());
    }
  }
  else {
    set_primitive_type(PT_POLYGONS);
    auto& coord_indices = empty_polygon_coord_indices();
    coord_indices.resize(get_num_primitives());
    size_t i(0);
    for (auto fit = m_polyhedron.facets_begin();
         fit != m_polyhedron.facets_end(); ++fit, ++i)
    {
      Epec_polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
      size_t circ_size = CGAL::circulator_size(hh);
      coord_indices[i].resize(circ_size);
      size_t j(0);
      do {
        coord_indices[i][j++] = hh->vertex()->m_index;
        hh->m_index = index++;
      } while (++hh != fit->facet_begin());
    }
  }

  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;
}

//! \brief cleans the normal array and the normal indices.
void Exact_polyhedron_geo::clean_normals()
{
  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_polyhedron) clean_polyhedron();
    if (m_dirty_polyhedron_facets) clean_polyhedron_facets();
    if (m_dirty_polyhedron_edges) clean_polyhedron_edges();

    if (m_smooth) calculate_single_normal_per_vertex();
    else if (m_creased) calculate_normal_per_facet();
    else calculate_multiple_normals_per_vertex();
  }
  else if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle == 0) calculate_normal_per_facet();
  else SGAL_error();
  m_dirty_normal_array = false;
  m_normal_array_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief cleans the polyhedron data structure.
void Exact_polyhedron_geo::clean_polyhedron()
{
  m_dirty_polyhedron = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  m_consistent = true;

  auto coord_array = get_coord_array();
  if (!coord_array || coord_array->empty()) return;

  clock_t start_time = clock();
  if (m_convex_hull) convex_hull();
  else {
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();

    Orient_polygon_soup_visitor visitor(coord_array);
    m_has_singular_vertices =
      boost::apply_visitor(visitor, m_facet_coord_indices);

    m_polyhedron.delegate(m_surface);
#if 0
    if (!m_polyhedron.normalized_border_is_valid())
      m_polyhedron.normalize_border();
#else
    m_polyhedron.normalize_border();
#endif
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
//                 [](Epec_polyhedron::Facet& facet)
//                 {
//                   Epec_polyhedron::Halfedge_const_handle h = facet.halfedge();
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
void Exact_polyhedron_geo::clear_polyhedron()
{
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
}

//! \brief draws the polygons.
void Exact_polyhedron_geo::draw(Draw_action* action)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_polyhedron() && is_convex_hull()) clean_polyhedron();
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();
  Boundary_set::draw(action);
}

//! \brief draws the polygons for selection.
void Exact_polyhedron_geo::isect(Isect_action* action)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_polyhedron() && is_convex_hull()) clean_polyhedron();
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();
  Boundary_set::isect(action);
}

//! \brief cleans the sphere bound.
void Exact_polyhedron_geo::clean_bounding_sphere()
{
  m_dirty_bounding_sphere = false;
  if (m_bb_is_pre_set) return;

  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_polyhedron() && is_convex_hull()) clean_polyhedron();
  if (is_dirty_coord_array()  ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();

  if (m_coord_array) {
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coords) m_bounding_sphere.set_around(coords->begin(), coords->end());
    else {
      auto exact_coords =
        boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
      SGAL_assertion(exact_coords);
      const auto& vecs = exact_coords->get_inexact_coords();
      m_bounding_sphere.set_around(vecs.begin(), vecs.end());
    }
  }
}

//! \brief computes the orientation of a point relative to the polyhedron.
CGAL::Oriented_side Exact_polyhedron_geo::oriented_side(const Epec_point_3& p)
{
  for (auto fi = m_polyhedron.facets_begin(); fi != m_polyhedron.facets_end();
       ++fi)
  {
    const Epec_plane_3& plane = fi->plane();
    CGAL::Oriented_side side =  plane.oriented_side(p);
    if (side == CGAL::ON_NEGATIVE_SIDE) continue;
    return side;
  }
  return CGAL::ON_NEGATIVE_SIDE;
}

//! \brief obtains the polyhedron data-structure.
const Epec_polyhedron&
Exact_polyhedron_geo::get_polyhedron(Boolean with_planes)
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (with_planes) {
    if (m_dirty_polyhedron_facets) clean_polyhedron_facets();
  }
  return m_polyhedron;
}

//! \brief sets the polyhedron data-structure.
void Exact_polyhedron_geo::set_polyhedron(Epec_polyhedron& polyhedron)
{
  m_polyhedron = polyhedron;
  m_dirty_polyhedron = false;
  clear_coord_array();
  clear_coord_indices();
  clear_facet_coord_indices();
}

/*! \brief Sets the flag that indicates whether to compute the convex hull
 * of the coordinate set.
 */
void Exact_polyhedron_geo::set_convex_hull(Boolean flag)
{
  if (m_convex_hull == flag) return;
  m_convex_hull = flag;
  clear_polyhedron();
}

//! \brief responds to a change in the coordinate array.
void Exact_polyhedron_geo::coord_content_changed(const Field_info* field_info)
{
  m_dirty_coord_array = false;
  clear_polyhedron();
  Boundary_set::coord_content_changed(field_info);
}

//! \brief obtains the ith 3D coordinate.
const Vector3f& Exact_polyhedron_geo::get_coord_3d(Uint i) const
{
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (coords) return (*coords)[i];

  auto exact_coords =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  SGAL_assertion(exact_coords);
  return exact_coords->get_inexact_coord(i);
}

//! \brief calculates multiple normals per vertex for all vertices.
void Exact_polyhedron_geo::calculate_multiple_normals_per_vertex()
{
  resize_facet_indices(m_facet_normal_indices, m_facet_coord_indices);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array());
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->clear();

  SGAL::calculate_multiple_normals_per_vertex(m_polyhedron, m_normal_array,
                                              m_facet_normal_indices);
  m_dirty_normal_indices = true;
  m_dirty_facet_normal_indices = false;
}

//! \brief determines whether the polyhedron is closed.
Boolean Exact_polyhedron_geo::is_closed()
{
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_polyhedron.is_closed();
}

//! \brief computes the volume of the convex hull of the polyhedron.
Float Exact_polyhedron_geo::volume_of_convex_hull()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (is_polyhedron_empty()) return 0.0f;

  // typedef CGAL::Exact_predicates_inexact_constructions_kernel   Epic_kernel;
  Float volume = 0.0f;
  if (is_convex_hull()) {
    typedef CGAL::Triangulation_3<Epec_kernel>                Triangulation;
    Triangulation tri(m_polyhedron.points_begin(), m_polyhedron.points_end());
    for (auto it = tri.finite_cells_begin(); it != tri.finite_cells_end(); ++it)
    {
      auto tetr = tri.tetrahedron(it);
      volume += CGAL::to_double(tetr.volume());
    }
  }
  else {
    Epec_polyhedron ch;
    CGAL::convex_hull_3(m_polyhedron.points_begin(), m_polyhedron.points_end(),
                        ch);

    typedef CGAL::Triangulation_3<Epec_kernel>                Triangulation;
    Triangulation tri(ch.points_begin(), ch.points_end());
    for (auto it = tri.finite_cells_begin(); it != tri.finite_cells_end(); ++it)
    {
      auto tetr = tri.tetrahedron(it);
      volume += CGAL::to_double(tetr.volume());
    }
  }

  return volume;
}

//! \brief prints statistics.
void Exact_polyhedron_geo::print_stat()
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_polyhedron() && is_convex_hull()) clean_polyhedron();
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();

  std::cout << "Container name: " << get_name() << std::endl;
  std::cout << "Container tag: " << get_tag() << std::endl;
  std::cout << "Is closed: " << is_closed() << std::endl;
  std::cout << "# vertices: " << m_polyhedron.size_of_vertices()
            << ", # edges: " << m_polyhedron.size_of_halfedges() / 2
            << ", # facets: " << m_polyhedron.size_of_facets()
            << std::endl;
  std::cout << "Volume of convex hull: " << volume_of_convex_hull()
            << std::endl;

  if (m_convex_hull)
    std::cout << "Convex hull took " << m_time << " seconds to compute"
              << std::endl;
}

//! \brief exports this container.
void Exact_polyhedron_geo::write(Formatter* formatter)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_polyhedron() && is_convex_hull()) clean_polyhedron();
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();
  Boundary_set::write(formatter);
}

//! \brief cleans (compute) the volume.
void Exact_polyhedron_geo::clean_volume()
{
  m_dirty_volume = false;

  m_volume = 0;
  if (is_polyhedron_empty()) return;

  Epec_point_3 origin(CGAL::ORIGIN);
  //! \todo Fix CGAL::volume() to accept CGAL::ORIGIN as an argument.
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                [&](Polyhedron::Facet& facet)
                {
                  SGAL_assertion(3 == CGAL::circulator_size(fit->facet_begin()));
                  auto h = facet.halfedge();
                  auto volume =
                    CGAL::volume(origin, h->vertex()->point(),
                                 h->next()->vertex()->point(),
                                 h->next()->next()->vertex()->point());
                  m_volume += CGAL::to_double(volume);
                });
}

//! \brief cleans (compute) the surface area.
void Exact_polyhedron_geo::clean_surface_area()
{
  m_dirty_surface_area = false;

  m_surface_area = 0;
  if (is_polyhedron_empty()) return;

  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                [&](Polyhedron::Facet& facet)
                {
                  SGAL_assertion(3 == CGAL::circulator_size(fit->facet_begin()));
                  auto h = facet.halfedge();
                  const auto& p1 = h->vertex()->point();
                  const auto& p2 = h->next()->vertex()->point();
                  const auto& p3 = h->next()->next()->vertex()->point();
                  // m_surface_area += CGAL::area(p1, p2, p3);
                  Kernel::Triangle_3 tri(p1, p2, p3);
                  m_surface_area += sqrtf(CGAL::to_double(tri.squared_area()));
                });
}

//! \brief computes the volume of the polyhedron.
Float Exact_polyhedron_geo::volume()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_volume) clean_volume();
  return m_volume;
}

//! \brief computes the surface area of the polyhedron.
Float Exact_polyhedron_geo::surface_area()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_surface_area) clean_surface_area();
  return m_surface_area;
}

//! \brief determines wheather the mesh is consistent.
Boolean Exact_polyhedron_geo::is_consistent()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_consistent;
}

//! \brief determines whether the mesh has singular vertices.
Boolean Exact_polyhedron_geo::has_singular_vertices()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_has_singular_vertices;
}

//! \brief obtains the number of border edges.
size_t Exact_polyhedron_geo::get_number_of_border_edges()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_polyhedron.size_of_border_edges();
}

//! \brief initializes the border edges.
void Exact_polyhedron_geo::init_border_edges(Boolean value)
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (!m_polyhedron.normalized_border_is_valid())
    m_polyhedron.normalize_border();
  auto it = m_polyhedron.border_edges_begin();
  for (; it != m_polyhedron.edges_end(); ++it) it->opposite()->set_flag(value);
}

SGAL_END_NAMESPACE
