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

#include <iostream>
#include <iomanip>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_writer.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_header_wrapper.hpp"
#include "dxf/Dxf_base_table.hpp"
#include "dxf/Dxf_table_entry.hpp"

DXF_BEGIN_NAMESPACE

//! \brief constructs
Dxf_writer::Dxf_writer() :
  m_data(nullptr),
  m_owned(false)
{}

//! \brief destructs.
Dxf_writer::~Dxf_writer()
{
  if (m_owned) {
    delete m_data;
    m_data = nullptr;
    m_owned = false;
  }
}

//! \brief writes a scene to an output stream in the dxf format.
void Dxf_writer::operator()(SGAL::Scene_graph* scene_graph,
                            std::ostream& os, const std::string& filename,
                            bool /* is_binary */)
{
  set_out(os);
  m_scene_graph = scene_graph;

  // Populate the Dxf_data with the minimal requirements
  m_data = new Dxf_data;
  init();

  // Export
  std::string comment("Generated by SGAL Version ");
  comment.append(std::to_string(SGAL_VERSION));
  export_string(999, comment);
  export_section([&]() { export_header(); });
  if (! m_data->m_classes.empty()) export_section([&]() { export_classes(); });
  if (! m_data->tables_empty()) export_section([&]() { export_tables(); });
  if (! m_data->m_blocks.empty()) export_section([&]() { export_blocks(); });
  export_section([&]() { export_entities(); });
  if (! m_data->m_objects.empty()) export_section([&]() { export_objects(); });

  if (! m_data->m_thumbnailimage.empty())
    export_section([&]() { export_thumbnailimage(); });
  if (! m_data->m_acdsdata.empty())
    export_section([&]() { export_acdsdata(); });
  export_string(0, "EOF");
}

//! \brief exports a given code.
void Dxf_writer::export_code(int code)
{ out() << std::setw(3) << code << std::endl; }

//! \brief export a string item.
void Dxf_writer::export_string(int code, const std::string& str)
{
  if (str.empty()) return;
  export_code(code);
  export_value(str);
}

#if ! ClimbingStats_COMPILER_CXX_GENERIC_LAMDAS
struct Visitor : public boost::static_visitor<void> {
  Visitor(Dxf_writer& writer, const std::list<int>& codes) :
    m_writer(writer),
    m_codes(codes)
  {}

  Dxf_writer& m_writer;
  const std::list<int>& m_codes;

  template <typename T>
  void operator()(const T* value)
  {
    for (auto code : m_codes) {
      m_writer.export_code(code);
      m_writer.export_value(*value++);
    }
  }
};
#endif

//! \brief writes the HEADER section.
void Dxf_writer::export_header()
{
  export_string(2, "HEADER");

  SGAL_assertion(m_data);
  auto& header = m_data->m_header;
  if (header.empty()) return;

  auto& vars = header.m_variables;

  // my_visitor visitor(m_data);
  const auto& members = Dxf_header_wrapper::s_header_members;

  for (const auto& var : vars) {
    export_code(9);
    export_value(var.m_name);
    auto it = members.find(var.m_name);
    SGAL_assertion(it != members.end());
    const auto& codes = it->second;
#if ClimbingStats_COMPILER_CXX_GENERIC_LAMDAS
    boost::apply_visitor([&](auto&& value) {
        for (auto code : codes) {
          export_code(code);
          export_value(*value++);
        }
      }, var.m_value);
#else
    Visitor visitor(*this, codes);
    boost::apply_visitor(visitor, var.m_value);
#endif
  }
}

//! \brief writes the CLASSES section.
void Dxf_writer::export_classes()
{
  export_string(2, "CLASSES");
  SGAL_assertion(m_data);
  auto& classes = m_data->m_classes;
  if (classes.empty()) return;

  //! \todo Export classes
}

//! \brief writes the TABLES section.
void Dxf_writer::export_tables()
{
  export_string(2, "TABLES");

  SGAL_assertion(m_data);
  if (m_data->tables_empty()) return;

  auto& appid_table = m_data->m_appid_table;
  if (! appid_table.empty()) export_table(appid_table, "APPID");

  auto& block_record_table = m_data->m_block_record_table;
  if (! block_record_table.empty())
    export_table(block_record_table, "BLOCK_RECORD");

  auto& dimstyle_table = m_data->m_dimstyle_table;
  if (! dimstyle_table.empty()) export_table(dimstyle_table, "DIMSTYLE");

  auto& layer_table = m_data->m_layer_table;
  if (! layer_table.empty()) export_table(layer_table, "LAYER");

  auto& ltype_table = m_data->m_ltype_table;
  if (! ltype_table.empty()) export_table(ltype_table, "LTYPE");

  auto& style_table = m_data->m_style_table;
  if (! style_table.empty()) export_table(style_table, "STYLE");

  auto& ucs_table = m_data->m_ucs_table;
  if (! ucs_table.empty()) export_table(ucs_table, "UCS");

  auto& view_table = m_data->m_view_table;
  if (! view_table.empty()) export_table(view_table, "VIEW");

  auto& vport_table = m_data->m_vport_table;
  if (! vport_table.empty()) export_table(vport_table, "VPORT");
}

//! \brief writes the BLOCKS section.
void Dxf_writer::export_blocks()
{
  export_string(2, "BLOCKS");
  SGAL_assertion(m_data);
  auto& blocks = m_data->m_blocks;
  if (blocks.empty()) return;

  //! \todo Export BLOCKS
}

//! \brief writes the ENTITIES section.
void Dxf_writer::export_entities()
{
  export_string(2, "ENTITIES");

  SGAL_assertion(m_data);
  auto& entities = m_data->m_entities;
  if (entities.empty()) return;

  //! \todo Export ENTITIES
}

//! \brief writes the OBJECTS section.
void Dxf_writer::export_objects()
{
  export_string(2, "OBJECTS");

  SGAL_assertion(m_data);
  auto& objects = m_data->m_objects;
  if (objects.empty()) return;

  //! \todo Export ENTITIES
}

//! \brief writes the THUMBNAILIMAGE section.
void Dxf_writer::export_thumbnailimage()
{
  export_string(2, "THUMBNAILIMAGE");

  SGAL_assertion(m_data);
  auto& thumbnailimage = m_data->m_thumbnailimage;
  if (thumbnailimage.empty()) return;

  //! \todo Export THUMBNAILIMAGE
}

//! \brief writes the ACDSDATA section.
void Dxf_writer::export_acdsdata()
{
  export_string(2, "ACDSDATA");
  SGAL_assertion(m_data);
  auto& acdsdata = m_data->m_acdsdata;
  if (acdsdata.empty()) return;

  //! \todo Export ACDSDATA
}

//! \brief exports a given base_table.
void Dxf_writer::export_base_table(const Dxf_base_table& base_table)
{
  export_string(5, base_table.m_handle);
  export_string(360, base_table.m_owner_dict);
  export_string(330, base_table.m_owner_obj);
}

//! \brief exports a given entry.
void Dxf_writer::export_base_entry(const Dxf_table_entry& base_entry)
{
  export_string(5, base_entry.m_handle);
  export_string(360, base_entry.m_owner_dict);
  export_string(330, base_entry.m_owner_obj);
}

void Dxf_writer::export_entry(const Dxf_appid_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_block_record_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_dimstyle_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_layer_entry& entry)
{
  export_string(2, entry.m_name);
  export_item(70, entry.m_flags);
  export_item(62, entry.m_color_index);
  export_item(420, entry.m_color, static_cast<int32_t>(-1));
  export_string(6, entry.m_line_type);

  //! \todo export optional
  // (290, entry.m_is_layer_plotted);
  // (370, entry.m_line_weight);
  // (390, entry.m_plot_style_pointer);
  // (347, entry.m_material_handle);
}

void Dxf_writer::export_entry(const Dxf_ltype_entry& entry)
{
  export_string(2, entry.m_name);
  export_item(70, entry.m_flags);
  export_string(3, entry.m_description);
  export_item(72, entry.m_alignment_code);
  export_item(73, entry.m_element_count);
  export_item(40, entry.m_total_pattern_length);

  //! \todo export optional
  // (49, entry.m_dash_dot_space_lengths);
  // (74, entry.m_complex_line_type_element_types);
  // (75, entry.m_shape_numbers);
  // (340, entry.m_style_pointers);
  // (46, entry.m_scale_values);
  // (50, entry.m_rotation_angles);
  // (44, entry.m_offsets);
  // (45, entry.m_offsets);
  // (9, entry.m_text_strings);
}

void Dxf_writer::export_entry(const Dxf_style_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_ucs_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_view_entry& entry)
{
}

void Dxf_writer::export_entry(const Dxf_vport_entry& entry)
{
}

//! \brief initializes with the minimal requirements.
void Dxf_writer::init()
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();
  auto it = Dxf_header::s_version_names.find(version);

  if (version <= 12) {
    // Minimal
    auto& vars = m_data->m_header.m_variables;
    SGAL_assertion(it != Dxf_header::s_version_names.end());
    vars.emplace_back("$ACADVER", new std::string(it->second));
    return;
  }

  // HEADER variables
  auto& vars = m_data->m_header.m_variables;
  vars.emplace_back("$ACADVER", new std::string(it->second));
  vars.emplace_back("$INSBASE", new double[3]{0, 0, 0});
  vars.emplace_back("$EXTMIN", new double[3]{0, 0, 0});
  vars.emplace_back("$EXTMAX", new double[3]{100, 100, 0});
  vars.emplace_back("$UNITMODE", new int16_t(0));
  vars.emplace_back("$AUNITS", new int16_t(0));
  vars.emplace_back("$ANGBASE", new double(0));
  vars.emplace_back("$ANGDIR", new int16_t(0));

  // CLASSES

  // TABLES
  auto& ltype_table = m_data->m_ltype_table;
  ltype_table.m_entries.emplace_back();
  auto& ltype_entry = ltype_table.m_entries.back();
  ltype_entry.m_name.assign("CONTINUOUS");
  ltype_entry.m_flags = Dxf_ltype_entry::REFERENCED;
  ltype_entry.m_description = "Solid line";
  ltype_entry.m_alignment_code = 65;
  ltype_entry.m_element_count = 0;
  ltype_entry.m_total_pattern_length = 0.0;

  auto& layer_table = m_data->m_layer_table;
  layer_table.m_entries.emplace_back();
  auto& layer_entry = layer_table.m_entries.back();
  layer_entry.m_name = "0";
  layer_entry.m_flags = 64;
  layer_entry.m_color_index = 7;
  layer_entry.m_line_type = "CONTINUOUS";

  // BLOCKS

  // ENtitIES

  // OBJECTS
}

DXF_END_NAMESPACE
