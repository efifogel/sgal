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
//
// EF: Codes 10 & 20 are duplicated???

#ifndef SGAL_DXF_HATCH_ENTITY_HPP
#define SGAL_DXF_HATCH_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"
#include "SGAL/Dxf_boundary_path.hpp"
#include "SGAL/Dxf_pattern_data.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_hatch_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  /// Member records
  //@{

  double m_elevation_point[3]; // Elevation point (in OCS)
  double m_extrusion_direction[3]; // Extrusion direction
                        // (optional; default = 0, 0, 1)
  String m_hatch_pattern_name; // Hatch pattern name
  int16_t m_flags;      // Solid fill flag (0 = pattern fill; 1 = solid fill);
                        // for MPolygon, the version of MPolygon
  int16_t m_pattern_fill_color; // For MPolygon, pattern fill color as the ACI
  int16_t m_associativity_flags; // Associativity flag
                        // 0 = non-associative;
                        // 1 = associative;
                        // for MPolygon, solid-fill flag
                        // 0 = lacks solid fill;
                        // 1 = has solid fill
  // int32_t Number of boundary paths (loops)
  std::vector<Dxf_boundary_path*> m_boundary_paths; // Boundary path data.
                        // Repeats number of times specified by code 91.
  int16_t m_hatch_style; // Hatch style:
                        // 0 = Hatch "odd parity" area (Normal style)
                        // 1 = Hatch outermost area only (Outer style)
                        // 2 = Hatch through entire area (Ignore style)
  int16_t m_hatch_patter_type; // Hatch pattern type:
                        // 0 = User-defined
                        // 1 = Predefined
                        // 2 = Custom
  double m_hatch_patter_angle; // Hatch pattern angle (pattern fill only)
  double m_hatch_patter_scale; // Hatch pattern scale or spacing (pattern fill
                        // only)
  int16_t m_boundary_annotation_flag; // For MPolygon, boundary annotation flag:
                        // 0 = boundary is not an annotated boundary
                        // 1 = boundary is an annotated boundary
  int16_t m_hatch_patter_double_flag;// Hatch pattern double flag (pattern fill
                        // only):
                        // 0 = not double
                        // 1 = double
  // int16_t Number of pattern definition lines
  std::vector<Dxf_pattern_data> m_pattern_line; // Pattern line data.
                        // Repeats number of times specified by code 78.
  double m_pixel_size;  // Pixel size used to determine the density to perform
                        // various intersection and ray casting operations in
                        // hatch pattern computation for associative hatches and
                        // hatches created with the Flood method of hatching
  int32_t m_num_sed_points;  // Number of seed points
  double m_offset_vetor; // For MPolygon, offset vector
  int32_t m_num_loops;  // For MPolygon, number of degenerate boundary paths
                        // (loops), where a degenerate boundary path is a border
                        // that is ignored by the hatch
  double m_seed_point[3]; // Seed point (in OCS)
  int32_t m_solid_hatch; // Indicates solid hatch or gradient; if solid hatch,
                        // the values for the remaining codes are ignored but
                        // must be present. Optional; if code 450 is in the
                        // file, then the following codes must be in the file:
                        // 451, 452, 453, 460, 461, 462, and 470. If code 450 is
                        // not in the file, then the following codes must not be
                        // in the file: 451, 452, 453, 460, 461, 462, and 470
                        // 0 = Solid hatch
                        // 1 = Gradient
  int32_t m_reserved1;  // Zero is reserved for future use
  int32_t m_color_defined;  // Records how colors were defined and is used only
                        // by dialog code:
                        // 0 = Two-color gradient
                        // 1 = Single-color gradient
  int32_t m_num_colors; // Number of colors:
                        // 0 = Solid hatch
                        // 2 = Gradient
  double m_rotation_angle; // Rotation angle in radians for gradients  0,
                        // (default = 0)
  double m_gradient;    // Gradient definition; corresponds to the Centered
                        // option on the Gradient Tab of the Boundary Hatch and
                        // Fill dialog box. Each gradient has two definitions,
                        // shifted and non-shifted. A Shift value describes the
                        // blend of the two definitions that should be used. A
                        // value of 0.0 means only the non-shifted version
                        // should be used, and a value of 1.0 means that only
                        // the shifted version should be used.
  double m_color_tint_value; // Color tint value used by dialog code
                        // (default = 0, 0; range is 0.0 to 1.0). The color tint
                        // value is a gradient color and controls the degree of
                        // tint in the dialog when the Hatch group code 452 is
                        // set to 1.
  double m_reserved2;   // Reserved for future use:
                        // 0 = First value
                        // 1 = Second value
  String m_string;      // String (default = LINEAR)

  //@}

  /// Member handlers
  //@{

  /*! Handle boundary paths.
   */
  void handle_boundary_paths(int32_t value);

  /*! Handle a value that requires special handling.
   */
  void handle_pattern_definition_lines_num(int16_t size);

  //@}
};

SGAL_END_NAMESPACE

#endif
