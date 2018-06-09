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
#include "dxf/Dxf_polyline_boundary_path.hpp"
#include "dxf/Dxf_parser.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_polyline_boundary_path>
  Dxf_polyline_boundary_path_wrapper;

template <>
const std::map<int, Dxf_polyline_boundary_path_wrapper::Record_member>
Dxf_polyline_boundary_path_wrapper::s_record_members = {
  {72, {&Dxf_polyline_boundary_path::m_has_bulge, 1, 0}},
  {73, {&Dxf_polyline_boundary_path::m_is_closed, 1, 0}},
  // {93,  Number of polyline vertices
  // {10, {{&Dxf_polyline_boundary_path::m_location, n, i, 0}},
  // {20, {{&Dxf_polyline_boundary_path::m_location, n, i, 1}},
  // {30, {{&Dxf_polyline_boundary_path::m_location, n, i, 2}},
  {42 , {&Dxf_polyline_boundary_path::m_bulge, 1, 0}}
};

//!
template <>
const std::map<int, Dxf_polyline_boundary_path_wrapper::Record_handler_type>
Dxf_polyline_boundary_path_wrapper::s_record_handlers = {
  {93, &Dxf_polyline_boundary_path::number_handler},
  {10, &Dxf_polyline_boundary_path::m_location_0_handler},
  {20, &Dxf_polyline_boundary_path::m_location_1_handler},
  {30, &Dxf_polyline_boundary_path::m_location_2_handler}
};

//!
void Dxf_polyline_boundary_path::number_handler(int32_t size)
{ m_locations.reserve(size); }

//!
void Dxf_polyline_boundary_path::m_location_0_handler(double coord)
{
  m_locations.resize(m_locations.size() + 1);
  m_locations.back()[0] = coord;
}

//!
void Dxf_polyline_boundary_path::m_location_1_handler(double coord)
{
  SGAL_assertion(! m_locations.empty());
  m_locations.back()[1] = coord;
}

//!
void Dxf_polyline_boundary_path::m_location_2_handler(double coord)
{
  SGAL_assertion(! m_locations.empty());
  m_locations.back()[2] = coord;
}

SGAL_END_NAMESPACE
