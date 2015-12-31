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

#ifndef SCGAL_EXACT_POLYHEDRON_GEO_BUILDER_HPP
#define SCGAL_EXACT_POLYHEDRON_GEO_BUILDER_HPP

#include <CGAL/basic.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

template <typename HDS>
class Exact_polyhedron_geo_builder : public CGAL::Modifier_base<HDS> {
protected:
  /*! The mesh for which a polyhedron data structure is constructed.
   * It is defined as const, because the array of indices of the mesh  may
   * need to be flattened.
   */
  Mesh_set* m_mesh_set;

  /*! Insert the vertices.
   * \param B (in) the halfedge data structure.
   */
  void insert_vertices(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    typedef typename HDS::Vertex Vertex;
    typedef typename Vertex::Point Point;

    boost::shared_ptr<Coord_array> coord_array = m_mesh_set->get_coord_array();

    boost::shared_ptr<Exact_coord_array_3d> exact_coords =
      boost::dynamic_pointer_cast<Exact_coord_array_3d>(coord_array);
    if (exact_coords) {
      for (auto it = exact_coords->begin(); it != exact_coords->end(); ++it)
        B.add_vertex(Point(*it));
      return;
    }
    boost::shared_ptr<Coord_array_3d> coords =
      boost::dynamic_pointer_cast<Coord_array_3d>(coord_array);
    if (coords) {
      for (auto it = coords->begin(); it != coords->end(); ++it)
        B.add_vertex(Point((*it)[0], (*it)[1], (*it)[2]));
      return;
    }
    SGAL_error();
  }

  /*! Insert the faces.
   * \param B (in) the halfedge data structure.
   */
  void insert_faces(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    // Add the faces:
    if (m_mesh_set->is_dirty_facet_coord_indices())
      m_mesh_set->clean_facet_coord_indices();

    switch (m_mesh_set->get_primitive_type()) {
     case Geo_set::PT_TRIANGLES: insert_triangles(B); break;
     case Geo_set::PT_QUADS: insert_quads(B); break;
     case Geo_set::PT_POLYGONS: insert_polygons(B); break;

     case Geo_set::PT_TRIANGLE_STRIP:
     case Geo_set::PT_TRIANGLE_FAN:
     case Geo_set::PT_QUAD_STRIP:
     default: break;
    }
  }

  /*! Insert a triangle.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first point of the face in
   *          the counterclockwise order. In other words, the first point is
   *          coords[indices[j]].
   */
  template <typename InputIterator>
  Uint insert_triangle(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                       InputIterator begin, InputIterator end, Uint j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    he->m_index = j;
    he->next()->m_index = j+1;
    he->next()->next()->m_index = j+2;
    return j+3;
  }

  /*! Insert triangles.
   * \param B (in) the halfedge data structure.
   */
  void insert_triangles(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh_set->get_facet_coord_indices();
    const auto& indices = boost::get<Mesh_set::Triangle_indices>(facet_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_triangle(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert a quadrilateral.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first pointof the face in
   *          the counterclockwise order. In other words, the first point is
   *          coords[indices[j]].
   */
  template <typename InputIterator>
  Uint insert_quad(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                   InputIterator begin, InputIterator end, Uint j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    he->m_index = j;
    he->next()->m_index = j+1;
    he->next()->next()->m_index = j+2;
    he->next()->next()->next()->m_index = j+3;
    return j+4;
  }

  /*! Insert quadrilaterals.
   * \param B (in) the halfedge data structure.
   */
  void insert_quads(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh_set->get_facet_coord_indices();
    const auto& indices = boost::get<Mesh_set::Quad_indices>(facet_indices);
    size_t j(0);
    for (Uint i = 0; i < indices.size(); ++i)
      j = insert_quad(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert a polygon.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first point of the face in
   *          the counterclockwise order. In other words, the first point is
   *          coords[indices[j]].
   */
  template <typename InputIterator>
  Uint insert_polygon(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                      InputIterator begin, InputIterator end, Uint j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    auto start_he = he;
    do {
      he->m_index = j++;
      he = he->next();
    } while (he != start_he);
    return j;
  }

  /*! Insert polygons.
   * \param B (in) the halfedge data structure.
   */
  void insert_polygons(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh_set->get_facet_coord_indices();
    const auto& indices = boost::get<Mesh_set::Polygon_indices>(facet_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_polygon(B, indices[i].begin(), indices[i].end(), j);
  }

public:
  /*! Set the Mesh_set.
   * \param mesh_set (in) the Mesh_set, which provides the
   *                 a. coordinate array, and
   *                 b. coordinate indices
   */
  void set_mesh_set(Mesh_set* mesh_set) { m_mesh_set = mesh_set; }

  /*! Build the polyhedral surface.
   * \param hds (out) the halfedge data structure, which stores the incidence
   *            relations between the cells of the polyhedral surface.
   */
  void operator()(HDS& hds)
  {
    // Postcondition: `hds' is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(m_mesh_set->get_coord_array()->size(),
                    m_mesh_set->get_num_primitives());
    insert_vertices(B);
    insert_faces(B);
    B.end_surface();
  }
};

SGAL_END_NAMESPACE

#endif
