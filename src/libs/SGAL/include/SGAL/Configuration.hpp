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
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONFIGURATION_HPP
#define SGAL_CONFIGURATION_HPP

/*! \file
 * A class to hold general scene related configurations.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

class Indexed_face_set;
class Container_proto;
class Element;
class Scene_graph;
class Accumulation;
class Multisample;

class SGAL_SGAL_DECL Configuration : public Container {
  friend class Indexed_face_set;
  friend class Shape;
  //dirty patch for accessing defauly flags 
  //(particullary - using display lists) from Indexed_face_set

public:
  enum {
    FIRST = Container::LAST - 1,
    POLY_MODE,
    DISPLAY_FPS,
    FIXED_HEADLIGHT,
    STENCIL_BITS,
    DEPTH_BITS,
    MIN_FRAME_RATE,
    MIN_ZOOM_DISTANCE,
    SPEED_FACTOR,
    GEOMETRY_DRAWING_MODE,
    TEXTURE_MAP,
    VERBOSITY_LEVEL,
    SEAMLESS_CUBE_MAP,
    OVERRIDE_MATERIAL,
    OVERRIDE_TEX_ENABLE,
    OVERRIDE_TEX_ENV,
    OVERRIDE_BLEND_FUNC,
    OVERRIDE_LIGHT_MODEL,
    OVERRIDE_TEX_GEN,
    OVERRIDE_LIGHT_ENABLE,
    LAST
  };

  /*! Mutually exclusive modes of drawing geometry */
  enum Geometry_drawing_mode {
    GDM_DIRECT = 0,
    GDM_DISPLAY_LIST,
    GDM_VERTEX_ARRAY
  };
  
  typedef boost::shared_ptr<Accumulation>               Shared_accumulation;
  typedef boost::shared_ptr<Multisample>                Shared_multisample;
  
  /*! Constructor */
  Configuration(Boolean proto = false);

  /*! Destructor */
  virtual ~Configuration() {};

  /*! Construct the prototype */
  static Configuration* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! */
  void set_global_lights_stationary(Boolean flag);

  /*! */
  Boolean are_global_lights_stationary();

  /*! */
  void set_fixed_head_light(Boolean flag);

  /*! */
  Boolean is_fixed_head_light();

  /*! Set the number of the desired stencil bits. */
  void set_number_of_stencil_bits(Uint bits);

  /*! Obtain the number of the desired stencil bits. */
  Uint get_number_of_stencil_bits() const;

  /*! Set the number of the desired depth bits. */
  void set_number_of_depth_bits(Uint bits);

  /*! Obtain the number of the desired depth bits. */
  Uint get_number_of_depth_bits() const;
  
  /*! */
  void set_min_frame_rate(Float rate);

  /*! */
  Float get_min_frame_rate() const;

  /*! */
  void set_min_zoom_distance(Float val);

  /*! */
  Float get_min_zoom_distance() const;

  /*! */
  void set_max_model_name(const std::string& name);

  /*! */
  std::string get_max_model_name() const;

  /*! Set the flag that indicates whether to display the number of frames per
   * second (fps).
   */
  void set_display_fps(Boolean flag);

  /*! Indicate whether the number of frames per second (fps) should be
   * displayed.
   */
  Boolean is_display_fps() const;

  /*! Set the polygon mode. */
  void set_poly_mode(Gfx::Poly_mode mode);
  
  /*! Obtain the polygon mode. */
  Gfx::Poly_mode get_poly_mode() const;

  /*! Determine whether texture-maping is enabled. */
  Boolean is_texture_map() const;

  /*! Set the texture-map flag. */
  void set_texture_map(Boolean flag);
  
  /*! Obtain the transformation speed scale. */
  Float get_speed_factor() const;

  /*! Set the geometry drawing mode. */
  void set_geometry_drawing_mode(Geometry_drawing_mode mode);

  /*! Obtain the geometry drawing mode. */
  Geometry_drawing_mode get_geometry_drawing_mode() const;

  /*! Set the accumulation object. */
  void set_accumulation(Shared_accumulation acc);
  
  /*! Obtain the accumulation object. */
  Shared_accumulation get_accumulation() const;

  /*! Set the multisample object. */
  void set_multisample(Shared_multisample ms);
  
  /*! Obtain the multisample object. */
  Shared_multisample get_multisample() const;

  /*! Set the verbosity level. */
  void set_verbosity_level(Uint level);

  /*! Obtain the verbosity level. */
  Uint get_verbosity_level() const;

  /*! Set the flag that indicates whether to apply filtering across faces of
   * the cubemap resulting in a seamless transition.
   */
  void set_seamless_cube_map(Boolean flag);

  /*! Detemine whether to apply filtering across faces of the cubemap
   * resulting in a seamless transition.
   */
  Boolean is_seamless_cube_map() const;
  
  /*! Set the flag that indicates whether to override the appearance
   * light-enable flag. If this flag is on, the (appearance) light is enabled
   * if the corresponding geometry does not have color.
   * Notice than When the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  void set_override_light_enable(Boolean flag);

  /*! Detemine whether to to override the appearance
   * light-enable flag. If this flag is on, the (appearance) light is enabled
   * if the corresponding geometry does not have color.
   * Notice than When the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean is_override_light_enable() const;
  
   /*! Set defualt values. */
  void reset(Geometry_drawing_mode def_geometry_drawing_mode =
               s_def_geometry_drawing_mode,
             Boolean def_are_global_lights_stationary =
               s_def_are_global_lights_stationary,
             Boolean def_texture_map = s_def_texture_map,
             Boolean def_is_fixed_head_Light =
               s_def_is_fixed_head_light,
             Uint def_stencil_bits = s_def_stencil_bits,
             Uint def_depth_bits = s_def_depth_bits,
             Float def_min_frame_Rate = s_def_min_frame_rate,
             Gfx::Poly_mode def_poly_mode = s_def_poly_mode,
             Boolean def_display_fps = s_def_display_fps,
             Float def_min_zoom_distance = s_def_min_zoom_distance,
             Float def_speed_factor = s_def_speed_factor,
             Boolean def_seamless_cube_map = s_def_seamless_cube_map);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Accumulation object. */
  Shared_accumulation m_accumulation;

  /*! Multisample object. */
  Shared_multisample m_multisample;
  
  /*! The geometry drawing-mode {direct, display list, or vertex array */
  Geometry_drawing_mode m_geometry_drawing_mode;

  Boolean m_are_global_lights_stationary;

  /*! Indicates whether to apply texture mapping. */
  Boolean m_texture_map;

  Boolean m_is_fixed_head_light;

  /* The desired stencil bits. */
  Uint m_stencil_bits;

  /* The desired depth bits. */
  Uint m_depth_bits;
  
  Float m_min_frame_rate;

  /*! The polygon mode. */
  Gfx::Poly_mode m_poly_mode;

  std::string m_poly_mode_str;

  /*! Indicates whether to display the Frame-Per-second. */
  Boolean m_display_fps;

  /*! */
  Float m_min_zoom_distance;

  /*! */
  std::string m_max_model_name;

  /*! */
  Float m_speed_factor;

  /*! The verbosity level. */
  Uint m_verbosity_level;

  /*! Detemines whether to apply filtering across faces of the cubemap
   * resulting in a seamless transition. This was a hardware limitation in
   * the past, but modern hardware is capable of interpolating across a
   * cube face boundary. 
   */
  Boolean m_seamless_cube_map;
  
  /*! Indicates whether to construct the appearance material attribute when
   * missing.
   * Notice that when the appearance chages, the material attribute of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_material;

  /*! Indicates whether to override the appearance texture enable flag. If
   * this flag is on, the (appearance) texture enable flag may be overriden.
   * Notice than when the appearance chages, the texture enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_tex_enable;

  /*! Indicates whether to override the appearance texture environment. If
   * this flag is on, the (appearance) texture environment may be overriden.
   * Notice than when the appearance chages, the texture environment of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_tex_env;

  /*! Indicates whether to override the appearance blend functions. If
   * this flag is on, the (appearance) blend functions may be overriden.
   * Notice than when the appearance chages, the blend functions of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_blend_func;

  /*! Indicates whether to override the appearance light_model. If
   * this flag is on, the (appearance) blend functions may be overriden.
   * Notice than when the appearance chages, the light model of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_light_model;

  /*! Indicates whether to override the appearance texture-generation flag and
   * construct the appearance texture-generation attribute when missing. If
   * this flag is on and the corresponding geometry does not generate
   * texture coordinates, the (appearance) texture-generation is enabled and
   * the (appearance) texture generation is created if missing.
   * Notice than when the appearance chages, the texture-generation flag (and
   * possibly attribute) of the previous appearance are not restored to their
   * original values.
   */
  Boolean m_override_tex_gen;

  /*! Indicates whether to override the appearance light-enable flag. If this
   * flag is on, the (appearance) light is enabled if the corresponding
   * geometry does not have color.
   * Notice than When the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_light_enable;

  /*! Indicates whether the accumulation is owned. If the accumulation is
   * owned, it is constructed and destructed by this construct.
   */
  Boolean m_owned_accumulation;
  
  // default values
  static const Geometry_drawing_mode s_def_geometry_drawing_mode;
  static const Boolean s_def_are_global_lights_stationary;
  static const Boolean s_def_texture_map;
  static const Boolean s_def_is_fixed_head_light;
  static const Uint s_def_stencil_bits;
  static const Uint s_def_depth_bits;
  static const Float s_def_min_frame_rate;
  static const Gfx::Poly_mode s_def_poly_mode;
  static const Boolean s_def_display_fps;
  static const Float s_def_min_zoom_distance;
  static const Float s_def_speed_factor;
  static const Uint s_def_verbose_level;
  static const Boolean s_def_seamless_cube_map;
  static const Boolean s_def_override_material;
  static const Boolean s_def_override_tex_enable;
  static const Boolean s_def_override_tex_env;
  static const Boolean s_def_override_blend_func;
  static const Boolean s_def_override_light_model;
  static const Boolean s_def_override_tex_gen;
  static const Boolean s_def_override_light_enable;
  
  static const Char* s_geometry_drawing_mode_names[];
};

/*! \brief lsConstruct the prototype. */
inline Configuration* Configuration::prototype()
{ return new Configuration(true); }

/*! \brief clones. */
inline Container* Configuration::clone() { return new Configuration(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Configuration::get_tag() const { return s_tag; }

/*! \brief */
inline void Configuration::set_global_lights_stationary(Boolean flag)
{ m_are_global_lights_stationary = flag; }

/*! \brief */
inline Boolean Configuration::are_global_lights_stationary ()
{ return m_are_global_lights_stationary; }

/*! \brief */
inline void Configuration::set_fixed_head_light(Boolean flag)
{ m_is_fixed_head_light = flag; }

/*! \brief */
inline Boolean Configuration::is_fixed_head_light()
{ return m_is_fixed_head_light; }

/*! \brief sets the number of the desired stencil bits. */
inline void Configuration::set_number_of_stencil_bits(Uint bits)
{ m_stencil_bits = bits; }

/*! \brief obtains the number of the desired stencil bits. */
inline Uint Configuration::get_number_of_stencil_bits() const
{ return m_stencil_bits; }

/*! \brief sets the number of the desired depth bits. */
inline void Configuration::set_number_of_depth_bits(Uint bits)
{ m_depth_bits = bits; }

/*! \brief obtains the number of the desired depth bits. */
inline Uint Configuration::get_number_of_depth_bits() const
{ return m_depth_bits; }
  
/*! \brief */
inline void Configuration::set_min_frame_rate(Float rate)
{ m_min_frame_rate = rate; }

/*! \brief */
inline Float Configuration::get_min_frame_rate() const
{ return m_min_frame_rate; }

/*! \brief */
inline void Configuration::set_min_zoom_distance(Float val)
{ m_min_zoom_distance = val; }

/*! \brief */
inline Float Configuration::get_min_zoom_distance() const
{ return m_min_zoom_distance; }

/*! \brief */
inline void Configuration::set_max_model_name(const std::string& name)
{ m_max_model_name = name; }

/*! \brief */
inline std::string Configuration::get_max_model_name() const
{ return m_max_model_name; }

/*! \brief sets the flag that indicates whether to display the number of
 * frames per second (fps).
 */
inline void Configuration::set_display_fps(Boolean flag)
{ m_display_fps = flag; }

/*! \brief indicates whether the number of frames per second (fps) should be
 * displayed.
 */
inline Boolean Configuration::is_display_fps() const { return m_display_fps; }

/*! \brief sets the polygon mode. */
inline void Configuration::set_poly_mode(Gfx::Poly_mode mode)
{ m_poly_mode = mode; }
  
/*! \brief obtains the polygon mode. */
inline Gfx::Poly_mode Configuration::get_poly_mode () const
{ return m_poly_mode; }

/*! \brief determines whether texture-maping is enabled. */
inline Boolean Configuration::is_texture_map() const { return m_texture_map; }

/*! \brief sets the texture-map flag. */
inline void Configuration::set_texture_map(Boolean flag)
{ m_texture_map = flag; }
  
/*! \brief obtains the transformation speed scale. */
inline Float Configuration::get_speed_factor() const {return m_speed_factor; }

/*! \brief sets the flag that indicates the drawing mode. */
inline void
Configuration::set_geometry_drawing_mode(Geometry_drawing_mode mode)
{ m_geometry_drawing_mode = mode; }

/*! \brief obtains the flag that indicates the drawing mode. */
inline Configuration::Geometry_drawing_mode
Configuration::get_geometry_drawing_mode() const
{ return m_geometry_drawing_mode; }

/*! \brief sets the accumulation object. */
inline void Configuration::set_accumulation(Shared_accumulation acc)
{ m_accumulation = acc; }
  
/*! \brief obtains the accumulation object. */
inline Configuration::Shared_accumulation Configuration::get_accumulation()
  const
{ return m_accumulation; }

/*! \brief sets the multisample object. */
inline void Configuration::set_multisample(Shared_multisample ms)
{ m_multisample = ms; }
  
/*! \brief obtains the multisample object. */
inline Configuration::Shared_multisample Configuration::get_multisample() const
{ return m_multisample; }

/*! \brief obtains the verbosity level. */
inline Uint Configuration::get_verbosity_level() const
{ return m_verbosity_level; }

/*! \brief sets the flag that indicates whether to apply filtering across
 * faces of the cubemap resulting in a seamless transition.
 */
inline void Configuration::set_seamless_cube_map(Boolean flag)
{ m_seamless_cube_map = flag; }

/*! \brief detemines whether to apply filtering across faces of the cubemap
 * resulting in a seamless transition.
 */
inline Boolean Configuration::is_seamless_cube_map() const
{ return m_seamless_cube_map; }
  
SGAL_END_NAMESPACE

#endif
