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
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Halftone.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

std::string Appearance::s_tag = "sgalAppearance";
Container_proto* Appearance::s_prototype = NULL;

// Default values:
Gfx::Poly_mode Appearance::m_def_poly_mode(Gfx::FILL_PMODE);
Gfx::Shade_model Appearance::m_def_shade_model(Gfx::SMOOTH_SHADE);
Gfx::Tex_env Appearance::m_def_tex_env(Gfx::MODULATE_TENV);

REGISTER_TO_FACTORY(Appearance, "Appearance");

/*! The parameter-less constructor */
Appearance::Appearance(Boolean proto) :
  Container(proto),
  m_tex_env(m_def_tex_env),
  m_dirty(true),
  m_own_material(false),
  m_own_tex_gen(false)
{ init(); }

/*! Destructor */
Appearance::~Appearance()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Appearance ...");
  // we do not delete the object that the appearance is pointing to since other
  // appearance can point ot them as well.
  if (m_own_material) {
    if (m_material) {
      delete m_material;
      m_material = NULL;
    }
    m_own_material = false;
  }
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/*! \brief assigns the appearance with the content of another appearance;
 * performs a shallow copy.
 */
void Appearance::set(Appearance* app)
{
  m_texture                = app->m_texture;
  m_tex_enable             = app->m_tex_enable;
  m_tex_mode               = app->m_tex_mode;
  m_tex_blend_color        = app->m_tex_blend_color;
  m_tex_env                = app->m_tex_env;
  m_tex_gen                = app->m_tex_gen;
  m_tex_gen_enable         = app->m_tex_gen_enable;
  m_material               = app->m_material;
  m_material_mode_enable   = app->m_material_mode_enable;
  m_light_enable           = app->m_light_enable;
  m_shade_model            = app->m_shade_model;
  m_transp_enable          = app->m_transp_enable;
  m_transp_mode            = app->m_transp_mode;
  m_alpha_func             = app->m_alpha_func;
  m_alpha_ref              = app->m_alpha_ref;
  m_blend_color            = app->m_blend_color;
  m_src_blend_func         = app->m_src_blend_func;
  m_dst_blend_func         = app->m_dst_blend_func;
  m_color_mask             = app->m_color_mask;
  m_depth_enable           = app->m_depth_enable;
  m_depth_func             = app->m_depth_func;
  m_depth_mask             = app->m_depth_mask;
  m_fog_enable             = app->m_fog_enable;
  m_poly_mode              = app->m_poly_mode;
  m_line_stipple_factor    = app->m_line_stipple_factor;
  m_line_stipple_pattern   = app->m_line_stipple_pattern;
  m_tex_transform          = app->m_tex_transform;
  m_back_material          = app->m_back_material;
  m_polygon_stipple_enable = app->m_polygon_stipple_enable;
  m_halftone               = app->m_halftone;

  m_pending = app->m_pending;
  m_override = app->m_override;
}

/*! \brief */
void Appearance::init()
{
  m_texture                = 0;
  m_tex_enable             = false;
  m_tex_mode               = Gfx::FAST_TEX;
  m_tex_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_tex_env                = Gfx::MODULATE_TENV;
  m_tex_gen                = 0;
  m_tex_gen_enable         = false;
  m_material               = 0;
  m_material_mode_enable   = Gfx::NO_COLOR_MATERIAL;
  m_light_enable           = 0;
  m_shade_model            = Gfx::SMOOTH_SHADE;
  m_transp_enable          = false;
  m_transp_mode            = Gfx::BLEND_TRANSP;
  m_alpha_func             = Gfx::ALWAYS_AFUNC;
  m_alpha_ref              = 0.0f;
  m_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_src_blend_func         = Gfx::ONE_SBLEND;
  m_dst_blend_func         = Gfx::ZERO_DBLEND;
  m_color_mask.set(0xff, 0xff, 0xff, 0xff);
  m_depth_enable           = false;
  m_depth_func             = Gfx::LESS_DFUNC;
  m_depth_mask             = true;
  m_fog_enable             = false;
  m_poly_mode              = m_def_poly_mode;
  m_line_stipple_factor    = 1;
  m_line_stipple_pattern   = 0xffff;
  m_tex_transform.make_identity();
  m_back_material          = 0;
  m_polygon_stipple_enable = false;
  m_halftone               = 0;

  m_pending.off();
  m_override.off();
}

/*!
 */
void Appearance::set_texture(Texture* texture)
{
  m_texture = texture;
  m_pending.on_bit(Gfx::TEXTURE);
  m_override.on_bit(Gfx::TEXTURE);

  // this is to indicate that the texture has changed and 
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
}

/*!
 */
void Appearance::set_halftone(Halftone* halftone)
{
  m_halftone = halftone;
  m_pending.on_bit(Gfx::HALFTONE_PATTERN);
  m_override.on_bit(Gfx::HALFTONE_PATTERN);

  m_dirty_flags.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
}

/*!
 */
void Appearance::set_tex_enable(Boolean tex_enable)
{
  m_pending.on_bit(Gfx::TEX_ENABLE);
  m_override.on_bit(Gfx::TEX_ENABLE);
  m_tex_enable = tex_enable;
}

/*!
 */
void Appearance::set_tex_mode(Gfx::Tex_mode tex_mode)
{
  m_pending.on_bit(Gfx::TEX_MODE);
  m_override.on_bit(Gfx::TEX_MODE);
  m_tex_mode = tex_mode;
}

/*!
 */
void Appearance::set_tex_blend_color(const Vector4f& tex_blend_color)
{
  set_tex_blend_color(tex_blend_color[0], tex_blend_color[1],
                      tex_blend_color[2], tex_blend_color[3]);
}

/*!
 */
void Appearance::set_tex_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  m_pending.on_bit(Gfx::TEX_BLEND_COLOR);
  m_override.on_bit(Gfx::TEX_BLEND_COLOR);
  m_tex_blend_color.set(v0, v1, v2, v3);
}

/*!
 */
void Appearance::set_tex_blend(float* blend_color)
{
  if (m_tex_env == Gfx::REPLACE_TENV || m_tex_env == Gfx::BLEND_TENV) {
    set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    if (blend_color && m_tex_env == Gfx::BLEND_TENV) {
      set_tex_blend_color(blend_color[0], blend_color[1],
                          blend_color[2], blend_color[3]);
    }
  } else {
    if (m_tex_env != Gfx::DECAL_TENV && m_texture &&
        m_texture->get_component_count() == 4)
    {
      //Timur[24/6/2001] 
      // Texture include alpha channel.
      // Must set blend functions.
      if (m_tex_env == Gfx::MODULATE_TENV) {
        set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
        set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      }
    }
    if (m_tex_env == Gfx::ADD_TENV) {
      m_tex_env = Gfx::MODULATE_TENV;
      set_src_blend_func(Gfx::ONE_SBLEND);
      set_dst_blend_func(Gfx::ONE_DBLEND);
    }
  }
  set_tex_env(m_tex_env);
}

/*!
 */
void Appearance::set_tex_env(Gfx::Tex_env tex_env)
{
  m_pending.on_bit(Gfx::TEX_ENV);
  m_override.on_bit(Gfx::TEX_ENV);
  m_tex_env = tex_env;
}

/*!
 */
void Appearance::set_tex_gen(Tex_gen* tex_gen)
{
  m_tex_gen = tex_gen;
  m_pending.on_bit(Gfx::TEX_GEN);
  m_override.on_bit(Gfx::TEX_GEN);
}

/*! \brief */
void Appearance::set_tex_gen_enable(Boolean tex_gen_enable)
{
  m_pending.on_bit(Gfx::TEX_GEN_ENABLE);
  m_override.on_bit(Gfx::TEX_GEN_ENABLE);
  m_tex_gen_enable = tex_gen_enable;
}

/*! \brief */
void Appearance::set_material(Material* material)
{
  if (m_own_material) {
    if (m_material) {
      delete m_material;
      m_material = NULL;
    }
    m_own_material = false;
  }
  m_pending.on_bit(Gfx::MATERIAL);
  m_override.on_bit(Gfx::MATERIAL);
  m_material = material;
}

/*! \brief */
void Appearance::material_changed(Field_info* /* field_info */)
{
  //! \todo what if m_own_material is set?
  m_pending.on_bit(Gfx::MATERIAL);
  m_override.on_bit(Gfx::MATERIAL);
}

/*! \brief */
void Appearance::set_back_material(Material* material)
{
  m_pending.on_bit(Gfx::BACK_MATERIAL);
  m_override.on_bit(Gfx::BACK_MATERIAL);
  m_back_material = material;
}

/*! \brief */
void Appearance::set_light_enable(Boolean light_enable)
{
  m_pending.on_bit(Gfx::LIGHT_ENABLE);
  m_override.on_bit(Gfx::LIGHT_ENABLE);
  m_light_enable = light_enable;
}

/*! \brief */
void Appearance::set_shade_model(Gfx::Shade_model shade_model)
{
  m_pending.on_bit(Gfx::SHADE_MODEL);
  m_override.on_bit(Gfx::SHADE_MODEL);
  m_shade_model = shade_model;
}

/*! \brief */
void Appearance::set_transp_enable(Boolean transp_enable)
{
  m_pending.on_bit(Gfx::TRANSP_ENABLE);
  m_override.on_bit(Gfx::TRANSP_ENABLE);
  m_transp_enable = transp_enable;
}

/*! \brief */
void Appearance::set_transp_mode(Gfx::Transparency_mode transp_mode)
{
  m_pending.on_bit(Gfx::TRANSP_MODE);
  m_override.on_bit(Gfx::TRANSP_MODE);
  m_transp_mode = transp_mode;
}

/*! \brief */
void Appearance::set_alpha_func(Gfx::Alpha_func alpha_func)
{
  m_pending.on_bit(Gfx::ALPHA_FUNC);
  m_override.on_bit(Gfx::ALPHA_FUNC);
  m_alpha_func = alpha_func;
}

/*! \brief */
void Appearance::set_alpha_ref(Float alpha_ref)
{
  m_pending.on_bit(Gfx::ALPHA_REF);
  m_override.on_bit(Gfx::ALPHA_REF);
  m_alpha_ref = alpha_ref;
}

/*! \brief */
void Appearance::set_blend_color(const Vector4f& blend_color)
{
  set_blend_color(blend_color[0], blend_color[1],
                  blend_color[2], blend_color[3]);
}

/*! \brief */
void Appearance::set_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  m_pending.on_bit(Gfx::BLEND_COLOR);
  m_override.on_bit(Gfx::BLEND_COLOR);
  m_blend_color.set(v0, v1, v2, v3);
}

/*! \brief */
void Appearance::set_src_blend_func(Gfx::Src_blend_func src_blend_func)
{
  m_pending.on_bit(Gfx::SRC_BLEND_FUNC);
  m_override.on_bit(Gfx::SRC_BLEND_FUNC);
  m_src_blend_func = src_blend_func;
}

/*! \brief */
void Appearance::set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func)
{
  m_pending.on_bit(Gfx::DST_BLEND_FUNC);
  m_override.on_bit(Gfx::DST_BLEND_FUNC);
  m_dst_blend_func = dst_blend_func;
}

/*! \brief */
void Appearance::set_color_mask(const Vector4ub& color_mask)
{ set_color_mask(color_mask[0], color_mask[1], color_mask[2], color_mask[3]); }

/*! \brief */
void Appearance::set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3)
{
  m_pending.on_bit(Gfx::COLOR_MASK);
  m_override.on_bit(Gfx::COLOR_MASK);
  m_color_mask.set(v0, v1, v2, v3);
}

/*! \brief */
void Appearance::set_depth_enable(Boolean depth_enable)
{
  m_pending.on_bit(Gfx::DEPTH_ENABLE);
  m_override.on_bit(Gfx::DEPTH_ENABLE);
  m_depth_enable = depth_enable;
}

/*! \brief */
void Appearance::set_depth_func(Gfx::Depth_func depth_func)
{
  m_pending.on_bit(Gfx::DEPTH_FUNC);
  m_override.on_bit(Gfx::DEPTH_FUNC);
  m_depth_func = depth_func;
}

/*! \brief */
void Appearance::set_depth_mask(Boolean depth_mask)
{
  m_pending.on_bit(Gfx::DEPTH_MASK);
  m_override.on_bit(Gfx::DEPTH_MASK);
  m_depth_mask = depth_mask;
}

/*! \brief */
void Appearance::set_fog_enable(Boolean fog_enable)
{
  m_pending.on_bit(Gfx::FOG_ENABLE);
  m_override.on_bit(Gfx::FOG_ENABLE);
  m_fog_enable = fog_enable;
}

/*! Set the polygon-stipple enable flag */
void Appearance::set_polygon_stipple_enable(Boolean enable)
{
  m_pending.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  m_override.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  m_polygon_stipple_enable = enable;
}

/*! \brief */
void Appearance::set_material_mode_enable(Gfx::Material_mode enable)
{
  m_pending.on_bit(Gfx::MATERIAL_MODE_ENABLE);
  m_override.on_bit(Gfx::MATERIAL_MODE_ENABLE);
  m_material_mode_enable = enable;
}

/*! \brief */
void Appearance::set_poly_mode(Gfx::Poly_mode poly_mode)
{
  m_pending.on_bit(Gfx::POLY_MODE);
  m_override.on_bit(Gfx::POLY_MODE);
  m_poly_mode = poly_mode;
}

/*! \brief */
void Appearance::set_line_stipple_pattern(Uint pattern)
{
  m_pending.on_bit(Gfx::LINE_STIPPLE_PATTERN);
  m_override.on_bit(Gfx::LINE_STIPPLE_PATTERN);
  m_line_stipple_pattern = pattern;
}

/*! \brief */
void Appearance::set_line_stipple_factor(Uint factor)
{
  m_pending.on_bit(Gfx::LINE_STIPPLE_FACTOR);
  m_override.on_bit(Gfx::LINE_STIPPLE_FACTOR);
  m_line_stipple_factor = factor;
}

/*! \brief */
void Appearance::set_tex_transform(const Matrix4f& tex_transform)
{
  m_pending.on_bit(Gfx::TEX_TRANSFORM);
  m_override.on_bit(Gfx::TEX_TRANSFORM);
  m_tex_transform = tex_transform;
}

/*! \brief */
void Appearance::set_inherit(const Bit_mask& inherit) { m_pending = inherit; }

/*! \brief */
void Appearance::get_inherit(Bit_mask& inherit) const { inherit = m_pending; }

/*! \brief */
void Appearance::draw(Draw_action* action)
{
  if (m_dirty) clean();
  
  // Obtain the configuration if exists:
  SGAL::Configuration* conf = action->get_configuration();
  
  if (m_dirty_flags.get_bit(Gfx::TEX_ENV) && m_texture && !m_texture->empty())
  {
    set_tex_blend();
    m_dirty_flags.off_bit(Gfx::TEX_ENV);
  }

  Context* context = action->get_context();
  if (context == NULL) return;

  context->draw_app(this);

  // disable the texture in case the image is not loaded yet
  if ((m_texture && m_texture->empty()) ||
      (conf && !conf->is_texture_map()))
    context->draw_tex_enable(false);
}

/*! \brief returns true if the material is transparent. If no material specified
 * return false.
 * @return true if the material is transparent.
 */
Boolean Appearance::is_transparent() const
{
  if (!m_material) return false;

  if (m_material->get_transparency() != 0) return true;

  if (m_src_blend_func != Gfx::ONE_SBLEND ||
    m_dst_blend_func != Gfx::ZERO_DBLEND) {
    return true;
  }

  return false;
}

/*! \brief notifies that the texture has been changed. */
void Appearance::texture_changed(Field_info* /* field_info */)
{
  // this is to indicate that the texture has changed and 
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
  set_rendering_required();
}

/*! \brief notifies that halftone has been changed. */
void Appearance::halftone_changed(Field_info* /* field_info */)
{
  // this is to indicate that the texture has changed and 
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
  set_rendering_required();
}

/*! \brief */
Boolean Appearance::attach_context(Context* context)
{
  Boolean result = Container::attach_context(context);
    Texture * texture = get_texture();
  if (texture)
    result &= texture->attach_context(context);

  return result;
}

/*! \brief */
Boolean Appearance::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
    Texture* texture = get_texture();
  if (texture)
    result &= texture->detach_context(context);

  return result;
}

/*! \brief initializes the appearance prototype. */
void Appearance::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;
  
  exec_func = static_cast<Execution_function>(&Appearance::material_changed);
  s_prototype->add_field_info(new SF_container(MATERIAL, "material",
                                               get_member_offset(&m_material),
                                               exec_func));

  exec_func = static_cast<Execution_function>(&Appearance::texture_changed);
  s_prototype->add_field_info(new SF_container(TEXTURE, "texture",
                                               get_member_offset(&m_texture),
                                               exec_func));

  exec_func = static_cast<Execution_function>(&Appearance::halftone_changed);
  s_prototype->add_field_info(new SF_container(HALFTONE_PATTERN, "halftone",
                                               get_member_offset(&m_halftone),
                                               exec_func));
}

/*! \brief deletes the appearance prototype. */
void Appearance::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the appearance prototype. */
Container_proto* Appearance::get_prototype() 
{  
  if (s_prototype == NULL) init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the appearance. */
void Appearance::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
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
      if (value.compare("MODULATE") == 0) {
        set_tex_env(Gfx::MODULATE_TENV);
      } else if (value.compare("DECAL") == 0) {
        set_tex_env(Gfx::DECAL_TENV);
      //} else if (value.equal("BLEND")) {
      //  m_tex_env = Gfx::BLEND_TENV;
      //} else if (value.equal("REPLACE")) {
      //  m_tex_env = Gfx::REPLACE_TENV;
      //} else if (value.equal("ADD")) {
      //  m_tex_env = Gfx::ADD_TENV;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "polygonStipple") {
      if (!compare_to_true(value)) set_polygon_stipple_enable(true);
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "material") {
      Material* material = dynamic_cast<Material*>(cont); 
      if (material) {
        set_material(material);
        if (!get_back_material()) set_back_material(material);
      }
      elem->mark_delete(cai);
      continue;
    }
    if (name == "texture") {
      Texture* texture = dynamic_cast<Texture*>(cont);
      set_texture(texture);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "textureTransform") {
      //! todo textureTransform
      elem->mark_delete(cai);
      continue;
    }
    if (name == "halftone") {
      Halftone* halftone = dynamic_cast<Halftone*>(cont);
      set_polygon_stipple_enable(true);
      set_halftone(halftone);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Set the textute related modes:
  if (m_texture) set_default_texture_attributes();

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief sets default attributes for texture mapping. */
void Appearance::set_default_texture_attributes()
{
  set_tex_enable(true);
  Gfx::Tex_env tex_env = Gfx::MODULATE_TENV;
  Uint color_control = GL_SEPARATE_SPECULAR_COLOR;
  if (m_material) {
    const Vector3f& diffuse_color = m_material->get_diffuse_color();
    if ((diffuse_color[0] == 0) && (diffuse_color[1] == 0) &&
        (diffuse_color[2] == 0) && (m_material->get_ambient_intensity() == 0))
      tex_env = Gfx::DECAL_TENV;
    const Vector3f& specular_color = m_material->get_specular_color();
    if ((specular_color[0] == 0) && (specular_color[1] == 0) &&
        (specular_color[2] == 0))
      color_control = GL_SINGLE_COLOR;
  }
  set_tex_env(tex_env);

  //! \todo move to Gfx:
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, color_control);
}

/*! \brief cleans the node before drawing. */
void Appearance::clean()
{
  // Construct a default material
  if (!m_material) {
    m_material = new Material();
    m_own_material = true;
  }

  // Setup sphere environment map if requested:
  Sphere_environment* sphere_env =
    dynamic_cast<Sphere_environment*>(get_texture());
  if (sphere_env) {
    set_tex_enable(true);
    Tex_gen* tex_gen = new Tex_gen();
    m_own_tex_gen = true;
    tex_gen->set_modes(Tex_gen::SPHERE_MAP);
    tex_gen->set_modet(Tex_gen::SPHERE_MAP);
    set_tex_gen(tex_gen);
    set_tex_gen_enable(true);
  }
  
  m_dirty = false;
}

/*! \brief writes this container. */
void Appearance::write(Formatter* formatter)
{
  formatter->container_begin(get_tag());
  formatter->single_container_begin("material");
  formatter->write(m_material);
  formatter->single_container_end();  
  formatter->container_end();  
}

#if 0
/*! \brief */
Attribute_list Appearance::get_attributes() 
{ 
  Attribute_list attrs; 
  Attribue attrib;

  attrs = Container::get_attributes();

  if (m_poly_mode != m_def_poly_mode) {
    attrib.first = "polyFillMode";
    switch (get_poly_mode()) {
     case Gfx::LINE_PMODE: attrib.second = "line"; break;
     case Gfx::POINT_PMODE: attrib.second = "point"; break;
     case Gfx::FILL_PMODE: break;
    }
    attrs.push_back(attrib);
  }

  if (m_shade_model != m_def_shade_model)
  {
    attrib.first = "shadeModel";
    attrib.second = "flat";
    attrs.push_back(attrib);
  }

  if (m_tex_env != m_def_tex_env) {
    attrib.first = "texEnv";
    if (m_tex_env == Gfx::MODULATE_TENV) {
      attrib.second = "MODULATE";
    } else if (m_tex_env == Gfx::DECAL_TENV) {
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

/*! Add the appearance into the scene graph. 
 * There are three ways an appearance can be added.
 * -# the apperance is defined as a child of a Shape object. In this case
 * we set the apperance on the shape to point to this.
 * -# the apperance is defined as a chile of an Image_background object. In
 * this case we set the apperance on the Imagebackground.
 * -# the apperance is decalred at the top level of the scene graph. In this
 * case no object is assigned with this apperance.
 *
 * @param sg a pointer to the scene graph
 * @param parent a pointer to the parent object. NULL if the apperance
 *       is defined in the top level.
 */
void Appearance::add_to_scene(Scene_graph* sg, XML_entity* parent) 
{
  Container::add_to_scene(sg, parent);
  if (sg) sg->add_container(this);
  if (parent && (parent->get_name() == g_navigation_root_name)) return;

  // an apperance can appear either in a shape or in an image background...
  Shape* shape = dynamic_cast<Shape*>(parent);
  if (shape) {
    shape->set_appearance(this);
    return;
  } 
  
  Image_background* bg = dynamic_cast<Image_background*>(parent);
  if (bg) {
    bg->set_appearance(this);
    return;
  }
}
#endif

SGAL_END_NAMESPACE
