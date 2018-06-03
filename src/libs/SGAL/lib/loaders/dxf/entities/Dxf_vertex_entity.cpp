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
#include "SGAL/Dxf_vertex_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_vertex_entity>  Dxf_vertex_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_vertex_entity_wrapper::Record_member>
Dxf_vertex_entity_wrapper::s_record_members = {
  {10, {&Dxf_vertex_entity::m_location, 3, 0}},
  {20, {&Dxf_vertex_entity::m_location, 3, 1}},
  {30, {&Dxf_vertex_entity::m_location, 3, 2}},
  {40, {&Dxf_vertex_entity::m_starting_width, 1, 0}},
  {41, {&Dxf_vertex_entity::m_ending_width, 1, 0}},
  {42, {&Dxf_vertex_entity::m_bulge, 1, 0}},
  {70, {&Dxf_vertex_entity::m_flags, 1, 0}},
  {50, {&Dxf_vertex_entity::m_curve_fit_tangent_direction, 1, 0}},
  {71, {&Dxf_vertex_entity::m_polyface_mesh_vertex_index1, 1, 0}},
  {72, {&Dxf_vertex_entity::m_polyface_mesh_vertex_index2, 1, 0}},
  {73, {&Dxf_vertex_entity::m_polyface_mesh_vertex_index3, 1, 0}},
  {74, {&Dxf_vertex_entity::m_polyface_mesh_vertex_index4, 1, 0}},
  {91, {&Dxf_vertex_entity::m_identifier, 1, 0}},
};

//! Record handlers
template <>
const std::map<int, Dxf_vertex_entity_wrapper::Record_handler_type>
Dxf_vertex_entity_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
