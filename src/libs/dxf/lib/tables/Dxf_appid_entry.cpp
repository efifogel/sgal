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
#include "dxf/Dxf_appid_entry.hpp"
#include "dxf/Dxf_record_wrapper.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_appid_entry>     Dxf_appid_wrapper;

//!
template <>
const std::map<int, Dxf_appid_wrapper::Record_member>
Dxf_appid_wrapper::s_record_members = {
  {2, {&Dxf_appid_entry::m_name, 1, 0}},
  {70, {&Dxf_appid_entry::m_flags, 1, 0}}
};

//!
template <>
const std::map<int, Dxf_appid_wrapper::Record_handler_type>
Dxf_appid_wrapper::s_record_handlers = {};

DXF_END_NAMESPACE
