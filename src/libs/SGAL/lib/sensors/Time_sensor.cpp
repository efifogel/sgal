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

/*! \file
 * Time_sensor nodes generate events as time passes. Time_sensor nodes can
 * be used for many purposes including:
 * 1. driving continuous simulations and animations;
 * 2. controlling periodic activities (e.g., one per minute);
 * 3. initiating single occurrence events such as an alarm clock.
 */

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Time_sensor.hpp"
#include "SGAL/Field_rule.hpp"
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

std::string Time_sensor::s_tag = "TimeSensor";
Container_proto* Time_sensor::s_prototype(nullptr);

// Default values:
Scene_time Time_sensor::s_def_cycle_interval = 1;
Boolean Time_sensor::s_def_enabled = true;
Boolean Time_sensor::s_def_loop = false;
Scene_time Time_sensor::s_def_start_time = 0;
Scene_time Time_sensor::s_def_stop_time = 0;
Float Time_sensor::s_def_fraction_bias = 0.0;
Float Time_sensor::s_def_fraction_scale = 1.0;

REGISTER_TO_FACTORY(Time_sensor, "Time_sensor");

//! \brief constructor
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
  m_true_fraction(false),
  m_fraction_bias(s_def_fraction_bias),
  m_fraction_scale(s_def_fraction_scale),
  m_is_active(false),
  m_time(-SGAL_EPSILON),
  m_time_in_cycle(-1000.0),     // negative number indicates no cycle has
                                // started yet
  m_start(0),
  m_stop(0),
  m_end_of_cycle(false),
  m_first_update(true),
  m_quit_time(0)
{ if (!proto && m_enabled) register_events(); }

//! \brief destructor
Time_sensor::~Time_sensor() { if (m_enabled) unregister_events(); }

//! \brief initializes the node prototype.
void Time_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  Execution_function exec_func;

  // Add the field-info records to the prototype:
  // cycleInterval
  Scene_time_handle_function cycle_interval_func =
    static_cast<Scene_time_handle_function>
    (&Time_sensor::cycle_interval_handle);
  s_prototype->add_field_info(new SF_time(CYCLE_INTERVAL,
                                          "cycleInterval",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          cycle_interval_func,
                                          s_def_cycle_interval));

  // frequency
  Uint_handle_function frequency_func =
    static_cast<Uint_handle_function>(&Time_sensor::frequency_handle);
  s_prototype->add_field_info(new SF_uint(FREQUENCY,
                                          "frequency",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          frequency_func));

  // enabled
  exec_func = static_cast<Execution_function>(&Time_sensor::execute_enabled);
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Time_sensor::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          enabled_func, s_def_enabled,
                                          exec_func));

  // loop
  Boolean_handle_function loop_func =
    static_cast<Boolean_handle_function>(&Time_sensor::loop_handle);
  s_prototype->add_field_info(new SF_bool(LOOP, "loop",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          loop_func, s_def_loop));

  // startTime
  Scene_time_handle_function start_time_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::start_time_handle);
  s_prototype->add_field_info(new SF_time(START_TIME, "startTime",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          start_time_func, s_def_start_time));

  // stopTime
  Scene_time_handle_function stop_time_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::stop_time_handle);
  s_prototype->add_field_info(new SF_time(STOP_TIME,
                                          "stopTime",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          stop_time_func, s_def_stop_time));

  // cycleTime
  Scene_time_handle_function cycle_time_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::cycle_time_handle);
  s_prototype->add_field_info(new SF_time(CYCLE_TIME,
                                          "cycleTime",
                                          Field_rule::RULE_OUT,
                                          cycle_time_func));

  // fraction
  Float_handle_function fraction_func =
    static_cast<Float_handle_function>(&Time_sensor::fraction_handle);
  s_prototype->add_field_info(new SF_float(FRACTION,
                                           "fraction_changed",
                                           Field_rule::RULE_OUT,
                                           fraction_func));

  // trueFractionChanged
  Boolean_handle_function true_fraction_func =
    static_cast<Boolean_handle_function>(&Time_sensor::true_fraction_handle);
  s_prototype->add_field_info(new SF_bool(TRUE_FRACTION,
                                          "trueFractionChanged",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          true_fraction_func));

  // FractionBias
  Float_handle_function fraction_bias_func =
    static_cast<Float_handle_function>(&Time_sensor::fraction_bias_handle);
  s_prototype->add_field_info(new SF_float(FRACTION_BIAS,
                                           "fractionBias",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           fraction_bias_func,
                                           s_def_fraction_bias));

  // FractionScale
  Float_handle_function fraction_scale_func =
    static_cast<Float_handle_function>(&Time_sensor::fraction_scale_handle);
  s_prototype->add_field_info(new SF_float(FRACTION_SCALE,
                                           "fractionScale",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           fraction_scale_func,
                                           s_def_fraction_scale));

  // isActive
  Boolean_handle_function is_active_func =
    static_cast<Boolean_handle_function>(&Time_sensor::is_active_handle);
  s_prototype->add_field_info(new SF_bool(IS_ACTIVE, "isActive",
                                          Field_rule::RULE_OUT,
                                          is_active_func));

  // time
  Scene_time_handle_function time_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::time_handle);
  s_prototype->add_field_info(new SF_time(TIME, "time",
                                          Field_rule::RULE_OUT,
                                          time_func));

  // start
  Scene_time_handle_function start_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::start_handle);
  exec_func = static_cast<Execution_function>(&Time_sensor::start);
  s_prototype->add_field_info(new SF_time(START,
                                          "start",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          start_func,
                                          exec_func, true));

  // startTime is initially blocked until the first UpdateTime. So Animations
  // won't start in the middle if the player was not ready to activate them
  // when activated

  // stop
  Scene_time_handle_function stop_func =
    static_cast<Scene_time_handle_function>(&Time_sensor::stop_handle);
  exec_func = static_cast<Execution_function>(&Time_sensor::stop);
  s_prototype->add_field_info(new SF_time(STOP,
                                          "stop",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          stop_func,
                                          exec_func));
}

//! \brief
void Time_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief
Container_proto* Time_sensor::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! Activated by a cascade on the enabled field.
 * Updates m_is_active and cascade it if needed
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::execute_enabled(const Field_info*)
{
  if (!m_enabled) {
    unregister_events();
#if 0
    //! \todo is it necessary?
    // If the sensor was just disabled - enable it for the last time
    // to send all events before disabling
    set_enabled(true);
    update_time();
    set_enabled(false);
    m_is_active = false;
    Field* is_active_field = get_field(IS_ACTIVE);
    if (is_active_field != nullptr) is_active_field->cascade();
#endif
    return;
  }

  // If the sensor was enabled, zero the time in cycle
  register_events();
};

/*! Start the animation by setting m_start_time = m_time + m_start.
 * Used to start the animation in m_start value in seconds
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::start(const Field_info * /* field_info */)
{
  m_start_time = m_time + m_start;
  Field* field = get_field(START_TIME);
  if (field != nullptr) field->cascade();
}

/*! Stop the animation by setting m_stop_time = m_time + m_stop.
 * Used to stop the animation in m_stop value in seconds
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Time_sensor::stop(const Field_info *)
{
  m_stop_time = m_time + m_stop;
  Field* field = get_field(STOP_TIME);
  if (field != nullptr) field->cascade();
}

/*! Blocks/Unblocks the startTime, start and cycleInterval fields
 * (which need to be blocked when the sensor is active.
 */
void Time_sensor::set_blocking_of_fields_for_active(Boolean block)
{
  auto* field = add_field(START_TIME);
  if (field != nullptr) field->set_blocked(block);
  field = add_field(START);
  if (field != nullptr) field->set_blocked(block);
  field = add_field(CYCLE_INTERVAL);
  if (field != nullptr) field->set_blocked(block);
  field = add_field(FRACTION);
  if (field != nullptr) field->set_blocked(block);
}

/*! \brief invoked every tick while the sensor is enabled.
 * Calculates and cascades the time sensor time dependent events.
 *
 * Any set_start_time events to an active time-sensor node are ignored, so we
 * can safely assume that the start time does not change while the sensor is
 * active.
 */
bool Time_sensor::update_time(Scene_time current_time)
{
  // Unblock the start field which are initially blocked - if this is the
  // first call to UpadteTime
  //! \todo this is unnecessary as the same effect can be achieved with a script
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
     (m_loop || ((current_time < (m_start_time + m_cycle_interval)) &&
                 ((m_start_time != s_def_start_time) || (m_frequency != 0)))));

  // If the new isActive is different than the old get its field
  // (to cascade it later)
  Field* is_active_field = nullptr;

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
    if (is_active_field != nullptr) is_active_field->cascade();
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
    else m_time_in_cycle = 0;

    m_end_of_cycle = false;
  }
  else m_time_in_cycle = m_time_in_cycle + delta_time;

  // If the cycle is over and loop is false return
  if (m_end_of_cycle && !m_loop) return true;

  // Initialize cycle_time_field pointer with nullptr
  Field* cycle_time_field = nullptr;

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

  m_true_fraction = true;

  // Cascade changed fields
  if (is_active_field != nullptr) is_active_field->cascade();
  if (cycle_time_field != nullptr) cycle_time_field->cascade();

  auto* fraction_field = get_field(FRACTION);
  if (fraction_field != nullptr) fraction_field->cascade();
  auto* true_fraction_field = get_field(TRUE_FRACTION);
  if (true_fraction_field != nullptr) true_fraction_field->cascade();
  auto* time_field = get_field(TIME);
  if (time_field != nullptr) time_field->cascade();

  return true;
};

//! \brief sets the attributes of the object extracted from an input file.
void Time_sensor::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "cycleInterval") {
      m_cycle_interval = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "frequency") {
      m_frequency = boost::lexical_cast<Uint>(value);
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
      m_start_time = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stopTime") {
      m_stop_time = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fractionBias") {
      m_fraction_bias = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fractionScale") {
      m_fraction_scale = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quitTime") {
      m_quit_time = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Time_sensor::add_to_scene(Scene_graph* sg) { sg->add_time_sensor(this); }

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


void Time_sensor::AddToScene(Scene_graph* sg, XML_entity* parent)
{
  Node::AddToScene(sg, parent);
}
#endif

//! \brief enables or disables the sensor.
void Time_sensor::set_enabled(Boolean enabled)
{
  if (enabled == m_enabled) return;
  m_enabled = enabled;
  (enabled) ? register_events() : unregister_events();
}

//! \brief registers the mouse and mostion events.
void Time_sensor::register_events() { Tick_event::doregister(this); }

//! \brief registers the mouse and mostion events.
void Time_sensor::unregister_events() { Tick_event::unregister(this); }

//! \brief prints out the name of this agent (for debugging purposes).
void Time_sensor::identify()
{ std::cout << "Agent: Time_sensor" << std::endl; }

//! \brief handles tick events.
void Time_sensor::handle(Tick_event* event)
{
  Scene_time sim_time = event->get_sim_time();
  Scene_time t = (m_frequency) ? m_time + 1.0 / m_frequency : sim_time;
  update_time(t);

  if ((m_quit_time != 0)  && (t >= m_quit_time)) exit(0);
}

//! \brief sets the start time of a cycle in seconds.
void Time_sensor::set_start_time(Scene_time start_time)
{
  if (m_is_active) return;
  m_start_time = start_time;
}

//! \brief set the stop time of a cycle in seconds.
void Time_sensor::set_stop_time(Scene_time stop_time)
{
  if (m_is_active && (stop_time <= m_start_time)) return;
  m_stop_time = stop_time;
}

//! \brief set the interval time of a cycle in seconds.
void Time_sensor::set_cycle_interval(Scene_time interval)
{ m_cycle_interval = interval; }

SGAL_END_NAMESPACE
