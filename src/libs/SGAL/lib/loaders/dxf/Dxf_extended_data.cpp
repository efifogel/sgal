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
#include "SGAL/Dxf_extended_data.hpp"
#include "SGAL/Dxf_parser.hpp"

SGAL_BEGIN_NAMESPACE

//!
const std::map<int, Dxf_parser::Extended_data_member>
Dxf_parser::s_extended_data_members = {
  {1000, {&Dxf_extended_data::m_name, 1, 0}},
  {1003, {&Dxf_extended_data::m_layer_name, 1, 0}},
  {1005, {&Dxf_extended_data::m_entity_handle, 1, 0}},
  //{1010, {&Dxf_extended_data::m_point, 3, 0}},
  //{1020, {&Dxf_extended_data::m_point, 3, 1}},
  //{1030, {&Dxf_extended_data::m_point, 3, 2}},
  {1040, {&Dxf_extended_data::m_real, 1, 0}},
  {1070, {&Dxf_extended_data::m_integer, 1, 0}},
  {1071, {&Dxf_extended_data::m_long, 1, 0}}
};

SGAL_END_NAMESPACE
