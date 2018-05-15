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

#ifndef SGAL_DXF_BASE_TABLE_HPP
#define SGAL_DXF_BASE_TABLE_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_base_table {
  String m_handle;      // Handle
  Uint m_owner_dict;    // Hard owner ID/handle to owner dictionary (optional)
  Uint m_owner_obj;     // Soft-pointer ID/handle to owner object
  String m_group;       // "{ACAD_XDICTIONARY" and "}" indicate the start and
                        // end, respectively, of an extension dictionary group.
                        // This group exists only if persistent reactors have
                        // been attached to this object (optional)
};

SGAL_END_NAMESPACE

#endif
