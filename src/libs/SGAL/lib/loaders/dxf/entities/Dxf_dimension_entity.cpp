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
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_dimension_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_dimension_entity>  Dxf_dimension_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_dimension_entity_wrapper::Record_member>
Dxf_dimension_entity_wrapper::s_record_members = {
  {280, {&Dxf_dimension_entity::m_version, 1, 0}},
  {2, {&Dxf_dimension_entity::m_block_name, 1, 0}},
  {10, {&Dxf_dimension_entity::m_definition_point_1, 3, 0}},
  {20, {&Dxf_dimension_entity::m_definition_point_1, 3, 1}},
  {30, {&Dxf_dimension_entity::m_definition_point_1, 3, 2}},
  {11, {&Dxf_dimension_entity::m_text_mid_point, 3, 0}},
  {21, {&Dxf_dimension_entity::m_text_mid_point, 3, 1}},
  {31, {&Dxf_dimension_entity::m_text_mid_point, 3, 2}},
  {70, {&Dxf_dimension_entity::m_dimension_type, 1, 0}},
  {71, {&Dxf_dimension_entity::m_attachment_point, 1, 0}},
  {72, {&Dxf_dimension_entity::m_text_line_spacing_style, 1, 0}},
  {41, {&Dxf_dimension_entity::m_text_line_spacing_factor, 1, 0}},
  {42, {&Dxf_dimension_entity::m_actual_measurement, 1, 0}},
  {1, {&Dxf_dimension_entity::m_text, 1, 0}},
  {53, {&Dxf_dimension_entity::m_text_rotation_angle, 1, 0}},
  {51, {&Dxf_dimension_entity::m_horizontal_direction_angle, 1, 0}},
  {210, {&Dxf_dimension_entity::m_normal, 3, 0}},
  {220, {&Dxf_dimension_entity::m_normal, 3, 1}},
  {230, {&Dxf_dimension_entity::m_normal, 3, 2}},
  {3, {&Dxf_dimension_entity::m_dimension_style_name, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_dimension_entity_wrapper::Record_handler_type>
Dxf_dimension_entity_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
