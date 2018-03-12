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

/*!
 * \todo Introduce "enable" event
 */

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Navigation_sensor.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Camera.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Navigation_sensor::s_tag = "NavigationSensor";
Container_proto* Navigation_sensor::s_prototype(NULL);

REGISTER_TO_FACTORY(Navigation_sensor, "Navigation_sensor");

//! \brief constructs.
Navigation_sensor::Navigation_sensor(const Vector3f& translation,
                                     const Rotation& rotation,
                                     Boolean proto) :
  Bindable_node(proto),
  m_scene_graph(NULL),
  m_translation(translation),
  m_rotation(rotation),
  m_drag_locked(false),
  m_is_left_button_double_click(false),
  m_reset(false),
  m_min_zoom_distance(0),
  m_dragging_speed(1),
  m_half_dragging_speed(0.5f),
  m_translation_done(false),
  m_rotation_done(false),
  m_translating(false), m_rotating(false)
{}

//! \brief destructs.
Navigation_sensor::~Navigation_sensor() {}

//! \brief initializes the node prototype.
void Navigation_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());

  // Add the object field infos to the prototype
  // translation
  Vector3f_handle_function translation_func =
    static_cast<Vector3f_handle_function>
    (&Navigation_sensor::translation_handle);
  s_prototype->add_field_info(new SF_vector3f(TRANSLATION,
                                              "translation",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              translation_func));

  // rotation
  Rotation_handle_function rotation_func =
    static_cast<Rotation_handle_function>(&Navigation_sensor::rotation_handle);
  s_prototype->add_field_info(new SF_rotation(ROTATION,
                                              "rotation",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              rotation_func));

  // minZoomDistance
  Float_handle_function min_zoom_distance_func =
    static_cast<Float_handle_function>
    (&Navigation_sensor::min_zoom_distance_handle);
  s_prototype->add_field_info(new SF_float(MIN_ZOOM_DISTANCE,
                                           "minZoomDistance",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           min_zoom_distance_func));

  // translationDone
  Boolean_handle_function translation_done_func =
    static_cast<Boolean_handle_function>
    (&Navigation_sensor::translation_done_handle);
  s_prototype->add_field_info(new SF_bool(TRANSLATION_DONE,
                                          "translationDone",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          translation_done_func));

  // rotationDone
  Boolean_handle_function rotation_done_func =
    static_cast<Boolean_handle_function>
    (&Navigation_sensor::rotation_done_handle);
  s_prototype->add_field_info(new SF_bool(ROTATION_DONE,
                                          "rotationDone",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          rotation_done_func));
}

//! \brief deletes the prototype.
void Navigation_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the prototype.
Container_proto* Navigation_sensor::get_prototype()
{
  if (!s_prototype) Navigation_sensor::init_prototype();
  return s_prototype;
}

//! \brief setss the translation field.
void Navigation_sensor::set_translation(const Vector3f& translation)
{ set_translation(translation[0], translation[1], translation[2]); }

//! \brief sets the translation field.
void Navigation_sensor::set_translation(float v0, float v1, float v2)
{ m_translation.set(v0, v1, v2); }

//! \brief sets the rotation field.
void Navigation_sensor::set_rotation(const Rotation& rotation)
{
  set_rotation(rotation[0], rotation[1], rotation[2], rotation.get_angle());
}

//! \brief sets the rotation field.
void Navigation_sensor::set_rotation(float v0, float v1, float v2, float angle)
{
  m_rotation.set_axis(v0, v1, v2);
  m_rotation.set_angle(angle);
}

//! \brief registers the mouse and mostion events.
void Navigation_sensor::register_events()
{
  Mouse_event::doregister(this);
  Motion_event::doregister(this);
}

//! registers the mouse and mostion events.
void Navigation_sensor::unregister_events()
{
  Mouse_event::unregister(this);
  Motion_event::unregister(this);
}

//! handles mouse events.
void Navigation_sensor::handle(Mouse_event* event)
{
  switch (event->get_button()) {
   case Mouse_event::LEFT_BUTTON:
    if (event->get_state() ==  Mouse_event::DOWN)
      left_button_down(event->get_x(), event->get_y());
    else left_button_up(event->get_x(), event->get_y());
    break;

   case Mouse_event::MIDDLE_BUTTON:
    // if (event->get_state() ==  Mouse_event::DOWN) {
    // } else {
    // }
    break;

   case Mouse_event::RIGHT_BUTTON:
    if (event->get_state() ==  Mouse_event::DOWN)
      right_button_down(event->get_x(), event->get_y());
    else right_button_up(event->get_x(), event->get_y());
    break;
  }
}

//! \brief handles motion events.
void Navigation_sensor::handle(Motion_event* event)
{ mouse_move(event->get_x(), event->get_y()); }

//! \brief prints an identification message.
void Navigation_sensor::identify()
{ std::cout << "Agent: Navigation_sensor" << std::endl; }

//! \brief activated when dragging is started.
void Navigation_sensor::start_dragging(const Vector2sh& /* point */)
{
  transform_done();
  m_reset_rotation = true;

#if 0
  // If the dragging is locked by an other sensor - return
  if (m_execution_coordinator->is_dragging_locked()) return;
  // Lock the dragging
  m_execution_coordinator->lock_dragging();
#endif

  m_drag_locked = true;
}

//! \brief sets the dragging speed of the sensor.
void Navigation_sensor::set_dragging_speed(float dragging_speed)
{
  m_dragging_speed = dragging_speed;
  if (m_dragging_speed == 0) m_dragging_speed = 0.01f;
  m_half_dragging_speed = m_dragging_speed * 0.5f;
}

//! \brief sets the minimum zoom in distance.
void Navigation_sensor::set_min_zoom_distance(float val)
{ m_min_zoom_distance = val; }

//! \brief handles mouse dragging.
void Navigation_sensor::mouse_drag(const Vector2sh& from,
                                   const Vector2sh& to)
{
  // If the dragging is not locked for this instance, return:
  if (!m_drag_locked) return;

  if (is_right_mouse_down() && !is_left_mouse_down()) {
    float dy = static_cast<float>(to[1] - from[1]);
    Vector3f point(0.0f, 0.0f, dy);
    translate(point, m_dragging_speed);
  }
  else if ((is_control_down() && is_left_mouse_down()) ||
           (is_left_mouse_down() && is_right_mouse_down()))
  {
    float dx = static_cast<float>(to[0] - from[0]);
    float dy = static_cast<float>(to[1] - from[1]);
    Vector3f point(dx, dy, 0.0f);
    translate(point, m_half_dragging_speed);
  }
  else if (is_left_mouse_down()) {
    track_ball(from, to, 0.01f);
  }

#if 0
  // Set the bounding sphere modified flag
  m_execution_coordinator->set_bounding_sphere_modified(true);
#endif
}

//! \brief issues events at the end of a transformation session.
void Navigation_sensor::transform_done()
{
  if (m_translating) {
    m_translating = false;
    m_translation_done = true;

    // update the field
    Field* field = get_field(TRANSLATION_DONE);
    if (field) field->cascade();
  }

  if (m_rotating) {
    m_rotating = false;
    m_rotation_done = true;

    // update the field
    Field* field = get_field(ROTATION_DONE);
    if (field) field->cascade();
  }
}

//! \brief unlockes the dragging at when it is done.
void Navigation_sensor::dragging_done(const Vector2sh& /* point */)
{
  transform_done();

  if (!m_drag_locked) return;

  m_drag_locked = false;
#if 0
  m_execution_coordinator->unlock_dragging();
#endif
}

//! \brief handles a track ball update.
void Navigation_sensor::track_ball(const Vector2sh& from,
                                   const Vector2sh& to, float speed)
{
  m_rotating = true;
  m_rotation_done = false;

  Context* context = m_scene_graph->get_context();

  // \todo return if the viewport is not initialized
  Uint x = 0, y = 0, w = 0, h = 0;
  context->get_viewport(x, y, w, h);
  if (w == 0) return;

  Camera* camera = context->get_active_camera();
  if (!camera) return;

  if (m_reset_rotation) {
    m_from = from;
    m_base_rotation = m_rotation;
    m_reset_rotation = false;
  }

  Vector3f p0p1;
  p0p1[0] = static_cast<float>(to[0] - m_from[0]);
  p0p1[1] = static_cast<float>(to[1] - m_from[1]);
  p0p1[2] = 0.0f;

  Vector3f rot_axis;
  rot_axis.cross(Vector3f(0, 0, 1), p0p1);

  const Matrix4f& camera_mat = camera->get_view_mat();
  Matrix4f inv_camera_mat;
  // The camera matrix consists of pure rotation (no scale or shear),
  // Therefore, transpose inverts the matrix:
  inv_camera_mat.transpose(camera_mat);

  Vector3f transformed_rot_axis;
  transformed_rot_axis.xform_vec(rot_axis, inv_camera_mat);

  // calculate the distance on the screen and from that the angle of roatation
  Vector3f p0(m_from[0], m_from[1], 0);
  Vector3f p1(to[0], to[1], 0);
  float radians = speed * sqrt(p1.sqr_distance(p0));

  // create the Rotation
  m_rotation.mult(Rotation(transformed_rot_axis[0], transformed_rot_axis[1],
                           transformed_rot_axis[2], radians),
                  m_base_rotation);

  // update the field

  Field* rot_field = get_field(ROTATION);
  if (rot_field) {
    rot_field->cascade();
#if 0
    m_execution_coordinator->set_current_view_calculation_required();
#endif
  }
  m_reset = false;
}

//! \brief handles a translation update.
void Navigation_sensor::translate(const Vector3f& distance, float speed)
{
  m_translating = true;
  m_translation_done = false;

  // get the camera's matrix
  Context* context = m_scene_graph->get_context();
  Camera* camera = context->get_active_camera();
  if (!camera) return;

  const Matrix4f& camera_mat = camera->get_view_mat();
  Matrix4f inv_camera_mat;

  // The camera matrix consists of pure rotation (no scale or shear),
  // Therefore, transpose inverts the matrix:
  inv_camera_mat.transpose(camera_mat);

  // transform the distance vector to the camera's view
  Vector3f transformed_distance;
  transformed_distance.xform_vec(distance, inv_camera_mat);

  // scale the translation by the speed factor
  Vector3f vec;
  vec.scale(speed, transformed_distance);

  Vector3f new_trans;
  new_trans = m_translation;
  new_trans.add(new_trans, vec);
  if (allow_zoom_in(new_trans)) {
    m_translation = new_trans;
    // Update the field:
    Field* trans_field = get_field(TRANSLATION);
    if (trans_field) {
      trans_field->cascade();
#if 0
      m_execution_coordinator->set_current_view_calculation_required();
#endif
    }
  }
  m_reset = false;
}

//! \brief
bool Navigation_sensor::allow_zoom_in(const Vector3f& trans)
{
  if (m_min_zoom_distance == 0) return true;

  // get the direction the camera looks in
  Vector3f view_dir(0, 0, -1);
  Context* context = m_scene_graph->get_context();
  Camera* camera = context->get_active_camera();
  const Matrix4f& camera_mat = camera->get_view_mat();
  view_dir.xform_vec(view_dir, camera_mat);

  // get the angle between the looking direction and the
  // vector connecting the camera position and the center
  // of the model
  Vector3f cam_offset = camera->get_offset();
  Vector3f cam2_model_vector = trans;
  cam2_model_vector.sub(cam_offset);
  cam2_model_vector.normalize();
  float cosa = cam2_model_vector.dot(view_dir);

  Vector3f pos = trans;
  float dist = pos.distance(cam_offset) * cosa;

  Vector3f old_cam_2p = m_translation;
  old_cam_2p.sub(cam_offset);
  old_cam_2p.normalize();
  float old_cosa = old_cam_2p.dot(view_dir);

  Vector3f old_pos = m_translation;
  float old_dist = old_pos.distance(cam_offset) * old_cosa;

  return (abs(dist) > m_min_zoom_distance  && dist * old_dist >= 0);
}

//! \brief sets the attributes of the object extracted from the input file.
void Navigation_sensor::set_attributes(Element* elem)
{
  Bindable_node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "translation") {
      Vector3f vec(value);
      set_translation(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      Rotation rot(value);
      set_rotation(rot);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Navigation_sensor::add_to_scene(Scene_graph* sg)
{
  set_scene_graph(sg);
}

#if 0
//! \brief
Attribute_list Navigation_sensor::get_attributes()
{
  Attribute_list attribs;
  attribs = Container::get_attributes();
  return attribs;
}
#endif

/*! \brief resets the navigation sensor to the default translation and
 * rotation.
 */
void Navigation_sensor::reset()
{
  // If the navigation sensor is already reset return
  if (m_reset) return;

  // Sets the rotation and translation fields to default values
  m_rotation = Rotation(0.0, 1.0, 0.0, 0.0);
  m_translation = Vector3f(0.0, 0.0, 0.0);

  Field* rot_field = get_field(ROTATION);
  if (rot_field) {
    rot_field->cascade();
#if 0
    m_execution_coordinator->set_current_view_calculation_required();
#endif
  }
  Field* trans_field = get_field(TRANSLATION);
  if (trans_field) {
    trans_field->cascade();
#if 0
    m_execution_coordinator->set_current_view_calculation_required();
#endif
  }

  // There is a left_button_double_click event.
  m_is_left_button_double_click = true;
  m_reset = true;

  // Mark the bounding sphere as modified in the execution coordinator
#if 0
  m_execution_coordinator->set_bounding_sphere_modified(true);
#endif
  set_rendering_required();
}

/*! \brief handles a double click event - returns the navigation to its default
 * translation and rotation
 * @param event (in) the event
 */
bool Navigation_sensor::left_button_double_click()
{
  reset();
  return true;
}

//! \brief
bool Navigation_sensor::is_left_button_double_click()
{
  bool double_click = m_is_left_button_double_click;
  m_is_left_button_double_click = false;
  return double_click;
}

//! \brief obtains the bindable stack.
Bindable_stack* Navigation_sensor::get_stack()
{
  return (m_scene_graph) ? m_scene_graph->get_navigation_info_stack() : nullptr;
}

//! \brief enables the bindable node.
void Navigation_sensor::enable()
{
  register_events();
  set_dragging_speed(m_scene_graph->compute_speed_factor());
}

//! \brief disabless the bindable node.
void Navigation_sensor::disable() { unregister_events(); }

#if 0
//! \brief
Window_handle* Drag_sensor::get_win_handle()
{
  Context* context = m_scene_graph->get_context();
  if (context) return context->get_win_handle();
  return 0;
}

//! \brief
Window_handle* Drag_sensor::get_win_handle() const
{
  Context* context = m_scene_graph->get_context();
  if (context) return context->get_win_handle();
  return 0;
}
#endif

SGAL_END_NAMESPACE
