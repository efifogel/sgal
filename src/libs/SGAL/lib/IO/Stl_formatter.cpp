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
    if (Geo_set::PT_TRIANGLES == mesh->get_primitive_type()) {
      Vector3f vert;
      const Array<Uint>& indices = mesh->get_flat_coord_indices();
      Uint j = 0;
      for (Uint i = 0; i < mesh->get_num_primitives(); ++i) {
        indent();
        out() << "facet normal 0 0 1";
        new_line();
        push_indent();
        indent();
        out() << "outer loop";
        new_line();
        push_indent();

        Uint index = indices[j++];
        const Vector3f& vertex1 = (*coords)[index];
        vert.xform_pt(vertex1, matrix);
        indent();
        out() << "vertex " << vert[0] << " " << vert[1] << " " << vert[2];
        new_line();

        index = indices[j++];
        const Vector3f& vertex2 = (*coords)[index];
        vert.xform_pt(vertex2, matrix);
        indent();
        out() << "vertex " << vert[0] << " " << vert[1] << " " << vert[2];
        new_line();

        index = indices[j++];
        const Vector3f& vertex3 = (*coords)[index];
        vert.xform_pt(vertex3, matrix);
        indent();
        out() << "vertex " << vert[0] << " " << vert[1] << " " << vert[2];
        new_line();
        pop_indent();
        indent();
        out() << "endloop";
        new_line();
        pop_indent();
        indent();
        out() << "endfacet";
        new_line();
      }
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
