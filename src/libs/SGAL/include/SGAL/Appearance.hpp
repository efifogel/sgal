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
  Appearance(Boolean proto = false);

  /*! Destructor */
  virtual ~Appearance();

  /*! Construct the prototype. */
  static Appearance* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the appearance with the content of another appearance. */
  void set(Appearance* app);

  /*! Set the texture attribute. */
  void set_texture(Texture* texture);

  /*! Obtain the texture attribute. */
  Texture* get_texture() const;   

  /*! Set the halftone attribute. */
  void set_halftone(Halftone* halftone);

  /*! Obtain the halftone attribute. */
  Halftone* get_halftone() const;
  
  /*! Set the texture-enable attribute. */
  void set_tex_enable(Boolean tex_enable);

  /*! Obtain the texture-enable attribute. */
  Boolean get_tex_enable() const;

  /*! Set the texture mode attribute. */
  void set_tex_mode(Gfx::Tex_mode tex_mode);

  /*! Obtain the texture mode attribute. */
  Gfx::Tex_mode get_tex_mode() const;

  /*! Set the texture blend-color attribute. */
  void set_tex_blend_color(const Vector4f& tex_blend_color);

  /*! Obtain the texture blend-color attribute. */
  void get_tex_blend_color(Vector4f& tex_blend_color) const;

  /*! Set the texture blend-color attribute. */
  void set_tex_blend_color(Float v0, Float v1, Float v2, Float v3);

  /*! Obtain the texture blend-color attribute. */
  void get_tex_blend_color(Float* v0, Float* v1, Float* v2, Float* v3) const;
  
  /*! Set the texture blend-color attribute. */
  void set_tex_blend(Float* blend_color = NULL);

  /*! Set the texture environment attribute. */
  void set_tex_env(Gfx::Tex_env tex_env);

  /*! Obtain the texture environment attribute. */
  Gfx::Tex_env get_tex_env() const;

  /*! Set the texture-generation attribute. */
  void set_tex_gen(Tex_gen* tex_gen);

  /*! Obtain the texture-generation attribute. */
  Tex_gen* get_tex_gen() const;

  /*! Set the texture-generation enable flag. */
  void set_tex_gen_enable(Boolean tex_gen_enable);

  /*! Obtain the the texture-generation enable flag. */
  Boolean get_tex_gen_enable() const;

  /*! Set the material attribute. */
  void set_material(Material* material);

  /*! Obtain the material attribute. */
  Material* get_material() const;

  /*! Set the back material attribute. */
  void set_back_material(Material* material);

  /*! Obtain the back material attribute. */
  Material* get_back_material() const;

  /*! Set the light enable flag. */
  void set_light_enable(Boolean light_enable);

  /*! Obtain the light enable flag. */
  Boolean get_light_enable() const;

  /*! Set the shade model attribute. */
  void set_shade_model(Gfx::Shade_model shade_model);

  /*! Obtain the shade model attribute. */
  Gfx::Shade_model get_shade_model() const;

  /*! Set the translucency enable flag. */
  void set_transp_enable(Boolean transp_enable);

  /*! Obtain the translucency enable flag. */
  Boolean get_transp_enable() const;

  /*! Set the translucency mode attribute. */
  void set_transp_mode(Gfx::Transparency_mode transp_mode);

  /*! Obtain the translucency mode attribute. */
  Gfx::Transparency_mode get_transp_mode() const;

  /*! Set the alpha function attribute. */
  void set_alpha_func(Gfx::Alpha_func alpha_func);

  /*! Obtain the alpha function attribute. */
  Gfx::Alpha_func get_alpha_func() const;

  /*! Set the alpha reference attribute. */
  void set_alpha_ref(Float alpha_ref);

  /*! Obtain the alpha reference attribute. */
  Float get_alpha_ref() const;

  /*! Set the blend color attribute. */
  void set_blend_color(const Vector4f& blend_color);

  /*! Obtain the blend color attribute. */
  void get_blend_color(Vector4f& blend_color) const;

  /*! Set the blend color attribute. */
  void set_blend_color(Float v0, Float v1, Float v2, Float v3);

  /*! Obtain the blend color attribute. */
  void get_blend_color(Float* v0, Float* v1, Float* v2, Float* v3) const;

  /*! Set the source blend function attribute. */
  void set_src_blend_func(Gfx::Src_blend_func srcblend_func);

  /*! Obtain the source blend function attribute. */
  Gfx::Src_blend_func get_src_blend_func() const;

  /*! Set the destination blend function attribute. */
  void set_dst_blend_func(Gfx::Dst_blend_func dst_blend_func);

  /*! Obtain the destination blend function attribute. */
  Gfx::Dst_blend_func get_dst_blend_func() const;
  
  /*! Set the color mask attribute. */
  void set_color_mask(const Vector4ub& color_mask);

  /*! Obtain the color mask attribute. */
  void get_color_mask(Vector4ub& color_mask) const;

  /*! Set the color mask attribute. */
  void set_color_mask(Ubyte v0, Ubyte v1, Ubyte v2, Ubyte v3); 

  /*! Obtain the color mask attribute. */
  void get_color_mask(Ubyte* v0, Ubyte* v1, Ubyte* v2, Ubyte* v3) const;

  /*! Set the depth enable flag. */
  void set_depth_enable(Boolean depth_enable);

  /*! Obtain the depth enable flag. */
  Boolean get_depth_enable() const;
  
  /*! Set the depth function attribute. */
  void set_depth_func(Gfx::Depth_func depth_func);

  /*! Obtain the depth function attribute. */
  Gfx::Depth_func get_depth_func() const;

  /*! Set the depth mask attribute. */
  void set_depth_mask(Boolean depth_mask);

  /*! Obtain the depth mask attribute. */
  Boolean get_depth_mask() const;

  /*! Set the fog-enable flag. */
  void set_fog_enable(Boolean fog_enable);

  /*! Obtain the fog-enable flag. */
  Boolean get_fog_enable() const;

  /*! Set the polygon-stipple enable flag.
   * \param enable indicates whether polygon stipple should be enabled
   */
  void set_polygon_stipple_enable(Boolean enable);

  /*! Obtain the polygon-stipple enable flag.
   * \return a flag that indicates whether polygon stipple should be enabled
   */
  Boolean get_polygon_stipple_enable() const;
  
  /*! Set the material-mode enable flag. */
  void set_material_mode_enable(Gfx::Material_mode material_mode_enable);

  /*! Obtain the material-mode enable flag. */
  Gfx::Material_mode get_material_mode_enable() const;

  /*! Set the polygon mode attribute. */
  void set_poly_mode(Gfx::Poly_mode poly_mode);

  /*! Obtain the polygon mode attribute. */
  Gfx::Poly_mode get_poly_mode() const;

  /*! Set the line stipple pattern attribute. */
  void set_line_stipple_pattern(Uint pattern);

  /*! Obtain the line stipple pattern attribute. */
  Uint get_line_stipple_pattern() const;

  /*! Set the line stipple factor attribute. */
  void set_line_stipple_factor(Uint factor);

  /*! Obtain the line stipple factor attribute. */
  Uint get_line_stipple_factor() const;

  /*! Set the texture transformation attribute. */
  void set_tex_transform(const Matrix4f& tex_transform);

  /*! Obtain the texture transformation attribute. */
  const Matrix4f& get_tex_transform() const;

  /*! Set the inheritance mask. */
  void set_inherit(const Bit_mask& inherit);

  /*! Obtain the inheritance mask. */
  void get_inherit(Bit_mask& inherit) const;

  /*! Set the state specified by the appearance. */
  virtual void draw(Draw_action* action);

  /*! Determine whether appearance is translucent. */
  Boolean is_transparent() const;

  /*! Notify that the material has changed. */
  void material_changed(Field_info* field_info);

  /*! Notify that the texture has changed. */
  void texture_changed(Field_info* field_info = NULL);

  /*! Notify that the halftone has changed. */
  void halftone_changed(Field_info* field_info = NULL);
  
  /*! Attache a given context. */
  virtual Boolean attach_context(Context* context); 

  /*! Detache a given context. */
  virtual Boolean detach_context(Context* context = 0); 

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);
    
  // virtual Attribute_list get_attributes();
  
  /*! Write this container. */
  virtual void write(Formatter* formatter);

  /*! Set default attributes for texture mapping. */
  void set_default_texture_attributes();
  
protected:
  SGAL_GFX_STRUCT;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

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

/*! \brief constructs the prototype. */
inline Appearance* Appearance::prototype() { return new Appearance(true); }

/*! \brief clones. */
inline Container* Appearance::clone() { return new Appearance(); }

/*! \brief obtains the texture attribute. */
inline Texture* Appearance::get_texture() const { return m_texture; }    

/*! \brief obtains the halftone attribute. */
inline Halftone* Appearance::get_halftone() const { return m_halftone; }    
  
/*! \brief obtains the texture-enable attribute. */
inline Boolean Appearance::get_tex_enable() const { return m_tex_enable; }

/*! \brief obtains the texture mode attribute. */
inline Gfx::Tex_mode Appearance::get_tex_mode() const { return m_tex_mode; }

/*! \brief obtains the texture blend-color attribute. */
inline void Appearance::get_tex_blend_color(Vector4f& tex_blend_color) const
{
  get_tex_blend_color(&tex_blend_color[0], &tex_blend_color[1],
                      &tex_blend_color[2], &tex_blend_color[3]);
}

/*! \brief obtains the texture blend-color attribute. */
inline void Appearance::get_tex_blend_color(Float* v0, Float* v1,
                                            Float* v2, Float* v3) const
{ m_tex_blend_color.get(v0, v1, v2, v3); }

/*! \brief obtains the texture environment attribute. */
inline Gfx::Tex_env Appearance::get_tex_env() const { return m_tex_env; }

/*! \brief obtains the texture-generation attribute. */
inline Tex_gen* Appearance::get_tex_gen() const { return m_tex_gen; }

/*! \brief obtains  the texture-generation enable flag. */
inline Boolean Appearance::get_tex_gen_enable() const
{ return m_tex_gen_enable; }

/*! \brief obtains the material attribute. */
inline Material* Appearance::get_material() const { return m_material; }

/*! \brief obtains the back material attribute. */
inline Material* Appearance::get_back_material() const
{ return m_back_material; }

/*! \brief obtains the light enable flag. */
inline Boolean Appearance::get_light_enable() const { return m_light_enable; }

/*! \brief obtains the shade model attribute. */
inline Gfx::Shade_model Appearance::get_shade_model() const
{ return m_shade_model; }

/*! \brief obtains the translucency enable flag. */
inline Boolean Appearance::get_transp_enable() const { return m_transp_enable; }

/*! \brief obtains the translucency mode attribute. */
inline Gfx::Transparency_mode Appearance::get_transp_mode() const
{ return m_transp_mode; }

/*! \brief obtains the alpha function attribute. */
inline Gfx::Alpha_func Appearance::get_alpha_func() const
{ return m_alpha_func; }

/*! \brief obtains the alpha reference attribute. */
inline Float Appearance::get_alpha_ref() const { return m_alpha_ref; }

/*! \brief obtains the blend color attribute. */
inline void Appearance::get_blend_color(Vector4f& blend_color) const
{
  get_blend_color(&blend_color[0], &blend_color[1],
                  &blend_color[2], &blend_color[3]);
}

/*! \brief obtains the blend color attribute. */
inline void Appearance::get_blend_color(Float* v0, Float* v1,
                                        Float* v2, Float* v3) const
{ m_blend_color.get(v0, v1, v2, v3); }

/*! \brief obtains the source blend function attribute. */
inline Gfx::Src_blend_func Appearance::get_src_blend_func() const
{ return m_src_blend_func; }

/*! \brief obtains the destination blend function attribute. */
inline Gfx::Dst_blend_func Appearance::get_dst_blend_func() const
{ return m_dst_blend_func; }
  
/*! \brief obtains the color mask attribute. */
inline void Appearance::get_color_mask(Vector4ub& color_mask) const
{
  get_color_mask(&color_mask[0], &color_mask[1],
                 &color_mask[2], &color_mask[3]);
}

/*! \brief obtains the color mask attribute. */
inline void Appearance::get_color_mask(Ubyte* v0, Ubyte* v1,
                                       Ubyte* v2, Ubyte* v3) const
{ m_color_mask.get(v0, v1, v2, v3); }

/*! \brief obtains the depth enable flag. */
inline Boolean Appearance::get_depth_enable() const { return m_depth_enable; }
  
/*! \brief obtains the depth function attribute. */
inline Gfx::Depth_func Appearance::get_depth_func() const
{ return m_depth_func; }

/*! \brief obtains the depth mask attribute. */
inline Boolean Appearance::get_depth_mask() const { return m_depth_mask; }

/*! \brief obtains the fog-enable flag. */
inline Boolean Appearance::get_fog_enable() const { return m_fog_enable; }

/*! \brief obtains the polygon stipple enable flag. */
inline Boolean Appearance::get_polygon_stipple_enable() const
{ return m_polygon_stipple_enable; }

/*! \brief obtains the material-mode enable flag. */
inline Gfx::Material_mode Appearance::get_material_mode_enable() const
{ return m_material_mode_enable; }

/*! \brief obtains the polygon mode attribute. */
inline Gfx::Poly_mode Appearance::get_poly_mode() const { return m_poly_mode; }

/*! \brief obtains the line stipple pattern attribute. */
inline Uint Appearance::get_line_stipple_pattern() const
{ return m_line_stipple_pattern; }

/*! \brief obtains the line stipple factor attribute. */
inline Uint Appearance::get_line_stipple_factor() const
{ return m_line_stipple_factor; }

/*! \brief obtains the texture transformation attribute. */
inline const Matrix4f& Appearance::get_tex_transform() const
{ return m_tex_transform; }

/*! \brief obtain the tag (type) of the container */
inline const std::string& Appearance::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
