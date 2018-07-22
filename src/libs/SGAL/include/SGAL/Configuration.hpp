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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONFIGURATION_HPP
#define SGAL_CONFIGURATION_HPP

/*! \file
 * A class to hold general scene related configurations.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Bindable_node.hpp"

SGAL_BEGIN_NAMESPACE

class Indexed_face_set;
class Container_proto;
class Element;
class Scene_graph;
class Accumulation;
class Multisample;
class Modeling;
class Dxf_configuration;
class Bindable_stack;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Configuration : public Bindable_node {
  friend class Option_parser;
  friend class Boundary_set;
  friend class Indexed_face_set;
  friend class Shape;
  //dirty patch for accessing default flags
  //(particullary - using display lists) from Indexed_face_set

public:
  enum {
    FIRST = Bindable_node::LAST - 1,
    ACCUMULATION,
    MODELING,
    DXF_CONFIGURATION,
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
    EXPORT_SCENE_ROOT,
    EXPORT_NON_VISIBLE,
    OVERRIDE_EXPORT_NON_VISIBLE,
    LAST
  };

  enum Viewpoint_mode {
    VM_VIEWING = 0,
    VM_MODELING,
    NUM_VIEWPOINT_MODES
  };

  /*! Mutually exclusive modes of drawing geometry */
  enum Geometry_drawing_mode {
    GDM_DIRECT = 0,
    GDM_DISPLAY_LIST,
    GDM_VERTEX_ARRAY
  };

  typedef boost::shared_ptr<Accumulation>             Shared_accumulation;
  typedef boost::shared_ptr<Multisample>              Shared_multisample;
  typedef boost::shared_ptr<Modeling>                 Shared_modeling;
  typedef boost::shared_ptr<Dxf_configuration>        Shared_dxf_configuration;

  /*! Construct. */
  Configuration(Boolean proto = false);

  /*! Destruct. */
  virtual ~Configuration() {};

  /*! Construct the prototype. */
  static Configuration* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Shared_accumulation* accumulation_handle(const Field_info*)
  { return &m_accumulation; }
  Shared_modeling* modeling_handle(const Field_info*)
  { return &m_modeling; }
  Shared_dxf_configuration* dxf_configuration_handle(const Field_info*)
  { return &m_dxf_configuration; }
  Gfx::Poly_mode* poly_mode_handle(const Field_info*) { return &m_poly_mode; }
  Boolean* display_fps_handle(const Field_info*) { return &m_display_fps; }
  Boolean* is_fixed_head_light_handle(const Field_info*)
  { return &m_is_fixed_head_light; }
  Uint* stencil_bits_handle(const Field_info*) { return &m_stencil_bits; }
  Uint* depth_bits_handle(const Field_info*) { return &m_depth_bits; }
  Float* min_frame_rate_handle(const Field_info*) { return &m_min_frame_rate; }
  Float* min_zoom_distance_handle(const Field_info*)
  { return &m_min_zoom_distance; }
  Float* speed_factor_handle(const Field_info*) { return &m_speed_factor; }
  Boolean* texture_map_handle(const Field_info*) { return &m_texture_map; }
  Uint* verbose_level_handle(const Field_info*) { return &m_verbose_level; }
  Boolean* seamless_cube_map_handle(const Field_info*)
  { return &m_seamless_cube_map; }
  Uint* export_scene_root_handle(const Field_info*)
  { return &m_export_scene_root; }
  Boolean* export_non_visible_handle(const Field_info*)
  { return &m_export_non_visible; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Obtain the bindable stack. */
  virtual Bindable_stack* get_stack();

  /*! Enable the bindable node. */
  virtual void enable();

  /*! Set the scene graph. */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  /*! Set the flag that indiocates whether vertex-buffer object should be used.
   * \param[in] flag indiocates whether vertex-buffer object should be used.
   */
  void set_use_vertex_buffer_object(Boolean flag);

  /*! Determine whether vertex-buffer object extension should be used..
   * \return true iff vertex-buffer object extension should be used..
   */
  Boolean do_use_vertex_buffer_object() const;

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

  /*! Set the viewpoint mode. */
  void set_viewpoint_mode(Viewpoint_mode mode);

  /*! Obtain the viewpoint mode. */
  Viewpoint_mode get_viewpoint_mode() const;

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

  /*! Set the modeling object. */
  void set_modeling(Shared_modeling acc);

  /*! Obtain the modeling object. */
  Shared_modeling get_modeling() const;

  /*! Set the dxf-configuration object. */
  void set_dxf_configuration(Shared_dxf_configuration dxf);

  /*! Obtain the dxf-configuration object. */
  Shared_dxf_configuration get_dxf_configuration() const;

  /*! Set the verbosity level. */
  void set_verbose_level(Uint level);

  /*! Obtain the verbosity level. */
  Uint get_verbose_level() const;

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

  /*! Set the root of the scene to export.
   */
  void set_export_scene_root(Uint level);

  /*! Obtain the root of the scene to export.
   */
  Uint get_export_scene_root() const;

  /*! Set the flag that indicates whether to export non-visible geometries.
   */
  void set_export_non_visible(Boolean flag);

  /*! Indicate whether to export non-visible geometries.
   */
  Boolean get_export_non_visible() const;

  /*! Set the flag that indicates whether to override the export
   * non-visible flag.
   */
  void set_override_export_non_visible(Boolean flag);

  /*! Detemine whether to to override the export
   * non-visible flag.
   */
  Boolean is_override_export_non_visible() const;

  /*! Set defualt values. */
  void reset(Viewpoint_mode = s_def_viewpoint_mode,
             Geometry_drawing_mode def_geometry_drawing_mode =
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

  /*! Compare the i-th option to a given option */
  static Boolean compare_viewpoint_mode_name(size_t i, const Char* opt);

protected:
  /*! The Scene_graph. */
  Scene_graph* m_scene_graph;

  /*! Merge another configuration node. */
  void merge(const Configuration* other);

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

  /*! Modeling object. */
  Shared_modeling m_modeling;

  /*! Dxf-configuration object. */
  Shared_dxf_configuration m_dxf_configuration;

  /*! The viewpoint mode. */
  Viewpoint_mode m_viewpoint_mode;

  /*! The geometry drawing-mode {direct, display list, or vertex array */
  Geometry_drawing_mode m_geometry_drawing_mode;

  /*! Indicates whether vertex buffer object should be used. */
  Boolean m_use_vertex_buffer_object;

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
  Uint m_verbose_level;

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

  //! The root of the scene to export.
  Uint m_export_scene_root;

  //! Indicates whether to export non-visible geometries.
  Boolean m_export_non_visible;

  /*! Indicates whether to override the export non-visible flag.
   */
  Boolean m_override_export_non_visible;

  // default values
  static const Viewpoint_mode s_def_viewpoint_mode;
  static const Geometry_drawing_mode s_def_geometry_drawing_mode;
  static const Boolean s_def_use_vertex_buffer_object;
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
  static const Uint s_def_export_scene_root;
  static const Boolean s_def_export_non_visible;
  static const Boolean s_def_override_export_non_visible;

  /*! Drawing mode names. */
  static const Char* s_geometry_drawing_mode_names[];

  /*! Viewpoint mode names. */
  static const char* s_viewpoint_mode_names[];
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief lsConstruct the prototype.
inline Configuration* Configuration::prototype()
{ return new Configuration(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Configuration::create()
{ return new Configuration(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Configuration::get_tag() const { return s_tag; }

//! \brief
inline void Configuration::set_global_lights_stationary(Boolean flag)
{ m_are_global_lights_stationary = flag; }

//! \brief
inline Boolean Configuration::are_global_lights_stationary ()
{ return m_are_global_lights_stationary; }

//! \brief
inline void Configuration::set_fixed_head_light(Boolean flag)
{ m_is_fixed_head_light = flag; }

//! \brief
inline Boolean Configuration::is_fixed_head_light()
{ return m_is_fixed_head_light; }

//! \brief sets the number of the desired stencil bits.
inline void Configuration::set_number_of_stencil_bits(Uint bits)
{ m_stencil_bits = bits; }

//! \brief obtains the number of the desired stencil bits.
inline Uint Configuration::get_number_of_stencil_bits() const
{ return m_stencil_bits; }

//! \brief sets the number of the desired depth bits.
inline void Configuration::set_number_of_depth_bits(Uint bits)
{ m_depth_bits = bits; }

//! \brief obtains the number of the desired depth bits.
inline Uint Configuration::get_number_of_depth_bits() const
{ return m_depth_bits; }

//! \brief
inline void Configuration::set_min_frame_rate(Float rate)
{ m_min_frame_rate = rate; }

//! \brief
inline Float Configuration::get_min_frame_rate() const
{ return m_min_frame_rate; }

//! \brief
inline void Configuration::set_min_zoom_distance(Float val)
{ m_min_zoom_distance = val; }

//! \brief
inline Float Configuration::get_min_zoom_distance() const
{ return m_min_zoom_distance; }

//! \brief
inline void Configuration::set_max_model_name(const std::string& name)
{ m_max_model_name = name; }

//! \brief
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

//! \brief sets the polygon mode.
inline void Configuration::set_poly_mode(Gfx::Poly_mode mode)
{ m_poly_mode = mode; }

//! \brief obtains the polygon mode.
inline Gfx::Poly_mode Configuration::get_poly_mode () const
{ return m_poly_mode; }

//! \brief determines whether texture-maping is enabled.
inline Boolean Configuration::is_texture_map() const { return m_texture_map; }

//! \brief sets the texture-map flag.
inline void Configuration::set_texture_map(Boolean flag)
{ m_texture_map = flag; }

//! \brief obtains the transformation speed scale.
inline Float Configuration::get_speed_factor() const {return m_speed_factor; }

//! \brief sets the viewpoint mode.
inline void Configuration::set_viewpoint_mode(Viewpoint_mode mode)
{ m_viewpoint_mode = mode; }

//! \brief obtains the viewpoint mode.
inline Configuration::Viewpoint_mode Configuration::get_viewpoint_mode() const
{ return m_viewpoint_mode; }

//! \brief sets the flag that indicates the drawing mode.
inline void
Configuration::set_geometry_drawing_mode(Geometry_drawing_mode mode)
{ m_geometry_drawing_mode = mode; }

//! \brief obtains the flag that indicates the drawing mode.
inline Configuration::Geometry_drawing_mode
Configuration::get_geometry_drawing_mode() const
{ return m_geometry_drawing_mode; }

/*! \brief sets the flag that indiocates whether vertex-buffer object should be
 * used
 */
inline void Configuration::set_use_vertex_buffer_object(Boolean flag)
{ m_use_vertex_buffer_object = flag; }

//! \brief determines whether vertex-buffer object should be used..
inline Boolean Configuration::do_use_vertex_buffer_object() const
{ return m_use_vertex_buffer_object; }

//! \brief sets the accumulation object.
inline void Configuration::set_accumulation(Shared_accumulation acc)
{ m_accumulation = acc; }

//! \brief obtains the accumulation object.
inline Configuration::Shared_accumulation Configuration::get_accumulation()
  const
{ return m_accumulation; }

//! \brief sets the multisample object.
inline void Configuration::set_multisample(Shared_multisample ms)
{ m_multisample = ms; }

//! \brief obtains the multisample object.
inline Configuration::Shared_multisample Configuration::get_multisample() const
{ return m_multisample; }

//! \brief sets the modeling object.
inline void Configuration::set_modeling(Shared_modeling modeling)
{ m_modeling = modeling; }

//! \brief obtains the modeling object.
inline Configuration::Shared_modeling Configuration::get_modeling() const
{ return m_modeling; }

//! \brief sets the dxf-configuration object.
inline void Configuration::set_dxf_configuration(Shared_dxf_configuration dxf)
{ m_dxf_configuration = dxf; }

//! \brief obtains the dxf-configuration object.
inline Configuration::Shared_dxf_configuration
Configuration::get_dxf_configuration() const
{ return m_dxf_configuration; }

//! \brief obtains the verbosity level.
inline Uint Configuration::get_verbose_level() const
{ return m_verbose_level; }

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

//! \brief compares the i-th option to a given option.
inline Boolean
Configuration::compare_viewpoint_mode_name(size_t i, const Char* opt)
{ return strcmp(s_viewpoint_mode_names[i], opt) == 0; }

//! \brief sets the root of the scene to export.
inline void Configuration::set_export_scene_root(Uint level)
{ m_export_scene_root = level; }

//! \brief obtains the root of the scene to export.
inline Uint Configuration::get_export_scene_root() const
{ return m_export_scene_root; }

//! \brief sets the flag that indicates whether to export non-visible geometries.
inline void Configuration::set_export_non_visible(Boolean flag)
{ m_export_non_visible = flag; }

//! \brief indicates whether to export non-visible geometries.
inline Boolean Configuration::get_export_non_visible() const
{ return m_export_non_visible; }

//! \brief sets the flag that indicates whether to override the export
inline void Configuration::set_override_export_non_visible(Boolean flag)
{ m_override_export_non_visible = flag; }

//! \brief detemines whether to to override the export
inline Boolean Configuration::is_override_export_non_visible() const
{ return m_override_export_non_visible; }

SGAL_END_NAMESPACE

#endif
