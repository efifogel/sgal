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

#include "SGAL/basic.hpp"
#include "SGAL/Stl_binary_formatter.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs from file name.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename) :
  Formatter(filename)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
}

//! \brief Construct an output formatter.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename,
                                           std::ostream& os) :
  Formatter(filename, os)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
}

//! \brief Construct an input formatter.
Stl_binary_formatter::Stl_binary_formatter(const std::string& filename,
                                           std::istream& is) :
  Formatter(filename, is)
{ }

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

//! \brief obtains the number of triangles of a mesh.
size_t Stl_binary_formatter::number_of_triangles(Shared_mesh_set mesh)
{
  auto type = mesh->get_primitive_type();
  if ((type == Geo_set::PT_TRIANGLES) || (type == Geo_set::PT_QUADS)) {
    auto num = mesh->get_num_primitives();
    return (type == Geo_set::PT_TRIANGLES) ? num : 2 * num;
  }

  size_t num_triangles(0);
  size_t num_vertices(0);
  const auto& indices = mesh->get_coord_indices();
  std::for_each(indices.begin(), indices.end(),
                [&](Uint index){
                  if (index == static_cast<Uint>(-1)) {
                    num_triangles += num_vertices - 3;
                    num_vertices = 0;
                  }
                  else ++num_vertices;
                });
  return num_triangles;
}

//! \brief obtains the total number of triangles.
size_t Stl_binary_formatter::number_of_triangles()
{
  size_t num_triangles(0);
  std::for_each(m_shapes.begin(), m_shapes.end(),
                [&](const World_shape& ws)
                {
                  auto geometry = ws.first->get_geometry();
                  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
                  if (mesh) num_triangles += number_of_triangles(mesh);
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
  if (Vector3f::collinear(p1, p2, p3)) {
    std::cerr << "Cannot write a triangular facet using collinear points!"
              << std::endl;
    return;
  }
  Vector3f n;
  n.normal(p1, p2, p3);
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
  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  auto coords = mesh->get_coord_array();
  if (!coords || (coords->size() == 0)) return;

  if (mesh->is_dirty_flat_coord_indices()) mesh->clean_flat_coord_indices();
  auto type = mesh->get_primitive_type();
  if ((Geo_set::PT_TRIANGLES != type) && (Geo_set::PT_QUADS != type)) {
    if (mesh->is_dirty_coord_indices()) mesh->clean_coord_indices();
  }
}

//! \brief writes a shape.
void Stl_binary_formatter::write(Shared_shape shape, Shared_matrix4f matrix)
{
  auto geometry = shape->get_geometry();
  auto mesh = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  auto coords = mesh->get_coord_array();
  if (!coords || (coords->size() == 0)) return;

  std::vector<Vector3f> world_coords(coords->size());
  std::vector<Vector3f>::iterator it;
  Uint i(0);
  for (it = world_coords.begin(); it != world_coords.end(); ++it)
    it->xform_pt(mesh->get_coord_3d(i++), *matrix);

  // Export the facets.
  if (mesh->is_dirty_flat_coord_indices()) mesh->clean_flat_coord_indices();
  auto type = mesh->get_primitive_type();
  if ((Geo_set::PT_TRIANGLES == type) || (Geo_set::PT_QUADS == type)) {
    const auto& indices = mesh->get_flat_coord_indices();
    if (indices.empty()) return;

    Uint j(0);
    for (Uint i = 0; i < mesh->get_num_primitives(); ++i) {
      const Vector3f& v1 = world_coords[indices[j++]];
      const Vector3f& v2 = world_coords[indices[j++]];
      const Vector3f& v3 = world_coords[indices[j++]];
      //! \todo fill spacer with color if required.
      Ushort spacer;
      if (Geo_set::PT_TRIANGLES == type) {
        if (mesh->is_ccw()) write_facet(v1, v2, v3, spacer);
        else write_facet(v3, v2, v1, spacer);
      }
      else {
        const Vector3f& v4 = world_coords[indices[j++]];
        SGAL_assertion(PT_QUADS == get_primitive_type());
        if (mesh->is_ccw()) write_facet(v1, v2, v3, v4, spacer);
        else write_facet(v4, v3, v2, v1, spacer);
      }
    }
  }
  else {
    if (mesh->is_dirty_coord_indices()) mesh->clean_coord_indices();
    const auto& indices = mesh->get_coord_indices();
    if (indices.empty()) return;
    //! \todo triangulate and export.
    SGAL_error_msg("Not impelmented yet!");
  }
}

//! \brief writes a scene-graph container.
void Stl_binary_formatter::write(Shared_container container)
{
  auto transform = boost::dynamic_pointer_cast<Transform>(container);
  if (transform) {
    // Push the transform matrix
    const Matrix4f& curr_mat = transform->get_matrix();
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

SGAL_END_NAMESPACE
