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
#include "SGAL/Dxf_dictionary_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_dictionary_object>  Dxf_dictionary_object_wrapper;

template <>
const std::map<int, Dxf_dictionary_object_wrapper::Record_member>
Dxf_dictionary_object_wrapper::s_record_members = {
  {5, {&Dxf_dictionary_object::m_handle, 1, 0}},
  {280, {&Dxf_dictionary_object::m_is_hard_owner, 1, 0}},
  {281, {&Dxf_dictionary_object::m_duplicate_record_handling, 1, 0}},
  // {3, entry name
  // {350, entry handle
};

//! \brief handles a value that requires special handling.
bool Dxf_dictionary_object::handle_value(int code, const String& value)
{
  if (3 == code) {
    m_entry_name = value;
    return true;
  }
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_dictionary_object::handle_value(int code, Uint value)
{
  if (350 == code) {
    m_value_handles[m_entry_name] = value;
    return true;
  }
  return false;
}

SGAL_END_NAMESPACE
