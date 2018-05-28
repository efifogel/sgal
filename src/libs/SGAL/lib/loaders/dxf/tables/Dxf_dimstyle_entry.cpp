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

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_dimstyle_entry.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_dimstyle_entry>          Dxf_dimstyle_wrapper;

template <>
const std::map<int, Dxf_dimstyle_wrapper::Record_member>
Dxf_dimstyle_wrapper::s_record_members = {
  {2, {&Dxf_dimstyle_entry::m_name, 1, 0}},
  {70, {&Dxf_dimstyle_entry::m_flags, 1, 0}},
  {3, {&Dxf_dimstyle_entry::m_dimpost, 1, 0}},
  {4, {&Dxf_dimstyle_entry::m_dimapost, 1, 0}},
  {5, {&Dxf_dimstyle_entry::m_object_id, 1, 0}},
  {6, {&Dxf_dimstyle_entry::m_object_id1, 1, 0}},
  {7, {&Dxf_dimstyle_entry::m_object_id2, 1, 0}},
  {40, {&Dxf_dimstyle_entry::m_dimensioning_scale_factor, 1, 0}},
  {41, {&Dxf_dimstyle_entry::m_dimensioning_arrow_size, 1, 0}},
  {42, {&Dxf_dimstyle_entry::m_dimension_extension_line_offset, 1, 0}},
  {43, {&Dxf_dimstyle_entry::m_dimension_line_increment, 1, 0}},
  {44, {&Dxf_dimstyle_entry::m_dimension_extension_line_extension, 1, 0}},
  {45, {&Dxf_dimstyle_entry::m_dimension_distance_rounding_value, 1, 0}},
  {46, {&Dxf_dimstyle_entry::m_dimension_line_extension, 1, 0}},
  {47, {&Dxf_dimstyle_entry::m_dimension_plus_tolerance, 1, 0}},
  {48, {&Dxf_dimstyle_entry::m_dimension_minus_tolerance, 1, 0}},
  {140, {&Dxf_dimstyle_entry::m_dimensioning_text_height, 1, 0}},
  {141, {&Dxf_dimstyle_entry::m_center_mark_size, 1, 0}},
  {142, {&Dxf_dimstyle_entry::m_dimensioning_tick_size, 1, 0}},
  {143, {&Dxf_dimstyle_entry::m_alternate_dimensioning_scale_factor, 1, 0}},
  {144, {&Dxf_dimstyle_entry::m_dimension_linear_measurement_scale_factor, 1, 0}},
  {145, {&Dxf_dimstyle_entry::m_dimension_vertical_text_position, 1, 0}},
  {146, {&Dxf_dimstyle_entry::m_dimension_unit_tolerance_decimal_places, 1, 0}},
  {147, {&Dxf_dimstyle_entry::m_dimension_line_gap, 1, 0}},
  {148, {&Dxf_dimstyle_entry::m_alternate_dimensioning_decimal_places, 1, 0}},
  {71, {&Dxf_dimstyle_entry::m_generate_dimension_tolerances, 1, 0}},
  {72, {&Dxf_dimstyle_entry::m_generate_dimension_limits, 1, 0}},
  {73, {&Dxf_dimstyle_entry::m_dimension_text_inside_horizontal, 1, 0}},
  {74, {&Dxf_dimstyle_entry::m_dimension_text_outside_horizontal, 1, 0}},
  {75, {&Dxf_dimstyle_entry::m_suppress_first_dimension_extension_line, 1, 0}},
  {76, {&Dxf_dimstyle_entry::m_suppress_second_dimension_extension_line, 1, 0}},
  {77, {&Dxf_dimstyle_entry::m_text_above_dimension_line, 1, 0}},
  {78, {&Dxf_dimstyle_entry::m_dimension_unit_zero_suppression, 1, 0}},
  {79, {&Dxf_dimstyle_entry::m_dimension_angle_zero_suppression, 1, 0}},
  {170, {&Dxf_dimstyle_entry::m_alternate_dimensioning_unit_rounding, 1, 0}},
  {171, {&Dxf_dimstyle_entry::m_use_alternate_dimensioning, 1, 0}},
  {172, {&Dxf_dimstyle_entry::m_dimension_tolerance_displace_scale_factor, 1, 0}},
  {173, {&Dxf_dimstyle_entry::m_use_separate_arrow_blocks_for_dimensions, 1, 0}},
  {174, {&Dxf_dimstyle_entry::m_force_dimension_text_inside_extensions, 1, 0}},
  {175, {&Dxf_dimstyle_entry::m_suppress_outside_extension_dimension_lines, 1, 0}},
  {176, {&Dxf_dimstyle_entry::m_dimension_line_color, 1, 0}},
  {177, {&Dxf_dimstyle_entry::m_dimension_extension_line_color, 1, 0}},
  {178, {&Dxf_dimstyle_entry::m_dimension_text_color, 1, 0}},
  {179, {&Dxf_dimstyle_entry::m_angular_dimension_precision, 1, 0}},
  {270, {&Dxf_dimstyle_entry::m_dimension_unit_format, 1, 0}},
  {271, {&Dxf_dimstyle_entry::m_force_dimension_line_extensions_outside_if_text_exists, 1, 0}},
  {272, {&Dxf_dimstyle_entry::m_dimension_tolerace_decimal_places, 1, 0}},
  {273, {&Dxf_dimstyle_entry::m_alternate_dimensioning_units, 1, 0}},
  {274, {&Dxf_dimstyle_entry::m_alternate_dimensioning_tolerance_decimal_places, 1, 0}},
  {275, {&Dxf_dimstyle_entry::m_dimensioning_angle_format, 1, 0}},
  {276, {&Dxf_dimstyle_entry::m_dimension_precision, 1, 0}},
  {277, {&Dxf_dimstyle_entry::m_dimension_non_angular_units, 1, 0}},
  {278, {&Dxf_dimstyle_entry::m_dimension_decilam_separator_char, 1, 0}},
  {279, {&Dxf_dimstyle_entry::m_dimension_text_movement_rule, 1, 0}},
  {280, {&Dxf_dimstyle_entry::m_dimension_text_justification, 1, 0}},
  // 281 DIMSD1
  // 282 DIMSD2
  {283, {&Dxf_dimstyle_entry::m_dimension_tolerance_vertical_justification, 1, 0}},
  {284, {&Dxf_dimstyle_entry::m_dimension_tolerance_zero_suppression, 1, 0}},
  {285, {&Dxf_dimstyle_entry::m_alternate_dimensioning_zero_suppression, 1, 0}},
  {286, {&Dxf_dimstyle_entry::m_alternate_dimensioning_tolerance_zero_suppression, 1, 0}},
  // 287 DIMFIT (obsolete, now use DIMATFIT and DIMTMOVE)
  {288, {&Dxf_dimstyle_entry::m_dimension_cursor_controls_text_position, 1, 0}},
  {289, {&Dxf_dimstyle_entry::m_dimension_text_and_arrow_placement, 1, 0}},
  {340, {&Dxf_dimstyle_entry::m_dimension_text_style, 1, 0}},
  {341, {&Dxf_dimstyle_entry::m_dimension_leader_block_name, 1, 0}},
  {342, {&Dxf_dimstyle_entry::m_arrow_block_name, 1, 0}},
  // 343 DIMBLK1 (handle of referenced BLOCK)
  // 344 DIMBLK2 (handle of referenced BLOCK)
  {371, {&Dxf_dimstyle_entry::m_dimension_line_weight, 1, 0}},
  {372, {&Dxf_dimstyle_entry::m_dimension_extension_line_weight, 1, 0}},
};

SGAL_END_NAMESPACE
