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
#include "SGAL/Dxf_pattern_data.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_pattern_data>  Dxf_pattern_data_wrapper;

template <>
const std::map<int, Dxf_pattern_data_wrapper::Record_member>
Dxf_pattern_data_wrapper::s_record_members = {
  {53, {&Dxf_pattern_data::m_angle, 1, 0}},
  {43, {&Dxf_pattern_data::m_base_point, 2, 0}},
  {44, {&Dxf_pattern_data::m_base_point, 2, 1}},
  {45, {&Dxf_pattern_data::m_offset, 2, 0}},
  {46, {&Dxf_pattern_data::m_offset, 2, 1}}
  // {79 Number of dash length items
  // {49 {&Dxf_pattern_data::m_dash_lengths, n, i}
};

template <>
const std::map<int, Dxf_pattern_data_wrapper::Record_handler_type>
Dxf_pattern_data_wrapper::s_record_handlers = {
  {79, &Dxf_pattern_data::handle_size},
  {49, &Dxf_pattern_data::handle_dash_length}
};

//!
void Dxf_pattern_data::handle_size(int16_t size)
{ m_dash_lengths.reserve(size); }

//!
void Dxf_pattern_data::handle_dash_length(double dash_length)
{ m_dash_lengths.push_back(dash_length); }

SGAL_END_NAMESPACE
