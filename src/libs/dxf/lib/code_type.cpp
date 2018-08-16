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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Code_type.hpp"

DXF_BEGIN_NAMESPACE

//! Name of code types
static const std::array<std::string, 8> s_code_type_names = {
  "STRING", "DOUBLE", "INT8", "INT16", "INT32", "UINT", "BOOL"
};

//! Ranges of DXF codes
static const std::vector<Code_range> s_code_ranges = {
  {   0,    9, Code_type::STRING},// SGAL::String (With the introduction of
                                  // extended symbol names in AutoCAD 2000, the
                                  // 255 character limit has been lifted. There
                                  // is no explicit limit to the number of bytes
                                  // per line, although most lines should fall
                                  // within 2049 bytes.)
  {  10,   59, Code_type::DOUBLE},// Double precision 3D point
  {  60,   79, Code_type::INT16}, // 16-bit integer value
  {  90,   99, Code_type::INT32}, // 32-bit integer value
  { 100,  100, Code_type::STRING},// SGAL::String (255-character maximum; less
                                  // for Unicode strings)
  { 102,  102, Code_type::STRING},// SGAL::String (255-character maximum; less
                                  // for Unicode strings)
  { 105,  105, Code_type::STRING},// SGAL::String representing hexadecimal (hex)
                                  // handle value
  { 110,  112, Code_type::DOUBLE},// x-value of UCS
  { 120,  122, Code_type::DOUBLE},// y-value of UCS
  { 130,  132, Code_type::DOUBLE},// z-value of UCS
  { 140,  149, Code_type::DOUBLE},// Double precision scalar floating-point value
  { 160,  169, Code_type::DOUBLE},// Double precision floating-point value
  { 170,  179, Code_type::INT16}, // 16-bit integer value
  { 210,  239, Code_type::DOUBLE},// x-value of extrusion direction
  { 270,  279, Code_type::INT16}, // 16-bit integer value
  { 280,  289, Code_type::INT8},  // 8-bit integer value
  { 290,  299, Code_type::BOOL},  // Boolean flag value
  { 300,  309, Code_type::STRING},// Arbitrary text string
  { 310,  319, Code_type::STRING},// SGAL::String representing hex value of
                                  // binary chunk
  { 320,  329, Code_type::UINT},  // SGAL::String representing hex handle value
  { 330,  369, Code_type::STRING},// SGAL::String representing hex object IDs
  { 370,  379, Code_type::INT8},  // 8-bit integer value
  { 380,  389, Code_type::INT8},  // 8-bit integer value
  { 390,  399, Code_type::STRING},// SGAL::String representing hex handle value
  { 400,  409, Code_type::INT16}, // 16-bit integer value
  { 410,  419, Code_type::STRING},// SGAL::String
  { 420,  429, Code_type::INT32}, // 32-bit integer value
  { 430,  439, Code_type::STRING},// SGAL::String
  { 440,  449, Code_type::INT32}, // 32-bit integer value
  { 450,  459, Code_type::INT32}, // Long ???
  { 460,  469, Code_type::DOUBLE},// Double-precision floating-point value
  { 470,  479, Code_type::STRING},// SGAL::String
  { 480,  481, Code_type::UINT},  // SGAL::String representing hex handle value
  { 999,  999, Code_type::STRING},// Comment (string)
  {1000, 1009, Code_type::STRING},// SGAL::String. (Same limits as indicated
                                  // with 0-9 code range.)
  {1010, 1059, Code_type::DOUBLE},// Double-precision floating-point value
  {1060, 1070, Code_type::INT16}, // 16-bit integer value
  {1071, 1071, Code_type::INT32}  // 32-bit integer value
};

//! \brief obtains the type of a code
Code_type code_type(int code)
{
  auto it = std::find_if(s_code_ranges.begin(), s_code_ranges.end(),
                         [&](const Code_range& code_range)
                         {
                           return ((code_range.m_min <= code) &&
                                   (code <= code_range.m_max));
                         });

  if (it == s_code_ranges.end()) {
    std::string unrecognized_msg("Unrecognized code (in range) ");
    unrecognized_msg += std::to_string(code) + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }
  return it->m_type;
}

//! \brief obtain the name of a code.
const std::string& code_type_name(Code_type ct)
{ return s_code_type_names[static_cast<size_t>(ct)]; }

DXF_END_NAMESPACE
