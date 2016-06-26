// Copyright (c) 2016 Israel.
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

#ifndef SGAL_HOLE_FILLER_VISITOR_HPP
#define SGAL_HOLE_FILLER_VISITOR_HPP

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/foreach.hpp>

#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;

SGAL_BEGIN_NAMESPACE

/*! Hole filler visitor fills triangulated holes in a polyhedron that has
 * holes (in other words, the polyhedron has border edges).
 * An object of this class can be used as a (boost) visitor when the polyhedron
 * are stored as a variant (multi-type, single value) of several possible
 * types of polyhedrons.
 */
class SGAL_SGAL_DECL Hole_filler_visitor : public boost::static_visitor<> {
public:
  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;
  typedef boost::shared_ptr<Coord_array_3d>             Shared_coord_array_3d;

  /*! Construct.
   * \param[in] refine
   * \param[in] fair
   * \param[out] coord_array
   */
  Hole_filler_visitor(Boolean refine, Boolean fair, Shared_coord_array coords) :
    m_refine(refine),
    m_fair(fair),
    m_coords(coords)
  {}

  /*! Obtain the new coordinate array.
   * \return the new coordinate array.
   */
  Shared_coord_array get_new_coord_array() const { return m_new_coords; }

  /*! The visitor operator.
   */
  template <typename Indices_>
  void operator()(Inexact_polyhedron& polyhedron, Indices_& indices)
  { SGAL_error_msg("Not implemented yet!"); }

  template <typename Indices_>
  void operator()(Epic_polyhedron& polyhedron, Indices_& indices)
  {
    Epic_kernel kernel;
    fill_holes(polyhedron, kernel, indices);
  }

  template <typename Indices_>
  void operator()(Epec_polyhedron& polyhedron, Indices_& indices)
  { SGAL_error_msg("Not implemented yet!"); }

private:
  /*! Indicates whether to refine the triangulation of a hole by applying
   * local averaging rules.
   */
  Boolean m_refine;

  /*! Indicates whether to smooth the triangulation of a hole to obtain
   * as-smooth-as-possible shape deformation.
   */
  Boolean m_fair;

  Shared_coord_array m_coords;

  Shared_coord_array m_new_coords;

  size_t m_num_filled_holes;

  // Incrementally fill the holes
  template <typename PolygonMesh, typename Kernel, typename Indices_>
  void fill_holes(PolygonMesh& pmesh, Kernel& kernel, Indices_& indices)
  {
    typedef PolygonMesh                                 Polyhedron;
    typedef typename Polyhedron::Halfedge_handle        Halfedge_handle;
    typedef typename Polyhedron::Facet_handle           Facet_handle;

    m_num_filled_holes = 0;
    BOOST_FOREACH(Halfedge_handle h, halfedges(pmesh)) {
      if (!h->is_border()) continue;
      ++m_num_filled_holes;

      std::vector<Facet_handle>  new_facets;
      triangulate_hole(pmesh, h, std::back_inserter(new_facets), kernel);
    }
    // std::cout << m_num_filled_holes << std::endl;
  }
  /*!
   */
  template <typename PolygonMesh, typename OutputIterator, class Kernel_>
  OutputIterator triangulate_hole(PolygonMesh& pmesh,
                                  typename boost::graph_traits<PolygonMesh>::
                                    halfedge_descriptor border_halfedge,
                                  OutputIterator oi,
                                  Kernel_& kernel)
  {
    typedef Kernel_                                     Kernel;
    typedef PolygonMesh                                 Polyhedron;
    typedef typename Polyhedron::Vertex_handle          Vertex_handle;

    typedef CGAL::Halfedge_around_face_circulator<PolygonMesh>
      Hedge_around_face_circulator;
    typedef typename boost::graph_traits<PolygonMesh>::halfedge_descriptor
      halfedge_descriptor;

    auto collinear = kernel.collinear_3_object();
    auto strictly_ordered =
      kernel.collinear_are_strictly_ordered_along_line_3_object();

    halfedge_descriptor h0(border_halfedge);
    auto h1 = CGAL::next(h0, pmesh);
    auto h2 = CGAL::next(h1, pmesh);

    const auto& p1 = CGAL::target(h0, pmesh)->point();
    const auto& p2 = CGAL::target(h1, pmesh)->point();
    const auto& p3 = CGAL::target(h2, pmesh)->point();

    if (collinear(p1, p2, p3)) {
      if (h0 == CGAL::next(h2, pmesh)) {
        std::cout << "Case 0" << std::endl;
        // The hole is a single triangle (of collinear vertices).
        CGAL::Euler::fill_hole(h0, pmesh);
        *oi++ = CGAL::face(h0, pmesh);
        return oi;
      }

      auto h = CGAL::Euler::add_face_to_border(h0, h2, pmesh);
      *oi++ = CGAL::face(h, pmesh);
      return oi;
    }

    if (m_refine || m_fair) {
      std::vector<Vertex_handle> new_vertices;
      bool success(true);
      if (m_fair) {
        success = CGAL::cpp11::get<0>
          (PMP::triangulate_refine_and_fair_hole
           (pmesh, border_halfedge, oi, std::back_inserter(new_vertices),
            PMP::parameters::vertex_point_map(get(CGAL::vertex_point, pmesh)).
            geom_traits(kernel)));
        // std::cout << "1 Number of facets in constructed patch: "
        //           << new_facets.size() << std::endl;
        // std::cout << "1 Number of vertices in constructed patch: "
        //           << new_vertices.size() << std::endl;
        // std::cout << " Fairing : " << (success ? "succeeded" : "failed")
        //           << std::endl;
        if (success) {
          if (! new_vertices.empty()) add_coords(new_vertices);
          return oi;
        }
      }
      PMP::triangulate_and_refine_hole
        (pmesh, border_halfedge, oi, std::back_inserter(new_vertices),
         PMP::parameters::vertex_point_map(get(CGAL::vertex_point, pmesh)).
         geom_traits(kernel));
      // std::cout << "2 Number of facets in constructed patch: "
      //           << new_facets.size() << std::endl;
      // std::cout << "2 Number of vertices in constructed patch: "
      //           << new_vertices.size() << std::endl;
      if (! new_vertices.empty()) add_coords(new_vertices);
      return oi;
    }

    PMP::triangulate_hole
      (pmesh, border_halfedge, oi,
       PMP::parameters::vertex_point_map(get(CGAL::vertex_point, pmesh)).
       geom_traits(kernel));
    // std::cout << "2 Number of facets in constructed patch: "
    //           << new_facets.size() << std::endl;
    // std::cout << "2 Number of vertices in constructed patch: "
    //           << new_vertices.size() << std::endl;

    // if (! new_facets.empty()) {
    //   for (auto facet: new_facets) {
    //   }
    // }
    return oi;
  }

  /*! Add the new coordinates to a new coordinate array.
   * \param[in] new_vertices the new coordinates.
   */
  template <typename Patch_>
  void add_coords(const Patch_& new_vertices)
  {
    auto old_size = m_coords->size();
    auto size = old_size + new_vertices.size();

    // Construct a new coordinate structure and copy the old coordinates
    // to the new structure. This is necessary, because the old one might
    // be used by other elements.
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coords);
    if (coords) {
      auto* new_coords = new Coord_array_3d(size);
      add_coords(new_coords, coords, new_vertices, old_size);
      return;
    }

    auto epec_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coords);
    if (epec_coords) {
      auto* new_coords = new Epec_coord_array_3d(size);
      add_coords(new_coords, epec_coords, new_vertices, old_size);
      return;
    }

    auto epic_coords =
      boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coords);
    SGAL_assertion(epic_coords);
    auto* new_coords = new Epic_coord_array_3d(size);
    add_coords(new_coords, epic_coords, new_vertices, old_size);
  }

  /*! Add the new coordinates to the new coordinate array.
    \param[out] new_coords the new coordinate array.
    \param[in] coords the old coordinate array.
    \param[in] new_vertices the new coordinates.
    \param[in] old_size the number of the old coordinates.
   */
  template <typename Coords_, typename SharedCoords_, typename Patch_>
  void add_coords(Coords_* new_coords, const SharedCoords_ coords,
                  const Patch_& new_vertices, size_t old_size)
  {
    // Copy the old ones.
    auto it = std::copy(coords->begin(), coords->end(), new_coords->begin());

    // Copy the new ones.
    auto index(old_size);
    for (auto vit: new_vertices) {
      vit->set_id(index++);
      assign(*it++, vit->point());
    }

    m_new_coords = Shared_coord_array(new_coords);
  }

  /*! Assign a point to a Vector3f.
   */
  template <typename Point_>
  void assign(Vector3f& res, const Point_& point) const
  { res = to_vector3f(point); }

  /*! Assign an inexact point to an epec point.
   */
  void assign(Epec_point_3& res, const Inexact_point_3& point) const
  { res = Epec_point_3(point.x(), point.y(), point.z()); }

  /*! Assign an epec point to an epec point.
   */
  void assign(Epec_point_3& res, const Epec_point_3& point) const
  { res = point; }

  /*! Assign an ipec point to an epec point.
   */
  void assign(Epec_point_3& res, const Epic_point_3& point) const
  { SGAL_error(); }

  /*! Assign an inexact point to an ipec point.
   */
  void assign(Epic_point_3& res, const Inexact_point_3& point) const
  { res = Epic_point_3(point.x(), point.y(), point.z()); }

  /*! Assign an epec point to an ipec point.
   */
  void assign(Epic_point_3& res, const Epec_point_3& point) const
  { SGAL_error(); }

  /*! Assign an ipec point to an ipec point.
   */
  void assign(Epic_point_3& res, const Epic_point_3& point) const
  { res = point; }
};

SGAL_END_NAMESPACE

#endif
