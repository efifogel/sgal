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

#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_block_record_entry.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to handles of members in the LAYER table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 *
 * The code 1070 Appears twice in the spec. The second appearance is ignored.
 */

typedef Dxf_record_wrapper<Dxf_block_record_entry>      Dxf_block_record_wrapper;

template <>
const std::map<int, Dxf_block_record_wrapper::Record_member>
Dxf_block_record_wrapper::s_record_members = {
  {2, {&Dxf_block_record_entry::m_name, 1, 0}},
  {340, {&Dxf_block_record_entry::m_layout_handle, 1, 0}},
  {70, {&Dxf_block_record_entry::m_insertion_units, 1, 0}},
  {280, {&Dxf_block_record_entry::m_explodability, 1, 0}},
  {281, {&Dxf_block_record_entry::m_scalability, 1, 0}},
  {310, {&Dxf_block_record_entry::m_bitmap_preview_data, 1, 0}},
  {1070, {&Dxf_block_record_entry::m_insert_units, 1, 0}},
  // {1070, {&Dxf_block_record_entry::m_m_design_center_version_number, 1, 0}},
  {1001, {&Dxf_block_record_entry::m_application_name, 1, 0}},
  {1000, {&Dxf_block_record_entry::m_string_data, 1, 0}},
  // {1002, {&Dxf_block_record_entry::m_xdata, n, i}},
};

SGAL_END_NAMESPACE
