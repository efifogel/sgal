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

#ifndef DXF_IMPORTER_HPP
#define DXF_IMPORTER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Base_loader.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

/*! Import a value.
 */
template <typename T>
struct Dxf_importer {

  SGAL::Base_loader& m_parser;

  Dxf_importer(SGAL::Base_loader& parser) : m_parser(parser) {}

  void operator()(T& variable)
  {
    m_parser.input_stream() >> variable;
    m_parser.inc_line();
    SGAL_TRACE_CODE(m_parser.get_trace_code(),
                    if (m_parser.get_verbose_level() >= 8)
                      std::cout << "[" << std::to_string(m_parser.line())
                                << "] " << "Importering dxf value: "
                                << variable << std::endl;);
  }
};

/*! Import an int8_t value.
 * C/C++ defines int8_t to be 'signed char'; thus, a negative integer cannot
 * be imported directly, since the preceding '-' is interpreted as the (sole)
 * char input.
 */
template <>
struct Dxf_importer<int8_t> {

  SGAL::Base_loader& m_parser;

  Dxf_importer(SGAL::Base_loader& parser) : m_parser(parser) {}

  void operator()(int8_t& variable)
  {
    // First read as an integer; then, cast to int8_t.
    int tmp;
    m_parser.input_stream() >> tmp;
    m_parser.inc_line();
    variable = (int8_t) tmp;
    SGAL_TRACE_CODE(m_parser.get_trace_code(),
                    if (m_parser.get_verbose_level() >= 8)
                      std::cout << "[" << std::to_string(m_parser.line()) << "] "
                                << "Importing dxf int8 value: "
                                << (int)(variable) << std::endl;);
  }
};

/*! Import a hex value.
 */
template <>
struct Dxf_importer<SGAL::Uint> {

  SGAL::Base_loader& m_parser;

  Dxf_importer(SGAL::Base_loader& parser) : m_parser(parser) {}

  void operator()(SGAL::Uint& variable)
  {
    m_parser.input_stream() >> std::hex >> variable >> std::dec;
    m_parser.inc_line();
    SGAL_TRACE_CODE(m_parser.get_trace_code(),
                    if (m_parser.get_verbose_level() >= 8)
                      std::cout << "[" << std::to_string(m_parser.line())
                                << "] " << "Importing dxf SGAL::Uint value: "
                                << std::hex << std::showbase << variable
                                << std::noshowbase << std::dec << std::endl;);
  }
};

/*! Import a SGAL::String value.
 */
template <>
struct Dxf_importer<SGAL::String> {

  SGAL::Base_loader& m_parser;

  Dxf_importer(SGAL::Base_loader& parser) : m_parser(parser) {}

  void operator()(SGAL::String& variable)
  {
    // use getline() cause the string might be empty.
    // When used immediately after whitespace-delimited input, getline
    // consumes the endline character left on the input stream by operator>>,
    // and returns immediately. Ignore all leftover characters.
    std::getline(m_parser.input_stream(), variable);
    m_parser.inc_line();
    variable.erase(variable.find_last_not_of(" \t\n\r\f\v") + 1);
    SGAL_TRACE_CODE(m_parser.get_trace_code(),
                    if (m_parser.get_verbose_level() >= 8)
                      std::cout << "[" << std::to_string(m_parser.line())
                                << "] " << "Importing dxf string value: "
                                << variable << std::endl;);
  }
};

DXF_END_NAMESPACE

#endif
