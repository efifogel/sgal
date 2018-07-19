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
#include "dxf/Dxf_circle_entity.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_circle_entity>  Dxf_circle_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_circle_entity_wrapper::Record_member>
Dxf_circle_entity_wrapper::s_record_members = {
  {39, {&Dxf_circle_entity::m_thickness, 1, 0}},
  {10, {&Dxf_circle_entity::m_center, 3, 0}},
  {20, {&Dxf_circle_entity::m_center, 3, 1}},
  {30, {&Dxf_circle_entity::m_center, 3, 2}},
  {40, {&Dxf_circle_entity::m_radius, 1, 0}},
  {210, {&Dxf_circle_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_circle_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_circle_entity::m_extrusion_direction, 3, 2}}
};

//! Record handlers
template <>
const std::map<int, Dxf_circle_entity_wrapper::Record_handler_type>
Dxf_circle_entity_wrapper::s_record_handlers = {};

DXF_END_NAMESPACE
