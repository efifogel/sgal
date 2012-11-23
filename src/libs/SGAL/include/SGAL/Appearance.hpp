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
// $Revision: 10977 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_APPEARANCE_HPP
#define SGAL_APPEARANCE_HPP

/*! \file
 * \brief The appearance container represents the appearance of a shape
 *              
 * Inherits from Container.
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Bit_mask.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

class Material;
class Texture;
class Tex_gen;
class Draw_action;
class Container_proto;
class Element;
class Formatter;
class Halftone;

/*! \brief A represenation of an appearance of a shape */
class SGAL_CLASSDEF Appearance : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    SGAL_STATE_ELEMENTS,
    LAST
  };

  /*! Constructor */
  Appearance(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Appearance();

  /*! Construct the prototype */
  static Appearance * prototype() { return new Appearance(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Appearance(); }

  void set(Appearance * app);

  /*! Set the texture attribute */
  void set_texture(Texture * texture);
  Texture* get_texture() const { return m_texture; }    

  /*! Set the halftone attribute */
  void set_halftone(Halftone* halftone);
  Halftone* get_halftone() const { return m_halftone; }    
  
  /*! Sets the texture-enable attribute */
  void set_tex_enable(Boolean tex_enable);
  Boolean get_tex_enable() const { return m_tex_enable; }

  /*! Set the texture mode attribute */
  void set_tex_mode(Gfx::Tex_mode tex_mode);
  Gfx::Tex_mode get_tex_mode() const { return m_tex_mode; }

  /*! Sets the texture blend-color attribute */
  void set_tex_blend_color(const Vector4f & tex_blend_color);
  void get_tex_blend_color(Vector4f & tex_blend_color) const
  {
    get_tex_blend_color(&tex_blend_color[0], &tex_blend_color[1],
                        &tex_blend_color[2], &tex_blend_color[3]);
  }

  void set_tex_blend_color(Float v0, Float v1, Float v2, Float v3);
  void get_tex_blend_color(Float * v0, Float * v1, Float * v2, Float * v3)
    const { m_tex_blend_color.get(v0, v1, v2, v3); }
  
  /*! Set the texture ??? */
  void set_tex_blend(Float * blend_color = NULL);

  /*! Set the texture environment attribute */
  void set_tex_env(Gfx::Tex_env tex_env);
  Gfx::Tex_env get_tex_env() const { return m_tex_env; }

  /*! Set the texture-generation attribute */
  void set_tex_gen(Tex_gen * tex_gen);
  Tex_gen * get_tex_gen() const { return m_tex_gen; }

  /*! Set the */
  void set_tex_gen_enable(Boolean tex_gen_enable);
  Boolean get_tex_gen_enable() const { return m_tex_gen_enable; }

  /*! Set the */
  void set_material(Material * material);

  /*! Set the */
  Material * get_material() const { return m_material; }

  /*! Set the */
  void set_back_material(Material * material);
  Material * get_back_material() const { return m_back_material; }

  /*! Set the */
  void set_light_enable(Boolean light_enable);
  Boolean get_light_enable() const { return m_light_enable; }

  /*! Set the */
  void set_shade_model(Gfx::Shade_model shade_model);
  Gfx::Shade_model get_shade_model() const { return m_shade_model; }

  /*! Set the */
  void set_transp_enable(Boolean transp_enable);
  Boolean get_transp_enable() const { return m_transp_enable; }

  /*! Set the */
  void set_transp_mode(Gfx::Transparency_mode transp_mode);
  Gfx::Transparency_mode get_transp_mode() const { return m_transp_mode; }

  /*! Set the */
  void set_alpha_func(Gfx::Alpha_func alpha_func);
  Gfx::Alpha_func get_alpha_func() const { return m_alpha_func; }

  /*! Set the */
  void set_alpha_ref(Float alpha_ref);
  Float get_alpha_ref() const { return m_alpha_ref; }

  /*! Set the */
  void set_blend_color(const Vector4f & blend_color);
  void get_blend_color(Vector4f & blend_color) const {
    get_blend_color(&blend_color[0], &blend_color[1],
                    &blend_color[2], &blend_color[3]);
  }

  void set_blend_color(Float v0, Float v1, Float v2, Float v3);
  void get_blend_color(Float * v0, Float * v1, Float * v2, Float * v3) const
  { m_blend_color.get(v0, v1, v2, v3); }

  /*! Set the */
  void set_src_blend_func(Gfx::Src_blend_func srcblend_func);
  Gfx::Src_blend_func get_src_blend_func() const { return m_src_blend_func; }

  /*! Set the */
  void set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func);
  Gfx::Dst_blend_func get_dst_blend_func() const { return m_dst_blend_func; }
  
  /*! Set the */
  void set_color_mask(const Vector4ub & color_mask);
  void get_color_mask(Vector4ub & color_mask) const {
    get_color_mask(&color_mask[0], &color_mask[1],
                   &color_mask[2], &color_mask[3]);
  }

  void set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3); 
  void get_color_mask(Ubyte * v0, Ubyte * v1, Ubyte * v2, Ubyte * v3) const
  { m_color_mask.get(v0, v1, v2, v3); }

  /*! Set the */
  void set_depth_enable(Boolean depth_enable);
  Boolean get_depth_enable() const { return m_depth_enable; }
  
  /*! Set the */
  void set_depth_func(Gfx::Depth_func depth_func);
  Gfx::Depth_func get_depth_func() const { return m_depth_func; }

  /*! Set the */
  void set_depth_mask(Boolean depth_mask);
  Boolean get_depth_mask() const { return m_depth_mask; }

  /*! Set the fog-enable flag */
  void set_fog_enable(Boolean fog_enable);

  /*! Obtain the fog-enable flag */
  Boolean get_fog_enable() const { return m_fog_enable; }

  /*! Set the polygon-stipple enable flag
   * \param enable indicates whether polygon stipple should be enabled
   */
  void set_polygon_stipple_enable(Boolean enable);

  /*! Obtain the polygon-stipple enable flag
   * \return a flag that indicates whether polygon stipple should be enabled
   */
  Boolean get_polygon_stipple_enable() const;
  
  /*! Set the material-mode enable flag */
  void set_material_mode_enable(Gfx::Material_mode material_mode_enable);

  /*! Obtain the material-mode enable flag */
  Gfx::Material_mode get_material_mode_enable() const
  { return m_material_mode_enable; }

  /*! Set the */
  void set_poly_mode(Gfx::Poly_mode poly_mode);
  Gfx::Poly_mode get_poly_mode() const { return m_poly_mode; }

  /*! Set the */
  void set_line_stipple_pattern(Uint pattern);
  Uint get_line_stipple_pattern() const { return m_line_stipple_pattern; }

  /*! Set the */
  void set_line_stipple_factor(Uint factor);
  Uint get_line_stipple_factor() const { return m_line_stipple_factor; }

  /*! Set the */
  void set_tex_transform(const Matrix4f & tex_transform);
  const Matrix4f & get_tex_transform() const { return m_tex_transform; }

  void set_inherit(const Bit_mask & inherit);
  void get_inherit(Bit_mask & inherit) const;

  virtual void draw(Draw_action * action);

  Boolean is_transparent() const;

  /*! Set the */
  void material_changed(Field_info * field_info);

  /*! Set the texture */
  void texture_changed(Field_info * field_info = NULL);

  /*! Set the halftone */
  void halftone_changed(Field_info * field_info = NULL);
  
  /*! Attache a given context */
  virtual Boolean attach_context(Context * context); 

  /*! Detache a given context */
  virtual Boolean detach_context(Context * context = 0); 

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto * get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);
    
  // virtual Attribute_list get_attributes();
  
  /*! Write this container */
  virtual void write(Formatter * formatter);

protected:
  SGAL_GFX_STRUCT;

  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Boolean m_dirty;
  
  Bit_mask m_dirty_flags;

  /*! Default value */
  static Gfx::Poly_mode m_def_poly_mode;
  static Gfx::Shade_model m_def_shade_model;
  static Gfx::Tex_env m_def_tex_env;

  // for each field, the corresponding bit is being set when the filed is 
  // set to a new value. it is then turned off after the drawing is performed
  Bit_mask m_pending;

  // for each field, the corresponding bit is being set when the filed is 
  // set to a new value. it remains on forever.
  Bit_mask m_override;
  Boolean m_skip_refer;

  //Gfx::Tex_env m_blend_func;

  /*! Indicates whether a material node has been newly created, (as the user
   * hasn't provided one,) and should be deleted when the node is destructed
   */
  Boolean m_is_default_material;

  /*! Indicates whether a tex_gen node has been newly created, (as the user
   * hasn't provided one,) and should be deleted when the node is destructed
   */
  Boolean m_is_default_tex_gen;

  /*! The default halftone pattern */
  static Ubyte s_def_halftone[];

  /*! Initialize the node upon construction */
  void init();

  /*! Clean the node before drawing */
  void clean();
  
  friend class Context;
};

/*! Obtain the polygon stipple enable flag */
inline Boolean Appearance::get_polygon_stipple_enable() const
{
  return m_polygon_stipple_enable;
}

SGAL_END_NAMESPACE

#endif
