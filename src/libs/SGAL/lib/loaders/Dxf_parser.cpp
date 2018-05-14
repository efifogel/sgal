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

const std::array<String, 8> Dxf_parser::s_code_type_names = {
  "STRING", "FLOAT", "DOUBLE", "INT8", "INT16", "INT32", "UINT", "BOOL"
};

const std::vector<Dxf_parser::Code_range> Dxf_parser::s_code_ranges = {
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
  {310, 319, UINT},     // String representing hex value of binary chunk
  {320, 329, UINT},     // String representing hex handle value
  {330, 369, UINT},     // String representing hex object IDs
  {370, 379, INT8},     // 8-bit integer value
  {380, 389, INT8},     // 8-bit integer value
  {390, 399, UINT},     // String representing hex handle value
  {400, 409, INT16},    // 16-bit integer value
  {410, 419, STRING},   // String
  {420, 429, INT32},    // 32-bit integer value
  {430, 439, STRING},   // String
  {440, 449, INT32},    // 32-bit integer value
  {450, 459, INT32},    // Long ???
  {460, 469, DOUBLE},   // Double-precision floating-point value
  {470, 479, STRING},   // String
  {480, 481, UINT},     // String representing hex handle value
  {999, 999, STRING},   // Comment (string)
  {1000, 1009, STRING}, // String. (Same limits as indicated with 0-9 code
                        // range.)
  {1010, 1059, FLOAT},  // Floating-point value
  {1060, 1070, INT16},  // 16-bit integer value
  {1071, 1071, INT32}   // 32-bit integer value
};

//!
const std::map<String, Dxf_parser::Section_parser> Dxf_parser::s_sections = {
  { "HEADER", &Dxf_parser::parse_header },
  { "CLASSES", &Dxf_parser::parse_classes },
  { "TABLES", &Dxf_parser::parse_tables },
  { "BLOCKS", &Dxf_parser::parse_blocks },
  { "ENTITIES", &Dxf_parser::parse_entities },
  { "OBJECTS", &Dxf_parser::parse_objects },
  { "THUMBNAILIMAGE", &Dxf_parser::parse_thumbnailimage }
};

//!
const std::map<int, Dxf_parser::Class_variable_type>
Dxf_parser::s_class_variables = {
  {1, &Dxf_class::m_record_name},
  {2, &Dxf_class::m_class_name},
  {3, &Dxf_class::m_application_name},
  {90, &Dxf_class::m_proxy_capability_flags},
  {91, &Dxf_class::m_instance_count},
  {280, &Dxf_class::m_was_class_loaded_with_file},
  {281, &Dxf_class::m_is_entity}
};

//!
const std::map<String, Dxf_parser::Table_parser>
Dxf_parser::s_tables = {
  { "APPID", &Dxf_parser::parse_appid_table },
  { "BLOCK_RECORD", &Dxf_parser::parse_block_record_table },
  { "DIMSTYLE", &Dxf_parser::parse_dimstyle_table },
  { "LAYER", &Dxf_parser::parse_layer_table },
  { "LTYPE", &Dxf_parser::parse_ltype_table },
  { "STYLE", &Dxf_parser::parse_style_table },
  { "UCS", &Dxf_parser::parse_ucs_table },
  { "VIEW", &Dxf_parser::parse_view_table },
  { "VPORT", &Dxf_parser::parse_vport_table }
};

//!
const std::map<int, Dxf_parser::Appid_table_variable_type>
Dxf_parser::s_appid_table_variables = {
  {2, &Dxf_appid_table::m_name},
  {70, &Dxf_appid_table::m_flags}
};

//!
const std::map<int, Dxf_parser::Block_record_table_variable_type>
Dxf_parser::s_block_record_table_variables = {
};

//!
const std::map<int, Dxf_parser::Dimstyle_table_variable_type>
Dxf_parser::s_dimstyle_table_variables = {
};

//!
const std::map<int, Dxf_parser::Layer_table_variable_type>
Dxf_parser::s_layer_table_variables = {
};

//!
const std::map<int, Dxf_parser::Ltype_table_variable_type>
Dxf_parser::s_ltype_table_variables = {
};

//!
const std::map<int, Dxf_parser::Style_table_variable_type>
Dxf_parser::s_style_table_variables = {
};

//!
const std::map<int, Dxf_parser::Ucs_table_variable_type>
Dxf_parser::s_ucs_table_variables = {
};

//!
const std::map<int, Dxf_parser::View_table_variable_type>
Dxf_parser::s_view_table_variables = {
};

//! \brief constructs.
Dxf_parser::Dxf_parser(std::istream& is, Scene_graph* sg) :
  m_is(is),
  m_scene_graph(sg)
{}

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
    auto sec_it = s_sections.find(section);
    if (sec_it == s_sections.end()) {
      SGAL_error_msg("unrecognize section");
    }
    (this->*(sec_it->second))();
  }

  return Loader_code::SUCCESS;
}

//! \brief obtains the type of a code
Dxf_parser::Code_type Dxf_parser::code_type(int code)
{
  auto it = std::find_if(s_code_ranges.begin(), s_code_ranges.end(),
                         [&](const Code_range& code_range)
                         {
                           return ((code_range.m_min <= code) &&
                                   (code <= code_range.m_max));
                         });

  if (it == s_code_ranges.end()) {
    std::string unrecognized_msg("unrecognized code range");
    unrecognized_msg += std::to_string(code) + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }
  return it->m_type;
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
  String str;
  m_is >> str;
  SGAL_assertion("ENDSEC" == str);
}

//! \brief parses one class.
void Dxf_parser::parse_class()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_class()" << std::endl;);

  m_classes.push_back(Dxf_class());
  auto& dxf_class = m_classes.back();

  bool done(false);
  while (!done) {
    int code;
    m_is >> code;
    if (0 == code) {
      done = true;
      break;
    }

    auto it = s_class_variables.find(code);
    SGAL_assertion(it != s_class_variables.end());
    auto handle = it->second;
    switch (code_type(code)) {
     case STRING: import_string_variable<String_class>(handle, dxf_class); break;
     case INT8: import_variable<Int8_class>(handle, dxf_class); break;
     case INT32: import_variable<Int32_class>(handle, dxf_class); break;
     default: SGAL_error();
    }
  }
}

//! \brief parses the CLASSES section.
void Dxf_parser::parse_classes()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_classes()" << std::endl;);

  int n;
  m_is >> n;
  SGAL_assertion(0 == n);

  do {
    String str;
    m_is >> str;
    if ("ENDSEC" == str) return;

    SGAL_assertion("CLASS" == str);
    parse_class();
  } while (true);
}

//! \brief parses common table section, which aplies to all table types.
int Dxf_parser::parse_common_table(Dxf_table& table, const std::string& type)
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_common_table()"
                  << std::endl;);

  size_t max_num(0);
  bool start(true);

  bool done(false);
  while (!done) {
    int code;
    m_is >> code;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::parse_common_table() code: "
                    << code << std::endl;);
    if (0 == code) {
      done = true;
      break;
    }

    String tmp;
    switch (code) {
     case 5: m_is >> table.m_handle; break;
     case 102:
      if (start) m_is >> table.m_group;
      else m_is >> tmp;
      start = ! start;
      break;
     case 360: m_is >> table.m_owner_dict; break;
     case 330: m_is >> table.m_owner_obj; break;
     case 100: m_is >> tmp; /* What is a subclass marker for? */ break;
     case 70: m_is >> max_num; break;
     default: SGAL_error();
    }
  }
  SGAL_assertion(start);

  std::string str;
  m_is >> str;
  SGAL_assertion(str == type);

  return max_num;
}

//! \brief parses a table of type APPID.
void Dxf_parser::parse_appid_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_appid_table()"
                  << std::endl;);

  auto* table = new Dxf_appid_table;
  m_tables.push_back(table);
  parse_common_table(*table, "APPID");

  bool done(false);
  while (!done) {
    int code;
    m_is >> code;
    if (0 == code) {
      done = true;
      break;
    }

    auto it = s_appid_table_variables.find(code);
    SGAL_assertion(it != s_appid_table_variables.end());
    auto handle = it->second;
    switch (code_type(code)) {
     STRING: import_string_variable<String_appid_table>(handle, *table); break;
     INT16: import_variable<Int16_appid_table>(handle, *table); break;
     default: SGAL_error();
    }
  }
}

//! \brief parses a table of type BLOCK_RECORD.
void Dxf_parser::parse_block_record_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_block_record_table()"
                  << std::endl;);
}

//! \brief parses a table of type DIMSTYLE.
void Dxf_parser::parse_dimstyle_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_dimstyle_table()"
                  << std::endl;);
}

//! \brief parses a table of type LAYER.
void Dxf_parser::parse_layer_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_layer_table()"
                  << std::endl;);
}

//! \brief parses a table of type LTYPE.
void Dxf_parser::parse_ltype_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_ltype_table()"
                  << std::endl;);
}

//! \brief parses a table of type STYLE.
void Dxf_parser::parse_style_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_style_table()"
                  << std::endl;);
}

//! \brief parses a table of type UCS.
void Dxf_parser::parse_ucs_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_ucs_table()"
                  << std::endl;);
}

//! \brief parses a table of type VIEW.
void Dxf_parser::parse_view_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_view_table()"
                  << std::endl;);
}

//! \brief parses a table of type VPORT.
void Dxf_parser::parse_vport_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_vport_table()"
                  << std::endl;);

  auto* table = new Dxf_vport_table;
  m_tables.push_back(table);
  parse_common_table(*table, "VPORT");

  bool done(false);
  while (!done) {
    int code;
    m_is >> code;
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::parse_vport_table() code: "
                    << code << std::endl;);
    if (0 == code) {
      done = true;
      break;
    }

    auto it = s_vport_table_variables.find(code);
    SGAL_assertion(it != s_vport_table_variables.end());
    auto handle = it->second;
    switch (code_type(code)) {
     STRING: import_string_variable<String_vport_table>(handle, *table); break;
     BOOL: import_variable<Bool_vport_table>(handle, *table); break;
     INT8: import_variable<Int8_vport_table>(handle, *table); break;
     INT16: import_variable<Int16_vport_table>(handle, *table); break;
     INT32: import_variable<Int32_vport_table>(handle, *table); break;
     DOUBLE: import_variable<Double_vport_table>(handle, *table); break;
     default: SGAL_error();
    }
  }
}

//! \brief parses one table.
void Dxf_parser::parse_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_table()" << std::endl;);

  int n;
  m_is >> n;
  SGAL_assertion(2 == n);
  std::string str;
  m_is >> str;
  auto table_it = s_tables.find(str);
  if (table_it == s_tables.end()) {
    SGAL_error_msg("unrecognize table");
  }
  (this->*(table_it->second))();


  // m_is >> n;
  // SGAL_assertion(n == 2);
  // std::string type;
  // m_is >> type;
  // while (type == "LAYER") {
  //   std::string layer_name;
  //   std::string linetype_name;
  //   int flags;
  //   int color_num;
  //   bool done(false);
  //   while (!done) {
  //     int code;
  //     m_is >> code;
  //     switch (code) {
  //      case 0: done = true; break;
  //      case 2: m_is >> layer_name; break;
  //      case 6: m_is >> linetype_name; break;
  //      case 62: m_is >> color_num; break;
  //      case 70: m_is >> flags; break;
  //      case 100: break;;
  //      default:
  //       std::string msg("unrecognized LAYER group code ");
  //       msg += std::to_string(code) + "!";
  //       SGAL_error_msg(msg.c_str());
  //     }
  //   }
  //   m_is >> type;

  m_is >> str;
  SGAL_assertion("ENDTAB" == str);
  m_is >> n;
  SGAL_assertion(0 == n);
}

//! \brief parses the TABLES section.
void Dxf_parser::parse_tables()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_tables()" << std::endl;);

  int n;
  m_is >> n;
  SGAL_assertion(0 == n);

  do {
    std::string str;
    m_is >> str;
    if ("ENDSEC" == str) return;

    SGAL_assertion("TABLE" == str);
    parse_table();
  } while (true);
}

void Dxf_parser::parse_blocks()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_blocks()" << std::endl;);

  std::string handle;
  std::string layer_name;
  std::string block_name;
  std::string xref_path_name;
  int flags;
  int follow_flag;
  double x_value;
  double y_value;
  double z_value;
  double fp;

  int n;
  m_is >> n;
  SGAL_assertion(n == 0);
  std::string str;
  m_is >> str;
  while (str == "BLOCK") {
    do {
      bool done(false);
      while (!done) {
        int code;
        m_is >> code;
        switch (code) {
         case 0: done = true; break;
         case 1: m_is >> xref_path_name; break;
         case 2:
         case 3: m_is >> block_name; break;     // block name
         case 5: m_is >> handle; break;         // handle
         case 8: m_is >> layer_name; break;     // layer name
         case 10: m_is >> x_value; break;       // x-value
         case 20: m_is >> y_value; break;       // y-value
         case 30: m_is >> z_value; break;       // z-value
         case 40:
         case 41:
         case 42: m_is >> fp; break;            // floating-point value
         case 66: m_is >> follow_flag; break;   // "Entities follow" flag
         case 70: m_is >> flags; break;         // block-type flags
         case 100: break;                       // subclass
         case 102: break;                       // control string
         default:
          std::string msg("unrecognized BLOCK group code ");
          msg += std::to_string(code) + "!";
          SGAL_error_msg(msg.c_str());
        }
      }
      m_is >> str;
    } while (str != "ENDBLK");
    m_is >> n;
    SGAL_assertion(n == 0);
    m_is >> str;
  }
  SGAL_assertion(str == "ENDSEC");
}

void Dxf_parser::parse_entities()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_entities()" << std::endl;);
}

void Dxf_parser::parse_objects()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_objects()" << std::endl;);
}

void Dxf_parser::parse_thumbnailimage()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_thumbnailimage()"
                  << std::endl;);
}

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
  return code_type(code);
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
  auto it = s_header_variables.find(str);
  if (it == s_header_variables.end()) {
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
                    << s_code_type_names[code_type] << std::endl;);
    switch (code_type) {
     case STRING: import_string_variable<String_header>(handle, m_header); break;
     case FLOAT: import_variable<Float_header>(handle, m_header); break;
     case DOUBLE: import_variable<Double_header>(handle, m_header); break;
     case INT8: import_variable<Int8_header>(handle, m_header); break;
     case INT16: import_variable<Int16_header>(handle, m_header); break;
     case INT32: import_variable<Int32_header>(handle, m_header); break;
     case UINT: import_variable<Uint_header>(handle, m_header); break;
     case BOOL: import_variable<Bool_header>(handle, m_header); break;
    }
    return;
  }

  SGAL_assertion((dim == 2) || (dim == 3));
  size_t i(0);
  for (auto code : codes) {
    auto code_type = read_code(code);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::read_header_variable() code type: "
                    << s_code_type_names[code_type] << std::endl;);
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
