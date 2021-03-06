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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Texture_transform.hpp"
#include "SGAL/Halftone.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Utilities.hpp"
#include <SGAL/to_boolean.hpp>

SGAL_BEGIN_NAMESPACE

std::string Appearance::s_tag = "Appearance";
Container_proto* Appearance::s_prototype(nullptr);

// Default values:
const Gfx::Light_model_color_control
  Appearance::s_def_light_model_color_control(Gfx::SINGLE_COLOR);
const Gfx::Poly_mode Appearance::s_def_poly_mode(Gfx::FILL_PMODE);
const Gfx::Shade_model Appearance::s_def_shade_model(Gfx::SMOOTH_SHADE);
const Gfx::Tex_env Appearance::s_def_tex_env(Gfx::MODULATE_TENV);
const Gfx::Depth_func Appearance::s_def_depth_function(Gfx::LESS_DFUNC);

const Char* Appearance::s_depth_func_names[] = {
  "never", "less", "equal", "lequal", "greater", "notequal", "gequal", "always"
};

REGISTER_TO_FACTORY(Appearance, "Appearance");

//! \brief constructor.
Appearance::Appearance(Boolean proto) : Container(proto) { init(); }

//! \brief destructor.
Appearance::~Appearance() {}

/*! \brief assigns the appearance with the content of another appearance;
 * performs a shallow copy.
 */
void Appearance::set(Appearance* app)
{
  m_texture                     = app->m_texture;
  m_tex_enable                  = app->m_tex_enable;
  m_tex_mode                    = app->m_tex_mode;
  m_tex_blend_color             = app->m_tex_blend_color;
  m_tex_env                     = app->m_tex_env;
  m_tex_gen                     = app->m_tex_gen;
  m_tex_gen_enable              = app->m_tex_gen_enable;
  m_material                    = app->m_material;
  m_material_mode_enable        = app->m_material_mode_enable;
  m_light_enable                = app->m_light_enable;
  m_shade_model                 = app->m_shade_model;
  m_transp_enable               = app->m_transp_enable;
  m_transp_mode                 = app->m_transp_mode;
  m_alpha_func                  = app->m_alpha_func;
  m_alpha_ref                   = app->m_alpha_ref;
  m_blend_color                 = app->m_blend_color;
  m_src_blend_func              = app->m_src_blend_func;
  m_dst_blend_func              = app->m_dst_blend_func;
  m_color_mask                  = app->m_color_mask;
  m_depth_enable                = app->m_depth_enable;
  m_depth_func                  = app->m_depth_func;
  m_depth_mask                  = app->m_depth_mask;
  m_fog_enable                  = app->m_fog_enable;
  m_poly_mode                   = app->m_poly_mode;
  m_line_stipple_factor         = app->m_line_stipple_factor;
  m_line_stipple_pattern        = app->m_line_stipple_pattern;
  m_tex_transform               = app->m_tex_transform;
  m_back_material               = app->m_back_material;
  m_polygon_stipple_enable      = app->m_polygon_stipple_enable;
  m_halftone                    = app->m_halftone;
  m_light_model_color_control   = app->m_light_model_color_control;

  m_pending = app->m_pending;
  m_override = app->m_override;
}

//! \brief
void Appearance::init()
{
  m_texture.reset();            // shared pointer
  m_tex_transform.reset();      // shared pointer
  m_tex_gen.reset();            // shared pointer
  m_material.reset();           // shared pointer
  m_back_material.reset();      // shared pointer
  m_halftone.reset();           // shared pointer
  m_tex_enable                = false;
  m_tex_mode                  = Gfx::FAST_TEX;
  m_tex_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_tex_env                   = s_def_tex_env;
  m_tex_gen_enable            = false;
  m_material_mode_enable      = Gfx::NO_COLOR_MATERIAL;
  m_light_enable              = false;
  m_shade_model               = Gfx::SMOOTH_SHADE;
  m_transp_enable             = false;
  m_transp_mode               = Gfx::BLEND_TRANSP;
  m_alpha_func                = Gfx::ALWAYS_AFUNC;
  m_alpha_ref                 = 0.0f;
  m_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_src_blend_func            = Gfx::ONE_SBLEND;
  m_dst_blend_func            = Gfx::ZERO_DBLEND;
  m_color_mask.set(0xff, 0xff, 0xff, 0xff);
  m_depth_enable              = true;
  m_depth_func                = s_def_depth_function;
  m_depth_mask                = true;
  m_fog_enable                = false;
  m_poly_mode                 = s_def_poly_mode;
  m_line_stipple_factor       = 1;
  m_line_stipple_pattern      = 0xffff;
  m_polygon_stipple_enable    = false;
  m_light_model_color_control = s_def_light_model_color_control;

  m_pending.off();
  m_override.off();
}

//! \brief sets the texture.
void Appearance::set_texture(Shared_texture texture)
{
  Observer observer(this, get_field_info(TEXTURE));
  if (m_texture) m_texture->unregister_observer(observer);
  m_texture = texture;
  if (m_texture) m_texture->register_observer(observer);
  m_pending.on_bit(Gfx::TEXTURE);
  m_override.on_bit(Gfx::TEXTURE);

  // this is to indicate that the texture has changed and
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
}

//! \brief sets the texture transformation.
void Appearance::set_tex_transform(Shared_texture_transform tex_transform)
{
  Observer observer(this, get_field_info(TEX_TRANSFORM));
  if (m_tex_transform) m_tex_transform->unregister_observer(observer);
  m_tex_transform = tex_transform;
  if (m_tex_transform) m_tex_transform->register_observer(observer);
  m_pending.on_bit(Gfx::TEX_TRANSFORM);
  m_override.on_bit(Gfx::TEX_TRANSFORM);
  m_tex_transform = tex_transform;
}

//! \brief
void Appearance::set_halftone(Shared_halftone halftone)
{
  Observer observer(this, get_field_info(HALFTONE_PATTERN));
  if (m_halftone) m_halftone->unregister_observer(observer);
  m_halftone = halftone;
  if (m_halftone) m_halftone->register_observer(observer);
  m_pending.on_bit(Gfx::HALFTONE_PATTERN);
  m_override.on_bit(Gfx::HALFTONE_PATTERN);

  m_dirty_flags.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
}

//! \brief
void Appearance::set_tex_enable(Boolean tex_enable)
{
  m_pending.on_bit(Gfx::TEX_ENABLE);
  m_override.on_bit(Gfx::TEX_ENABLE);
  m_tex_enable = tex_enable;
}

//! \brief
void Appearance::set_tex_mode(Gfx::Tex_mode tex_mode)
{
  m_pending.on_bit(Gfx::TEX_MODE);
  m_override.on_bit(Gfx::TEX_MODE);
  m_tex_mode = tex_mode;
}

//! \brief
void Appearance::set_tex_blend_color(const Vector4f& tex_blend_color)
{
  set_tex_blend_color(tex_blend_color[0], tex_blend_color[1],
                      tex_blend_color[2], tex_blend_color[3]);
}

//! \brief
void Appearance::set_tex_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  m_pending.on_bit(Gfx::TEX_BLEND_COLOR);
  m_override.on_bit(Gfx::TEX_BLEND_COLOR);
  m_tex_blend_color.set(v0, v1, v2, v3);
}

//! \brief sets the texture environment attribute.
void Appearance::set_tex_env(Gfx::Tex_env tex_env)
{
  m_pending.on_bit(Gfx::TEX_ENV);
  m_override.on_bit(Gfx::TEX_ENV);
  m_tex_env = tex_env;
}

//! \brief sets the texture-generation attribute.
void Appearance::set_tex_gen(Shared_tex_gen tex_gen)
{
  Observer observer(this, get_field_info(TEX_GEN));
  if (m_tex_gen) m_tex_gen->unregister_observer(observer);
  m_tex_gen = tex_gen;
  if (m_tex_gen) m_tex_gen->register_observer(observer);
  m_pending.on_bit(Gfx::TEX_GEN);
  m_override.on_bit(Gfx::TEX_GEN);
}

//! \brief
void Appearance::set_tex_gen_enable(Boolean tex_gen_enable)
{
  m_pending.on_bit(Gfx::TEX_GEN_ENABLE);
  m_override.on_bit(Gfx::TEX_GEN_ENABLE);
  m_tex_gen_enable = tex_gen_enable;
}

//! \brief sets the material attribute.
void Appearance::set_material(Shared_material material)
{
  SGAL_assertion(material);
  Observer observer(this, get_field_info(MATERIAL));
  if (m_material) m_material->unregister_observer(observer);
  m_material = material;
  m_material->register_observer(observer);

  if (material->get_transparency() != 0.0f) {
    set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  }
  else {
    set_src_blend_func(Gfx::ONE_SBLEND);
    set_dst_blend_func(Gfx::ZERO_DBLEND);
  }

  m_pending.on_bit(Gfx::MATERIAL);
  m_override.on_bit(Gfx::MATERIAL);
}

//! \brief processes change of material.
void Appearance::material_changed(const Field_info* /* field_info */)
{
  if (!m_material) return;
  if (m_material->get_transparency() != 0.0f) {
    set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  }

  m_pending.on_bit(Gfx::MATERIAL);
  m_override.on_bit(Gfx::MATERIAL);
  process_content_changed();
}

//! \brief notifies that the back material has changed.
void Appearance::back_material_changed(const Field_info* /* field_info */)
{
  //! \todo need to unregister the old back material.
  m_pending.on_bit(Gfx::BACK_MATERIAL);
  m_override.on_bit(Gfx::BACK_MATERIAL);
  process_content_changed();
}

//! \brief
void Appearance::set_back_material(Shared_material material)
{
  Observer observer(this, get_field_info(BACK_MATERIAL));
  if (m_back_material) m_back_material->unregister_observer(observer);
  m_back_material = material;
  if (m_back_material) m_back_material->register_observer(observer);
  m_pending.on_bit(Gfx::BACK_MATERIAL);
  m_override.on_bit(Gfx::BACK_MATERIAL);
}

//! \brief
void Appearance::set_light_enable(Boolean light_enable)
{
  m_pending.on_bit(Gfx::LIGHT_ENABLE);
  m_override.on_bit(Gfx::LIGHT_ENABLE);
  m_light_enable = light_enable;
}

//! \brief
void Appearance::set_shade_model(Gfx::Shade_model shade_model)
{
  m_pending.on_bit(Gfx::SHADE_MODEL);
  m_override.on_bit(Gfx::SHADE_MODEL);
  m_shade_model = shade_model;
}

//! \brief
void Appearance::set_transp_enable(Boolean transp_enable)
{
  m_pending.on_bit(Gfx::TRANSP_ENABLE);
  m_override.on_bit(Gfx::TRANSP_ENABLE);
  m_transp_enable = transp_enable;
}

//! \brief
void Appearance::set_transp_mode(Gfx::Transparency_mode transp_mode)
{
  m_pending.on_bit(Gfx::TRANSP_MODE);
  m_override.on_bit(Gfx::TRANSP_MODE);
  m_transp_mode = transp_mode;
}

//! \brief
void Appearance::set_alpha_func(Gfx::Alpha_func alpha_func)
{
  m_pending.on_bit(Gfx::ALPHA_FUNC);
  m_override.on_bit(Gfx::ALPHA_FUNC);
  m_alpha_func = alpha_func;
}

//! \brief
void Appearance::set_alpha_ref(Float alpha_ref)
{
  m_pending.on_bit(Gfx::ALPHA_REF);
  m_override.on_bit(Gfx::ALPHA_REF);
  m_alpha_ref = alpha_ref;
}

//! \brief
void Appearance::set_blend_color(const Vector4f& blend_color)
{
  set_blend_color(blend_color[0], blend_color[1],
                  blend_color[2], blend_color[3]);
}

//! \brief
void Appearance::set_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  m_pending.on_bit(Gfx::BLEND_COLOR);
  m_override.on_bit(Gfx::BLEND_COLOR);
  m_blend_color.set(v0, v1, v2, v3);
}

//! \brief
void Appearance::set_src_blend_func(Gfx::Src_blend_func src_blend_func)
{
  m_pending.on_bit(Gfx::SRC_BLEND_FUNC);
  m_override.on_bit(Gfx::SRC_BLEND_FUNC);
  m_src_blend_func = src_blend_func;
}

//! \brief
void Appearance::set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func)
{
  m_pending.on_bit(Gfx::DST_BLEND_FUNC);
  m_override.on_bit(Gfx::DST_BLEND_FUNC);
  m_dst_blend_func = dst_blend_func;
}

//! \brief
void Appearance::set_color_mask(const Vector4ub& color_mask)
{ set_color_mask(color_mask[0], color_mask[1], color_mask[2], color_mask[3]); }

//! \brief
void Appearance::set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3)
{
  m_pending.on_bit(Gfx::COLOR_MASK);
  m_override.on_bit(Gfx::COLOR_MASK);
  m_color_mask.set(v0, v1, v2, v3);
}

//! \brief
void Appearance::set_depth_enable(Boolean depth_enable)
{
  m_pending.on_bit(Gfx::DEPTH_ENABLE);
  m_override.on_bit(Gfx::DEPTH_ENABLE);
  m_depth_enable = depth_enable;
}

//! \brief
void Appearance::set_depth_func(Gfx::Depth_func depth_func)
{
  m_pending.on_bit(Gfx::DEPTH_FUNC);
  m_override.on_bit(Gfx::DEPTH_FUNC);
  m_depth_func = depth_func;
}

//! \brief
void Appearance::set_depth_mask(Boolean depth_mask)
{
  m_pending.on_bit(Gfx::DEPTH_MASK);
  m_override.on_bit(Gfx::DEPTH_MASK);
  m_depth_mask = depth_mask;
}

//! \brief
void Appearance::set_fog_enable(Boolean fog_enable)
{
  m_pending.on_bit(Gfx::FOG_ENABLE);
  m_override.on_bit(Gfx::FOG_ENABLE);
  m_fog_enable = fog_enable;
}

//! \brief sets the polygon-stipple enable flag.
void Appearance::set_polygon_stipple_enable(Boolean enable)
{
  m_pending.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  m_override.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  m_polygon_stipple_enable = enable;
}

//! \brief
void Appearance::set_material_mode_enable(Gfx::Material_mode enable)
{
  m_pending.on_bit(Gfx::MATERIAL_MODE_ENABLE);
  m_override.on_bit(Gfx::MATERIAL_MODE_ENABLE);
  m_material_mode_enable = enable;
}

//! \brief
void Appearance::set_poly_mode(Gfx::Poly_mode poly_mode)
{
  m_pending.on_bit(Gfx::POLY_MODE);
  m_override.on_bit(Gfx::POLY_MODE);
  m_poly_mode = poly_mode;
}

//! \brief
void Appearance::set_line_stipple_pattern(Uint pattern)
{
  m_pending.on_bit(Gfx::LINE_STIPPLE_PATTERN);
  m_override.on_bit(Gfx::LINE_STIPPLE_PATTERN);
  m_line_stipple_pattern = pattern;
}

//! \brief
void Appearance::set_line_stipple_factor(Uint factor)
{
  m_pending.on_bit(Gfx::LINE_STIPPLE_FACTOR);
  m_override.on_bit(Gfx::LINE_STIPPLE_FACTOR);
  m_line_stipple_factor = factor;
}

/*! \brief sets the attribute that specifies whether a single color should be
 * generated from the lighting computation for a vertex.
 */
void
Appearance::set_light_model_color_control(Gfx::Light_model_color_control model)
{
  m_pending.on_bit(Gfx::LIGHT_MODEL_COLOR_CONTROL);
  m_override.on_bit(Gfx::LIGHT_MODEL_COLOR_CONTROL);
  m_light_model_color_control = model;
}

//! \brief
void Appearance::set_inherit(const Bit_mask& inherit) { m_pending = inherit; }

//! \brief
void Appearance::get_inherit(Bit_mask& inherit) const { inherit = m_pending; }

//! \brief applies the appearance.
void Appearance::draw(Draw_action* action)
{
  Context* context = action->get_context();
  if (context == nullptr) return;
  context->draw_app(this);
}

//! \brief determines whether the appearance is translucent.
Boolean Appearance::is_transparent() const
{
  return ((m_src_blend_func != Gfx::ONE_SBLEND) ||
          (m_dst_blend_func != Gfx::ZERO_DBLEND));
}

//! \brief notifies that the texture has been changed.
void Appearance::texture_changed(const Field_info* /* field_info */)
{
  m_pending.on_bit(Gfx::TEXTURE);
  m_override.on_bit(Gfx::TEXTURE);

  // this is to indicate that the texture has changed and
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
  process_content_changed();
}

//! \brief notifies that the texture transform has been changed.
void Appearance::tex_transform_changed(const Field_info* /* field_info */)
{
  m_pending.on_bit(Gfx::TEX_TRANSFORM);
  m_override.on_bit(Gfx::TEX_TRANSFORM);
  process_content_changed();
}

//! \brief notifies that halftone has been changed.
void Appearance::halftone_changed(const Field_info* /* field_info */)
{
  m_pending.on_bit(Gfx::HALFTONE_PATTERN);
  m_override.on_bit(Gfx::HALFTONE_PATTERN);
  m_dirty_flags.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  set_rendering_required();
  process_content_changed();
}

//! \brief notifies that the texture generation has changed.
void Appearance::tex_gen_changed(const Field_info* /* field_info */)
{
  m_pending.on_bit(Gfx::TEX_GEN);
  m_override.on_bit(Gfx::TEX_GEN);
  process_content_changed();
}

//! \brief
Boolean Appearance::attach_context(Context* context)
{
  Boolean result = Container::attach_context(context);
  Shared_texture texture = get_texture();
  if (texture) result &= texture->attach_context(context);
  return result;
}

//! \brief
Boolean Appearance::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
  Shared_texture texture = get_texture();
  if (texture) result &= texture->detach_context(context);
  return result;
}

//! \brief initializes the appearance prototype.
void Appearance::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;

  exec_func = static_cast<Execution_function>(&Appearance::material_changed);
  Shared_container_handle_function material_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Appearance::material_handle);
  s_prototype->add_field_info(new SF_shared_container(MATERIAL, "material",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      material_func,
                                                      exec_func));

  exec_func = static_cast<Execution_function>(&Appearance::texture_changed);
  Shared_container_handle_function texture_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Appearance::texture_handle);
  s_prototype->add_field_info(new SF_shared_container(TEXTURE, "texture",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      texture_func, exec_func));

  exec_func = static_cast<Execution_function>(&Appearance::tex_gen_changed);
  Shared_container_handle_function tex_gen_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Appearance::tex_gen_handle);
  s_prototype->add_field_info(new SF_shared_container(TEX_GEN,
                                                      "textureGeneration",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      tex_gen_func, exec_func));

  exec_func = static_cast<Execution_function>(&Appearance::halftone_changed);
  Shared_container_handle_function halftone_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Appearance::halftone_handle);
  s_prototype->add_field_info(new SF_shared_container(HALFTONE_PATTERN,
                                                      "halftone",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      halftone_func,
                                                      exec_func));

  exec_func =
    static_cast<Execution_function>(&Appearance::back_material_changed);
  Shared_container_handle_function back_material_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Appearance::back_material_handle);
  s_prototype->add_field_info(new SF_shared_container(BACK_MATERIAL,
                                                      "backMaterial",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      back_material_func,
                                                      exec_func));

  // drawDepth
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto depth_mask_func =
    static_cast<Boolean_handle_function>(&Appearance::depth_mask_handle);
  s_prototype->add_field_info(new SF_bool(DEPTH_MASK, "drawDepth",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          depth_mask_func, true, exec_func));

  // testDepth
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto depth_enable_func =
    static_cast<Boolean_handle_function>(&Appearance::depth_enable_handle);
  s_prototype->add_field_info(new SF_bool(DEPTH_ENABLE, "testDepth",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          depth_enable_func, true, exec_func));

  // testFunction
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto depth_func_func =
    static_cast<Uint_handle_function>(&Appearance::depth_func_handle);
  s_prototype->add_field_info(new SF_uint(DEPTH_FUNC, "testFunction",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          depth_func_func, true, exec_func));
}

//! \brief deletes the appearance prototype.
void Appearance::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the appearance prototype.
Container_proto* Appearance::get_prototype()
{
  if (s_prototype == nullptr) init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the appearance.
void Appearance::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "polyFillMode") {
      if (value == "point") set_poly_mode(Gfx::POINT_PMODE);
      else if (value == "line") set_poly_mode(Gfx::LINE_PMODE);
      else set_poly_mode(Gfx::FILL_PMODE);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "shadeModel") {
      if (value == "flat") set_shade_model(Gfx::FLAT_SHADE);
      else set_shade_model(Gfx::SMOOTH_SHADE);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "texEnv") {
      if ("MODULATE" == value) set_tex_env(Gfx::MODULATE_TENV);
      else if ("DECAL" == value) set_tex_env(Gfx::DECAL_TENV);
      else if ("BLEND" == value) set_tex_env(Gfx::BLEND_TENV);
      else if ("REPLACE" == value) set_tex_env(Gfx::REPLACE_TENV);
      else if ("ADD" == value) set_tex_env(Gfx::ADD_TENV);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "polygonStipple") {
      if (!to_boolean(value)) set_polygon_stipple_enable(true);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDepth") {
      set_depth_mask(to_boolean(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "testDepth") {
      set_depth_enable(to_boolean(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthFunction") {
      auto num = sizeof(s_depth_func_names) / sizeof(char*);
      const auto** found =
        std::find(s_depth_func_names, &s_depth_func_names[num], value);
      auto index = found - s_depth_func_names;
      if (index < num) set_depth_func(static_cast<Gfx::Depth_func>(index));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "material") {
      Shared_material material = boost::dynamic_pointer_cast<Material>(cont);
      if (material) {
        set_material(material);
        if (!get_back_material()) set_back_material(material);
      }
      elem->mark_delete(cai);
      continue;
    }
    if (name == "texture") {
      Shared_texture texture = boost::dynamic_pointer_cast<Texture>(cont);
      set_texture(texture);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "textureTransform") {
      Shared_texture_transform texture_transform =
        boost::dynamic_pointer_cast<Texture_transform>(cont);
      set_tex_transform(texture_transform);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "halftone") {
      Shared_halftone halftone = boost::dynamic_pointer_cast<Halftone>(cont);
      set_polygon_stipple_enable(true);
      set_halftone(halftone);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! \brief
Attribute_list Appearance::get_attributes()
{
  Attribute_list attrs;
  Attribue attrib;

  attrs = Container::get_attributes();

  if (m_poly_mode != s_def_poly_mode) {
    attrib.first = "polyFillMode";
    switch (get_poly_mode()) {
     case Gfx::LINE_PMODE: attrib.second = "line"; break;
     case Gfx::POINT_PMODE: attrib.second = "point"; break;
     case Gfx::FILL_PMODE: break;
    }
    attrs.push_back(attrib);
  }

  if (m_shade_model != s_def_shade_model) {
    attrib.first = "shadeModel";
    attrib.second = "flat";
    attrs.push_back(attrib);
  }

  if (m_tex_env != s_def_tex_env) {
    attrib.first = "texEnv";
    if (m_tex_env == Gfx::MODULATE_TENV) {
      attrib.second = "MODULATE";
    }
    else if (m_tex_env == Gfx::DECAL_TENV) {
      attrib.second = "DECAL";
    //} else if (m_tex_env == Gfx::BLEND_TENV) {
    //  attrib.second = "BLEND";
    //} else if (m_tex_env == Gfx::REPLACE_TENV) {
    //  attrib.second = "REPLACE";
    //} else if (m_tex_env == Gfx::ADD_TENV) {
    //  attrib.second = "ADD";
    }
    attrs.push_back(attrib);
  }

  return attrs;
}

#endif

//! \brief cleans the texture enable flag.
void Appearance::clean_tex_enable()
{ if (m_texture && !m_texture->empty()) set_tex_enable(true); }

/*! \brief cleans the texture environment attribute.
 * \todo The texture image should modify the diffuse color according to the
 * number of components as follows:
 * 1. Diffuse color is multiplied by the greyscale values in the texture image.
 * 2. Diffuse color is multiplied by the greyscale values in the texture image;
 *    material transparency is multiplied by transparency values in texture
 *    image.
 * 3. RGB colors in the texture image replace the material's diffuse color.
 * 4. RGB colors in the texture image replace the material's diffuse color;
 *    transparency values in the texture image replace the material's
 *    transparency.
 *
 * For 1 and 2 components the OpenGL texture environment can be utilized to
 * apply the specified behavior above. However, in order to replace the
 * diffuse color by the texture color shaders must be applied.
 */
void Appearance::clean_tex_env()
{
  if (!m_tex_enable) return;

  SGAL_assertion(m_texture);
  SGAL_assertion(m_material);
  Uint num_compnents = m_texture->get_component_count();
  if ((1 == num_compnents) || (2 == num_compnents))
    set_tex_env(Gfx::MODULATE_TENV);
  else {
    const Vector3f& diffuse_color = m_material->get_diffuse_color();
    if ((diffuse_color[0] == 0) && (diffuse_color[1] == 0) &&
        (diffuse_color[2] == 0))
      set_tex_env(Gfx::ADD_TENV);
    else
      set_tex_env(Gfx::MODULATE_TENV);
  }
}

//! \brief cleans the blend functions.
void Appearance::clean_blend_func()
{
  // If texture is enabled and texture is either 2 components or 4 components,
  // and the texture environment is either MODULATE, BLEND, or ADD, override
  // the blend functions.
  if (!m_tex_enable) return;

  SGAL_assertion(m_texture);
  Uint num_compnents = m_texture->get_component_count();
  if ((num_compnents == 2) || (num_compnents == 4)) {
    if ((Gfx::ADD_TENV == m_tex_env) || (Gfx::MODULATE_TENV == m_tex_env)) {
      set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
      set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    }
    else if (m_tex_env == Gfx::ADD_TENV) {
      set_src_blend_func(Gfx::ONE_SBLEND);
      set_dst_blend_func(Gfx::ONE_DBLEND);
    }
  }
}

//! \brief cleans the light model.
void Appearance::clean_light_model()
{
  if (!m_tex_enable) return;

  SGAL_assertion(m_material);
  Gfx::Light_model_color_control color_control = Gfx::SEPARATE_SPECULAR_COLOR;
  if (m_material) {
    const Vector3f& specular_color = m_material->get_specular_color();
    if ((specular_color[0] == 0) && (specular_color[1] == 0) &&
        (specular_color[2] == 0))
      color_control = Gfx::SINGLE_COLOR;
  }
  set_light_model_color_control(color_control);
}

//! \brief cleans the material attribute.
void Appearance::clean_material()
{ if (!m_material) m_material = Shared_material(new Material()); }

//! \brief cleans the texture generation attribute.
void Appearance::clean_tex_gen()
{
  if (!m_tex_gen) m_tex_gen = Shared_tex_gen(new Tex_gen());
  set_tex_gen_enable(true);

  // Setup the textute-generation functions.
  Shared_texture texture = get_texture();
  if (boost::dynamic_pointer_cast<Texture_2d>(texture)) {
    // Setup standard texture map if requested:
#if 0
    get_tex_gen()->set_mode_s(Tex_gen::EYE_LINEAR);
    get_tex_gen()->set_mode_t(Tex_gen::EYE_LINEAR);
#else
    get_tex_gen()->set_mode_s(Tex_gen::OBJECT_LINEAR);
    get_tex_gen()->set_mode_t(Tex_gen::OBJECT_LINEAR);
#endif
  }
  else if (boost::dynamic_pointer_cast<Sphere_environment>(texture)) {
    // Setup sphere environment map if requested:
    get_tex_gen()->set_mode_s(Tex_gen::SPHERE_MAP);
    get_tex_gen()->set_mode_t(Tex_gen::SPHERE_MAP);
  }
  else if (boost::dynamic_pointer_cast<Cube_environment>(texture)) {
    // Setup cube environment map if requested:
#if 0
    get_tex_gen()->set_mode_s(Tex_gen::NORMAL_MAP);
    get_tex_gen()->set_mode_t(Tex_gen::NORMAL_MAP);
    get_tex_gen()->set_mode_r(Tex_gen::NORMAL_MAP);
#else
    get_tex_gen()->set_mode_s(Tex_gen::REFLECTION_MAP);
    get_tex_gen()->set_mode_t(Tex_gen::REFLECTION_MAP);
    get_tex_gen()->set_mode_r(Tex_gen::REFLECTION_MAP);
#endif
  }
}

//! \brief Process change of field.
void Appearance::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case MATERIAL:
    SGAL_assertion(m_material);
    if (m_material->get_transparency() != 0.0f) {
      set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
      set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    }
    else {
      set_src_blend_func(Gfx::ONE_SBLEND);
      set_dst_blend_func(Gfx::ZERO_DBLEND);
    }

    m_pending.on_bit(Gfx::MATERIAL);
    m_override.on_bit(Gfx::MATERIAL);
    break;

   case BACK_MATERIAL:
    m_pending.on_bit(Gfx::BACK_MATERIAL);
    m_override.on_bit(Gfx::BACK_MATERIAL);
    break;

   case TEXTURE:
    m_pending.on_bit(Gfx::TEXTURE);
    m_override.on_bit(Gfx::TEXTURE);
    break;

   case TEX_TRANSFORM:
    m_pending.on_bit(Gfx::TEX_TRANSFORM);
    m_override.on_bit(Gfx::TEX_TRANSFORM);
    break;

   case TEX_GEN:
    m_pending.on_bit(Gfx::TEX_GEN);
    m_override.on_bit(Gfx::TEX_GEN);
    break;

   case HALFTONE_PATTERN:
    m_pending.on_bit(Gfx::HALFTONE_PATTERN);
    m_override.on_bit(Gfx::HALFTONE_PATTERN);
    break;

   default: break;
  }
  process_content_changed();
}

//! \brief writes a field of this container.
void Appearance::write_field(const Field_info* field_info,
                              Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (BACK_MATERIAL == field_info->get_id()) {
      if (get_material() == get_back_material()) return;
      field_info->write(this, formatter);
      return;
    }
  }
  field_info->write(this, formatter);
}

SGAL_END_NAMESPACE
