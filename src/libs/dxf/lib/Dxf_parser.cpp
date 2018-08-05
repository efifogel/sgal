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

#include <string>
#include <iostream>
#include <functional>
#include <streambuf>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_base_boundary_path.hpp"
#include "dxf/Dxf_boundary_path.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"
#include "dxf/Dxf_pattern_data.hpp"

DXF_BEGIN_NAMESPACE

//! Default color palette
std::vector<SGAL::Vector3f> Dxf_parser::s_palette = {
  {0, 0, 0},            //  0, Black,     0,  0,  0, #000000
  {1, 0, 0},            //  1, Red,     255,  0,  0, #ff0000
  {1, 1, 0},            //  2, Yellow,  255,255,  0, #ffff00
  {0, 1, 0},            //  3, Green,     0,255,  0, #00ff00
  {0, 1, 1},            //  4, Cyan,      0,255,255, #00ffff
  {0, 0, 1},            //  5, Blue,      0,  0,  1, #0000ff
  {1, 0, 1},            //  6, Magenta, 255,  0,255, #ff00ff
  {1, 1, 1},            //  7, White,     1,  1,  1, #ffffff
  {0.5, 0.5, 0.5},      //  8
  {0.5, 0, 0},          //  9
  {0.5, 0.5, 0},        // 10
  {0, 0.5, 0},          // 11
  {0, 0.5, 0.5},        // 12
  {0, 0, 0.5},          // 13
  {0.5, 0, 0.5},        // 14
  {0.75, 0.75, 0.75}    // 15
};

//! Name of code types
const std::array<SGAL::String, 8> Dxf_parser::s_code_type_names = {
  "STRING", "DOUBLE", "INT8", "INT16", "INT32", "UINT", "BOOL"
};

const std::vector<Dxf_parser::Code_range> Dxf_parser::s_code_ranges = {
  {0, 9, STRING},       // SGAL::String (With the introduction of extended
                        // symbol names in AutoCAD 2000, the 255 character limit
                        // has been lifted. There is no explicit limit to the
                        // number of bytes per line, although most lines should
                        // fall within 2049 bytes.)
  {10, 59, DOUBLE},     // Double precision 3D point
  {60, 79, INT16},      // 16-bit integer value
  {90, 99, INT32},      // 32-bit integer value
  {100, 100, STRING},   // SGAL::String (255-character maximum; less for Unicode
                        // strings)
  {102, 102, STRING},   // SGAL::String (255-character maximum; less for Unicode
                        // strings)
  {105, 105, STRING},   // SGAL::String representing hexadecimal (hex) handle
                        // value
  {110, 112, DOUBLE},   // x-value of UCS
  {120, 122, DOUBLE},   // y-value of UCS
  {130, 132, DOUBLE},   // z-value of UCS
  {140, 149, DOUBLE},   // Double precision scalar floating-point value
  {160, 169, DOUBLE},   // Double precision floating-point value
  {170, 179, INT16},    // 16-bit integer value
  {210, 239, DOUBLE},   // x-value of extrusion direction
  {270, 279, INT16},    // 16-bit integer value
  {280, 289, INT8},     // 8-bit integer value
  {290, 299, BOOL},     // Boolean flag value
  {300, 309, STRING},   // Arbitrary text string
  {310, 319, STRING},   // SGAL::String representing hex value of binary chunk
  {320, 329, UINT},     // SGAL::String representing hex handle value
  {330, 369, STRING},   // SGAL::String representing hex object IDs
  {370, 379, INT8},     // 8-bit integer value
  {380, 389, INT8},     // 8-bit integer value
  {390, 399, STRING},   // SGAL::String representing hex handle value
  {400, 409, INT16},    // 16-bit integer value
  {410, 419, STRING},   // SGAL::String
  {420, 429, INT32},    // 32-bit integer value
  {430, 439, STRING},   // SGAL::String
  {440, 449, INT32},    // 32-bit integer value
  {450, 459, INT32},    // Long ???
  {460, 469, DOUBLE},   // Double-precision floating-point value
  {470, 479, STRING},   // SGAL::String
  {480, 481, UINT},     // SGAL::String representing hex handle value
  {999, 999, STRING},   // Comment (string)
  {1000, 1009, STRING}, // SGAL::String. (Same limits as indicated with 0-9 code
                        // range.)
  {1010, 1059, DOUBLE}, // Double-precision floating-point value
  {1060, 1070, INT16},  // 16-bit integer value
  {1071, 1071, INT32}   // 32-bit integer value
};

//!
const std::map<SGAL::String, Dxf_parser::Section_parser>
Dxf_parser::s_sections = {
  {"HEADER", &Dxf_parser::parse_header},
  {"CLASSES", &Dxf_parser::parse_classes},
  {"TABLES", &Dxf_parser::parse_tables},
  {"BLOCKS", &Dxf_parser::parse_blocks},
  {"ENTITIES", &Dxf_parser::parse_entities},
  {"OBJECTS", &Dxf_parser::parse_objects},
  {"THUMBNAILIMAGE", &Dxf_parser::parse_thumbnailimage}
};

//!
const std::map<int, Dxf_parser::Class_member_type>
Dxf_parser::s_class_members = {
  {1, &Dxf_class::m_record_name},
  {2, &Dxf_class::m_class_name},
  {3, &Dxf_class::m_application_name},
  {90, &Dxf_class::m_proxy_capability_flags},
  {91, &Dxf_class::m_instance_count},
  {280, &Dxf_class::m_was_class_loaded_with_file},
  {281, &Dxf_class::m_is_entity}
};

//!
const std::map<SGAL::String, Dxf_parser::Table_parser>
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

/// Appid table
//@{
typedef Dxf_record_wrapper<Dxf_appid_entry>     Dxf_appid_wrapper;

//!
template <>
const std::map<int, Dxf_appid_wrapper::Record_member>
Dxf_appid_wrapper::s_record_members = {
  {2, {&Dxf_appid_entry::m_name, 1, 0}},
  {70, {&Dxf_appid_entry::m_flags, 1, 0}}
};

//!
template <>
const std::map<int, Dxf_appid_wrapper::Record_handler_type>
Dxf_appid_wrapper::s_record_handlers = {};
//@}

/// Ucs table
typedef Dxf_record_wrapper<Dxf_ucs_entry>               Dxf_ucs_wrapper;

//!
template <>
const std::map<int, Dxf_ucs_wrapper::Record_member>
Dxf_ucs_wrapper::s_record_members = {};

//!
template <>
const std::map<int, Dxf_ucs_wrapper::Record_handler_type>
Dxf_ucs_wrapper::s_record_handlers = {};
//@}

//!
typedef Dxf_simple_record_wrapper<Dxf_base_table>       Dxf_base_table_wrapper;
template <>
const std::map<int, Dxf_base_table_wrapper::Record_member_type>
Dxf_base_table_wrapper::s_record_members = {
  {5, &Dxf_base_table::m_handle},
  {360, &Dxf_base_table::m_owner_dict},
  {330, &Dxf_base_table::m_owner_obj}
};

//!
typedef Dxf_simple_record_wrapper<Dxf_table_entry>      Dxf_entry_wrapper;
template <>
const std::map<int, Dxf_entry_wrapper::Record_member_type>
Dxf_entry_wrapper::s_record_members = {
  {5, &Dxf_table_entry::m_handle},
  {105, &Dxf_table_entry::m_handle}, // DYMSTYLE only, probably due to old vers.
  {360, &Dxf_table_entry::m_owner_dict},
  {330, &Dxf_table_entry::m_owner_obj}
};

//!
const std::map<SGAL::String, Dxf_parser::Entity_parser>
Dxf_parser::s_entities = {
  { "3DFACE", &Dxf_parser::parse_3dface_entity },
  { "3DSOLID", &Dxf_parser::parse_3dsolid_entity },
  { "ACAD_PROXY_ENTITY", &Dxf_parser::parse_acad_proxy_entity },
  { "ARC", &Dxf_parser::parse_arc_entity },
  { "ARCALIGNEDTEXT", &Dxf_parser::parse_arcalignedtext_entity },
  { "ATTDEF", &Dxf_parser::parse_attdef_entity },
  { "ATTRIB", &Dxf_parser::parse_attrib_entity },
  { "BODY", &Dxf_parser::parse_body_entity },
  { "CIRCLE", &Dxf_parser::parse_circle_entity },
  { "DIMENSION", &Dxf_parser::parse_dimension_entity },
  { "ELLIPSE", &Dxf_parser::parse_ellipse_entity },
  { "HATCH", &Dxf_parser::parse_hatch_entity },
  { "IMAGE", &Dxf_parser::parse_image_entity },
  { "INSERT", &Dxf_parser::parse_insert_entity },
  { "LEADER", &Dxf_parser::parse_leader_entity },
  { "LINE", &Dxf_parser::parse_line_entity },
  { "LWPOLYLINE", &Dxf_parser::parse_lwpolyline_entity },
  { "MLINE", &Dxf_parser::parse_mline_entity },
  { "MTEXT", &Dxf_parser::parse_mtext_entity },
  { "OLEFRAME", &Dxf_parser::parse_oleframe_entity },
  { "OLE2FRAME", &Dxf_parser::parse_ole2frame_entity },
  { "POINT", &Dxf_parser::parse_point_entity },
  { "POLYLINE", &Dxf_parser::parse_polyline_entity },
  { "RAY", &Dxf_parser::parse_ray_entity },
  { "REGION", &Dxf_parser::parse_region_entity },
  { "RTEXT", &Dxf_parser::parse_rtext_entity },
  { "SEQEND", &Dxf_parser::parse_seqend_entity },
  { "SHAPE", &Dxf_parser::parse_shape_entity },
  { "SOLID", &Dxf_parser::parse_solid_entity },
  { "SPLINE", &Dxf_parser::parse_spline_entity },
  { "TEXT", &Dxf_parser::parse_text_entity },
  { "TOLERANCE", &Dxf_parser::parse_tolerance_entity },
  { "TRACE", &Dxf_parser::parse_trace_entity },
  { "VERTEX", &Dxf_parser::parse_vertex_entity },
  { "VIEWPORT", &Dxf_parser::parse_viewport_entity },
  { "WIPEOUT", &Dxf_parser::parse_wipeout_entity },
  { "XLINE", &Dxf_parser::parse_xline_entity }
};

//! Object parsers
const std::map<SGAL::String, Dxf_parser::Object_parser>
Dxf_parser::s_objects = {
  {"ACAD_PROXY_OBJECT", &Dxf_parser::parse_acad_proxy_object},
  {"ACDBDICTIONARYWDFLT", &Dxf_parser::parse_acdbdictionarywdflt_object},
  {"ACDBNAVISWORKSMODELDEF", &Dxf_parser::parse_acdbnavisworksmodeldef_object},
  {"ACDBPLACEHOLDER", &Dxf_parser::parse_acdbplaceholder_object},
  {"DATATABLE", &Dxf_parser::parse_datatable_object},
  {"DICTIONARY", &Dxf_parser::parse_dictionary_object},
  {"DICTIONARYVAR", &Dxf_parser::parse_dictionaryvar_object},
  {"DIMASSOC", &Dxf_parser::parse_dimassoc_object},
  {"FIELD", &Dxf_parser::parse_field_object},
  {"GEODATA", &Dxf_parser::parse_geodata_object},
  {"GROUP", &Dxf_parser::parse_group_object},
  {"IDBUFFER", &Dxf_parser::parse_idbuffer_object},
  {"IMAGEDEF", &Dxf_parser::parse_imagedef_object},
  {"IMAGEDEF_REACTOR", &Dxf_parser::parse_imagedef_reactor_object},
  {"LAYER_FILTER", &Dxf_parser::parse_layer_filter_object},
  {"LAYER_INDEX", &Dxf_parser::parse_layer_index_object},
  {"LAYOUT", &Dxf_parser::parse_layout_object},
  {"LIGHTLIST", &Dxf_parser::parse_lightlist_object},
  {"MATERIAL", &Dxf_parser::parse_material_object},
  {"MLINESTYLE", &Dxf_parser::parse_mlinestyle_object},
  {"OBJECT_PTR", &Dxf_parser::parse_object_ptr_object},
  {"PLOTSETTINGS", &Dxf_parser::parse_plotsettings_object},
  {"RASTERVARIABLES", &Dxf_parser::parse_rastervariables_object},
  {"RENDER", &Dxf_parser::parse_render_object},
  {"SECTION", &Dxf_parser::parse_section_object},
  {"SORTENTSTABLE", &Dxf_parser::parse_sortentstable_object},
  {"SPATIAL_FILTER", &Dxf_parser::parse_spatial_filter_object},
  {"SPATIAL_INDEX", &Dxf_parser::parse_spatial_index_object},
  {"SUNSTUDY", &Dxf_parser::parse_sunstudy_object},
  {"TABLESTYLE", &Dxf_parser::parse_tablestyle_object},
  {"UNDERLAYDEFINITION", &Dxf_parser::parse_underlaydefinition_object},
  {"VBA_PROJECT", &Dxf_parser::parse_vba_project_object},
  {"VISUALSTYLE", &Dxf_parser::parse_visualstyle_object},
  {"WIPEOUTVARIABLES", &Dxf_parser::parse_wipeoutvariables_object},
  {"XRECORD", &Dxf_parser::parse_xrecord_object}
};

//!
typedef Dxf_simple_record_wrapper<Dxf_base_object>
                                                Dxf_base_object_wrapper;

//!
template <>
const std::map<int, Dxf_base_object_wrapper::Record_member_type>
Dxf_base_object_wrapper::s_record_members = {
  {5, &Dxf_base_object::m_handle},
  {360, &Dxf_base_object::m_owner_dict},
  {330, &Dxf_base_object::m_owner_object}
};

//! \brief constructs.
Dxf_parser::Dxf_parser() :
  Base_loader(),
  m_pending_code(0),
  m_is_pending(false),
  m_extended_data(nullptr),
  m_report_unrecognized_code(false),
  m_active_polyline_entity(nullptr),
  m_arcs_refinement_num(16),
  m_lines_num(0),
  m_polylines_num(0),
  m_lwpolylines_num(0),
  m_circles_num(0),
  m_arcs_num(0),
  m_hatches_num(0),
  m_splines_num(0),
  m_solids_num(0),
  m_inserts_num(0)
{}

//! \brief parses.
SGAL::Loader_code Dxf_parser::operator()(std::istream& is,
                                         SGAL::Scene_graph* sg,
                                         const SGAL::String& filename)
{
  Base_loader::operator()(is, sg, filename);
  m_pending_code = 0;
  m_is_pending = false;
  m_extended_data = nullptr;

  //! \todo Make the Configuration a singleton and remove it from the
  // scene-graph. Then, move the following code exerpt to dxf_init().
  auto* conf = sg->get_configuration();
  if (conf) {
    auto dxf_conf = conf->get_dxf_configuration();
    if (dxf_conf) {
      auto& palette_file_name = dxf_conf->get_palette_file_name();
      init_palette(palette_file_name);
    }
  }

  while (true) {
    int n;
    import_code(n);
    SGAL_assertion(n == 0);
    std::string str;
    import_value(str);
    if (str == "EOF") break;
    SGAL_assertion(str == "SECTION");
    import_code(n);
    SGAL_assertion(n == 2);
    std::string section;
    import_value(section);
    auto sec_it = s_sections.find(section);
    if (sec_it == s_sections.end()) {
      SGAL_error_msg("Unrecognize section");
    }
    (this->*(sec_it->second))();
  }

  //! \todo Instead of storing the data in the parser records and then copy it
  // over to the right place, intercept the parsing and store the data at the
  // right place immediately after read from the imput stream.

  //
  m_scene_graph->set_input_format_id(SGAL::File_format_3d::ID_DXF);
  process_layers();
  auto* root = m_scene_graph->initialize();

  add_background(root);
  process_entities(m_entities, root);

#if ! defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(m_trace_code)) {
    std::cout << "Processed " << m_lines_num << " lines" << std::endl;
    std::cout << "Processed " << m_polylines_num << " polylines" << std::endl;
    std::cout << "Processed " << m_lwpolylines_num << " lwpolylines" << std::endl;
    std::cout << "Processed " << m_circles_num << " circles" << std::endl;
    std::cout << "Processed " << m_arcs_num << " arcs" << std::endl;
    std::cout << "Processed " << m_hatches_num << " hatches" << std::endl;
    std::cout << "Processed " << m_solids_num << " solids" << std::endl;
    std::cout << "Processed " << m_inserts_num << " inserts" << std::endl;
  }
#endif

  clear();

  return SGAL::Loader_code::SUCCESS;
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
    std::string unrecognized_msg("Unrecognized code (in range) ");
    unrecognized_msg +=
      std::to_string(code) + "\", at line " + std::to_string(m_line) + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }
  return it->m_type;
}

//! \brief parses the header section.
void Dxf_parser::parse_header()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing HEADER section" << std::endl;);

  bool done(false);
  while (!done) {
    int n;
    import_code(n);
    switch (n) {
     case 0: done = true; break;
     case 9: read_header_member(); break;
     case 999: read_comment(); break;
    }
  }
  SGAL::String str;
  import_value(str);
  SGAL_assertion("ENDSEC" == str);
}

//! \brief parses one class.
void Dxf_parser::parse_class()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse class" << std::endl;);

  m_classes.push_back(Dxf_class());
  auto& dxf_class = m_classes.back();

  bool done(false);
  while (!done) {
    int code;
    import_code(code);
    if (0 == code) {
      done = true;
      break;
    }

    auto it = s_class_members.find(code);
    SGAL_assertion(it != s_class_members.end());
    auto handle = it->second;
    switch (code_type(code)) {
     case STRING: assign_member<String_class>(handle, dxf_class); break;
     case INT8: assign_member<Int8_class>(handle, dxf_class); break;
     case INT32: assign_member<Int32_class>(handle, dxf_class); break;
     default: SGAL_error();
    }
  }
}

//! \brief parses the CLASSES section.
void Dxf_parser::parse_classes()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parser CLASSES section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    SGAL::String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    SGAL_assertion("CLASS" == str);
    parse_class();
  } while (true);
}

//! \brief parses a table of type APPID.
void Dxf_parser::parse_appid_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse APPID table" << std::endl;);
  parse_table(m_appid_table, "APPID");
}

//! \brief parses a table of type BLOCK_RECORD.
void Dxf_parser::parse_block_record_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse BLOCK_RECORD table" << std::endl;);
  parse_table(m_block_record_table, "BLOCK_RECORD");
}

//! \brief parses a table of type DIMSTYLE.
void Dxf_parser::parse_dimstyle_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing DIMSTYLE table" << std::endl;);
  parse_table(m_dimstyle_table, "DIMSTYLE");
}

//! \brief parses a table of type LAYER.
void Dxf_parser::parse_layer_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing LAYER table" << std::endl;);
  parse_table(m_layer_table, "LAYER");
}

//! \brief parses a table of type LTYPE.
void Dxf_parser::parse_ltype_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing LTYPE table" << std::endl;);
  parse_table(m_ltype_table, "LTYPE");
}

//! \brief parses a table of type STYLE.
void Dxf_parser::parse_style_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing STYLE table" << std::endl;);
  parse_table(m_style_table, "STYLE");
}

//! \brief parses a table of type UCS.
void Dxf_parser::parse_ucs_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing UCS table" << std::endl;);
  parse_table(m_ucs_table, "UCS");
}

//! \brief parses a table of type VIEW.
void Dxf_parser::parse_view_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing VIEW table" << std::endl;);
  parse_table(m_view_table, "VIEW");
}

//! \brief parses a table of type VPORT.
void Dxf_parser::parse_vport_table()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing VPORT table" << std::endl;);
  parse_table(m_vport_table, "VPORT");
}

//! \brief parses the TABLES section.
void Dxf_parser::parse_tables()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing TABLES section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    std::string str;
    import_value(str);
    if ("ENDSEC" == str) return;

    SGAL_assertion("TABLE" == str);

    int n;
    import_code(n);
    SGAL_assertion(2 == n);
    import_value(str);
    auto table_it = s_tables.find(str);
    if (table_it == s_tables.end()) {
      SGAL_error_msg("unrecognize table");
    }
    (this->*(table_it->second))();

    import_code(n);
    SGAL_assertion(0 == n);

  } while (true);
}

//! \brief parses one block.
void Dxf_parser::parse_block(Dxf_block& block)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing block" << std::endl;);

  while (true) {
    int code;
    import_code(code);
    if (0 == code) return;

    if (100 == code) {
      import_value(m_marker);
      continue;
    }

    if (102 == code) {
      read_xdata_block(code, block);
      continue;
    }

    auto& members = Dxf_record_wrapper<Dxf_block>::s_record_members;
    if (assign_record_value(code, block, members)) continue;

    handle_record_special_value(code, block);
  }
}

//! \brief parses one ENDBLK.
void Dxf_parser::parse_endblk()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing endblk" << std::endl;);

  while (true) {
    int code;
    import_code(code);
    if (0 == code) break;

    if (100 == code) {
      import_value(m_marker);
      continue;
    }

    SGAL::String str;
    import_value(str);
  }
}

//! \brief parses BLOCKS section
void Dxf_parser::parse_blocks()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing BLOCKS section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  Dxf_block* block(nullptr);
  do {
    SGAL::String str;
    import_value(str);
    if ("BLOCK" == str) {
      SGAL_assertion(! block);
      m_blocks.push_back(Dxf_block());
      block = &(m_blocks.back());
      parse_block(*block);
      continue;
    }

    if ("ENDBLK" == str) {
      SGAL_assertion(block);
      parse_endblk();
      block = nullptr;
      continue;
    }

    if ("ENDSEC" == str) return;

    SGAL_assertion(block);
    auto it = s_entities.find(str);
    if (it != s_entities.end()) {
      auto* entity = (this->*(it->second))();
      if (entity) block->m_entities.push_back(entity);
      continue;
    }

    //! EF: Do we need to check for user entities here?

    SGAL::String msg("Unrecognize entity inside block \"");
    msg += str + "\", at line " + std::to_string(m_line);
    SGAL_error_msg(msg.c_str());

  } while (true);
}

//! \brief parse ENTITIES section
void Dxf_parser::parse_entities()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing ENTITIES section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    SGAL::String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    auto it = s_entities.find(str);
    if (it != s_entities.end()) {
      auto* entity = (this->*(it->second))();
      if (entity) m_entities.push_back(entity);
      continue;
    }

    // Look for user defined entities that are defined in a CLASS block
    auto cit = std::find_if(m_classes.begin(), m_classes.end(),
                            [&](const Dxf_class& my_class)
                            {
                              return ((my_class.m_record_name == str) &&
                                      my_class.m_is_entity);
                            });
    if (cit != m_classes.end()) {
      auto* user_entity = new Dxf_user_entity;
      m_entities.push_back(user_entity);
      parse_record(*user_entity);
      continue;
    }

    SGAL::String msg("Unrecognize entity \"");
    msg += str + "\", at line " + std::to_string(m_line);
    SGAL_error_msg(msg.c_str());

  } while (true);
}

//! \brief parse OBJECTS section
void Dxf_parser::parse_objects()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing OBJECTS section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    SGAL::String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    auto it = s_objects.find(str);
    if (it != s_objects.end()) {
      (this->*(it->second))();
      continue;
    }

    // Look for user defined objects that are defined in a CLASS block
    auto cit = std::find_if(m_classes.begin(), m_classes.end(),
                            [&](const Dxf_class& my_class)
                            {
                              return ((my_class.m_record_name == str) &&
                                      ! my_class.m_is_entity);
                            });
    if (cit != m_classes.end()) {
      m_user_objects.resize(m_user_objects.size() + 1);
      auto& user_object = m_user_objects.back();
      parse_record(user_object);
      continue;
    }

    SGAL::String msg("Unrecognize object \"");
    msg += str + "\", at line " + std::to_string(m_line);
    SGAL_error_msg(msg.c_str());

  } while (true);
}

//! \brief parses the thumbnail image section.
void Dxf_parser::parse_thumbnailimage()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing THUMBNAILIMAGE section"
                              << std::endl;);
}

//! \brief reads a HEADER veriable.
void Dxf_parser::read_header_member()
{
  char c;
  (*m_is) >> c;
  SGAL_assertion(c == '$');
  std::string str;
  import_value(str);
  auto it = s_header_members.find(str);
  if (it == s_header_members.end()) {
    if (m_report_unrecognized_code) {
      std::string unrecognized_msg("Unrecognized header variable ");
      unrecognized_msg += str + ", at line " + std::to_string(m_line) + "!";
      SGAL_warning_msg(0, unrecognized_msg.c_str());
    }

    // Error recovery.
    // Consume the values until one of the following codes is read:
    //  (i) '0', which implies the end of the HEADER section, or,
    // (ii) '9', which implies the beginning if a new header variable.
    while (true) {
      int code;
      import_code(code);
      if ((0 == code) || (9 == code)) {
        export_code(code);
        break;
      }
      read_unrecognized(code);
    }
    return;
  }

  const auto& header_var = it->second;
  const auto& codes = header_var.m_codes;
  auto& handle = header_var.m_handle;

  auto dim = codes.size();
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 8)
                    std::cout << "Parsing header member dimension: "
                              << dim << std::endl;);
  if (1 == dim) {
    auto expected_code = codes.front();
    int code;
    import_code(code);
    if (code != expected_code) {
      // Error recovery
      read_unrecognized(code);
      return;
    }
    auto ct = code_type(code);
    SGAL_TRACE_CODE(m_trace_code,
                    if (get_verbose_level() >= 8)
                      std::cout << "Parsing header member code type: "
                                << s_code_type_names[ct] << std::endl;);
    switch (ct) {
     case STRING: assign_member<String_header>(handle, m_header); break;
     case DOUBLE: assign_member<Double_header>(handle, m_header); break;
     case INT8: assign_member<Int8_header>(handle, m_header); break;
     case INT16: assign_member<Int16_header>(handle, m_header); break;
     case INT32: assign_member<Int32_header>(handle, m_header); break;
     case UINT: assign_member<Uint_header>(handle, m_header); break;
     case BOOL: assign_member<Bool_header>(handle, m_header); break;
    }
    return;
  }

  SGAL_assertion((dim == 2) || (dim == 3));
  size_t i(0);
  for (auto expected_code : codes) {
    int code;
    import_code(code);
    if (code != expected_code) {
      // Error recovery
      read_unrecognized(code);
      continue;
    }
    auto ct = code_type(code);
    SGAL_TRACE_CODE(m_trace_code,
                    if (get_verbose_level() >= 8)
                      std::cout << "Parsing header member code type: "
                                << s_code_type_names[ct] << std::endl;);
    SGAL_assertion(DOUBLE == ct);
    if (dim == 2)
      (*m_is) >> (m_header.*(boost::get<Double_2d_header>(handle)))[i++];
    else (*m_is) >> (m_header.*(boost::get<Double_3d_header>(handle)))[i++];
    ++m_line;
  }
}

//! \brief reads a comment line.
void Dxf_parser::read_comment()
{
  std::string str;
  std::getline(*m_is, str);
}

//! \brief handles an unrecognized code.
void Dxf_parser::read_unrecognized(int code)
{
  SGAL::String str;
  bool bval;
  int ival;
  SGAL::Uint uval;
  double dval;
  SGAL::String msg("Unrecognized code ");
  msg += std::to_string(code);

  std::stringstream stream;

  switch (code_type(code)) {
   case STRING:
    import_value(str);
    msg += ", string value: " + str;
    break;

   case BOOL:
    import_value(bval);
    msg += ", Bool value: " + std::to_string(bval);
    break;

   case INT8:
   case INT16:
   case INT32:
    import_value(ival);
    msg += ", int value: " + std::to_string(ival);
    break;

   case UINT:
    import_value(uval);
    stream << std::hex << uval;
    msg += ", unsigned int value: 0x" + stream.str();
    break;

   case DOUBLE:
    import_value(dval);
    msg += ", double value: " + std::to_string(dval);
    break;

   default: SGAL_error();
  }

  if (m_report_unrecognized_code) {
    msg += ", at line " + std::to_string(m_line);
    SGAL_warning_msg(0, msg.c_str());
  }
}

//! \brief parses a 3dface entity.
Dxf_base_entity* Dxf_parser::parse_3dface_entity()
{ return parse_entity("3dface", new Dxf_3dface_entity); }

//! \brief parses a 3dsolid entity.
Dxf_base_entity* Dxf_parser::parse_3dsolid_entity()
{ return parse_entity("3dsolid", new Dxf_3dsolid_entity); }

//! \brief parses an acad_proxy entity.
Dxf_base_entity* Dxf_parser::parse_acad_proxy_entity()
{ return parse_entity("acad_proxy", new Dxf_acad_proxy_entity); }

//! \brief parses an arc entity.
Dxf_base_entity* Dxf_parser::parse_arc_entity()
{ return parse_entity("arc", new Dxf_arc_entity); }

//! \brief parses an arcalignedtext entity.
Dxf_base_entity* Dxf_parser::parse_arcalignedtext_entity()
{ return parse_entity("arcalignedtext", new Dxf_arcalignedtext_entity); }

//! \brief parses an attdef entity.
Dxf_base_entity* Dxf_parser::parse_attdef_entity()
{ return parse_entity("attdef", new Dxf_attdef_entity); }

//! \brief parses an attrib entity.
Dxf_base_entity* Dxf_parser::parse_attrib_entity()
{ return parse_entity("attrib", new Dxf_attrib_entity); }

//! \brief parses a body entity.
Dxf_base_entity* Dxf_parser::parse_body_entity()
{ return parse_entity("body", new Dxf_body_entity); }

//! \brief parses a circle entity.
Dxf_base_entity* Dxf_parser::parse_circle_entity()
{ return parse_entity("circle", new Dxf_circle_entity); }

//! \brief parses a dimension entity.
Dxf_base_entity* Dxf_parser::parse_dimension_entity()
{ return parse_entity("dimension", new Dxf_dimension_entity); }

//! \brief parses an ellipse entity.
Dxf_base_entity* Dxf_parser::parse_ellipse_entity()
{ return parse_entity("ellipse", new Dxf_ellipse_entity); }

//! \brief parses a hatch entity.
Dxf_base_entity* Dxf_parser::parse_hatch_entity()
{ return parse_entity("hatch", new Dxf_hatch_entity); }

//! \brief parses an image entity.
Dxf_base_entity* Dxf_parser::parse_image_entity()
{ return parse_entity("image", new Dxf_image_entity); }

//! \brief parses an insert entity.
Dxf_base_entity* Dxf_parser::parse_insert_entity()
{ return parse_entity("insert", new Dxf_insert_entity); }

//! \brief parses a leader entity.
Dxf_base_entity* Dxf_parser::parse_leader_entity()
{ return parse_entity("leader", new Dxf_leader_entity); }

//! \brief parses a line entity.
Dxf_base_entity* Dxf_parser::parse_line_entity()
{ return parse_entity("line", new Dxf_line_entity); }

//! \brief parses a lwpolyline entity.
Dxf_base_entity* Dxf_parser::parse_lwpolyline_entity()
{ return parse_entity("lwpolyline", new Dxf_lwpolyline_entity); }

//! \brief parses am mline entity.
Dxf_base_entity* Dxf_parser::parse_mline_entity()
{ return parse_entity("mline", new Dxf_mline_entity); }

//! \brief parses an mtext entity.
Dxf_base_entity* Dxf_parser::parse_mtext_entity()
{ return parse_entity("mtext", new Dxf_mtext_entity); }

//! \brief parses an oleframe entity.
Dxf_base_entity* Dxf_parser::parse_oleframe_entity()
{ return parse_entity("oleframe", new Dxf_oleframe_entity); }

//! \brief parses an ole2frame entity.
Dxf_base_entity* Dxf_parser::parse_ole2frame_entity()
{ return parse_entity("ole2frame", new Dxf_ole2frame_entity); }

//! \brief parses a point entity.
Dxf_base_entity* Dxf_parser::parse_point_entity()
{ return parse_entity("point", new Dxf_point_entity); }

//! \brief parses a polyline entity.
Dxf_base_entity* Dxf_parser::parse_polyline_entity()
{
  auto* polyline = new Dxf_polyline_entity;
  parse_entity("polyline", polyline);
  m_active_polyline_entity = polyline;
  return polyline;
}

//! \brief parses a ray entity.
Dxf_base_entity* Dxf_parser::parse_ray_entity()
{ return parse_entity("ray", new Dxf_ray_entity); }

//! \brief parses a region entity.
Dxf_base_entity* Dxf_parser::parse_region_entity()
{ return parse_entity("region", new Dxf_region_entity); }

//! \brief parses an rtext entity.
Dxf_base_entity* Dxf_parser::parse_rtext_entity()
{ return parse_entity("rtext", new Dxf_rtext_entity); }

//! \brief parses a seqend entity.
Dxf_base_entity* Dxf_parser::parse_seqend_entity()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing SEQEND entity" << std::endl;);
  Dxf_seqend_entity seqend;
  parse_record(seqend);
  m_active_polyline_entity = nullptr;
  return nullptr;
}

//! \brief parses a shape entity.
Dxf_base_entity* Dxf_parser::parse_shape_entity()
{ return parse_entity("shape", new Dxf_shape_entity); }

//! \brief parses a solid entity.
Dxf_base_entity* Dxf_parser::parse_solid_entity()
{ return parse_entity("solid", new Dxf_solid_entity); }

//! \brief parses a spline entity.
Dxf_base_entity* Dxf_parser::parse_spline_entity()
{ return parse_entity("spline", new Dxf_spline_entity); }

//! \brief parses a text entity.
Dxf_base_entity* Dxf_parser::parse_text_entity()
{ return parse_entity("text", new Dxf_text_entity); }

//! \brief parses a tolerance entity.
Dxf_base_entity* Dxf_parser::parse_tolerance_entity()
{ return parse_entity("tolerance", new Dxf_tolerance_entity); }

//! \brief parses a trace entity.
Dxf_base_entity* Dxf_parser::parse_trace_entity()
{ return parse_entity("trace", new Dxf_trace_entity); }

//! \brief parses a vertex entity.
Dxf_base_entity* Dxf_parser::parse_vertex_entity()
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing VERTEX entity" << std::endl;);

  SGAL_assertion(m_active_polyline_entity);
  auto& vertices = m_active_polyline_entity->m_vertex_entities;
  vertices.resize(vertices.size() + 1);
  auto& vertex = vertices.back();
  parse_record(vertex);
  return nullptr;
}

//! \brief parses a viewport entity.
Dxf_base_entity* Dxf_parser::parse_viewport_entity()
{ return parse_entity("viewport", new Dxf_viewport_entity); }

//! \brief parses a wipeout entity.
Dxf_base_entity* Dxf_parser::parse_wipeout_entity()
{ return parse_entity("wipeout", new Dxf_wipeout_entity); }

//! \brief parses an xline entity.
Dxf_base_entity* Dxf_parser::parse_xline_entity()
{ return parse_entity("xline", new Dxf_xline_entity); }

// Object parsers
//! \brief parses a object.
void Dxf_parser::parse_acad_proxy_object()
{ parse_record(m_acad_proxy_object); }

//! \brief parses a object.
void Dxf_parser::parse_acdbdictionarywdflt_object()
{ parse_record(m_acdbdictionarywdflt_object); }

//! \brief parses a object.
void Dxf_parser::parse_acdbplaceholder_object()
{ parse_record(m_acdbplaceholder_object); }

//! \brief parses a object.
void Dxf_parser::parse_acdbnavisworksmodeldef_object()
{ parse_record(m_acdbnavisworksmodeldef_object); }

//! \brief parses a object.
void Dxf_parser::parse_datatable_object()
{ parse_record(m_datatable_object); }

//! \brief parses a object.
void Dxf_parser::parse_dictionary_object()
{ parse_record(m_dictionary_object); }

//! \brief parses a object.
void Dxf_parser::parse_dictionaryvar_object()
{ parse_record(m_dictionaryvar_object); }

//! \brief parses a object.
void Dxf_parser::parse_dimassoc_object()
{ parse_record(m_dimassoc_object); }

//! \brief parses a object.
void Dxf_parser::parse_field_object()
{ parse_record(m_field_object); }

//! \brief parses a object.
void Dxf_parser::parse_geodata_object()
{ parse_record(m_geodata_object); }

//! \brief parses a object.
void Dxf_parser::parse_group_object()
{ parse_record(m_group_object); }

//! \brief parses a object.
void Dxf_parser::parse_idbuffer_object()
{ parse_record(m_idbuffer_object); }

//! \brief parses a object.
void Dxf_parser::parse_imagedef_object()
{ parse_record(m_imagedef_object); }

//! \brief parses a object.
void Dxf_parser::parse_imagedef_reactor_object()
{ parse_record(m_imagedef_reactor_object); }

//! \brief parses a object.
void Dxf_parser::parse_layer_index_object()
{ parse_record(m_layer_index_object); }

//! \brief parses a object.
void Dxf_parser::parse_layer_filter_object()
{ parse_record(m_layer_filter_object); }

//! \brief parses a object.
void Dxf_parser::parse_layout_object()
{ parse_record(m_layout_object); }

//! \brief parses a object.
void Dxf_parser::parse_lightlist_object()
{ parse_record(m_lightlist_object); }

//! \brief parses a object.
void Dxf_parser::parse_material_object()
{
  m_material_objects.resize(m_material_objects.size() + 1);
  auto& material_object = m_material_objects.back();
  parse_record(material_object);
}

//! \brief parses a object.
void Dxf_parser::parse_mlinestyle_object()
{ parse_record(m_mlinestyle_object); }

//! \brief parses a object.
void Dxf_parser::parse_object_ptr_object()
{ parse_record(m_object_ptr_object); }

//! \brief parses a object.
void Dxf_parser::parse_plotsettings_object()
{ parse_record(m_plotsettings_object); }

//! \brief parses a object.
void Dxf_parser::parse_rastervariables_object()
{ parse_record(m_rastervariables_object); }

//! \brief parses a object.
void Dxf_parser::parse_render_object()
{ parse_record(m_render_object); }

//! \brief parses a object.
void Dxf_parser::parse_section_object()
{ parse_record(m_section_object); }

//! \brief parses a object.
void Dxf_parser::parse_spatial_index_object()
{ parse_record(m_spatial_index_object); }

//! \brief parses a object.
void Dxf_parser::parse_spatial_filter_object()
{ parse_record(m_spatial_filter_object); }

//! \brief parses a object.
void Dxf_parser::parse_sortentstable_object()
{ parse_record(m_sortentstable_object); }

//! \brief parses a object.
void Dxf_parser::parse_sunstudy_object()
{ parse_record(m_sunstudy_object); }

//! \brief parses a object.
void Dxf_parser::parse_tablestyle_object()
{ parse_record(m_tablestyle_object); }

//! \brief parses a object.
void Dxf_parser::parse_underlaydefinition_object()
{ parse_record(m_underlaydefinition_object); }

//! \brief parses a object.
void Dxf_parser::parse_visualstyle_object()
{ parse_record(m_visualstyle_object); }

//! \brief parses a object.
void Dxf_parser::parse_vba_project_object()
{ parse_record(m_vba_project_object); }

//! \brief parses a object.
void Dxf_parser::parse_wipeoutvariables_object()
{ parse_record(m_wipeoutvariables_object); }

//! \brief parses a object.
void Dxf_parser::parse_xrecord_object()
{ parse_record(m_xrecord_object); }

//! \brief parses a regular boundary path.
void Dxf_parser::parse_boundary_path(Dxf_boundary_path& path)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing boundary_path" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_base_boundary_path>
      Base_boundary_path_wrapper;
    auto& base_members = Base_boundary_path_wrapper::s_record_members;
    Dxf_base_boundary_path& base_path = path;
    if (assign_record_value(code, base_path, base_members)) continue;
    auto& base_handlers = Base_boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, base_path, base_handlers)) continue;

    // Handle codes of the (non-polyline) boundary-path record:
    typedef Dxf_record_wrapper<Dxf_boundary_path>       Boundary_path_wrapper;
    auto& members = Boundary_path_wrapper::s_record_members;
    if (assign_record_value(code, path, members)) continue;
    auto& handlers = Boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, path, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a polyline boundary path.
void Dxf_parser::parse_polyline_boundary_path(Dxf_polyline_boundary_path& path)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing polyline_boundary_path" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_base_boundary_path>
      Base_boundary_path_wrapper;
    auto& base_members = Base_boundary_path_wrapper::s_record_members;
    Dxf_base_boundary_path& base_path = path;
    if (assign_record_value(code, base_path, base_members)) continue;
    auto& base_handlers = Base_boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, base_path, base_handlers)) continue;

    // Handle codes of the polyline boundary-path record:
    typedef Dxf_record_wrapper<Dxf_polyline_boundary_path>
      Polyline_boundary_path_wrapper;
    auto& members = Polyline_boundary_path_wrapper::s_record_members;
    if (assign_record_value(code, path, members)) continue;
    auto& handlers = Polyline_boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, path, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a pattern data.
void Dxf_parser::parse_pattern_data(Dxf_pattern_data& pattern_data)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing pattern_data" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_pattern_data>       Pattern_data_wrapper;
    auto& members = Pattern_data_wrapper::s_record_members;
    if (assign_record_value(code, pattern_data, members)) continue;
    auto& handlers = Pattern_data_wrapper::s_record_handlers;
    if (handle_record_value(code, pattern_data, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a line edge
void Dxf_parser::parse_line_edge(Dxf_line_edge& edge)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing line_edge" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_line_edge>   Line_edge_wrapper;
    auto& members = Line_edge_wrapper::s_record_members;
    if (assign_record_value(code, edge, members)) continue;
    auto& handlers = Line_edge_wrapper::s_record_handlers;
    if (handle_record_value(code, edge, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a circle edge
void Dxf_parser::parse_circle_edge(Dxf_circle_edge& edge)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing circle_edge" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_circle_edge>   Circle_edge_wrapper;
    auto& members = Circle_edge_wrapper::s_record_members;
    if (assign_record_value(code, edge, members)) continue;
    auto& handlers = Circle_edge_wrapper::s_record_handlers;
    if (handle_record_value(code, edge, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses an ellipse edge
void Dxf_parser::parse_ellipse_edge(Dxf_ellipse_edge& edge)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing ellipse_edge" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_ellipse_edge>   Ellipse_edge_wrapper;
    auto& members = Ellipse_edge_wrapper::s_record_members;
    if (assign_record_value(code, edge, members)) continue;
    auto& handlers = Ellipse_edge_wrapper::s_record_handlers;
    if (handle_record_value(code, edge, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a spellipse edge
void Dxf_parser::parse_spline_edge(Dxf_spline_edge& edge)
{
  SGAL_TRACE_CODE(m_trace_code,
                  if (get_verbose_level() >= 5)
                    std::cout << "Parsing spline_edge" << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_spline_edge>   Spline_edge_wrapper;
    auto& members = Spline_edge_wrapper::s_record_members;
    if (assign_record_value(code, edge, members)) continue;
    auto& handlers = Spline_edge_wrapper::s_record_handlers;
    if (handle_record_value(code, edge, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief clears the parser. Deallocate data structure and prepare for reuse.
// At this point only the hatch entity and the material objects are stored in
// containers, which must be cleared.
void Dxf_parser::clear()
{
  // Clear tables
  m_appid_table.clear();
  m_block_record_table.clear();
  m_dimstyle_table.clear();
  m_layer_table.clear();
  m_ltype_table.clear();
  m_style_table.clear();
  m_ucs_table.clear();
  m_view_table.clear();
  m_vport_table.clear();

  // Clear classes
  m_classes.clear();

  // Clear blocks
  m_blocks.clear();

  // Clear entities

  for (auto* entity : m_entities) delete entity;
  m_entities.clear();

  // Clear objects
  m_material_objects.clear();
  m_color_arrays.clear();
  m_pattern_appearances.clear();
}

DXF_END_NAMESPACE
