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

#ifndef SGAL_DXF_PLOTSETTINGS_OBJECT_HPP
#define SGAL_DXF_PLOTSETTINGS_OBJECT_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_plotsettings_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;
  String m_page_setup_name; // Page Setup name
  String m_printer_name; // Name of system printer or plot configuration file
  String m_paper_size;  // Paper size
  String m_plot_view_name; // Plot view name
  double m_unprintable_left_margin_size; // Size, in millimeters, of
                        // unprintable margin on left side of paper
  double m_unprintable_bottom_margin_size; // Size, in millimeters, of
                        // unprintable margin on bottom of paper
  double m_unprintable_right_margin_size; // Size, in millimeters, of
                        // unprintable margin on right side of paper
  double m_unprintable_top_margin_size; // Size, in millimeters, of
                        // unprintable margin on top of paper
  double m_paper_width; // Plot paper size: physical paper width in millimeters
  double m_paper_height; // Plot paper size: physical paper height in millimeters
  double m_plot_origin_x_value; // Plot origin: X value of origin offset in
                        // millimeters
  double m_plot_origin_y_value; // Plot origin: Y value of origin offset in
                        // millimeters
  double m_plot_window_lower_left_x_value; // Plot window area: X value of
                        // lower-left window corner
  double m_plot_window_lower_left_y_value; // Plot window area: Y value of
                        // upper-right window corner
  double m_plot_window_upper_right_x_value; // Plot window area: X value of
                        // lower-left window corner
  double m_plot_window_upper_right_y_value; // Plot window area: Y value of
                        // upper-right window corner
  double m_custom_print_scale_numerator; // Numerator of custom print scale:
                        // real world (paper) units
  double m_custom_print_scale_denominator; // Denominator of custom print scale:
                        // drawing units
  int16_t m_plot_layout_flags; // Plot layout flag:
                        // 1 = PlotViewportBorders
                        // 2 = ShowPlotStyles
                        // 4 = PlotCentered
                        // 8 = PlotHidden
                        // 16 = UseStandardScale
                        // 32 = PlotPlotStyles
                        // 64 = ScaleLineweights
                        // 128 = PrintLineweights
                        // 512 = DrawViewportsFirst
                        // 1024 = ModelType
                        // 2048 = UpdatePaper
                        // 4096 = ZoomToPaperOnUpdate
                        // 8192 = Initializing
                        // 16384 = PrevPlotInit
  int16_t m_plot_paper_units; // Plot paper units:
                        // 0 = Plot in inches
                        // 1 = Plot in millimeters
                        // 2 = Plot in pixels
  int16_t m_plot_rotation; // Plot rotation:
                        // 0 = No rotation
                        // 1 = 90 degrees counterclockwise
                        // 2 = Upside-down
                        // 3 = 90 degrees clockwise
  int16_t m_plot_type;  // Plot type (portion of paper space to output to the
                        // media):
                        // 0 = Last screen display
                        // 1 = Drawing extents
                        // 2 = Drawing limits
                        // 3 = View specified by code 6
                        // 4 = Window specified by codes 48, 49, 140, and 141
                        // 5 = Layout information
  String m_current_style_sheet; // Current style sheet
  int16_t m_standard_scale; // Standard scale type:
                        // 0 = Scaled to Fit
                        // 1 = 1/128"=1'; 2 = 1/64"=1'; 3 = 1/32"=1'
                        // 4 = 1/16"=1'; 5 = 3/32"=1'; 6 = 1/8"=1'
                        // 7 = 3/16"=1'; 8 = 1/4"=1'; 9 = 3/8"=1'
                        // 10 = 1/2"=1'; 11 = 3/4"=1'; 12 = 1"=1'
                        // 13 = 3"=1'; 14 = 6"=1'; 15 = 1'=1'
                        // 16 = 1:1 ; 17= 1:2; 18 = 1:4; 19 = 1:8; 20 = 1:10;
                        // 21 = 1:16
                        // 22 = 1:20; 23 = 1:30; 24 = 1:40; 25 = 1:50;
                        // 26 = 1:100
                        // 27 = 2:1; 28 = 4:1; 29 = 8:1; 30 = 10:1; 31 = 100:1;
                        // 32 = 1000:1
  int16_t m_shade_plot_mode; // ShadePlot mode:
                        // 0 = As Displayed
                        // 1 = Wireframe
                        // 2 = Hidden
                        // 3 = Rendered
  int16_t m_shade_plot_resolution_level; // ShadePlot resolution level:
                        // 0 = Draft
                        // 1 = Preview
                        // 2 = Normal
                        // 3 = Presentation
                        // 4 = Maximum
                        // 5 = Custom
  int16_t m_shade_plot_custom_dpi; // ShadePlot custom DPI:
  // Valid range: 100 to 32767
  // Only applied when the ShadePlot resolution level is set to 5 (Custom)
  double m_standard_scale_value; // A floating point scale factor that
                        // represents the standard scale value specified in code
                        // 75
  double m_paper_image_origin_x; // Paper image origin: X value
  double m_paper_image_origin_y; // Paper image origin: Y value
  String m_shade_plot_object; // ShadePlot ID/Handle (optional)

  /*! Handle a marker.
   */
  bool handle_marker(const String& marker);
};

SGAL_END_NAMESPACE

#endif
