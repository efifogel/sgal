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

#ifndef SGAL_SINGLE_KEY_SENSOR_HPP
#define SGAL_SINGLE_KEY_SENSOR_HPP

/*! \file
 * The Single Key Sensor handles single key events. It can generate key press
 * and key-press time events for a single specific keyboard key.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Keyboard_event;
class Container_proto;

class SGAL_SGAL_DECL Single_key_sensor : public Agent, public Node {
public:
  enum {
    FIRST = Container::LAST - 1,
    PRESS,
    TIME,
    STATE,
    INT_STATE,
    NUMBER_OF_STATES,
    LAST
  };

  /*! Constructor. */
  Single_key_sensor(Boolean proto = false);

  /*! Destructor. */
  virtual ~Single_key_sensor();

  /* Construct the prototype. */
  static Single_key_sensor* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initializes the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the node (does nothing). */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the keyboard event */
  void register_events();

  /*! Unregisters the keyboard event */
  void unregister_events();

  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify();

  /*! Handle mouse events. */
  virtual void handle(Keyboard_event* event);

  /*! Set the flag that indicates whether the sensor is triggered on release.
   */
  void set_trigger_on_release(Boolean flag) { m_trigger_on_release = flag; }

  /*! Determine whether the sensor should be triggered on release. */
  Boolean is_trigger_on_release() const { return m_trigger_on_release;}

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The designated key. */
  Uint m_key;

  /*! Indicates that the designated key has been pressed. */
  Boolean m_press;

  /*! The time of the event. */
  Scene_time m_time;

  /*! Indicates whether it is a boolean state */
  Boolean m_boolean;

  /*! A binary state toggled each press */
  Boolean m_state;

  /*! An integer state increased each press. */
  Int m_int_state;

  /*! Number of integer states. */
  Uint m_num_states;

  /*! Indicates whether the sensor is triggered on release. */
  Boolean m_trigger_on_release;

  /*! Default Values. */
  static Uint s_def_num_states;
  static Boolean s_def_trigger_on_release;
};

/* \brief constructs the prototype. */
inline Single_key_sensor* Single_key_sensor::prototype()
{ return new Single_key_sensor(true); }

/*! \brief constructs a new instance. */
inline Container* Single_key_sensor::clone()
{ return  new Single_key_sensor(); }

/*! \brief draws the node (does nothing). */
inline
Action::Trav_directive Single_key_sensor::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

SGAL_END_NAMESPACE

#endif
