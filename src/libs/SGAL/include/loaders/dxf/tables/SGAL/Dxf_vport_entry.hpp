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

#ifndef SGAL_DXF_VPORT_ENTRY_HPP
#define SGAL_DXF_VPORT_ENTRY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_table_entry.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_vport_entry : Dxf_table_entry {
  String m_name;        // Viewport name
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
  double m_lower_left[2]; // Lower-left corner of viewport
  double m_upper_right[2]; // Upper-right corner of viewport
  double m_view_center[2]; // View center point (in DCS)
  double m_snap_base[2]; // Snap base point (in DCS)
  double m_snap_spacing[2]; // Snap spacing X and Y
  double m_grid_spacing[2]; // Grid spacing X and Y
  double m_view_direction[3]; // View direction from target point (in WCS)
  double m_view_target[3]; // View target point (in WCS)
  double m_lens_length; // Lens length
  double m_front_clipping_plane; // Front clipping plane (offset from target
                        // point)
  double m_back_clipping_plane; // Back clipping plane (offset from target point)
  double m_view_height; // View height
  double m_aspect_ratio; // Viewport aspect ratio
  double m_snap_rotation_angle; // Snap rotation angle
  double m_view_twist_angle; // View twist angle
  String m_soft_frozen_layer; // Soft-pointer ID/handle to frozen layer objects;
                        // repeats for each frozen layers
  String m_hard_frozen_layer; // Hard-pointer ID/handle to frozen layer objects;
                        // repeats for each frozen layers
  String m_plot_style_sheet; // Plot style sheet

  int16_t m_view_mode;  // View mode (see VIEWMODE system variable)
  int16_t m_circle_sides; // Circle sides
  int16_t m_fast_zoom;  // Fast zoom setting
  int16_t m_ucs_icon;   // UCSICON setting
  int16_t m_snap_on;    // Snap on/off
  int16_t m_grid_on;    // Grid on/off
  int16_t m_snap_style; // Snap style
  int16_t m_snap_isopair; // Snap isopair
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
  int16_t m_ucs_up;     // Value of UCSVP for this viewport.
                        // 1 = viewport stores its own UCS which will become
                        //     the current UCS whenever the viewport is
                        //     activated.
                        // 0 = UCS will not change when this viewport is
                        //     activated.
  double m_ucs_origin[3]; // UCS origin 110, 120, 130
  double m_ucs_x_axis[3]; // UCS X-axis 111, 121, 131
  double m_ucs_y_axis[3]; // UCS X-axis 112, 122, 132
  String m_ucs_handle;    // ID/handle of AcDbUCSTableRecord if UCS is a named .
                        // UCS If not present, then UCS is unnamed 345
  String m_base_ucs_handle; // ID/handle of AcDbUCSTableRecord of base UCS if
                        // UCS is orthographic (79 code is non-zero). If not
                        // present and 79 code is non-zero, then base UCS is
                        // taken to be WORLD 346
  int16_t m_orthographic_type; // Orthographic type of UCS
                        // 0 = UCS is not orthographic
                        // 1 = Top
                        // 2 = Bottom
                        // 3 = Front
                        // 4 = Back
                        // 5 = Left
                        // 6 = Right
  double m_elevation;   // Elevation
  int16_t m_shade_plot_setting; // Shade plot setting
  int16_t m_major_grid_lines; // Major grid lines
  String m_background_object_pointer; // Soft-pointer ID/handle to background
                        // object (optional)
  String m_shade_plot_object_pointer; // Soft-pointer ID/handle to shade plot
                        // object (optional)
  String m_visual_style_object_pointer; // Hard-pointer ID/handle to visual
                        // style object (optional)
  bool m_is_default_lighting_on; // Default Lighting On flag
  int8_t m_default_lighting_type; // Default Lighting type
                        // 0 = One distant light
                        // 1 = Two distant lights
  double m_brightness;  // Brightness
  double m_contrast;    // Contrast
  int16_t m_ambient_color; // Ambient color (only output when non-black) 63
  int32_t m_ambient_color_i32; // Ambient color
  String m_ambient_color_name; // Ambient color
};

SGAL_END_NAMESPACE

#endif
