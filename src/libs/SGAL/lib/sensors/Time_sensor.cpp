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
// $Source: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Time_sensor nodes generate events as time passes. Time_sensor nodes can
 * be used for many purposes including:
 * 1. driving continuous simulations and animations;
 * 2. controlling periodic activities (e.g., one per minute);
 * 3. initiating single occurrence events such as an alarm clock. 
 */

#include "SGAL/Time_sensor.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Tick_event.hpp"

SGAL_BEGIN_NAMESPACE

std::string Time_sensor::s_tag = "sgalTimeSensor";
Container_proto * Time_sensor::s_prototype = 0;

// Default values:
Scene_time Time_sensor::s_def_cycle_interval = 1;
Boolean Time_sensor::s_def_enabled = SGAL_TRUE;
Boolean Time_sensor::s_def_loop = SGAL_FALSE;
Scene_time Time_sensor::s_def_start_time = 0;
Scene_time Time_sensor::s_def_stop_time = 0;
Float Time_sensor::s_def_fraction_bias = 0.0;
Float Time_sensor::s_def_fraction_scale = 1.0;

REGISTER_TO_FACTORY(Time_sensor, "Time_sensor");

/*! Constructor */
Time_sensor::Time_sensor(Boolean proto) :
  Node(proto),
  m_cycle_interval(s_def_cycle_interval),
  m_frequency(0),
  m_enabled(s_def_enabled),
  m_loop(s_def_loop),
  m_start_time(s_def_start_time),
  m_stop_time(s_def_stop_time),
  m_cycle_time(0),
  m_fraction(0),
  m_true_fraction(SGAL_FALSE),
  m_fraction_bias(s_def_fraction_bias),
  m_fraction_scale(s_def_fraction_scale),
  m_is_active(SGAL_FALSE),
  m_time(-SGAL_EPSILON),
  m_time_in_cycle(-1000.0),     // negative number indicates no cycle has
                                // started yet
  m_start(0),
  m_stop(0),
  m_end_of_cycle(SGAL_FALSE),
  m_first_update(SGAL_TRUE),
  m_quit_time(0)
{
  if (!proto && m_enabled) register_events();
}

/*! Destructor */
Time_sensor::~Time_sensor()
{
  if (m_enabled) unregister_events();
}

/*! initializes the node prototype */
void Time_sensor::init_prototype()
{
  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;
  
  // cycleInterval
  SF_time * ci = new SF_time(CYCLE_INTERVAL, "cycleInterval",
                             get_member_offset(&m_cycle_interval));
  s_prototype->add_field_info(ci);

  // frequency
  s_prototype->add_field_info(new SF_int(FREQUENCY, "frequency",
                                         get_member_offset(&m_frequency)));

  // enabled
  exec_func = static_cast<Execution_function>(&Time_sensor::execute_enabled);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          get_member_offset(&m_enabled),
                                          exec_func));

  // loop
  s_prototype->add_field_info(new SF_bool(LOOP, "loop",
                                          get_member_offset(&m_loop)));

  // startTime
  SF_time * st = new SF_time(START_TIME, "startTime",
                             get_member_offset(&m_start_time), NULL, SGAL_TRUE);
  s_prototype->add_field_info(st);

  // startTime is initially blocked until the first UpdateTime. So Animations
  // won't start in the middle, if the player was not ready to activate them
  // when activated
  s_prototype->add_field_info(new SF_time(STOP_TIME, "stopTime",
                                          get_member_offset(&m_stop_time)));
  s_prototype->add_field_info(new SF_time(CYCLE_TIME, "cycleTime",
                                          get_member_offset(&m_cycle_time)));
  s_prototype->add_field_info(new SF_float(FRACTION, "fraction_changed",
                                           get_member_offset(&m_fraction)));

  // trueFractionChanged
  SF_bool * tfc = new SF_bool(TRUE_FRACTION, "trueFractionChanged",
                              get_member_offset(&m_true_fraction));
  s_prototype->add_field_info(tfc);

  // sgalFractionBias
  SF_float * fb = new SF_float(FRACTION_BIAS, "fractionBias",
                               get_member_offset(&m_fraction_bias));
  s_prototype->add_field_info(fb);

  // sgalFractionScale
  SF_float * fs = new SF_float(FRACTION_SCALE, "fractionScale",
                               get_member_offset(&m_fraction_scale));
  s_prototype->add_field_info(fs);
  
  s_prototype->add_field_info(new SF_bool(IS_ACTIVE, "isActive",
                                          get_member_offset(&m_is_active)));
  s_prototype->add_field_info(new SF_time(TIME, "time",
                                          get_member_offset(&m_time)));

  exec_func =
    static_cast<Execution_function>(&Time_sensor::start);
  s_prototype->add_field_info(new SF_time(START, "start",
                                          get_member_offset(&m_start),
                                          exec_func, true));
  // startTime is initially blocked until the first UpdateTime. So Animations
  // won't start in the middle if the player was not ready to activate them
  // when activated
  exec_func = static_cast<Execution_function>(&Time_sensor::stop);
  s_prototype->add_field_info(new SF_time(STOP, "stop",
                                          get_member_offset(&m_stop),
                                          exec_func));
}

/*! */
void Time_sensor::delete_prototype() { delete s_prototype; }

/*! */
Container_proto * Time_sensor::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
} 

/*! Activated by a cascade on the enabled field.
 * Updates m_is_active and cascade it if needed
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::execute_enabled(Field_info *)
{
  //! \todo Fix the response fumction of the Time_sensor::enabled field
#if 0
  // If the sensor was just disabled - enable it for the last time
  // to send all events before disabling
  if (!m_enabled) {
    set_enabled(SGAL_TRUE);
    update_time();
    set_enabled(SGAL_FALSE);
    m_is_active = false;
    Field * is_active_field = get_field(IS_ACTIVE);
    if (is_active_field != NULL) is_active_field->cascade();
  }
  // If the sensor was enabled - zero the time in cycle
  else {
    m_time_in_cycle = 0.0;
  }
#endif
};

/*! Start the animation by setting m_start_time = m_time + m_start.
 * Used to start the animation in m_start value in seconds
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::start(Field_info *)
{
  m_start_time = m_time + m_start;
  Field * field = get_field(START_TIME);
  if (field != NULL) field->cascade();
}

/*! Stop the animation by setting m_stop_time = m_time + m_stop.
 * Used to stop the animation in m_stop value in seconds
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::stop(Field_info *)
{
  m_stop_time = m_time + m_stop;
  Field * field = get_field(STOP_TIME);
  if (field != NULL) field->cascade();
}

/*! Blocks/Unblocks the startTime, start and cycleInterval fields 
 * (which need to be blocked when the sensor is active.
 */
void Time_sensor::set_blocking_of_fields_for_active(Boolean block)
{
  Field * field = add_field(START_TIME);
  if (field != NULL) field->set_blocked(block);
  field = add_field(START);
  if (field != NULL) field->set_blocked(block);
  field = add_field(CYCLE_INTERVAL);
  if (field != NULL) field->set_blocked(block);
  field = add_field(FRACTION);
  if (field != NULL) field->set_blocked(block);
}

/*! \brief invoked every tick while the sensor is enabled.
 * Calculates and cascades the time sensor time dependent events.
 */
bool Time_sensor::update_time(Scene_time current_time)
{
  // Unblock the start fields which are initially blocked - if this is the
  // first call to UpadteTime
  if (m_first_update) {
    m_first_update = false;
    set_blocking_of_fields_for_active(false);
  }
  
  // Calculate the delta time since the last time
  Scene_time delta_time = current_time - m_time;

  // Set the time member
  m_time = current_time;

  // If the time sensor is not enabled return
  if (!m_enabled) return true;
  
  // Check if the sensor is active according to its start time, stop time,
  // loop flag, and cycle interval:
  Boolean is_active =
    ((m_start_time <= current_time) &&
     ((m_start_time >= m_stop_time) ||
      ((m_start_time < m_stop_time) && (current_time < m_stop_time))) &&
     (m_loop || ((current_time < (m_start_time + m_cycle_interval))  &&
                 ((m_start_time != s_def_start_time) || (m_frequency != 0)))));
  
  // If the new isActive is different than the old get its field
  // (to cascade it later)
  Field * is_active_field = NULL;

  if (m_is_active != is_active) {
    m_is_active = is_active;
    is_active_field = get_field(IS_ACTIVE);
    // if the sensor was just activated reset the timeInCycle
    if (is_active) m_time_in_cycle = -1;

    // Block or Unblock the startTime and cycleInterval fields 
    // when the sensor is active ot not active
    set_blocking_of_fields_for_active(is_active);
  }

  // if the sensor is not active - return
  if (!is_active) {
    if (is_active_field != NULL) is_active_field->cascade();
    return true;
  }
  
  // Update the time in the cycle - if cycle ended (or if its the
  // first cycle indicated by a negative number) start from 0.0
  if ((m_end_of_cycle && m_loop) || m_time_in_cycle < 0) {
    // if the cycle is starting from startTime in the past - 
    // start from currentTime - m_start_time
    if ((current_time - m_start_time) < m_cycle_interval)
      m_time_in_cycle = current_time - m_start_time;
    // else start from 0
    else
      m_time_in_cycle = 0;

    m_end_of_cycle = false;
  }
  else
    m_time_in_cycle = m_time_in_cycle + delta_time;

  // If the cycle is over and loop is false return
  if (m_end_of_cycle && !m_loop) return true;

  // Initialize cycle_time_field pointer with NULL 
  Field * cycle_time_field = NULL;

  // Check if cycle ended:
  if (m_time_in_cycle > m_cycle_interval) {
    m_time_in_cycle = m_cycle_interval;
    m_end_of_cycle = true;
  }

  // Set m_cycleTime if it is the begining of a cycle
  if (m_time_in_cycle == 0) {
    m_cycle_time = current_time;
    cycle_time_field = get_field(CYCLE_TIME);
  }

  // Update fraction
  m_fraction = static_cast<Float>(m_time_in_cycle / m_cycle_interval);
  
  // Use the fraction addition and factor to calculate the final fraction
  m_fraction = m_fraction_bias + (m_fraction * m_fraction_scale);

  // If the fraction is not between 0-1 loop it to be
  if (m_loop) {
    if (m_fraction > 1) m_fraction -= 1;
    if (m_fraction < 0) m_fraction += 1;
  }
  
  m_true_fraction = SGAL_TRUE;
  
  // Cascade changed fields
  if (is_active_field != NULL) is_active_field->cascade();
  if (cycle_time_field != NULL) cycle_time_field->cascade();

  Field * fraction_field = get_field(FRACTION);
  if (fraction_field != NULL) fraction_field->cascade();
  Field * true_fraction_field = get_field(TRUE_FRACTION);
  if (true_fraction_field != NULL) true_fraction_field->cascade();
  Field * time_field = get_field(TIME);
  if (time_field != NULL) time_field->cascade();

  return true;
};

/*! \brief sets the attributes of the object extracted from an input file */
void Time_sensor::set_attributes(Element * elem) 
{ 
  Node::set_attributes(elem);
  
  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "cycleInterval") {
      m_cycle_interval = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "frequency") {
      m_frequency = atoi(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "enabled") {
      set_enabled(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "loop") {
      m_loop = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "startTime") {
      m_start_time = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stopTime") {
      m_stop_time = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fractionBias") {
      m_fraction_bias = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fractionScale") {
      m_fraction_scale = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quitTime") {
      m_quit_time = atoff(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */  
void Time_sensor::add_to_scene(Scene_graph * sg)
{
  sg->add_time_sensor(this);
}

#if 0
/*!
 * Get a list of atributes in this object. This method is called only 
 * from the Builder side. 
 *
 * @return a list of attributes 
 */
Attribute_list Time_sensor::get_attributes()
{ 
  Attribute_list attribs; 
  Attribue attrib;
  char buf[32];

  attribs = Node::get_attributes();

  if (m_cycle_interval != s_def_cycle_interval) {
    attrib.first = "cycleInterval";
    sprintf(buf, "%g", m_cycle_interval);
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_start_time != s_def_start_time) {
    attrib.first = "startTime";
    sprintf(buf, "%g", m_start_time);
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_stop_time != s_def_stop_time) {
    attrib.first = "stopTime";
    sprintf(buf, "%g", m_stop_time);
    attrib.second = buf;
    attribs.push_back(attrib);
  } 

  if (m_fraction_bias != s_def_fraction_bias) {
    attrib.first = "sgalFractionBias";
    sprintf(buf, "%g", m_fraction_bias);
    attrib.second = buf;
    attribs.push_back(attrib);
  } 

  if (m_fraction_scale != s_def_fraction_scale) {
    attrib.first = "sgalFractionScale";
    sprintf(buf, "%g", m_fraction_scale);
    attrib.second = buf;
    attribs.push_back(attrib);
  }   

  if (m_enabled != s_def_enabled) {
    attrib.first = "enabled";
    attrib.second = (m_enabled) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  if (m_loop != s_def_loop) {
    attrib.first = "loop";
    attrib.second = (m_loop) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  return attribs; 
};


void Time_sensor::AddToScene(Scene_graph * sg, XML_entity * parent)
{
  Node::AddToScene(sg, parent);
}
#endif

/*! Enable or disable the sensor
 */
void Time_sensor::set_enabled(Boolean enabled)
{
  if (enabled == m_enabled) return;
  m_enabled = enabled;
  (enabled) ? register_events() : unregister_events();
}

/*! Register the mouse and mostion events
 */
void Time_sensor::register_events()
{
  Tick_event::doregister(this);
}

/*! Register the mouse and mostion events
 */
void Time_sensor::unregister_events()
{
  Tick_event::unregister(this);
}
  
/*! Print out the name of this agent (for debugging purposes)
 */
void Time_sensor::identify()
{
  std::cout << "Agent: Time_sensor" << std::endl;
}

/*! Handle tick events */
void Time_sensor::handle(Tick_event * event)
{
  clock_t sim_time = event->get_sim_time();
  Scene_time t = (m_frequency) ?
    m_time + 1.0 / m_frequency : (Scene_time) sim_time / CLOCKS_PER_SEC;
  update_time(t);

  if (m_quit_time != 0  && t >= m_quit_time) exit(0);
}

SGAL_END_NAMESPACE
