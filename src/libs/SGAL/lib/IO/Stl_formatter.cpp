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

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Stl_formatter::Stl_formatter() {}

//! \brief constructs an output formatter.
Stl_formatter::Stl_formatter(std::ostream& os) : Text_formatter(os) {}

//! \brief constructs an input formatter.
Stl_formatter::Stl_formatter(std::istream& is) : Text_formatter(is) {}

//! \brief destructor
Stl_formatter::~Stl_formatter() {}

//! \brief writes the headers of the scene graph.
void Stl_formatter::begin()
{
  SGAL_assertion(m_out != NULL);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  out() << "solid xxx" << std::endl;
  new_line();
}

//! \brief writes the routing statements.
void Stl_formatter::end()
{
  out() << "endsolid xxx" << std::endl;
  new_line();
}

//! \brief writes a scene-graph node.
void Stl_formatter::write(Container* container) { container->write(this); }

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
