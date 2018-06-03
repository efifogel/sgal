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
#include "SGAL/Dxf_material_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_material_object>  Dxf_material_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_material_object_wrapper::Record_member>
Dxf_material_object_wrapper::s_record_members = {
  {1, {&Dxf_material_object::m_name, 1, 0}},
  {2, {&Dxf_material_object::m_description, 1, 0}},
  {70, {&Dxf_material_object::m_override_ambient_color, 1, 0}},
  {40, {&Dxf_material_object::m_ambient_color_factor, 1, 0}},
  {90, {&Dxf_material_object::m_ambient_color_value, 1, 0}},
  {71, {&Dxf_material_object::m_override_diffuse_color, 1, 0}},
  {41, {&Dxf_material_object::m_diffuse_color_factor, 1, 0}},
  {91, {&Dxf_material_object::m_diffuse_color_value, 1, 0}},
  {42, {&Dxf_material_object::m_diffuse_map_blend_factor, 1, 0}},
  {72, {&Dxf_material_object::m_diffuse_map_source, 1, 0}},
  {3, {&Dxf_material_object::m_diffuse_map_file_name, 1, 0}},
  {73, {&Dxf_material_object::m_diffuse_map_projection_method, 1, 0}},
  {74, {&Dxf_material_object::m_diffuse_map_tiling_method, 1, 0}},
  {75, {&Dxf_material_object::m_diffuse_map_auto_transform_method, 1, 0}},
  {43, {&Dxf_material_object::m_diffuse_map_transformation_matrix, 1, 0}},
  {44, {&Dxf_material_object::m_specular_gloss_factor, 1, 0}},
  {76, {&Dxf_material_object::m_override_specular_color, 1, 0}},
  {45, {&Dxf_material_object::m_specular_color_factor, 1, 0}},
  {92, {&Dxf_material_object::m_specular_color_value, 1, 0}},
  {46, {&Dxf_material_object::m_specular_map_blend_factor, 1, 0}},
  {77, {&Dxf_material_object::m_specular_map_source, 1, 0}},
  {4, {&Dxf_material_object::m_specular_map_file_name, 1, 0}},
  {78, {&Dxf_material_object::m_specular_map_projection_method, 1, 0}},
  {79, {&Dxf_material_object::m_specular_map_tiling_method, 1, 0}},
  {170, {&Dxf_material_object::m_specular_map_auto_transform_method, 1, 0}},
  {47, {&Dxf_material_object::m_specular_map_transformation_matrix, 1, 0}},
  {48, {&Dxf_material_object::m_reflection_map_blend_factor, 1, 0}},
  {171, {&Dxf_material_object::m_reflection_map_source, 1, 0}},
  {6, {&Dxf_material_object::m_reflection_map_file_name, 1, 0}},
  {172, {&Dxf_material_object::m_reflection_map_projection_method, 1, 0}},
  {173, {&Dxf_material_object::m_reflection_map_tiling_method, 1, 0}},
  {174, {&Dxf_material_object::m_reflection_map_auto_transform_method, 1, 0}},
  {49, {&Dxf_material_object::m_reflection_map_transformation_matrix, 1, 0}},
  {140, {&Dxf_material_object::m_opacity_factor, 1, 0}},
  {141, {&Dxf_material_object::m_opacity_map_blend_factor, 1, 0}},
  {175, {&Dxf_material_object::m_opacity_map_source, 1, 0}},
  {7, {&Dxf_material_object::m_opacity_map_file_name, 1, 0}},
  {176, {&Dxf_material_object::m_opacity_map_projection_method, 1, 0}},
  {177, {&Dxf_material_object::m_opacity_map_tiling_method, 1, 0}},
  {178, {&Dxf_material_object::m_opacity_map_auto_transform_method, 1, 0}},
  {142, {&Dxf_material_object::m_opacity_map_transformation_matrix, 1, 0}},
  {143, {&Dxf_material_object::m_bump_map_blend_factor, 1, 0}},
  {179, {&Dxf_material_object::m_bump_map_source, 1, 0}},
  {8, {&Dxf_material_object::m_bump_map_file_name, 1, 0}},
  {270, {&Dxf_material_object::m_bump_map_projection_method, 1, 0}},
  {271, {&Dxf_material_object::m_bump_map_tiling_method, 1, 0}},
  {272, {&Dxf_material_object::m_bump_map_auto_transform_method, 1, 0}},
  {144, {&Dxf_material_object::m_bump_map_transformation_matrix, 1, 0}},
  {145, {&Dxf_material_object::m_refraction_index, 1, 0}},
  {146, {&Dxf_material_object::m_refraction_map_blend_factor, 1, 0}},
  {273, {&Dxf_material_object::m_refraction_map_source, 1, 0}},
  {9, {&Dxf_material_object::m_refraction_map_file_name, 1, 0}},
  {274, {&Dxf_material_object::m_refraction_map_projection_method, 1, 0}},
  {275, {&Dxf_material_object::m_refraction_map_tiling_method, 1, 0}},
  {276, {&Dxf_material_object::m_refraction_map_auto_transform_method, 1, 0}},
  {147, {&Dxf_material_object::m_refraction_map_transformation_matrix, 1, 0}},
  {460, {&Dxf_material_object::m_color_bleed_scale, 1, 0}},
  {461, {&Dxf_material_object::m_indirect_dump_scale, 1, 0}},
  {462, {&Dxf_material_object::m_reflectance_scale, 1, 0}},
  {463, {&Dxf_material_object::m_transmittance_scale, 1, 0}},
  {290, {&Dxf_material_object::m_is_two_sided, 1, 0}},
  {464, {&Dxf_material_object::m_luminance, 1, 0}},
  {270, {&Dxf_material_object::m_luminance_mode, 1, 0}},
  {271, {&Dxf_material_object::m_normal_map_method, 1, 0}},
  {465, {&Dxf_material_object::m_normal_map_strength, 1, 0}},
  {42, {&Dxf_material_object::m_normal_map_blend_factor, 1, 0}},
  {72, {&Dxf_material_object::m_normal_map_source, 1, 0}},
  {3, {&Dxf_material_object::m_normal_map_file_name, 1, 0}},
  {73, {&Dxf_material_object::m_normal_map_projection_method, 1, 0}},
  {74, {&Dxf_material_object::m_normal_map_tiling_method, 1, 0}},
  {75, {&Dxf_material_object::m_normal_map_auto_transform_method, 1, 0}},
  {43, {&Dxf_material_object::m_normal_map_transformation_matrix, 1, 0}},
  {293, {&Dxf_material_object::m_is_anonymous, 1, 0}},
  {272, {&Dxf_material_object::m_global_illumination_mode, 1, 0}},
  {273, {&Dxf_material_object::m_final_gather_mode, 1, 0}},
  {300, {&Dxf_material_object::m_gen_proc_name, 1, 0}},
  {291, {&Dxf_material_object::m_gen_proc_boolean_value, 1, 0}},
  {271, {&Dxf_material_object::m_gen_proc_integer_value, 1, 0}},
  {469, {&Dxf_material_object::m_gen_proc_real_value, 1, 0}},
  {301, {&Dxf_material_object::m_gen_proc_text_value, 1, 0}},
  {292, {&Dxf_material_object::m_gen_proc_table_end, 1, 0}},
  {62, {&Dxf_material_object::m_gen_proc_color_index_value, 1, 0}},
  {420, {&Dxf_material_object::m_gen_proc_color_rgb_value, 1, 0}},
  {430, {&Dxf_material_object::m_gen_proc_color_name, 1, 0}},
  {270, {&Dxf_material_object::m_map_u_tile, 1, 0}},
  {148, {&Dxf_material_object::m_translucence, 1, 0}},
  {90, {&Dxf_material_object::m_self_illumination, 1, 0}},
  {468, {&Dxf_material_object::m_reflectivity, 1, 0}},
  {93, {&Dxf_material_object::m_illumination_model, 1, 0}},
  {94, {&Dxf_material_object::m_channel_flags, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_material_object_wrapper::Record_handler_type>
Dxf_material_object_wrapper::s_record_handlers = {};

SGAL_END_NAMESPACE
