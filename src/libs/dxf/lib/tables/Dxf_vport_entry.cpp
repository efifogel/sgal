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

#include "dxf/basic.hpp"
#include "dxf/Dxf_vport_entry.hpp"
#include "dxf/Dxf_record_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to handles of members in the VPORT table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 *
 * Unfortunately, not all code numbers agree with specifications found
 * elsewhereat, e.g., https://www.autodesk.com/techpubs/autocad/acad2000/dxf/.
 * 45 => 40 (view height)
 * new: 41, 65, 73, 75, 76, 77, 78
 */

typedef Dxf_record_wrapper<Dxf_vport_entry>             Dxf_vport_wrapper;

//!
template <>
const std::map<int, Dxf_vport_wrapper::Record_member>
Dxf_vport_wrapper::s_record_members = {
  {2, {&Dxf_vport_entry::m_name, 1, 0}},
  {70, {&Dxf_vport_entry::m_flags, 1, 0}},
  {10, {&Dxf_vport_entry::m_lower_left, 2, 0}},
  {20, {&Dxf_vport_entry::m_lower_left, 2, 1}},
  {11, {&Dxf_vport_entry::m_upper_right, 2, 0}},
  {21, {&Dxf_vport_entry::m_upper_right, 2, 1}},
  {12, {&Dxf_vport_entry::m_view_center, 2, 0}},
  {22, {&Dxf_vport_entry::m_view_center, 2, 1}},
  {13, {&Dxf_vport_entry::m_snap_base, 2, 0}},
  {23, {&Dxf_vport_entry::m_snap_base, 2, 1}},
  {14, {&Dxf_vport_entry::m_snap_spacing, 2, 0}},
  {24, {&Dxf_vport_entry::m_snap_spacing, 2, 1}},
  {15, {&Dxf_vport_entry::m_grid_spacing, 2, 0}},
  {25, {&Dxf_vport_entry::m_grid_spacing, 2, 1}},
  {16, {&Dxf_vport_entry::m_view_direction, 3, 0}},
  {26, {&Dxf_vport_entry::m_view_direction, 3, 1}},
  {36, {&Dxf_vport_entry::m_view_direction, 3, 2}},
  {17, {&Dxf_vport_entry::m_view_target, 3, 0}},
  {27, {&Dxf_vport_entry::m_view_target, 3, 1}},
  {37, {&Dxf_vport_entry::m_view_target, 3, 2}},
  {40, {&Dxf_vport_entry::m_view_height, 1, 0}},  // see 45 and comment above
  {41, {&Dxf_vport_entry::m_aspect_ratio, 1, 0}}, // see comment above
  {42, {&Dxf_vport_entry::m_lens_length, 1, 0}},
  {43, {&Dxf_vport_entry::m_front_clipping_plane, 1, 0}},
  {44, {&Dxf_vport_entry::m_back_clipping_plane, 1, 0}},
  {45, {&Dxf_vport_entry::m_view_height, 1, 0}},
  {50, {&Dxf_vport_entry::m_snap_rotation_angle, 1, 0}},
  {51, {&Dxf_vport_entry::m_view_twist_angle, 1, 0}},
  {331, {&Dxf_vport_entry::m_soft_frozen_layer, 1, 0}},
  {341, {&Dxf_vport_entry::m_hard_frozen_layer, 1, 0}},
  {1, {&Dxf_vport_entry::m_plot_style_sheet, 1, 0}},
  {71, {&Dxf_vport_entry::m_view_mode, 1, 0}},
  {72, {&Dxf_vport_entry::m_circle_sides, 1, 0}},
  {73, {&Dxf_vport_entry::m_fast_zoom, 1, 0}},
  {74, {&Dxf_vport_entry::m_ucs_icon, 1, 0}},
  {75, {&Dxf_vport_entry::m_snap_on, 1, 0}},
  {76, {&Dxf_vport_entry::m_grid_on, 1, 0}},
  {77, {&Dxf_vport_entry::m_snap_style, 1, 0}},
  {78, {&Dxf_vport_entry::m_snap_isopair, 1, 0}},
  {281, {&Dxf_vport_entry::m_render_mode, 1, 0}},
  {65, {&Dxf_vport_entry::m_ucs_up, 1, 0}},
  {110, {&Dxf_vport_entry::m_ucs_origin, 3, 0}},
  {120, {&Dxf_vport_entry::m_ucs_origin, 3, 1}},
  {130, {&Dxf_vport_entry::m_ucs_origin, 3, 2}},
  {111, {&Dxf_vport_entry::m_ucs_x_axis, 3, 0}},
  {121, {&Dxf_vport_entry::m_ucs_x_axis, 3, 1}},
  {131, {&Dxf_vport_entry::m_ucs_x_axis, 3, 2}},
  {112, {&Dxf_vport_entry::m_ucs_y_axis, 3, 0}},
  {122, {&Dxf_vport_entry::m_ucs_y_axis, 3, 1}},
  {132, {&Dxf_vport_entry::m_ucs_y_axis, 3, 2}},
  {345, {&Dxf_vport_entry::m_ucs_handle, 1, 0}},
  {346, {&Dxf_vport_entry::m_base_ucs_handle, 1, 0}},
  {79, {&Dxf_vport_entry::m_orthographic_type, 1, 0}},
  {146, {&Dxf_vport_entry::m_elevation, 1, 0}},
  {170, {&Dxf_vport_entry::m_shade_plot_setting, 1, 0}},
  {61, {&Dxf_vport_entry::m_major_grid_lines, 1, 0}},
  {332, {&Dxf_vport_entry::m_background_object_pointer, 1, 0}},
  {333, {&Dxf_vport_entry::m_shade_plot_object_pointer, 1, 0}},
  {348, {&Dxf_vport_entry::m_visual_style_object_pointer, 1, 0}},
  {292, {&Dxf_vport_entry::m_is_default_lighting_on, 1, 0}},
  {282, {&Dxf_vport_entry::m_default_lighting_type, 1, 0}},
  {141, {&Dxf_vport_entry::m_brightness, 1, 0}},
  {142, {&Dxf_vport_entry::m_contrast, 1, 0}},
  {63, {&Dxf_vport_entry::m_ambient_color, 1, 0}},
  {421, {&Dxf_vport_entry::m_ambient_color_i32, 1, 0}},
  {31, {&Dxf_vport_entry::m_ambient_color_name, 1, 0}}
};

//!
template <>
const std::map<int, Dxf_vport_wrapper::Record_handler_type>
Dxf_vport_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
