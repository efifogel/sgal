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

/*! \file
 */

#include <iterator>

#include "SGAL/Obj_formatter.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Obj_formatter::Obj_formatter() : m_index(1)
{ m_matrices.emplace(Matrix4f()); }

//! \brief constructs an output formatter.
Obj_formatter::Obj_formatter(std::ostream& os) :
  Text_formatter(os),
  m_index(1)
{ m_matrices.emplace(Matrix4f()); }

//! \brief constructs an input formatter.
Obj_formatter::Obj_formatter(std::istream& is) :
  Text_formatter(is),
  m_index(1)
{}

//! \brief destructor
Obj_formatter::~Obj_formatter() { m_matrices.pop(); }

//! \brief writes the headers of the scene graph.
void Obj_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  indent();
  out() << "mtllib 3GO_logo.mtl";
  new_line();
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);
}

//! \brief writes the routing statements.
void Obj_formatter::end() {}

//! \brief exports a vertex.
void Obj_formatter::vertex(const Vector3f& p)
{
  indent();
  out() << "v " << p[0] << " " << p[1] << " " << p[2];
  new_line();
}

//! \brief exports a triangular facet.
void Obj_formatter::triangle(Uint i0, Uint i1, Uint i2)
{
  indent();
  out() << "f " << i0+m_index << " " << i1+m_index << " " << i2+m_index;
  new_line();
}

//! \brief exports a quadrilateral facet.
void Obj_formatter::quad(Uint i0, Uint i1, Uint i2, Uint i3)
{
  out() << "f " << i0+m_index << " " << i1+m_index << " " << i2+m_index
        << " " << i3+m_index;
  new_line();
}

//! \brief writes a scene-graph container.
void Obj_formatter::write(Container* container)
{
  Transform* transform = dynamic_cast<Transform*>(container);
  if (transform) {
    // Push the transform matrix
    const Matrix4f& new_matrix = transform->get_matrix();
    const Matrix4f& top_matrix = m_matrices.top();
    m_matrices.emplace(Matrix4f(top_matrix, new_matrix));

    // Process the children.
    transform->write(this);

    // Pop the transform matrix
    m_matrices.pop();
    return;
  }
  container->write(this);
}

SGAL_END_NAMESPACE
