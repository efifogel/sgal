// Copyright (c) 2004,2018 Israel.
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

#include <string>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_parser.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

const std::array<String, 8> Dxf_parser::m_code_type_names = {
  "STRING", "FLOAT", "DOUBLE", "INT8", "INT16", "INT32", "UINT", "BOOL"
};

const std::vector<Dxf_parser::Code_range> Dxf_parser::m_code_ranges = {
  {0, 9, STRING},       // String (With the introduction of extended symbol
                        // names in AutoCAD 2000, the 255 character limit has
                        // been lifted. There is no explicit limit to the
                        // number of bytes per line, although most lines should
                        // fall within 2049 bytes.)
  {10, 59, DOUBLE},     // Double precision 3D point
  {60, 79, INT16},      // 16-bit integer value
  {90, 99, INT32},      // 32-bit integer value
  {100, 100, STRING},   // String (255-character maximum; less for Unicode
                        // strings)
  {102, 102, STRING},   // String (255-character maximum; less for Unicode
                        // strings)
  {105, 105, STRING},   // String representing hexadecimal (hex) handle value
  {140, 147, DOUBLE},   // Double precision scalar floating-point value
  {170, 175, INT16},    // 16-bit integer value
  {280, 289, INT8},     // 8-bit integer value
  {290, 299, BOOL},     // Boolean flag value
  {300, 309, STRING},   // Arbitrary text string
  {310, 319, STRING},   // String representing hex value of binary chunk
  {320, 329, STRING},   // String representing hex handle value
  {330, 369, STRING},   // String representing hex object IDs
  {370, 379, INT8},     // 8-bit integer value
  {380, 389, INT8},     // 8-bit integer value
  {390, 399, UINT},     // String representing hex handle value
  {400, 409, INT16},    // 16-bit integer value
  {410, 419, STRING},   // String
  {999, 999, STRING},   // Comment (string)
  {1000, 1009, STRING}, // String. (Same limits as indicated with 0-9 code
                        // range.)
  {1010, 1059, FLOAT},  // Floating-point value
  {1060, 1070, INT16},  // 16-bit integer value
  {1071, 1071, INT32}   // 32-bit integer value
};

const std::map<String, Dxf_parser::Section_parser> Dxf_parser::m_sections = {
  { "HEADER", &Dxf_parser::parse_header },
  { "CLASSES", &Dxf_parser::parse_classes },
  { "TABLES", &Dxf_parser::parse_tables },
  { "BLOCKS", &Dxf_parser::parse_blocks },
  { "ENTITIES", &Dxf_parser::parse_entities },
  { "OBJECTS", &Dxf_parser::parse_objects },
  { "THUMBNAILIMAGE", &Dxf_parser::parse_thumbnailimage }
};

#include "Dxf_header_variables.cpp"

//! \brief constructs.
Dxf_parser::Dxf_parser(std::istream& is, Scene_graph* sg) :
  m_is(is),
  m_scene_graph(sg)
{
  int16_t Dxf_header::*p = &Dxf_header::m_acadmaintver;
}

//! \brief parses.
Loader_code Dxf_parser::operator()()
{
  bool done(false);
  while (!done) {
    int n;
    m_is >> n;
    SGAL_assertion(n == 0);
    std::string str;
    m_is >> str;
    if (str == "EOF") {
      done = true;
      break;
    }

    SGAL_assertion(str == "SECTION");
    m_is >> n;
    SGAL_assertion(n == 2);
    std::string section;
    m_is >> section;
    auto sec_it = m_sections.find(section);
    if (sec_it == m_sections.end()) {
      SGAL_error_msg("unrecognize section");
    }
    (this->*(sec_it->second))();
  }

  return Loader_code::SUCCESS;
}

//! \brief parses the header section.
void Dxf_parser::parse_header()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_header()" << std::endl;);

  bool done(false);
  while (!done) {
    int n;
    m_is >> n;
    switch (n) {
     case 0: done = true; break;
     case 9: read_header_variable(); break;
     case 999: read_comment(); break;
    }
  }
}

void Dxf_parser::parse_classes() {}

void Dxf_parser::parse_tables() {}

void Dxf_parser::parse_blocks() {}

void Dxf_parser::parse_entities() {}

void Dxf_parser::parse_objects() {}

void Dxf_parser::parse_thumbnailimage() {}

/*! \brief reads a value from the input string and verify that it matches a
 * given code.
 */
Dxf_parser::Code_type Dxf_parser::read_code(int code)
{
  int n;
  m_is >> n;
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::read_header_variable() code: "
                  << n << std::endl;);
  SGAL_assertion(n == code);
  auto cr_it = std::find_if(m_code_ranges.begin(), m_code_ranges.end(),
                            [&](const Code_range& code_range)
                            {
                              return ((code_range.m_min <= code) &&
                                      (code <= code_range.m_max));
                            });

  if (cr_it == m_code_ranges.end()) {
    std::string unrecognized_msg("unrecognized code range");
    unrecognized_msg += std::to_string(code) + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }
  return cr_it->m_type;
}

//! \brief reads a HEADER veriable.
void Dxf_parser::read_header_variable()
{
  char c;
  m_is >> c;
  SGAL_assertion(c == '$');
  std::string str;
  m_is >> str;
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::read_header_variable() name: "
                  << str << std::endl;);
  auto it = m_header_variables.find(str);
  if (it == m_header_variables.end()) {
    std::string unrecognized_msg("unrecognized header variable ");
    unrecognized_msg += str + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }

  const auto& header_var = it->second;
  const auto& codes = header_var.m_codes;
  auto& handle = header_var.m_handle;

  auto dim = codes.size();
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::read_header_variable() dimension: "
                  << dim << std::endl;);
  if (1 == dim) {
    auto code = codes.front();
    auto code_type = read_code(code);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::read_header_variable() code type: "
                    << m_code_type_names[code_type] << std::endl;);
    switch (code_type) {
     case STRING: import_header_variable<String_header>(handle); break;
     case FLOAT: import_header_variable<Float_header>(handle); break;
     case DOUBLE: import_header_variable<Double_header>(handle); break;
     case INT8: import_header_variable<Int8_header>(handle); break;
     case INT16: import_header_variable<Int16_header>(handle); break;
     case INT32: import_header_variable<Int32_header>(handle); break;
     case UINT: import_header_variable<Uint_header>(handle); break;
     case BOOL: import_header_variable<Bool_header>(handle); break;
    }
    return;
  }

  SGAL_assertion((dim == 2) || (dim == 3));
  size_t i(0);
  for (auto code : codes) {
    auto code_type = read_code(code);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::read_header_variable() code type: "
                    << m_code_type_names[code_type] << std::endl;);
    SGAL_assertion(DOUBLE == code_type);
    if (dim == 2)
      m_is >> (m_header.*(boost::get<Double_2d_header>(handle)))[i++];
    else m_is >> (m_header.*(boost::get<Double_3d_header>(handle)))[i++];
  }
}

//! \brief reads a comment line.
void Dxf_parser::read_comment()
{
  m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::string str;
  std::getline(m_is, str);
}

SGAL_END_NAMESPACE
