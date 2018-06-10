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
#include "dxf/Dxf_mlinestyle_object.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_mlinestyle_object>  Dxf_mlinestyle_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_mlinestyle_object_wrapper::Record_member>
Dxf_mlinestyle_object_wrapper::s_record_members = {
  {2, {&Dxf_mlinestyle_object::m_style_name, 1, 0}},
  {70, {&Dxf_mlinestyle_object::m_flags, 1, 0}},
  {3, {&Dxf_mlinestyle_object::m_style_description, 1, 0}},
  {62, {&Dxf_mlinestyle_object::m_fill_color, 1, 0}},
  {51, {&Dxf_mlinestyle_object::m_start_angle, 1, 0}},
  {52, {&Dxf_mlinestyle_object::m_end_angle, 1, 0}},
};

//! Record handlers
template <>
const std::map<int, Dxf_mlinestyle_object_wrapper::Record_handler_type>
Dxf_mlinestyle_object_wrapper::s_record_handlers = {
  {71, &Dxf_mlinestyle_object::handle_elements_num},
  {49, &Dxf_mlinestyle_object::handle_element_offset},
  {62, &Dxf_mlinestyle_object::handle_element_color},
  {6, &Dxf_mlinestyle_object::handle_element_line_style}
};

DXF_END_NAMESPACE
