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
#include "dxf/Dxf_insert_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_insert_entity>  Dxf_insert_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_insert_entity_wrapper::Record_member>
Dxf_insert_entity_wrapper::s_record_members = {
  {66, {&Dxf_insert_entity::m_has_attributes, 1, 0}},
  {2, {&Dxf_insert_entity::m_name, 1, 0}},
  {10, {&Dxf_insert_entity::m_location, 3, 0}},
  {20, {&Dxf_insert_entity::m_location, 3, 1}},
  {30, {&Dxf_insert_entity::m_location, 3, 2}},
  {41, {&Dxf_insert_entity::m_x_scale_factor, 1, 0}},
  {42, {&Dxf_insert_entity::m_y_scale_factor, 1, 0}},
  {43, {&Dxf_insert_entity::m_z_scale_factor, 1, 0}},
  {50, {&Dxf_insert_entity::m_rotation, 1, 0}},
  {70, {&Dxf_insert_entity::m_column_count, 1, 0}},
  {71, {&Dxf_insert_entity::m_row_count, 1, 0}},
  {44, {&Dxf_insert_entity::m_column_spacing, 1, 0}},
  {45, {&Dxf_insert_entity::m_row_spacing, 1, 0}},
  {210, {&Dxf_insert_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_insert_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_insert_entity::m_extrusion_direction, 3, 2}}
};

//! Record handlers
template <>
const std::map<int, Dxf_insert_entity_wrapper::Record_handler_type>
Dxf_insert_entity_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
