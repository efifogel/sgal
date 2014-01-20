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
// $Id: $
// $Revision: 12379 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 */

#include <iterator>

#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

/*! Begin a new line */
void Vrml_formatter::new_line()
{
  if (m_indented) {
    out() << std::endl;
    m_indented = false;
  }
}

/*! Writes an indentation */
void Vrml_formatter::indent()
{
  if (!m_indented) for (Uint i = 0; i < m_indent; ++i) out() << "  ";
  m_indented = true;
}

/*! Write the headers of the scene graph */
void Vrml_formatter::begin()
{
  SGAL_assertion(m_out != NULL);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  out() << "#VRML V2.0 utf8" << std::endl
        << "# Generated by SGAL Version " << SGAL_VERSION << std::endl;
  new_line();
}

/*! Write the routing statements */
void Vrml_formatter::end()
{
  // set_mode(*m_out, m_old_out_mode);
}

/*! Write a scene-graph node */
void Vrml_formatter::write(Container* container)
{
  container->write(this);
}

/*! Write the container header */
void Vrml_formatter::container_begin(const std::string& tag)
{
  indent();
  out() << tag << " {";
  push_indent();
}

/*! Write the container tailer */
void Vrml_formatter::container_end()
{
  pop_indent();
  indent();
  out() << "}";
  new_line();
}

/*! Write the header of a container multi-field */
void Vrml_formatter::multi_container_begin(const std::string& name)
{
  new_line();
  indent();
  out() << name << " [";
  new_line();
  push_indent();
}

/*! Write the tailer of a container multi-field */
void Vrml_formatter::multi_container_end()
{
  pop_indent();
  indent();
  out() << "]";
  new_line();
}

/*! Write the header of a container single-field */
void Vrml_formatter::single_container_begin(const std::string& name)
{
  new_line();
  indent();
  out() << name << " ";
}

/*! Write the tailer of a container single-field */
void Vrml_formatter::single_container_end() {}

/*! Write a multi Uint field */
void Vrml_formatter::multi_uint(const std::string& name,
                                const Array<Uint>& value)
{
  const Uint* vec = value.get_vector();
  new_line();
  indent();
  out() << name << " [";
  new_line();
  push_indent();
  indent();
  std::copy(vec, &vec[value.size()], std::ostream_iterator<Uint>(out(), " "));
  new_line();
  pop_indent();
  indent();
  out() << "]";
  new_line();
}

SGAL_END_NAMESPACE
