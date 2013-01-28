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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#include "gl2ps.h"

#include "SGAL/basic.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Override_geo_prop.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Halftone.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Open_gl_os_init.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
//! \todo #include "Win_handle.h"

SGAL_BEGIN_NAMESPACE

#define GL2PS

#if defined(NO_PLUGIN)
Pointer_array* Context::s_contexts = NULL;
#else
Context* Context::s_context = NULL;
#endif

Context* Context::s_current_context = NULL;

// NextId, must start at 1 so we don't clobber GL default targets of 0.
// Int Context::s_nextId = 1; 
// IntArray* Context::s_freeIdArray = 0;

#if !defined(NO_AUDIO)
Ref_array* Context::s_sounds_array = NULL;
Ref_array* Context::s_sound_players_array = NULL;
Boolean Context::s_sound_initialized = false;
#endif

/*! Constructor */
Context::Context() :
  //! \todo m_win_handle(0),
  m_active_camera(0)
{ init(); }

/*! \brief */
/*! \todo Window_handle
void Context::set_win_handle(Window_handle* win_handle)
{
  m_win_handle = win_handle;
  init();
}
*/

/*! \brief */
/*! \todo Window_handle
Window_handle* Context::get_win_handle()
{ return m_win_handle; }
*/

/*! \brief */
/*! \todo Window_handle
Window_handle* Context::get_win_handle() const
{ return m_win_handle; }
*/

/*! \brief */
void Context::init()    
{
  m_mat_stack_depth = 1;
  m_light_stack_depth = 0;
  m_fog_stack_depth = 0;

  // Bottom of fog stack contains an empty, disabled fog so that when the
  // last fog is popped, fog will be disabled
  // Fog* baseFog = new Fog;
  // baseFog->set_On(false);
  // baseFog->ref();
  // m_fogStack[0] = baseFog;
  
  // State stack starts out at zero and increases with pushes
  m_stack_depth = 0;

  // Override stack has not ever been pushed so initialize it to -1
  m_override_app_stack_top = -1;
  m_override_geo_prop_stack_top = -1;

  // Initialize the accumulated graphics state
  m_current_state = &m_current_state_stack[m_stack_depth];

  // Initialize the default context state:
  m_default_state = &m_default_state_;

  // Set all the bits so that a pop state at the original
  // top of the stack will restore to the default state.

  m_current_state->m_pending.off();
  m_current_state->m_override.off();
    
  m_current_state->m_texture               = 0;
  m_current_state->m_halftone              = 0;
  m_current_state->m_tex_enable            = false;
  m_current_state->m_tex_mode              = Gfx::FAST_TEX;
  m_current_state->m_tex_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_current_state->m_tex_env               = Gfx::MODULATE_TENV;
  m_current_state->m_tex_gen               = 0;
  m_current_state->m_tex_gen_enable        = false;
  m_current_state->m_material              = 0;
  m_current_state->m_material_mode_enable  = Gfx::NO_COLOR_MATERIAL;
  m_current_state->m_light_enable          = false;
  m_current_state->m_shade_model           = Gfx::SMOOTH_SHADE;
  m_current_state->m_transp_enable         = false;
  m_current_state->m_transp_mode           = Gfx::BLEND_TRANSP;
  m_current_state->m_alpha_func            = Gfx::ALWAYS_AFUNC;
  m_current_state->m_alpha_ref             = 0.0f;
  m_current_state->m_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_current_state->m_src_blend_func        = Gfx::ONE_SBLEND;
  m_current_state->m_dst_blend_func        = Gfx::ZERO_DBLEND;
  m_current_state->m_color_mask.set(0xff, 0xff, 0xff, 0xff);
  m_current_state->m_depth_enable          = false;
  m_current_state->m_depth_func            = Gfx::LESS_DFUNC;
  m_current_state->m_depth_mask            = true;
  m_current_state->m_fog_enable            = false;
  m_current_state->m_poly_mode             = Gfx::FILL_PMODE;
  m_current_state->m_cull_face             = Gfx::NO_CULL;
  m_current_state->m_light_model_sides     = Gfx::ONE_SIDE;
  m_current_state->m_line_width            = 1.0f;
  m_current_state->m_point_size            = 1.0f;
  m_current_state->m_line_stipple_factor   = 1;
  m_current_state->m_line_stipple_pattern  = 0xffff;
  m_current_state->m_tex_transform.make_identity();
  m_current_state->m_back_material         = 0;
  m_current_state->m_polygon_stipple_enable = false;

  // Copy TopStack to default_state so they start out the same
  m_default_state_.copy(m_current_state_stack[0]);
  m_default_state_.m_pending.on();     // Drawing of all elemnets is pending

  // Don't enable normal vector normalization by default
  m_normalize_enable = false;

  // Set local viewer by default
  m_local_viewer = true;

  m_last_app = NULL;

  m_is_current = false;
  m_made_current = false;

#if defined(_WIN32)
//! \todo  m_gfx_handle = new Open_gl_win32;
#elif defined(_X11)
//! \todo  m_gfx_handle = new Open_gl_x11;
#elif defined(_MAC)
//! \todo  m_gfx_handle = new Open_gl_mac;
#endif

  //! \todo m_gfx_handle->set_window(m_win_handle);

  // Populate configuration:
  init_context_attributes();

  // Remember this context:
#if defined(NO_PLUGIN)
  for (size_t i = 0; i < s_contexts->get_count(); i++) {
    if (s_contexts->get(i) != 0) {
      break;
    }
  }
  s_contexts->set(i, this);
#else
  s_context = this;
#endif
    
    // Make sure all the lights are initially undefined
  Light_target* lights = m_light_stack[m_light_stack_depth];

  for (int i = 0; i < SGAL_MAX_LIGHTS; i++) {
    lights[i].m_set = false;
    lights[i].m_enabled = false;
    lights[i].m_defined = 0;
  }

  // Make the default color white
  glColor4f(1, 1, 1, 1);

  // initialize the viewport
  m_viewport[0] = 0;
  m_viewport[1] = 0;
  m_viewport[2] = 0;
  m_viewport[3] = 0;
}

/*! \brief */
void Context::init_context_attributes()
{
  int tmp;
  glGetIntegerv(GL_RED_BITS, &tmp); m_red_bits = tmp;
  glGetIntegerv(GL_GREEN_BITS, &tmp); m_green_bits = tmp;
  glGetIntegerv(GL_BLUE_BITS, &tmp); m_blue_bits = tmp;
  glGetIntegerv(GL_ALPHA_BITS, &tmp); m_alpha_bits = tmp;
  glGetIntegerv(GL_DEPTH_BITS, &tmp); m_depth_bits = tmp;
  glGetIntegerv(GL_STENCIL_BITS, &tmp); m_stencil_bits = tmp;
  glGetIntegerv(GL_ACCUM_RED_BITS, &tmp); m_accum_red_bits = tmp;
  glGetIntegerv(GL_ACCUM_GREEN_BITS, &tmp); m_accum_green_bits = tmp;
  glGetIntegerv(GL_ACCUM_BLUE_BITS, &tmp); m_accum_blue_bits = tmp;
  glGetIntegerv(GL_ACCUM_ALPHA_BITS, &tmp); m_accum_alpha_bits = tmp;
}

/*! Destructor */
Context::~Context()
{
  //! \todo delete m_gfx_handle;

#if defined(NO_PLUGIN)
  for (size_t i = 0; i < s_contexts->get_count(); i++) {
    if (s_contexts->get(i) == this) {
      s_contexts->set(i, 0);
      break;
    }
  }
#else
  s_context = 0;
#endif
}

/*! \brief */
void Context::set_viewport(Uint x, Uint y, Uint w, Uint h)
{
  if (static_cast<Uint>(m_viewport[0]) != x ||
      static_cast<Uint>(m_viewport[1]) != y ||
      static_cast<Uint>(m_viewport[2]) != w ||
      static_cast<Uint>(m_viewport[3]) != h)
  {
    m_viewport[0] = static_cast<Int>(x);
    m_viewport[1] = static_cast<Int>(y);
    m_viewport[2] = static_cast<Int>(w);
    m_viewport[3] = static_cast<Int>(h);
    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
  }
}

/*! \brief */
void Context::get_viewport(Uint& x, Uint& y, Uint& w, Uint& h) const
{
  x = (Uint) m_viewport[0];
  y = (Uint) m_viewport[1];
  w = (Uint) m_viewport[2];
  h = (Uint) m_viewport[3];
}

/*! \brief */
void Context::set_normalize_enable(Boolean normalize_enable)
{
  if (normalize_enable == m_normalize_enable) return;
  m_normalize_enable = normalize_enable;    
  if (normalize_enable) glEnable(GL_NORMALIZE);
  else glDisable(GL_NORMALIZE);
}

/*! \brief */
void Context::set_local_viewer(Boolean local_viewer)
{
  if (local_viewer == m_local_viewer) return;
  m_local_viewer = local_viewer;
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer);
}

/*! \brief */
void Context::set_texture(Texture* texture)
{
  m_default_state->m_texture = texture;
  m_default_state->m_pending.on_bit(Gfx::TEXTURE);
  // DrawTexture(texture);
}

/*! \brief */
void Context::draw_texture(Texture* texture)
{
  std::cout << "Context::draw_texture" << texture << std::endl;
  if ((m_current_state->m_texture == texture) &&
      (!texture || !texture->is_dirty()))
    return;

  std::cout << "Context::draw_texture 2" << texture << std::endl;
  m_current_state->m_texture = texture;
  if (texture) texture->draw(this);
}

/*! \brief */
void Context::set_halftone(Halftone* halftone)
{
  m_default_state->m_halftone = halftone;
  m_default_state->m_pending.on_bit(Gfx::HALFTONE_PATTERN);
}

/*! \brief */
void Context::draw_halftone(Halftone* halftone)
{
  if ((m_current_state->m_halftone == halftone) &&
      (!halftone || !halftone->is_dirty()))
    return;

  m_current_state->m_halftone = halftone;
  if (halftone) halftone->draw(this);
}

/*! \brief */
void Context::set_tex_enable(Boolean tex_enable)
{
  m_default_state->m_tex_enable = tex_enable;
  m_default_state->m_pending.on_bit(Gfx::TEX_ENABLE);
  // DrawTexEnable(tex_enable);
}

/*! \brief */
void Context::draw_tex_enable(Boolean tex_enable)
{
  if (m_current_state->m_tex_enable == tex_enable) return;
  m_current_state->m_tex_enable = tex_enable;
  if (tex_enable) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
}

/*! \brief */
void Context::set_tex_mode(Gfx::Tex_mode tex_mode)
{
  m_default_state->m_tex_mode = tex_mode;
  m_default_state->m_pending.on_bit(Gfx::TEX_MODE);
  // DrawTex_mode(tex_mode);
}

/*! \brief */
void Context::draw_tex_mode(Gfx::Tex_mode tex_mode)
{
  if (m_current_state->m_tex_mode == tex_mode) return;
  m_current_state->m_tex_mode = tex_mode;
}

/*! \brief */
void Context::set_tex_blend_color(const Vector4f& tex_blend_color)
{
  m_default_state->m_tex_blend_color = tex_blend_color;
  m_default_state->m_pending.on_bit(Gfx::TEX_BLEND_COLOR);
  // DrawTexBlendColor(tex_blend_color);
}

/*! \brief */
void Context::draw_tex_blend_color(const Vector4f& color)
{
  if (m_current_state->m_tex_blend_color == color) return;
  m_current_state->m_tex_blend_color = color;
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (GLfloat*) &color);
}

/*! \brief */
void Context::set_tex_env(Gfx::Tex_env tex_env)
{
  m_default_state->m_tex_env = tex_env;
  m_default_state->m_pending.on_bit(Gfx::TEX_ENV);
  // DrawTexEnv(tex_env);
}

/*! \brief */
void Context::draw_tex_env(Gfx::Tex_env tex_env)
{
  if (m_current_state->m_tex_env == tex_env) return;
  m_current_state->m_tex_env = tex_env;
  static GLenum s_tex_env_tokens[] = {
    GL_MODULATE,
    GL_BLEND,
    GL_REPLACE,
    GL_ADD,
    GL_DECAL
  };
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, s_tex_env_tokens[tex_env]);
}

/*! \brief */
void Context::set_tex_gen(Tex_gen* tex_gen)
{
  m_default_state->m_tex_gen = tex_gen;
  m_default_state->m_pending.on_bit(Gfx::TEX_GEN);
  // DrawTexGen(tex_gen);
}

/*! \brief */
void Context::draw_tex_gen(Tex_gen* tex_gen)
{
  if ((m_current_state->m_tex_gen == tex_gen) &&
      (tex_gen && !tex_gen->is_changed()))
    return;
  m_current_state->m_tex_gen = tex_gen;
  if (tex_gen) tex_gen->draw(this);
}

/*! \brief */
void Context::set_tex_gen_enable(Boolean tex_gen_enable)
{
  m_default_state->m_tex_gen_enable = tex_gen_enable;
  m_default_state->m_pending.on_bit(Gfx::TEX_GEN_ENABLE);
  // Drawtex_gen_enable(tex_gen_enable);
}

/*! \brief */
void Context::draw_tex_gen_enable(Boolean tex_gen_enable)
{
  if (m_current_state->m_tex_gen_enable == tex_gen_enable) return;
  m_current_state->m_tex_gen_enable = tex_gen_enable;

  if (tex_gen_enable) {
    Tex_gen* tgen = m_current_state->m_tex_gen;
    if (tgen) {
      if (tgen->get_mode_s() != Tex_gen::OFF) glEnable(GL_TEXTURE_GEN_S);
      if (tgen->get_mode_t() != Tex_gen::OFF) glEnable(GL_TEXTURE_GEN_T);
      if (tgen->get_mode_r() != Tex_gen::OFF) glEnable(GL_TEXTURE_GEN_R);
      if (tgen->get_mode_q() != Tex_gen::OFF) glEnable(GL_TEXTURE_GEN_Q);
    }
  } else {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
}

/*! \brief */
void Context::set_material(Material* material)
{
  m_default_state->m_material = material;
  m_default_state->m_pending.on_bit(Gfx::MATERIAL);
  // DrawMaterial(material);
}

/*! \brief If the material and the back material are the same, apply to front
 * and back
 */
void Context::draw_material(Material* material, Material* back_material)
{
  if ((m_current_state->m_material == material) &&
      material && !material->is_changed())
    return;

  m_current_state->m_material = material;

  if (material) {
    material->draw((back_material == material) ?
                   Material::FRONT_AND_BACK : Material::FRONT,
                   this);
  }
}

/*! \brief */
void Context::set_light_enable(Boolean light_enable)
{
  m_default_state->m_light_enable = light_enable;
  m_default_state->m_pending.on_bit(Gfx::LIGHT_ENABLE);
  // DrawLightEnable(light_enable);
}

/*! \brief */
void Context::draw_light_enable(Boolean light_enable)
{
  if (m_current_state->m_light_enable == light_enable) return;
  m_current_state->m_light_enable = light_enable;
  if (light_enable) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
}

/*! \brief */
void Context::set_shade_model(Gfx::Shade_model shade_model)
{
  m_default_state->m_shade_model = shade_model;
  m_default_state->m_pending.on_bit(Gfx::SHADE_MODEL);
  // DrawShade_model(shade_model);
}

/*! \brief */
void Context::draw_shade_model(Gfx::Shade_model shade_model)
{
  if (m_current_state->m_shade_model == shade_model) return;
  m_current_state->m_shade_model = shade_model;

  static GLenum sShade_model_tokens[] = {GL_FLAT, GL_SMOOTH};
  glShadeModel(sShade_model_tokens[shade_model]);
}

/*! \brief */
void Context::set_transp_enable(Boolean transp_enable)
{
  m_default_state->m_transp_enable = transp_enable;
  if (transp_enable) {  // Need to enable transparency
    switch (m_default_state->m_transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      set_blend_funcs(Gfx::SRC_ALPHA_SBLEND, Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      set_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
    }
  }
  else {    // Need to disable transparency
    switch (m_default_state->m_transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      set_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      break;
    }
  }
  m_default_state->m_pending.on_bit(Gfx::TRANSP_ENABLE);
  // DrawTranspEnable(transp_enable);
}

/*! \brief */
void Context::draw_transp_enable(Boolean transp_enable)
{
  if (m_current_state->m_transp_enable == transp_enable) return;
  m_current_state->m_transp_enable = transp_enable;
  if (transp_enable) {  // Need to enable transparency
    switch (m_current_state->m_transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      draw_blend_funcs(Gfx::SRC_ALPHA_SBLEND, Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      glEnable(GL_POLYGON_STIPPLE);
      draw_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
    }
  } else {    // Need to disable transparency
    switch (m_current_state->m_transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      draw_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      glDisable(GL_POLYGON_STIPPLE);
      break;
    }
  }
}

/*! \brief */
void Context::set_transp_mode(Gfx::Transparency_mode transp_mode)
{
  m_default_state->m_transp_mode = transp_mode;
  if (transp_mode) {  // Need to enable transparency
    switch (transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      set_blend_funcs(Gfx::SRC_ALPHA_SBLEND, Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      set_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
    }
  }
  m_default_state->m_pending.on_bit(Gfx::TRANSP_MODE);
  // DrawTransp_mode(transp_mode);
}

/*! \brief */
void Context::draw_transp_mode(Gfx::Transparency_mode transp_mode)
{
  if (m_current_state->m_transp_mode == transp_mode) return;
  m_current_state->m_transp_mode = transp_mode;
  if (m_current_state->m_transp_enable) {  // Need to enable transparency
    switch (transp_mode) {
     case Gfx::NICE_TRANSP:
     case Gfx::BLEND_TRANSP:
      glDisable(GL_POLYGON_STIPPLE);
      draw_blend_funcs(Gfx::SRC_ALPHA_SBLEND, Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      break;
     case Gfx::FAST_TRANSP:
     case Gfx::SCREEN_DOOR_TRANSP:
      glEnable(GL_POLYGON_STIPPLE);
      draw_blend_funcs(Gfx::ONE_SBLEND, Gfx::ZERO_DBLEND);
      break;
    }
  }
}

/*! \brief */
static GLenum s_alpha_func_tokens[] = 
{
  GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, 
  GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS 
};

/*! \brief */
void Context::set_alpha_func(Gfx::Alpha_func alpha_func)
{
  m_default_state->m_alpha_func = alpha_func;
  m_default_state->m_pending.on_bit(Gfx::ALPHA_FUNC);
  // draw_alpha_func(alpha_func);
}

/*! \brief */
void Context::draw_alpha_func(Gfx::Alpha_func alpha_func)
{
  if (m_current_state->m_alpha_func == alpha_func) return;
  m_current_state->m_alpha_func = alpha_func;
  if (alpha_func != GL_ALWAYS) {
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(s_alpha_func_tokens[alpha_func], m_current_state->m_alpha_ref);
  }
  else glDisable(GL_ALPHA_TEST);
}

/*! \brief */
void Context::set_alpha_ref(float alpha_ref)
{
    m_default_state->m_alpha_ref = alpha_ref;
    m_default_state->m_pending.on_bit(Gfx::ALPHA_REF);
    // draw_alpha_ref(alpha_ref);
}

/*! \brief */
void Context::draw_alpha_ref(float alpha_ref)
{
  if (m_current_state->m_alpha_ref == alpha_ref) return;
  m_current_state->m_alpha_ref = alpha_ref;
  glAlphaFunc(s_alpha_func_tokens[m_current_state->m_alpha_func], alpha_ref);
}

/*! \brief */
void Context::set_blend_color(const Vector4f& blend_color)
{
  m_default_state->m_blend_color = blend_color;
  m_default_state->m_pending.on_bit(Gfx::BLEND_COLOR);
  // DrawBlendColor(blend_color);
}

/*! \brief */
void Context::draw_blend_color(const Vector4f& blend_color)
{
  if (m_current_state->m_blend_color == blend_color) return;
  m_current_state->m_blend_color = blend_color;
}

/*! \brief */
static GLenum s_src_blend_func_tokens[] = 
{
  GL_ZERO, GL_ONE, GL_DST_COLOR, 
  GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE, 
  GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, 
  GL_ONE_MINUS_DST_ALPHA, 
};

/*! \brief */
static GLenum s_dst_blend_func_tokens[] = 
{
  GL_ZERO, GL_ONE, GL_SRC_COLOR, 
  GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, 
  GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, 
  GL_ONE_MINUS_DST_ALPHA, 
};

/*! \brief */
void Context::set_src_blend_func(Gfx::Src_blend_func src_blend_func)
{
  m_default_state->m_src_blend_func = src_blend_func;
  m_default_state->m_pending.on_bit(Gfx::SRC_BLEND_FUNC);
  // DrawSrc_blend_func(src_blend_func);
}

/*! \brief */
void Context::draw_src_blend_func(Gfx::Src_blend_func src_blend_func)
{
  if (m_current_state->m_src_blend_func == src_blend_func) return;
  m_current_state->m_src_blend_func = src_blend_func;
  if (m_current_state->m_dst_blend_func == Gfx::ZERO_DBLEND) {
    if (src_blend_func == Gfx::ONE_SBLEND) glDisable(GL_BLEND);
    else glEnable(GL_BLEND);
  }
  glBlendFunc(s_src_blend_func_tokens[src_blend_func], 
              s_dst_blend_func_tokens[m_current_state->m_dst_blend_func]);
}

/*! \brief */
void Context::set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func)
{
  m_default_state->m_dst_blend_func = dst_blend_func;
  m_default_state->m_pending.on_bit(Gfx::DST_BLEND_FUNC);
  // DrawDstBlendFunc(dst_blend_func);
}

/*! \brief */
void Context::draw_dst_blend_func(Gfx::Dst_blend_func dst_blend_func)
{
  if (m_current_state->m_dst_blend_func == dst_blend_func) return;
  m_current_state->m_dst_blend_func = dst_blend_func;
  if (m_current_state->m_src_blend_func == Gfx::ONE_SBLEND) {
    if (dst_blend_func == Gfx::ZERO_DBLEND) glDisable(GL_BLEND);
    else glEnable(GL_BLEND);
  }
  glBlendFunc(s_src_blend_func_tokens[m_current_state->m_src_blend_func], 
              s_dst_blend_func_tokens[dst_blend_func]);
}

/*! \brief */
void Context::set_color_mask(const Vector4ub& color_mask)
{
  m_default_state->m_color_mask = color_mask;
  m_default_state->m_pending.on_bit(Gfx::COLOR_MASK);
  // DrawColorMask(color_mask);
}

/*! \brief */
void Context::draw_color_mask(const Vector4ub& color_mask)
{
  if (m_current_state->m_color_mask == color_mask) return;
  m_current_state->m_color_mask = color_mask;
  glColorMask(color_mask[0] != 0, color_mask[1] != 0, 
              color_mask[2] != 0, color_mask[3] != 0);
}

/*! \brief */
void Context::set_depth_enable(Boolean depth_enable)
{
  m_default_state->m_depth_enable = depth_enable;
  m_default_state->m_pending.on_bit(Gfx::DEPTH_ENABLE);
  // DrawDepthEnable(depth_enable);
}

/*! \brief */
void Context::draw_depth_enable(Boolean depth_enable)
{
  if (m_current_state->m_depth_enable == depth_enable) return;
  m_current_state->m_depth_enable = depth_enable;
  if (depth_enable) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);
}

/*! \brief */
void Context::set_depth_func(Gfx::Depth_func depth_func)
{
  m_default_state->m_depth_func = depth_func;
  m_default_state->m_pending.on_bit(Gfx::DEPTH_FUNC);
  // DrawDepthFunc(depth_func);
}

/*! \brief */
void Context::draw_depth_func(Gfx::Depth_func depth_func)
{
  if (m_current_state->m_depth_func == depth_func) return;
  m_current_state->m_depth_func = depth_func;
  static GLenum s_depth_func_tokens[] = {
    GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, 
    GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS 
  };
  glDepthFunc(s_depth_func_tokens[depth_func]);
}

/*! \brief */
void Context::set_depth_mask(Boolean depth_mask)
{
  m_default_state->m_depth_mask = depth_mask;
  m_default_state->m_pending.on_bit(Gfx::DEPTH_MASK);
  // DrawDepthMask(depth_mask);
}

/*! \brief */
void Context::draw_depth_mask(Boolean depth_mask)
{
  if (m_current_state->m_depth_mask == depth_mask) return;
  m_current_state->m_depth_mask = depth_mask;
  glDepthMask(depth_mask);
}

/*! \brief */
void Context::set_fog_enable(Boolean fog_enable)
{
  m_default_state->m_fog_enable = fog_enable;
  m_default_state->m_pending.on_bit(Gfx::FOG_ENABLE);
  // DrawFogEnable(fog_enable);
}

/*! \brief */
void Context::draw_fog_enable(Boolean /* fog_enable */)
{
#if 0
  // if the current fog is turned off, dont allow it to be enabled
  if (m_fogStackDepth > 1 && !m_fogStack[m_fogStackDepth]->get_on())
    fog_enable = false;

  if (m_current_state->m_fog_enable == fog_enable) return;

  m_current_state->m_fog_enable = fog_enable;
  if (fog_enable) glEnable(GL_FOG);
  else glDisable(GL_FOG);
#endif
}

/*! \brief */
void Context::set_polygon_stipple_enable(Boolean enable)
{
  m_default_state->m_polygon_stipple_enable = enable;
  m_default_state->m_pending.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
}

/*! \brief */
void Context::draw_polygon_stipple_enable(Boolean enable)
{
  if (m_current_state->m_polygon_stipple_enable == enable) return;
  m_current_state->m_polygon_stipple_enable = enable;
  if (enable) glEnable(GL_POLYGON_STIPPLE);
  else glDisable(GL_POLYGON_STIPPLE);
}

/*! \brief */
void Context::set_material_mode_enable(Gfx::Material_mode material_mode_enable)
{
  m_default_state->m_material_mode_enable = material_mode_enable;
  m_default_state->m_pending.on_bit(Gfx::MATERIAL_MODE_ENABLE);
  // DrawMaterial_mode_enable(material_mode_enable);
}

/*! \brief */
void Context::draw_material_mode_enable(Gfx::Material_mode material_mode_enable)
{
  if (m_current_state->m_material_mode_enable == material_mode_enable) return;
  m_current_state->m_material_mode_enable = material_mode_enable;

  if (material_mode_enable == Gfx::COLOR_MATERIAL) {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  }
  else {
    glDisable(GL_COLOR_MATERIAL);

    if (m_current_state->m_material) {
      m_current_state->m_material->draw(Material::FRONT, this);
    }

    if (m_current_state->m_back_material) {
      m_current_state->m_back_material->draw(Material::BACK, this);
    }
  }
}

/*! \brief */
void Context::set_poly_mode(Gfx::Poly_mode poly_mode)
{
  m_default_state->m_poly_mode = poly_mode;
  m_default_state->m_pending.on_bit(Gfx::POLY_MODE);
  // DrawPoly_mode(poly_mode);
}

/*! \brief */
void Context::draw_poly_mode(Gfx::Poly_mode poly_mode)
{
  if (m_current_state->m_poly_mode == poly_mode) return;
  m_current_state->m_poly_mode = poly_mode;
  static GLenum sPoly_modeTokens[] = {GL_POINT, GL_LINE, GL_FILL};
  glPolygonMode(GL_FRONT_AND_BACK, sPoly_modeTokens[poly_mode]);
}

/*! \brief */
void Context::set_cull_face(Gfx::Cull_face cull_face)
{
  m_default_state->m_cull_face = cull_face;
  m_default_state->m_pending.on_bit(Gfx::CULL_FACE);
  // DrawCull_face(cull_face);
}

/*! \brief */
void Context::draw_cull_face(Gfx::Cull_face cull_face)
{
  // Use override value if one has been set
  if (m_override_geo_prop_stack_top != -1) {
    Bit_mask geo_override;
    m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
      get_override(geo_override);

    if (geo_override.get_bit(Gfx::CULL_FACE)) {
      cull_face =
        m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
        get_cull_face();
    }
  }

  if (m_current_state->m_cull_face == cull_face) return;

  m_current_state->m_cull_face = cull_face;
  static GLenum sCull_faceTokens[] = {
    GL_FALSE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
  };
  if (cull_face == Gfx::NO_CULL) glDisable(GL_CULL_FACE);
  else {
    glEnable(GL_CULL_FACE);
    glCullFace(sCull_faceTokens[cull_face]);
  }
}

void Context::set_light_model_sides(Gfx::Light_model_sides light_model_sides)
{
  m_default_state->m_light_model_sides = light_model_sides;
  m_default_state->m_pending.on_bit(Gfx::LIGHT_MODEL_SIDES);
  // DrawLight_model_sides(light_model_sides);
}

/*! \brief */
void Context::draw_light_model_sides(Gfx::Light_model_sides light_model_sides)
{
  // Use override value if one has been set
  if (m_override_geo_prop_stack_top != -1) {
    Bit_mask geo_override;
    m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
      get_override(geo_override);

    if (geo_override.get_bit(Gfx::LIGHT_MODEL_SIDES)) {
      light_model_sides =
        m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
        get_light_model_sides();
    }
  }

  if (m_current_state->m_light_model_sides == light_model_sides) return;
  m_current_state->m_light_model_sides = light_model_sides;

  if (light_model_sides == Gfx::ONE_SIDE)
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, false);
  else
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
}

/*! \brief */
void Context::set_line_width(Float line_width)
{
  m_default_state->m_line_width = line_width;
  m_default_state->m_pending.on_bit(Gfx::LINE_WIDTH);
  // DrawLine_width(line_width);
}

/*! \brief */
void Context::draw_line_width(Float line_width)
{
  // Use override value if one has been set
  if (m_override_geo_prop_stack_top != -1) {
    Bit_mask geo_override;
    m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
      get_override(geo_override);
    
    if (geo_override.get_bit(LINE_WIDTH)) {
      line_width =
        m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
        get_line_width();
    }
  }

  if (m_current_state->m_line_width == line_width) return;
  m_current_state->m_line_width = line_width;
  glLineWidth(line_width);
#ifdef GL2PS
  gl2psLineWidth(line_width);
#endif
}

/*! \brief */
void Context::set_point_size(Float point_size)
{   
  m_default_state->m_point_size = point_size;
  m_default_state->m_pending.on_bit(Gfx::POINT_SIZE);
  // DrawPointSize(point_size);
}

/*! \brief */
void Context::draw_point_size(Float point_size)
{   
  // Use override value if one has been set
  if (m_override_geo_prop_stack_top != -1) {
    Bit_mask geo_override;
    m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
      get_override(geo_override);

    if (geo_override.get_bit(POINT_SIZE)) {
      point_size =
        m_override_geo_prop_stack[m_override_geo_prop_stack_top]->
        get_point_size();
    }
  }
        
  if (m_current_state->m_point_size == point_size) return;
  m_current_state->m_point_size = point_size;
  glPointSize(point_size);
  gl2psPointSize(point_size);
}   

/*! \brief */
void Context::set_line_stipple_pattern(Uint pattern)
{
  m_default_state->m_line_stipple_pattern = pattern;
  m_default_state->m_pending.on_bit(Gfx::LINE_STIPPLE_PATTERN);
  // DrawLineStipplePattern(pattern);
}

/*! \brief */
void Context::draw_line_stipple_pattern(Uint pattern)
{
  if (m_current_state->m_line_stipple_pattern == pattern) return;
  m_current_state->m_line_stipple_pattern = pattern;
  if (pattern == 0xffff) glDisable(GL_LINE_STIPPLE);
  else {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(m_current_state->m_line_stipple_factor, pattern);
  }
}

/*! \brief */
void Context::set_line_stipple_factor(Uint factor)
{
  m_default_state->m_line_stipple_factor = factor;
  m_default_state->m_pending.on_bit(Gfx::LINE_STIPPLE_FACTOR);
  // DrawLineStippleFactor(factor);
}

/*! \brief */
void Context::draw_line_stipple_factor(Uint factor)
{
  if (m_current_state->m_line_stipple_factor == factor) return;
  m_current_state->m_line_stipple_factor = factor;
  if (m_current_state->m_line_stipple_pattern != 0xffff) {
    glLineStipple(factor, m_current_state->m_line_stipple_pattern);
  }
}

/*! \brief */
void Context::set_tex_transform(const Matrix4f& matrix)
{
  m_default_state->m_tex_transform = matrix;
  m_default_state->m_pending.on_bit(Gfx::TEX_TRANSFORM);
  // DrawTexTransform(matrix);
}

/*! \brief */
void Context::draw_tex_transform(const Matrix4f& matrix)
{
  if (m_current_state->m_tex_transform == matrix) return;
  m_current_state->m_tex_transform = matrix;
  GLint matrix_mode;
  glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf((GLfloat*) &matrix);
  glMatrixMode(matrix_mode);  // restore matrix mode so modelview
  // operations can continue as before
}

/*! \brief */
void Context::set_back_material(Material* material)
{
  m_default_state->m_back_material = material;
  m_default_state->m_pending.on_bit(Gfx::BACK_MATERIAL);
  // DrawBackMaterial(material);
}

/*! \brief if the material and the back material are the same, the back
 * material has already been applied through the draw_material()
 * function.
 */
void Context::draw_back_material(Material* material, Material* back_material)
{
  if (m_current_state->m_back_material == back_material &&
      back_material && !back_material->is_changed())
    return;

  m_current_state->m_back_material = back_material;
  if (back_material && back_material != material) {
    back_material->draw(Material::BACK, this);
  }
}

/*! \brief */
void Context::clear(Uint which)
{
  unsigned int glclear = 0;

  if (which & Gfx::COLOR_CLEAR) {
    glclear |= GL_COLOR_BUFFER_BIT;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
  }
  if (which & Gfx::DEPTH_CLEAR) {
    glclear |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(1.0);
  }
  if (which & Gfx::STENCIL_CLEAR) {
    glclear |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(0);
  }
  glClear(glclear);
}

/*! \brief */
void Context::clear(Uint which, Float r, Float g, Float b, Float a,
                    Int stencil)
{
  unsigned int glclear = 0;

  if (which & Gfx::COLOR_CLEAR) {
    glclear |= GL_COLOR_BUFFER_BIT;
    glClearColor(r, g, b, a);
  }
  if (which & Gfx::DEPTH_CLEAR) {
    glclear |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(1.0);
  }
  if (which & Gfx::STENCIL_CLEAR) {
    glclear |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(stencil);
  }
  glClear(glclear);
}

/*! \brief */
void Context::clear(Uint which, Float r, Float g, Float b, Float a)
{
  unsigned int glclear = 0;

  if (which & Gfx::COLOR_CLEAR) {
    glclear |= GL_COLOR_BUFFER_BIT;
    glClearColor(r, g, b, a);
  }
  if (which & Gfx::DEPTH_CLEAR) {
    glclear |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(1.0);
  }
  if (which & Gfx::STENCIL_CLEAR) {
    glclear |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(0);
  }
  glClear(glclear);
}

/*! \brief */
void Context::clear_color_buffer()
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  clear(which);
}

/*! \brief */
void Context::clear_depth_buffer()
{
  Uint which = 0;
  which |= Gfx::DEPTH_CLEAR;
  clear(which);
}

/*! \brief */
void Context::clear_stencil_buffer()
{
  Uint which = 0;
  which |= Gfx::STENCIL_CLEAR;
  clear(which);
}

/*! \brief */
void Context::clear_color_depth_buffer()
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  which |= Gfx::DEPTH_CLEAR;
  clear(which);
}

/*! \brief */
void Context::clear_color_depth_buffer(const Vector4f& color)
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  which |= Gfx::DEPTH_CLEAR;
  clear(which, color);
}

/*! \brief */
void Context::clear_color_depth_stencil_buffer()
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  which |= Gfx::DEPTH_CLEAR;
  which |= Gfx::STENCIL_CLEAR;
  clear(which);
}

/*! \brief */
void Context::clear_color_depth_stencil_buffer(const Vector4f& color)
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  which |= Gfx::DEPTH_CLEAR;
  which |= Gfx::STENCIL_CLEAR;
  clear(which, color);
}

/*! \brief */
void Context::clear_color_depth_stencil_buffer(const Vector4f& color,
                                               Int stencil)
{
  Uint which = 0;
  which |= Gfx::COLOR_CLEAR;
  which |= Gfx::DEPTH_CLEAR;
  which |= Gfx::STENCIL_CLEAR;
  clear(which, color, stencil);
}

/*! \brief */
void Context::push_lights()
{
  Int s = m_light_stack_depth + 1;

  // Push light stack
  // !!! Should lazily push lights since this isn't cheap !!!
  ::memcpy(m_light_stack[s], m_light_stack[s - 1],
           sizeof(Light_target) * SGAL_MAX_LIGHTS);

  // We've taken a reference to these lights so ref'm
  Light_target* current_lights = m_light_stack[m_light_stack_depth];

  for (int i = 0; i < SGAL_MAX_LIGHTS; i++)
    if (current_lights[i].m_defined != 0) {
      current_lights[i].m_defined->ref();
    }
  
  // Push the stack pointer
  m_light_stack_depth = s;
}

/*! \brief */
void Context::push_state()
{
  Int s = m_stack_depth + 1;

  push_lights();
  // PushFog(0);

  // Push state stack
  m_current_state_stack[s].copy(m_current_state_stack[s-1]);
  m_stack_depth = s;
  m_current_state = &m_current_state_stack[s];
}

/*! \brief draws the new state is performed in 2 passes. The boolean 
 * (enable/disable)elements are handled first, then the rest of the elements. 
 * This is done in order to maintain proper state Darw to enable flags and 
 * their corresponding data.
 * Each pass is performed in 2 steps. During the 1st step the default state
 * is used to Draw the new state for all state elements that their notInherit
 * bit transition from TRUE to FALSE. In other words, those elements that were
 * set according to the previous state, but are not be set (are inherited)
 * according to the new state.
 * Finally the default mask and the current mask are updated.
 */
void Context::pop_state()
{
  Int s = m_stack_depth - 1;

  Gfx* new_gfx = &m_current_state_stack[s];
  
  // Compute the bit mask that represent the TRUE to FALSE transition:
  Bit_mask def_draw(m_default_state->m_pending);
  def_draw.or_equal(m_current_state->m_override);
  def_draw.and_not_equal(new_gfx->m_override);

  // Draw using Default state:
  draw_state_elements(def_draw, m_default_state);
  m_default_state->m_pending.and_not_equal(def_draw);

  // Draw using new state:
  draw_state_elements(new_gfx->m_override, new_gfx);
  new_gfx->m_pending.and_not_equal(new_gfx->m_override);

  // Update:
  m_current_state->m_override.set(new_gfx->m_override);

  m_stack_depth = s;
  m_current_state = &m_current_state_stack[s];

  // Restore light & fog state
  // PopFog();
  pop_lights();
}

/*! \brief */
void Context::pop_lights()
{
  // Pop lights - lights are tricky because they can "float" amongst GL light
  // targets. Also, lights need to be restored with the matrix they were last
  // bound with. 

  int i, j;
  Light_target* prev_lights = m_light_stack[m_light_stack_depth];
  Light_target* current_lights = m_light_stack[m_light_stack_depth-1];

  for (i = 0; i < SGAL_MAX_LIGHTS; i++) {
    if (prev_lights[i].m_defined != 0) {
      if (prev_lights[i].m_enabled) {
        int found = -1;

        for (j = 0; j < SGAL_MAX_LIGHTS && found == -1; j++) {
          if (prev_lights[i].m_defined == current_lights[j].m_defined)
            found = j;
        }

        if (found == -1) {
          // We have not found the light in the current light list
          // we should turn it off.
          glDisable(GL_LIGHT0 + i);
        }
        else if (!current_lights[found].m_enabled) {
          // If we found it but it's not enabled: turn it off
          glDisable(GL_LIGHT0 + found);
          // Also restore this light with the old matrix just
          // in case it get enabled later.
          // current_lights[found].m_defined->Pop(current_lights[found].m_mat, this); // TBD
        }
        else {
          // Restore this light with the old matrix
          //current_lights[found].m_defined->Pop(current_lights[found].m_mat, this); // TBD
        }
      }
      
      prev_lights[i].m_defined->unref();
    }
  }

  m_light_stack_depth--;
}

/*! \brief */
void Context::make_current()
{
  //! \todo if (!m_win_handle) return;
  // Ignore same context.
  if (s_current_context == this) return;
  s_current_context = this;

  m_current_state = &m_current_state_stack[m_stack_depth];
  
  //! \todo m_gfx_handle->make_current();
  //! \todo m_win_handle = win_handle;
    
  if (!m_made_current) {
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    if (m_viewport[2] != 0 && m_viewport[3] != 0)
      m_made_current = true;  
  }
  m_is_current = true;

  Vector4f amb(0.0f, 0.0f, 0.0f, 1.0f);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float*) &amb);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, m_local_viewer);
}

/*! \brief */
/*! \todo Window_handle 
void Context::make_current(Window_handle* win_handle)
{
  // Ignore same context.
  if (s_current_context == this) return;
  s_current_context = this;

  m_current_state = &m_current_state_stack[m_stack_depth];
  
  //! \todo m_gfx_handle->make_current();
  //! \todo m_win_handle = win_handle;
    
  if (!m_made_current) {
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    if (m_viewport[2] != 0 && m_viewport[3] != 0)
      m_made_current = true;  
  }
  m_is_current = true;

  Vector4f amb(0.0f, 0.0f, 0.0f, 1.0f);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float*) &amb);
}
*/

/*! \brief */
void Context::release_current() { s_current_context = 0; }

/*! \brief */
void Context::delete_context()
{
  if (s_current_context == this) {
    s_current_context = 0;
  }

  /*! \todo if (m_gfx_handle) {
    m_gfx_handle->destroy_current();
  }
  */
}

/*! \brief */
void Context::swap_buffers()
{
  //! \todo m_gfx_handle->swap_buffers();
  TRACE_MSG(Trace::GRAPHICS, "Context::swap_buffers()\n");
}

/*! \brief */
void Context::push_override_appearance(Appearance* app)
{
  if (m_override_app_stack_top == SGAL_MAX_STACK - 1) {
    fprintf(stderr, "overflow in Context override appearance stack");
    return;
  }
  m_override_app_stack[++m_override_app_stack_top] = app;
}

/*! \brief */
Appearance* Context::pop_override_appearance()
{
  if (m_override_app_stack_top == -1) {
    fprintf(stderr, "underflow in Context override appearance stack");  
    return (0);
  }

  Appearance* app = m_override_app_stack[m_override_app_stack_top];
  m_override_app_stack_top--;
  return(app);
}

/*! \brief */
void Context::push_override_geo_prop(Override_geo_prop* gprop)
{
  if (m_override_geo_prop_stack_top == SGAL_MAX_STACK - 1) {
    fprintf(stderr, "overflow in Context override geometry property stack");  
    return;
  }
  m_override_geo_prop_stack[++m_override_geo_prop_stack_top] = gprop;
}

/* \brief */
Override_geo_prop* Context::pop_override_geo_prop()
{
  if (m_override_geo_prop_stack_top == -1) {
    fprintf(stderr, "underflow in Context override geometry property stack");  
    return (0);
  }

  Override_geo_prop* gp =
    m_override_geo_prop_stack[m_override_geo_prop_stack_top];
  m_override_geo_prop_stack_top--;

  // make sure these are evaluated in next call to set:
  m_current_state->m_cull_face = (Gfx::Cull_face) -1;
  m_current_state->m_light_model_sides = (Gfx::Light_model_sides) -1 ;
  m_current_state->m_line_width = -1.0f;
  m_current_state->m_point_size = -1.0f;

  // Return the popped properties
  return(gp);
}

/*! \brief */
void Context::set_blend_funcs(Gfx::Src_blend_func src_blend_func,
                              Gfx::Dst_blend_func dst_blend_func)
{
  m_default_state->m_src_blend_func = src_blend_func;
  m_default_state->m_dst_blend_func = dst_blend_func;
}

/*! \brief */
void Context::draw_blend_funcs(Gfx::Src_blend_func src_blend_func,
                               Gfx::Dst_blend_func dst_blend_func)
{
  if (m_current_state->m_src_blend_func != src_blend_func) {
    if (m_current_state->m_dst_blend_func == Gfx::ZERO_DBLEND) {
      if (src_blend_func == Gfx::ONE_SBLEND) glDisable(GL_BLEND);
      else glEnable(GL_BLEND);
    }
    glBlendFunc(s_src_blend_func_tokens[src_blend_func], 
                s_dst_blend_func_tokens[m_current_state->m_dst_blend_func]);
    m_current_state->m_src_blend_func = src_blend_func;
  }

  if (m_current_state->m_dst_blend_func != dst_blend_func) {
    if (m_current_state->m_src_blend_func == Gfx::ONE_SBLEND) {
      if (dst_blend_func == Gfx::ZERO_DBLEND) glDisable(GL_BLEND);
      else glEnable(GL_BLEND);
    }

    glBlendFunc(s_src_blend_func_tokens[m_current_state->m_src_blend_func], 
                s_dst_blend_func_tokens[dst_blend_func]);
    m_current_state->m_dst_blend_func = dst_blend_func;
  }
}

/*! \brief */
void Context::draw_state_elements(const Bit_mask& set_mask_ptr,
                                  const Gfx* gfx) 
{
  Bit_mask tmp_mask(set_mask_ptr);

  // Loop through all the state element bits to determine which ones we need
  // to actually draw. Stop when all the bits have been turned off.
  for (int i = 0; tmp_mask.get_mask(); i++) {
    if (!tmp_mask.get_bit(i)) continue;

    tmp_mask.off_bit(i);

    switch (i) {
     case Gfx::TEX_ENABLE: draw_tex_enable(gfx->m_tex_enable); break;
     case Gfx::TEX_GEN_ENABLE:
      draw_tex_gen_enable(gfx->m_tex_gen_enable); break;

     case Gfx::LIGHT_ENABLE: draw_light_enable(gfx->m_light_enable); break;
     case Gfx::TRANSP_ENABLE: draw_transp_enable(gfx->m_transp_enable); break;
     case Gfx::DEPTH_ENABLE: draw_depth_enable(gfx->m_depth_enable); break;
     case Gfx::FOG_ENABLE: draw_fog_enable(gfx->m_fog_enable); break;
     case Gfx::MATERIAL_MODE_ENABLE:
      draw_material_mode_enable(gfx->m_material_mode_enable); break;

     case Gfx::POLYGON_STIPPLE_ENABLE:
      draw_polygon_stipple_enable(gfx->m_polygon_stipple_enable); break;

     case Gfx::TEXTURE: draw_texture(gfx->m_texture); break;
     case Gfx::HALFTONE_PATTERN: draw_halftone(gfx->m_halftone); break;
     case Gfx::TEX_MODE: draw_tex_mode(gfx->m_tex_mode); break;
     case Gfx::TEX_BLEND_COLOR:
      draw_tex_blend_color(gfx->m_tex_blend_color); break;

     case Gfx::TEX_ENV: draw_tex_env(gfx->m_tex_env); break;
     case Gfx::TEX_GEN: draw_tex_gen(gfx->m_tex_gen); break;
     case Gfx::MATERIAL:
      draw_material(gfx->m_material, gfx->m_back_material); break;

     case Gfx::SHADE_MODEL: draw_shade_model(gfx->m_shade_model); break;
     case Gfx::TRANSP_MODE: draw_transp_mode(gfx->m_transp_mode); break;
     case Gfx::ALPHA_FUNC: draw_alpha_func(gfx->m_alpha_func); break;
     case Gfx::ALPHA_REF: draw_alpha_ref(gfx->m_alpha_ref); break;
     case Gfx::BLEND_COLOR: draw_blend_color(gfx->m_blend_color); break;
     case Gfx::SRC_BLEND_FUNC:
      draw_src_blend_func(gfx->m_src_blend_func); break;
      
     case Gfx::DST_BLEND_FUNC:
      draw_dst_blend_func(gfx->m_dst_blend_func); break;
      
     case Gfx::COLOR_MASK: draw_color_mask(gfx->m_color_mask); break;
     case Gfx::DEPTH_FUNC: draw_depth_func(gfx->m_depth_func); break;
     case Gfx::DEPTH_MASK: draw_depth_mask(gfx->m_depth_mask); break;
     case Gfx::POLY_MODE: draw_poly_mode(gfx->m_poly_mode); break;
     case Gfx::LINE_STIPPLE_PATTERN:
      draw_line_stipple_pattern(gfx->m_line_stipple_pattern); break;

     case Gfx::LINE_STIPPLE_FACTOR:
      draw_line_stipple_factor(gfx->m_line_stipple_factor); break;

     case Gfx::TEX_TRANSFORM: draw_tex_transform(gfx->m_tex_transform); break;
     case Gfx::BACK_MATERIAL:
      draw_back_material(gfx->m_material, gfx->m_back_material); break;

      // These are geoset-specific, not in appearance but in context
     case Gfx::CULL_FACE: draw_cull_face(gfx->m_cull_face); break;
     case Gfx::LIGHT_MODEL_SIDES:
      draw_light_model_sides(gfx->m_light_model_sides); break;
     case Gfx::LINE_WIDTH: draw_line_width(gfx->m_line_width); break;
     case Gfx::POINT_SIZE: draw_point_size(gfx->m_point_size); break;

      // special one:
     case Gfx::INHERIT: break;
    }
    // {
    //     char msg[128];
    //     sprintf(msg, "Context::draw_state_elements1() %d", i);
    //     DBG(EGLError::get_instance()->report_error(String(msg)));
    // }
  }
}

/*! \breif */
void Context::draw_state_elements(const Bit_mask& set_mask_ptr,
                                  const Appearance* app) 
{
  Bit_mask tmp_mask(set_mask_ptr);

  // Loop through all the state element bits to determine which ones we need
  // to actually draw. Stop when all the bits have been turned off.
  for (int i = 0; tmp_mask.get_mask(); i++) {
    if (!tmp_mask.get_bit(i)) {
      continue;
    }
    tmp_mask.off_bit(i);

    switch (i) {
     case Gfx::TEX_ENABLE: draw_tex_enable(app->m_tex_enable); break;
     case Gfx::TEX_GEN_ENABLE:
      draw_tex_gen_enable(app->m_tex_gen_enable); break;

     case Gfx::LIGHT_ENABLE: draw_light_enable(app->m_light_enable); break;
     case Gfx::TRANSP_ENABLE: draw_transp_enable(app->m_transp_enable); break;
     case Gfx::DEPTH_ENABLE: draw_depth_enable(app->m_depth_enable); break;
     case Gfx::FOG_ENABLE: draw_fog_enable(app->m_fog_enable); break;
     case Gfx::MATERIAL_MODE_ENABLE:
      draw_material_mode_enable(app->m_material_mode_enable); break;

     case Gfx::POLYGON_STIPPLE_ENABLE:
      draw_polygon_stipple_enable(app->m_polygon_stipple_enable); break;
      
     case Gfx::TEXTURE: draw_texture(app->m_texture); break;
     case Gfx::HALFTONE_PATTERN: draw_halftone(app->m_halftone); break;
     case Gfx::TEX_MODE: draw_tex_mode(app->m_tex_mode); break;
     case Gfx::TEX_BLEND_COLOR:
      draw_tex_blend_color(app->m_tex_blend_color); break;

     case Gfx::TEX_ENV: draw_tex_env(app->m_tex_env); break;
     case Gfx::TEX_GEN: draw_tex_gen(app->m_tex_gen); break;
     case Gfx::MATERIAL:
      draw_material(app->m_material, app->m_back_material); break;

     case Gfx::SHADE_MODEL: draw_shade_model(app->m_shade_model); break;
     case Gfx::TRANSP_MODE: draw_transp_mode(app->m_transp_mode); break;
     case Gfx::ALPHA_FUNC: draw_alpha_func(app->m_alpha_func); break;
     case Gfx::ALPHA_REF: draw_alpha_ref(app->m_alpha_ref); break;
     case Gfx::BLEND_COLOR: draw_blend_color(app->m_blend_color); break;
     case Gfx::SRC_BLEND_FUNC:
      draw_src_blend_func(app->m_src_blend_func); break;

     case Gfx::DST_BLEND_FUNC:
      draw_dst_blend_func(app->m_dst_blend_func); break;
      
     case Gfx::COLOR_MASK: draw_color_mask(app->m_color_mask); break;
     case Gfx::DEPTH_FUNC: draw_depth_func(app->m_depth_func); break;
     case Gfx::DEPTH_MASK: draw_depth_mask(app->m_depth_mask); break;
     case Gfx::POLY_MODE: draw_poly_mode(app->m_poly_mode); break;
     case Gfx::LINE_STIPPLE_PATTERN:
      draw_line_stipple_pattern(app->m_line_stipple_pattern); break;

     case Gfx::LINE_STIPPLE_FACTOR:
      draw_line_stipple_factor(app->m_line_stipple_factor); break;

     case Gfx::TEX_TRANSFORM: draw_tex_transform(app->m_tex_transform); break;
     case Gfx::BACK_MATERIAL:
      draw_back_material(app->m_material, app->m_back_material); break;
            
      // These are geoset-specific, not in appearance but in context
     case Gfx::CULL_FACE: break;
     case Gfx::LIGHT_MODEL_SIDES: break;
     case Gfx::LINE_WIDTH: break;
     case Gfx::POINT_SIZE: break;

      // special one:
     case Gfx::INHERIT: break;
    }
    // {
    //     char msg[128];
    //     sprintf(msg, "Context::draw_state_elements2() %d", i);
    //     DBG(EGLError::get_instance()->report_error(String(msg)));
    // }
  }
}

// inline void* operator new(size_t, void* t) { return t; }

/*! \brief */
void Context::draw_app(Appearance* app)
{
  Appearance* new_app;
  Appearance* local_app;
  Ubyte space_for_local_app[sizeof(Appearance)];

  // If we have an over ride appearance and there's an appearance to override
  // then build a new local appearance which inherits from the incoming
  // appearance everything not set by the override appearance.
  if (m_override_app_stack_top != -1) {
    // The appearance comes from the stack
    Appearance* override_app = m_override_app_stack[m_override_app_stack_top];

    if (app != 0) {
      // Call the constructor for the temporary new local appearance
      // Note: the space is already allocated on the stack
      // Save the reference to the newly constructed appearance, 
      local_app = ::new(space_for_local_app) Appearance;
  
      // mark the appearance to prevent it from altering the scene
      // graph by calling refer() when we setTexture, etc.
      local_app->m_skip_refer = true;

      // Get the override's inherit mask to figure out
      // which fields to override
      Bit_mask override_inherit_mask;
      override_app->get_inherit(override_inherit_mask);

      // Get the incoming inherit mask to figure out
      // which fields to set in the local app that are not overriden
      // but set in the incoming appearance
      Bit_mask app_inherit_mask;
      app->get_inherit(app_inherit_mask);

      // Local vars for initializing various "sets"
      float r, g, b, t;
      unsigned char v0, v1, v2, v3;

      // Loop through all the state element bits copying the appropriate
      // inherited appearance:
      // TEXTURE:
      if (override_inherit_mask.get_bit(Gfx::TEXTURE) == 0) {
        local_app->set_texture(override_app->get_texture());
      } else if (app_inherit_mask.get_bit(Gfx::TEXTURE) == 0) {
        local_app->set_texture(app->get_texture());
      }

      // HALFTONE:
      if (override_inherit_mask.get_bit(Gfx::HALFTONE_PATTERN) == 0) {
        local_app->set_halftone(override_app->get_halftone());
      } else if (app_inherit_mask.get_bit(Gfx::HALFTONE_PATTERN) == 0) {
        local_app->set_halftone(app->get_halftone());
      }
      
      // TEX_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::TEX_ENABLE) == 0) {
        local_app->set_tex_enable(override_app->get_tex_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::TEX_ENABLE) == 0) {
        local_app->set_tex_enable(app->get_tex_enable());
      }

      // TEX_MODE:
      if (override_inherit_mask.get_bit(Gfx::TEX_MODE) == 0) {
        local_app->set_tex_mode(override_app->get_tex_mode());
      }
      else if (app_inherit_mask.get_bit(Gfx::TEX_MODE) == 0) {
        local_app->set_tex_mode(app->get_tex_mode());
      }

      // TEX_BLEND_COLOR:
      if (override_inherit_mask.get_bit(Gfx::TEX_BLEND_COLOR) == 0) {
        override_app->get_tex_blend_color(&r, &g, &b, &t);
        local_app->set_tex_blend_color(r,  g,  b,  t);
      } else {
        if (app_inherit_mask.get_bit(Gfx::TEX_BLEND_COLOR) == 0) {
          app->get_tex_blend_color(&r, &g, &b, &t);
          local_app->set_tex_blend_color(r, g, b, t);
        }
      }

      // TEX_ENV:
      if (override_inherit_mask.get_bit(Gfx::TEX_ENV) == 0) {
        local_app->set_tex_env(override_app->get_tex_env());
      }
      else if (app_inherit_mask.get_bit(Gfx::TEX_ENV) == 0) {
        local_app->set_tex_env(app->get_tex_env());
      }

      // TEX_GEN:
      if (override_inherit_mask.get_bit(Gfx::TEX_GEN) == 0) {
        local_app->set_tex_gen(override_app->get_tex_gen());
      }
      else if (app_inherit_mask.get_bit(Gfx::TEX_GEN) == 0) {
        local_app->set_tex_gen(app->get_tex_gen());
      }

      // TEX_GEN_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::TEX_GEN_ENABLE) == 0) {
        local_app->set_tex_gen_enable(override_app->get_tex_gen_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::TEX_GEN_ENABLE) == 0) {
        local_app->set_tex_gen_enable(app->get_tex_gen_enable());
      }

      // LIGHT_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::LIGHT_ENABLE) == 0) {
        local_app->set_light_enable(override_app->get_light_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::LIGHT_ENABLE) == 0) {
        local_app->set_light_enable(app->get_light_enable());
      }

      // SHADE_MODEL:
      if (override_inherit_mask.get_bit(Gfx::SHADE_MODEL) == 0) {
        local_app->set_shade_model(override_app->get_shade_model());
      }
      else if (app_inherit_mask.get_bit(Gfx::SHADE_MODEL) == 0) {
        local_app->set_shade_model(app->get_shade_model());
      }
            
      // TRANSP_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::TRANSP_ENABLE) == 0) {
        local_app->set_transp_enable(override_app->get_transp_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::TRANSP_ENABLE) == 0) {
        local_app->set_transp_enable(app->get_transp_enable());
      }

      // TRANSP_MODE:
      if (override_inherit_mask.get_bit(Gfx::TRANSP_MODE) == 0) {
        local_app->set_transp_mode(override_app->get_transp_mode());
      }
      else if (app_inherit_mask.get_bit(Gfx::TRANSP_MODE) == 0) {
        local_app->set_transp_mode(app->get_transp_mode());
      }

      // ALPHA_FUNC:
      if (override_inherit_mask.get_bit(Gfx::ALPHA_FUNC) == 0) {
        local_app->set_alpha_func(override_app->get_alpha_func());
      }
      else if (app_inherit_mask.get_bit(Gfx::ALPHA_FUNC) == 0) {
        local_app->set_alpha_func(app->get_alpha_func());
      }
            
      // ALPHA_REF:
      if (override_inherit_mask.get_bit(Gfx::ALPHA_REF) == 0) {
        local_app->set_alpha_ref(override_app->get_alpha_ref());
      }
      else if (app_inherit_mask.get_bit(Gfx::ALPHA_REF) == 0) {
        local_app->set_alpha_ref(app->get_alpha_ref());
      }

      // BLEND_COLOR:
      if (override_inherit_mask.get_bit(Gfx::BLEND_COLOR) == 0) {
        override_app->get_blend_color(&r, &g, &b, &t);
        local_app->set_blend_color(r, g, b, t);
      } else {
        if (app_inherit_mask.get_bit(Gfx::BLEND_COLOR) == 0) {
          app->get_blend_color(&r, &g, &b, &t);
          local_app->set_blend_color(r, g, b, t);
        }
      }

      // SRC_BLEND_FUNC:
      if (override_inherit_mask.get_bit(Gfx::SRC_BLEND_FUNC) == 0) {
        local_app->set_src_blend_func(override_app->get_src_blend_func());
      }
      else if (app_inherit_mask.get_bit(Gfx::SRC_BLEND_FUNC) == 0) {
        local_app->set_src_blend_func(app->get_src_blend_func());
      }

      // DST_BLEND_FUNC:
      if (override_inherit_mask.get_bit(Gfx::DST_BLEND_FUNC) == 0) {
        local_app->set_dst_blend_func(override_app->get_dst_blend_func());
      }
      else if (app_inherit_mask.get_bit(Gfx::DST_BLEND_FUNC) == 0) {
        local_app->set_dst_blend_func(app->get_dst_blend_func());
      }

      // COLOR_MASK:
      if (override_inherit_mask.get_bit(Gfx::COLOR_MASK) == 0) {
        override_app->get_color_mask(&v0, &v1, &v2, &v3);
        local_app->set_color_mask(v0, v1, v2, v3); 
      } else {
        if (app_inherit_mask.get_bit(Gfx::COLOR_MASK) == 0) {
          app->get_color_mask(&v0,  &v1,  &v2,  &v3);
          local_app->set_color_mask(v0, v1, v2, v3); 
        }
      }

      // DEPTH_FUNC:
      if (override_inherit_mask.get_bit(Gfx::DEPTH_FUNC) == 0) {
        local_app->set_depth_func(override_app->get_depth_func());
      }
      else if (app_inherit_mask.get_bit(Gfx::DEPTH_FUNC) == 0) {
        local_app->set_depth_func(app->get_depth_func());
      }

      // DEPTH_MASK:
      if (override_inherit_mask.get_bit(Gfx::DEPTH_MASK) == 0) {
        local_app->set_depth_mask(override_app->get_depth_mask());
      }
      else if (app_inherit_mask.get_bit(Gfx::DEPTH_MASK) == 0) {
        local_app->set_depth_mask(app->get_depth_mask());
      }

      // FOG_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::FOG_ENABLE) == 0) {
        local_app->set_fog_enable(override_app->get_fog_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::FOG_ENABLE) == 0) {
        local_app->set_fog_enable(app->get_fog_enable());
      }

      // POLYGON_STIPPLE_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::POLYGON_STIPPLE_ENABLE) == 0) {
        local_app->
          set_polygon_stipple_enable(override_app->get_polygon_stipple_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::POLYGON_STIPPLE_ENABLE) == 0) {
        local_app->
          set_polygon_stipple_enable(app->get_polygon_stipple_enable());
      }
      
      // POLY_MODE:
      if (override_inherit_mask.get_bit(Gfx::POLY_MODE) == 0) {
        local_app->set_poly_mode(override_app->get_poly_mode());
      }
      else if (app_inherit_mask.get_bit(Gfx::POLY_MODE) == 0) {
        local_app->set_poly_mode(app->get_poly_mode());
      }

      // LINE_STIPPLE_PATTERN:
      if (override_inherit_mask.get_bit(Gfx::LINE_STIPPLE_PATTERN) == 0) {
        local_app->
          set_line_stipple_pattern(override_app->get_line_stipple_pattern());
      }
      else if (app_inherit_mask.get_bit(Gfx::LINE_STIPPLE_PATTERN) == 0) {
        local_app->set_line_stipple_pattern(app->get_line_stipple_pattern());
      }

      // LINE_STIPPLE_FACTOR:
      if (override_inherit_mask.get_bit(Gfx::LINE_STIPPLE_FACTOR) == 0) {
        local_app->
          set_line_stipple_factor(override_app->get_line_stipple_factor());
      }
      else if (app_inherit_mask.get_bit(Gfx::LINE_STIPPLE_FACTOR) == 0) {
        local_app->set_line_stipple_factor(app->get_line_stipple_factor());
      }

      // TEX_TRANSFORM:
      if (override_inherit_mask.get_bit(Gfx::TEX_TRANSFORM) == 0) {
        local_app->set_tex_transform(override_app->get_tex_transform());
      } else {
        if (app_inherit_mask.get_bit(Gfx::TEX_TRANSFORM) == 0) {
          local_app->set_tex_transform(app->get_tex_transform());
        }
      }

      // MATERIAL:
      if (override_inherit_mask.get_bit(Gfx::MATERIAL) == 0) {
        local_app->set_material(override_app->get_material());
      }
      else if (app_inherit_mask.get_bit(Gfx::MATERIAL) == 0) {
        local_app->set_material(app->get_material());
      }

      // BACK_MATERIAL:
      if (override_inherit_mask.get_bit(Gfx::BACK_MATERIAL) == 0) {
        local_app->set_back_material(override_app->get_back_material());
      }
      else if (app_inherit_mask.get_bit(Gfx::BACK_MATERIAL) == 0) {
        local_app->set_back_material(app->get_back_material());
      }

      // DEPTH_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::DEPTH_ENABLE) == 0) {
        local_app->set_depth_enable(override_app->get_depth_enable());
      } else if (app_inherit_mask.get_bit(Gfx::DEPTH_ENABLE) == 0) {
        local_app->set_depth_enable(app->get_depth_enable());
      }

      // MATERIAL_MODE_ENABLE:
      if (override_inherit_mask.get_bit(Gfx::MATERIAL_MODE_ENABLE) == 0) {
        local_app->set_material_mode_enable(override_app->get_material_mode_enable());
      }
      else if (app_inherit_mask.get_bit(Gfx::MATERIAL_MODE_ENABLE) == 0) {
        local_app->set_material_mode_enable(app->get_material_mode_enable());
      }

      new_app = local_app;
    } else {
      // app == 0
      local_app = 0;
      new_app  = override_app;
    }
  } else {
    // m_override_app_stack_top == -1
    local_app = 0;
    new_app = app;
  }

  // If the new appearance is 0, go back to the Default state:
  if (new_app == 0) {
    Bit_mask def_draw(m_default_state->m_pending);
    def_draw.or_equal(m_current_state->m_override);

    // If there exist pending default elements or overriden current elements:
    if (def_draw.get_mask()) {
      // Draw using Default state:
      draw_state_elements(def_draw, m_default_state);
      m_default_state->m_pending.and_not_equal(def_draw);

      // Turn off override, as the default state is the current state:
      m_current_state->m_override.off();
      return;
    }
    return;
  }

  // Otherwise use the new appearance:
  Bit_mask def_draw(m_default_state->m_pending);
  def_draw.or_equal(m_current_state->m_override);
  def_draw.and_not_equal(new_app->m_override);

  // If either:
  // 1. The given appearance is not the same as the last appearance, or
  // 2. At least one (relevant) new Appearnce field is pending, or
  // 3. At least one relevant default field is pending
  if (m_last_app != new_app ||
      m_last_app->m_override.get_mask() != new_app->m_override.get_mask() ||
      new_app->m_pending.get_mask() ||
      def_draw.get_mask())
  {
    // Draw using Default state:
    draw_state_elements(def_draw, m_default_state);
    m_default_state->m_pending.and_not_equal(def_draw);

    // Draw using new state:
    draw_state_elements(new_app->m_override, new_app);
    new_app->m_pending.and_not_equal(new_app->m_override);

    // Set the override of the current state accordingly:
    m_current_state->m_override.set(new_app->m_override);
    
    m_last_app = (new_app == local_app) ? 0 : new_app;
    return;
  }
}

/*!
 */
Light* Context::get_light(const Int i) const
{
  return (0 <= i && i < SGAL_MAX_LIGHTS) ?
    m_light_stack[m_light_stack_depth][i].m_defined : 0;
}

/*! Returns the index of the light associated with the given viewing matrix.
 */
Int Context::get_light_target(Light* light, const Matrix4f& mat, 
                              Int& already_defined)
{
  int i;
  Light_target* lights = m_light_stack[m_light_stack_depth];

  // Get the incoming light state
  Boolean on = light->get_on();

  // Assume that we have yet to define this light
  already_defined = -1;

  // Find the GL target which already has 'l' defined 
  for (i = 0; i < SGAL_MAX_LIGHTS; i++) {
    // If it's defined we want to return it
    if (lights[i].m_defined == light) {
      // If the light has not changed state and it's defined
      if (lights[i].m_enabled == on) {
        // If the matrix is the same return the light index
        // and the fact it's all set up.
        if (mat == lights[i].m_mat) {
          already_defined = i;
          goto l4;  // Just return i
        }
        else goto l3;  // Set the matrix and return
      }
      else goto l2;  // Just enable/disable the light, mat, and return
    }
  }

  // The light hasn't been defined yet. Find a free spot:
  for (i = 0 ; i < SGAL_MAX_LIGHTS ; i++) {
    if (lights[i].m_defined == 0) goto l1;
  }

  /* There are no empty spots left. Reuse a defined light that is currently
   * disabled:
   */
  for (i = SGAL_MAX_LIGHTS-1; i >= 0; i--) {
    if (!lights[i].m_enabled) goto l0;
  }

 l0:
  // If we're going to reuse a light-slot make sure we unref
  // the light since it was ref'd when it was put on the list
  if (lights[i].m_defined != 0) lights[i].m_defined->unref();

 l1:
  // Properly enable GL light target if different
  lights[i].m_defined = light;
  light->ref();
  lights[i].m_set = true;

 l2:
  // Set the light to the state it think it is in.
  if (on) glEnable(GL_LIGHT0 + i);
  else glDisable(GL_LIGHT0 + i);

  lights[i].m_enabled = on;

 l3:
  lights[i].m_mat = mat;

 l4:
  return i;
}

/*! \brief */
void Context::disable_light_targets()
{
  Light_target* lights;
  lights = m_light_stack[m_light_stack_depth];

  for (int i = 0; i < SGAL_MAX_LIGHTS; i++) {
    if (lights[i].m_enabled == true) {
      lights[i].m_enabled = false;
      glDisable(GL_LIGHT0 + i);
    }
  }
}

/*! \brief */
void Context::disable_light(Light* light)
{
#if defined(NO_PLUGIN)
  for (size_t i = 0; i < s_contexts->get_count(); i++) {
    Context* ctx = (Context*) s_contexts->get(i);
    if (ctx == 0) continue;
    Light_target* lights = ctx->m_light_stack[ctx->m_light_stack_depth];
#else
  {
    Light_target* lights =
      s_context->m_light_stack[s_context->m_light_stack_depth];
#endif
    for (int j = 0; j < SGAL_MAX_LIGHTS; j++) {
      if (lights[j].m_defined == light && lights[j].m_enabled == true) {
        lights[j].m_enabled = false;
        glDisable(GL_LIGHT0 + j);
      }
    }
  }
}

/*! \brief obtains the aspect ration based on the view port. */
float Context::get_aspect_ratio() const
{
  if (m_viewport[3] == 0) return 1;
  return ((float) m_viewport[2] / (float) m_viewport[3]);
}
 
/*! \brief */
Uint Context::get_red_bits() const { return m_red_bits; }
Uint Context::get_green_bits() const { return m_green_bits; }
Uint Context::get_blue_bits() const { return m_blue_bits; }
Uint Context::get_alpha_bits() const { return m_alpha_bits; }

Uint Context::get_accum_red_bits() const { return m_accum_red_bits; }
Uint Context::get_accum_green_bits() const { return m_accum_green_bits; }
Uint Context::get_accum_blue_bits() const { return m_accum_blue_bits; }
Uint Context::get_accum_alpha_bits() const { return m_accum_alpha_bits; }

Uint Context::get_depth_bits() const { return m_depth_bits; }
Uint Context::get_stencil_bits() const { return m_stencil_bits; }

///////////////////////  NOT USED  /////////////////////////////

#if defined(NO_PLUGIN) && !defined(NO_AUDIO)
void Context::init_class()
{
#if defined(NO_PLUGIN)
  s_context_list = new Ref_array(4);
#endif

#if !defined(NO_AUDIO)
  s_sounds_array = new Ref_array(16);
  s_sound_players_array = new Ref_array(8);
  s_sound_initialized = false;
#endif
}
#endif

#if !defined(NO_AUDIO)
/*! \brief InitSound */
void Context::init_sound()
{
  int i, n = Sound_player::get_num_available_sound_players();
  for (i = 0; i < n; i++) {
    Sound_player* sp = new SoundPlayer;
    if (sp->open_audio_port() == -1)
      warn("Failed to open AudioPort for Audio Resource\n");
    else
      s_sound_Players_Array->append(sp);
  }
  s_sound_initialized = true;
}
#endif

/*
Context* Context::get_current()
{
#if defined(NO_PLUGIN)
    return EPrivate::get_context();
#else
    return s_context;
#endif
}
*/

#if 0
Boolean Context::sync_fog_state(Fog* fog)
{
  if (m_default_state->m_fog_enable == false) {
    return false;
  }
  draw_fog_enable(fog->get_on());
  return (fog->geton());
}

/*! \brief */
void Context::push_fog(Fog* fog)
{
  // If there is no fog to push (i.e. in pushState), replicate top of stack
  if (fog == 0) {
    fog = m_fogStack[m_fog_stack_depth];
  }

  // Push fog stack
  m_fog_stack[++m_fog_stack_depth] = fog;
  fog->push();
  
  // We've taken a reference to this fog
  // fog->ref();
}

/*! \brief */
void Context::pop_fog()
{
  // Pop fog stack
  Fog* fog = m_fog_stack[m_fog_stack_depth--];

  // Restore previous fog state if necessary
  //m_fogStack[m_fogStackDepth]->Pop(fog, this);
  m_fog_stack[m_fog_stack_depth]->pop(fog);

  // Release reference to this fog
  // fog->unref();
}
#endif

#if 0
/*! \brief */
int Context::new_id(Int block)
{
  int id;

  if (block == 1 && s_free_id_array->get_count() > 0) {
    id = s_free_id_array->get(0);
    s_free_id_array->remove_index(0);
    return id;
  } else {
    int tmp = s_nextId;
    s_nextId += block;
    return tmp;
  }
}
#endif

#if 0
/*! \brief */
void Context::free_id(Int id, Int block)
{
  int j;
  // Make sure we release the resource associated with this id in all
  // contexts
#if defined(NO_PLUGIN)
  for (size_t i = 0; i < s_contexts->get_count(); i++) {
    Context* ctx = (Context*) s_contexts->get(i);
    if (ctx == 0) continue;
    Int_array& darray = ctx->m_dirty_array;
#else
    {
      Int_array& darray = s_context->m_dirty_array;
#endif
      for (j = 0; j < block; j++) {
        if (id+j >= (int) darray.get_size()) {
          int new_size = (int) darray.get_size() * 2;
          while (id+j >= new_size) new_size *= 2;
          darray.set_Size((size_t) new_size);
          darray.set((size_t) id + j, 0);
        } else darray.set((size_t) id + j, 0);
      }
    }
    for (j = 0; j < block; j++) s_free_id_array->append((size_t) id + j);
  }
#endif

#if 0
/*! \brief */
Boolean Context::is_dirty(Int id, Int count)
{
  Int_array& darray = m_dirty_array;

  if (id >= (int) darray.get_size()) {
    int new_size = (int) darray.get_size() * 2;
    while (id >= new_size) newSize *= 2;
    darray.set_size((size_t) newSize);
    darray.set((size_t) id, count);
    return true;
    }
  if (count <= darray.get((size_t) id)) {
    return false;
  } else {
    darray.set((size_t) id, count);
    return true;
  }
}

#endif

SGAL_END_NAMESPACE
