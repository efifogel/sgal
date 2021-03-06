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

/*!
 * This class is used as a container of fields. That is, it (indirectly)
 * derives from 'Container' through 'Node'. As such, for each field f a
 * pointer to a member function that obtains a pointer to the data member
 * that stores the value of the field f is used. This member function is
 * invoked relative to the based 'Container' class.
 * Keep 'Node' as the first derived class. Otherwise, MSVC complains.
 */
class Key_sensor : public Node, public Agent {
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

  /*! Constructor
   */
  Key_sensor(Boolean proto = false);

  /*! Destructor */
  virtual ~Key_sensor() {};

  /*! Construct the prototype.
   */
  static Key_sensor* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Enactivate or disactivates this key sensor. */
  void set_active(Boolean active);

  /*! Activate the key sensor. */
  void activate(const Field_info* field_info = nullptr);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Uint* key_handle(const Field_info*) { return &m_key; }
  Uint* action_key_handle(const Field_info*) { return &m_action_key; }
  Boolean* shift_key_handle(const Field_info*) { return &m_shift_key; }
  Boolean* control_key_handle(const Field_info*) { return &m_control_key; }
  Boolean* alt_key_handle(const Field_info*) { return &m_alt_key; }
  Boolean* active_handle(const Field_info*) { return &m_active; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the key sensor to a given scene graph.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Draw the node (does nothing) */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the mouse and mostion events. */
  void register_events();

  /*! Register the mouse and mostion events. */
  void unregister_events();

  /*! print out the name of this agent (for debugging purposes). */
  virtual void identify();

  /*! Handle a mouse events. */
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

  Uint m_key;

  Uint m_action_key;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

//! \brief constructs the prototype.
inline Key_sensor* Key_sensor::prototype() { return new Key_sensor(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Key_sensor::create()
{ return new Key_sensor(); }

//! \brief draws the node (does nothing).
inline Action::Trav_directive Key_sensor::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; };

//! \brief obtains the tag (type) of the container.
inline const std::string& Key_sensor::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
