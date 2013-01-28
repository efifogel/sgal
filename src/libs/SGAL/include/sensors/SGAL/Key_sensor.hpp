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
// $Source$
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_KEY_SENSOR_HPP
#define SGAL_KEY_SENSOR_HPP

/*! \file
 * The Key Sensor activates events regarding pressing of keyboard keys.
 *
 * The Key sensor is implemented according to X3D/VRML key sensor definition.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Keyboard_event;
class Container_proto;
class Scene_graph;

class Key_sensor : public Agent, public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    KEY_PRESS,
    KEY_RELEASE,
    ACTION_KEY_PRESS,
    ACTION_KEY_RELEASE,
    SHIFT_KEY,
    CONTROL_KEY,
    ALT_KEY,
    IS_ACTIVE,
    LAST
  };

  /*! Constructor */
  Key_sensor(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Key_sensor() {};

  /*! Construct the prototype */
  static  Key_sensor* prototype() { return new Key_sensor(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Key_sensor(); }

  /*! Enactivate or disactivates this key sensor */
  void set_active(Boolean active);

  /*! Activate the key sensor */
  void activate(Field_info* field_info = NULL);

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Draw the node (does nothing) */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the mouse and mostion events */
  void register_events();

  /*! Register the mouse and mostion events */
  void unregister_events();
  
  /*! print out the name of this agent (for debugging purposes) */
  virtual void identify();

  /*! Handle mouse events */
  virtual void handle(Keyboard_event* event);
  
protected:
  /*! The Scene_graph */
  Scene_graph* m_scene_graph;

  Boolean m_enabled;
  Boolean m_active;
  Boolean m_shift_key;
  Boolean m_control_key;
  Boolean m_alt_key;

  /*! Indicates whether any key is currently pressed */
  Boolean m_pressed;
  
  Int m_key;
  Int m_action_key;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! Draws the node (does nothing) */
inline Action::Trav_directive Key_sensor::draw(Draw_action* /* draw_action */)
{
  return Action::TRAV_CONT;
}; 

SGAL_END_NAMESPACE

#endif
