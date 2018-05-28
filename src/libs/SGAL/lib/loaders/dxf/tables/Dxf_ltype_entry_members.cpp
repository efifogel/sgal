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
#include "SGAL/Dxf_ltype_entry.hpp"
#include "SGAL/Dxf_table.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF LTYPE table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to handles of members in the LTYPE table struct.
 * The DXF LTYPE table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 */

typedef Dxf_table<Dxf_ltype_entry>            Dxf_ltype_table;

template <>
const std::map<int, Dxf_ltype_table::Table_entry_member>
Dxf_ltype_table::s_entry_members = {
  {2, {&Dxf_ltype_entry::m_name, 1, 0}},
  {70, {&Dxf_ltype_entry::m_flags, 1, 0}},
  {3, {&Dxf_ltype_entry::m_description, 1, 0}},
  {72, {&Dxf_ltype_entry::m_alignment_code, 1, 0}},
  {73, {&Dxf_ltype_entry::m_element_count, 1, 0}},
  {40, {&Dxf_ltype_entry::m_total_pattern_length, 1, 0}},
  {49, {&Dxf_ltype_entry::m_dash_dot_space_lengths, 1, 0}},
  {74, {&Dxf_ltype_entry::m_complex_line_type_element_types, 1, 0}},
  {75, {&Dxf_ltype_entry::m_shape_numbers, 1, 0}},
  {340, {&Dxf_ltype_entry::m_style_pointers, 1, 0}},
  // {46, {&Dxf_ltype_entry::m_scale_values, n, x}},
  {50, {&Dxf_ltype_entry::m_rotation_angles, 1, 0}}
  // {44, {&Dxf_ltype_entry::m_offsets, n, i, 0}},
  // {45, {&Dxf_ltype_entry::m_offsets, n, i, 1}},
  // {9, {&Dxf_ltype_entry::m_text_strings, n, 0}},
};

SGAL_END_NAMESPACE
