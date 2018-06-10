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
#include "dxf/Dxf_extended_data.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_extended_data>  Dxf_extended_data_wrapper;

template <>
const std::map<int, Dxf_extended_data_wrapper::Record_member>
Dxf_extended_data_wrapper::s_record_members = {
  {1000, {&Dxf_extended_data::m_name, 1, 0}},
  {1003, {&Dxf_extended_data::m_layer_name, 1, 0}},
  {1005, {&Dxf_extended_data::m_entity_handle, 1, 0}},
  {1010, {&Dxf_extended_data::m_point, 3, 0}},
  {1020, {&Dxf_extended_data::m_point, 3, 1}},
  {1030, {&Dxf_extended_data::m_point, 3, 2}},
  {1040, {&Dxf_extended_data::m_real, 1, 0}},
  {1070, {&Dxf_extended_data::m_integer, 1, 0}},
  {1071, {&Dxf_extended_data::m_long, 1, 0}},
  {1004, {&Dxf_extended_data::m_binary_data, 1, 0}},
  {1011, {&Dxf_extended_data::m_worldspace_position, 3, 0}},
  {1021, {&Dxf_extended_data::m_worldspace_position, 3, 1}},
  {1031, {&Dxf_extended_data::m_worldspace_position, 3, 2}},
  {1012, {&Dxf_extended_data::m_worldspace_displacement, 3, 0}},
  {1022, {&Dxf_extended_data::m_worldspace_displacement, 3, 1}},
  {1032, {&Dxf_extended_data::m_worldspace_displacement, 3, 2}},
  {1013, {&Dxf_extended_data::m_world_direction, 3, 0}},
  {1023, {&Dxf_extended_data::m_world_direction, 3, 1}},
  {1033, {&Dxf_extended_data::m_world_direction, 3, 2}},
  {1041, {&Dxf_extended_data::m_distance, 1, 0}},
  {1042, {&Dxf_extended_data::m_scale_factor, 1, 0}},

};

DXF_END_NAMESPACE
