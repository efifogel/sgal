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

#ifndef DXF_TEXT_ENTITY_HPP
#define DXF_TEXT_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_text_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  double m_thickness;   // Thickness (optional; default = 0)
  double m_location[3]; // First alignment point (in OCS)
  double m_text_height; // Text height
  SGAL::String m_value; // Default value (the string itself)
  double m_rotation;    // Text rotation (optional; default = 0)
  double m_relative_x_scale_factor; // Relative X scale factor—width (optional;
                        // default = 1)
                        // This value is also adjusted when fit-type text is
                        // used
  double m_oblique_angle; // Oblique angle (optional; default = 0)
  SGAL::String m_text_style_name; // Text style name (optional,
                        // default = STANDARD)
  int16_t m_text_generation_flags; // Text generation flags (optional,
                        // default = 0):
                        // 2 = Text is backward (mirrored in X)
                        // 4 = Text is upside down (mirrored in Y)
  int16_t m_horizontal_text_justification; // Horizontal text justification type
                        // (optional, default = 0) integer codes (not bit-coded):
                        // 0 = Left
                        // 1= Center
                        // 2 = Right
                        // 3 = Aligned (if vertical alignment = 0)
                        // 4 = Middle (if vertical alignment = 0)
                        // 5 = Fit (if vertical alignment = 0)
                        // See the Group 72 and 73 integer codes table for
                        // clarification
  double m_second_alignment_point[3]; // Second alignment point (in OCS)
                        // (optional)
  double m_normal[3];   // Extrusion direction (optional; default = 0, 0, 1)
  int16_t m_vertical_text_justification; // Vertical text justification type
                        // (optional, default = 0): integer codes (not
                        // bit-coded):
                        // 0 = Baseline
                        // 1 = Bottom
                        // 2 = Middle
                        // 3 = Top
                        // See the Group 72 and 73 integer codes table for
                        // clarification
};

DXF_END_NAMESPACE

#endif
