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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 */

#include <iterator>

#include "SGAL/Stl_formatter.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Stl_formatter::Stl_formatter()
{ m_matrices.emplace(Matrix4f()); }

//! \brief constructs an output formatter.
Stl_formatter::Stl_formatter(std::ostream& os) : Text_formatter(os)
{ m_matrices.emplace(Matrix4f()); }

//! \brief constructs an input formatter.
Stl_formatter::Stl_formatter(std::istream& is) : Text_formatter(is) {}

//! \brief destructor
Stl_formatter::~Stl_formatter() { m_matrices.pop(); }

//! \brief writes the headers of the scene graph.
void Stl_formatter::begin()
{
  SGAL_assertion(m_out != NULL);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  out() << "solid sgal" << std::endl;
  new_line();
}

//! \brief writes the routing statements.
void Stl_formatter::end()
{
  out() << "endsolid sgal" << std::endl;
  new_line();
}

//! \brief computes the normal of a plane given by three points.
void Stl_formatter::compute_normal(const Vector3f& p1, const Vector3f& p2,
                                   const Vector3f& p3, Vector3f& normal)
{
  Vector3f l1, l2;
  l1.sub(p2, p1);
  l2.sub(p3, p2);
  normal.cross(l1, l2);
  normal.normalize();
}

//! \brief exports a vertex.
void Stl_formatter::vertex(const Vector3f& p)
{
  indent();
  out() << "vertex " << p[0] << " " << p[1] << " " << p[2];
  new_line();
}

//! \brief exports a facet header.
void Stl_formatter::facet_begin(const Vector3f& n)
{
  indent();
  out() << "facet normal " << n[0] << " " << n[1] << " " << n[2];
  new_line();
  push_indent();
  indent();
  out() << "outer loop";
  new_line();
  push_indent();
}

//! \brief exports a facet trailer.
void Stl_formatter::facet_end()
{
  pop_indent();
  indent();
  out() << "endloop";
  new_line();
  pop_indent();
  indent();
  out() << "endfacet";
  new_line();
}

//! \brief exports a facet.
void Stl_formatter::facet(const Vector3f& p1, const Vector3f& p2,
                          const Vector3f& p3, const Vector3f& normal)
{
  facet_begin(normal);
  vertex(p1);
  vertex(p2);
  vertex(p3);
  facet_end();;
}

//! \brief exports a facet.
void Stl_formatter::facet(const Vector3f& local_p1, const Vector3f& local_p2,
                          const Vector3f& local_p3, const Matrix4f& matrix)
{
  Vector3f p1, p2, p3;
  p1.xform_pt(local_p1, matrix);
  p2.xform_pt(local_p2, matrix);
  p3.xform_pt(local_p3, matrix);
  Vector3f n;
  compute_normal(p1, p2, p3, n);
  facet(p1, p2, p3, n);
}

//! \brief exports a quadrilateral facet.
void Stl_formatter::facet(const Vector3f& local_p1, const Vector3f& local_p2,
                          const Vector3f& local_p3, const Vector3f& local_p4,
                          const Matrix4f& matrix)
{
  Vector3f p1, p2, p3, p4;
  p1.xform_pt(local_p1, matrix);
  p2.xform_pt(local_p2, matrix);
  p3.xform_pt(local_p3, matrix);
  p4.xform_pt(local_p4, matrix);

  //! \todo split the quadrilateral based on its shape.
  Vector3f n1;
  compute_normal(p1, p2, p3, n1);
  facet(p1, p2, p3, n1);

  Vector3f n2;
  compute_normal(p1, p3, p4, n2);
  facet(p1, p3, p4, n2);
}

//! \brief writes a scene-graph container.
void Stl_formatter::write(Container* container)
{
  Transform* transform = dynamic_cast<Transform*>(container);
  if (transform) {
    // Push the transform matrix
    const Matrix4f& new_matrix = transform->get_matrix();
    const Matrix4f& top_matrix = m_matrices.top();
    Matrix4f m;
    m.mult(top_matrix, new_matrix);
    m_matrices.push(m);

    // Process the children.
    transform->write(this);

    // Pop the transform matrix
    m_matrices.pop();
    return;
  }
  Mesh_set* mesh = dynamic_cast<Mesh_set*>(container);
  if (mesh) {
    // Apply the active (top) transform matrix and export the facets.
    const Matrix4f& matrix = m_matrices.top();
    const Mesh_set::Shared_coord_array coords = mesh->get_coord_array();
    if ((Geo_set::PT_TRIANGLES == mesh->get_primitive_type()) ||
        (Geo_set::PT_QUADS == mesh->get_primitive_type()))
    {
      Vector3f vert;
      const Array<Uint>& indices = mesh->get_flat_coord_indices();
      Uint j = 0;
      for (Uint i = 0; i < mesh->get_num_primitives(); ++i) {
        const Vector3f& v1 = (*coords)[indices[j++]];
        const Vector3f& v2 = (*coords)[indices[j++]];
        const Vector3f& v3 = (*coords)[indices[j++]];
        if (Geo_set::PT_TRIANGLES == mesh->get_primitive_type()) {
          if (mesh->is_ccw()) facet(v1, v2, v3, matrix);
          else facet(v3, v2, v1, matrix);
        }
        else {
          const Vector3f& v4 = (*coords)[indices[j++]];
          SGAL_assertion(Geo_set::PT_QUADS == mesh->get_primitive_type());
          if (mesh->is_ccw()) facet(v1, v2, v3, v4, matrix);
          else facet(v4, v3, v2, v1, matrix);
        }
      }
    }
    else {
      //! \todo triangulate and export.
      SGAL_error_msg("Not impelmented yet!");
    }
  }
  container->write(this);
}

//! \brief writes the container header.
void Stl_formatter::container_begin(const std::string& tag) {}

//! \brief writes the container tailer.
void Stl_formatter::container_end() {}

//! \brief writes the header of a container multi-field.
void Stl_formatter::multi_container_begin(const std::string& name) {}

//! \brief writes the tailer of a container multi-field.
void Stl_formatter::multi_container_end() {}

//! \brief writes the header of a container single-field.
void Stl_formatter::single_container_begin(const std::string& name) {}

//! \brief writes the tailer of a container single-field.
void Stl_formatter::single_container_end() {}

/*! Write a single Boolean field */
void Stl_formatter::single_boolean(const std::string& name,
                                   Boolean value, Boolean default_value)
{}

//! \brief writes a multi-Boolean field.
void Stl_formatter::multi_boolean(const std::string& name,
                                  const Array<Boolean>& value,
                                  const Array<Boolean>& default_value)
{}

SGAL_END_NAMESPACE
