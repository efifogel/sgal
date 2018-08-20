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
#include "dxf/Dxf_base_entry.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"

DXF_BEGIN_NAMESPACE

//!
typedef Dxf_simple_record_wrapper<Dxf_base_entry>       Dxf_entry_wrapper;
template <>
const std::map<int, Dxf_entry_wrapper::Record_member_type>
Dxf_entry_wrapper::s_record_members = {
  {5, &Dxf_base_entry::m_handle},
  {105, &Dxf_base_entry::m_handle}, // DYMSTYLE only; Lame!!!
  {360, &Dxf_base_entry::m_owner_dict},
  {330, &Dxf_base_entry::m_owner_handle}
};

DXF_END_NAMESPACE
