// Copyright (c) 2004,2018 Israel.
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

template <>
const std::map<int, Dxf_spline_entity_wrapper::Record_member>
Dxf_spline_entity_wrapper::s_record_members = {
  {210, {&Dxf_spline_entity::m_normal, 3, 0}},
  {220, {&Dxf_spline_entity::m_normal, 3, 1}},
  {230, {&Dxf_spline_entity::m_normal, 3, 2}},
  {70, {&Dxf_spline_entity::m_flags, 1, 0}},
  {71, {&Dxf_spline_entity::m_degree_of_curve, 1, 0}},
  // {72}; Number of knots
  // {73}; Number of control points
  // {74}; Number of fit points (if any)
  {42, {&Dxf_spline_entity::m_knot_tolerance, 1, 0}},
  {43, {&Dxf_spline_entity::m_control_point_tolerance, 1, 0}},
  {44, {&Dxf_spline_entity::m_fit_tolerance, 1, 0}},
  {12, {&Dxf_spline_entity::m_start_tangent, 1, 0}},
  {22, {&Dxf_spline_entity::m_start_tangent, 1, 0}},
  {32, {&Dxf_spline_entity::m_start_tangent, 1, 0}},
  {13, {&Dxf_spline_entity::m_end_tangent, 1, 0}},
  {23, {&Dxf_spline_entity::m_end_tangent, 1, 0}},
  {33, {&Dxf_spline_entity::m_end_tangent, 1, 0}},
  // {40, {&Dxf_spline_entity::m_knot_values, n, i}},
  {41, {&Dxf_spline_entity::m_weight, 1, 0}}
  // {10, {&Dxf_spline_entity::m_control_points, n, i}},
  // {20, {&Dxf_spline_entity::m_control_points, n, i}},
  // {30, {&Dxf_spline_entity::m_control_points, n, i}},
  // {11, {&Dxf_spline_entity::m_fit_points, n, i}},
  // {21, {&Dxf_spline_entity::m_fit_points, n, i}},
  // {21, {&Dxf_spline_entity::m_fit_points, n, i}},
};

//! \brief handles a value that requires special handling.
bool Dxf_spline_entity::handle_value(int code, int16_t value)
{
  switch (code) {
   case 72: m_knot_values.reserve(value); return true;
   case 73: m_control_points.reserve(value); return true;
   case 74: m_fit_points.reserve(value); return true;
  }
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_spline_entity::handle_value(int code, double value)
{
  size_t i;

  switch (code) {
   case 40:
    i = m_knot_values.size();
    m_knot_values.resize(i + 1);
    m_knot_values[i] = value;
    return true;

   case 10:
    i = m_control_points.size();
    m_control_points.resize(i + 1);
    m_control_points[i][0] = value;
    return true;

   case 20:
    i = m_control_points.size();
    m_control_points[i-1][1] = value;
    return true;

   case 30:
    i = m_control_points.size();
    m_control_points[i-1][2] = value;
    return true;

   case 11:
    i = m_fit_points.size();
    m_fit_points.resize(i + 1);
    m_fit_points[i][0] = value;
    return true;

   case 21:
    i = m_fit_points.size();
    m_fit_points[i-1][1] = value;
    return true;

   case 31:
    i = m_fit_points.size();
    m_fit_points[i-1][2] = value;
    return true;
  }
  return false;
}

SGAL_END_NAMESPACE
