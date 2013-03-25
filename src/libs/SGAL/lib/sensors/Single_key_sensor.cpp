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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Single_key_sensor - implementation
 */

#include <iostream>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Single_key_sensor.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

std::string Single_key_sensor::s_tag = "SingleKeySensor";
Container_proto* Single_key_sensor::s_prototype = NULL;

/*! Default Values */
Uint Single_key_sensor::s_def_num_states = 1;
Boolean Single_key_sensor::s_def_trigger_on_release(true);

REGISTER_TO_FACTORY(Single_key_sensor, "Single_key_sensor");

/*! A parameter-less constructor */
Single_key_sensor::Single_key_sensor(Boolean proto) :
  Node(proto),
  m_press(false),
  m_time(0.0),
  m_boolean(false),
  m_state(true),
  m_int_state(0),
  m_num_states(s_def_num_states),
  m_trigger_on_release(s_def_trigger_on_release)
{}

/*! \brief constructs a new instance. */
Container* Single_key_sensor::clone()
{
  Single_key_sensor* sensor =  new Single_key_sensor();
  sensor->register_events();
  return sensor;
}

/*! The destructor */
Single_key_sensor::~Single_key_sensor()
{ unregister_events(); }

/*! \brief initializes the node prototype. */
void Single_key_sensor::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto();

  // Add the object fields to the prototype
  s_prototype->add_field_info(new SF_bool(PRESS, "press",
                                          get_member_offset(&m_press)));
  s_prototype->add_field_info(new SF_time(TIME, "pressTime",
                                          get_member_offset(&m_time)));
  s_prototype->add_field_info(new SF_bool(STATE, "state",
                                          get_member_offset(&m_state)));
  s_prototype->add_field_info(new SF_int(INT_STATE, "intState",
                                          get_member_offset(&m_int_state)));
  s_prototype->add_field_info(new SF_uint(NUMBER_OF_STATES, "numberOfStates",
                                          get_member_offset(&m_num_states)));
}

/*! \brief deletes the node prototype. */
void Single_key_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Single_key_sensor::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief registers the keyboard event for this agent. */
void Single_key_sensor::register_events()
{ Keyboard_event::doregister(this); }

/*! \brief unregisters the keyboard event for this agent. */
void Single_key_sensor::unregister_events()
{ Keyboard_event::unregister(this); }
  
/*! \brief prints out the name of this agent (for debugging purposes). */
void Single_key_sensor::identify()
{ std::cout << "Agent: Single_key_sensor" << std::endl; }

/*! Handles keyboard events */
void Single_key_sensor::handle(Keyboard_event* event)
{
  if (event->get_key() != m_key) return;
  m_press = event->get_pressed();
  if (m_trigger_on_release && m_press) return;
  if (!m_trigger_on_release && !m_press) return;

  // m_time = m_execution_coordinator->get_scene_time();
  Field* field = get_field(PRESS);
  if (field) field->cascade();

  field = get_field(TIME);
  if (field) field->cascade();

  if (m_boolean) m_state = !m_state;
  field = get_field(STATE);
  if (field) field->cascade();

  m_int_state++;
  m_int_state %= m_num_states;
  field = get_field(INT_STATE);
  if (field) field->cascade();
}
  
/*! \brief sets the attributes of this object. */
void Single_key_sensor::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;

  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "key") {
      m_key = value[1];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "numberOfStates") {
      m_num_states = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "boolean") {
      m_boolean = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "state") {
      m_state = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "intState") {
      m_int_state = atoi(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*! Get a list of atributes in this object. This method is called only 
 * from the Builder side. 
 *
 * @return a list of attributes 
 */
Attribute_list Single_key_sensor::get_attributes()
{ 
  Attribute_list attribs; 
  Attribue attrib;
  char buf[32];

  attribs = Node::get_attributes();

  attrib.first = "key";
  sprintf(buf, "%i", m_key);
  attrib.second = buf;
  attribs.push_back(attrib);

  return attribs; 
};
#endif

SGAL_END_NAMESPACE
