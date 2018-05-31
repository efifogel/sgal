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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_IMPORTER_HPP
#define SGAL_DXF_IMPORTER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Dxf_base_parser.hpp"

SGAL_BEGIN_NAMESPACE

template <typename T>
struct Dxf_importer {
};

#if 0
/*! Import a value.
 */
template <typename T>
struct Dxf_importer {

  Dxf_base_parser& m_parser;

  void operator()(T& variable)
  {
    m_parser.m_is >> variable;
    ++(m_parser.m_line);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "[" << std::to_string(m_line) << "]"
                    << "Importering dxf value: "
                    << variable << std::endl;);
  }
};

/*! Import an int_8 value.
 */
template <>
struct Dxf_importer<int_8> {

  Dxf_base_parser& m_parser;

  void operator()(int_8& variable)
  {
    // First read as an integer; then, cast to int8_t.
    int tmp;
    m_parser.m_is >> tmp;
    ++(m_parser.m_line);
    variable = (int8_t) tmp;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "[" << std::to_string(m_line) << "]"
                    << "Importing dxf int8 value: "
                    << (int)(variable) << std::endl;);
  }
};

/*! Import a Uint value.
 */
template <>
struct Dxf_importer<Uint> {

  Dxf_base_parser& m_parser;

  void operator()(Uint& variable)
  {
    m_parser.m_is >> std::hex >> variable >> std::dec;
    ++(m_parser.m_line);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "[" << std::to_string(m_line) << "]"
                    << "Importing dxf Uint value: "
                    << "0x" << std::hex << variable << std::dec << std::endl;);
  }
};

/*! Import a String value.
 */
template <>
struct Dxf_importer<String> {

  Dxf_base_parser& m_parser;

  void operator()(String& variable)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline
    // consumes the endline character left on the input stream by operator>>,
    // and returns immediately. Ignore all leftover characters.
    std::getline(m_parser.m_is, variable);
    ++(m_parser.m_line);
    variable.erase(variable.find_last_not_of(" \t\n\r\f\v") + 1);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "[" << std::to_string(m_line) << "]"
                    << "Importing dxf string value: "
                    << variable << std::endl;);
  }
};
#endif

SGAL_END_NAMESPACE

#endif
