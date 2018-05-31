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
#include "SGAL/Dxf_layout_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_layout_object>  Dxf_layout_object_wrapper;

template <>
const std::map<int, Dxf_layout_object_wrapper::Record_member>
Dxf_layout_object_wrapper::s_record_members = {
  {70, {&Dxf_layout_object::m_layout_flags, 1, 0}},
  {71, {&Dxf_layout_object::m_tab_order, 1, 0}},
  {10, {&Dxf_layout_object::m_minimum_limits, 2, 0}},
  {20, {&Dxf_layout_object::m_minimum_limits, 2, 1}},
  {11, {&Dxf_layout_object::m_maximum_limits, 2, 0}},
  {21, {&Dxf_layout_object::m_maximum_limits, 2, 1}},
  {12, {&Dxf_layout_object::m_insertion_base_point, 3, 0}},
  {22, {&Dxf_layout_object::m_insertion_base_point, 3, 1}},
  {32, {&Dxf_layout_object::m_insertion_base_point, 3, 2}},
  {14, {&Dxf_layout_object::m_minimum_extents, 3, 0}},
  {24, {&Dxf_layout_object::m_minimum_extents, 3, 1}},
  {34, {&Dxf_layout_object::m_minimum_extents, 3, 2}},
  {15, {&Dxf_layout_object::m_maximum_extents, 3, 0}},
  {25, {&Dxf_layout_object::m_maximum_extents, 3, 1}},
  {35, {&Dxf_layout_object::m_maximum_extents, 3, 2}},
  {146, {&Dxf_layout_object::m_elevation, 1, 0}},
  {13, {&Dxf_layout_object::m_ucs_origin, 3, 0}},
  {23, {&Dxf_layout_object::m_ucs_origin, 3, 1}},
  {33, {&Dxf_layout_object::m_ucs_origin, 3, 2}},
  {16, {&Dxf_layout_object::m_ucs_x_axis, 3, 0}},
  {26, {&Dxf_layout_object::m_ucs_x_axis, 3, 1}},
  {36, {&Dxf_layout_object::m_ucs_x_axis, 3, 2}},
  {17, {&Dxf_layout_object::m_ucs_y_axis, 3, 0}},
  {27, {&Dxf_layout_object::m_ucs_y_axis, 3, 1}},
  {37, {&Dxf_layout_object::m_ucs_y_axis, 3, 2}},
  {76, {&Dxf_layout_object::m_ucs_orthographic_type, 1, 0}},
  {331, {&Dxf_layout_object::m_viewport, 1, 0}},
  {345, {&Dxf_layout_object::m_table_record, 1, 0}},
  {346, {&Dxf_layout_object::m_table_record_base, 1, 0}},
  {333, {&Dxf_layout_object::m_shade_plot, 1, 0}}
};

SGAL_END_NAMESPACE
