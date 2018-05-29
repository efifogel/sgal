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

SGAL_END_NAMESPACE
