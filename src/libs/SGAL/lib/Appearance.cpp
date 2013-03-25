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
// $Id: $
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
#include "SGAL/Tex_gen.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Texture_2d.hpp"

SGAL_BEGIN_NAMESPACE

std::string Appearance::s_tag = "sgalAppearance";
Container_proto* Appearance::s_prototype = NULL;

// Default values:
const Gfx::Light_model_color_control
  Appearance::s_def_light_model_color_control(Gfx::SINGLE_COLOR);
const Gfx::Poly_mode Appearance::s_def_poly_mode(Gfx::FILL_PMODE);
const Gfx::Shade_model Appearance::s_def_shade_model(Gfx::SMOOTH_SHADE);
const Gfx::Tex_env Appearance::s_def_tex_env(Gfx::MODULATE_TENV);

REGISTER_TO_FACTORY(Appearance, "Appearance");

/*! The parameter-less constructor */
Appearance::Appearance(Boolean proto) :
  Container(proto),
  m_owned_material(false),
  m_owned_tex_gen(false),
  m_material_prev(NULL),
  m_tex_gen_prev(NULL)
{ init(); }

/*! Destructor */
Appearance::~Appearance()
{
  // delete the owned material attribute if present
  if (m_owned_material) {
    SGAL_assertion(m_material_prev);
    delete m_material_prev;
    m_material_prev = NULL;
    m_owned_material = false;
  }

  // delete the owned texture-generation attribute if present
  if (m_owned_tex_gen) {
    SGAL_assertion(m_tex_gen_prev);
    delete m_tex_gen_prev;
    m_tex_gen_prev = NULL;
    m_owned_tex_gen = false;
  }
}

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

/*! \brief */
void Appearance::init()
{
  m_texture                   = 0;
  m_tex_enable                = false;
  m_tex_mode                  = Gfx::FAST_TEX;
  m_tex_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_tex_env                   = Gfx::MODULATE_TENV;
  m_tex_gen                   = 0;
  m_tex_gen_enable            = false;
  m_material                  = 0;
  m_material_mode_enable      = Gfx::NO_COLOR_MATERIAL;
  m_light_enable              = 0;
  m_shade_model               = Gfx::SMOOTH_SHADE;
  m_transp_enable             = false;
  m_transp_mode               = Gfx::BLEND_TRANSP;
  m_alpha_func                = Gfx::ALWAYS_AFUNC;
  m_alpha_ref                 = 0.0f;
  m_blend_color.set(0.0f, 0.0f, 0.0f, 0.0f);
  m_src_blend_func            = Gfx::ONE_SBLEND;
  m_dst_blend_func            = Gfx::ZERO_DBLEND;
  m_color_mask.set(0xff, 0xff, 0xff, 0xff);
  m_depth_enable              = false;
  m_depth_func                = Gfx::LESS_DFUNC;
  m_depth_mask                = true;
  m_fog_enable                = false;
  m_poly_mode                 = s_def_poly_mode;
  m_line_stipple_factor       = 1;
  m_line_stipple_pattern      = 0xffff;
  m_tex_transform.make_identity();
  m_back_material             = 0;
  m_polygon_stipple_enable    = false;
  m_halftone                  = 0;
  m_light_model_color_control = s_def_light_model_color_control;

  m_pending.off();
  m_override.off();
}

/*! \brief */
void Appearance::set_texture(Texture* texture)
{
  m_texture = texture;
  m_pending.on_bit(Gfx::TEXTURE);
  m_override.on_bit(Gfx::TEXTURE);

  // this is to indicate that the texture has changed and 
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
}

/*! \brief */
void Appearance::set_halftone(Halftone* halftone)
{
  m_halftone = halftone;
  m_pending.on_bit(Gfx::HALFTONE_PATTERN);
  m_override.on_bit(Gfx::HALFTONE_PATTERN);

  m_dirty_flags.on_bit(Gfx::POLYGON_STIPPLE_ENABLE);
}

/*! \brief */
void Appearance::set_tex_enable(Boolean tex_enable)
{
  m_pending.on_bit(Gfx::TEX_ENABLE);
  m_override.on_bit(Gfx::TEX_ENABLE);
  m_tex_enable = tex_enable;
}

/*! \brief */
void Appearance::set_tex_mode(Gfx::Tex_mode tex_mode)
{
  m_pending.on_bit(Gfx::TEX_MODE);
  m_override.on_bit(Gfx::TEX_MODE);
  m_tex_mode = tex_mode;
}

/*! \brief */
void Appearance::set_tex_blend_color(const Vector4f& tex_blend_color)
{
  set_tex_blend_color(tex_blend_color[0], tex_blend_color[1],
                      tex_blend_color[2], tex_blend_color[3]);
}

/*! \brief */
void Appearance::set_tex_blend_color(Float v0, Float v1, Float v2, Float v3)
{
  m_pending.on_bit(Gfx::TEX_BLEND_COLOR);
  m_override.on_bit(Gfx::TEX_BLEND_COLOR);
  m_tex_blend_color.set(v0, v1, v2, v3);
}

/*! \brief sets the texture environment attribute. */
void Appearance::set_tex_env(Gfx::Tex_env tex_env)
{
  m_pending.on_bit(Gfx::TEX_ENV);
  m_override.on_bit(Gfx::TEX_ENV);
  m_tex_env = tex_env;
}

/*! \brief sets the texture-generation attribute. */
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

/*! \brief sets the material attribute. */
void Appearance::set_material(Material* material)
{
  SGAL_assertion(material);
  m_material = material;

  if (material->get_transparency() != 0.0f) {
    set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  }
  
  m_pending.on_bit(Gfx::MATERIAL);
  m_override.on_bit(Gfx::MATERIAL);
}

/*! \brief processes change of material. */
void Appearance::material_changed(Field_info* /* field_info */)
{
  //! \todo what if material is set?
  if (m_material->get_transparency() != 0.0f) {
    set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  }

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

/*! \brief sets the polygon-stipple enable flag. */
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

/*! \brief sets the texture transformation. */
void Appearance::set_tex_transform(const Matrix4f& tex_transform)
{
  m_pending.on_bit(Gfx::TEX_TRANSFORM);
  m_override.on_bit(Gfx::TEX_TRANSFORM);
  m_tex_transform = tex_transform;
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

/*! \brief */
void Appearance::set_inherit(const Bit_mask& inherit) { m_pending = inherit; }

/*! \brief */
void Appearance::get_inherit(Bit_mask& inherit) const { inherit = m_pending; }

/*! \brief applies the appearance. */
void Appearance::draw(Draw_action* action)
{
  Context* context = action->get_context();
  if (context == NULL) return;
  context->draw_app(this);
}

/*! \brief determines whether the appearance is translucent. */
Boolean Appearance::is_transparent() const
{
  return ((m_src_blend_func != Gfx::ONE_SBLEND) ||
          (m_dst_blend_func != Gfx::ZERO_DBLEND));
}

/*! \brief notifies that the texture has been changed. */
void Appearance::texture_changed(Field_info* /* field_info */)
{
  m_pending.on_bit(Gfx::TEXTURE);
  m_override.on_bit(Gfx::TEXTURE);

  // this is to indicate that the texture has changed and 
  // the tex blend func has to be re-evaluated
  m_dirty_flags.on_bit(Gfx::TEX_ENV);
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
  Texture* texture = get_texture();
  if (texture) result &= texture->attach_context(context);
  return result;
}

/*! \brief */
Boolean Appearance::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
  Texture* texture = get_texture();
  if (texture) result &= texture->detach_context(context);
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
      if (value.compare("MODULATE") == 0)
        set_tex_env(Gfx::MODULATE_TENV);
      else if (value.compare("DECAL") == 0)
        set_tex_env(Gfx::DECAL_TENV);
      //} else if (value.equal("BLEND")) {
      //  m_tex_env = Gfx::BLEND_TENV;
      //} else if (value.equal("REPLACE")) {
      //  m_tex_env = Gfx::REPLACE_TENV;
      //} else if (value.equal("ADD")) {
      //  m_tex_env = Gfx::ADD_TENV;
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
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
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

  // Remove all the deleted attributes:
  elem->delete_marked();
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

/*! \brief cleans the texture enable flag. */
void Appearance::clean_tex_enable()
{ if (m_texture && !m_texture->empty()) set_tex_enable(true); }

/*! \brief cleans the texture environment attribute. */
void Appearance::clean_tex_env()
{
  if (!m_tex_enable) return;

  SGAL_assertion(m_texture);
  SGAL_assertion(m_material);
  Uint num_compnents = m_texture->get_component_count();
  const Vector3f& diffuse_color = m_material->get_diffuse_color();
  if (((diffuse_color[0] == 0) && (diffuse_color[1] == 0) &&
       (diffuse_color[2] == 0) && (m_material->get_ambient_intensity() == 0)) ||
      num_compnents == 4)
    set_tex_env(Gfx::DECAL_TENV);
}

/*! \brief cleans the blend functions. */
void Appearance::clean_blend_func()
{
  // If texture is enabled and texture is either 2 components or 4 components,
  // and the texture environment is either MODULATE, BLEND, or ADD, override
  // the blend functions.
  if (!m_tex_enable) return;

  SGAL_assertion(m_texture);
  Uint num_compnents = m_texture->get_component_count();
  if ((num_compnents == 2) || (num_compnents == 4)) {
    if ((m_tex_env == Gfx::BLEND_TENV) || (m_tex_env == Gfx::MODULATE_TENV)) {
      set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
      set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    }
    else if (m_tex_env == Gfx::ADD_TENV) {
      set_src_blend_func(Gfx::ONE_SBLEND);
      set_dst_blend_func(Gfx::ONE_DBLEND);
    }
  }
}

/*! \brief cleans the light model. */
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

/*! \brief cleans the material attribute. */
void Appearance::clean_material()
{
  // Construct a new owned texture generation attribute if needed, and delete
  // the previously constructed owned texture generation attribute if not
  // needed any more.
  if (m_owned_material) {
    SGAL_assertion(m_material_prev);
    if (!m_material) set_material(m_material_prev);
    else if (m_material != m_material_prev) {
      delete m_material_prev;
      m_material_prev = NULL;
      m_owned_material = false;
    }
  }
  else {
    if (!m_material) {
      Material* material = new Material();
      SGAL_assertion(material);
      set_material(material);
      m_owned_material = true;
    }
  }
}

/*! \brief cleans the texture generation attribute. */
void Appearance::clean_tex_gen()
{
  // Construct a new owned texture generation attribute if needed, and delete
  // the previously constructed owned texture generation attribute if not
  // needed any more.
  set_tex_gen_enable(true);
  if (m_owned_tex_gen) {
    SGAL_assertion(m_tex_gen_prev);
    if (!m_tex_gen) set_tex_gen(m_tex_gen_prev);
    else if (m_tex_gen != m_tex_gen_prev) {
      delete m_tex_gen_prev;
      m_tex_gen_prev = NULL;
      m_owned_tex_gen = false;
    }
  }
  else {
    if (!m_tex_gen) {
      Tex_gen* tex_gen = new Tex_gen();
      SGAL_assertion(tex_gen);
      set_tex_gen(tex_gen);
      m_owned_tex_gen = true;
    }
  }

  // Setup the textute-generation functions.
  Texture* texture = get_texture();
  if (dynamic_cast<Texture_2d*>(texture)) {
    // Setup standard texture map if requested:
#if 0
    get_tex_gen()->set_mode_s(Tex_gen::EYE_LINEAR);
    get_tex_gen()->set_mode_t(Tex_gen::EYE_LINEAR);
#else
    get_tex_gen()->set_mode_s(Tex_gen::OBJECT_LINEAR);
    get_tex_gen()->set_mode_t(Tex_gen::OBJECT_LINEAR);
#endif
  }
  else if (dynamic_cast<Sphere_environment*>(texture)) {
    // Setup sphere environment map if requested:
    get_tex_gen()->set_mode_s(Tex_gen::SPHERE_MAP);
    get_tex_gen()->set_mode_t(Tex_gen::SPHERE_MAP);
  }
  else if (dynamic_cast<Cube_environment*>(texture)) {
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

SGAL_END_NAMESPACE
