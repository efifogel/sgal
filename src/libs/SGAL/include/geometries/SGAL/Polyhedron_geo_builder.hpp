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

#ifndef SGAL_POLYHEDRON_GEO_BUILDER_HPP
#define SGAL_POLYHEDRON_GEO_BUILDER_HPP

#include <stdexcept>
#include <boost/type_traits.hpp>

#include <CGAL/basic.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

template <typename HDS>
class Polyhedron_geo_builder : public CGAL::Modifier_base<HDS> {
private:
  /*! Indicates wheather the mesh is a consistent. */
  Boolean m_consistent;

protected:
  /*! The mesh for which a polyhedron data structure is constructed.
   * It is defined as const, because the array of indices of the mesh  may
   * need to be flattened.
   */
  Mesh_set* m_mesh;

  /*! Insert the vertices.
   * \param B (in) the halfedge data structure.
   */
  void insert_vertices(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    typedef typename HDS::Vertex Vertex;
    typedef typename Vertex::Point Point;

    boost::shared_ptr<Coord_array_3d> coords =
      boost::static_pointer_cast<Coord_array_3d>(m_mesh->get_coord_array());
    SGAL_assertion(coords);

    // Add the points:
    for (auto it = coords->begin(); it != coords->end(); ++it)
      B.add_vertex(Point((*it)[0], (*it)[1], (*it)[2]));
  }

  /*! Insert a triangle.
   * \param[in] B the halfedge data structure.
   * \param[in] indices the indices of the triangle vertices.
   * \param[in] j the index of the index of the first point of the triangle.
   *              In other words, the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_triangle(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                         InputIterator begin, InputIterator end, size_t j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    SGAL_assertion(B.error());
    he->m_index = j;
    he->next()->m_index = j+1;
    he->next()->next()->m_index = j+2;
    return j+3;
  }

  /*! Insert a quadrilateral.
   * \param[in] B the halfedge data structure.
   * \param[in] indices the indices of the quad vertices.
   * \param[in] j the index of the index of the first point of the quad.
   *              In other words,  the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_quad(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                     InputIterator begin, InputIterator end, size_t j)
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

  /*! Insert a polygon.
   * \param[in] B the halfedge data structure.
   * \param[in] indices the indices of the polygon vertices.
   * \param[in] j the index of the index of the first point of the polygon.
   *              In other words, the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_polygon(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                        InputIterator begin, InputIterator end, size_t j)
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

  /*! Insert triangles.
   * \param B (in) the halfedge data structure.
   */
  void insert_triangles(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh->get_facet_coord_indices();
    const auto& indices = boost::get<Triangle_indices>(facet_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_triangle(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert quadrilaterals.
   * \param B (in) the halfedge data structure.
   */
  void insert_quads(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh->get_facet_coord_indices();
    const auto& indices = boost::get<Quad_indices>(facet_indices);
    size_t j(0);
    for (Uint i = 0; i < indices.size(); ++i)
      j = insert_quad(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert polygons.
   * \param B (in) the halfedge data structure.
   */
  void insert_polygons(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& facet_indices = m_mesh->get_facet_coord_indices();
    const auto& indices = boost::get<Polygon_indices>(facet_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_polygon(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert the faces.
   * \param B (in) the halfedge data structure.
   */
  void insert_faces(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    // Add the faces:
    if (m_mesh->is_dirty_facet_coord_indices())
      m_mesh->clean_facet_coord_indices();
    switch (m_mesh->get_primitive_type()) {
     case Geo_set::PT_TRIANGLES: insert_triangles(B); break;
     case Geo_set::PT_QUADS: insert_quads(B); break;
     case Geo_set::PT_POLYGONS: insert_polygons(B); break;

     case Geo_set::PT_TRIANGLE_STRIP:
     case Geo_set::PT_TRIANGLE_FAN:
     case Geo_set::PT_QUAD_STRIP:
     default: break;
    }
  }

public:
  /*! Construct.
   * \param mesh_set (in) the Mesh_set, which provides the
   *                 a. coordinate array, and
   *                 b. coordinate indices
   */
  Polyhedron_geo_builder(Mesh_set* mesh) : m_mesh(mesh) {}

  /*! Build the polyhedral surface.
   * \param[out] hds the halfedge data structure, which stores the incidence
   *            relations between the cells of the polyhedral surface.
   */
  void operator()(HDS& hds)
  {
    m_consistent = true;

    // Postcondition: `hds' is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(m_mesh->get_coord_array()->size(),
                    m_mesh->get_num_primitives());
    try {
      insert_vertices(B);
      insert_faces(B);
      B.end_surface();
    }
    catch (std::exception& e) {
      m_consistent = false;
      // std::cerr << e.what() << std::endl;
      B.rollback();
    }
  }

  /*! Determine wheather the mesh is consistent.
   * \return true if the the mesh is consistent and false otherwise.
   */
  Boolean is_consistent();
};

//! \brief determines wheather the mesh is consistent.
template <typename HDS>
inline Boolean Polyhedron_geo_builder<HDS>::is_consistent()
{ return m_consistent; }

SGAL_END_NAMESPACE

#endif
