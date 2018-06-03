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
#include "SGAL/Dxf_leader_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_leader_entity>  Dxf_leader_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_leader_entity_wrapper::Record_member>
Dxf_leader_entity_wrapper::s_record_members = {
  {3, {&Dxf_leader_entity::m_dimension_style_name, 1, 0}},
  {71, {&Dxf_leader_entity::m_use_arrowheads, 1, 0}},
  {72, {&Dxf_leader_entity::m_path_type, 1, 0}},
  {73, {&Dxf_leader_entity::m_annotation_type, 1, 0}},
  {74, {&Dxf_leader_entity::m_hookline_direction, 1, 0}},
  {75, {&Dxf_leader_entity::m_hookline, 1, 0}},
  {40, {&Dxf_leader_entity::m_text_annotation_height, 1, 0}},
  {41, {&Dxf_leader_entity::m_text_annotation_width, 1, 0}},
  {76, {&Dxf_leader_entity::m_vertex_count, 1, 0}},
  // {10, {&Dxf_leader_entity::m_vertices, n, i, 0}},
  // {20, {&Dxf_leader_entity::m_vertices, n, i, 1}},
  // {30, {&Dxf_leader_entity::m_vertices, 1, i, 2}},
  {77, {&Dxf_leader_entity::m_override_color, 1, 0}},
  {340, {&Dxf_leader_entity::m_associated_annotation_reference, 1, 0}},
  {210, {&Dxf_leader_entity::m_normal, 3, 0}},
  {220, {&Dxf_leader_entity::m_normal, 3, 1}},
  {230, {&Dxf_leader_entity::m_normal, 3, 2}},
  {211, {&Dxf_leader_entity::m_right, 3, 0}},
  {221, {&Dxf_leader_entity::m_right, 3, 1}},
  {231, {&Dxf_leader_entity::m_right, 3, 2}},
  {212, {&Dxf_leader_entity::m_block_offset, 3, 0}},
  {222, {&Dxf_leader_entity::m_block_offset, 3, 1}},
  {232, {&Dxf_leader_entity::m_block_offset, 3, 2}},
  {213, {&Dxf_leader_entity::m_annotation_offset, 3, 0}},
  {223, {&Dxf_leader_entity::m_annotation_offset, 3, 1}},
  {233, {&Dxf_leader_entity::m_annotation_offset, 3, 2}}
};

//! Record handlers
template <>
const std::map<int, Dxf_leader_entity_wrapper::Record_handler_type>
Dxf_leader_entity_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
