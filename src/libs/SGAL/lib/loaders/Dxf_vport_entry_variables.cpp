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
#include "SGAL/Dxf_parser.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to the variable handle in the VPORT table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 */

const std::map<int, Dxf_parser::Vport_entry_variable_type>
Dxf_parser::s_vport_entry_variables = {
  {100, &Dxf_vport_table::m_marker},
  {2, &Dxf_vport_table::m_name},
  {70, &Dxf_vport_table::m_flags},
//   {10, &Dxf_vport_table::m_lower_left[0]},
//   {20, &Dxf_vport_table::m_lower_left[1]},
//   {11, &Dxf_vport_table::m_upper_right[0]},
//   {21, &Dxf_vport_table::m_upper_right[1]},
//   {12, &Dxf_vport_table::m_view_center[0]},
//   {22, &Dxf_vport_table::m_view_center[1]},
//   {13, &Dxf_vport_table::m_snap_base[0]},
//   {23, &Dxf_vport_table::m_snap_base[1]},
//   {14, &Dxf_vport_table::m_snap_spacing[0]},
//   {24, &Dxf_vport_table::m_snap_spacing[1]},
//   {15, &Dxf_vport_table::m_grid_spacing[0]},
//   {25, &Dxf_vport_table::m_grid_spacing[1]},
//   {16, &Dxf_vport_table::m_view_direction[0]},
//   {26, &Dxf_vport_table::m_view_direction[1]},
//   {36, &Dxf_vport_table::m_view_direction[2]},
//   {17, &Dxf_vport_table::m_view_target[0]},
//   {27, &Dxf_vport_table::m_view_target[1]},
//   {37, &Dxf_vport_table::m_view_target[2]},
  {42, &Dxf_vport_table::m_lens_length},
  {43, &Dxf_vport_table::m_front_clipping_plane},
  {44, &Dxf_vport_table::m_back_clipping_plane},
  {45, &Dxf_vport_table::m_view_height},
  {50, &Dxf_vport_table::m_snap_rotation_angle},
  {51, &Dxf_vport_table::m_view_twist_angle},
  {72, &Dxf_vport_table::m_circle_sides},
  {331, &Dxf_vport_table::m_soft_frozen_layer},
  {341, &Dxf_vport_table::m_hard_frozen_layer},
  {70, &Dxf_vport_table::m_perspective_mode},
  {1, &Dxf_vport_table::m_plot_style_sheet},
  {281, &Dxf_vport_table::m_render_mode},
  {71, &Dxf_vport_table::m_view_mode},
  {74, &Dxf_vport_table::m_ucs_icon},
//   {110, &Dxf_vport_table::m_ucs_origin[0]},
//   {120, &Dxf_vport_table::m_ucs_origin[1]},
//   {130, &Dxf_vport_table::m_ucs_origin[2]},
//   {111, &Dxf_vport_table::m_ucs_x_axis[0]},
//   {121, &Dxf_vport_table::m_ucs_x_axis[1]},
//   {131, &Dxf_vport_table::m_ucs_x_axis[2]},
//   {112, &Dxf_vport_table::m_ucs_y_axis[0]},
//   {122, &Dxf_vport_table::m_ucs_y_axis[1]},
//   {132, &Dxf_vport_table::m_ucs_y_axis[2]},
  {345, &Dxf_vport_table::m_ucs_handle},
  {346, &Dxf_vport_table::m_base_ucs_handle},
  {79, &Dxf_vport_table::m_orthographic_type},
  {146, &Dxf_vport_table::m_elevation},
  {170, &Dxf_vport_table::m_shade_plot_setting},
  {61, &Dxf_vport_table::m_major_grid_lines},
  {332, &Dxf_vport_table::m_background_object_pointer},
  {333, &Dxf_vport_table::m_shade_plot_object_pointer},
  {348, &Dxf_vport_table::m_visual_style_object_pointer},
  {292, &Dxf_vport_table::m_is_default_lighting_on},
  {282, &Dxf_vport_table::m_default_lighting_type},
  {141, &Dxf_vport_table::m_brightness},
  {142, &Dxf_vport_table::m_contrast},
  {63, &Dxf_vport_table::m_ambient_color},
  {421, &Dxf_vport_table::m_ambient_color_i32},
  {31, &Dxf_vport_table::m_ambient_color_name}
};

SGAL_END_NAMESPACE
