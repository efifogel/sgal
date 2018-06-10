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

#ifndef DXF_DIMENSION_ENTITY_HPP
#define DXF_DIMENSION_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_dimension_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  int8_t m_version;     // Version number:
                        // 0 = 2010
  SGAL::String m_block_name;  // Name of the block that contains the entities
                        // that make up the dimension picture
  double m_definition_point_1[3]; // Definition point (in WCS)
  double m_text_mid_point[3]; // Middle point of dimension text (in OCS)
  int16_t m_dimension_type; // Dimension type:
                        // Values 0-6 are integer values that represent the
                        // dimension type. Values 32, 64, and 128 are bit
                        // values, which are added to the integer values (value
                        // 32 is always set in R13 and later releases)
                        // 0 = Rotated, horizontal, or vertical
                        // 1 = Aligned
                        // 2 = Angular
                        // 3 = Diameter
                        // 4 = Radius
                        // 5 = Angular 3-point
                        // 6 = Ordinate
                        // 32 = Indicates that the block reference (group code
                        //      2) is referenced by this dimension only
                        // 64 = Ordinate type. This is a bit value (bit 7) used
                        //      only with integer value 6. If set, ordinate is
                        //      X-type; if not set, ordinate is Y-type
                        // 128 = This is a bit value (bit 8) added to the other
                        //       group 70 values if the dimension text has been
                        //       positioned at a user-defined location rather
                        // than at the default location
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
  int16_t m_text_line_spacing_style; // Dimension text line-spacing style
                        // (optional):
                        // 1 (or missing) = At least (taller characters will
                        //                  override)
                        // 2 = Exact (taller characters will not override)
  double m_text_line_spacing_factor; // Dimension text-line spacing factor
                        // (optional):
                        // Percentage of default (3-on-5) line spacing to be
                        // applied. Valid values range from 0.25 to 4.00
  double m_actual_measurement; // Actual measurement (optional; read-only value)
  SGAL::String m_text;        // Dimension text explicitly entered by the user.
                        // Optional; default is the measurement.
                        // If null or "<>", the dimension measurement is drawn
                        // as the text, if "" (one blank space), the text is
                        // suppressed. Anything else is drawn as the text
  double m_text_rotation_angle; // The optional group code 53 is the rotation
                        // angle of the dimension text away from its default
                        // orientation (the direction of the dimension line)
                        // (optional)
  double m_horizontal_direction_angle; // All dimension types have an optional
                        // 51 group code, which indicates the horizontal
                        // direction for the dimension entity. The dimension
                        // entity determines the orientation of dimension text
                        // and lines for horizontal, vertical, and rotated
                        // linear dimensions
                        // This group value is the negative of the angle between
                        // the OCS X axis and the UCS X axis. It is always in
                        // the XY plane of the OCS
  double m_normal[3];   // Extrusion direction (optional; default = 0, 0, 1)
  SGAL::String m_dimension_style_name; // Dimension style name
};

DXF_END_NAMESPACE

#endif
