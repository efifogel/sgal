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

class SGAL_CLASSDEF Touch_sensor : public Agent, public Drag_sensor, public Node
{
public:
  enum {
    FIRST = Node::LAST - 1,
    ENABLED,
    HITNORMAL,
    HITPOINT,
    HITTEXCOORD,
    IS_ACTIVE,
    IS_OVER,
    EXACTIVATE,
    TOUCH_TIME,
    ROUTEDNODE,
    NUMBER_OF_SELECTION_IDS,
    FIRST_SELECTION_ID,
    OVER_SELECTION_ID,
    ACTIVE_SELECTION_ID,
    LAST
  };

  /*! Constructor */
  Touch_sensor(Boolean enabled = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Touch_sensor();

  /*! Construct the prototype. */
  static Touch_sensor* prototype();

  /*! Clone. */
  virtual Container* clone();

  // attributes mutators - used by the scene graph picking mechanism
  void set_normal(const Vector3f& normal);

  void set_point(const Vector3f& point);

  void set_tex_coord(const Vector2f& tex_coord);

  /*! Set the flag that indicates whether the cursor hoovers above a selected
   * geometry.
   * \param over (in) the flag.
   */
  void set_is_over(const Boolean over);

  /*! Set the flag that indicates whether the mouse is pressed when the cursor
   * is above a selected geometry.
   * \param active (in) the flag.
   */
  void set_is_active(const Boolean active);
  
  /* Sets the routed node pointer. */
  void set_routed_node(Container* node);

  Container* get_routed_node() const;

  /*! Initialize the object prototype. */
  virtual void init_prototype();

  /*! Delete the object prototype. */
  virtual void delete_prototype();

  /*! Obtain the object prototype. */
  virtual Container_proto* get_prototype();
  
  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the touch sensor object to a given scene.
   * This includes adding it to the container of touch sensors in the
   * scene graph and setting the base selection id.
   * \param scene_graph the given scene.
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);
  
  /*! Write this container */
  virtual void write(Formatter* formatter);

  /*! Draw the node while traversing the scene graph */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Register the mouse and mostion events. */
  void register_events();

  /*! Register the mouse and mostion events. */
  void unregister_events();
  
  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify();

  /*! Handle mouse events. */
  virtual void handle(Mouse_event* event);
  
  /*! Handle motion events. */
  virtual void handle(Motion_event* event);

  /*! Handle mouse events. */
  virtual void handle(Passive_motion_event* event);
  
  /*! Handle tick events. */
  virtual void handle(Tick_event* event);

  // This function is executed when the exActivate field is cascaded from
  // another field.
  virtual void external_activate(Field_info*);

  // Returns the priority of the touch sensor - is higher than the priority
  // of the navigation sensor.
  /*! \todo Ulong filter_priority() const { return MEDIUM_PRIORITY; } */

  /*! Obtain the first id of a unique range of color ids used for picking. */
  Uint get_first_selection_id() const;

  /*! Set the first id of a unique range of color ids used for picking. */
  void set_start_selection_ids(Uint first_id);

  /*! Obtain the number of ids in a unique range of color ids used for picking.
   */
  Uint get_num_selection_ids() const;

  /*! Set the number of ids in a unique range of color ids used for picking. */
  void set_num_selection_ids(Uint num_ids);

  /*! Obtain the raw id of the geometry, which the cursor is hoovering above. */
  Uint get_selection_id() const;

  /*! Set the raw id of the geometry, which the cursor is hoovering above. */
  void set_selection_id(Uint id);
  
  /*! Obtain the (normalized) id of the geometry, which the cursor is hoovering
   * above.
   */
  Uint get_over_selection_id() const;

  /*! Set the (normalized) id of the geometry, which the cursor is hoovering
   * above.
   */
  void set_over_selection_id(Uint id);
  
  /*! Obtain the (normalized) id of the geometry, which the cursor was above
   * when dragging started.
   */
  Uint get_active_selection_id() const;

  /*! Set the (normalized) id of the geometry, which the cursor was above when
   * dragging started.
   */
  void set_active_selection_id(Uint id);

  /*! Is the given id in the range of color ids. */
  Boolean is_in_range(Uint id);

  /*! Enable or disable the touch sensor. */
  void set_enabled(Boolean enabled);

  /*! Determine whether the touch sensor is enabled. */
  Boolean get_enabled() const;

  /*! Set the scene-graph pointer. */
  void set_scene_graph(Scene_graph* sg);

  /*! Obtain the scene-graph pointer. */
  Scene_graph* get_scene_graph() const;
  
protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Invoked when dragging starts.
   * Locks the dragging for the current sensor if possible (if not returns).
   * Updates and cascades the m_is_active field
   * @param point (in) not used for now
   */
  virtual void start_dragging(const Vector2sh& point);

  /*! \brief Invoked when dragging stops.
   * Updates and cascades the m_is_active field.
   * Updates and cascades the m_touch_time field.
   * Unlockes the dragging in the execution coordinator.
   * @param point (in) not used for now
   */
  virtual void dragging_done(const Vector2sh& point); 

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The scene graph */
  Scene_graph* m_scene_graph;

  /*! The start id of a unique range of color ids used for picking */
  Uint m_first_selection_id;

  /*! The number of ids of a unique range of color ids used for picking */
  Uint m_num_selection_ids;

  /*! The raw id of the geometry, which the cursor is hoovering above. */
  Uint m_selection_id;

  /*! The (normalized) id of the geometry, which the cursor is hoovering above.
   */
  Uint m_over_selection_id;

  /*! The (normalized) id of the geometry, which the cursor was above when
   * dragging started.
   */
  Uint m_active_selection_id;

  /*! Indicates whether the touch sensor is enabled. */
  Boolean m_enabled;
  
  Vector3f m_hit_normal;

  Vector3f m_hit_point;

  Vector2f m_hit_tex_coord;

  /*! Indicates whether the mouse is pressed when the cursor is above a selected
   * geometry.
   */
  Boolean m_is_active;

  Boolean m_ex_activate;

  /*! Indicates whether the cursor hoovers above a selected geometry. */
  Boolean m_is_over;

  Scene_time m_touch_time;

  Vector3f m_last_normal;
  Vector3f m_last_point;
  Vector2f m_last_tex_coord;
  Uint m_last_selection_id;
  Boolean m_drag_locked;

  Container* m_routed_node;

  // default values
  static bool s_def_enabled;
};

/*! \brief constructs the prototype. */
inline Touch_sensor* Touch_sensor::prototype()
{ return new Touch_sensor(false, true); }

/*! \brief clones. */
inline Container* Touch_sensor::clone() { return new Touch_sensor(); }

/*! \brief draws the node while traversing the scene graph. */
inline Action::Trav_directive Touch_sensor::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

/*! \brief obtains the first id of a unique range of color ids used for picking.
 */
inline Uint Touch_sensor::get_first_selection_id() const
{ return m_first_selection_id; }

/*! \brief sets the first id of a unique range of color ids used for picking. */
inline void Touch_sensor::set_start_selection_ids(Uint first_id)
{ m_first_selection_id = first_id; }

/*! \brief obtains the number of ids in a unique range of color ids used for
 * picking.
 */
inline Uint Touch_sensor::get_num_selection_ids() const
{ return m_num_selection_ids; }

/*! \brief sets the number of ids in a unique range of color ids used for
 * picking.
 */
inline void Touch_sensor::set_num_selection_ids(Uint num_ids)
{ m_num_selection_ids = num_ids; }

/*! \brief obtains the id of the geometry, which the cursor is hoovering above.
 */
inline Uint Touch_sensor::get_selection_id() const { return m_selection_id; }

/*! \brief sets the id of the geometry, which the cursor is hoovering above. */
inline void Touch_sensor::set_selection_id(Uint id) { m_selection_id = id; }

/*! \brief obtains the (normalized) id of the geometry, which the cursor is
 * hoovering above.
 */
inline Uint Touch_sensor::get_over_selection_id() const
{ return m_over_selection_id; }

/*! \brief obtains the (normalized) id of the geometry, which the cursor was
 * above when dragging started.
 */
inline Uint Touch_sensor::get_active_selection_id() const
{ return m_active_selection_id; }

/*! \brief determines whether the given id in the range of color ids. */
inline Boolean Touch_sensor::is_in_range(Uint id)
{
  return ((m_first_selection_id <= id) &&
          (id < (m_first_selection_id + m_num_selection_ids)));
}

/*! \brief determines whether the touch sensor is enabled. */
inline Boolean Touch_sensor::get_enabled() const { return m_enabled; }

/*! \brief sets the scene-graph pointer. */
inline void Touch_sensor::set_scene_graph(Scene_graph* sg)
{ m_scene_graph = sg; }

/*! \brief obtains the scene-graph pointer. */
inline Scene_graph* Touch_sensor::get_scene_graph() const
{ return m_scene_graph; }
  
/*! \brief obtains the tag (type) of the container. */
inline const std::string& Touch_sensor::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
