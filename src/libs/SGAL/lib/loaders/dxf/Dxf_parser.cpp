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

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Dxf_parser.hpp"
#include "SGAL/Dxf_simple_record_wrapper.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_boundary_path.hpp"
#include "SGAL/Dxf_polyline_boundary_path.hpp"
#include "SGAL/Dxf_pattern_data.hpp"

SGAL_BEGIN_NAMESPACE

const std::array<String, 8> Dxf_parser::s_code_type_names = {
  "STRING", "DOUBLE", "INT8", "INT16", "INT32", "UINT", "BOOL"
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
  {110, 112, DOUBLE},   // x-value of UCS
  {120, 122, DOUBLE},   // y-value of UCS
  {130, 132, DOUBLE},   // z-value of UCS
  {140, 149, DOUBLE},   // Double precision scalar floating-point value
  {170, 179, INT16},    // 16-bit integer value
  {210, 239, DOUBLE},   // x-value of extrusion direction
  {270, 279, INT16},    // 16-bit integer value
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
  {1010, 1059, DOUBLE}, // Double-precision floating-point value
  {1060, 1070, INT16},  // 16-bit integer value
  {1071, 1071, INT32}   // 32-bit integer value
};

//!
const std::map<String, Dxf_parser::Section_parser> Dxf_parser::s_sections = {
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
typedef Dxf_record_wrapper<Dxf_appid_entry>     Dxf_appid_wrapper;
template <>
const std::map<int, Dxf_appid_wrapper::Record_member>
Dxf_appid_wrapper::s_record_members = {
  {2, {&Dxf_appid_entry::m_name, 1, 0}},
  {70, {&Dxf_appid_entry::m_flags, 1, 0}}
};

//!
typedef Dxf_record_wrapper<Dxf_ucs_entry>               Dxf_ucs_wrapper;
template <>
const std::map<int, Dxf_ucs_wrapper::Record_member>
Dxf_ucs_wrapper::s_record_members = {
};

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
const std::map<String, Dxf_parser::Entity_parser>
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

//!
typedef Dxf_simple_record_wrapper<Dxf_base_entity>      Base_entity_wrapper;
template <>
const std::map<int, Base_entity_wrapper::Record_member_type>
Base_entity_wrapper::s_record_members = {
  {5, &Dxf_base_entity::m_handle},
  {330, &Dxf_base_entity::m_owner_handle},
  {360, &Dxf_base_entity::m_owner_dict},
  // {330, &Dxf_base_entity::m_owner_block}, reuse of same code ???
  {67, &Dxf_base_entity::m_is_in_paper_space},
  {410, &Dxf_base_entity::m_layout_tab_name},
  {8, &Dxf_base_entity::m_layer},
  {6, &Dxf_base_entity::m_line_type_name},
  {62, &Dxf_base_entity::m_color},
  {370, &Dxf_base_entity::m_lineweight_enum_value},
  {48, &Dxf_base_entity::m_line_type_scale},
  {60, &Dxf_base_entity::m_is_visible},
  {92, &Dxf_base_entity::m_image_byte_count},
   // {310, &Dxf_base_entity::m_preview_image_data},
  {420, &Dxf_base_entity::m_color_24_bit},
  {430, &Dxf_base_entity::m_color_name},
  {440, &Dxf_base_entity::m_transparency},
  {390, &Dxf_base_entity::m_plot_style},
  {284, &Dxf_base_entity::m_shadow_mode}
};

//! Object parsers
const std::map<String, Dxf_parser::Object_parser>
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
Dxf_parser::Dxf_parser(std::istream& is, Scene_graph* sg,
                       const String& filename) :
  Dxf_base_parser(is, filename),
  m_pending_code(0),
  m_is_pending(false),
  m_scene_graph(sg),
  m_extended_data(nullptr)
{}

//! \brief parses.
Loader_code Dxf_parser::operator()()
{
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
    std::string unrecognized_msg("unrecognized code range ");
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
    import_code(n);
    switch (n) {
     case 0: done = true; break;
     case 9: read_header_member(); break;
     case 999: read_comment(); break;
    }
  }
  String str;
  import_value(str);
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
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_classes()" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    SGAL_assertion("CLASS" == str);
    parse_class();
  } while (true);
}

//! \brief parses a table of type APPID.
void Dxf_parser::parse_appid_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_appid_table()"
                  << std::endl;);
  parse_table(m_appid_table, "APPID");
}

//! \brief parses a table of type BLOCK_RECORD.
void Dxf_parser::parse_block_record_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_block_record_table()"
                  << std::endl;);
  parse_table(m_block_record_table, "BLOCK_RECORD");
}

//! \brief parses a table of type DIMSTYLE.
void Dxf_parser::parse_dimstyle_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_dimstyle_table()"
                  << std::endl;);
  parse_table(m_dimstyle_table, "DIMSTYLE");
}

//! \brief parses a table of type LAYER.
void Dxf_parser::parse_layer_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_layer_table()"
                  << std::endl;);
  parse_table(m_layer_table, "LAYER");
}

//! \brief parses a table of type LTYPE.
void Dxf_parser::parse_ltype_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_ltype_table()"
                  << std::endl;);
  parse_table(m_ltype_table, "LTYPE");
}

//! \brief parses a table of type STYLE.
void Dxf_parser::parse_style_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_style_table()"
                  << std::endl;);
  parse_table(m_style_table, "STYLE");
}

//! \brief parses a table of type UCS.
void Dxf_parser::parse_ucs_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_ucs_table()"
                  << std::endl;);
  parse_table(m_ucs_table, "UCS");
}

//! \brief parses a table of type VIEW.
void Dxf_parser::parse_view_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_view_table()"
                  << std::endl;);
  parse_table(m_view_table, "VIEW");
}

//! \brief parses a table of type VPORT.
void Dxf_parser::parse_vport_table()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_vport_table()"
                  << std::endl;);
  parse_table(m_vport_table, "VPORT");
}

//! \brief parses the TABLES section.
void Dxf_parser::parse_tables()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_tables()" << std::endl;);

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
void Dxf_parser::parse_block()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_block()" << std::endl;);

  m_blocks.push_back(Dxf_block());
  auto& block = m_blocks.back();

  bool done(false);
  while (!done) {
    int code;
    import_code(code);
    if (0 == code) {
      done = true;
      break;
    }

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
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_endblk()" << std::endl;);

  bool done(false);
  while (!done) {
    int code;
    import_code(code);
    if (0 == code) {
      done = true;
      break;
    }

    if (100 == code) {
      import_value(m_marker);
      continue;
    }

    String str;
    import_value(str);
  }
}

//! \brief parses BLOCKS section
void Dxf_parser::parse_blocks()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_blocks()" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    if ("BLOCK" == str) parse_block();
    else if ("ENDBLK" == str) parse_endblk();
  } while (true);
}

//! \brief parse ENTITIES section
void Dxf_parser::parse_entities()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_entities()" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    String str;
    import_value(str);
    if ("ENDSEC" == str) return;

    auto it = s_entities.find(str);
    if (it == s_entities.end()) {
      SGAL_error_msg("unrecognize entity");
    }
    (this->*(it->second))();

  } while (true);
}

//! \brief parse OBJECTS section
void Dxf_parser::parse_objects()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_objects()" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  do {
    String str;
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

    String msg("Unrecognize object \"");
    msg += str + "\", at line " + std::to_string(m_line);
    SGAL_error_msg(msg.c_str());

  } while (true);
}

//! \brief parses the thumbnail image section.
void Dxf_parser::parse_thumbnailimage()
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_thumbnailimage()"
                  << std::endl;);
}

/*! \brief reads a value from the input string and verifies that it matches a
 * given code.
 */
Dxf_parser::Code_type Dxf_parser::read_verify_code(int expected)
{
  int code;
  import_code(code);
  SGAL_assertion(code == expected);
  return code_type(code);
}

//! \brief reads a HEADER veriable.
void Dxf_parser::read_header_member()
{
  char c;
  m_is >> c;
  SGAL_assertion(c == '$');
  std::string str;
  import_value(str);
  auto it = s_header_members.find(str);
  if (it == s_header_members.end()) {
    std::string unrecognized_msg("unrecognized header variable ");
    unrecognized_msg += str + "!";
    SGAL_error_msg(unrecognized_msg.c_str());
  }

  const auto& header_var = it->second;
  const auto& codes = header_var.m_codes;
  auto& handle = header_var.m_handle;

  auto dim = codes.size();
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::read_header_member() dimension: "
                  << dim << std::endl;);
  if (1 == dim) {
    auto code = codes.front();
    auto code_type = read_verify_code(code);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::read_header_member() code type: "
                    << s_code_type_names[code_type] << std::endl;);
    switch (code_type) {
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
  for (auto code : codes) {
    auto code_type = read_verify_code(code);
    SGAL_TRACE_CODE(Trace::DXF,
                    std::cout << "Dxf_parser::read_header_member() code type: "
                    << s_code_type_names[code_type] << std::endl;);
    SGAL_assertion(DOUBLE == code_type);
    if (dim == 2)
      m_is >> (m_header.*(boost::get<Double_2d_header>(handle)))[i++];
    else m_is >> (m_header.*(boost::get<Double_3d_header>(handle)))[i++];
    ++m_line;
  }
}

//! \brief reads a comment line.
void Dxf_parser::read_comment()
{
  std::string str;
  std::getline(m_is, str);
}

//! \brief handles an unrecognized code.
void Dxf_parser::read_unrecognized(int code)
{
  String str;
  bool bval;
  int ival;
  Uint uval;
  double dval;
  String msg("Unrecognized code ");
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
  msg += ", at line " + std::to_string(m_line);
  SGAL_warning_msg(0, msg.c_str());
}

//! \brief parses a 3dface entity.
void Dxf_parser::parse_3dface_entity()
{ parse_record(m_3dface_entity); }

//! \brief parses a 3dsolid entity.
void Dxf_parser::parse_3dsolid_entity()
{ parse_record(m_3dsolid_entity); }

//! \brief parses an acad_proxy entity.
void Dxf_parser::parse_acad_proxy_entity()
{ parse_record(m_acad_proxy_entity); }

//! \brief parses an arc entity.
void Dxf_parser::parse_arc_entity()
{ parse_record(m_arc_entity); }

//! \brief parses an arcalignedtext entity.
void Dxf_parser::parse_arcalignedtext_entity()
{ parse_record(m_arcalignedtext_entity); }

//! \brief parses an attdef entity.
void Dxf_parser::parse_attdef_entity()
{ parse_record(m_attdef_entity); }

//! \brief parses an attrib entity.
void Dxf_parser::parse_attrib_entity()
{ parse_record(m_attrib_entity); }

//! \brief parses a body entity.
void Dxf_parser::parse_body_entity()
{ parse_record(m_body_entity); }

//! \brief parses a circle entity.
void Dxf_parser::parse_circle_entity()
{ parse_record(m_circle_entity); }

//! \brief parses a dimension entity.
void Dxf_parser::parse_dimension_entity()
{ parse_record(m_dimension_entity); }

//! \brief parses an ellipse entity.
void Dxf_parser::parse_ellipse_entity()
{ parse_record(m_ellipse_entity); }

//! \brief parses a hatch entity.
void Dxf_parser::parse_hatch_entity()
{ parse_record(m_hatch_entity); }

//! \brief parses an image entity.
void Dxf_parser::parse_image_entity()
{ parse_record(m_image_entity); }

//! \brief parses an insert entity.
void Dxf_parser::parse_insert_entity()
{ parse_record(m_insert_entity); }

//! \brief parses a leader entity.
void Dxf_parser::parse_leader_entity()
{ parse_record(m_leader_entity); }

//! \brief parses a line entity.
void Dxf_parser::parse_line_entity()
{ parse_record(m_line_entity); }

//! \brief parses a lwpolyline entity.
void Dxf_parser::parse_lwpolyline_entity()
{ parse_record(m_lwpolyline_entity); }

//! \brief parses am mline entity.
void Dxf_parser::parse_mline_entity()
{ parse_record(m_mline_entity); }

//! \brief parses an mtext entity.
void Dxf_parser::parse_mtext_entity()
{ parse_record(m_mtext_entity); }

//! \brief parses an oleframe entity.
void Dxf_parser::parse_oleframe_entity()
{ parse_record(m_oleframe_entity); }

//! \brief parses an ole2frame entity.
void Dxf_parser::parse_ole2frame_entity()
{ parse_record(m_ole2frame_entity); }

//! \brief parses a point entity.
void Dxf_parser::parse_point_entity()
{ parse_record(m_point_entity); }

//! \brief parses a polyline entity.
void Dxf_parser::parse_polyline_entity()
{ parse_record(m_polyline_entity); }

//! \brief parses a ray entity.
void Dxf_parser::parse_ray_entity()
{ parse_record(m_ray_entity); }

//! \brief parses a region entity.
void Dxf_parser::parse_region_entity()
{ parse_record(m_region_entity); }

//! \brief parses an rtext entity.
void Dxf_parser::parse_rtext_entity()
{ parse_record(m_rtext_entity); }

//! \brief parses a seqend entity.
void Dxf_parser::parse_seqend_entity()
{ parse_record(m_seqend_entity); }

//! \brief parses a shape entity.
void Dxf_parser::parse_shape_entity()
{ parse_record(m_shape_entity); }

//! \brief parses a solid entity.
void Dxf_parser::parse_solid_entity()
{ parse_record(m_solid_entity); }

//! \brief parses a spline entity.
void Dxf_parser::parse_spline_entity()
{ parse_record(m_spline_entity); }

//! \brief parses a text entity.
void Dxf_parser::parse_text_entity()
{ parse_record(m_text_entity); }

//! \brief parses a tolerance entity.
void Dxf_parser::parse_tolerance_entity()
{ parse_record(m_tolerance_entity); }

//! \brief parses a trace entity.
void Dxf_parser::parse_trace_entity()
{ parse_record(m_trace_entity); }

//! \brief parses a vertex entity.
void Dxf_parser::parse_vertex_entity()
{ parse_record(m_vertex_entity); }

//! \brief parses a viewport entity.
void Dxf_parser::parse_viewport_entity()
{ parse_record(m_viewport_entity); }

//! \brief parses a wipeout entity.
void Dxf_parser::parse_wipeout_entity()
{ parse_record(m_wipeout_entity); }

//! \brief parses an xline entity.
void Dxf_parser::parse_xline_entity()
{ parse_record(m_xline_entity); }

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
{ parse_record(m_material_object); }

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
  while (true) {
    int code;
    import_code(code);
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
  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_boundary_path>       Boundary_path_wrapper;
    auto& members = Boundary_path_wrapper ::s_record_members;
    Dxf_boundary_path& base_path = path;
    if (assign_record_value(code, base_path, members)) continue;
    auto& handlers = Boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, base_path, handlers)) continue;

    // Handle codes of the polyline boundary-path record:
    typedef Dxf_record_wrapper<Dxf_polyline_boundary_path>
      Polyline_boundary_path_wrapper;
    auto& pl_members = Polyline_boundary_path_wrapper::s_record_members;
    if (assign_record_value(code, path, pl_members)) continue;
    auto& pl_handlers = Polyline_boundary_path_wrapper::s_record_handlers;
    if (handle_record_value(code, path, pl_handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

//! \brief parses a pattern data.
void Dxf_parser::parse_pattern_data(Dxf_pattern_data& pattern_data)
{
  SGAL_TRACE_CODE(Trace::DXF,
                  std::cout << "Dxf_parser::parse_pattern_data()"
                  << std::endl;);

  while (true) {
    int code;
    import_code(code);

    // Handle codes of the base boundary-path record:
    typedef Dxf_record_wrapper<Dxf_pattern_data>       Pattern_data_wrapper;
    auto& members = Pattern_data_wrapper ::s_record_members;
    if (assign_record_value(code, pattern_data, members)) continue;
    auto& handlers = Pattern_data_wrapper::s_record_handlers;
    if (handle_record_value(code, pattern_data, handlers)) continue;

    // The code hasn't been processed yet, export it.
    export_code(code);
    return;
  }
}

SGAL_END_NAMESPACE
