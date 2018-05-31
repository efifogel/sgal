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
#include "SGAL/Dxf_boundary_path.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_boundary_path>   Dxf_boundary_path_wrapper;

template <>
const std::map<int, Dxf_boundary_path_wrapper::Record_member>
Dxf_boundary_path_wrapper::s_record_members = {
  // {92, Boundary path type flag (bit coded)
  // {93, Number of edges in this boundary path (only if boundary is not a
                        // polyline)
  // {97, Number of source boundary objects
  // {330, Reference to source boundary objects (multiple entries)
};

//!
template <>
const std::map<int, Dxf_boundary_path_wrapper::Record_handler_type>
Dxf_boundary_path_wrapper::s_record_handlers = {
  {97, &Dxf_boundary_path::number_handler},
  {330, &Dxf_boundary_path::source_objects_handler}
};

//!
void Dxf_boundary_path::number_handler(int32_t size)
{ m_source_objects.reserve(size); }

//!
void Dxf_boundary_path::source_objects_handler(Uint handler)
{
  SGAL_assertion(! m_source_objects.empty());
  m_source_objects.back() = handler;
}

SGAL_END_NAMESPACE
