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
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TIME_SENSOR_HPP
#define SGAL_TIME_SENSOR_HPP

/*! \file
 * The Time Sensor activates time events in the scene graph.
 *
 * The Time sensor is implemented according to X3D/VRML time sensor definition.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Tick_event;

/*!
 * This class is used as a container of fields. That is, it (indirectly)
 * derives from 'Container' through 'Node'. As such, for each field f a
 * pointer to a member function that obtains a pointer to the data member
 * that stores the value of the field f is used. This member function is
 * invoked relative to the based 'Container' class.
 * Keep 'Node' as the first derived class. Otherwise, MSVC complains.
 */
class SGAL_SGAL_DECL Time_sensor : public Node, public Agent {
public:
  enum {
    FIRST = Node::LAST - 1,
    CYCLE_INTERVAL,
    FREQUENCY,
    ENABLED,
    LOOP,
    START_TIME,
    STOP_TIME,
    CYCLE_TIME,
    FRACTION,
    TRUE_FRACTION,
    FRACTION_BIAS,
    FRACTION_SCALE,
    IS_ACTIVE,
    TIME,
    START,
    STOP,
    LAST
  };

  /*! Constructor */
  Time_sensor(Boolean proto = false);

  /*! Destructor */
  virtual ~Time_sensor();

  /*! Construct the prototype */
  static Time_sensor* prototype() { return new Time_sensor(true); }

  /*! Clone */
  virtual Container* clone() { return new Time_sensor(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Scene_time* cycle_interval_handle(Field_info*) { return &m_cycle_interval; }
  Uint* frequency_handle(Field_info*) { return &m_frequency; }
  Boolean* enabled_handle(Field_info*) { return &m_enabled; }
  Boolean* loop_handle(Field_info*) { return &m_loop; }
  Scene_time* start_time_handle(Field_info*) { return &m_start_time; }
  Scene_time* stop_time_handle(Field_info*) { return &m_stop_time; }
  Scene_time* cycle_time_handle(Field_info*) { return &m_cycle_time; }
  Float* fraction_handle(Field_info*) { return &m_fraction; }
  Boolean* is_active_handle(Field_info*) { return &m_is_active; }

  Boolean* true_fraction_handle(Field_info*) { return &m_true_fraction; }
  Float* fraction_bias_handle(Field_info*) { return &m_fraction_bias; }
  Float* fraction_scale_handle(Field_info*) { return &m_fraction_scale; }

  Scene_time* time_handle(Field_info*) { return &m_time; }
  Scene_time* start_handle(Field_info*) { return &m_start; }
  Scene_time* stop_handle(Field_info*) { return &m_stop; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  // Execution function - executed input events on fields
  virtual void execute_enabled(Field_info*);

  // start function - starts the animation according to m_start
  virtual void start(Field_info*);

  // stop function - stop the animation accordint to m_stop
  virtual void stop(Field_info*);

  /*! Invoke every tick while the sensor is enabled */
  virtual bool update_time(Scene_time current_time);

  /*! Draw the node while traversing the scene graph */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the mouse and mostion events */
  void register_events();

  /*! Register the mouse and mostion events */
  void unregister_events();

  /*! Print out the name of this agent (for debugging purposes) */
  virtual void identify();

  /*! Handle tick events */
  virtual void handle(Tick_event* event);

  // virtual void AddToScene(Scene_graph* sg, XML_entity* parent);

  /*! Enable or disables the sensor */
  void set_enabled(Boolean enabled);

  /*! Obtain the sensor status */
  bool get_enabled() const { return m_enabled; }

  /*! Set the start time of a cycle in seconds */
  void set_start_time(Scene_time start_time) { m_start_time = start_time; }

  /*! Obtain the start time of a cycle in seconds */
  Scene_time get_start_time() const { return m_start_time; }

  /*! Set the stop time of a cycle in seconds */
  void set_stop_time(Scene_time stop_time) { m_stop_time = stop_time; }

  /*! Obtain the stop time of a cycle in seconds */
  Scene_time get_stop_time() const { return m_stop_time; }

  /*! Set the interval time of a cycle in seconds */
  void set_cycle_interval(Scene_time interval) { m_cycle_interval = interval; }

  /*! Obtain the interval time of a cycle in seconds */
  Scene_time get_cycle_interval() const { return m_cycle_interval; }

protected :
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The interval time of a cycle in seconds */
  Scene_time m_cycle_interval;

  /*! If not 0, the frequency of time & fraction events */
  Uint m_frequency;

  /*! Indicates whether the sensor is enabled */
  Boolean m_enabled;

  /*! Indicates whether the sensor is active through cycles generated in a
   * loop, or just a single cycle.
   */
  Boolean m_loop;

  Scene_time m_start_time;
  Scene_time m_stop_time;
  Scene_time m_cycle_time;

  /*! A floating point value in the closed interval [0, 1] representing the
   * relative progress of time within a cycle
   */
  Float m_fraction;

  /*! Indicates that the fraction has changed */
  Boolean m_true_fraction;

  Float m_fraction_bias;
  Float m_fraction_scale;
  Boolean m_is_active;
  Scene_time m_time;
  Scene_time m_time_in_cycle;

  Scene_time m_start;
  Scene_time m_stop;
  Boolean m_end_of_cycle;
  Boolean m_first_update;

  Scene_time m_quit_time;

  // Default values
  static Scene_time s_def_cycle_interval;
  static Boolean s_def_enabled;
  static Boolean s_def_loop;
  static Scene_time s_def_start_time;
  static Scene_time s_def_stop_time;
  static Float s_def_fraction_bias;
  static Float s_def_fraction_scale;

  // Blocks/Unblocks the startTime and cycleInterval fields
  // (which need to be blocked when the sensor is active.
  void set_blocking_of_fields_for_active(Boolean block);
};

/*! Draws the node while traversing the scene graph
 */
inline Action::Trav_directive Time_sensor::draw(Draw_action* /* draw_action */)
{
  return Action::TRAV_CONT;
}

SGAL_END_NAMESPACE

#endif
