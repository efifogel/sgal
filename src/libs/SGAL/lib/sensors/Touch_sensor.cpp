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
// $Revision: 12369 $
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
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Formatter.hpp"

// #include "Os.h"
// #include "Event_manager_int.h"
// #include "SGAL/Mouse_event_data.hpp"
// #include "SGAL/Cursor_data.hpp"

SGAL_BEGIN_NAMESPACE

Boolean Touch_sensor::s_def_enabled(true);

std::string Touch_sensor::s_tag = "TouchSensor";
Container_proto* Touch_sensor::s_prototype = NULL;

REGISTER_TO_FACTORY(Touch_sensor, "Touch_sensor");

/*! Constructor */
Touch_sensor::Touch_sensor(Boolean enabled, Boolean proto) :
  Node(proto),
  m_scene_graph(NULL),
  m_first_selection_id(0),
  m_num_selection_ids(1),
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
  m_drag_locked(false),
  m_routed_node(0)
{ if (!proto && m_enabled) register_events(); }

/*! Destructor */
Touch_sensor::~Touch_sensor() { if (m_enabled) unregister_events(); }

/*! \brief initializes the prototype. */
void Touch_sensor::init_prototype()
{
  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // The prototype shuold be allocated only once for all instances
  if (s_prototype) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;

  // Add the object fields to the prototype
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          get_member_offset(&m_enabled)));
  s_prototype->
    add_field_info(new SF_vector3f(HITNORMAL, "hitNormal",
                                   get_member_offset(&m_hit_normal)));

  s_prototype->add_field_info(new SF_vector3f(HITPOINT, "hitPoint",
                                              get_member_offset(&m_hit_point)));
  s_prototype->
    add_field_info(new SF_vector2f(HITTEXCOORD, "hitTexCoord",
                                   get_member_offset(&m_hit_tex_coord)));
  s_prototype->add_field_info(new SF_bool(IS_ACTIVE, "isActive",
                                          get_member_offset(&m_is_active)));

  exec_func = static_cast<Execution_function>(&Touch_sensor::external_activate);
  s_prototype->
    add_field_info(new SF_bool(EXACTIVATE, "ex_activate",
                               get_member_offset(&m_ex_activate), exec_func));

  s_prototype->add_field_info(new SF_bool(IS_OVER, "isOver",
                                          get_member_offset(&m_is_over)));

  s_prototype->add_field_info(new SF_time(TOUCH_TIME, "touchTime",
                                          get_member_offset(&m_touch_time)));
  exec_func =
    static_cast<Execution_function>(&Touch_sensor::set_rendering_required);
  SF_container* field;
  field = new SF_container(ROUTEDNODE, "enbRoutedNode",
                           get_member_offset(&m_routed_node), exec_func);
  s_prototype->add_field_info(field);

  s_prototype->
    add_field_info(new SF_int(NUMBER_OF_SELECTION_IDS, "numberOfSelectionIds",
                              get_member_offset(&m_num_selection_ids)));
  s_prototype->
    add_field_info(new SF_int(OVER_SELECTION_ID, "overSelectionId",
                              get_member_offset(&m_over_selection_id)));
  s_prototype->
    add_field_info(new SF_int(ACTIVE_SELECTION_ID, "activeSelectionId",
                              get_member_offset(&m_active_selection_id)));
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
  m_enabled = enabled;
  (enabled) ? register_events() : unregister_events();
}

/*! This function is executed when the ex_activate field is cascaded from
 * another field
 */
void Touch_sensor::external_activate(Field_info *)
{
  // Activate cascade on routed_node and touch_time only for mouse up
  if (m_ex_activate) return;

  set_rendering_required();
  
  // Cascafe the m_routed_node field
  Field* routed_node_field = get_field(ROUTEDNODE);
  if (routed_node_field) routed_node_field->cascade();

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

void Touch_sensor::set_point(const Vector3f& point)
{
  m_last_point = m_hit_point;
  m_hit_point = point;
}

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
  set_active_selection_id(m_selection_id - m_first_selection_id);
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
    Field* routed_node_field = get_field(ROUTEDNODE);
    if (routed_node_field) routed_node_field->cascade();

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
    if (name == "numberOfSelectionIds") {
      m_num_selection_ids = strtoul(value.c_str(), NULL, 10);
      elem->mark_delete(ai);
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
  m_first_selection_id = sg->reserve_selection_ids(get_num_selection_ids());
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

/*! \brief adds a touch sensor object to the scene. */
void Touch_sensor::add_to_scene(Scene_graph* sg, XML_entity* parent) 
{ 
  Node::add_to_scene(sg, parent);
  char id = sg->AddTouchSensor(this);
  Group* group = dynamic_cast<Group *>(parent);
  ASSERT(group);
  if (group) group->set_has_touch_sensor(id);
  m_scene_graph = sg;
}
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
    set_over_selection_id(m_selection_id - m_first_selection_id);
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

/*! \brief sets the routed node pointer. */
void Touch_sensor::set_routed_node(Container* node) { m_routed_node = node; }

/*! \brief gets the routed node pointer. */
Container* Touch_sensor::get_routed_node() const { return m_routed_node; }

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

SGAL_END_NAMESPACE
