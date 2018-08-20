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
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"
#include "dxf/Dxf_writer.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_header_wrapper.hpp"
#include "dxf/Dxf_base_table.hpp"
#include "dxf/Dxf_base_entry.hpp"
#include "dxf/Dxf_block.hpp"
#include "dxf/Dxf_endblk.hpp"
#include "dxf/Line_weight.hpp"

#include "dxf/Dxf_dictionary_object.hpp"

DXF_BEGIN_NAMESPACE

const size_t Dxf_writer::s_vport_table_handle(0x8);
const size_t Dxf_writer::s_vport_entry_handle(0x29);
const size_t Dxf_writer::s_ltype_table_handle(0x5);
const size_t Dxf_writer::s_ltype_byblock_handle(0x14);
const size_t Dxf_writer::s_ltype_bylayer_handle(0x15);
const size_t Dxf_writer::s_ltype_continuous_handle(0x16);
const size_t Dxf_writer::s_layer_table_handle(0x2);
const size_t Dxf_writer::s_layer_entry_handle(0x10);
const size_t Dxf_writer::s_style_table_handle(0x3);
const size_t Dxf_writer::s_style_entry_handle(0x11);
const size_t Dxf_writer::s_view_table_handle(0x6);
const size_t Dxf_writer::s_ucs_table_handle(0x7);
const size_t Dxf_writer::s_appid_table_handle(0x9);
const size_t Dxf_writer::s_appid_entry_handle(0x12);
const size_t Dxf_writer::s_dimstyle_table_handle(0xA);
const size_t Dxf_writer::s_dimstyle_entry_handle(0x27);
const size_t Dxf_writer::s_block_record_table_handle(0x1);
const size_t Dxf_writer::s_block_record_model_space_handle(0x1F);
const size_t Dxf_writer::s_block_record_paper_space_handle(0x1B);
const size_t Dxf_writer::s_block_model_space_handle(0x20);
const size_t Dxf_writer::s_endblk_model_space_handle(0x21);
const size_t Dxf_writer::s_block_paper_space_handle(0x1C);
const size_t Dxf_writer::s_endblk_paper_space_handle(0x1D);
const size_t Dxf_writer::s_disctionary1_handle(0xC);
const size_t Dxf_writer::s_disctionary2_handle(0xD);

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

  auto* data = new Dxf_data;
  init(data);           // populate the data with the minimal requirements
  update(data);         // Add the scene
  export_data(data);    // export
}

//! \brief adds the scene to a DXF data structure.
void Dxf_writer::update(Dxf_data* data)
{
}

//! \brief exports a DXF data structure.
void Dxf_writer::export_data(Dxf_data* data)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();

  std::string comment("Generated by SGAL Version ");
  comment.append(std::to_string(SGAL_VERSION));
  export_string(999, comment);
  export_section([&]() { export_header(data); });
  if (! data->m_classes.empty() || ((version > 12)))
    export_section([&]() { export_classes(data); });
  if (! data->tables_empty()) export_section([&]() { export_tables(data); });
  if (! data->m_blocks.empty()) export_section([&]() { export_blocks(data); });
  export_section([&]() { export_entities(data); });
  if (! data->m_objects.empty()) export_section([&]() { export_objects(data); });

  if (! data->m_thumbnailimage.empty())
    export_section([&]() { export_thumbnailimage(data); });
  if (! data->m_acdsdata.empty())
    export_section([&]() { export_acdsdata(data); });
  export_string(0, "EOF");
}

//! \brief exports a given code.
void Dxf_writer::export_code(int code)
{ out() << std::setw(3) << code << std::endl; }

//! \brief export a string item.
void Dxf_writer::export_string(int code, const std::string& str)
{ export_item(code, str); }

//! \brief exports a string item if the string is not empty.
void Dxf_writer::export_nonempty_string(int code, const std::string& str)
{
  if (str.empty()) return;
  export_string(code, str);
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
void Dxf_writer::export_header(Dxf_data* data)
{
  export_string(2, "HEADER");

  SGAL_assertion(data);
  auto& header = data->m_header;
  if (header.empty()) return;

  auto& vars = header.m_variables;

  // my_visitor visitor(data);
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
void Dxf_writer::export_classes(Dxf_data* data)
{
  export_string(2, "CLASSES");
  SGAL_assertion(data);
  auto& classes = data->m_classes;
  if (classes.empty()) return;

  //! \todo Export classes
}

//! \brief writes the TABLES section.
void Dxf_writer::export_tables(Dxf_data* data)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();

  export_string(2, "TABLES");

  SGAL_assertion(data);
  if (data->tables_empty()) return;

  auto& vport_table = data->m_vport_table;
  if ((version > 12) || ! vport_table.empty())
    export_table(vport_table, "VPORT");

  auto& ltype_table = data->m_ltype_table;
  if ((version > 12) || ! ltype_table.empty())
    export_table(ltype_table, "LTYPE");

  auto& layer_table = data->m_layer_table;
  if ((version > 12) || ! layer_table.empty())
    export_table(layer_table, "LAYER");

  auto& style_table = data->m_style_table;
  if ((version > 12) || ! style_table.empty())
    export_table(style_table, "STYLE");

  auto& view_table = data->m_view_table;
  if ((version > 12) || ! view_table.empty())
    export_table(view_table, "VIEW");

  auto& ucs_table = data->m_ucs_table;
  if ((version > 12) || ! ucs_table.empty())
    export_table(ucs_table, "UCS");

  auto& appid_table = data->m_appid_table;
  if ((version > 12) || ! appid_table.empty())
    export_table(appid_table, "APPID");

  auto& dimstyle_table = data->m_dimstyle_table;
  if ((version > 12) || ! dimstyle_table.empty())
    export_table(dimstyle_table, "DIMSTYLE");

  auto& block_record_table = data->m_block_record_table;
  if ((version > 12) || ! block_record_table.empty())
    export_table(block_record_table, "BLOCK_RECORD");
}

//! \brief writes the BLOCKS section.
void Dxf_writer::export_blocks(Dxf_data* data)
{
  export_string(2, "BLOCKS");
  SGAL_assertion(data);
  auto& blocks = data->m_blocks;
  if (blocks.empty()) return;

  for (const auto& block : data->m_blocks) {
    export_string(0, "BLOCK");
    export_block(block.first);
    export_string(0, "ENDBLK");
    export_endblk(block.second);
  }
  //! \todo Export BLOCKS
}

//! \brief writes the ENTITIES section.
void Dxf_writer::export_entities(Dxf_data* data)
{
  export_string(2, "ENTITIES");

  SGAL_assertion(data);
  auto& entities = data->m_entities;
  if (entities.empty()) return;

  //! \todo Export ENTITIES
}

//! \brief writes the OBJECTS section.
void Dxf_writer::export_objects(Dxf_data* data)
{
  export_string(2, "OBJECTS");

  SGAL_assertion(data);
  auto& objects = data->m_objects;
  if (objects.empty()) return;

  for (const auto* base_object : data->m_objects) {
    auto* dictionary_object =
      dynamic_cast<const Dxf_dictionary_object*>(base_object);
    if (dictionary_object) export_object(*dictionary_object);
  }
}

//! \brief writes the THUMBNAILIMAGE section.
void Dxf_writer::export_thumbnailimage(Dxf_data* data)
{
  export_string(2, "THUMBNAILIMAGE");

  SGAL_assertion(data);
  auto& thumbnailimage = data->m_thumbnailimage;
  if (thumbnailimage.empty()) return;

  //! \todo Export THUMBNAILIMAGE
}

//! \brief writes the ACDSDATA section.
void Dxf_writer::export_acdsdata(Dxf_data* data)
{
  export_string(2, "ACDSDATA");
  SGAL_assertion(data);
  auto& acdsdata = data->m_acdsdata;
  if (acdsdata.empty()) return;

  //! \todo Export ACDSDATA
}

//! \brief exports a given base_table.
void Dxf_writer::export_base_table(const Dxf_base_table& base_table)
{
  export_nonempty_string(5, base_table.m_handle);
  export_nonempty_string(360, base_table.m_owner_dict);
  export_nonempty_string(330, base_table.m_owner_obj);
  export_string(100, "AcDbSymbolTable");
}

//! \brief exports a given entry.
void Dxf_writer::export_base_entry(const Dxf_base_entry& base_entry)
{
  export_nonempty_string(5, base_entry.m_handle);
  export_nonempty_string(360, base_entry.m_owner_dict);
  export_nonempty_string(330, base_entry.m_owner_obj);
  export_string(100, "AcDbSymbolTableRecord");
}

//! \brief exports a DIMSTYLE entry.
inline void Dxf_writer::export_entry(const Dxf_dimstyle_entry& entry,
                                     const std::string& name)
{
  export_string(100, "AcDbDimStyleTable");
  export_string(0, name);
  export_dimstyle_base_entry(entry);
  export_entry(entry);
}

//! \brief exports a DIMSTYLE base entry.
void Dxf_writer::export_dimstyle_base_entry(const Dxf_base_entry& base_entry)
{
  export_nonempty_string(105, base_entry.m_handle);
  export_nonempty_string(360, base_entry.m_owner_dict);
  export_nonempty_string(330, base_entry.m_owner_obj);
  export_string(100, "AcDbSymbolTableRecord");
}

//! \brief exports a VPORT table entry.
void Dxf_writer::export_entry(const Dxf_vport_entry& entry)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();

  export_string(100, "AcDbViewportTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_vport_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(10, entry, members);
  export_member(20, entry, members);
  export_member(11, entry, members);
  export_member(21, entry, members);
  export_member(12, entry, members);
  export_member(22, entry, members);
  export_member(13, entry, members);
  export_member(23, entry, members);
  export_member(14, entry, members);
  export_member(24, entry, members);
  export_member(15, entry, members);
  export_member(25, entry, members);
  export_member(16, entry, members);
  export_member(26, entry, members);
  export_member(36, entry, members);
  export_member(17, entry, members);
  export_member(27, entry, members);
  export_member(37, entry, members);
  if (version < 19) export_member(40, entry, members);
  export_member(41, entry, members);
  export_member(42, entry, members);
  export_member(43, entry, members);
  export_member(44, entry, members);
  if (version >= 19) export_member(45, entry, members);
  export_member(50, entry, members);
  export_member(51, entry, members);
  export_nonempty_member(331, entry, members);
  export_nonempty_member(341, entry, members);
  export_nonempty_member(1, entry, members);
  export_member(71, entry, members);
  export_member(72, entry, members);
  export_member(73, entry, members);
  export_member(74, entry, members);
  export_member(75, entry, members);
  export_member(76, entry, members);
  export_member(77, entry, members);
  export_member(78, entry, members);
  export_member(281, entry, members);
  export_member(65, entry, members);
  export_member(110, entry, members);
  export_member(120, entry, members);
  export_member(130, entry, members);
  export_member(111, entry, members);
  export_member(121, entry, members);
  export_member(131, entry, members);
  export_member(112, entry, members);
  export_member(122, entry, members);
  export_member(132, entry, members);
  export_nonempty_member(345, entry, members);
  export_nonempty_member(346, entry, members);
  export_member(79, entry, members);
  export_member(146, entry, members);
  if (version >= 19) export_member(170, entry, members);
  if (version >= 19) export_member(61, entry, members);
  export_nonempty_member(332, entry, members);
  export_nonempty_member(333, entry, members);
  export_member(348, entry, members);
  export_member(292, entry, members);
  export_member(282, entry, members);
  export_member(141, entry, members);
  export_member(142, entry, members);
  export_member(63, entry, members);
  export_member(421, entry, members);
  export_nonempty_member(431, entry, members);
}

//! \brief exports a LTYPE table entry.
void Dxf_writer::export_entry(const Dxf_ltype_entry& entry)
{
  export_string(100, "AcDbLinetypeTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_ltype_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(3, entry, members);
  export_member(72, entry, members);
  export_member(73, entry, members);
  export_member(40, entry, members);

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

//! \brief exports a LAYER table entry.
void Dxf_writer::export_entry(const Dxf_layer_entry& entry)
{
  export_string(100, "AcDbLayerTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_layer_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(62, entry, members);
  // export_member(420, entry, members);
  export_member(6, entry, members);
  export_item(290, entry.m_is_layer_plotted, entry.s_def_line_weight);
  export_member(370, entry, members);
  export_nonempty_member(390, entry, members);
  export_nonempty_member(347, entry, members);
}

//! \brief exports a STYLE table entry.
void Dxf_writer::export_entry(const Dxf_style_entry& entry)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();

  export_string(100, "AcDbTextStyleTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_style_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(40, entry, members);
  export_member(41, entry, members);
  export_member(50, entry, members);
  export_member(71, entry, members);
  export_member(42, entry, members);
  export_member(3, entry, members);
  export_member(4, entry, members);
  if (version >= 19) export_member(1071, entry, members);
}

//! \brief exports a VIEW table entry.
void Dxf_writer::export_entry(const Dxf_view_entry& entry)
{
  export_string(100, "AcDbViewTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_view_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(40, entry, members);
  export_member(10, entry, members);
  export_member(20, entry, members);
  export_member(41, entry, members);
  export_member(11, entry, members);
  export_member(21, entry, members);
  export_member(31, entry, members);
  export_member(12, entry, members);
  export_member(22, entry, members);
  export_member(32, entry, members);
  export_member(42, entry, members);
  export_member(43, entry, members);
  export_member(44, entry, members);
  export_member(50, entry, members);
  export_member(71, entry, members);
  export_member(281, entry, members);
  export_member(72, entry, members);
  export_member(73, entry, members);
  export_member(332, entry, members);
  export_member(334, entry, members);
  export_member(348, entry, members);
  export_member(361, entry, members);
  export_member(110, entry, members);
  export_member(120, entry, members);
  export_member(130, entry, members);
  export_member(111, entry, members);
  export_member(121, entry, members);
  export_member(131, entry, members);
  export_member(112, entry, members);
  export_member(122, entry, members);
  export_member(132, entry, members);
  export_member(79, entry, members);
  export_member(146, entry, members);
  export_member(345, entry, members);
  export_member(346, entry, members);
}

//! \brief exports a UCS table entry.
void Dxf_writer::export_entry(const Dxf_ucs_entry& entry)
{
  export_string(100, "AcDbUCSTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_ucs_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_member(10, entry, members);
  export_member(20, entry, members);
  export_member(30, entry, members);
  export_member(11, entry, members);
  export_member(21, entry, members);
  export_member(31, entry, members);
  export_member(12, entry, members);
  export_member(22, entry, members);
  export_member(32, entry, members);
  export_member(79, entry, members);
  export_member(146, entry, members);
  export_member(346, entry, members);
  export_member(71, entry, members);
  export_member(13, entry, members);
  export_member(23, entry, members);
  export_member(33, entry, members);
}

//! \brief exports a APPID table entry.
void Dxf_writer::export_entry(const Dxf_appid_entry& entry)
{
  export_string(100, "AcDbRegAppTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_appid_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
}

//! \brief exports a DIMSTYLE table entry.
void Dxf_writer::export_entry(const Dxf_dimstyle_entry& entry)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto version = dxf_conf->get_version();

  export_string(100, "AcDbDimStyleTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_dimstyle_entry>::s_record_members;
  export_member(2, entry, members);
  export_member(70, entry, members);
  export_nonempty_member(3, entry, members);
  export_nonempty_member(4, entry, members);
  export_nonempty_member(5, entry, members);
  export_nonempty_member(6, entry, members);
  export_nonempty_member(7, entry, members);
  export_member(40, entry, members);
  export_member(41, entry, members);
  export_member(42, entry, members);
  export_member(43, entry, members);
  export_member(44, entry, members);
  export_member(45, entry, members);
  export_member(46, entry, members);
  export_member(47, entry, members);
  export_member(48, entry, members);
  export_member(140, entry, members);
  export_member(141, entry, members);
  export_member(142, entry, members);
  export_member(143, entry, members);
  export_member(144, entry, members);
  export_member(145, entry, members);
  export_member(146, entry, members);
  export_member(147, entry, members);
  export_member(148, entry, members);
  export_member(71, entry, members);
  export_member(72, entry, members);
  export_member(73, entry, members);
  export_member(74, entry, members);
  export_member(75, entry, members);
  export_member(76, entry, members);
  export_member(77, entry, members);
  export_member(78, entry, members);
  export_member(79, entry, members);
  export_member(170, entry, members);
  export_member(171, entry, members);
  export_member(172, entry, members);
  export_member(173, entry, members);
  export_member(174, entry, members);
  export_member(175, entry, members);
  export_member(176, entry, members);
  export_member(177, entry, members);
  export_member(178, entry, members);
  export_member(179, entry, members);
  if (version <= 12) export_member(270, entry, members);
  export_member(271, entry, members);
  export_member(272, entry, members);
  export_member(273, entry, members);
  export_member(274, entry, members);
  export_member(275, entry, members);
  if (version > 12) {
    export_member(276, entry, members);
    export_member(277, entry, members);
  }
  export_member(278, entry, members);
  export_member(279, entry, members);
  export_member(280, entry, members);
  export_member(281, entry, members);
  export_member(282, entry, members);
  export_member(283, entry, members);
  export_member(284, entry, members);
  export_member(285, entry, members);
  export_member(286, entry, members);
  export_member(288, entry, members);
  export_member(289, entry, members);
  export_member(340, entry, members);
  export_member(341, entry, members);
  export_nonempty_member(342, entry, members);
  export_nonempty_member(343, entry, members);
  export_nonempty_member(344, entry, members);
  export_member(371, entry, members);
  export_member(372, entry, members);
}

//! \brief exports a BLOCK_RECORD_entry table entry.
void Dxf_writer::export_entry(const Dxf_block_record_entry& entry)
{
  export_string(100, "AcDbBlockTableRecord");

  auto& members = Dxf_record_wrapper<Dxf_block_record_entry>::s_record_members;
  export_member(2, entry, members);
  export_nonempty_member(340, entry, members);
  export_member(70, entry, members);
  export_member(280, entry, members);
  export_member(281, entry, members);
  export_nonempty_member(310, entry, members);
  // {1070, {&Dxf_block_record_entry::m_design_center_version_number, 1, 0}},
  // {1070, {&Dxf_block_record_entry::m_insert_units, 1, 0}},
  export_nonempty_member(1001, entry, members);
  export_nonempty_member(1000, entry, members);
  // {1002, {&Dxf_block_record_entry::m_xdata, n, i}},
}

//! \brief exports a BLOCK record.
void Dxf_writer::export_block(const Dxf_block& block)
{
  auto& members = Dxf_record_wrapper<Dxf_block>::s_record_members;

  export_member(5, block, members);
  export_member(330, block, members);
  export_string(100, "AcDbEntity");
  export_item(67, block.m_paper_space, block.s_def_paper_space);
  export_member(8, block, members);
  export_string(100, "AcDbBlockBegin");
  export_member(2, block, members);
  export_member(70, block, members);
  export_member(10, block, members);
  export_member(20, block, members);
  export_member(30, block, members);
  export_member(3, block, members);
  export_member(1, block, members);
  export_nonempty_member(4, block, members);
}

//! \brief exports an ENDBLK record.
void Dxf_writer::export_endblk(const Dxf_endblk& endblk)
{
  auto& members = Dxf_record_wrapper<Dxf_endblk>::s_record_members;

  export_member(5, endblk, members);
  export_member(330, endblk, members);
  export_string(100, "AcDbEntity");
  export_member(8, endblk, members);
  export_string(100, "AcDbBlockEnd");
}

//! \brief exports a base object record.
void Dxf_writer::export_base_object(const Dxf_base_object& base_object)
{
  auto& members = Dxf_simple_record_wrapper<Dxf_base_object>::s_record_members;
  export_simple_member(5, base_object, members);
  export_nonempty_simple_member(360, base_object, members);
  export_simple_member(330, base_object, members);
}

//! \brief exports a DICT>IONARY object record.
void Dxf_writer::export_object(const Dxf_dictionary_object& object)
{
  export_string(0, "DICTIONARY");
  export_base_object(object);
  auto& members = Dxf_record_wrapper<Dxf_dictionary_object>::s_record_members;
  export_string(100, "AcDbDictionary");
  export_item(280, object.m_is_hard_owner, object.s_def_is_hard_owner);
  export_member(281, object, members);
  for (const auto& entry : object.m_value_handles) {
    export_item(3, entry.first);
    export_item(350, entry.second);
  }
}

//! \brief initializes a DXF data structure with the minimal requirements.
void Dxf_writer::init(Dxf_data* data)
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
    auto& vars = data->m_header.m_variables;
    SGAL_assertion(it != Dxf_header::s_version_names.end());
    vars.emplace_back("$ACADVER", new std::string(it->second));
    return;
  }

  // HEADER variables
  auto& vars = data->m_header.m_variables;
  vars.emplace_back("$ACADVER", new std::string(it->second));
  vars.emplace_back("$HANDSEED", new std::string("20000"));

  // CLASSES

  // TABLES

  // VPORT
  auto vport_handle = to_hex_string(s_vport_table_handle);
  auto& vport_table = data->m_vport_table;
  vport_table.m_handle = vport_handle;
  vport_table.m_owner_obj = "0";

  vport_table.m_entries.emplace_back();
  auto& vport_entry = vport_table.m_entries.back();
  vport_entry.m_handle = to_hex_string(s_vport_entry_handle);
  vport_entry.m_owner_obj = vport_handle;
  vport_entry.m_name = "*ACTIVE";
  vport_entry.m_flags = 0;
  vport_entry.m_lower_left[0] = 0.0;
  vport_entry.m_lower_left[1] = 0.0;
  vport_entry.m_upper_right[0] = 1.0;
  vport_entry.m_upper_right[1] = 1.0;
  vport_entry.m_view_center[0] = 209.475294253;
  vport_entry.m_view_center[1] = 86.0026335861;
  vport_entry.m_snap_base[0] = 0;
  vport_entry.m_snap_base[1] = 0;
  vport_entry.m_snap_spacing[0] = 10;
  vport_entry.m_snap_spacing[1] = 10;
  vport_entry.m_grid_spacing[0] = 10;
  vport_entry.m_grid_spacing[1] = 10;
  vport_entry.m_view_direction[0] = 0;
  vport_entry.m_view_direction[1] = 0;
  vport_entry.m_view_direction[2] = 1;
  vport_entry.m_view_target[0] = 0;
  vport_entry.m_view_target[1] = 0;
  vport_entry.m_view_target[1] = 0;
  vport_entry.m_view_height = 319.744231092;
  vport_entry.m_aspect_ratio = 2.12946428571;
  vport_entry.m_lens_length = 50;
  vport_entry.m_front_clipping_plane = 0;
  vport_entry.m_back_clipping_plane = 0;
  vport_entry.m_snap_rotation_angle = 0;
  vport_entry.m_view_twist_angle = 0;
  vport_entry.m_view_mode = 0;
  vport_entry.m_circle_sides = 100;
  vport_entry.m_fast_zoom = 1;
  vport_entry.m_ucs_icon = 3;
  vport_entry.m_snap_on = 0;
  vport_entry.m_grid_on = 0;
  vport_entry.m_snap_style = 0;
  vport_entry.m_snap_isopair = 0;
  vport_entry.m_render_mode = 0;
  vport_entry.m_ucs_up = 1;
  vport_entry.m_ucs_origin[0] = 0;
  vport_entry.m_ucs_origin[1] = 0;
  vport_entry.m_ucs_origin[2] = 0;
  vport_entry.m_ucs_x_axis[0] = 1;
  vport_entry.m_ucs_x_axis[1] = 0;
  vport_entry.m_ucs_x_axis[2] = 0;
  vport_entry.m_ucs_y_axis[0] = 0;
  vport_entry.m_ucs_y_axis[1] = 1;
  vport_entry.m_ucs_y_axis[2] = 0;
  vport_entry.m_orthographic_type = 0;
  vport_entry.m_elevation = 0;
  vport_entry.m_visual_style_object_pointer = "10020";
  // vport_entry.(60) = 7;
  // vport_entry.(61) = 5;
  vport_entry.m_is_default_lighting_on = 1;
  vport_entry.m_default_lighting_type = 1;
  vport_entry.m_brightness = 0;
  vport_entry.m_contrast = 0;
  vport_entry.m_ambient_color = 250;
  vport_entry.m_ambient_color_i32 = 3358443;

  // LTYPE
  auto ltype_handle = to_hex_string(s_ltype_table_handle);
  auto& ltype_table = data->m_ltype_table;
  ltype_table.m_handle = ltype_handle;
  ltype_table.m_owner_obj = "0";

  ltype_table.m_entries.emplace_back();
  auto& ltype_entry1 = ltype_table.m_entries.back();
  ltype_entry1.m_handle = to_hex_string(s_ltype_byblock_handle);
  ltype_entry1.m_owner_obj = ltype_handle;
  ltype_entry1.m_name.assign("ByBlock");
  ltype_entry1.m_flags = 0;
  ltype_entry1.m_description = "";
  ltype_entry1.m_alignment_code = 65;
  ltype_entry1.m_element_count = 0;
  ltype_entry1.m_total_pattern_length = 0.0;

  ltype_table.m_entries.emplace_back();
  auto& ltype_entry2 = ltype_table.m_entries.back();
  ltype_entry2.m_handle = to_hex_string(s_ltype_bylayer_handle);
  ltype_entry2.m_owner_obj = ltype_handle;
  ltype_entry2.m_name.assign("ByLayer");
  ltype_entry2.m_flags = 0;
  ltype_entry2.m_description = "";
  ltype_entry2.m_alignment_code = 65;
  ltype_entry2.m_element_count = 0;
  ltype_entry2.m_total_pattern_length = 0.0;

  ltype_table.m_entries.emplace_back();
  auto& ltype_entry3 = ltype_table.m_entries.back();
  ltype_entry3.m_handle = to_hex_string(s_ltype_continuous_handle);
  ltype_entry3.m_owner_obj = ltype_handle;
  ltype_entry3.m_name.assign("Continuous");
  ltype_entry3.m_flags = 0;
  ltype_entry3.m_description = "Solid line";
  ltype_entry3.m_alignment_code = 65;
  ltype_entry3.m_element_count = 0;
  ltype_entry3.m_total_pattern_length = 0.0;

  // LAYER
  auto layer_handle = to_hex_string(s_layer_table_handle);
  auto& layer_table = data->m_layer_table;
  layer_table.m_handle = layer_handle;
  layer_table.m_owner_obj = "0";

  layer_table.m_entries.emplace_back();
  auto& layer_entry = layer_table.m_entries.back();
  layer_entry.m_handle = to_hex_string(s_layer_entry_handle);
  layer_entry.m_owner_obj = layer_handle;
  layer_entry.m_name = "0";
  layer_entry.m_flags = 0;
  layer_entry.m_color_index = 7;
  layer_entry.m_line_type = "Continuous";

  //! \todo why "F"?
  layer_entry.m_plot_style_pointer = "F";

  // STYLE
  auto style_handle = to_hex_string(s_style_table_handle);
  auto& style_table = data->m_style_table;
  style_table.m_handle = style_handle;
  style_table.m_owner_obj = "0";

  style_table.m_entries.emplace_back();
  auto& style_entry = style_table.m_entries.back();
  style_entry.m_handle = to_hex_string(s_style_entry_handle);
  style_entry.m_owner_obj = style_handle;
  style_entry.m_name = "Standard";
  style_entry.m_flags = 0;
  style_entry.m_text_height = 0.0;
  style_entry.m_width_factor = 1.0;
  style_entry.m_oblique_angle = 0.0;
  style_entry.m_text_generation_flags = 0;
  style_entry.m_last_height_used = 1.0;
  style_entry.m_primary_font_file_name = "txt";
  style_entry.m_big_font_file_name = "";

  // VIEW can be empty
  auto view_handle = to_hex_string(s_view_table_handle);
  auto& view_table = data->m_view_table;
  view_table.m_handle = view_handle;
  view_table.m_owner_obj = "0";

  // UCS can be empty
  auto ucs_handle = to_hex_string(s_ucs_table_handle);
  auto& ucs_table = data->m_ucs_table;
  ucs_table.m_handle = ucs_handle;
  ucs_table.m_owner_obj = "0";

  // APPID
  auto appid_handle = to_hex_string(s_appid_table_handle);
  auto& appid_table = data->m_appid_table;
  appid_table.m_handle = appid_handle;
  appid_table.m_owner_obj = "0";

  appid_table.m_entries.emplace_back();
  auto& appid_entry = appid_table.m_entries.back();
  appid_entry.m_handle = to_hex_string(s_appid_entry_handle);
  appid_entry.m_owner_obj = appid_handle;
  appid_entry.m_name = "ACAD";
  appid_entry.m_flags = 0;

  // DIMSTYLE
  auto dimstyle_handle = to_hex_string(s_dimstyle_table_handle);
  auto& dimstyle_table = data->m_dimstyle_table;
  dimstyle_table.m_handle = dimstyle_handle;
  dimstyle_table.m_owner_obj = "0";

  dimstyle_table.m_entries.emplace_back();
  auto& dimstyle_entry = dimstyle_table.m_entries.back();
  dimstyle_entry.m_handle = to_hex_string(s_dimstyle_entry_handle);
  dimstyle_entry.m_owner_obj = dimstyle_handle;
  dimstyle_entry.m_name = "Standard";
  dimstyle_entry.m_flags = 0;
  dimstyle_entry.m_dimensioning_scale_factor = 1.0;
  dimstyle_entry.m_dimensioning_arrow_size = 0.18;
  dimstyle_entry.m_dimension_extension_line_offset = 0.0625;
  dimstyle_entry.m_dimension_line_increment = 0.38;
  dimstyle_entry.m_dimension_extension_line_extension = 0.18;
  dimstyle_entry.m_dimension_distance_rounding_value = 0.0;
  dimstyle_entry.m_dimension_line_extension = 0.0;
  dimstyle_entry.m_dimension_plus_tolerance = 0.0;
  dimstyle_entry.m_dimension_minus_tolerance = 0.0;
  dimstyle_entry.m_dimensioning_text_height = 0.18;
  dimstyle_entry.m_center_mark_size = 0.09;
  dimstyle_entry.m_dimensioning_tick_size = 0.0;
  dimstyle_entry.m_alternate_dimensioning_scale_factor = 25.4;
  dimstyle_entry.m_dimension_linear_measurement_scale_factor = 1;
  dimstyle_entry.m_dimension_vertical_text_position = 0;
  dimstyle_entry.m_dimension_unit_tolerance_decimal_places = 1;
  dimstyle_entry.m_dimension_line_gap = 0.09;
  dimstyle_entry.m_alternate_dimensioning_decimal_places = 0;
  dimstyle_entry.m_generate_dimension_tolerances = 0;
  dimstyle_entry.m_generate_dimension_limits = 0;
  dimstyle_entry.m_dimension_text_inside_horizontal = 0;
  dimstyle_entry.m_dimension_text_outside_horizontal = 1;
  dimstyle_entry.m_suppress_first_dimension_extension_line = 0;
  dimstyle_entry.m_suppress_second_dimension_extension_line = 0;
  dimstyle_entry.m_text_above_dimension_line = 0;
  dimstyle_entry.m_dimension_unit_zero_suppression = 0;
  dimstyle_entry.m_dimension_angle_zero_suppression = 0;
  dimstyle_entry.m_alternate_dimensioning_unit_rounding = 0;
  dimstyle_entry.m_use_alternate_dimensioning = 2;
  dimstyle_entry.m_dimension_tolerance_displace_scale_factor = 0;
  dimstyle_entry.m_use_separate_arrow_blocks_for_dimensions = 0;
  dimstyle_entry.m_force_dimension_text_inside_extensions = 0;
  dimstyle_entry.m_suppress_outside_extension_dimension_lines = 0;
  dimstyle_entry.m_dimension_line_color = 0;
  dimstyle_entry.m_dimension_extension_line_color = 0;
  dimstyle_entry.m_dimension_text_color = 0;
  dimstyle_entry.m_angular_dimension_precision = 0;
  dimstyle_entry.m_force_dimension_line_extensions_outside_if_text_exists = 4;
  dimstyle_entry.m_dimension_tolerace_decimal_places = 4;
  dimstyle_entry.m_alternate_dimensioning_units = 2;
  dimstyle_entry.m_alternate_dimensioning_tolerance_decimal_places = 2;
  dimstyle_entry.m_dimensioning_angle_format = 0;
  dimstyle_entry.m_dimension_precision = 0;
  dimstyle_entry.m_dimension_non_angular_units = 2;
  dimstyle_entry.m_dimension_decilam_separator_char = 46;
  dimstyle_entry.m_dimension_text_movement_rule = 0;
  dimstyle_entry.m_dimension_text_justification = 0;
  // dimstyle_entry.DIMSD1 = 0;
  // dimstyle_entry.DIMSD2 = 0;
  dimstyle_entry.m_dimension_tolerance_vertical_justification = 1;
  dimstyle_entry.m_dimension_tolerance_zero_suppression = 0;
  dimstyle_entry.m_alternate_dimensioning_zero_suppression = 0;
  dimstyle_entry.m_alternate_dimensioning_tolerance_zero_suppression = 0;
  dimstyle_entry.m_dimension_cursor_controls_text_position = 0;
  dimstyle_entry.m_dimension_text_and_arrow_placement = 3;
  dimstyle_entry.m_dimension_text_style = "Standard";
  dimstyle_entry.m_dimension_leader_block_name = "";
  dimstyle_entry.m_dimension_line_weight =
    static_cast<int8_t>(Line_weight::BY_BLOCK);
  dimstyle_entry.m_dimension_extension_line_weight =
    static_cast<int8_t>(Line_weight::BY_BLOCK);

  // BLOCK_RECORD
  auto block_record_handle = to_hex_string(s_block_record_table_handle);
  auto& block_record_table = data->m_block_record_table;
  block_record_table.m_handle = block_record_handle;
  block_record_table.m_owner_obj = "0";

  block_record_table.m_entries.emplace_back();
  auto& block_record_entry1 = block_record_table.m_entries.back();
  block_record_entry1.m_handle = to_hex_string(s_block_record_model_space_handle);
  block_record_entry1.m_owner_obj = block_record_handle;
  block_record_entry1.m_name = "*Model_Space";
  block_record_entry1.m_insertion_units = 0;
  block_record_entry1.m_explodability = 1;
  block_record_entry1.m_scalability = 0;

  block_record_table.m_entries.emplace_back();
  auto& block_record_entry2 = block_record_table.m_entries.back();
  block_record_entry2.m_handle = to_hex_string(s_block_record_paper_space_handle);
  block_record_entry2.m_owner_obj = block_record_handle;
  block_record_entry2.m_name = "*Paper_Space";
  block_record_entry2.m_insertion_units = 0;
  block_record_entry2.m_explodability = 1;
  block_record_entry2.m_scalability = 0;

  // BLOCKS
  auto& blocks = data->m_blocks;

  blocks.emplace_back();
  auto& full_block1 = blocks.back();
  auto& block1 = full_block1.first;
  block1.m_handle = to_hex_string(s_block_model_space_handle);
  block1.m_owner_handle = to_hex_string(s_block_record_model_space_handle);;
  block1.m_layer_name = "0";
  block1.m_name = "*Model_Space";
  block1.m_flags = 0;
  block1.m_base_point[0] = 0.0;
  block1.m_base_point[1] = 0.0;
  block1.m_base_point[2] = 0.0;
  block1.m_name = "*Model_Space";
  block1.m_xref_path_name = "";

  auto& edblk1 = full_block1.second;
  edblk1.m_handle = to_hex_string(s_endblk_model_space_handle);
  edblk1.m_owner_handle = to_hex_string(s_block_record_model_space_handle);;
  edblk1.m_layer_name = "0";

  // *PAPER_SPACE
  blocks.emplace_back();
  auto& full_block2 = blocks.back();
  auto& block2 = full_block2.first;
  block2.m_handle = to_hex_string(s_block_paper_space_handle);
  block2.m_owner_handle = to_hex_string(s_block_record_paper_space_handle);;
  block2.m_layer_name = "0";
  block2.m_name = "*Paper_Space";
  block2.m_flags = 0;
  block2.m_base_point[0] = 0.0;
  block2.m_base_point[1] = 0.0;
  block2.m_base_point[2] = 0.0;
  block2.m_name = "*Paper_Space";
  block2.m_xref_path_name = "";

  auto& edblk2 = full_block2.second;
  edblk2.m_handle = to_hex_string(s_endblk_paper_space_handle);
  edblk2.m_owner_handle = to_hex_string(s_block_record_paper_space_handle);;
  edblk2.m_layer_name = "0";

  // ENtitIES can be empty.

  // OBJECTS
  auto& objects = data->m_objects;

  // DICTIONARY, the root dict, one entry named ACAD_GROUP
  auto* dictionary_object1 = new Dxf_dictionary_object;
  objects.push_back(dictionary_object1);
  dictionary_object1->m_duplicate_record_handling = 1;
  dictionary_object1->m_handle = to_hex_string(s_disctionary1_handle);
  dictionary_object1->m_owner_handle = "0";
  dictionary_object1->m_value_handles["ACAD_GROUP"] =
    to_hex_string(s_disctionary2_handle);

  // DICTONARY ACAD_GROUP can be empty
  auto* dictionary_object2 = new Dxf_dictionary_object;
  objects.push_back(dictionary_object2);
  dictionary_object2->m_duplicate_record_handling = 1;
  dictionary_object2->m_handle = to_hex_string(s_disctionary2_handle);
  dictionary_object2->m_owner_handle = to_hex_string(s_disctionary1_handle);
}

DXF_END_NAMESPACE
