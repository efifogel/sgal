// Copyright (c) 2018 Israel.
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
#include <utility>

#include "SGAL/basic.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Json_formatter.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Json_formatter::Json_formatter(const std::string& filename) :
  Text_formatter(filename)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
}

//! \brief constructs an output formatter.
Json_formatter::Json_formatter(const std::string& filename, std::ostream& os) :
  Text_formatter(filename, os)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
}

//! \brief constructs an input formatter.
Json_formatter::Json_formatter(const std::string& filename, std::istream& is) :
  Text_formatter(filename, is)
{}

//! \brief destruct.
Json_formatter::~Json_formatter() {}

//! \brief writes the begin statement.
void Json_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  object_begin();
}

//! \brief writes the end statement.
void Json_formatter::end()
{
  std::function<void(const Json_formatter&)> start_op = &Json_formatter::object_begin;
  object("metadata", start_op,
         [&]() {
           attribute("version", "1.0");
           object_separator();
           attribute("type", "Object");
           object_separator();
           attribute ("generator", "SGAL");
         });
  object_separator();
  geometries();
  object_separator();
  materials();
  object_separator();
  // object("object", Object_exporter());
  new_line();
  object_end();
  new_line();
}

//! \brief writes a vertex.
void Json_formatter::vertex(const Vector3f& p)
{
  indent();
  out() << "vertex " << p[0] << " " << p[1] << " " << p[2];
  new_line();
}

//! \brief writes a facet header.
void Json_formatter::facet_begin(const Vector3f& n)
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
void Json_formatter::facet_end()
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
void Json_formatter::facet(const Vector3f& p1, const Vector3f& p2,
                           const Vector3f& p3, const Vector3f& normal)
{
  facet_begin(normal);
  vertex(p1);
  vertex(p2);
  vertex(p3);
  facet_end();
}

//! \brief writes a scene-graph container.
void Json_formatter::write(Shared_container container)
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
    // Observe that we push the shape even if it is not visible.
    m_shapes.push_back(std::make_pair(shape, m_matrices.top()));
    return;
  }
  container->write(this);
}

void Json_formatter::out_string(const std::string& name) { out() << name; }

void Json_formatter::name_value_separator() { out() << ": "; }

void Json_formatter::object_separator()
{
  out() << ",";
  new_line();
}

void Json_formatter::out_value(const std::string& value)
{ out_string(value); }

template <typename Value>
void Json_formatter::attribute(const std::string& name, const Value& value)
{
  indent();
  out_string(name);
  name_value_separator();
  out_value(value);
}

void Json_formatter::object_begin()
{
  indent();
  out() << "{";
  new_line();
  push_indent();
}

void Json_formatter::object_end()
{
  pop_indent();
  indent();
  out() << "}";
}

void Json_formatter::array_begin()
{
  indent();
  out() << "[";
  new_line();
  push_indent();
}

void Json_formatter::array_end()
{
  pop_indent();
  indent();
  out() << "]";
}

void Json_formatter::geometries()
{
  indent();
  out_string("geometries");
  name_value_separator();
  array_begin();

  array_end();
}

void Json_formatter::materials()
{
  indent();
  out_string("materials");
  name_value_separator();
  array_begin();

  array_end();
}

SGAL_END_NAMESPACE
