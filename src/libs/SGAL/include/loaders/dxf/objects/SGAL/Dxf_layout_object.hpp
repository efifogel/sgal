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

#ifndef SGAL_DXF_LAYOUT_OBJECT_HPP
#define SGAL_DXF_LAYOUT_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_layout_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  int16_t m_layout_flags; // Flag (bit-coded) to control the following:
                        // 1 = Indicates the PSLTSCALE value for this layout
                        //     when this layout is current
                        // 2 = Indicates the LIMCHECK value for this layout when
                        // this layout is current
  int16_t m_tab_order;  // Tab order. This number is an ordinal indicating this
                        // layout's ordering in the tab control that is attached
                        // to the AutoCAD drawing frame window. Note that the
                        // "Model" tab always appears as the first tab
                        // regardless of its tab order
  double m_minimum_limits[2]; // Minimum limits for this layout (defined by
                        // LIMMIN while this layout is current)
  double m_maximum_limits[2]; // Maximum limits for this layout (defined by
                        // LIMMAX while this layout is current):
  double m_insertion_base_point[3]; // Insertion base point for this layout
                        // (defined by INSBASE while this layout is current):
  double m_minimum_extents[3]; // Minimum extents for this layout (defined by
                        // EXTMIN while this layout is current):
  double m_maximum_extents[3]; // Maximum extents for this layout (defined by
                        // EXTMAX while this layout is current):
  double m_elevation;   // Elevation
  double m_ucs_origin[3]; // UCS origin
  double m_ucs_x_axis[3]; // UCS X-axis
  double m_ucs_y_axis[3]; // UCS Y axis
  int16_t m_ucs_orthographic_type; // Orthographic type of UCS
                        // 0 = UCS is not orthographic
                        // 1 = Top; 2 = Bottom
                        // 3 = Front; 4 = Back
                        // 5 = Left; 6 = Right
  Uint m_viewport;      // ID/handle to the viewport that was last active in this
                        // layout when the layout was current
  Uint m_table_record;  // ID/handle of AcDbUCSTableRecord if UCS is a named
                        // UCS. If not present, then UCS is unnamed
  Uint m_table_record_base; // ID/handle of AcDbUCSTableRecord of base UCS if
                        // UCS is orthographic (76 code is non-zero). If not
                        // present and 76 code is non-zero, then base UCS is
                        // taken to be WORLD
  Uint m_shade_plot;    // Shade plot ID
};

SGAL_END_NAMESPACE

#endif
