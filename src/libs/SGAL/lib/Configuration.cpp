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
// $Revision: 11860 $
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
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Multisample.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Configuration::s_tag = "sgalConfiguration";
Container_proto* Configuration::s_prototype = NULL;

// Default values:
const Configuration::Geometry_drawing_mode
  Configuration::s_def_geometry_drawing_mode = Configuration::GDM_VERTEX_ARRAY;
const Boolean Configuration::s_def_are_global_lights_stationary = false;
const Boolean Configuration::s_def_texture_map = true;
const Boolean Configuration::s_def_is_fixed_head_light = true;
const Float Configuration::s_def_min_frame_rate = 15;
const Gfx::Poly_mode Configuration::s_def_poly_mode = Gfx::FILL_PMODE;
const Boolean Configuration::s_def_display_fps = false;
const Float Configuration::s_def_min_zoom_distance = 0;
const Float Configuration::s_def_speed_factor = 100;
const Uint Configuration::s_def_verbose_level = 0;
const Boolean Configuration::s_def_seamless_cube_map = true;

const Char* Configuration::s_geometry_drawing_mode_names[] =
  { "direct", "displayList", "vertexArray" };

REGISTER_TO_FACTORY(Configuration, "Configuration");

/*! Constructor */
Configuration::Configuration(Boolean proto) :
  Container(proto),
  m_accumulation(NULL),
  m_multisample(NULL),
  m_geometry_drawing_mode(s_def_geometry_drawing_mode),
  m_are_global_lights_stationary(s_def_are_global_lights_stationary),
  m_texture_map(s_def_texture_map),
  m_is_fixed_head_light(s_def_is_fixed_head_light),
  m_min_frame_rate(s_def_min_frame_rate),
  m_poly_mode(s_def_poly_mode),
  m_display_fps(s_def_display_fps),
  m_min_zoom_distance(s_def_min_zoom_distance),
  m_speed_factor(s_def_speed_factor),
  m_verbosity_level(s_def_verbose_level),
  m_seamless_cube_map(s_def_seamless_cube_map),
  m_owned_accumulation(false)
{}

/*! \brief sets defualt values. */
void Configuration::reset(Geometry_drawing_mode def_geometry_drawing_mode,
                          Boolean def_are_global_lights_stationary,
                          Boolean def_texture_map,
                          Boolean def_is_fixed_head_light,
                          Float def_min_frame_rate,
                          Gfx::Poly_mode def_poly_mode,
                          Boolean def_display_fps,
                          Float def_min_zoom_distance,
                          Float def_speed_factor,
                          Boolean def_seamless_cube_map)
{
  if (m_accumulation) m_accumulation->reset();
  
  m_geometry_drawing_mode = def_geometry_drawing_mode;
  m_are_global_lights_stationary = def_are_global_lights_stationary;
  m_is_fixed_head_light = def_is_fixed_head_light;
  m_min_frame_rate = def_min_frame_rate;
  m_poly_mode = def_poly_mode;
  m_display_fps = def_display_fps;
  m_min_zoom_distance = def_min_zoom_distance;
  m_seamless_cube_map = def_seamless_cube_map;
}

/*! \brief initializess the node prototype. */
void Configuration::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func;
  
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_int(POLY_MODE, "polyMode",
                                         get_member_offset(&m_poly_mode),
                                         exec_func));

  s_prototype->add_field_info(new SF_bool(DISPLAY_FPS, "displayFPS",
                                          get_member_offset(&m_display_fps),
                                          exec_func));

  s_prototype->
    add_field_info(new SF_bool(FIXED_HEADLIGHT, "fixedHeadLight",
                               get_member_offset(&m_is_fixed_head_light),
                               exec_func));

  s_prototype->
    add_field_info(new SF_float(MIN_FRAME_RATE, "minFrameRate",
                                get_member_offset(&m_min_frame_rate)));

  s_prototype->
    add_field_info(new SF_float(MIN_ZOOM_DISTANCE, "minZoomDistance",
                                get_member_offset(&m_min_zoom_distance)));

  s_prototype->
    add_field_info(new SF_float(SPEED_FACTOR, "speedFacotr",
                                get_member_offset(&m_speed_factor)));

  s_prototype->
    add_field_info(new SGAL::SF_bool(TEXTURE_MAP, "textureMap",
                                     get_member_offset(&m_texture_map)));

  s_prototype->
    add_field_info(new SF_uint(VERBOSITY_LEVEL, "verbosityLevel",
                               get_member_offset(&m_verbosity_level)));

  s_prototype->
    add_field_info(new SGAL::SF_bool(SEAMLESS_CUBE_MAP, "seamlessCubeMap",
                                     get_member_offset(&m_seamless_cube_map)));
  
}

/*! \brief deletes the node prototype */
void Configuration::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Configuration::get_prototype() 
{  
  if (!s_prototype) Configuration::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object. */
void Configuration::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "minFrameRate") {
      set_min_frame_rate(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "globalLightsStationary") {
      set_global_lights_stationary(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "geometryDrawingMode") {
      Uint num = sizeof(s_geometry_drawing_mode_names) / sizeof(char *);
      const char** found = std::find(s_geometry_drawing_mode_names,
                                     &s_geometry_drawing_mode_names[num],
                                     strip_double_quotes(value));
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
      std::string tmp = strip_double_quotes(value);
      if (tmp == "line") set_poly_mode(Gfx::LINE_PMODE);
      else if (tmp == "point") set_poly_mode(Gfx::POINT_PMODE);
      else std::cerr << "Unrecognized polygon mode \"" << tmp << "\"!"
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
      set_min_zoom_distance(atoff(value.c_str()));
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
      set_verbosity_level(strtoul(value.c_str(), NULL, 10));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "seamlessCubeMap") {
      set_seamless_cube_map(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string & name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "accumulation") {
      Accumulation* acc = dynamic_cast<Accumulation*>(cont);
      set_accumulation(acc);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "multisample") {
      Multisample* ms = dynamic_cast<Multisample*>(cont);
      set_multisample(ms);
      elem->mark_delete(cai);
      continue;
    }
  }
  
  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene. */  
void Configuration::add_to_scene(Scene_graph* sg)
{ sg->set_configuration(this); }

/*! \brief sets the verbosity level. */
void Configuration::set_verbosity_level(Uint level)
{
  m_verbosity_level = level;
  SGAL::Field* field = get_field(VERBOSITY_LEVEL);
  if (field) field->cascade();
}

#if 0
/*! \brief */
Attribute_list Configuration::get_attributes()
{  
  Attribute_list attrs; 
  Attribue attrib;
  char buf[32];

  attrs = Container::get_attributes();

  if (m_min_frame_rate != s_def_min_frame_rate) {
    attrib.first = "minFrameRate";
    sprintf(buf, "%g", m_min_frame_rate);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_are_global_lights_stationary != s_def_are_global_lights_stationary) {
    attrib.first = "globalLightsStationary";
    attrib.second = TRUE_STR;
    attrs.push_back(attrib);
  }
  if (m_geometry_drawing_mode != s_def_geometry_drawing_mode) {
    attrib.first = "geometryDrawingMode";
    attrib.second = s_geometry_drawing_mode_names[m_geometry_drawing_mode]);
    attrs.push_back(attrib);
  }
  if (m_is_fixed_head_light != s_def_is_fixed_head_light) {
    attrib.first = "fixedHeadLight";
    attrib.second = FALSE_STR;
    attrs.push_back(attrib);
  }
  if (m_display_fps != s_def_display_fps) {
    attrib.first = "displayFPS";
    attrib.second = TRUE_STR;
    attrs.push_back(attrib);
  }
  if (m_min_zoom_distance != s_def_min_zoom_distance) {
    attrib.first = "minZoomDistance";
    sprintf(buf, "%g", m_min_zoom_distance);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_poly_mode != s_def_poly_mode) {
    attrib.first = "polyMode";
    if (m_poly_mode == Gfx::LINE_PMODE) {
      attrib.second = "line";
    } else {
      attrib.second = "point";
    }
    attrs.push_back(attrib);
  } 
  if (!get_max_model_name().empty()) {
    attrib.first = "max_model_name";
    attrib.second = get_max_model_name();
    attrs.push_back(attrib);
  }  

  if (m_accumulation) m_accumulation->get_attributes();
  if (m_multisample) m_multisample->get_attributes();

  return attrs;
}
#endif

SGAL_END_NAMESPACE
