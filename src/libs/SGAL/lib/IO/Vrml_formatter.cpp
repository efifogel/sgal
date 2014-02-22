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

//! \brief constructor.
Vrml_formatter::Vrml_formatter() {}

//! \brief constructs an output formatter.
Vrml_formatter::Vrml_formatter(std::ostream& os) : Text_formatter(os) {}

//! \brief constructs an input formatter.
Vrml_formatter::Vrml_formatter(std::istream& is) : Text_formatter(is) {}

//! \brief destructor
Vrml_formatter::~Vrml_formatter() {}

//! \brief exports the headers of the scene graph.
void Vrml_formatter::begin()
{
  SGAL_assertion(m_out != NULL);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  out() << "#VRML V2.0 utf8" << std::endl
        << "# Generated by SGAL Version " << SGAL_VERSION << std::endl;
  new_line();
}

//! \brief exports the routing statements.
void Vrml_formatter::end()
{
  // set_mode(*m_out, m_old_out_mode);
}

//! \brief exports the container header.
void Vrml_formatter::container_begin(const std::string& tag)
{
  indent();
  out() << tag << " {";
  push_indent();
}

//! \brief exports the container tailer.
void Vrml_formatter::container_end()
{
  pop_indent();
  indent();
  out() << "}";
  new_line();
}

//! \brief exports the header of a container multi-field.
void Vrml_formatter::multi_container_begin(const std::string& name)
{
  new_line();
  indent();
  out() << name << " [";
  new_line();
  push_indent();
}

//! \brief exports the tailer of a container multi-field.
void Vrml_formatter::multi_container_end()
{
  pop_indent();
  indent();
  out() << "]";
  new_line();
}

//! \brief exports the header of a container single-field.
void Vrml_formatter::single_container_begin(const std::string& name)
{
  new_line();
  indent();
  out() << name << " ";
}

//! \brief exports the tailer of a container single-field.
void Vrml_formatter::single_container_end() {}

//! \brief exports a single string field.
inline void Vrml_formatter::single_string(const std::string& name,
                                          const std::string& value,
                                          const std::string& default_value)
{
  if (value == default_value) return;
  new_line();
  indent();
  out() << name << " \"" << value << "\"";
  new_line();
}

//! \brief exports a single Boolean field.
void Vrml_formatter::single_boolean(const std::string& name,
                                    Boolean value, Boolean default_value)
{
  if (value == default_value) return;
  new_line();
  indent();
  out() << name << " " << (value ? "TRUE" : "FALSE");
  new_line();
}

//! \brief exports a multi-Boolean field.
void Vrml_formatter::multi_boolean(const std::string& name,
                                   const std::vector<Boolean>& value,
                                   const std::vector<Boolean>& default_value)
{
  if ((value.size() == 0) || (value.size() != default_value.size())) return;
  if (! std::equal(value.begin(), value.end(), default_value.begin())) return;

  new_line();
  indent();
  out() << name << " [";
  new_line();
  push_indent();
  indent();
  std::copy(value.begin(), value.end(),
            std::ostream_iterator<Boolean>(out(), " "));
  new_line();
  pop_indent();
  indent();
  out() << "]";
  new_line();
}

SGAL_END_NAMESPACE
