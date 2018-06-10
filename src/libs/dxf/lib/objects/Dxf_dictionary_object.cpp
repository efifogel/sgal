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
#include "dxf/Dxf_dictionary_object.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_dictionary_object>  Dxf_dictionary_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_dictionary_object_wrapper::Record_member>
Dxf_dictionary_object_wrapper::s_record_members = {
  {5, {&Dxf_dictionary_object::m_handle, 1, 0}},
  {280, {&Dxf_dictionary_object::m_is_hard_owner, 1, 0}},
  {281, {&Dxf_dictionary_object::m_duplicate_record_handling, 1, 0}},
};

//! Record handlers
template <>
const std::map<int, Dxf_dictionary_object_wrapper::Record_handler_type>
Dxf_dictionary_object_wrapper::s_record_handlers = {
  {3, &Dxf_dictionary_object::handle_entry_name},
  {350, &Dxf_dictionary_object::handle_entry_value}
};

DXF_END_NAMESPACE
