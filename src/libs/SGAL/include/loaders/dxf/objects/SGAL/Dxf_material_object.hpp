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

#ifndef SGAL_DXF_MATERIAL_OBJECT_HPP
#define SGAL_DXF_MATERIAL_OBJECT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_object.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_material_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  String m_name;        // Material name (string)
  String m_description; // m_Description (string, default null string)
  int16_t m_override_ambient_color; // Ambient color method (default = 0):
                        // 0 = Use current color
                        // 1 = Override current color
  double m_ambient_color_factor; //  Ambient color factor
                        // (real, default = 1.0; valid range is 0.0 to 1.0)
  int32_t m_ambient_color_value; // Ambient color value (unsigned 32-bit
                        // integer representing an AcCmEntityColor)
  int16_t m_override_diffuse_color; // Diffuse color method (default = 0):
                        // 0 = Use current color
                        // 1 = Override current color
  double m_diffuse_color_factor; // Diffuse color factor (real,
                        // default = 1.0; valid range is 0.0 to 1.0)
  int32_t m_diffuse_color_value; // Diffuse color value (unsigned 32-bit
                        // integer representing an AcCmEntityColor)
  double m_diffuse_map_blend_factor; // Diffuse map blend factor (real,
                        // default = 1.0)
  int16_t m_diffuse_map_source; // // Diffuse map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null
                        //     file name specifies no map)
  String m_diffuse_map_file_name; // Diffuse map file name (string,
                        // default = null string)
  int16_t m_diffuse_map_projection_method; // Projection method of diffuse
                        // map mapper (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
  int16_t m_diffuse_map_tiling_method; // Tiling method of diffuse map mapper
                        // (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
  int16_t m_diffuse_map_auto_transform_method; // Auto transform method of
                        // diffuse map mapper (bitset, default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents; translate
                        //     mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  double m_diffuse_map_transformation_matrix; // Transform matrix of diffuse
                        // map mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_specular_gloss_factor; // Specular gloss factor (real, default = 0.5)
  int16_t m_override_specular_color; // Specular color method (default = 0):
                        // 0 = Use current color
                        // 1 = Override current color
  double m_specular_color_factor; // Specular color factor (real,
                        // default = 1.0; valid range is 0.0 to 1.0)
  int32_t m_specular_color_value; // Specular color value (unsigned 32-bit
                        // integer representing an AcCmEntityColor)
  double m_specular_map_blend_factor; // Specular map blend factor (real;
                        // default = 1.0)
  int16_t m_specular_map_source; // Specular map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null
                        // file name specifies no map)
  String m_specular_map_file_name; // Specular map file name (string;
                        // default = null string)
  int16_t m_specular_map_projection_method; // Projection method of specular map
                        // mapper (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
  int16_t m_specular_map_tiling_method; // Tiling method of specular map mapper
                        // (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
  int16_t m_specular_map_auto_transform_method; // Auto transform method of
                        // specular map mapper (bitset; default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents; translate
                        //     mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  double m_specular_map_transformation_matrix; // Transform matrix of specular
                        // map mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_reflection_map_blend_factor; // Blend factor of reflection map
                        // (real, default = 1.0)
  int16_t m_reflection_map_source; // Reflection map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null
                        //     file name specifies no map)
  String m_reflection_map_file_name; // Reflection map file name
                        // (string; default = null string)
  int16_t m_reflection_map_projection_method; // Projection method of reflection
                        // map mapper (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
   int16_t m_reflection_map_tiling_method; // Tiling method of reflection map
                        // mapper (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
   int16_t m_reflection_map_auto_transform_method; // Auto transform method of
                        // reflection map mapper (bitset; default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents;
                        // translate mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  String m_reflection_map_transformation_matrix; // Transform matrix of
                        // reflection map mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_opacity_factor; // Opacity percent (real; default = 1.0)
  double m_opacity_map_blend_factor; // Blend factor of opacity map (real;
                        // default = 1.0)
  int16_t m_opacity_map_source; // Opacity map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null
                        //     file name specifies no map)
  String m_opacity_map_file_name; // Opacity map file name (string;
                        // default = null string)
  int16_t m_opacity_map_projection_method; // Projection method of opacity map
                        // mapper (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
  int16_t m_opacity_map_tiling_method; // Tiling method of opacity map mapper
                        // (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
  int16_t m_opacity_map_auto_transform_method; // Auto transform method of
                        // opacity map mapper (bitset; default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents; translate
                        //     mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  double m_opacity_map_transformation_matrix; // Transform matrix of opacity map
                        // mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_bump_map_blend_factor; // Blend factor of bump map (real;
                        // default = 1.0)
  int16_t m_bump_map_source; // Bump map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null file
                        //     name specifies no map)
  String m_bump_map_file_name; // Bump map file name (string; default = null
                        // string)
  int16_t m_bump_map_projection_method; // Projection method of bump map mapper
                        // (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
  int16_t m_bump_map_tiling_method; // Tiling method of bump map mapper
                        // (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
  int16_t m_bump_map_auto_transform_method; // Auto transform method of bump map
                        // mapper (bitset; default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents; translate
                        //     mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  double m_bump_map_transformation_matrix; // Transform matrix of bump map
                        // mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_refraction_index; // Refraction index (real; default = 1.0)
  double m_refraction_map_blend_factor; // Blend factor of refraction map (real;
                        // default = 1.0)
  int16_t m_refraction_map_source; // Refraction map source (default = 1):
                        // 0 = Use current scene
                        // 1 = Use image file (specified by file name; null file
                        //     name specifies no map)
  String m_refraction_map_file_name; // Refraction map file name
                        // (string; default = null string)
  int16_t m_refraction_map_projection_method; // Projection method of refraction
                        // map mapper (default = 1):
                        // 1 = Planar
                        // 2 = Box
                        // 3 = Cylinder
                        // 4 = Sphere
  int16_t m_refraction_map_tiling_method; // Tiling method of refraction map
                        // mapper (default = 1):
                        // 1 = Tile
                        // 2 = Crop
                        // 3 = Clamp
  int16_t m_refraction_map_auto_transform_method; // Auto transform method of
                        // refraction map mapper (bitset; default = 1):
                        // 1 = No auto transform
                        // 2 = Scale mapper to current entity extents; translate
                        //     mapper to entity origin
                        // 4 = Include current block transform in mapper
                        //     transform
  double m_refraction_map_transformation_matrix; // Transform matrix of
                        // refraction map mapper (16 reals; row major format;
                        // default = identity matrix)
  double m_color_bleed_scale; // Color Bleed Scale
  double m_indirect_dump_scale; // Indirect Dump Scale
  double m_reflectance_scale; // Reflectance Scale
  double m_transmittance_scale; // Transmittance Scale
  bool m_is_two_sided;  // Two-sided Material
  double m_luminance;   // Luminance
  int16_t m_luminance_mode; // Luminance Mode
  int16_t m_normal_map_method; // Normal Map Method
  double m_normal_map_strength; // Normal Map Strength
  double m_normal_map_blend_factor; // Normal Map Blend Factor
  int16_t m_normal_map_source; // Normal Map Source
  String m_normal_map_file_name; // Normal Map Source File Name
  int16_t m_normal_map_projection_method; // Normal Mapper Projection
  int16_t m_normal_map_tiling_method; // Normal Mapper Tiling
  int16_t m_normal_map_auto_transform_method; // Normal Mapper Auto Transform
  double m_normal_map_transformation_matrix; // Normal Mapper Transform
  bool m_is_anonymous;  // Material is Anonymous
  int16_t m_global_illumination_mode; // Global Illumination Mode
  int16_t m_final_gather_mode; // Final Gather Mode
  String m_gen_proc_name; // GenProcName
  bool m_gen_proc_boolean_value; // GenProcValBool
  int16_t m_gen_proc_integer_value; // GenProcValInt
  double m_gen_proc_real_value; // GenProcValReal
  String m_gen_proc_text_value; // GenProcValText
  bool m_gen_proc_table_end; // GenProcTableEnd
  int16_t m_gen_proc_color_index_value; // GenProcValColorIndex
  int32_t m_gen_proc_color_rgb_value; // GenProcValColorRGB
  String m_gen_proc_color_name; // GenProcValColorName
  int16_t m_map_u_tile; // Map UTile
  double m_translucence; // Translucence
  int32_t m_self_illumination; // Self-Illuminaton
  double m_reflectivity; // Reflectivity
  int32_t m_illumination_model; // Illumination Model
  int32_t m_channel_flags; // Channel Flags
};

SGAL_END_NAMESPACE

#endif
