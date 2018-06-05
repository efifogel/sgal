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
#include "SGAL/Dxf_view_entry.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF VPORT table variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to handles of members in the LAYER table struct.
 * The DXF VPORT table variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 */

typedef Dxf_record_wrapper<Dxf_view_entry>              Dxf_view_wrapper;

template <>
const std::map<int, Dxf_view_wrapper::Record_member>
Dxf_view_wrapper::s_record_members = {
  {2, {&Dxf_view_entry::m_name, 1, 0}},
  {70, {&Dxf_view_entry::m_flags, 1, 0}},
  {40, {&Dxf_view_entry::m_view_height, 1, 0}},
  {10, {&Dxf_view_entry::m_view_center_point, 2, 0}},
  {20, {&Dxf_view_entry::m_view_center_point, 2, 1}},
  {41, {&Dxf_view_entry::m_view_width, 1, 0}},
  {11, {&Dxf_view_entry::m_view_direction, 3, 0}},
  {21, {&Dxf_view_entry::m_view_direction, 3, 1}},
  {31, {&Dxf_view_entry::m_view_direction, 3, 2}},
  {12, {&Dxf_view_entry::m_target_point, 3, 0}},
  {22, {&Dxf_view_entry::m_target_point, 3, 1}},
  {32, {&Dxf_view_entry::m_target_point, 3, 2}},
  {42, {&Dxf_view_entry::m_lens_length, 1, 0}},
  {43, {&Dxf_view_entry::m_front_clipping_plane, 1, 0}},
  {44, {&Dxf_view_entry::m_back_clipping_plane, 1, 0}},
  {50, {&Dxf_view_entry::m_twist_angle, 1, 0}},
  {71, {&Dxf_view_entry::m_view_mode, 1, 0}},
  {281, {&Dxf_view_entry::m_render_mode, 1, 0}},
  {72, {&Dxf_view_entry::m_is_associated_ucs_present, 1, 0}},
  {73, {&Dxf_view_entry::m_is_camera_plottable, 1, 0}},
  {332, {&Dxf_view_entry::m_background_object_pointer, 1, 0}},
  {334, {&Dxf_view_entry::m_section_object_pointer, 1, 0}},
  {348, {&Dxf_view_entry::m_visual_style_object_pointer, 1, 0}},
  {361, {&Dxf_view_entry::m_sun_ownership_pointer, 1, 0}},
  {110, {&Dxf_view_entry::m_ucs_origin, 3, 0}},
  {120, {&Dxf_view_entry::m_ucs_origin, 3, 1}},
  {130, {&Dxf_view_entry::m_ucs_origin, 3, 2}},
  {111, {&Dxf_view_entry::m_ucs_x_axis, 3, 0}},
  {121, {&Dxf_view_entry::m_ucs_x_axis, 3, 1}},
  {131, {&Dxf_view_entry::m_ucs_x_axis, 3, 2}},
  {112, {&Dxf_view_entry::m_ucs_y_axis, 3, 0}},
  {122, {&Dxf_view_entry::m_ucs_y_axis, 3, 1}},
  {132, {&Dxf_view_entry::m_ucs_y_axis, 3, 2}},
  {79, {&Dxf_view_entry::m_orthographic_view_type, 1, 0}},
  {146, {&Dxf_view_entry::m_ucs_elevation, 1, 0}},
  {345, {&Dxf_view_entry::m_ucs_handle, 1, 0}},
  {346, {&Dxf_view_entry::m_base_ucs_handle, 1, 0}}
};

SGAL_END_NAMESPACE
