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

#ifndef DXF_UCS_ENTRY_HPP
#define DXF_UCS_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entry.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_ucs_entry : Dxf_base_entry {
  /// Record members
  //@{
  SGAL::String m_name;  // UCS name
  int16_t m_flags;      // Standard flag values (bit-coded values):
                        // 16 = If set, table entry is externally dependent on
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
  double m_origin[3];   // Origin (in WCS)
  double m_x_direction[3]; // X-axis direction (in WCS)
  double m_y_direction[3]; // X-axis direction (in WCS)
  int16_t m_dummy;      // Always 0
  double m_elevation;   // Elevation
  SGAL::String m_base_ucs_handle; // ID/handle of base UCS if this is an
                        // orthographic. This code is not present if the 79 code
                        // is 0. If this code is not present and 79 code is
                        // non-zero, then base UCS is assumed to be WORLD
  int16_t m_orthographic_type; // Orthographic type (optional; always appears in
                        // pairs with the 13, 23, 33 codes):
                        // 1 = Top; 2 = Bottom
                        // 3 = Front; 4 = Back
                        // 5 = Left; 6 = Right
  double m_type_origin[3]; // Origin for this orthographic type relative to this
                        // UCS
};

DXF_END_NAMESPACE

#endif
