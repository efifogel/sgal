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

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_base_entity.hpp"
#include "dxf/Dxf_base_object.hpp"

DXF_BEGIN_NAMESPACE

//! Default handles
const size_t Dxf_data::s_vport_table_handle(0x8);
const size_t Dxf_data::s_vport_entry_handle(0x29);
const size_t Dxf_data::s_ltype_table_handle(0x5);
const size_t Dxf_data::s_ltype_byblock_handle(0x14);
const size_t Dxf_data::s_ltype_bylayer_handle(0x15);
const size_t Dxf_data::s_ltype_continuous_handle(0x16);
const size_t Dxf_data::s_layer_table_handle(0x2);
const size_t Dxf_data::s_layer_entry_handle(0x10);
const size_t Dxf_data::s_style_table_handle(0x3);
const size_t Dxf_data::s_style_entry_handle(0x11);
const size_t Dxf_data::s_view_table_handle(0x6);
const size_t Dxf_data::s_ucs_table_handle(0x7);
const size_t Dxf_data::s_appid_table_handle(0x9);
const size_t Dxf_data::s_appid_entry_handle(0x12);
const size_t Dxf_data::s_dimstyle_table_handle(0xA);
const size_t Dxf_data::s_dimstyle_entry_handle(0x27);
const size_t Dxf_data::s_block_record_table_handle(0x1);
const size_t Dxf_data::s_block_record_model_space_handle(0x1F);
const size_t Dxf_data::s_block_record_paper_space_handle(0x1B);
const size_t Dxf_data::s_block_model_space_handle(0x20);
const size_t Dxf_data::s_endblk_model_space_handle(0x21);
const size_t Dxf_data::s_block_paper_space_handle(0x1C);
const size_t Dxf_data::s_endblk_paper_space_handle(0x1D);
const size_t Dxf_data::s_disctionary1_handle(0xC);
const size_t Dxf_data::s_disctionary2_handle(0xD);

//! Default layer name
const std::string Dxf_data::s_def_layer("0");

//! \brief constructs
Dxf_data::Dxf_data() :
  m_handle(100)
{}

//! \brief destructs.
Dxf_data::~Dxf_data()
{
  // Clear header
  m_header.clear();

  // Clear classes
  m_classes.clear();

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

  // Clear blocks
  m_blocks.clear();

  // Clear entities
  for (auto* entity : m_entities) delete entity;
  m_entities.clear();

  // Clear objects
  for (auto* object : m_objects) delete object;
  m_objects.clear();
}

//! \brief determiness whether there are any tables.
bool Dxf_data::tables_empty() const
{
  return (m_appid_table.empty() &&
          m_block_record_table.empty() &&
          m_dimstyle_table.empty() &&
          m_layer_table.empty() &&
          m_ltype_table.empty() &&
          m_style_table.empty() &&
          m_ucs_table.empty() &&
          m_view_table.empty() &&
          m_vport_table.empty());
}

DXF_END_NAMESPACE
