// Copyright (c) 2014 Israel.
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

#include <vector>
#include <iterator>

#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/utility.h>

#include "SGAL/basic.hpp"
#include "SGAL/Stl_binary_formatter.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs from file name.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename) :
  Formatter(filename)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
  m_export_non_visible = false;
}

//! \brief constructs an output formatter.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename,
                                           std::ostream& os) :
  Formatter(filename, os)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
  m_export_non_visible = false;
}

//! \brief Construct an input formatter.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename,
                                           std::istream& is) :
  Formatter(filename, is)
{ m_export_non_visible = false; }

//! \brief writes the begin statement.
void Stl_binary_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  const char header[80] = "SGAL";
  write(header, 80);
}

//! \brief writes the end statement.
void Stl_binary_formatter::end()
{
  // Clean the shapes
  std::for_each(m_shapes.begin(), m_shapes.end(),
                [&](const World_shape& ws)
                { clean(ws.first); });

  // Obtain the number of triangles
  auto num_tris = number_of_triangles();

  // Write the shapes
  write(static_cast<Int32>(num_tris));
  std::for_each(m_shapes.begin(), m_shapes.end(),
                [&](const World_shape& ws)
                { write(ws.first, ws.second); });
  m_shapes.clear();
}

//! \brief obtains the total number of triangles.
size_t Stl_binary_formatter::number_of_triangles()
{
  size_t num_triangles(0);
  std::for_each(m_shapes.begin(), m_shapes.end(),
                [&](const World_shape& ws)
                {
                  auto geometry = ws.first->get_geometry();
                  if (!geometry) return;
                  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
                  if (!mesh) return;
                  const auto& indices = mesh->get_facet_coord_indices();
                  Count_triangles_visitor visitor;
                  num_triangles += boost::apply_visitor(visitor, indices);
                });
  return num_triangles;
}

//! \brief writes a string.
void Stl_binary_formatter::write(const char* str, size_t size)
{ this->out().write(str, 80); }

//! \brief writes a 32-bit integer.
void Stl_binary_formatter::write(Int32 i)
{ this->out().write(reinterpret_cast<const char*>(&i), sizeof(Int32)); }

//! \brief writes a Float.
void Stl_binary_formatter::write(Float f)
{ this->out().write(reinterpret_cast<const char*>(&f), sizeof(Float)); }

//! \brief writes a vector3f.
void Stl_binary_formatter::write(const Vector3f& v)
{
  write(v[0]);
  write(v[1]);
  write(v[2]);
}

//! \brief writes a triangular facet.
void Stl_binary_formatter::write_facet(const Vector3f& p1, const Vector3f& p2,
                                       const Vector3f& p3, Ushort spacer)
{
  Vector3f n;
  auto is_collinear = Vector3f::collinear(p1, p2, p3);
  if (! is_collinear) n.normal(p1, p2, p3);
  SGAL_warning_msg(! is_collinear, "The facet is degenerate!");
  write(n);
  write(p1);
  write(p2);
  write(p3);

  out().write((char*)&spacer, sizeof(Ushort));
}

//! \brief writes a quadrilateral facet.
void Stl_binary_formatter::write_facet(const Vector3f& p1, const Vector3f& p2,
                                       const Vector3f& p3, const Vector3f& p4,
                                       Ushort spacer)
{
  Vector3f l11, l12;
  l11.sub(p3, p2);
  l12.sub(p1, p2);

  Vector3f l21, l22;
  l21.sub(p2, p1);
  l22.sub(p4, p1);

  if (abs(l11.dot(l12)) < abs(l21.dot(l22))) {
    write_facet(p1, p2, p3, spacer);
    write_facet(p1, p3, p4, spacer);
  }
  else {
    write_facet(p1, p2, p4, spacer);
    write_facet(p2, p3, p4, spacer);
  }
}

//! \bried cleans a shape.
void Stl_binary_formatter::clean(Shared_shape shape)
{
  auto geometry = shape->get_geometry();
  if (!geometry) return;
  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  if (!mesh) return;
  auto coords = mesh->get_coord_array();
  if (!coords || (coords->size() == 0)) return;

  if (mesh->is_dirty_facet_coord_indices()) mesh->clean_facet_coord_indices();
}

//! \brief writes a shape.
void Stl_binary_formatter::write(Shared_shape shape, Shared_matrix4f matrix)
{
  auto geometry = shape->get_geometry();
  if (!geometry) return;
  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  if (!mesh) return;
  auto coords = mesh->get_coord_array();
  if (!coords || (coords->size() == 0)) return;

  std::vector<Vector3f> world_coords(coords->size());
  Uint i(0);
  for (auto it = world_coords.begin(); it != world_coords.end(); ++it)
    it->xform_pt(mesh->get_coord_3d(i++), *matrix);

  // Export the facets.
  if (mesh->is_dirty_facet_coord_indices()) mesh->clean_facet_coord_indices();
  const auto& indices = mesh->get_facet_coord_indices();
  Export_facet_visitor visitor(mesh, world_coords, *this);
  boost::apply_visitor(visitor, indices);

  world_coords.clear();
}

//! \brief writes a scene-graph container.
void Stl_binary_formatter::write(Shared_container container)
{
  auto transform = boost::dynamic_pointer_cast<Transform>(container);
  if (transform) {
    // Push the transform matrix
    const auto& curr_mat = transform->get_matrix();
    auto last_mat = m_matrices.top();
    Shared_matrix4f next_mat(new Matrix4f);
    next_mat->mult(*last_mat, curr_mat);
    m_matrices.push(next_mat);

    // Process the children.
    transform->write(this);

    // Pop the transform matrix
    m_matrices.pop();
    return;
  }
  auto shape = boost::dynamic_pointer_cast<Shape>(container);
  if (shape) {
    if (shape->is_visible())
        m_shapes.push_back(std::make_pair(shape, m_matrices.top()));
    return;
  }
  container->write(this);
}

//! \brief obtains the number of triangles.
size_t Stl_binary_formatter::Count_triangles_visitor::
operator()(const Triangle_indices& indices) { return indices.size(); }

//! \brief obtains the number of triangles.
size_t Stl_binary_formatter::Count_triangles_visitor::
operator()(const Quad_indices& indices) { return 2 * indices.size(); }

//! \brief obtains the number of triangles.
size_t Stl_binary_formatter::Count_triangles_visitor::
operator()(const Polygon_indices& indices)
{
  size_t count(0);
  for (const auto& polygon: indices) count += polygon.size() - 2;
  return count;
}

//! \brief obtains the number of triangles.
size_t Stl_binary_formatter::Count_triangles_visitor::
operator()(const Flat_indices& indices)
{
  size_t num_triangles(0);
  size_t num_vertices(0);
  std::for_each(indices.begin(), indices.end(),
                [&](Uint index){
                  if (index == -1) {
                    num_triangles += num_vertices - 3;
                    num_vertices = 0;
                  }
                  else ++num_vertices;
                });
  return num_triangles;
}

//! \brief export triangles.
void Stl_binary_formatter::Export_facet_visitor::
operator()(const Triangle_indices& indices)
{
  SGAL_assertion (Geo_set::PT_TRIANGLES == m_mesh->get_primitive_type());
  if (indices.empty()) return;
  for (size_t i = 0; i < indices.size(); ++i) {
    const Vector3f& v1 = m_world_coords[indices[i][0]];
    const Vector3f& v2 = m_world_coords[indices[i][1]];
    const Vector3f& v3 = m_world_coords[indices[i][2]];
    //! \todo fill spacer with color if required.
    Ushort spacer;
    if (m_mesh->is_ccw()) m_formater.write_facet(v1, v2, v3, spacer);
    else m_formater.write_facet(v3, v2, v1, spacer);
  }
}

//! \brief export quads.
void Stl_binary_formatter::Export_facet_visitor::
operator()(const Quad_indices& indices)
{
  SGAL_assertion (Geo_set::PT_QUADS == m_mesh->get_primitive_type());
  if (indices.empty()) return;
  for (size_t i = 0; i < indices.size(); ++i) {
    const Vector3f& v1 = m_world_coords[indices[i][0]];
    const Vector3f& v2 = m_world_coords[indices[i][1]];
    const Vector3f& v3 = m_world_coords[indices[i][2]];
    const Vector3f& v4 = m_world_coords[indices[i][3]];
    Ushort spacer;
    if (m_mesh->is_ccw()) m_formater.write_facet(v1, v2, v3, v4, spacer);
    else m_formater.write_facet(v4, v3, v2, v1, spacer);
  }
}

//! \brief export polygons.
void Stl_binary_formatter::Export_facet_visitor::
operator()(const Polygon_indices& indices)
{
  SGAL_assertion(Geo_set::PT_POLYGONS == m_mesh->get_primitive_type());
  if (indices.empty()) return;

  typedef CGAL::Triple<size_t, size_t, size_t> Triangle;
  for (const auto& polygon: indices) {
    std::vector<Inexact_point_3> polyline;
    polyline.reserve(polygon.size());
    for (auto& index: polygon) {
      const Vector3f& v = m_world_coords[index];
      auto pit = polyline.begin();
      polyline.emplace(pit++, v[0], v[1], v[2]);
    }
    std::vector<Triangle> patch;
    patch.reserve(polyline.size() -2);
    CGAL::Polygon_mesh_processing::
      triangulate_hole_polyline(polyline, std::back_inserter(patch));
    for (const auto& triangle: patch) {
      Ushort spacer;
      const auto& v1 = m_world_coords[polygon[triangle.first]];
      const auto& v2 = m_world_coords[polygon[triangle.second]];
      const auto& v3 = m_world_coords[polygon[triangle.third]];
      if (m_mesh->is_ccw()) m_formater.write_facet(v1, v2, v3, spacer);
      else m_formater.write_facet(v3, v2, v1, spacer);
    }
  }
}

SGAL_END_NAMESPACE
