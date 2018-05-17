// Copyright (c) 2004,2018 Israel.
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
#include "SGAL/Dxf_style_entry.hpp"
#include "SGAL/Dxf_table.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to handles of members in the LAYER table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 */

typedef Dxf_table<Dxf_style_entry>            Dxf_style_table;

template <>
const std::map<int, Dxf_style_table::Table_entry_member>
Dxf_style_table::s_entry_members = {
  {2, {&Dxf_style_entry::m_name, 1, 0}},
  {70, {&Dxf_style_entry::m_flags, 1, 0}},
  {40, {&Dxf_style_entry::m_text_height, 1, 0}},
  {41, {&Dxf_style_entry::m_width_factor, 1, 0}},
  {50, {&Dxf_style_entry::m_oblique_angle, 1, 0}},
  {71, {&Dxf_style_entry::m_text_generation_flags, 1, 0}},
  {42, {&Dxf_style_entry::m_last_height_used, 1, 0}},
  {3, {&Dxf_style_entry::m_primary_font_file_name, 1, 0}},
  {4, {&Dxf_style_entry::m_big_font_file_name, 1, 0}},
  {1071, {&Dxf_style_entry::m_font_flags, 1, 0}}
};

SGAL_END_NAMESPACE
