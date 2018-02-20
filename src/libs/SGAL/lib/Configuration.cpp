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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Modeling.hpp"
#include "SGAL/Window_item.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Configuration::s_tag = "Configuration";
Container_proto* Configuration::s_prototype(nullptr);

// Default values:
const Configuration::Viewpoint_mode
  Configuration::s_def_viewpoint_mode(VM_VIEWING);
const Configuration::Geometry_drawing_mode
  Configuration::s_def_geometry_drawing_mode(Configuration::GDM_VERTEX_ARRAY);
const Boolean Configuration::s_def_use_vertex_buffer_object(true);
const Boolean Configuration::s_def_are_global_lights_stationary(false);
const Boolean Configuration::s_def_texture_map(true);
const Boolean Configuration::s_def_is_fixed_head_light(true);
const Uint Configuration::s_def_stencil_bits(SGAL_DEF_WINDOW_STENCIL_BITS);
const Uint Configuration::s_def_depth_bits(SGAL_DEF_WINDOW_DEPTH_BITS);
const Float Configuration::s_def_min_frame_rate(15);
const Gfx::Poly_mode Configuration::s_def_poly_mode(Gfx::FILL_PMODE);
const Boolean Configuration::s_def_display_fps(false);
const Float Configuration::s_def_min_zoom_distance(0);
const Float Configuration::s_def_speed_factor(100);
const Uint Configuration::s_def_verbose_level(0);
const Boolean Configuration::s_def_seamless_cube_map(true);
const Boolean Configuration::s_def_override_material(true);
const Boolean Configuration::s_def_override_tex_enable(true);
const Boolean Configuration::s_def_override_tex_env(true);
const Boolean Configuration::s_def_override_blend_func(true);
const Boolean Configuration::s_def_override_light_model(true);
const Boolean Configuration::s_def_override_tex_gen(true);
const Boolean Configuration::s_def_override_light_enable(true);
const Boolean Configuration::s_def_export_scene(false);

const Char* Configuration::s_geometry_drawing_mode_names[] =
  { "direct", "displayList", "vertexArray" };

const Char* Configuration::s_viewpoint_mode_names[] =
  { "viewing", "modeling" };

REGISTER_TO_FACTORY(Configuration, "Configuration");

//! \brief constructs.
Configuration::Configuration(Boolean proto) :
  Bindable_node(proto),
  m_scene_graph(nullptr),
  m_viewpoint_mode(s_def_viewpoint_mode),
  m_geometry_drawing_mode(s_def_geometry_drawing_mode),
  m_use_vertex_buffer_object(s_def_use_vertex_buffer_object),
  m_are_global_lights_stationary(s_def_are_global_lights_stationary),
  m_texture_map(s_def_texture_map),
  m_is_fixed_head_light(s_def_is_fixed_head_light),
  m_stencil_bits(s_def_stencil_bits),
  m_depth_bits(s_def_depth_bits),
  m_min_frame_rate(s_def_min_frame_rate),
  m_poly_mode(s_def_poly_mode),
  m_display_fps(s_def_display_fps),
  m_min_zoom_distance(s_def_min_zoom_distance),
  m_speed_factor(s_def_speed_factor),
  m_verbosity_level(s_def_verbose_level),
  m_seamless_cube_map(s_def_seamless_cube_map),
  m_override_material(Configuration::s_def_override_material),
  m_override_tex_enable(Configuration::s_def_override_tex_enable),
  m_override_tex_env(Configuration::s_def_override_tex_env),
  m_override_blend_func(Configuration::s_def_override_blend_func),
  m_override_light_model(Configuration::s_def_override_light_model),
  m_override_tex_gen(Configuration::s_def_override_tex_gen),
  m_override_light_enable(Configuration::s_def_override_light_enable),
  m_export_scene(s_def_export_scene)
{}

//! \brief sets defualt values.
void Configuration::reset(Viewpoint_mode viewpoint_mode,
                          Geometry_drawing_mode def_geometry_drawing_mode,
                          Boolean def_are_global_lights_stationary,
                          Boolean def_texture_map,
                          Boolean def_is_fixed_head_light,
                          Uint def_stencil_bits,
                          Uint def_depth_bits,
                          Float def_min_frame_rate,
                          Gfx::Poly_mode def_poly_mode,
                          Boolean def_display_fps,
                          Float def_min_zoom_distance,
                          Float def_speed_factor,
                          Boolean def_seamless_cube_map)
{
  if (m_accumulation) m_accumulation->reset();
  if (m_modeling) m_modeling->reset();

  m_viewpoint_mode = viewpoint_mode;
  m_geometry_drawing_mode = def_geometry_drawing_mode;
  m_are_global_lights_stationary = def_are_global_lights_stationary;
  m_texture_map = def_texture_map;
  m_is_fixed_head_light = def_is_fixed_head_light;
  m_stencil_bits = def_stencil_bits;
  m_depth_bits = def_depth_bits;
  m_min_frame_rate = def_min_frame_rate;
  m_poly_mode = def_poly_mode;
  m_display_fps = def_display_fps;
  m_min_zoom_distance = def_min_zoom_distance;
  m_speed_factor = def_speed_factor;
  m_seamless_cube_map = def_seamless_cube_map;
}

//! \brief initializess the node prototype.
void Configuration::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func;

  // polyMode
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Uint_handle_function poly_mode_func =
    reinterpret_cast<Uint_handle_function>(&Configuration::poly_mode_handle);
  s_prototype->add_field_info(new SF_uint(POLY_MODE, "polyMode",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          poly_mode_func,
                                          s_def_poly_mode, exec_func));

  // displayFPS
  Boolean_handle_function display_fps_func =
    static_cast<Boolean_handle_function>(&Configuration::display_fps_handle);
  s_prototype->add_field_info(new SF_bool(DISPLAY_FPS, "displayFPS",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          display_fps_func,
                                          s_def_display_fps, exec_func));

  // fixedHeadLight
  Boolean_handle_function is_fixed_head_light_func =
    static_cast<Boolean_handle_function>
    (&Configuration::is_fixed_head_light_handle);
  s_prototype->add_field_info(new SF_bool(FIXED_HEADLIGHT, "fixedHeadLight",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_fixed_head_light_func,
                                          s_def_is_fixed_head_light,
                                          exec_func));

  // stencilBits
  Uint_handle_function stencil_bits_func =
    static_cast<Uint_handle_function>(&Configuration::stencil_bits_handle);
  s_prototype->add_field_info(new SF_uint(STENCIL_BITS, "stencilBits",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          stencil_bits_func,
                                          s_def_stencil_bits));

  // depthBits
  Uint_handle_function depth_bits_func =
    static_cast<Uint_handle_function>(&Configuration::depth_bits_handle);
  s_prototype->add_field_info(new SF_uint(DEPTH_BITS, "depthBits",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          depth_bits_func, s_def_depth_bits));

  // minFrameRate
  Float_handle_function min_frame_rate_func =
    static_cast<Float_handle_function>(&Configuration::min_frame_rate_handle);
  s_prototype->add_field_info(new SF_float(MIN_FRAME_RATE, "minFrameRate",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           min_frame_rate_func,
                                           s_def_min_frame_rate));

  // minZoomDistance
  Float_handle_function min_zoom_distance_func =
    static_cast<Float_handle_function>
    (&Configuration::min_zoom_distance_handle);
  s_prototype->add_field_info(new SF_float(MIN_ZOOM_DISTANCE,
                                           "minZoomDistance",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           min_zoom_distance_func,
                                           s_def_min_zoom_distance));

  // speedFacotor
  Float_handle_function speed_factor_func =
    static_cast<Float_handle_function>(&Configuration::speed_factor_handle);
  s_prototype->add_field_info(new SF_float(SPEED_FACTOR, "speedFacotor",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           speed_factor_func,
                                           s_def_speed_factor));

  // textureMap
  Boolean_handle_function texture_map_func =
    static_cast<Boolean_handle_function>(&Configuration::texture_map_handle);
  s_prototype->add_field_info(new SF_bool(TEXTURE_MAP, "textureMap",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          texture_map_func,
                                          s_def_texture_map));

  // verbosityLevel
  Uint_handle_function verbosity_level_func =
    static_cast<Uint_handle_function>(&Configuration::verbosity_level_handle);
  s_prototype->add_field_info(new SF_uint(VERBOSITY_LEVEL, "verbosityLevel",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          verbosity_level_func,
                                          s_def_verbose_level));

  // seamlessCubeMap
  Boolean_handle_function seamless_cube_map_func =
    static_cast<Boolean_handle_function>
    (&Configuration::seamless_cube_map_handle);
  s_prototype->add_field_info(new SF_bool(SEAMLESS_CUBE_MAP, "seamlessCubeMap",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          seamless_cube_map_func,
                                          s_def_seamless_cube_map));

  // Accumulation
  Shared_container_handle_function accumulation_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Configuration::accumulation_handle);
  s_prototype->add_field_info(new SF_shared_container(ACCUMULATION,
                                                      "accumulation",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      accumulation_func,
                                                      exec_func));

  // Modeling
  Shared_container_handle_function modeling_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Configuration::modeling_handle);
  s_prototype->add_field_info(new SF_shared_container(MODELING,
                                                      "modeling",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      modeling_func,
                                                      exec_func));

  // exportScene
  Boolean_handle_function export_scene_func =
    static_cast<Boolean_handle_function>(&Configuration::export_scene_handle);
  s_prototype->add_field_info(new SF_bool(EXPORT_SCENE, "exportScene",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          export_scene_func,
                                          s_def_export_scene, exec_func));
}

//! \brief deletes the node prototype.
void Configuration::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Configuration::get_prototype()
{
  if (!s_prototype) Configuration::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Configuration::set_attributes(Element* elem)
{
  Bindable_node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "stencilBits") {
      set_number_of_stencil_bits(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthBits") {
      set_number_of_depth_bits(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "minFrameRate") {
      set_min_frame_rate(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "globalLightsStationary") {
      set_global_lights_stationary(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "viewpointMode") {
      auto num = sizeof(s_viewpoint_mode_names) / sizeof(char *);
      const auto** found =
        std::find(s_viewpoint_mode_names, &s_viewpoint_mode_names[num], value);
      Uint index = found - s_viewpoint_mode_names;
      if (index < num) m_viewpoint_mode = static_cast<Viewpoint_mode>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "geometryDrawingMode") {
      auto num = sizeof(s_geometry_drawing_mode_names) / sizeof(char *);
      const auto** found = std::find(s_geometry_drawing_mode_names,
                                     &s_geometry_drawing_mode_names[num],
                                     value);
      Uint index = found - s_geometry_drawing_mode_names;
      if (index < num)
        m_geometry_drawing_mode = static_cast<Geometry_drawing_mode>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fixedHeadLight") {
      set_fixed_head_light(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "polyMode") {
      if (value == "line") set_poly_mode(Gfx::LINE_PMODE);
      else if (value == "point") set_poly_mode(Gfx::POINT_PMODE);
      else std::cerr << "Unrecognized polygon mode \"" << value << "\"!"
                     << std::endl;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "displayFPS") {
      set_display_fps(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "minZoomDistance") {
      set_min_zoom_distance(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "maxModelName") {
      set_max_model_name(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "textureMap") {
      m_texture_map = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "verbosityLevel") {
      set_verbosity_level(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "seamlessCubeMap") {
      set_seamless_cube_map(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideMaterial") {
      m_override_material = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideTexEnable") {
      m_override_tex_enable = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideTexEnv") {
      m_override_tex_env = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideBlendFunc") {
      m_override_blend_func = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideLightModel") {
      m_override_light_model = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideTexGen") {
      m_override_tex_gen = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "overrideLightEnable") {
      m_override_light_enable = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "exportScene") {
      set_export_scene(boost::lexical_cast<Boolean>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "accumulation") {
      auto acc = boost::dynamic_pointer_cast<Accumulation>(cont);
      set_accumulation(acc);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "multisample") {
      Shared_multisample ms = boost::dynamic_pointer_cast<Multisample>(cont);
      set_multisample(ms);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "modeling") {
      auto modeling = boost::dynamic_pointer_cast<Modeling>(cont);
      set_modeling(modeling);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Configuration::add_to_scene(Scene_graph* sg)
{
  set_scene_graph(sg);
  auto* top = top_stack();
  if (top) merge(static_cast<Configuration*>(top));
  push_stack(this);     // push the new node onto the top of the stack
}

//! \brief sets the verbosity level.
void Configuration::set_verbosity_level(Uint level)
{
  m_verbosity_level = level;
  SGAL::Field* field = get_field(VERBOSITY_LEVEL);
  if (field) field->cascade();
}

//! \brief obtains the bindable stack.
Bindable_stack* Configuration::get_stack()
{ return m_scene_graph->get_configuration_stack(); }

//! \brief enables the camera---called when the camera is bound.
void Configuration::enable() {}

//! \brief merges another configuration node.
/*! \todo We use a stack of configurations. When a new configuration is
 * introduced, for every attribute we carry over the value from the previous
 * configuration if this value is not equal to its default. If the attribute
 * was excplicitly set with its default value (remaining intact) it is ignored
 * and the intension to force its default value is lost. We need to add a
 * Boolean flag for each attribute indicating whether it was explicitly set.
 */
void Configuration::merge(const Configuration* other)
{
  if (other->m_accumulation) m_accumulation = other->m_accumulation;
  if (other->m_modeling) m_modeling = other->m_modeling;
  if (other->m_viewpoint_mode != s_def_viewpoint_mode)
    m_viewpoint_mode = other->m_viewpoint_mode;
  if (other->m_geometry_drawing_mode != s_def_geometry_drawing_mode)
    m_geometry_drawing_mode = other->m_geometry_drawing_mode;
  if (other->m_use_vertex_buffer_object != s_def_use_vertex_buffer_object)
    m_use_vertex_buffer_object = other->m_use_vertex_buffer_object;
  if (other->m_are_global_lights_stationary != s_def_are_global_lights_stationary)
    m_are_global_lights_stationary = other->m_are_global_lights_stationary;
  if (other->m_texture_map != s_def_texture_map)
    m_texture_map = other->m_texture_map;
  if (other->m_is_fixed_head_light != s_def_is_fixed_head_light)
    m_is_fixed_head_light = other->m_is_fixed_head_light;
  if (other->m_stencil_bits != s_def_stencil_bits)
    m_stencil_bits = other->m_stencil_bits;
  if (other->m_depth_bits != s_def_depth_bits)
    m_depth_bits = other->m_depth_bits;
  if (other->m_min_frame_rate != s_def_min_frame_rate)
    m_min_frame_rate = other->m_min_frame_rate;
  if (other->m_poly_mode != s_def_poly_mode)
    m_poly_mode = other->m_poly_mode;
  if (other->m_display_fps != s_def_display_fps)
    m_display_fps = other->m_display_fps;
  if (other->m_min_zoom_distance != s_def_min_zoom_distance)
    m_min_zoom_distance = other->m_min_zoom_distance;
  if (other->m_speed_factor != s_def_speed_factor)
    m_speed_factor = other->m_speed_factor;
  if (other->m_verbosity_level != s_def_verbose_level)
    m_verbosity_level = other->m_verbosity_level;
  if (other->m_seamless_cube_map != s_def_seamless_cube_map)
    m_seamless_cube_map = other->m_seamless_cube_map;
  if (other->m_override_material != s_def_override_material)
    m_override_material = other->m_override_material;
  if (other->m_override_tex_enable != s_def_override_tex_enable)
    m_override_tex_enable = other->m_override_tex_enable;
  if (other->m_override_tex_env != s_def_override_tex_env)
    m_override_tex_env = other->m_override_tex_env;
  if (other->m_override_blend_func != s_def_override_blend_func)
    m_override_blend_func = other->m_override_blend_func;
  if (other->m_override_light_model != s_def_override_light_model)
    m_override_light_model = other->m_override_light_model;
  if (other->m_override_tex_gen != s_def_override_tex_gen)
    m_override_tex_gen = other->m_override_tex_gen;
  if (other->m_override_light_enable != s_def_override_light_enable)
    m_override_light_enable = other->m_override_light_enable;
  if (other->m_export_scene != s_def_export_scene)
    m_export_scene = other->m_export_scene;
}

SGAL_END_NAMESPACE
