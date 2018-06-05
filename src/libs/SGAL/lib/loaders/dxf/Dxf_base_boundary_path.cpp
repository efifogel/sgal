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

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_base_boundary_path.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_base_boundary_path>
  Dxf_base_boundary_path_wrapper;

//! Record members
template <>
const std::map<int, Dxf_base_boundary_path_wrapper::Record_member>
Dxf_base_boundary_path_wrapper::s_record_members = {
  // {92, Boundary path type flag (bit coded)
};

//! Record handles
template <>
const std::map<int, Dxf_base_boundary_path_wrapper::Record_handler_type>
Dxf_base_boundary_path_wrapper::s_record_handlers = {
  {97, &Dxf_base_boundary_path::handle_source_objects_num},
  {330, &Dxf_base_boundary_path::handle_source_object}
};

//! \brief handles the number of source objects.
void Dxf_base_boundary_path::handle_source_objects_num(int32_t size)
{ m_source_objects.reserve(size); }

//! \brief handles a source-object item.
void Dxf_base_boundary_path::handle_source_object(const String& source_object)
{
  m_source_objects.resize(m_source_objects.size() + 1);
  m_source_objects.back() = source_object;
}

SGAL_END_NAMESPACE
