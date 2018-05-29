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
#include "SGAL/Dxf_lwpolyline_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_lwpolyline_entity>  Dxf_lwpolyline_entity_wrapper;

template <>
const std::map<int, Dxf_lwpolyline_entity_wrapper::Record_member>
Dxf_lwpolyline_entity_wrapper::s_record_members = {
  // {90, Number of vertices
  {70, {&Dxf_lwpolyline_entity::m_flags, 1, 0}},
  {43, {&Dxf_lwpolyline_entity::m_constant_width, 1, 0}},
  {38, {&Dxf_lwpolyline_entity::m_elevation, 1, 0}},
  {39, {&Dxf_lwpolyline_entity::m_thickness, 1, 0}},
  // {10, {&Dxf_lwpolyline_entity::m_vertices, n, i, 0}},
  // {20, {&Dxf_lwpolyline_entity::m_vertices, n, i, 1}},
  // {30, {&Dxf_lwpolyline_entity::m_vertices, n, i, 2}},
  {91, {&Dxf_lwpolyline_entity::m_identifier, 1, 0}},
  {40, {&Dxf_lwpolyline_entity::m_starting_width, 1, 0}},
  {41, {&Dxf_lwpolyline_entity::m_end_width, 1, 0}},
  // {42, {&Dxf_lwpolyline_entity::m_bulges, n, i, 0}},
  {210, {&Dxf_lwpolyline_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_lwpolyline_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_lwpolyline_entity::m_extrusion_direction, 3, 2}}
};

//! \brief handles a value that requires special handling.
bool Dxf_lwpolyline_entity::handle_value(int code, double value)
{
  size_t i;

  switch (code) {
   case 10:
    m_vertices.resize(m_vertices.size() + 1);
    m_vertices.back()[0] = value;
    return true;

   case 20:
    i = m_vertices.size();
    m_vertices.back()[1] = value;
    return true;

   case 30:
    i = m_vertices.size();
    m_vertices.back()[2] = value;
    return true;

   case 40:
    m_bulges.resize(m_bulges.size() + 1);
    m_bulges.back() = value;
    return true;
  }
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_lwpolyline_entity::handle_value(int code, int32_t value)
{
  if (90 == code) {
    m_vertices.reserve(code);
    m_bulges.reserve(code);
    return true;
  }
  return false;
}

SGAL_END_NAMESPACE
