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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_ucs_entry.hpp"
#include "dxf/Dxf_record_wrapper.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_ucs_entry>               Dxf_ucs_wrapper;

//!
template <>
const std::map<int, Dxf_ucs_wrapper::Record_member>
Dxf_ucs_wrapper::s_record_members = {
  {2, {&Dxf_ucs_entry::m_name, 1, 0}},
  {70, {&Dxf_ucs_entry::m_flags, 1, 0}},
  {10, {&Dxf_ucs_entry::m_origin, 3, 0}},
  {20, {&Dxf_ucs_entry::m_origin, 3, 1}},
  {30, {&Dxf_ucs_entry::m_origin, 3, 2}},
  {11, {&Dxf_ucs_entry::m_x_direction, 3, 0}},
  {21, {&Dxf_ucs_entry::m_x_direction, 3, 1}},
  {31, {&Dxf_ucs_entry::m_x_direction, 3, 2}},
  {12, {&Dxf_ucs_entry::m_y_direction, 3, 0}},
  {22, {&Dxf_ucs_entry::m_y_direction, 3, 1}},
  {32, {&Dxf_ucs_entry::m_y_direction, 3, 2}},
  {79, {&Dxf_ucs_entry::m_dummy, 1, 0}},
  {146, {&Dxf_ucs_entry::m_elevation, 1, 0}},
  {346, {&Dxf_ucs_entry::m_base_ucs_handle, 1, 0}},
  {71, {&Dxf_ucs_entry::m_orthographic_type, 1, 0}},
  {13, {&Dxf_ucs_entry::m_type_origin, 3, 0}},
  {23, {&Dxf_ucs_entry::m_type_origin, 3, 1}},
  {33, {&Dxf_ucs_entry::m_type_origin, 3, 2}}
};

//!
template <>
const std::map<int, Dxf_ucs_wrapper::Record_handler_type>
Dxf_ucs_wrapper::s_record_handlers = {};

DXF_END_NAMESPACE
