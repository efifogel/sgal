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
#include "SGAL/Dxf_spline_entity.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_spline_entity>  Dxf_spline_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_spline_entity_wrapper::Record_member>
Dxf_spline_entity_wrapper::s_record_members = {
  {210, {&Dxf_spline_entity::m_normal, 3, 0}},
  {220, {&Dxf_spline_entity::m_normal, 3, 1}},
  {230, {&Dxf_spline_entity::m_normal, 3, 2}},
  {70, {&Dxf_spline_entity::m_flags, 1, 0}},
  {71, {&Dxf_spline_entity::m_degree_of_curve, 1, 0}},
  {42, {&Dxf_spline_entity::m_knot_tolerance, 1, 0}},
  {43, {&Dxf_spline_entity::m_control_point_tolerance, 1, 0}},
  {44, {&Dxf_spline_entity::m_fit_tolerance, 1, 0}},
  {12, {&Dxf_spline_entity::m_start_tangent, 3, 0}},
  {22, {&Dxf_spline_entity::m_start_tangent, 3, 1}},
  {32, {&Dxf_spline_entity::m_start_tangent, 3, 2}},
  {13, {&Dxf_spline_entity::m_end_tangent, 3, 0}},
  {23, {&Dxf_spline_entity::m_end_tangent, 3, 1}},
  {33, {&Dxf_spline_entity::m_end_tangent, 3, 2}},
  {41, {&Dxf_spline_entity::m_weight, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_spline_entity_wrapper::Record_handler_type>
Dxf_spline_entity_wrapper::s_record_handlers = {
  {72, &Dxf_spline_entity::handle_knot_values_num},
  {73, &Dxf_spline_entity::handle_control_points_num},
  {74, &Dxf_spline_entity::handle_fit_points_num},
  {40, &Dxf_spline_entity::handle_knot_value},
  {10, &Dxf_spline_entity::handle_control_point_x},
  {20, &Dxf_spline_entity::handle_control_point_y},
  {30, &Dxf_spline_entity::handle_control_point_z},
  {11, &Dxf_spline_entity::handle_fit_point_x},
  {21, &Dxf_spline_entity::handle_fit_point_y},
  {31, &Dxf_spline_entity::handle_fit_point_z}
};

SGAL_END_NAMESPACE
