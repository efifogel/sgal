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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_POLYHEDRON_GEO_BUILDER_HPP
#define SGAL_POLYHEDRON_GEO_BUILDER_HPP

#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

/*! Add a vertex to the builder.
 * Distinguish between the different cases of source and target types.
 */
template <typename TargetPoint_, typename SourcePoint_>
struct Vertex_adder {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const SourcePoint_& point, Size_ id) {}
};

// Target is Inexact.
template <typename SourcePoint_>
struct Vertex_adder<Inexact_point_3, SourcePoint_> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const SourcePoint_& point, Size_ id)
  {
    auto x = to_float(point[0]);
    auto y = to_float(point[1]);
    auto z = to_float(point[2]);
    auto v = B.add_vertex(Inexact_point_3(x, y, z));
    v->set_id(id);
  }
};

// Target is Epic.
template <>
struct Vertex_adder<Epic_point_3, Vector3f> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const Vector3f& point, Size_ id)
  {
    auto v = B.add_vertex(Epic_point_3(point[0], point[1], point[2]));
    v->set_id(id);
  }
};

template <>
struct Vertex_adder<Epic_point_3, Epic_point_3> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const Epic_point_3& point, Size_ id)
  {
    auto v = B.add_vertex(point);
    v->set_id(id);
  }
};

template <>
struct Vertex_adder<Epic_point_3, Epec_point_3>{
  template <typename Builder_, typename Size_>
  void operator()(Builder_& /* B */, const Epec_point_3& /* p */, Size_ /* id */)
  { SGAL_error_msg("Not implemented Yet!"); }
};

// Target is Epec.
template <>
struct Vertex_adder<Epec_point_3, Vector3f> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const Vector3f& point, Size_ id)
  {
    auto v = B.add_vertex(Epec_point_3(point[0], point[1], point[2]));
    v->set_id(id);
  }
};

template <>
struct Vertex_adder<Epec_point_3, Epic_point_3> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& /* B */, const Epic_point_3& /* p */, Size_ /* id */)
  { SGAL_error_msg("Not implemented Yet!"); }
};

template <>
struct Vertex_adder<Epec_point_3, Epec_point_3> {
  template <typename Builder_, typename Size_>
  void operator()(Builder_& B, const Epec_point_3& point, Size_ id)
  {
    auto v = B.add_vertex(point);
    v->set_id(id);
  }
};

template <typename HDS>
class Polyhedron_geo_builder : public CGAL::Modifier_base<HDS> {
private:
  /*! Indicates wheather the mesh is a consistent. */
  Boolean m_consistent;

protected:
  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;

  /*! The primitive type of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Geo_set::Primitive_type m_primitive_type;

  /*! The number of facets of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Size m_num_primitives;

  /*! The coordinates of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Shared_coord_array m_coord_array;

  /*! The coordinate indices of the mesh for which a polyhedron data structure is
   * constructed.
   */
  const Facet_indices m_coord_indices;

  /*! Insert the vertices.
   * \param[in] B the halfedge data structure.
   */
  void insert_vertices(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    typedef typename HDS::Vertex Vertex;
    typedef typename Vertex::Point Point;

    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coords) {
      Vertex_adder<Point, Vector3f> adder;
      Size i(0);
      for (auto it = coords->begin(); it != coords->end(); ++it)
        adder(B, *it, i++);
      return;
    }

    auto epic_coords =
      boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coord_array);
    if (epic_coords) {
      Vertex_adder<Point, Epic_point_3> adder;
      Size i(0);
      for (auto it = epic_coords->begin(); it != epic_coords->end(); ++it)
        adder(B, *it, i++);
      return;
    }

    auto epec_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
    if (epec_coords) {
      Vertex_adder<Point, Epec_point_3> adder;
      Size i(0);
      for (auto it = epec_coords->begin(); it != epec_coords->end(); ++it)
        adder(B, *it, i++);
      return;
    }

    SGAL_error();
  }

  /*! Insert the faces.
   * \param[in] B the halfedge data structure.
   */
  void insert_faces(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    // Add the faces:
    switch (m_primitive_type) {
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
   * \param[in] B the halfedge data structure.
   * \param[in] begin the begin iterator of the indices of the triangle vertices.
   * \param[in] end the past-the-end iterator of the indices of the triangle
   *            vertices.
   * \param[in] j the index of the index of the first point of the triangle.
   *            In other words, the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_triangle(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                         InputIterator begin, InputIterator end, size_t j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    SGAL_assertion(!B.error());
    he->set_id(j);
    he->next()->set_id(j+1);
    he->next()->next()->set_id(j+2);
    return j+3;
  }

  /*! Insert a quadrilateral.
   * \param[in] B the halfedge data structure.
   * \param[in] begin the begin iterator of the indices of the triangle vertices.
   * \param[in] end the past-the-end iterator of the indices of the triangle
   *            vertices.
   * \param[in] j the index of the index of the first point of the quad.
   *            In other words,  the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_quad(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                     InputIterator begin, InputIterator end, size_t j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    SGAL_assertion(!B.error());
    he->set_id(j);
    he->next()->set_id(j+1);
    he->next()->next()->set_id(j+2);
    he->next()->next()->next()->set_id(j+3);
    return j+4;
  }

  /*! Insert a polygon.
   * \param[in] B the halfedge data structure.
   * \param[in] begin the begin iterator of the indices of the triangle vertices.
   * \param[in] end the past-the-end iterator of the indices of the triangle
   *            vertices.
   * \param[in] j the index of the index of the first point of the polygon.
   *            In other words, the first vertex is coords[indices[j]].
   */
  template <typename InputIterator>
  size_t insert_polygon(CGAL::Polyhedron_incremental_builder_3<HDS>& B,
                        InputIterator begin, InputIterator end, size_t j)
  {
    if (!B.test_facet(begin, end))
      throw(std::runtime_error("Error: inconsistent mesh!"));
    auto he = B.add_facet(begin, end);
    SGAL_assertion(!B.error());
    auto start_he = he;
    do {
      he->set_id(j++);
      he = he->next();
    } while (he != start_he);
    return j;
  }

  /*! Insert triangles.
   * \param[in] B the halfedge data structure.
   */
  void insert_triangles(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& indices = boost::get<Triangle_indices>(m_coord_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_triangle(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert quadrilaterals.
   * \param[in] B the halfedge data structure.
   */
  void insert_quads(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& indices = boost::get<Quad_indices>(m_coord_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_quad(B, indices[i].begin(), indices[i].end(), j);
  }

  /*! Insert polygons.
   * \param[in] B the halfedge data structure.
   */
  void insert_polygons(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    const auto& indices = boost::get<Polygon_indices>(m_coord_indices);
    size_t j(0);
    for (size_t i = 0; i < indices.size(); ++i)
      j = insert_polygon(B, indices[i].begin(), indices[i].end(), j);
  }

public:
  /*! Construct.
   * \param[in] mesh_set the Mesh_set, which provides the
   *            a. coordinate array, and
   *            b. coordinate indices
   */
  Polyhedron_geo_builder(Geo_set::Primitive_type primitive_type,
                         Size num_primitives,
                         Shared_coord_array coord_array,
                         const Facet_indices& coord_indices) :
    m_primitive_type(primitive_type),
    m_num_primitives(num_primitives),
    m_coord_array(coord_array),
    m_coord_indices(coord_indices)
  {}

  /*! Build the polyhedral surface.
   * \param[out] hds the halfedge data structure, which stores the incidence
   *            relations between the cells of the polyhedral surface.
   */
  void operator()(HDS& hds)
  {
    m_consistent = true;

    // Postcondition: `hds' is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(m_coord_array->size(), m_num_primitives);
    try {
      insert_vertices(B);
      insert_faces(B);
      B.end_surface();
    }
    catch (std::exception& e) {
      m_consistent = false;
      std::cerr << e.what() << std::endl;
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
