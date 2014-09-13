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

#include "SGAL/Utilities.hpp"
#include "SGAL/Key_sensor.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

std::string Key_sensor::s_tag = "KeySensor";
Container_proto* Key_sensor::s_prototype(nullptr);

REGISTER_TO_FACTORY(Key_sensor, "Key_sensor");

#define SHIFT_CODE 16
#define CTRL_CODE 17

//! \brief constructor.
Key_sensor::Key_sensor(Boolean proto) :
  Node(proto),
  m_scene_graph(nullptr),
  m_enabled(true),
  m_active(false),
  m_shift_key(false),
  m_control_key(false),
  m_alt_key(false),
  m_pressed(false),
  m_key(0),
  m_action_key(0)
{}

//! \brief initializes the node prototype.
void Key_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  // keyPress
  Uint_handle_function key_func =
    static_cast<Uint_handle_function>(&Key_sensor::key_handle);
  s_prototype->add_field_info(new SF_uint(KEY_PRESS, "keyPress",
                                          RULE_EXPOSED_FIELD,
                                          key_func));

  // keyRelease
  // Int_handle_function key_func =
  //   static_cast<Int_handle_function>(&Key_sensor::key_handle);
  s_prototype->add_field_info(new SF_uint(KEY_RELEASE, "keyRelease",
                                          RULE_EXPOSED_FIELD,
                                          key_func));

  // actionKeyPress
  Uint_handle_function action_key_func =
    static_cast<Uint_handle_function>(&Key_sensor::action_key_handle);
  s_prototype->add_field_info(new SF_uint(ACTION_KEY_PRESS,
                                          "actionKeyPress",
                                          RULE_EXPOSED_FIELD,
                                          action_key_func));

  // actionKeyRelease
  // Int_handle_function action_key_func =
  //   static_cast<Int_handle_function>(&Key_sensor::action_key_handle);
  s_prototype->add_field_info(new SF_uint(ACTION_KEY_RELEASE,
                                          "actionKeyRelease",
                                          RULE_EXPOSED_FIELD,
                                          action_key_func));

  // shiftKey
  Boolean_handle_function shift_key_func =
    static_cast<Boolean_handle_function>(&Key_sensor::shift_key_handle);
  s_prototype->add_field_info(new SF_bool(SHIFT_KEY, "shiftKey",
                                          RULE_EXPOSED_FIELD,
                                          shift_key_func));

  // controlKey
  Boolean_handle_function control_key_func =
    static_cast<Boolean_handle_function>(&Key_sensor::control_key_handle);
  s_prototype->add_field_info(new SF_bool(CONTROL_KEY, "controlKey",
                                          RULE_EXPOSED_FIELD,
                                          control_key_func));

  // altKey
  Boolean_handle_function alt_key_func =
    static_cast<Boolean_handle_function>(&Key_sensor::alt_key_handle);
  s_prototype->add_field_info(new SF_bool(ALT_KEY, "altKey",
                                          RULE_EXPOSED_FIELD,
                                          alt_key_func));

  // isActive
  Execution_function exec_func =
    static_cast<Execution_function>(&Key_sensor::activate);
  Boolean_handle_function active_func =
    static_cast<Boolean_handle_function>(&Key_sensor::active_handle);
  s_prototype->add_field_info(new SF_bool(IS_ACTIVE, "isActive",
                                          RULE_EXPOSED_FIELD,
                                          active_func,
                                          exec_func));
}

//!
void Key_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//!
Container_proto* Key_sensor::get_prototype()
{
  if (!s_prototype) Key_sensor::init_prototype();
  return s_prototype;
}

//! Registers the mouse and mostion events.
void Key_sensor::register_events() { Keyboard_event::doregister(this); }

//! Registers the mouse and mostion events.
void Key_sensor::unregister_events() { Keyboard_event::unregister(this); }

//! Prints out the name of this agent (for debugging purposes).
void Key_sensor::identify()
{ std::cout << "Agent: Key_sensor" << std::endl; }

//! Handles keyboard events.
void Key_sensor::handle(Keyboard_event* event)
{
  m_key = event->get_key();
  m_pressed = event->get_pressed();
  Field* field = (m_pressed) ? get_field(KEY_PRESS) : get_field(KEY_RELEASE);
  if (field) field->cascade();
}

/*! Makes the key sensor to be the active key sensor (also makes the current
 * active key sensor inactive).
 * At most one KeySensor can be active at any given time.
 */
void Key_sensor::activate(const Field_info* /* field_info */)
{
  // Get the current active key-sensor, and set this as the new active one:
  //! \todo use the execution coordinator instead of the scene graph
  Key_sensor* current = m_scene_graph->get_active_key_sensor();
  m_scene_graph->set_active_key_sensor(this);
  // Inactivate the current key sensor
  if (current) {
    current->set_active(false);

    // Cascade the isActive field on current key sensor if needed
    Field* current_field = current->get_field(IS_ACTIVE);
    if (current_field) current_field->cascade();
  }
  set_active(true);                      // mark as active

  // Cascade the isActive field if needed
  Field* field = get_field(IS_ACTIVE);
  if (field) field->cascade();
}

#if 0
/*! Invoked when a key is pressed
 * @param event (in) the identifier of the pressed key
 */
bool Key_sensor::Key_down()
{
  UShort key_code = event.Code();
  Field* field;

  // Set the suitable member with the pressed key info and get the suitable
  // field
  if (key_code == SHIFT_CODE) {
    m_shift_key = true;
    field = get_field(SHIFTKEY);
  }
  else if (key_code == CTRL_CODE) {
    m_control_key = true;
    field = get_field(CONTROLKEY);
  }
  else {
    m_key = (Int)key_code;
    field = get_field(KEY_RELEASE);
  }

  // Cascade the field if exists
  if (field) field->cascade();
}

/*! Invoked when a key is released
 * @param event (in) the identifier of the pressed key
 */
void Key_sensor::Key_up()
{
  Field* field;

  // Set the suitable member with the pressed key info and get the suitable
  // field
  if (key_code == SHIFT_CODE) {
    m_shift_key = false;
    field = get_field(SHIFTKEY);
  }
  else if (key_code == CTRL_CODE) {
    m_control_key = false;
    field = get_field(CONTROLKEY);
  }
  else {
    m_key = (Int)key_code;
    field = get_field(KEY_PRESS);
  }

  // Cascade the field if exists
  if (field) field->cascade();
}
#endif

//! \brief sets the attributes of this container.
void Key_sensor::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      m_enabled = compare_to_true(value);
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Key_sensor::add_to_scene(Scene_graph * sg)
{
  m_scene_graph = sg;
  if (m_enabled) sg->set_active_key_sensor(this);
}

#if 0
/*! Get a list of atributes in this object. This method is called only
 * from the Builder side.
 *
 * @return a list of attributes
 */
Attribute_list Key_sensor::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  attrib.first = "isActive";
  attrib.second = (m_active) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
};

/*! \brief adds this key sensor to the scene graph.
 * Also set the execution coordinator to point at this one as active if needed.
 * @param parent (in) pointer to the parent node
 */
void Key_sensor::add_to_scene(Scene_graph* sg)
{
  Node::add_to_scene(sg, parent);

  // If this key sensor is marked active and there is no active key
  // sensor pointed by the execution coordinator already - set
  // the execution coordinator to point to this:
  if (m_active && m_execution_coordinator->get_active_key_sensor() == nullptr)
    // Set this as the active key sensor
    m_execution_coordinator->set_active_key_sensor(this);
  else
    // if there is another key sensor already active - deactivate this one
    m_active = false;
}
#endif

/*! Enactivate or disactivate this key sensor */
void Key_sensor::set_active(Boolean active)
{
  m_active = active;
  if (active) register_events();
  else unregister_events();
  m_pressed = false;
}

SGAL_END_NAMESPACE
