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

class Configuration : public Container {
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
    MIN_FRAME_RATE,
    MIN_ZOOM_DISTANCE,
    SPEED_FACTOR,
    GEOMETRY_DRAWING_MODE,
    TEXTURE_MAP,
    VERBOSITY_LEVEL,
    SEAMLESS_CUBE_MAP,
    OVERRIDE_LIGHT_ENABLE,
    LAST
  };

  /*! Mutually exclusive modes of drawing geometry */
  enum Geometry_drawing_mode {
    GDM_DIRECT = 0,
    GDM_DISPLAY_LIST,
    GDM_VERTEX_ARRAY
  };
  
  /*! Constructor */
  Configuration(Boolean proto = false);

  /*! Destructor */
  virtual ~Configuration() {};

  /*! Construct the prototype */
  static Configuration* prototype();

  /*! Clone */
  virtual Container* clone();

  void set_global_lights_stationary(Boolean flag)
  { m_are_global_lights_stationary = flag; }

  Boolean are_global_lights_stationary ()
  { return m_are_global_lights_stationary; }

  void set_fixed_head_light( Boolean flag ) { m_is_fixed_head_light = flag; }

  Boolean is_fixed_head_light() { return m_is_fixed_head_light; }

  void set_min_frame_rate(float rate) { m_min_frame_rate = rate; }

  Float get_min_frame_rate() const { return m_min_frame_rate; }

  void set_min_zoom_distance(Float val) { m_min_zoom_distance = val; }

  Float get_min_zoom_distance() const { return m_min_zoom_distance; }

  void set_max_model_name(const std::string & name)
  { m_max_model_name = name; }

  std::string get_max_model_name() const { return m_max_model_name; }

  /*! Set the flag that indicates whether to display the number of frames per
   * second (fps).
   */
  void set_display_fps(Boolean flag) { m_display_fps = flag; }

  /*! Indicates whether the number of frames per second (fps) should be
   * displayed.
   */
  Boolean is_display_fps() const { return m_display_fps; }

  /*! Set the polygon mode. */
  void set_poly_mode(Gfx::Poly_mode mode) { m_poly_mode = mode; }
  
  /*! Obtain the polygon mode. */
  Gfx::Poly_mode get_poly_mode () const { return m_poly_mode; }

  /*! Is texture-map enabled? */
  Boolean is_texture_map() const { return m_texture_map; }

  /*! Set the texture-map flag. */
  void set_texture_map(Boolean flag) { m_texture_map = flag; }
  
  /*! Obtain the transformation speed scale. */
  Float get_speed_factor() const {return m_speed_factor; }

  /*! Set the geometry drawing mode. */
  void set_geometry_drawing_mode(Geometry_drawing_mode mode);

  /*! Obtain the geometry drawing mode. */
  Geometry_drawing_mode get_geometry_drawing_mode() const;

  /*! Set the accumulation object. */
  void set_accumulation(Accumulation* acc) { m_accumulation = acc; }
  
  /*! Obtain the accumulation object. */
  Accumulation* get_accumulation() const { return m_accumulation; }

  /*! Set the multisample object. */
  void set_multisample(Multisample* ms) { m_multisample = ms; }
  
  /*! Obtain the multisample object. */
  Multisample* get_multisample() const { return m_multisample; }

  /*! Set the verbosity level. */
  void set_verbosity_level(Uint level);

  /*! Obtain the verbosity level. */
  Uint get_verbosity_level() const { return m_verbosity_level; }

  /*! Set the flag that indicates whether to apply filtering across faces of
   * the cubemap resulting in a seamless transition.
   */
  void set_seamless_cube_map(Boolean flag) { m_seamless_cube_map = flag; }

  /*! Detemine whether to apply filtering across faces of the cubemap
   * resulting in a seamless transition.
   */
  Boolean is_seamless_cube_map() const { return m_seamless_cube_map; }
  
  /*! Set the flag that indicates whether to override the appearance
   * light-enable flag. If this flag is on, the (appearance) light is enabled
   * if the corresponding geometry does not have color.
   * Notice than When the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  void set_override_light_enable(Boolean flag)
  { m_override_light_enable = flag; }

  /*! Detemine whether to to override the appearance
   * light-enable flag. If this flag is on, the (appearance) light is enabled
   * if the corresponding geometry does not have color.
   * Notice than When the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean is_override_light_enable() const { return m_override_light_enable; }
  
   /*! Set defualt values. */
  void reset(Geometry_drawing_mode def_geometry_drawing_mode =
               s_def_geometry_drawing_mode,
             Boolean def_are_global_lights_stationary =
               s_def_are_global_lights_stationary,
             Boolean def_texture_map = s_def_texture_map,
             Boolean def_is_fixed_head_Light =
               s_def_is_fixed_head_light,
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
  Accumulation* m_accumulation;

  /*! Multisample object. */
  Multisample* m_multisample;
  
  /*! The geometry drawing-mode {direct, display list, or vertex array */
  Geometry_drawing_mode m_geometry_drawing_mode;

  Boolean m_are_global_lights_stationary;

  /*! Indicate whether to apply texture mapping. */
  Boolean m_texture_map;;

  Boolean m_is_fixed_head_light;

  Float m_min_frame_rate;

  /*! The polygon mode. */
  Gfx::Poly_mode m_poly_mode;

  std::string m_poly_mode_str;

  /*! Indicates whether to display the Frame-Per-second. */
  Boolean m_display_fps;

  Float m_min_zoom_distance;

  std::string m_max_model_name;

  Float m_speed_factor;

  /*! The verbosity level. */
  Uint m_verbosity_level;

  /*! Detemines whether to apply filtering across faces of the cubemap
   * resulting in a seamless transition. This was a hardware limitation in
   * the past, but modern hardware is capable of interpolating across a
   * cube face boundary. 
   */
  Boolean m_seamless_cube_map;
  
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
  static const Float s_def_min_frame_rate;
  static const Gfx::Poly_mode s_def_poly_mode;
  static const Boolean s_def_display_fps;
  static const Float s_def_min_zoom_distance;
  static const Float s_def_speed_factor;
  static const Uint s_def_verbose_level;
  static const Boolean s_def_seamless_cube_map;
  static const Boolean s_def_override_light_enable;
  static const Boolean s_def_override_blend_func;
  static const Boolean s_def_override_tex_gen;
  
  static const Char* s_geometry_drawing_mode_names[];
};

/*! \brief lsConstruct the prototype. */
inline Configuration* Configuration::prototype()
{ return new Configuration(true); }

/*! \brief clones. */
inline Container* Configuration::clone() { return new Configuration(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Configuration::get_tag() const { return s_tag; }

/*! \brief sets the flag that indicates the drawing mode. */
inline
void Configuration::set_geometry_drawing_mode(Geometry_drawing_mode mode)
{ m_geometry_drawing_mode = mode; }

/*! \brief obtains the flag that indicates the drawing mode. */
inline Configuration::Geometry_drawing_mode
Configuration::get_geometry_drawing_mode() const
{ return m_geometry_drawing_mode; }
  
SGAL_END_NAMESPACE

#endif
