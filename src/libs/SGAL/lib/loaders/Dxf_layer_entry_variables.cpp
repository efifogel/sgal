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
#include "SGAL/Dxf_layer_entry.hpp"
#include "SGAL/Dxf_table.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to the variable handle in the VPORT table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 *
 * Unfortunately, not all code numbers agree with specifications found
 * elsewhereat, e.g., https://www.autodesk.com/techpubs/autocad/acad2000/dxf/.
 * 45 => 40 (view height)
 * new: 41, 65, 73, 75, 76, 77, 78
 */

typedef Dxf_table<Dxf_layer_entry>            Dxf_layer_table;

template <>
const std::map<int, Dxf_layer_table::Table_entry_variable>
Dxf_layer_table::s_entry_variables = {
  {100, {&Dxf_layer_entry::m_marker, 1, 0}},
  {2, {&Dxf_layer_entry::m_name, 1, 0}},
  {70, {&Dxf_layer_entry::m_flags, 1, 0}},
  {62, {&Dxf_layer_entry::m_color, 1, 0}},
  {6, {&Dxf_layer_entry::m_line_type, 1, 0}},
  {290, {&Dxf_layer_entry::m_is_layer_plotted, 1, 0}},
  {370, {&Dxf_layer_entry::m_line_weight, 1, 0}},
  {390, {&Dxf_layer_entry::m_plot_style_pointer, 1, 0}},
  {347, {&Dxf_layer_entry::m_material_handle, 1, 0}}
};

SGAL_END_NAMESPACE
