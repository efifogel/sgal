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

#ifndef DXF_DATA_HPP
#define DXF_DATA_HPP

#include <vector>
#include <list>
#include <string>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_class.hpp"
#include "dxf/Dxf_block.hpp"
#include "dxf/Dxf_endblk.hpp"
#include "dxf/Dxf_section.hpp"
#include "dxf/Dxf_table.hpp"
#include "dxf/Dxf_appid_entry.hpp"
#include "dxf/Dxf_block_record_entry.hpp"
#include "dxf/Dxf_dimstyle_entry.hpp"
#include "dxf/Dxf_layer_entry.hpp"
#include "dxf/Dxf_ltype_entry.hpp"
#include "dxf/Dxf_style_entry.hpp"
#include "dxf/Dxf_ucs_entry.hpp"
#include "dxf/Dxf_view_entry.hpp"
#include "dxf/Dxf_vport_entry.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_extended_data;
class Dxf_base_entity;
class Dxf_base_object;

struct SGAL_SGAL_DECL Dxf_data {
  /*! Construct.
   */
  Dxf_data();

  /*! Destruct.
   */
  virtual ~Dxf_data();

  /*! Determines whether there are non-empty tables.
   */
  bool tables_empty() const;

  //! Header data
  Dxf_header m_header;

  // Classes
  std::list<Dxf_class> m_classes;

  // Tables
  typedef Dxf_table<Dxf_appid_entry>            Appid_table;
  typedef Dxf_table<Dxf_block_record_entry>     Block_record_table;
  typedef Dxf_table<Dxf_dimstyle_entry>         Dimstyle_table;
  typedef Dxf_table<Dxf_layer_entry>            Layer_table;
  typedef Dxf_table<Dxf_ltype_entry>            Ltype_table;
  typedef Dxf_table<Dxf_style_entry>            Style_table;
  typedef Dxf_table<Dxf_ucs_entry>              Ucs_table;
  typedef Dxf_table<Dxf_view_entry>             View_table;
  typedef Dxf_table<Dxf_vport_entry>            Vport_table;

  Appid_table m_appid_table;
  Block_record_table m_block_record_table;
  Dimstyle_table m_dimstyle_table;
  Layer_table m_layer_table;
  Ltype_table m_ltype_table;
  Style_table m_style_table;
  Ucs_table m_ucs_table;
  View_table m_view_table;
  Vport_table m_vport_table;

  //! Blocks
  std::list<std::pair<Dxf_block, Dxf_endblk> > m_blocks;

  //! Entities
  std::vector<Dxf_base_entity*> m_entities;

  //! Objects
  std::vector<Dxf_base_object*> m_objects;

  //! Thumbnail images
  Dxf_section m_thumbnailimage;

  //! Acds data
  Dxf_section m_acdsdata;

  //! Next available handle.
  size_t m_handle;

  //! Default handles
  static const size_t s_vport_table_handle;
  static const size_t s_vport_entry_handle;
  static const size_t s_ltype_table_handle;
  static const size_t s_ltype_byblock_handle;
  static const size_t s_ltype_bylayer_handle;
  static const size_t s_ltype_continuous_handle;
  static const size_t s_layer_table_handle;
  static const size_t s_layer_entry_handle;
  static const size_t s_style_table_handle;
  static const size_t s_style_entry_handle;
  static const size_t s_view_table_handle;
  static const size_t s_ucs_table_handle;
  static const size_t s_appid_table_handle;
  static const size_t s_appid_entry_handle;
  static const size_t s_dimstyle_table_handle;
  static const size_t s_dimstyle_entry_handle;
  static const size_t s_block_record_table_handle;
  static const size_t s_block_record_model_space_handle;
  static const size_t s_block_record_paper_space_handle;
  static const size_t s_block_model_space_handle;
  static const size_t s_endblk_model_space_handle;
  static const size_t s_block_paper_space_handle;
  static const size_t s_endblk_paper_space_handle;
  static const size_t s_disctionary_handle;
  static const size_t s_disctionary1_handle;
  static const size_t s_disctionary2_handle;

  //! Default layer name
  static const std::string s_def_layer;
};

DXF_END_NAMESPACE

#endif
