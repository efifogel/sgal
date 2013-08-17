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

#ifndef SGAL_PROXIMITY_SENSOR_HPP
#define SGAL_PROXIMITY_SENSOR_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Scene_graph;
class Passive_motion_event;

class SGAL_SGAL_DECL Proximity_sensor : public Agent, public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    ENABLED,
    POSITION,
    ORIENTATION,
    LAST
  };

  /*! Constructor */
  Proximity_sensor(Boolean enabled = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Proximity_sensor();

  /*! Construct the prototype */
  static Proximity_sensor* prototype()
  { return new Proximity_sensor(false, true); }

  /*! Clone */
  virtual Container* clone() { return new Proximity_sensor(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Draw the node while traversing the scene graph */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the mouse and mostion events */
  void register_events();

  /*! Register the mouse and mostion events */
  void unregister_events();

  /*! Print out the name of this agent (for debugging purposes) */
  virtual void identify();

  /*! Handle mouse events */
  virtual void handle(Mouse_event* event);

  /*! Handle motion events */
  virtual void handle(Motion_event* event);

  /*! Handle mouse events */
  virtual void handle(Passive_motion_event* event);

  void set_enabled(bool enabled);
  bool get_enabled() const { return m_enabled; }

  /*! Set the scene-graph pointer */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  /*! Obtain the scene-graph pointer */
  Scene_graph* get_scene_graph() const { return m_scene_graph; }

protected :
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The scene graph */
  Scene_graph* m_scene_graph;

  /*! Indicates whether the sensor is enabled */
  Boolean m_enabled;

  Vector3f m_position;
  Rotation m_orientation;

  // default values
  static bool s_def_enabled;
};

/*! Draws the node while traversing the scene graph */
inline
Action::Trav_directive Proximity_sensor::draw(Draw_action* /* draw_action */)
{
  return Action::TRAV_CONT;
}

SGAL_END_NAMESPACE

#endif
