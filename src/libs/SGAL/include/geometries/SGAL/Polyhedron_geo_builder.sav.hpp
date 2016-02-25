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

#include <stdexcept>
#include <boost/type_traits.hpp>

#include <CGAL/basic.h>
#include <CGAL/Modifier_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Polyhedron_geo_incremental_builder.hpp"

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
   * \param[in] B the halfedge data structure builder.
   */
  void insert_vertices(Polyhedron_geo_incremental_builder<HDS>& B);

  /*! Insert the faces.
   * \param[in] B the halfedge data structure builder.
   */
  void insert_faces(Polyhedron_geo_incremental_builder<HDS>& B);

  /*! Insert a triangle.
   * \param[in] B the halfedge data structure builder.
   * \param[in] j the index of the index of the first point. In other words,
   *            the first point is coords[indices[j]].
   */
  Uint insert_triangle(Polyhedron_geo_incremental_builder<HDS>& B, Uint j);

  /*! Insert triangles.
   * \param[in] B the halfedge data structure builder.
   */
  void insert_triangles(Polyhedron_geo_incremental_builder<HDS>& B);

  /*! Insert a quadrilateral.
   * \param[in] B the halfedge data structure builder.
   * \param[in] j the index of the index of the first point. In other words,
   *            the first point is coords[indices[j]].
   */
  Uint insert_quad(Polyhedron_geo_incremental_builder<HDS>& B, Uint j);

  /*! Insert quadrilaterals.
   * \param[in] B the halfedge data structure builder.
   */
  void insert_quads(Polyhedron_geo_incremental_builder<HDS>& B);

  /*! Insert a polygon.
   * \param[in] B the halfedge data structure builder.
   * \param[in] j the index of the index of the first point. In other words,
   *            the first point is coords[indices[j]].
   */
  Uint insert_polygon(Polyhedron_geo_incremental_builder<HDS>& B, Uint j);

  /*! Insert polygons.
   * \param[in] B the halfedge data structure builder.
   */
  void insert_polygons(Polyhedron_geo_incremental_builder<HDS>& B);

public:
  /*! Set the Mesh_set.
   * \param mesh_set (in) the Mesh_set, which provides the
   *                 a. coordinate array, and
   *                 b. coordinate indices
   */
  void set_mesh_set(Mesh_set* mesh_set) { m_mesh_set = mesh_set; }

  /*! Build the polyhedral surface.
   * \param[out] hds the halfedge data structure, which stores the incidence
   *            relations between the cells of the polyhedral surface.
   * Postcondition: `hds' is a valid polyhedral surface.
   */
  void operator()(HDS& hds)
  {
    Polyhedron_geo_incremental_builder<HDS> B(hds, true);
    B.begin_surface(m_mesh_set->get_coord_array()->size(),
                    m_mesh_set->get_num_primitives());
    try {
      insert_vertices(B);
      insert_faces(B);
      B.end_surface();
    }
    catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      B.rollback();
    }
  }
};

//! \brief inserts the vertices.
template <typename HDS>
void Polyhedron_geo_builder<HDS>::
insert_vertices(Polyhedron_geo_incremental_builder<HDS>& B)
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

//! \brief inserts the faces.
template <typename HDS>
void Polyhedron_geo_builder<HDS>::
insert_faces(Polyhedron_geo_incremental_builder<HDS>& B)
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

//! \brief inserts a triangle.
template <typename HDS>
Uint Polyhedron_geo_builder<HDS>::
insert_triangle(Polyhedron_geo_incremental_builder<HDS>& B, Uint j)
{
  size_t indices[] = {
    m_mesh_set->get_flat_coord_index(j),
    m_mesh_set->get_flat_coord_index(j+1),
    m_mesh_set->get_flat_coord_index(j+2)
  };
  if (!B.test_facet(&indices[0], &indices[3]))
    throw(std::runtime_error("Error: inconsistent mesh!"));
  auto he = B.add_facet(&indices[0], &indices[3]);
  SGAL_assertion(B.error());
  he->m_index = j;
  he->next()->m_index = j+1;
  he->next()->next()->m_index = j+2;
  j += 3;
  return j;
}

//! \brief inserts triangles.
template <typename HDS>
void Polyhedron_geo_builder<HDS>::
insert_triangles(Polyhedron_geo_incremental_builder<HDS>& B)
{
  auto num_facets = m_mesh_set->get_num_primitives();
  Uint j = 0;
  for (Uint i = 0; i < num_facets; ++i) j = insert_triangle(B, j);
}

//! \brief inserts a quadrilateral.
template <typename HDS>
Uint Polyhedron_geo_builder<HDS>::
insert_quad(Polyhedron_geo_incremental_builder<HDS>& B, Uint j)
{
  size_t indices[] = {
    m_mesh_set->get_flat_coord_index(j),
    m_mesh_set->get_flat_coord_index(j+1),
    m_mesh_set->get_flat_coord_index(j+2),
    m_mesh_set->get_flat_coord_index(j+3)
  };
  if (!B.test_facet(&indices[0], &indices[4]))
    throw(std::runtime_error("Error: inconsistent mesh!"));
  auto he = B.add_facet(&indices[0], &indices[4]);
  he->m_index = j;
  he->next()->m_index = j+1;
  he->next()->next()->m_index = j+2;
  he->next()->next()->next()->m_index = j+3;
  j += 4;
  return j;
}

//! \brief inserts quadrilaterals.
template <typename HDS>
void Polyhedron_geo_builder<HDS>::
insert_quads(Polyhedron_geo_incremental_builder<HDS>& B)
{
  auto num_facets = m_mesh_set->get_num_primitives();
  Uint j = 0;
  for (Uint i = 0; i < num_facets; ++i) j = insert_quad(B, j);
}

//! \brief inserts a polygon.
template <typename HDS>
Uint Polyhedron_geo_builder<HDS>::
insert_polygon(Polyhedron_geo_incremental_builder<HDS>& B, Uint j)
{
  std::vector<size_t> indices;
  auto k = j;
  for (; m_mesh_set->get_coord_index(j) != (Uint) -1; ++j)
    indices.push_back(m_mesh_set->get_coord_index(j));
  ++j;
  if (!B.test_facet(indices.begin(), indices.end()))
    throw(std::runtime_error("Error: inconsistent mesh!"));
  auto he = B.add_facet(indices.begin(), indices.end());
  auto start_he = he;
  do {
    he->m_index = k++;
    he = he->next();
  } while (he != start_he);
  return j;
}

//! \brief inserts polygons.
template <typename HDS>
void Polyhedron_geo_builder<HDS>::
insert_polygons(Polyhedron_geo_incremental_builder<HDS>& B)
{
  auto num_facets = m_mesh_set->get_num_primitives();
  Uint j = 0;
  for (Uint i = 0; i < num_facets; ++i) j = insert_polygon(B, j);
}

SGAL_END_NAMESPACE

#endif
