// Copyright (c) 2004 Israel.
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
// $Source$
// $Revision: 10984 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GFX_HPP
#define SGAL_GFX_HPP

/*! \file
 *
 */

#include <string.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Bit_mask.hpp"
#include "SGAL/Vector4ub.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Texture;
class Halftone;
class Tex_gen;
class Material;

#define SGAL_GFX_STRUCT \
  Float m_alpha_ref; \
  Gfx::Tex_mode m_tex_mode; \
  Texture* m_texture; \
  Vector4f m_tex_blend_color; \
  Gfx::Tex_env m_tex_env; \
  Tex_gen* m_tex_gen; \
  Material* m_material; \
  Gfx::Shade_model m_shade_model; \
  Gfx::Transparency_mode m_transp_mode; \
  Gfx::Alpha_func m_alpha_func; \
  Vector4f m_blend_color; \
  Gfx::Src_blend_func m_src_blend_func; \
  Gfx::Dst_blend_func m_dst_blend_func; \
  Vector4ub m_color_mask; \
  Gfx::Depth_func m_depth_func; \
  Boolean m_depth_mask; \
  Gfx::Poly_mode m_poly_mode; \
  Gfx::Cull_face m_cull_face; \
  Gfx::Light_model_sides m_light_model_sides; \
  Gfx::Light_model_color_control m_light_model_color_control; \
  Gfx::Material_mode m_material_mode_enable; \
  Uint m_line_stipple_pattern; \
  Uint m_line_stipple_factor; \
  Matrix4f m_tex_transform; \
  Material* m_back_material; \
  Float m_line_width; \
  Float m_point_size; \
  Halftone* m_halftone; \
 \
  Boolean m_tex_enable; \
  Boolean m_tex_gen_enable; \
  Boolean m_light_enable; \
  Boolean m_transp_enable; \
  Boolean m_fog_enable; \
  Boolean m_depth_enable;  \
  Boolean m_polygon_stipple_enable; \
  Boolean m_pad[1]

class Gfx {
public:
#define SGAL_STATE_ELEMENTS_ENABLE \
  TEX_ENABLE,\
  TEX_GEN_ENABLE, \
  LIGHT_ENABLE, \
  TRANSP_ENABLE, \
  FOG_ENABLE, \
  DEPTH_ENABLE, \
  MATERIAL_MODE_ENABLE, \
  POLYGON_STIPPLE_ENABLE

#define SGAL_STATE_ELEMENTS_OTHER \
  TEXTURE, \
  TEX_MODE, \
  TEX_BLEND_COLOR, \
  TEX_ENV, \
  TEX_GEN, \
  SHADE_MODEL, \
  TRANSP_MODE, \
  ALPHA_FUNC, \
  ALPHA_REF, \
  BLEND_COLOR, \
  SRC_BLEND_FUNC, \
  DST_BLEND_FUNC, \
  COLOR_MASK, \
  DEPTH_FUNC, \
  DEPTH_MASK, \
  POLY_MODE, \
  LINE_STIPPLE_PATTERN, \
  LINE_STIPPLE_FACTOR, \
  TEX_TRANSFORM, \
  MATERIAL, \
  BACK_MATERIAL, \
  HALFTONE_PATTERN

#define SGAL_STATE_ELEMENTS \
  SGAL_STATE_ELEMENTS_OTHER, \
  SGAL_STATE_ELEMENTS_ENABLE

  // We use a bit-mask of size 64 bits. The bit mask accomodates the state
  // elements. Currently, there are 36 state elements.
  enum {
    BEGIN_STATE_ELEMENTS = -1,
    SGAL_STATE_ELEMENTS,
    CULL_FACE,
    LIGHT_MODEL_SIDES,
    LIGHT_MODEL_COLOR_CONTROL,
    LINE_WIDTH,
    POINT_SIZE,
    INHERIT,
    NUM_STATE_ELEMENTS
  };

  enum Tex_mode {
    FAST_TEX,
    NICE_TEX,
    NON_PERSP_TEX,
    PERSP_TEX
  };

  enum Tex_env {
    MODULATE_TENV,
    BLEND_TENV,
    REPLACE_TENV,
    ADD_TENV,
    DECAL_TENV
  };

  enum Shade_model {
    FLAT_SHADE,
    SMOOTH_SHADE
  };

  enum Transparency_mode {
    FAST_TRANSP,
    NICE_TRANSP,
    BLEND_TRANSP,
    SCREEN_DOOR_TRANSP
  };

  enum Alpha_func {
    NEVER_AFUNC,
    LESS_AFUNC,
    EQUAL_AFUNC,
    LEQUAL_AFUNC,
    GREATER_AFUNC, 
    NOTEQUAL_AFUNC,
    GEQUAL_AFUNC,
    ALWAYS_AFUNC
  };

  enum Src_blend_func {
    ZERO_SBLEND,
    ONE_SBLEND,
    DST_COLOR_SBLEND,
    ONE_MINUS_DST_COLOR_SBLEND,
    SRC_ALPHA_SATURATE_SBLEND, 
    SRC_ALPHA_SBLEND,
    ONE_MINUS_SRC_ALPHA_SBLEND,
    DST_ALPHA_SBLEND, 
    ONE_MINUS_DST_ALPHA_SBLEND
  };

  enum Dst_blend_func {
    ZERO_DBLEND,
    ONE_DBLEND,
    SRC_COLOR_DBLEND,
    ONE_MINUS_SRC_COLOR_DBLEND,
    SRC_ALPHA_DBLEND,
    ONE_MINUS_SRC_ALPHA_DBLEND,
    DST_ALPHA_DBLEND,
    ONE_MINUS_DST_ALPHA_DBLEND
  };

  enum Depth_func {
    NEVER_DFUNC,
    LESS_DFUNC,
    EQUAL_DFUNC,
    LEQUAL_DFUNC,
    GREATER_DFUNC,
    NOTEQUAL_DFUNC,
    GEQUAL_DFUNC,
    ALWAYS_DFUNC
  };

  enum Poly_mode {
    POINT_PMODE, 
    LINE_PMODE, 
    FILL_PMODE
  };

  enum Cull_face {
    NO_CULL, 
    FRONT_CULL, 
    BACK_CULL, 
    BOTH_CULL
  };

  enum Light_model_sides {
    ONE_SIDE,
    TWO_SIDE
  };

  enum Light_model_color_control {
    SEPARATE_SPECULAR_COLOR,
    SINGLE_COLOR
  };
  
  enum Clear_bits {
    COLOR_CLEAR = 0x1, 
    DEPTH_CLEAR = 0x2,
    STENCIL_CLEAR = 0x4
  };

  enum Color_mode {
    COLORMODE_RGB,
    COLORMODE_INDEX
  };

  enum Material_mode {
    NO_COLOR_MATERIAL,
    COLOR_MATERIAL
  };

  Bit_mask m_pending;
  Bit_mask m_override;

  SGAL_GFX_STRUCT;

  void copy(const Gfx & gfx)
  {
    m_pending.set(gfx.m_pending);
    m_override.set(gfx.m_override);
    ::memcpy(&m_alpha_ref, &gfx.m_alpha_ref,
             (size_t) &m_pad[1] - (size_t) &m_alpha_ref);
  }
  static Gfx::Depth_func find_depth_function(const char * name);
};

SGAL_END_NAMESPACE

#endif
