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

#include "dxf/basic.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_mtext_entity.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_mtext_entity>  Dxf_mtext_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_mtext_entity_wrapper::Record_member>
Dxf_mtext_entity_wrapper::s_record_members = {
  {10, {&Dxf_mtext_entity::m_insertion_point, 3, 0}},
  {20, {&Dxf_mtext_entity::m_insertion_point, 3, 1}},
  {30, {&Dxf_mtext_entity::m_insertion_point, 3, 2}},
  {40, {&Dxf_mtext_entity::m_initial_text_height, 1, 0}},
  {41, {&Dxf_mtext_entity::m_reference_rectangle_width, 1, 0}},
  {71, {&Dxf_mtext_entity::m_attachment_point, 1, 0}},
  {72, {&Dxf_mtext_entity::m_Ddrawing_direction, 1, 0}},
  {1, {&Dxf_mtext_entity::m_test, 1, 0}},
  {3, {&Dxf_mtext_entity::m_extended_text, 1, 0}},
  {7, {&Dxf_mtext_entity::m_text_style_name, 1, 0}},
  {210, {&Dxf_mtext_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_mtext_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_mtext_entity::m_extrusion_direction, 3, 2}},
  {11, {&Dxf_mtext_entity::m_x_axis_direction, 3, 0}},
  {21, {&Dxf_mtext_entity::m_x_axis_direction, 3, 1}},
  {31, {&Dxf_mtext_entity::m_x_axis_direction, 3, 2}},
  {42, {&Dxf_mtext_entity::m_horizontal_width, 1, 0}},
  {43, {&Dxf_mtext_entity::m_vertical_height, 1, 0}},
  {50, {&Dxf_mtext_entity::m_rotation_angle, 1, 0}},
  {73, {&Dxf_mtext_entity::m_line_spacing_style, 1, 0}},
  {44, {&Dxf_mtext_entity::m_line_spacing_factor, 1, 0}},
  {90, {&Dxf_mtext_entity::m_background_fill_setting, 1, 0}},
  {420, {&Dxf_mtext_entity::m_background_color_rgb, 1, 0}},
  {430, {&Dxf_mtext_entity::m_background_color_name, 1, 0}},
  {45, {&Dxf_mtext_entity::m_fill_box_scale, 1, 0}},
  {63, {&Dxf_mtext_entity::m_background_fill_color, 1, 0}},
  {441, {&Dxf_mtext_entity::m_background_fill_color_transparency, 1, 0}},
  {75, {&Dxf_mtext_entity::m_column_type, 1, 0}},
  {76, {&Dxf_mtext_entity::m_column_count, 1, 0}},
  {78, {&Dxf_mtext_entity::m_is_column_flow_reversed, 1, 0}},
  {79, {&Dxf_mtext_entity::m_is_column_auto_height, 1, 0}},
  {48, {&Dxf_mtext_entity::m_column_width, 1, 0}},
  {49, {&Dxf_mtext_entity::m_column_gutter, 1, 0}},
  {50, {&Dxf_mtext_entity::m_column_heights, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_mtext_entity_wrapper::Record_handler_type>
Dxf_mtext_entity_wrapper::s_record_handlers = {};

DXF_END_NAMESPACE
