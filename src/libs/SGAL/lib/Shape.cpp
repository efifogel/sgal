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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

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
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Stl_binary_formatter.hpp"
#include "SGAL/Obj_formatter.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/io_vector2f.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Shape::s_tag = "Shape";
Container_proto* Shape::s_prototype(nullptr);

// Default values:
const Boolean Shape::s_def_is_visible(true);
const Vector2f Shape::s_def_depth_range(0, 1);
const Vector4ub Shape::s_def_color_mask(0xff, 0xff, 0xff, 0xff);
const Gfx::Cull_face Shape::s_def_cull_face(Gfx::NO_CULL);

const Char* Shape::s_cull_face_names[] = { "no", "front", "back", "both" };

REGISTER_TO_FACTORY(Shape, "Shape");

//! brief constructor.
// The geometry is not a mandatory field, but the appearance is. In other words
// every shape must have an appearance. If an appearance does not exists,
// a default appearance is constructed (when the appearance field is retrieved).
//
Shape::Shape(Boolean proto) :
  Node(proto),
  m_depth_range(s_def_depth_range),
  m_color_mask(s_def_color_mask),
  m_cull_face(s_def_cull_face),
  m_is_visible(s_def_is_visible),
  m_priority(0),
  m_draw_backface(false),
  m_dirty_geometry(false),
  m_dirty_appearance(true),
  m_dirty_appearance_fields(true),
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
  const auto* field_info = get_field_info(APPEARANCE);
  field_changed(field_info);
}

//! \brief obtains the appearance.
Shape::Shared_appearance Shape::get_appearance()
{
  if (m_dirty_appearance) clean_appearance();
  if (m_appearance && m_dirty_appearance_fields) clean_appearance_fields();
  return m_appearance;
}

//! \brief adds a geometry to the shape at the end of the list.
void Shape::set_geometry(Shared_geometry geometry)
{
  Observer observer(this, get_field_info(GEOMETRY));
  if (m_geometry) m_geometry->unregister_observer(observer);
  m_geometry = geometry;
  if (m_geometry) m_geometry->register_observer(observer);

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
void Shape::clean_bounding_sphere()
{
  SGAL_TRACE_CODE(Tracer::BOUNDING_SPHERE,
                  std::cout << "Shape::clean " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  if (!is_visible()) {
    m_bounding_sphere.set_radius(0);
    m_dirty_bounding_sphere = false;
    return;
  }

  if (m_dirty_geometry) clean_geometry();
  if (m_geometry) m_bounding_sphere = m_geometry->get_bounding_sphere();
  m_dirty_bounding_sphere = false;
}

//! \brief draws the appearance and then all the geometries.
Action::Trav_directive Shape::draw(Draw_action* draw_action)
{
  if (!is_visible()) return Action::TRAV_CONT;

  if (m_dirty_geometry) clean_geometry();
  if (!m_geometry) return Action::TRAV_CONT;

  if (m_dirty_appearance) clean_appearance();
  if (!m_appearance) return Action::TRAV_CONT;
  if (m_dirty_appearance_fields) clean_appearance_fields();

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
void Shape::cull(Cull_context& cull_context) { cull_context.add_shape(this); }

//! \brief draws the geometry.
void Shape::draw_geometry(Draw_action* action)
{
  if (!m_geometry) return;

  auto* context = action->get_context();
  if (m_depth_range != s_def_depth_range)
    glDepthRange(m_depth_range[0], m_depth_range[1]);
  if (m_color_mask != s_def_color_mask)
    context->draw_color_mask(m_color_mask);
  if (m_cull_face != s_def_cull_face)
    context->draw_cull_face(m_cull_face);

  m_geometry->draw(action);

  if (m_cull_face != s_def_cull_face)
    context->draw_cull_face(s_def_cull_face);
  if (m_color_mask != s_def_color_mask)
    context->draw_color_mask(s_def_color_mask);
  if (m_depth_range != s_def_depth_range)
    glDepthRange(s_def_depth_range[0], s_def_depth_range[1]);
}

//! \brief draws the shape for selection.
void Shape::isect(Isect_action* isect_action)
{
  if (!is_visible()) return;
  if (m_dirty_geometry) clean_geometry();
  if (!m_geometry) return;

  Uint id = isect_action->get_id();
  Uchar pixel[] = {0, 0, 0, 0};
  if (id != 0) isect_action->get_color(id, pixel);
  glColor4ub(pixel[0], pixel[1], pixel[2], pixel[3]);
  m_geometry->isect(isect_action);
}

//! \brief cleans the apperances.
void Shape::clean_appearance_fields()
{
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

#if 0
  //! \todo
  Shared_text text = boost::dynamic_pointer_cast<Text>(m_geometry);
  if (text) {
    text->set_appearance(m_appearance);
    m_is_text_object = true;
  }
#endif

  m_dirty_appearance_fields = false;
}

//! \brief cleans the apperance. Construct a new appearance if does not exist.
void Shape::clean_appearance()
{
  if (!m_appearance) {
    m_appearance = Shared_appearance(new Appearance);
    SGAL_assertion(m_appearance);
  }
  m_dirty_appearance = false;
}

//! \brief clean the geometry.
void Shape::clean_geometry()
{
  m_dirty_geometry = false;
}

//! \brief sets the attributes of the shape.
void Shape::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "cullFace") {
      auto num = sizeof(s_cull_face_names) / sizeof(char*);
      const auto** found =
        std::find(s_cull_face_names, &s_cull_face_names[num], value);
      auto index = found - s_cull_face_names;
      if (index < num) m_cull_face = static_cast<Gfx::Cull_face>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "visible") {
      if (!compare_to_true(value)) set_invisible();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthRange") {
      m_depth_range = boost::lexical_cast<Vector2f>(value);
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
      auto app = boost::dynamic_pointer_cast<Appearance>(cont);
      set_appearance(app);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "geometry") {
      auto geo = boost::dynamic_pointer_cast<Geometry>(cont);
      set_geometry(geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the node prototype.
void Shape::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // visible
  auto exec_func =
    static_cast<Execution_function>(&Node::bounding_sphere_changed);
  auto is_visible_func =
    static_cast<Boolean_handle_function>(&Shape::is_visible_handle);
  s_prototype->add_field_info(new SF_bool(ISVISIBLE,
                                          "visible",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          is_visible_func,
                                          s_def_is_visible,
                                          exec_func));

  // geometry
  exec_func = static_cast<Execution_function>(&Shape::geometry_changed);
  auto geometry_func =
    reinterpret_cast<Shared_container_handle_function>(&Shape::geometry_handle);
  s_prototype->add_field_info(new SF_shared_container(GEOMETRY,
                                                      "geometry",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      geometry_func,
                                                      exec_func));

  // appearance
  exec_func = static_cast<Execution_function>(&Shape::appearance_changed);
  auto appearance_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Shape::appearance_handle);
  s_prototype->add_field_info(new SF_shared_container(APPEARANCE,
                                                      "appearance",
                                                      Field_rule::RULE_EXPOSED_FIELD,
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
void Shape::appearance_changed(const Field_info* field_info)
{ Container::field_changed(field_info); }

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
  Container::field_changed(field_info);
}

/*! \brief turns on the flag that indicates whether the shape should be
 * rendered.
 */
void Shape::set_visible()
{
  if (!m_is_visible) {
    m_is_visible = true;
    m_dirty_bounding_sphere = true;
  }
}

/*! \brief turns off the flag that indicates whether the shape should be
 * rendered.
 */
void Shape::set_invisible()
{
  if (m_is_visible) {
    m_is_visible = false;
    m_dirty_bounding_sphere = true;
  }
}

//! \brief set the flag that indicates whether the shape should be rendered.
void Shape::set_visible(Boolean flag)
{
  if (flag != m_is_visible) {
    m_is_visible = flag;
    m_dirty_bounding_sphere = true;
  }
}

//! \brief processes change of field.
void Shape::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case GEOMETRY:
    m_dirty_geometry = false;
    m_dirty_bounding_sphere = true;
    break;

   case APPEARANCE:
    m_dirty_appearance = false;
    if (m_appearance) m_dirty_appearance_fields = true;
    break;

   default: break;
  }
  Node::field_changed(field_info);
}

//! \brief writes this container.
void Shape::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Tracer::EXPORT,
                  std::cout << "Shape: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);

  /* The STL binary formatter must count all triangles and write the number
   * before writting any triangles. Therefore, the STL binary formatter
   * intercept the writing of shapes. As a consequence, this function should
   * never be invoked with the STL binary formatter.
   */
  SGAL_assertion(!dynamic_cast<Stl_binary_formatter*>(formatter));

  if (m_dirty_geometry) clean_geometry();
  if (m_dirty_appearance) clean_appearance();
  if (m_dirty_appearance_fields) clean_appearance_fields();

  if (! is_visible() && ! formatter->get_export_non_visible()) return;

  auto* obj_formatter = dynamic_cast<Obj_formatter*>(formatter);
  if (obj_formatter) obj_formatter->set_visible(true);

  Container::write(formatter);
}

SGAL_END_NAMESPACE
