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
#include "SGAL/Dxf_imagedef_reactor_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_imagedef_reactor_object>
  Dxf_imagedef_reactor_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_imagedef_reactor_object_wrapper::Record_member>
Dxf_imagedef_reactor_object_wrapper::s_record_members = {};

//! Record handlers
template <>
const std::map<int, Dxf_imagedef_reactor_object_wrapper::Record_handler_type>
Dxf_imagedef_reactor_object_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
