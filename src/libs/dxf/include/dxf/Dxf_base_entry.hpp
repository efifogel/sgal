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

#ifndef DXF_TABLE_ENTRY_HPP
#define DXF_TABLE_ENTRY_HPP

#include <string>
#include <vector>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_extended_data.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_base_entry {
  /// Member records
  //@{
  SGAL::String m_handle; // Handle
  SGAL::String m_owner_dict; // Hard owner ID/handle to owner dictionary
                        // (optional)
  SGAL::String m_owner_handle; // Soft-pointer ID/handle to owner object
  std::map<SGAL::String, std::vector<SGAL::String> > m_xdata; //
                        // "{ACAD_XDICTIONARY" and "}"
                        // indicate the start and end, respectively, of an
                        // extension dictionary group.
                        // This group exists only if persistent reactors have
                        // been attached to this object (optional)
  std::vector<Dxf_extended_data> m_extended_data;
  //@}

  /*! Initialize */
  void init() {}
};

DXF_END_NAMESPACE

#endif
