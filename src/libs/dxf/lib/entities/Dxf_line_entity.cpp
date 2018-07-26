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
#include "dxf/Dxf_line_entity.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_line_entity>  Dxf_line_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_line_entity_wrapper::Record_member>
Dxf_line_entity_wrapper::s_record_members = {
  {39, {&Dxf_line_entity::m_thickness, 1, 0}},
  {10, {&Dxf_line_entity::m_start, 3, 0}},
  {20, {&Dxf_line_entity::m_start, 3, 1}},
  {30, {&Dxf_line_entity::m_start, 3, 2}},
  {11, {&Dxf_line_entity::m_end, 3, 0}},
  {21, {&Dxf_line_entity::m_end, 3, 1}},
  {31, {&Dxf_line_entity::m_end, 3, 2}},
  {210, {&Dxf_line_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_line_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_line_entity::m_extrusion_direction, 3, 2}},
};

//! Record handlers
template <>
const std::map<int, Dxf_line_entity_wrapper::Record_handler_type>
Dxf_line_entity_wrapper::s_record_handlers = {};

//! \brief constructs (set default values).
Dxf_line_entity::Dxf_line_entity() :
  m_thickness(0),
  m_extrusion_direction{0, 0, 1},
  m_start{0, 0, 0},
  m_end{0, 0, 0}
{}

DXF_END_NAMESPACE
