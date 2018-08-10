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

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_class.hpp"
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
#include "dxf/Dxf_block.hpp"

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

  // Blocks
  std::list<Dxf_block> m_blocks;

  // Entities
  std::vector<Dxf_base_entity*> m_entities;

  // Objects
  std::vector<Dxf_base_object*> m_objects;
};

DXF_END_NAMESPACE

#endif
