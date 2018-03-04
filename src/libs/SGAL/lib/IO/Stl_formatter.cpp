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

#include "SGAL/basic.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Stl_formatter.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Stl_formatter::Stl_formatter(const std::string& filename) :
  Text_formatter(filename)
{
  m_matrices.emplace(Matrix4f());
  m_export_non_visible = false;
}

//! \brief constructs an output formatter.
Stl_formatter::Stl_formatter(const std::string& filename, std::ostream& os) :
  Text_formatter(filename, os)
{
  m_matrices.emplace(Matrix4f());
  m_export_non_visible = false;
}

//! \brief constructs an input formatter.
Stl_formatter::Stl_formatter(const std::string& filename, std::istream& is) :
  Text_formatter(filename, is)
{ m_export_non_visible = false; }

//! \brief destructor
Stl_formatter::~Stl_formatter() { m_matrices.pop(); }

//! \brief writes the begin statement.
void Stl_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  out() << "solid sgal" << std::endl;
  new_line();
}

//! \brief writes the end statement.
void Stl_formatter::end()
{
  out() << "endsolid sgal" << std::endl;
  new_line();
}

//! \brief writes a vertex.
void Stl_formatter::vertex(const Vector3f& p)
{
  indent();
  out() << "vertex " << p[0] << " " << p[1] << " " << p[2];
  new_line();
}

//! \brief writes a facet header.
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

//! \brief writes a facet trailer.
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

//! \brief writes a triangular facet.
void Stl_formatter::facet(const Vector3f& p1, const Vector3f& p2,
                          const Vector3f& p3, const Vector3f& normal)
{
  facet_begin(normal);
  vertex(p1);
  vertex(p2);
  vertex(p3);
  facet_end();
}

//! \brief writes a scene-graph container.
void Stl_formatter::write(Shared_container container)
{
  auto transform = boost::dynamic_pointer_cast<Transform>(container);
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
