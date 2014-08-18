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

#ifndef SCGAL_POLYHEDRON_GEO_BUILDER_HPP
#define SCGAL_POLYHEDRON_GEO_BUILDER_HPP

#include <boost/type_traits.hpp>

#include <CGAL/basic.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

template <typename HDS>
class Polyhedron_geo_builder : public CGAL::Modifier_base<HDS> {
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

    boost::shared_ptr<Coord_array_3d> coords =
      boost::static_pointer_cast<Coord_array_3d>(m_mesh_set->get_coord_array());
    SGAL_assertion(coords);

    // Add the points:
    for (auto it = coords->begin(); it != coords->end(); ++it)
      B.add_vertex(Point((*it)[0], (*it)[1], (*it)[2]));
  }

  /*! Insert the faces.
   * \param B (in) the halfedge data structure.
   */
  void insert_faces(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    // Add the faces:
    m_mesh_set->clean_flat_coord_indices();
    if (m_mesh_set->are_coord_indices_flat()) {
      if (m_mesh_set->get_primitive_type() == Geo_set::PT_TRIANGLES) {
        insert_triangles(B);
        return;
      }
      if (m_mesh_set->get_primitive_type() == Geo_set::PT_QUADS) {
        insert_quads(B);
        return;
      }
      SGAL_error();
      return;
    }
    insert_polygons(B);
  }

  /*! Insert a triangle.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first point. In other words,
   *          the first point is coords[indices[j]].
   */
  Uint insert_triangle(CGAL::Polyhedron_incremental_builder_3<HDS>& B, Uint j)
  {
    B.begin_facet();
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j));
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+1));
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+2));
    typename HDS::Halfedge_handle he = B.end_facet();
    he->m_index = j;
    he->next()->m_index = j+1;
    he->next()->next()->m_index = j+2;
    j += 3;
    return j;
  }

  /*! Insert triangles.
   * \param B (in) the halfedge data structure.
   */
  void insert_triangles(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    unsigned int num_facets = m_mesh_set->get_num_primitives();
    Uint j = 0;
    for (Uint i = 0; i < num_facets; ++i) j = insert_triangle(B, j);
  }

  /*! Insert a quadrilateral.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first point. In other words,
   *          the first point is coords[indices[j]].
   */
  Uint insert_quad(CGAL::Polyhedron_incremental_builder_3<HDS>& B, Uint j)
  {
    B.begin_facet();
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j));
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+1));
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+2));
    B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+3));
    typename HDS::Halfedge_handle he = B.end_facet();
    he->m_index = j;
    he->next()->m_index = j+1;
    he->next()->next()->m_index = j+2;
    he->next()->next()->next()->m_index = j+3;
    j += 4;
    return j;
  }

  /*! Insert quadrilaterals.
   * \param B (in) the halfedge data structure.
   */
  void insert_quads(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    unsigned int num_facets = m_mesh_set->get_num_primitives();
    Uint j = 0;
    for (Uint i = 0; i < num_facets; ++i) j = insert_quad(B, j);
  }

  /*! Insert a polygon.
   * \param B (in) the halfedge data structure.
   * \param j (in) the index of the index of the first point. In other words,
   *          the first point is coords[indices[j]].
   */
  Uint insert_polygon(CGAL::Polyhedron_incremental_builder_3<HDS>& B, Uint j)
  {
    Uint k = j;
    B.begin_facet();
    for (; m_mesh_set->get_coord_index(j) != (Uint) -1; ++j)
      B.add_vertex_to_facet(m_mesh_set->get_coord_index(j));
    ++j;
    typename HDS::Halfedge_handle he = B.end_facet();
    typename HDS::Halfedge_handle start_he = he;
    do {
      he->m_index = k++;
      he = he->next();
    } while (he != start_he);
    return j;
  }

  /*! Insert polygons.
   * \param B (in) the halfedge data structure.
   */
  void insert_polygons(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    unsigned int num_facets = m_mesh_set->get_num_primitives();
    Uint j = 0;
    for (Uint i = 0; i < num_facets; ++i) j = insert_polygon(B, j);
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
