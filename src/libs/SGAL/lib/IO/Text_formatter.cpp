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

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Text_formatter.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Text_formatter::Text_formatter() : m_indent(0), m_indented(false) {}

//! \brief constructs an output formatter from an output stream.
Text_formatter::Text_formatter(std::ostream& os) :
  Formatter(os),
  m_indent(0),
  m_indented(false)
{}

//! \brief constructs an input formatter from an input stream.
Text_formatter::Text_formatter(std::istream& is) :
  Formatter(is),
  m_indent(0),
  m_indented(false)
{}

//! \brief destructor.
Text_formatter::~Text_formatter() {}

//! \brief begins a new line.
void Text_formatter::new_line()
{
  if (m_indented) {
    out() << std::endl;
    m_indented = false;
  }
}

//! \brief writes an indentation.
void Text_formatter::indent()
{
  if (!m_indented) for (Uint i = 0; i < m_indent; ++i) out() << "  ";
  m_indented = true;
}

SGAL_END_NAMESPACE
