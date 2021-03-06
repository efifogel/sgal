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

#include <string>
#include <iostream>
#include <functional>
#include <streambuf>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"
#include "SGAL/Geometry_format.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_header_wrapper.hpp"
#include "dxf/Dxf_header_variable.hpp"
#include "dxf/Dxf_base_boundary_path.hpp"
#include "dxf/Dxf_boundary_path.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"
#include "dxf/Dxf_pattern_data.hpp"
#include "dxf/Dxf_line_edge.hpp"
#include "dxf/Dxf_circle_edge.hpp"
#include "dxf/Dxf_ellipse_edge.hpp"
#include "dxf/Dxf_spline_edge.hpp"
#include "dxf/Dxf_base_entity.hpp"
#include "dxf/Dxf_base_object.hpp"
#include "dxf/Dxf_section.hpp"
#include "dxf/Dxf_builder.hpp"

// Entities
#include "dxf/Dxf_3dface_entity.hpp"
#include "dxf/Dxf_3dsolid_entity.hpp"
#include "dxf/Dxf_acad_proxy_entity.hpp"
#include "dxf/Dxf_arc_entity.hpp"
#include "dxf/Dxf_arcalignedtext_entity.hpp"
#include "dxf/Dxf_attdef_entity.hpp"
#include "dxf/Dxf_attrib_entity.hpp"
#include "dxf/Dxf_body_entity.hpp"
#include "dxf/Dxf_circle_entity.hpp"
#include "dxf/Dxf_dimension_entity.hpp"
#include "dxf/Dxf_ellipse_entity.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_image_entity.hpp"
#include "dxf/Dxf_insert_entity.hpp"
#include "dxf/Dxf_leader_entity.hpp"
#include "dxf/Dxf_line_entity.hpp"
#include "dxf/Dxf_lwpolyline_entity.hpp"
#include "dxf/Dxf_mline_entity.hpp"
#include "dxf/Dxf_mtext_entity.hpp"
#include "dxf/Dxf_oleframe_entity.hpp"
#include "dxf/Dxf_ole2frame_entity.hpp"
#include "dxf/Dxf_point_entity.hpp"
#include "dxf/Dxf_polyline_entity.hpp"
#include "dxf/Dxf_ray_entity.hpp"
#include "dxf/Dxf_region_entity.hpp"
#include "dxf/Dxf_rtext_entity.hpp"
#include "dxf/Dxf_seqend_entity.hpp"
#include "dxf/Dxf_shape_entity.hpp"
#include "dxf/Dxf_solid_entity.hpp"
#include "dxf/Dxf_spline_entity.hpp"
#include "dxf/Dxf_text_entity.hpp"
#include "dxf/Dxf_tolerance_entity.hpp"
#include "dxf/Dxf_trace_entity.hpp"
#include "dxf/Dxf_vertex_entity.hpp"
#include "dxf/Dxf_viewport_entity.hpp"
#include "dxf/Dxf_wipeout_entity.hpp"
#include "dxf/Dxf_xline_entity.hpp"
#include "dxf/Dxf_user_entity.hpp"

// Objects
#include "dxf/Dxf_acad_proxy_object.hpp"
#include "dxf/Dxf_acdbdictionarywdflt_object.hpp"
#include "dxf/Dxf_acdbnavisworksmodeldef_object.hpp"
#include "dxf/Dxf_acdbplaceholder_object.hpp"
#include "dxf/Dxf_datatable_object.hpp"
#include "dxf/Dxf_dictionary_object.hpp"
#include "dxf/Dxf_dictionaryvar_object.hpp"
#include "dxf/Dxf_dimassoc_object.hpp"
#include "dxf/Dxf_field_object.hpp"
#include "dxf/Dxf_geodata_object.hpp"
#include "dxf/Dxf_group_object.hpp"
#include "dxf/Dxf_idbuffer_object.hpp"
#include "dxf/Dxf_imagedef_object.hpp"
#include "dxf/Dxf_imagedef_reactor_object.hpp"
#include "dxf/Dxf_layer_filter_object.hpp"
#include "dxf/Dxf_layer_index_object.hpp"
#include "dxf/Dxf_layout_object.hpp"
#include "dxf/Dxf_lightlist_object.hpp"
#include "dxf/Dxf_material_object.hpp"
#include "dxf/Dxf_mlinestyle_object.hpp"
#include "dxf/Dxf_object_ptr_object.hpp"
#include "dxf/Dxf_plotsettings_object.hpp"
#include "dxf/Dxf_rastervariables_object.hpp"
#include "dxf/Dxf_render_object.hpp"
#include "dxf/Dxf_section_object.hpp"
#include "dxf/Dxf_sortentstable_object.hpp"
#include "dxf/Dxf_spatial_filter_object.hpp"
#include "dxf/Dxf_spatial_index_object.hpp"
#include "dxf/Dxf_sunstudy_object.hpp"
#include "dxf/Dxf_tablestyle_object.hpp"
#include "dxf/Dxf_underlaydefinition_object.hpp"
#include "dxf/Dxf_vba_project_object.hpp"
#include "dxf/Dxf_visualstyle_object.hpp"
#include "dxf/Dxf_wipeoutvariables_object.hpp"
#include "dxf/Dxf_xrecord_object.hpp"
#include "dxf/Dxf_user_object.hpp"

DXF_BEGIN_NAMESPACE

//! Stores all the dxf data of all parser invocations.
std::map<SGAL::String, Dxf_data*> Dxf_parser::s_datas;

//!
const std::map<SGAL::String, Dxf_parser::Section_parser>
Dxf_parser::s_sections = {
  {"HEADER", &Dxf_parser::parse_header},
  {"CLASSES", &Dxf_parser::parse_classes},
  {"TABLES", &Dxf_parser::parse_tables},
  {"BLOCKS", &Dxf_parser::parse_blocks},
  {"ENTITIES", &Dxf_parser::parse_entities},
  {"OBJECTS", &Dxf_parser::parse_objects},
  {"THUMBNAILIMAGE", &Dxf_parser::parse_thumbnailimage},
  {"ACDSDATA", &Dxf_parser::parse_acdsdata},
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

//! \brief constructs.
Dxf_parser::Dxf_parser() :
  Base_loader(),
  m_trace_code_parsing(static_cast<size_t>(SGAL::Tracer::INVALID)),
  m_trace_code_building(static_cast<size_t>(SGAL::Tracer::INVALID)),
  m_data(nullptr),
  m_pending_code(0),
  m_is_pending(false),
  m_extended_data(nullptr),
  m_report_unrecognized_code(false),
  m_active_polyline_entity(nullptr)
{}

//! \brief destructs.
Dxf_parser::~Dxf_parser()
{
  for (auto& data : s_datas) delete data.second;
  s_datas.clear();
}

//! \brief parses.
SGAL::Loader_code Dxf_parser::operator()(std::istream& is,
                                         const SGAL::String& filename,
                                         SGAL::Scene_graph* scene_graph,
                                         SGAL::Group* root)
{
  m_data = new Dxf_data;

  Base_loader::operator()(is, filename, scene_graph, root);
  m_pending_code = 0;
  m_is_pending = false;

  //! \todo Make the Configuration a singleton and remove it from the
  // scene-graph. Then, move the following code exerpt to dxf_init().
  auto* conf = scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);

  while (true) {
    int n;
    import_code(n);
    if (999 == n) {
      read_comment();
      continue;
    }
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
    if (sec_it == s_sections.end()) SGAL_error_msg("Unrecognize section");
    (this->*(sec_it->second))();
  }

  Dxf_builder builder(*m_data, scene_graph, get_trace_code_building());
  builder(root);

  // Store the data for future use:
  if (dxf_conf->get_store_data()) s_datas[filename] = m_data;
  else delete m_data;
  m_data = nullptr;

  return SGAL::Loader_code::SUCCESS;
}

//! \brief parses the header section.
void Dxf_parser::parse_header()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing HEADER section" << std::endl;);

  while (true) {
    int n;
    import_code(n);
    if (0 == n) break;
    if (9 == n) read_header_member(); continue;
    if (999 == n) read_comment(); continue;
  }
  SGAL::String str;
  import_value(str);
  SGAL_assertion("ENDSEC" == str);
}

//! \brief parses one class.
void Dxf_parser::parse_class()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse class" << std::endl;);

  m_data->m_classes.emplace_back();
  auto& dxf_class = m_data->m_classes.back();

  while (true) {
    int code;
    import_code(code);
    if (0 == code) break;

    auto it = s_class_members.find(code);
    SGAL_assertion(it != s_class_members.end());
    auto handle = it->second;
    switch (code_type(code)) {
     case Code_type::STRING: assign_member<String_class>(handle, dxf_class); break;
     case Code_type::INT8: assign_member<Int8_class>(handle, dxf_class); break;
     case Code_type::INT32: assign_member<Int32_class>(handle, dxf_class); break;
     default: SGAL_error();
    }
  }
}

//! \brief parses the CLASSES section.
void Dxf_parser::parse_classes()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse APPID table" << std::endl;);

  parse_table(m_data->m_appid_table, "APPID");
}

//! \brief parses a table of type BLOCK_RECORD.
void Dxf_parser::parse_block_record_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parse BLOCK_RECORD table" << std::endl;);

  parse_table(m_data->m_block_record_table, "BLOCK_RECORD");
}

//! \brief parses a table of type DIMSTYLE.
void Dxf_parser::parse_dimstyle_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing DIMSTYLE table" << std::endl;);

  parse_table(m_data->m_dimstyle_table, "DIMSTYLE");
}

//! \brief parses a table of type LAYER.
void Dxf_parser::parse_layer_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing LAYER table" << std::endl;);

  parse_table(m_data->m_layer_table, "LAYER");
}

//! \brief parses a table of type LTYPE.
void Dxf_parser::parse_ltype_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing LTYPE table" << std::endl;);

  parse_table(m_data->m_ltype_table, "LTYPE");
}

//! \brief parses a table of type STYLE.
void Dxf_parser::parse_style_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing STYLE table" << std::endl;);

  parse_table(m_data->m_style_table, "STYLE");
}

//! \brief parses a table of type UCS.
void Dxf_parser::parse_ucs_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing UCS table" << std::endl;);

  parse_table(m_data->m_ucs_table, "UCS");
}

//! \brief parses a table of type VIEW.
void Dxf_parser::parse_view_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing VIEW table" << std::endl;);

  parse_table(m_data->m_view_table, "VIEW");
}

//! \brief parses a table of type VPORT.
void Dxf_parser::parse_vport_table()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 4)
                    std::cout << "Parsing VPORT table" << std::endl;);

  parse_table(m_data->m_vport_table, "VPORT");
}

//! \brief parses the TABLES section.
void Dxf_parser::parse_tables()
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
void Dxf_parser::parse_endblk(Dxf_endblk& endblk)
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing BLOCKS section" << std::endl;);

  int n;
  import_code(n);
  SGAL_assertion(0 == n);

  std::pair<Dxf_block, Dxf_endblk>* block(nullptr);
  do {
    SGAL::String str;
    import_value(str);
    if ("BLOCK" == str) {
      SGAL_assertion(! block);
      m_data->m_blocks.emplace_back();
      block = &(m_data->m_blocks.back());
      parse_block(block->first);
      continue;
    }

    if ("ENDBLK" == str) {
      SGAL_assertion(block);
      parse_endblk(block->second);
      block = nullptr;
      continue;
    }

    if ("ENDSEC" == str) return;

    SGAL_assertion(block);
    auto it = s_entities.find(str);
    if (it != s_entities.end()) {
      auto* entity = (this->*(it->second))();
      if (entity) block->first.m_entities.push_back(entity);
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
      if (entity) m_data->m_entities.push_back(entity);
      continue;
    }

    // Look for user defined entities that are defined in a CLASS block
    auto cit = std::find_if(m_data->m_classes.begin(), m_data->m_classes.end(),
                            [&](const Dxf_class& my_class)
                            {
                              return ((my_class.m_record_name == str) &&
                                      my_class.m_is_entity);
                            });
    if (cit != m_data->m_classes.end()) {
      auto* user_entity = new Dxf_user_entity;
      m_data->m_entities.push_back(user_entity);
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
    auto cit = std::find_if(m_data->m_classes.begin(), m_data->m_classes.end(),
                            [&](const Dxf_class& my_class)
                            {
                              return ((my_class.m_record_name == str) &&
                                      ! my_class.m_is_entity);
                            });
    if (cit != m_data->m_classes.end()) {
      auto* user_object = new Dxf_user_object;
      m_data->m_objects.push_back(user_object);
      parse_record(*user_object);
      continue;
    }

    SGAL::String msg("Unrecognize object \"");
    msg += str + "\", at line " + std::to_string(m_line);
    SGAL_error_msg(msg.c_str());

  } while (true);
}

//! \brief parse a generic section and stores the data.
void Dxf_parser::parse_section(const std::string& name, Dxf_section& section)
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 2)
                    std::cout << "Parsing " << name <<  " section"
                              << std::endl;);
  while (true) {
    int n;
    import_code(n);
    if (0 == n) break;

    SGAL::String str;
    import_value(str);

    section.m_items.emplace_back(n, str);
  }

  SGAL::String str;
  import_value(str);
  if ("ENDSEC" == str) return;

  auto* sub_section = new Dxf_section;
  section.m_sub_sections.emplace_back(str, sub_section);
  parse_section(str, *sub_section);
}

//! \brief parses the thumbnail image section.
void Dxf_parser::parse_thumbnailimage()
{ parse_section("THUMBNAILIMAGE", m_data->m_acdsdata); }

//! \brief parses the acdsdata section.
void Dxf_parser::parse_acdsdata()
{ parse_section("ACDSDATA", m_data->m_acdsdata); }

//! \brief reads a HEADER veriable.
void Dxf_parser::read_header_member()
{
  std::string str;
  import_value(str);
  SGAL_assertion(str[0] == '$');
  const auto& members = Dxf_header_wrapper::s_header_members;
  auto it = members.find(str);
  if (it == members.end()) {
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

  const auto& codes = it->second;
  auto& header = m_data->m_header;

  auto dim = codes.size();
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 8)
                    std::cout << "Parsing header member dimension: "
                              << dim << std::endl;);

  auto& vars = m_data->m_header.m_variables;

  std::string* str_p(nullptr);
  double* double_p;
  int8_t* int8_p;
  int16_t* int16_p;
  int32_t* int32_p;
  SGAL::Uint* uint_p;
  bool* bool_p;

  auto cit = codes.begin();
  auto expected_code = *cit++;
  int code;
  import_code(code);
  if (code != expected_code) {
    // Error recovery
    read_unrecognized(code);
    return;
  }
  auto ct = code_type(code);
  SGAL_TRACE_CODE(get_trace_code_parsing(),
                  if (get_verbose_level() >= 8)
                    std::cout << "Parsing header member code type: "
                              << code_type_name(ct) << std::endl;);

  switch (ct) {
   case Code_type::STRING:
    str_p = new std::string;
    import_value(str_p[0]);
    vars.emplace_back(str, str_p);
    break;

   case Code_type::DOUBLE:
    double_p = new double[dim];
    import_value(double_p[0]);
    vars.emplace_back(str, double_p);
    break;

   case Code_type::INT8:
    int8_p = new int8_t;
    import_value(int8_p[0]);
    vars.emplace_back(str, int8_p);
    break;

   case Code_type::INT16:
    int16_p = new int16_t;
    import_value(int16_p[0]);
    vars.emplace_back(str, int16_p);
    break;

   case Code_type::INT32:
    int32_p = new int32_t;
    import_value(int32_p[0]);
    vars.emplace_back(str, int32_p);
    break;

   case Code_type::UINT:
    uint_p = new SGAL::Uint;
    import_value(uint_p[0]);
    vars.emplace_back(str, uint_p);
    break;

   case Code_type::BOOL:
    bool_p = new bool;
    import_value(bool_p[0]);
    vars.emplace_back(str, bool_p);
    break;

   default: SGAL_error();
  }

  size_t i(1);
  for (; cit != codes.end(); ++cit) {
    auto expected_code = *cit;
    int code;
    import_code(code);
    if (code != expected_code) {
      // Error recovery
      read_unrecognized(code);
      return;
    }
    auto ct = code_type(code);
    SGAL_TRACE_CODE(get_trace_code_parsing(),
                    if (get_verbose_level() >= 8)
                      std::cout << "Parsing header member code type: "
                                << code_type_name(ct) << std::endl;);

    switch (ct) {
     case Code_type::STRING: import_value(str_p[i]);    break;
     case Code_type::DOUBLE: import_value(double_p[i]); break;
     case Code_type::INT8:   import_value(int8_p[i]);   break;
     case Code_type::INT16:  import_value(int16_p[i]);  break;
     case Code_type::INT32:  import_value(int32_p[i]);  break;
     case Code_type::UINT:   import_value(uint_p[i]);   break;
     case Code_type::BOOL:   import_value(bool_p[i]);   break;
     default: SGAL_error();
    }
    ++i;
  }
#if 0
  typedef Dxf_header_full_wrapper::String_header        String_header;
  typedef Dxf_header_full_wrapper::Double_header        Double_header;
  typedef Dxf_header_full_wrapper::Int8_header          Int8_header;
  typedef Dxf_header_full_wrapper::Int16_header         Int16_header;
  typedef Dxf_header_full_wrapper::Int32_header         Int32_header;
  typedef Dxf_header_full_wrapper::Uint_header          Uint_header;
  typedef Dxf_header_full_wrapper::Bool_header          Bool_header;
  typedef Dxf_header_full_wrapper::Double_2d_header     Double_2d_header;
  typedef Dxf_header_full_wrapper::Double_3d_header     Double_3d_header;

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
    SGAL_TRACE_CODE(get_trace_code_parsing(),
                    if (get_verbose_level() >= 8)
                      std::cout << "Parsing header member code type: "
                                << code_type_name(ct) << std::endl;);

    switch (ct) {
     case Code_type::STRING: assign_member<String_header>(handle, header); break;
     case Code_type::DOUBLE: assign_member<Double_header>(handle, header); break;
     case Code_type::INT8: assign_member<Int8_header>(handle, header); break;
     case Code_type::INT16: assign_member<Int16_header>(handle, header); break;
     case Code_type::INT32: assign_member<Int32_header>(handle, header); break;
     case Code_type::UINT: assign_member<Uint_header>(handle, header); break;
     case Code_type::BOOL: assign_member<Bool_header>(handle, header); break;
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
    SGAL_TRACE_CODE(get_trace_code_parsing(),
                    if (get_verbose_level() >= 8)
                      std::cout << "Parsing header member code type: "
                                << code_type_name(ct) << std::endl;);
    SGAL_assertion(DOUBLE == ct);
    if (dim == 2)
      (*m_is) >> (header.*(boost::get<Double_2d_header>(handle)))[i++];
    else (*m_is) >> (header.*(boost::get<Double_3d_header>(handle)))[i++];
    ++m_line;
  }
#endif
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
   case Code_type::STRING:
    import_value(str);
    msg += ", string value: " + str;
    break;

   case Code_type::BOOL:
    import_value(bval);
    msg += ", Bool value: " + std::to_string(bval);
    break;

   case Code_type::INT8:
   case Code_type::INT16:
   case Code_type::INT32:
    import_value(ival);
    msg += ", int value: " + std::to_string(ival);
    break;

   case Code_type::UINT:
    import_value(uval);
    stream << std::hex << uval;
    msg += ", unsigned int value: 0x" + stream.str();
    break;

   case Code_type::DOUBLE:
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
Dxf_base_object* Dxf_parser::parse_acad_proxy_object()
{ return parse_object("acad_proxy", new Dxf_acad_proxy_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_acdbdictionarywdflt_object()
{
  return parse_object("acdbdictionarywdflt",
                      new Dxf_acdbdictionarywdflt_object);
}

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_acdbplaceholder_object()
{ return parse_object("acdbplaceholder", new Dxf_acdbplaceholder_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_acdbnavisworksmodeldef_object()
{
  return parse_object("acdbnavisworksmodeldef",
                      new Dxf_acdbnavisworksmodeldef_object);
}

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_datatable_object()
{ return parse_object("datatable", new Dxf_datatable_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_dictionary_object()
{ return parse_object("dictionary", new Dxf_dictionary_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_dictionaryvar_object()
{ return parse_object("dictionaryvar", new Dxf_dictionaryvar_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_dimassoc_object()
{ return parse_object("dimassoc", new Dxf_dimassoc_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_field_object()
{ return parse_object("field", new Dxf_field_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_geodata_object()
{ return parse_object("geodata", new Dxf_geodata_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_group_object()
{ return parse_object("group", new Dxf_group_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_idbuffer_object()
{ return parse_object("idbuffer", new Dxf_idbuffer_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_imagedef_object()
{ return parse_object("imagedef", new Dxf_imagedef_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_imagedef_reactor_object()
{ return parse_object("imagedef_reactor", new Dxf_imagedef_reactor_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_layer_index_object()
{ return parse_object("layer_index", new Dxf_layer_index_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_layer_filter_object()
{ return parse_object("layer_filter", new Dxf_layer_filter_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_layout_object()
{ return parse_object("layout", new Dxf_layout_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_lightlist_object()
{ return parse_object("lightlist", new Dxf_lightlist_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_material_object()
{ return parse_object("material", new Dxf_material_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_mlinestyle_object()
{ return parse_object("mlinestyle", new Dxf_mlinestyle_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_object_ptr_object()
{ return parse_object("object_ptr", new Dxf_object_ptr_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_plotsettings_object()
{ return parse_object("plotsettings", new Dxf_plotsettings_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_rastervariables_object()
{ return parse_object("rastervariables", new Dxf_rastervariables_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_render_object()
{ return parse_object("render", new Dxf_render_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_section_object()
{ return parse_object("section", new Dxf_section_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_spatial_index_object()
{ return parse_object("spatial_index", new Dxf_spatial_index_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_spatial_filter_object()
{ return parse_object("spatial_filter", new Dxf_spatial_filter_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_sortentstable_object()
{ return parse_object("sortentstable", new Dxf_sortentstable_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_sunstudy_object()
{ return parse_object("sunstudy", new Dxf_sunstudy_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_tablestyle_object()
{ return parse_object("tablestyle", new Dxf_tablestyle_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_underlaydefinition_object()
{ return parse_object("underlaydefinition", new Dxf_underlaydefinition_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_visualstyle_object()
{ return parse_object("visualstyle", new Dxf_visualstyle_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_vba_project_object()
{ return parse_object("vba_project", new Dxf_vba_project_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_wipeoutvariables_object()
{ return parse_object("wipeoutvariables", new Dxf_wipeoutvariables_object); }

//! \brief parses a object.
Dxf_base_object* Dxf_parser::parse_xrecord_object()
{ return parse_object("xrecord", new Dxf_xrecord_object); }

//! \brief parses a regular boundary path.
void Dxf_parser::parse_boundary_path(Dxf_boundary_path& path)
{
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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
  SGAL_TRACE_CODE(get_trace_code_parsing(),
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

DXF_END_NAMESPACE
