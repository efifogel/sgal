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

#ifndef SGAL_DXF_VIEW_ENTRY_HPP
#define SGAL_DXF_VIEW_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_table_entry.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_view_entry: public Dxf_table_entry {
  String m_name;        // Style name
  int16_t m_flags;      // 16 = If set, table entry is externally dependent on
                        //      an xref
                        // 32 = If both this bit and bit 16 are set, the
                        //      externally dependent xref has been successfully
                        // resolved
                        // 64 = If set, the table entry was referenced by at
                        //      least one entity in the drawing the last time
                        //      the drawing was edited. (This flag is for the
                        //      benefit of AutoCAD commands. It can be ignored
                        //      by most programs that read DXF files and does
                        //      not need to be set by programs that write DXF
                        //      files)
  double m_view_height; // View height (in DCS)
  double m_view_center_point[2]; // View center point (in DCS)
  double m_view_width;  // View width (in DCS)
  double m_view_direction[3]; // View direction from target (in WCS)
  double m_target_point[3]; // Target point (in WCS)
  double m_lens_length; // Lens length
  double m_front_clipping_plane; // Front clipping plane (offset from target
                        // point)
  double m_back_clipping_plane; // Back clipping plane (offset from target
                        // point)
  double m_twist_angle; // Twist angle
  int16_t m_view_mode;  // View mode (see VIEWMODE system variable)
  int8_t m_render_mode; // Render mode:
                        // 0 = 2D Optimized (classic 2D)
                        // 1 = Wireframe
                        // 2 = Hidden line
                        // 3 = Flat shaded
                        // 4 = Gouraud shaded
                        // 5 = Flat shaded with wireframe
                        // 6 = Gouraud shaded with wireframe
                        // All rendering modes other than 2D Optimized engage
                        // the new 3D graphics pipeline. These values directly
                        // correspond to the SHADEMODE command and the
                        // AcDbAbstractViewTableRecord::RenderMode enum
  int16_t m_is_associated_ucs_present; // 1 if there is a UCS associated to
                        // this view; 0 otherwise
  int16_t m_is_camera_plottable; // 1 if the camera is plottable
  Uint m_background_object_pointer; // Soft-pointer ID/handle to background
                        // object (optional)
  Uint m_section_object_pointer; // Soft-pointer ID/handle to live section
                        // object (optional)
  Uint m_visual_style_object_pointer; // Hard-pointer ID/handle to visual style
                        // object (optional)
  Uint m_sun_ownership_pointer; // Sun hard ownership ID
  double m_ucs_origin[3]; // UCS origin (appears only if code 72 is set to 1)
  double m_ucs_x_axis[3]; // UCS X-axis (appears only if code 72 is set to 1)
  double m_ucs_y_axis[3]; // UCS Y-axis (appears only if code 72 is set to 1)
  int16_t m_orthographic_view_type; // Orthographic type of UCS (appears only
                        // if code 72 is set to 1):
                        // 0 = UCS is not orthographic;
                        // 1 = Top; 2 = Bottom;
                        // 3 = Front; 4 = Back;
                        // 5 = Left; 6 = Right
  double m_ucs_elevation; // UCS Elevation (appears only if code 72 is set to 1)
  Uint m_ucs_handle;    // ID/handle of AcDbUCSTableRecord if UCS is a named
                        // UCS.  If not present, then UCS is unnamed. (appears
                        // only if code 72 is set to 1)
  Uint m_base_ucs_handle; // ID/handle of AcDbUCSTableRecord of base UCS if UCS
                        // is orthographic (79 code is non-zero).  If not
                        // present and 79 code is non-zero, then base UCS is
                        // taken to be WORLD. (appears only if code 72 is set to
                        // 1)
};

SGAL_END_NAMESPACE

#endif
