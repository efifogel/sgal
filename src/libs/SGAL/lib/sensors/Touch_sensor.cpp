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

/*! The Touch_sensor tracks the location and state of the pointing device and
 * detects when the user points at geometry contained by the Touch_sensor
 * node's parent group
 */

#include <stdlib.h>
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Execution_function.hpp"

// #include "Os.h"
// #include "Event_manager_int.h"
// #include "SGAL/Mouse_event_data.hpp"
// #include "SGAL/Cursor_data.hpp"

SGAL_BEGIN_NAMESPACE

Boolean Touch_sensor::s_def_enabled(true);

const std::string Touch_sensor::s_tag = "TouchSensor";
Container_proto* Touch_sensor::s_prototype(NULL);

REGISTER_TO_FACTORY(Touch_sensor, "Touch_sensor");

/*! Constructor */
Touch_sensor::Touch_sensor(Boolean enabled, Boolean proto) :
  Node(proto),
  m_scene_graph(NULL),
  m_start_selection_id(0),
  m_num_selection_ids(0),
  m_selection_id(0),
  m_over_selection_id(0),
  m_active_selection_id(0),
  m_enabled(enabled),
  m_hit_normal(0, 0, 0),
  m_hit_point(0, 0, 0),
  m_hit_tex_coord(0, 0),
  m_is_active(false),
  m_ex_activate(false),
  m_is_over(false),
  m_touch_time(0),
  m_last_normal(0, 0, 0),
  m_last_point(0, 0, 0),
  m_last_tex_coord(0, 0),
  m_last_selection_id(0),
  m_drag_locked(false)
  // m_routed_node(NULL)
{ if (!proto && m_enabled) register_events(); }

/*! Destructor */
Touch_sensor::~Touch_sensor() { set_enabled(false); }

/*! \brief initializes the prototype. */
void Touch_sensor::init_prototype()
{
  // The prototype shuold be allocated only once for all instances.
  if (s_prototype) return;

  // Allocate a prototype instance.
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;

  // Add the object fields to the prototype
  // enabled
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Touch_sensor::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED,
                                          "enabled",
                                          RULE_EXPOSED_FIELD,
                                          enabled_func));

  // hitNormal
  Vector3f_handle_function hit_normal_func =
    static_cast<Vector3f_handle_function>(&Touch_sensor::hit_normal_handle);
  s_prototype->add_field_info(new SF_vector3f(HITNORMAL,
                                              "hitNormal",
                                              RULE_EXPOSED_FIELD,
                                              hit_normal_func));

  // hitPoint
  Vector3f_handle_function hit_point_func =
    static_cast<Vector3f_handle_function>(&Touch_sensor::hit_point_handle);
  s_prototype->add_field_info(new SF_vector3f(HITPOINT,
                                              "hitPoint",
                                              RULE_EXPOSED_FIELD,
                                              hit_point_func));

  // hitTexCoord
  Vector2f_handle_function hit_tex_coord_func =
    static_cast<Vector2f_handle_function>(&Touch_sensor::hit_tex_coord_handle);
  s_prototype->add_field_info(new SF_vector2f(HITTEXCOORD,
                                              "hitTexCoord",
                                              RULE_EXPOSED_FIELD,
                                              hit_tex_coord_func));

  // isActive
  Boolean_handle_function is_active_func =
    static_cast<Boolean_handle_function>(&Touch_sensor::is_active_handle);
  s_prototype->add_field_info(new SF_bool(IS_ACTIVE,
                                          "isActive",
                                          RULE_EXPOSED_FIELD,
                                          is_active_func));

  // exActivate
  exec_func = static_cast<Execution_function>(&Touch_sensor::external_activate);
  Boolean_handle_function ex_activate_func =
    static_cast<Boolean_handle_function>(&Touch_sensor::ex_activate_handle);
  s_prototype->add_field_info(new SF_bool(EXACTIVATE,
                                          "exActivate",
                                          RULE_EXPOSED_FIELD,
                                          ex_activate_func, exec_func));

  // isOver
  Boolean_handle_function is_over_func =
    static_cast<Boolean_handle_function>(&Touch_sensor::is_over_handle);
  s_prototype->add_field_info(new SF_bool(IS_OVER,
                                          "isOver",
                                          RULE_EXPOSED_FIELD,
                                          is_over_func));

  // touchTime
  Scene_time_handle_function touch_time_func =
    static_cast<Scene_time_handle_function>(&Touch_sensor::touch_time_handle);
  s_prototype->add_field_info(new SF_time(TOUCH_TIME,
                                          "touchTime",
                                          RULE_EXPOSED_FIELD,
                                          touch_time_func));

  // overSelectionId
  Uint_handle_function over_selection_id_func =
    static_cast<Uint_handle_function>(&Touch_sensor::over_selection_id_handle);
  s_prototype->add_field_info(new SF_uint(OVER_SELECTION_ID,
                                          "overSelectionId",
                                          RULE_EXPOSED_FIELD,
                                          over_selection_id_func));

  // activeSelectionId
  Uint_handle_function active_selection_id_func =
    static_cast<Uint_handle_function>
    (&Touch_sensor::active_selection_id_handle);
  s_prototype->add_field_info(new SF_uint(ACTIVE_SELECTION_ID,
                                          "activeSelectionId",
                                          RULE_EXPOSED_FIELD,
                                          active_selection_id_func));

  // RoutedNode
  // exec_func =
  //   static_cast<Execution_function>(&Touch_sensor::set_rendering_required);
  // SF_container* field;
  // s_prototype->add_field_info(new SF_container(ROUTED_NODE, "RoutedNode",
  //                                              routed_node_func, exec_func));
}

/*! \brief deletes the prototype. */
void Touch_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Touch_sensor::get_prototype()
{
  if (!s_prototype) Touch_sensor::init_prototype();
  return s_prototype;
}

/*! \brief enables or disables the sensor. */
void Touch_sensor::set_enabled(bool enabled)
{
  if (enabled == m_enabled) return;
  m_enabled = enabled;
  (enabled) ? register_events() : unregister_events();
}

/*! \brief this function is executed when the ex_activate field is cascaded from
 * another field
 */
void Touch_sensor::external_activate(const Field_info *)
{
  // Activate cascade on routed_node and touch_time only for mouse up
  if (m_ex_activate) return;

  set_rendering_required();

  // Cascafe the m_routed_node field
  // Field* routed_node_field = get_field(ROUTEDNODE);
  // if (routed_node_field) routed_node_field->cascade();

  // Update and cascade the m_touch_time field
  //!\todo m_touch_time = m_execution_coordinator->get_scene_time();
  Field* is_touch_time_field = get_field(TOUCH_TIME);
  if (is_touch_time_field) is_touch_time_field->cascade();
}

/*! \brief */
void Touch_sensor::set_normal(const Vector3f& normal)
{
  m_last_normal = m_hit_normal;
  m_hit_normal = normal;
}

/*! \brief */
void Touch_sensor::set_point(const Vector3f& point)
{
  m_last_point = m_hit_point;
  m_hit_point = point;
}

/*! \brief */
void Touch_sensor::set_tex_coord(const Vector2f& tex_coord)
{
  m_last_tex_coord = m_hit_tex_coord;
  m_hit_tex_coord = tex_coord;
}

/*! \brief sets the flag that indicates whether the cursor hoovers above a
 * selected geometry.
 */
void Touch_sensor::set_is_over(const Boolean over)
{
  m_is_over = over;
  Field* field = get_field(IS_OVER);
  if (field) field->cascade();
}

/*! \brief sets the flag that indicates whether the mouse is pressed when the
 * cursor is above a selected geometry.
 */
void Touch_sensor::set_is_active(const Boolean active)
{
  m_is_active = active;
  Field* field = get_field(IS_ACTIVE);
  if (field) field->cascade();
}

/*! \brief invoked when dragging starts. */
void Touch_sensor::start_dragging(const Vector2sh& /* point */)
{
  if (!m_enabled) return;

  // if the mouse is not over the sensor geometry - return
  if (m_selection_id == 0) return;

  // if dragging is locked by another sensor - return
  //! \todo if (m_execution_coordinator->is_dragging_locked()) return;

  // lock the dragging for this sensor instance
  //! \todo m_execution_coordinator->lock_draging();
  m_drag_locked = true;

  // Update and cascade the m_is_active field
  set_is_active(true);
  set_active_selection_id(m_selection_id - m_start_selection_id);
}

/*! \brief invoked when dragging stops. */
void Touch_sensor::dragging_done(const Vector2sh& point)
{
  if (!m_enabled) return;

  // If the dragging is not locked for this sensor - return
  if (!m_drag_locked) return;

  // Patch: When mouse is released not over the clicked
  //        object the animation is NOT activated
  set_is_over(false);
  if (m_scene_graph) m_scene_graph->isect(point[0], point[1]);
  /*! \todo the function set_cursor was replaced by handle passive events
   * set_cursor(Cursor_data());
   */
  // End of patch

  if (m_last_selection_id > 0) {
    // Update and cascade the m_is_active field
    set_is_active(false);

    // Cascafe the m_routed_node field
    // Field* routed_node_field = get_field(ROUTEDNODE);
    // if (routed_node_field) routed_node_field->cascade();

    // Update and cascade the m_touch_time field
    //! \todo m_touch_time = m_execution_coordinator->get_scene_time();
    Field* is_touch_time_field = get_field(TOUCH_TIME);
    if (is_touch_time_field) is_touch_time_field->cascade();
  }
  /*! \todo
   * else if (!m_execution_coordinator->is_mouse_over())
   * m_execution_coordinator->unlock_scope();
   */
  // Unlock the dragging

  m_drag_locked = false;
  //! \todo m_execution_coordinator->unlock_dragging();
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Touch_sensor::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      set_enabled(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the touch sensor to a given scene. */
void Touch_sensor::add_to_scene(Scene_graph* sg)
{
  m_scene_graph = sg;
  sg->add_touch_sensor(this);
}

/*! \brief writes this container. */
void Touch_sensor::write(Formatter* formatter)
{
  formatter->container_begin(get_tag());
  formatter->single_boolean("enabled", m_enabled, s_def_enabled);
  formatter->container_end();
}

#if 0
/*! \brief obtains the list of atributes of this object. */
Attribute_list Touch_sensor::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  if (m_enabled != s_def_enabled) {
    attrib.first = "enabled";
    attrib.second = (m_enabled) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  return attribs;
};
#endif

/*! \brief registers the mouse and mostion events. */
void Touch_sensor::register_events()
{
  Mouse_event::doregister(this);
  Motion_event::doregister(this);
  Passive_motion_event::doregister(this);
  Tick_event::doregister(this);
}

/*! registers the mouse and mostion events. */
void Touch_sensor::unregister_events()
{
  Mouse_event::unregister(this);
  Motion_event::unregister(this);
  Passive_motion_event::unregister(this);
  Tick_event::unregister(this);
}

/*! \brief handles mouse events. */
void Touch_sensor::handle(Mouse_event* event)
{
  switch (event->get_button()) {
   case Mouse_event::LEFT_BUTTON:
    if (event->get_state() ==  Mouse_event::DOWN)
      left_button_down(event->get_x(), event->get_y());
    else
      left_button_up(event->get_x(), event->get_y());
    break;

   case Mouse_event::MIDDLE_BUTTON:
    // if (event->get_state() ==  Mouse_event::DOWN) {
    // } else {
    // }
    break;

   case Mouse_event::RIGHT_BUTTON:
    if (event->get_state() ==  Mouse_event::DOWN)
      right_button_down(event->get_x(), event->get_y());
    else
      right_button_up(event->get_x(), event->get_y());
    break;
  }
}

/*! \brief handles motion events. */
void Touch_sensor::handle(Motion_event* event)
{ mouse_move(event->get_x(), event->get_y()); }

/*! \brief handles passive motion events.
 * Updates and cascades the m_is_over field if needed.
 */
void Touch_sensor::handle(Passive_motion_event* event)
{
  if (!m_enabled) return;

  if (m_scene_graph) m_scene_graph->isect(event->get_x(), event->get_y());

  // \todo If dragging is locked by another sensor return
  // if (m_execution_coordinator->is_dragging_locked() && (!m_drag_locked))

  // If selection changed, update and cascade the appropriate fields:
  if (m_last_selection_id != m_selection_id) {
    set_is_over(m_selection_id != 0);
    set_over_selection_id(m_selection_id - m_start_selection_id);
    m_last_selection_id = m_selection_id;
  }

#if 0
  //! \todo
  // Change the cursor type only if the mouse is not over any other
  // touch sensor
  if (!m_execution_coordinator->is_mouse_over()) {
    if (m_is_over) m_execution_coordinator->lock_scope();
    else if (!is_left_mouse_down ()) m_execution_coordinator->unlock_scope();

    if (m_is_over)
      m_execution_coordinator->set_mouse_over();
    // If the cursor is over this sensor's geometry - change the cursor type
    if (m_is_over && (is_left_mouse_down () || is_right_mouse_down ()))
      m_execution_coordinator->set_cursor_type(ctGRABHAND);
    else if (m_is_over)
      m_execution_coordinator->set_cursor_type(ctHAND);
    else
      m_execution_coordinator->set_cursor_type(ctARROW);
  }
#endif
}

/*! \brief prints an identification message. */
void Touch_sensor::identify()
{ std::cout << "Agent: Touch_sensor" << std::endl; }

#if 0
/*! \brief */
bool Touch_sensor::update_cursor(const Mouse_event_data& data)
{
  if (!m_enabled) return true;

  // Change the cursor type only if the mouse is not over any other
  // touch sensor
  if (!m_execution_coordinator->is_mouse_over()) {
    if (m_last_selection_id > 0)
      // If the cursor is over this sensor's geometry - change the cursor type
      m_execution_coordinator->set_mouse_over();
    if ((m_last_selection_id > 0) &&
        (data.is_left_button_down() || data.is_right_button_down()))
      m_execution_coordinator->set_cursor_type(ctGRABHAND);
    else if (m_last_selection_id > 0)
      m_execution_coordinator->set_cursor_type(ctHAND);
    else
      m_execution_coordinator->set_cursor_type(ctARROW);
  }

  return true;
}

/*! \brief */
bool Touch_sensor::update_cursor(const Mouse_wheel_data& data)
{
  return true;
}

/*! \brief */
bool Touch_sensor::update_cursor(const Key_event_data& data)
{
  if (!m_enabled) return true;

  // Change the cursor type only if the mouse is not over any other
  // touch sensor
  if (!m_execution_coordinator->is_mouse_over()) {
    if (m_last_selection_id > 0)
      // If the cursor is over this sensor's geometry - change the cursor type
      m_execution_coordinator->set_mouse_over();
    if ((m_last_selection_id > 0) &&
        (is_left_mouse_down() || is_right_mouse_down()))
      m_execution_coordinator->set_cursor_type(ctGRABHAND);
    else if (m_last_selection_id > 0)
      m_execution_coordinator->set_cursor_type(ctHAND);
    else
      m_execution_coordinator->set_cursor_type(ctARROW);
  }

  return true;
}
#endif

/*! \brief handles tick events. */
void Touch_sensor::handle(Tick_event* event)
{
  clock_t sim_time = event->get_sim_time();
  m_touch_time = (Scene_time) sim_time / CLOCKS_PER_SEC;
}

/*! \brief sets the (normalized) id of the geometry, which the cursor is
 * hoovering above.
 */
void Touch_sensor::set_over_selection_id(Uint id)
{
  m_over_selection_id = id;
  Field* field = get_field(OVER_SELECTION_ID);
  if (field) field->cascade();
}

/*! \brief sets the (normalized) id of the geometry, which the cursor was above
 * when dragging started.
 */
inline void Touch_sensor::set_active_selection_id(Uint id)
{
  m_active_selection_id = id;
  Field* field = get_field(ACTIVE_SELECTION_ID);
  if (field) field->cascade();
}

/*! \brief determines whether the given id in the range of color ids. */
Boolean Touch_sensor::is_in_range(Uint id)
{
  return ((m_start_selection_id <= id) &&
          (id < (m_start_selection_id + m_num_selection_ids)));
}

SGAL_END_NAMESPACE
