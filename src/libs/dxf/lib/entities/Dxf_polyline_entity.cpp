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
#include "dxf/Dxf_polyline_entity.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_polyline_entity>  Dxf_polyline_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_polyline_entity_wrapper::Record_member>
Dxf_polyline_entity_wrapper::s_record_members = {
  {10, {&Dxf_polyline_entity::m_elevation, 3, 0}},
  {20, {&Dxf_polyline_entity::m_elevation, 3, 1}},
  {30, {&Dxf_polyline_entity::m_elevation, 3, 2}},
  {39, {&Dxf_polyline_entity::m_thickness, 1, 0}},
  {70, {&Dxf_polyline_entity::m_flag, 1, 0}},
  {40, {&Dxf_polyline_entity::m_start_width, 1, 0}},
  {41, {&Dxf_polyline_entity::m_end_width, 1, 0}},
  {71, {&Dxf_polyline_entity::m_m_count, 1, 0}},
  {72, {&Dxf_polyline_entity::m_n_count, 1, 0}},
  {73, {&Dxf_polyline_entity::m_m_density, 1, 0}},
  {74, {&Dxf_polyline_entity::m_n_density, 1, 0}},
  {75, {&Dxf_polyline_entity::m_type, 1, 0}},
  {210, {&Dxf_polyline_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_polyline_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_polyline_entity::m_extrusion_direction, 3, 2}}
};

//! Record handlers
template <>
const std::map<int, Dxf_polyline_entity_wrapper::Record_handler_type>
Dxf_polyline_entity_wrapper::s_record_handlers = {
  {66, &Dxf_polyline_entity::handle_obsolete}
};

DXF_END_NAMESPACE
