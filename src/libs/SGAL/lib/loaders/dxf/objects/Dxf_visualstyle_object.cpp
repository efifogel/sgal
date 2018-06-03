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

#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_visualstyle_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_visualstyle_object>
  Dxf_visualstyle_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_visualstyle_object_wrapper::Record_member>
Dxf_visualstyle_object_wrapper::s_record_members = {
  {2, {&Dxf_visualstyle_object::m_description, 1, 0}},
  {70, {&Dxf_visualstyle_object::m_type, 1, 0}},
  {71, {&Dxf_visualstyle_object::m_face_lighting_model, 1, 0}},
  {72, {&Dxf_visualstyle_object::m_face_lighting_quality, 1, 0}},
  {73, {&Dxf_visualstyle_object::m_face_color_mode, 1, 0}},
  {90, {&Dxf_visualstyle_object::m_face_modifier, 1, 0}},
  {40, {&Dxf_visualstyle_object::m_face_opacity_level, 1, 0}},
  {41, {&Dxf_visualstyle_object::m_face_specular_level, 1, 0}},
  {62, {&Dxf_visualstyle_object::m_color1, 1, 0}},
  {63, {&Dxf_visualstyle_object::m_color2, 1, 0}},
  {421, {&Dxf_visualstyle_object::m_face_style_mono_color, 1, 0}},
  {74, {&Dxf_visualstyle_object::m_edge_style_model, 1, 0}},
  {91, {&Dxf_visualstyle_object::m_edge_style, 1, 0}},
  {64, {&Dxf_visualstyle_object::m_edge_intersection_color, 1, 0}},
  {65, {&Dxf_visualstyle_object::m_edge_obscured_color, 1, 0}},
  {75, {&Dxf_visualstyle_object::m_edge_obscured_line_type, 1, 0}},
  {175, {&Dxf_visualstyle_object::m_edge_intersection_line_type, 1, 0}},
  {42, {&Dxf_visualstyle_object::m_edge_crease_angle, 1, 0}},
  {92, {&Dxf_visualstyle_object::m_edge_modifiers, 1, 0}},
  {66, {&Dxf_visualstyle_object::m_edge_color, 1, 0}},
  {43, {&Dxf_visualstyle_object::m_edge_opacity_level, 1, 0}},
  {76, {&Dxf_visualstyle_object::m_edge_width, 1, 0}},
  {77, {&Dxf_visualstyle_object::m_edge_overhang, 1, 0}},
  {78, {&Dxf_visualstyle_object::m_edge_jitter, 1, 0}},
  {67, {&Dxf_visualstyle_object::m_edge_silhouette_color, 1, 0}},
  {79, {&Dxf_visualstyle_object::m_edge_silhouette_width, 1, 0}},
  {170, {&Dxf_visualstyle_object::m_edge_halo_gap, 1, 0}},
  {171, {&Dxf_visualstyle_object::m_edge_iso_line_count, 1, 0}},
  {290, {&Dxf_visualstyle_object::m_hide_edge_line_precision, 1, 0}},
  {174, {&Dxf_visualstyle_object::m_edge_style_apply_flags, 1, 0}},
  {93, {&Dxf_visualstyle_object::m_display_style_settings, 1, 0}},
  {44, {&Dxf_visualstyle_object::m_brightness, 1, 0}},
  {173, {&Dxf_visualstyle_object::m_shadow_type, 1, 0}},
  {291, {&Dxf_visualstyle_object::m_internal_flag, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_visualstyle_object_wrapper::Record_handler_type>
Dxf_visualstyle_object_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
