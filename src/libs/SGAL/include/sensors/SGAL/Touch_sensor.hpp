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

#ifndef SGAL_TOUCH_SENSOR_HPP
#define SGAL_TOUCH_SENSOR_HPP

/*! \file
 * The Touch Sensor activates events regarding touching of a geometry
 * in the scene graph with the mouse pointer.
 *
 * The Touch sensor is implemented according to X3D/VRML touch sensor
 * definition.
 *
 * In addition to the X3D touch sensor definition this sensor
 * also supports a SFNode event out (ROUTEDNODE)
 * which cascaded for a mouseup event.
 */

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Drag_sensor.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Vector2sh;
class Scene_graph;
class Passive_motion_event;
class Tick_event;
class Formatter;

class SGAL_CLASSDEF Touch_sensor : public Agent, public Drag_sensor,
                                   public Node
{
public:
  enum {
    FIRST = Container::LAST - 1,
    ENABLED,
    HITNORMAL,
    HITPOINT,
    HITTEXCOORD,
    ISACTIVE,
    ISOVER,
    EXACTIVATE,
    TOUCHTIME,
    ROUTEDNODE,
    NUMBER_OF_SELECTION_IDS,
    FIRST_SELECTION_ID,
    SELECTION_ID,
    LAST
  };

  /*! Constructor */
  Touch_sensor(Boolean enabled = true, Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Touch_sensor();

  /*! Construct the prototype */
  static Touch_sensor* prototype()
  { return new Touch_sensor(SGAL_FALSE, SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Touch_sensor(); }

  // attributes mutators - used by the scene graph picking mechanism
  void set_normal(const Vector3f& normal);
  void set_point(const Vector3f& point);
  void set_tex_coord(const Vector2f& tex_coord);
  void set_is_over(const Boolean over);

  // Sets the routed node pointer
  void set_routed_node(Container* node);
  Container* get_routed_node() const;

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
  
  /*! Write this container */
  virtual void write(Formatter* formatter);

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
  
  /*! Handle tick events */
  virtual void handle(Tick_event* event);

  // This function is executed when the exActivate field is cascaded from
  // another field
  virtual void external_activate(Field_info*);

  // Returns the priority of the touch sensor - is higher than the priority
  // of the navigation sensor
  /*! \todo Ulong filter_priority() const { return MEDIUM_PRIORITY; } */

  /*! Obtain the first id of a unique range of color ids used for picking */
  Uint get_first_selection_id() const { return m_first_selection_id; }

  /*! Set the first id of a unique range of color ids used for picking */
  void set_start_selection_ids(Uint first_id)
  { m_first_selection_id = first_id; }

  /*! Obtain the number of ids in a unique range of color ids used for picking
   */
  Uint get_num_selection_ids() const { return m_num_selection_ids; }

  /*! Set the number of ids in a unique range of color ids used for picking */
  void set_num_selection_ids(Uint num_ids)
  { m_num_selection_ids = num_ids; }

  /*! Obtain the currently picked color id */
  Uint get_selection_id() const { return m_selection_id; }

  /*! Set the currently picked color id */
  void set_selection_id(Uint id) { m_selection_id = id; }
  
  /*! Is the given id in the range of color ids */
  Boolean is_in_range(Uint id)
  {
    return ((m_first_selection_id <= id) &&
            (id < (m_first_selection_id + m_num_selection_ids)));
  }

  /*! Enable or disable the touch sensor */
  void set_enabled(Boolean enabled);

  /*! Is the touch sensor enabled? */
  Boolean get_enabled() const { return m_enabled; }

  /*! Set the scene-graph pointer */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  /*! Obtain the scene-graph pointer */
  Scene_graph* get_scene_graph() const { return m_scene_graph; }
  
protected :
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  // Functions the implement the empty functions of EDragSensor
  // Used to calculate and activate the touch sensor events by
  // cascading the suitable fields
  virtual void start_dragging(const Vector2sh & point);
  virtual void dragging_done(const Vector2sh & point); 

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The scene graph */
  Scene_graph* m_scene_graph;

  /*! The start id of a unique range of color ids used for picking */
  Uint m_first_selection_id;

  /*! The number of ids of a unique range of color ids used for picking */
  Uint m_num_selection_ids;

  /*! The id currently selected */
  Uint m_selection_id;
  
  Boolean m_enabled;
  Vector3f m_hit_normal;
  Vector3f m_hit_point;
  Vector2f m_hit_tex_coord;
  Boolean m_is_active;
  Boolean m_ex_activate;
  Boolean m_is_over;
  Scene_time m_touch_time;

  Vector3f m_last_normal;
  Vector3f m_last_point;
  Vector2f m_last_tex_coord;
  Boolean m_last_is_over;
  Boolean m_drag_locked;

  Container* m_routed_node;

  // default values
  static bool s_def_enabled;
};

/*! Draws the node while traversing the scene graph
 */
inline Action::Trav_directive Touch_sensor::draw(Draw_action* /* draw_action */)
{
  return Action::TRAV_CONT;
}

SGAL_END_NAMESPACE

#endif
