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
#include "dxf/Dxf_text_entity.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_text_entity>  Dxf_text_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_text_entity_wrapper::Record_member>
Dxf_text_entity_wrapper::s_record_members = {
  {39, {&Dxf_text_entity::m_thickness, 1, 0}},
  {10, {&Dxf_text_entity::m_location, 3, 0}},
  {20, {&Dxf_text_entity::m_location, 3, 1}},
  {30, {&Dxf_text_entity::m_location, 3, 2}},
  {40, {&Dxf_text_entity::m_text_height, 1, 0}},
  {1, {&Dxf_text_entity::m_value, 1, 0}},
  {50, {&Dxf_text_entity::m_rotation, 1, 0}},
  {41, {&Dxf_text_entity::m_relative_x_scale_factor, 1, 0}},
  {51, {&Dxf_text_entity::m_oblique_angle, 1, 0}},
  {7, {&Dxf_text_entity::m_text_style_name, 1, 0}},
  {71, {&Dxf_text_entity::m_text_generation_flags, 1, 0}},
  {72, {&Dxf_text_entity::m_horizontal_text_justification, 1, 0}},
  {11, {&Dxf_text_entity::m_second_alignment_point, 3, 0}},
  {21, {&Dxf_text_entity::m_second_alignment_point, 3, 1}},
  {31, {&Dxf_text_entity::m_second_alignment_point, 3, 2}},
  {210, {&Dxf_text_entity::m_normal, 3, 0}},
  {220, {&Dxf_text_entity::m_normal, 3, 1}},
  {230, {&Dxf_text_entity::m_normal, 3, 2}},
  {73, {&Dxf_text_entity::m_vertical_text_justification, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_text_entity_wrapper::Record_handler_type>
Dxf_text_entity_wrapper::s_record_handlers = {};

DXF_END_NAMESPACE
