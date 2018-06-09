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

#ifndef SGAL_DXF_VISUALSTYLE_OBJECT_HPP
#define SGAL_DXF_VISUALSTYLE_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_visualstyle_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  /// \name Data members
  //@{

  String m_description;         // Description
  int16_t m_type;               // Type
  int16_t m_face_lighting_model; // Face lighting model
                                // 0 =Invisible
                                // 1 = Visible
                                // 2 = Phong
                                // 3 = Gooch
  int16_t m_face_lighting_quality; // Face lighting quality
                                // 0 = No lighting
                                // 1 = Per face lighting
                                // 2 = Per vertex lighting
  int16_t m_face_color_mode;    // Face color mode
                                // 0 = No color
                                // 1 = Object color
                                // 2 = Background color
                                // 3 = Custom color
                                // 4 = Mono color
                                // 5 = Tinted
                                // 6 = Desaturated
  int32_t m_face_modifier;      // Face modifiers
                                // 0 = No modifiers
                                // 1 = Opacity
                                // 2 = Specular
  double m_face_opacity_level;  //
  double m_face_specular_level; // Face specular level

  int16_t m_color1;             // Color
  int16_t m_color2;             // Color
  int32_t m_face_style_mono_color; // Face style mono color
  int16_t m_edge_style_model;   // Edge style model
                                // 0 = No edges
                                // 1 = Isolines
                                // 2 = Facet edges
  int32_t m_edge_style;         // Edge style
  int16_t m_edge_intersection_color; // Edge intersection color
  int16_t m_edge_obscured_color; // Edge obscured color
  int16_t m_edge_obscured_line_type; // Edge obscured linetype
  int16_t m_edge_intersection_line_type; // Edge intersection line type
  double m_edge_crease_angle;   // Edge crease angle
  int32_t m_edge_modifiers;     // Edge modifiers
  int16_t m_edge_color;         // Edge color
  double m_edge_opacity_level;  // Edge opacity level
  int16_t m_edge_width;         // Edge width
  int16_t m_edge_overhang;      // Edge overhang
  int16_t m_edge_jitter;        // Edge jitter
  int16_t m_edge_silhouette_color; // Edge silhouette color
  int16_t m_edge_silhouette_width; // Edge silhouette width
  int16_t m_edge_halo_gap;      // Edge halo gap
  int16_t m_edge_iso_line_count; // Number of edge isolines
  bool m_hide_edge_line_precision; // Edge hide precision flag
  int16_t m_edge_style_apply_flags; // Edge style apply flag
  int32_t m_display_style_settings; // Display style display settings
  double m_brightness;          // Brightness
  int16_t m_shadow_type;        // Shadow type
  bool m_internal_flag;         // Internal use only flag

  //@}
};

SGAL_END_NAMESPACE

#endif
