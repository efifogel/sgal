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

//! \brief constructs
Dxf_data::Dxf_data() {}

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
