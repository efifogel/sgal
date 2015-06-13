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

#include <iostream>
#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Obj_formatter.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Shape::s_tag = "Shape";
Container_proto* Shape::s_prototype(nullptr);

// Default values:
const Boolean Shape::s_def_is_visible(true);
const Vector2f Shape::s_def_depth_range(0, 1);
const Vector4ub Shape::s_def_color_mask(0xff, 0xff, 0xff, 0xff);
const Gfx::Depth_func Shape::s_def_depth_function(Gfx::LESS_DFUNC);
const Gfx::Cull_face Shape::s_def_cull_face(Gfx::NO_CULL);

const Char* Shape::s_depth_function_names[] = {
  "never", "less", "equal", "lequal", "greater", "notequal", "gequal", "always"
};

const Char* Shape::s_cull_face_names[] = { "no", "front", "back", "both" };

REGISTER_TO_FACTORY(Shape, "Shape");

//! brief constructor.
Shape::Shape(Boolean proto) :
  Node(proto),
  m_draw_depth(true),
  m_test_depth(true),
  m_depth_range(s_def_depth_range),
  m_depth_function(s_def_depth_function),
  m_color_mask(s_def_color_mask),
  m_cull_face(s_def_cull_face),
  m_is_visible(s_def_is_visible),
  m_priority(0),
  m_draw_backface(false),
  m_dirty(true),
  m_dirty_appearance(true),
  m_texture_map(Configuration::s_def_texture_map),
  m_override_material(Configuration::s_def_override_material),
  m_override_tex_enable(Configuration::s_def_override_tex_enable),
  m_override_tex_env(Configuration::s_def_override_tex_env),
  m_override_blend_func(Configuration::s_def_override_blend_func),
  m_override_light_model(Configuration::s_def_override_light_model),
  m_override_tex_gen(Configuration::s_def_override_tex_gen),
  m_override_light_enable(Configuration::s_def_override_light_enable)
{}

//! \brief destructor.
Shape::~Shape()
{
  if (m_geometry) {
    Observer observer(this, get_field_info(GEOMETRY));
    m_geometry->unregister_observer(observer);
  }
}

//! \brief sets the appearance of the object.
void Shape::set_appearance(Shared_appearance app)
{
  m_appearance = app;
  m_dirty = true;
  m_dirty_appearance = true;

  const auto* field_info = get_field_info(APPEARANCE);
  field_changed(field_info);
}

//! \brief adds a geometry to the shape at the end of the list.
void Shape::set_geometry(Shared_geometry geometry)
{
  Observer observer(this, get_field_info(GEOMETRY));
  if (m_geometry) m_geometry->unregister_observer(observer);
  m_geometry = geometry;
  if (m_geometry) m_geometry->register_observer(observer);
  m_dirty = true;
  m_dirty_sphere_bound = true;

  const Field_info* field_info = get_field_info(GEOMETRY);
  field_changed(field_info);

#if 0
  //! \todo
  // the text object needs to pass a pointer to the appearance
  // for the FontTexture object
  Shared_text text = boost::dynamic_pointer_cast<Text>(geometry);
  if (text) {
    text->set_appearance(m_appearance);
    m_is_text_object = true;
  }
#endif
}

//! \brief cleans the bounding sphere of all geometries in the shape.
void Shape::clean_sphere_bound()
{
  if (!is_visible()) {
    m_sphere_bound.set_radius(0);
    m_dirty_sphere_bound = false;
    return;
  }

  if (m_geometry) m_sphere_bound = *m_geometry->get_sphere_bound();
  m_dirty_sphere_bound = false;
}

//! \brief draws the appearance and then all the geometries.
Action::Trav_directive Shape::draw(Draw_action* draw_action)
{
  if (!m_geometry || !is_visible()) return Action::TRAV_CONT;
  if (m_dirty) clean();

  int pass_no = draw_action->get_pass_no();
  if (!m_draw_backface && m_appearance->is_transparent() && (pass_no == 0)) {
    draw_action->set_second_pass_required(true);
    return Action::TRAV_CONT;
  }

  m_appearance->draw(draw_action);
  draw_geometry(draw_action);

  return Action::TRAV_CONT;
}

//! \brief culls the node if invisible and prepare for rendering.
void Shape::cull(Cull_context& cull_context)
{ cull_context.add_shape(this); }

//! \brief draws the geometry.
void Shape::draw_geometry(Draw_action* action)
{
  if (m_geometry) {
    Context* context = action->get_context();
    if (!m_draw_depth) context->draw_depth_mask(false);
    if (!m_test_depth) context->draw_depth_enable(false);
    if (m_depth_range != s_def_depth_range)
      glDepthRange(m_depth_range[0], m_depth_range[1]);
    if (m_depth_function != s_def_depth_function)
      context->draw_depth_func(m_depth_function);
    if (m_color_mask != s_def_color_mask)
      context->draw_color_mask(m_color_mask);
    if (m_cull_face != s_def_cull_face)
      context->draw_cull_face(m_cull_face);

    m_geometry->draw(action);

    if (m_cull_face != s_def_cull_face)
      context->draw_cull_face(s_def_cull_face);
    if (m_color_mask != s_def_color_mask)
      context->draw_color_mask(s_def_color_mask);
    if (m_depth_function != s_def_depth_function)
      context->draw_depth_func(s_def_depth_function);
    if (m_depth_range != s_def_depth_range)
      glDepthRange(s_def_depth_range[0], s_def_depth_range[1]);
    if (!m_test_depth) context->draw_depth_enable(true);
    if (!m_draw_depth) context->draw_depth_mask(true);  }
}

//! \brief draws the shape for selection.
void Shape::isect(Isect_action* isect_action)
{
  if (!is_visible()) return;
  if (!m_geometry) return;

  Uint id = isect_action->get_id();
  Uchar pixel[] = {0, 0, 0, 0};
  if (id != 0) isect_action->get_color(id, pixel);
  glColor4ub(pixel[0], pixel[1], pixel[2], pixel[3]);
  m_geometry->isect(isect_action);
}

//! \brief cleans the apperances.
void Shape::clean()
{
  // Create an appearance if missing:
  if (m_dirty_appearance) clean_appearance();
  // Create a material if missing:
  if (m_override_material) m_appearance->clean_material();
  // Enable texture if texture exists and not empty:
  if (m_override_tex_enable) m_appearance->clean_tex_enable();
  // Disable the texture if texture mapping is not desired:
  if (!m_texture_map) m_appearance->set_tex_enable(false);

  if (m_override_tex_env) m_appearance->clean_tex_env();
  if (m_override_blend_func) m_appearance->clean_blend_func();
  if (m_override_light_model) m_appearance->clean_light_model();

  // If the geometry has no color coordinates, enabled the light by default.
  if (m_override_light_enable) {
    if (m_geometry && !m_geometry->has_color())
      m_appearance->set_light_enable(true);
  }

  if (m_override_tex_gen) {
    // Enable texture generation if texture is enabled, and the geometry does
    // not have a texture-coordinate array.
    if (m_appearance->get_tex_enable() &&
        !m_geometry->do_generate_tex_coord() && !m_geometry->has_tex_coord())
      m_appearance->clean_tex_gen();
  }

  if (m_dirty_appearance) m_dirty_appearance = false;

#if 0
  //! \todo
  Shared_text text = boost::dynamic_pointer_cast<Text>(m_geometry);
  if (text) {
    text->set_appearance(m_appearance);
    m_is_text_object = true;
  }
#endif

  m_dirty = false;
}

//! \brief cleans the apperance.
void Shape::clean_appearance()
{
  if (!m_appearance) {
    m_appearance = Shared_appearance(new Appearance);
    SGAL_assertion(m_appearance);
  }
}

//! \brief sets the attributes of the shape.
void Shape::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "cullFace") {
      Uint num = sizeof(s_cull_face_names) / sizeof(char*);
      const char** found = std::find(s_cull_face_names,
                                     &s_cull_face_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_cull_face_names;
      if (index < num)
        m_cull_face = static_cast<Gfx::Cull_face>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthFunction") {
      Uint num = sizeof(s_depth_function_names) / sizeof(char*);
      const char** found = std::find(s_depth_function_names,
                                      &s_depth_function_names[num],
                                      strip_double_quotes(value));
      Uint index = found - s_depth_function_names;
      if (index < num)
        m_depth_function = static_cast<Gfx::Depth_func>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "visible") {
      if (!compare_to_true(value)) set_invisible();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDepth") {
      m_draw_depth = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "testDepth") {
      m_test_depth = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthRange") {
      m_depth_range = Vector2f(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "colorMask") {
      m_color_mask = Vector4ub(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "priority") {
      set_priority(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawBackface") {
      m_draw_backface = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "appearance") {
      Shared_appearance app = boost::dynamic_pointer_cast<Appearance>(cont);
      set_appearance(app);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "geometry") {
      Shared_geometry geo = boost::dynamic_pointer_cast<Geometry>(cont);
      set_geometry(geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! \brief obtains a list of attributes (called in the save process).
Attribute_list Shape::get_attributes()
{
  Attribute_list attrs;
  attrs = Node::get_attributes();
  Attribue attrib;

  if (m_is_visible != true) {
    attrib.first = "visible";
    attrib.second = "FALSE";
    attrs.push_back(attrib);
  }

  return attrs;
}
#endif

//! \brief initializes the node prototype.
void Shape::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // visible
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::sphere_bound_changed);
  Boolean_handle_function is_visible_func =
    static_cast<Boolean_handle_function>(&Shape::is_visible_handle);
  s_prototype->add_field_info(new SF_bool(ISVISIBLE,
                                          "visible",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_visible_func,
                                          s_def_is_visible,
                                          exec_func));

  // geometry
  exec_func = static_cast<Execution_function>(&Shape::geometry_changed);
  Shared_container_handle_function geometry_func =
    reinterpret_cast<Shared_container_handle_function>(&Shape::geometry_handle);
  s_prototype->add_field_info(new SF_shared_container(GEOMETRY,
                                                      "geometry",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      geometry_func,
                                                      exec_func));

  // appearance
  exec_func = static_cast<Execution_function>(&Shape::appearance_changed);
  Shared_container_handle_function appearance_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Shape::appearance_handle);
  s_prototype->add_field_info(new SF_shared_container(APPEARANCE,
                                                      "appearance",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      appearance_func,
                                                      exec_func));
}

//! \brief deletes the node prototype.
void Shape::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Shape::get_prototype()
{
  if (s_prototype == nullptr) Shape::init_prototype();
  return s_prototype;
}

//! \brief adds the container to the given scene.
void Shape::add_to_scene(Scene_graph* sg)
{
  auto* config = sg->get_configuration();
  if (config) m_texture_map = config->is_texture_map();
}

//! \brief
Boolean Shape::attach_context(Context* context)
{
  Boolean result = Node::attach_context(context);
  Shared_appearance app = get_appearance();
  if (app) result &= app->attach_context(context);
  return result;
}

//! \brief
Boolean Shape::detach_context(Context* context)
{
  Boolean result = Node::detach_context(context);
  auto app = get_appearance();
  if (app) result &= app->detach_context(context);
  return result;
}

//! \brief processes change of appearance.
void Shape::appearance_changed(const Field_info* /* field_info. */)
{
  m_dirty = true;
  m_dirty_appearance = true;
}

//! \brief processes change of geometry.
void Shape::geometry_changed(const Field_info* field_info)
{
  // Observe that the observer, being a pair of this container object and the
  // field_info argument, is automatically unregistered as an observer
  // of the previous value of the m_geometry.
  if (m_geometry) {
    Observer observer(this, field_info);
    if (m_geometry) m_geometry->register_observer(observer);
  }
  m_dirty_sphere_bound = true;
  Container::field_changed(field_info);
}

/*! \brief turns on the flag that indicates whether the shape should be
 * rendered.
 */
void Shape::set_visible()
{
  if (!m_is_visible) {
    m_is_visible = true;
    m_dirty_sphere_bound = true;
  }
}

/*! \brief turns off the flag that indicates whether the shape should be
 * rendered.
 */
void Shape::set_invisible()
{
  if (m_is_visible) {
    m_is_visible = false;
    m_dirty_sphere_bound = true;
  }
}

//! \brief set the flag that indicates whether the shape should be rendered.
void Shape::set_visible(Boolean flag)
{
  if (flag != m_is_visible) {
    m_is_visible = flag;
    m_dirty_sphere_bound = true;
  }
}

//! \brief processes change of field.
void Shape::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case GEOMETRY:
    m_dirty_sphere_bound = true;
    break;

   case APPEARANCE:
    m_dirty = true;
    m_dirty_appearance = true;
    break;
   default: break;
  }
  Node::field_changed(field_info);
}

//! \brief writes this container.
void Shape::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Shape: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  if (m_dirty) clean();
  auto* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  auto* obj_formatter = dynamic_cast<Obj_formatter*>(formatter);
  if (stl_formatter || obj_formatter) {
    if (!is_visible()) return;
    if (obj_formatter) obj_formatter->set_visible(true);
  }

  Container::write(formatter);
}

SGAL_END_NAMESPACE
