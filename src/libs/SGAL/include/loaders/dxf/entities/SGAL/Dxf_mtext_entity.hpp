// Copyright (c) 2004,2018 Israel.
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

#ifndef SGAL_DXF_MTEXT_ENTITY_HPP
#define SGAL_DXF_MTEXT_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_mtext_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  /// \name Data members
  //@{

  double m_insertion_point[3]; // Insertion point
  double m_initial_text_height; // Nominal (initial) text height
  double m_reference_rectangle_width; // Reference rectangle width
  int16_t m_attachment_point; // Attachment point:
                        // 1 = Top left
                        // 2 = Top center
                        // 3 = Top right
                        // 4 = Middle left
                        // 5 = Middle center
                        // 6 = Middle right
                        // 7 = Bottom left
                        // 8 = Bottom center
                        // 9 = Bottom right
  int16_t m_Ddrawing_direction; // rawing direction:
                        // 1 = Left to right
                        // 3 = Top to bottom
                        // 5 = By style (the flow direction is inherited
                        // from the associated text style)
  String m_test;        // Text string. If the text string is less than 250
                        // characters, all characters appear in group 1. If the
                        // text string is greater than 250 characters, the
                        // string is divided into 250-character chunks, which
                        // appear in one or more group 3 codes. If group 3 codes
                        // are used, the last group is a group 1 and has fewer
                        // than 250 characters
  String m_extended_text; // Additional text (always in 250-character chunks)
                        // (optional)
  String m_text_style_name; // Text style name (STANDARD if not provided)
                        // (optional)
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)
  double m_x_axis_direction[3]; // X-axis direction vector (in WCS)
                        // A group code 50 (rotation angle in radians) passed as
                        // DXF input is converted to the equivalent direction
                        // vector (if both a code 50 and codes 11, 21, 31 are
                        // passed, the last one wins). This is provided as a
                        // convenience for conversions from text objects
  double m_horizontal_width; // Horizontal width of the characters that make up
                        // the mtext entity. This value will always be equal to
                        // or less than the value of group code 41 (read-only,
                        // ignored if supplied)
  double m_vertical_height; // Vertical height of the mtext entity (read-only,
                        // ignored if supplied)
  double m_rotation_angle; // Rotation angle in radians
  int16_t m_line_spacing_style; // Mtext line spacing style (optional):
                        // 1 = At least (taller characters will override)
                        // 2 = Exact (taller characters will not override)
  double m_line_spacing_factor; // Mtext line spacing factor (optional):
                        // Percentage of default (3-on-5) line spacing to be
                        // applied. Valid values range from 0.25 to 4.00
  int32_t m_background_fill_setting; // Background fill setting:
                        // 0 = Background fill off
                        // 1 = Use background fill color
                        // 2 = Use drawing window color as background fill color
  int32_t m_background_color_rgb; // Background color (if RGB color)
  String m_background_color_name; // Background color (if color name)
  double m_fill_box_scale; // Fill box scale (optional):
                        // Determines how much border there is around the text.
  int16_t m_background_fill_color; // Background fill color (optional): Color
                        // to use for background fill when group code 90 is 1.
  int32_t m_background_fill_color_transparency; // Transparency of background
                        // fill color (not implemented)
  int16_t m_column_type; // Column type
  int16_t m_column_count; // Column count
  int16_t m_is_column_flow_reversed; // Column Flow Reversed
  int16_t m_is_column_auto_height; // Column Autoheight
  double m_column_width; // Column width
  double m_column_gutter; // Column gutter
  double m_column_heights; // Column heights; this code is followed by a column
                        // count (Int16), and then the number of column heights

  //@}

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(Dxf_parser& /* parser */, int code, double value);
};

SGAL_END_NAMESPACE

#endif
