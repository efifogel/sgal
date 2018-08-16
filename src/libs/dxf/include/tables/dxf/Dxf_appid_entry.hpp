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

#ifndef DXF_APPID_ENTRY_HPP
#define DXF_APPID_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entry.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_appid_entry : Dxf_base_entry {
  /// Record members
  //@{
  SGAL::String m_name;  // User-supplied (or application-supplied) application
                        // name (for extended data).
  int16_t m_flags;      // 16 = If set, table entry is externally dependent on
                        //      an xref
                        // 32 = If both this bit and bit 16 are set, the
                        //      externally dependent xref has been successfully
                        //      resolved
                        // 64 = If set, the table entry was referenced by at
                        //      least one entity in the drawing the last time
                        //      the drawing was edited. (This flag is for the
                        //      benefit of AutoCAD commands. It can be ignored
                        //      by most programs that read DXF files and need
                        //      not be set by programs that write DXF files)
  //@}
};

DXF_END_NAMESPACE

#endif
