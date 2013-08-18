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

#ifndef SGAL_NAVIGATION_SENSOR_HPP
#define SGAL_NAVIGATION_SENSOR_HPP

/*! \file
 * The Navigation Sensor senses navigation inputs from the user.
 *
 * The Navigation Sensor inherites from Drag_sensor and Bindable_node.
 * The Drag_sensor functionality is used to receive the navigation
 * inputs from the user.
 * The navigation inputs are set into the object data members.
 * For every update the scene graph event Cascade is activated using
 * the container functionality.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Drag_sensor.hpp"
#include "SGAL/Bindable_node.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector2sh.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

class Bindable_stack;
class Element;
class Container_proto;
class Mouse_event;
class Motion_event;
class Scene_graph;

class SGAL_SGAL_DECL Navigation_sensor : public Agent, public Drag_sensor,
                                         public Bindable_node
{
public:
  enum {
    FIRST = Bindable_node::LAST - 1,
    TRANSLATION,
    ROTATION,
    TRANSLATION_DONE,
    ROTATION_DONE,
    MIN_ZOOM_DISTANCE,
    LAST
  };

  /*! Constructor
   * \param translation (in) the init value of translation.
   * \param rotation (in) the init value of rotation.
   */
  Navigation_sensor(const Vector3f& translation = Vector3f(),
                    const Rotation& rotation = Rotation(),
                    Boolean proto = false);

  /*! Destructor */
  virtual ~Navigation_sensor();

  /* Construct the prototype. */
  static Navigation_sensor* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f* translation_handle(Field_info*) { return &m_translation; }
  Rotation* rotation_handle(Field_info*) { return &m_rotation; }
  Float* min_zoom_distance_handle(Field_info*) { return &m_min_zoom_distance; }
  Boolean* translation_done_handle(Field_info*) { return &m_translation_done; }
  Boolean* rotation_done_handle(Field_info*) { return &m_rotation_done; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Set the rotation field. */
  void set_rotation(const Rotation& rotation);

  /*! Set the rotation field. */
  void set_rotation(Float v0, Float v1, Float v2, Float v3);

  /*! Set the translation field. */
  void set_translation(const Vector3f& translation);

  /*! Set the translation field. */
  void set_translation(Float v0, Float v1, Float v2);

  /*! Set the scene graph. */
  void set_scene_graph(Scene_graph* sg);

#if 0
  Window_handle* get_win_handle();
  Window_handle* get_win_handle() const;
#endif

  /*! Register the mouse and mostion events. */
  void register_events();

  /*! Register the mouse and mostion events. */
  void unregister_events();

  /*! Print out the name of this agent (for debugging purposes) */
  virtual void identify();

  /*! Handle mouse events. */
  virtual void handle(Mouse_event* event);

  /*! Handle motion events. */
  virtual void handle(Motion_event* event);

  /*! Set the dragging speed of the sensor.
   * \param dragging_speed the dragging speed
   */
  void set_dragging_speed(Float dragging_speed);

  /*! Obtain the dragging speed. */
  Float get_dragging_speed() const;

  /*! Set the minimum zoom in distance.
   * This limits the zoomin on the object.
   */
  void set_min_zoom_distance(Float val);

  virtual bool is_left_button_double_click();

  /*! Reset the sensor to the default translation and rotation. */
  void reset();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  // functions that implement the virtual functions of Event_filter and
  // Drag_sensor, and implement the navigation sensor mechanism.
  virtual bool left_button_double_click();

  /*! Activated when dragging is started
   * If dragging is not locked by another sensor -
   * Locks the dragging in the execution coordinator.
   */
  virtual void start_dragging(const Vector2sh& point);

  /*! Handle mouse dragging.
   * \param from (in) the point from which the dragging has started.
   * \param to (in) the point where the dragging has reached.
   */
  virtual void mouse_drag(const Vector2sh& from, const Vector2sh & to);

  /*! Unlocke the dragging at when it is done. */
  virtual void dragging_done(const Vector2sh& point);

  /*! Handle a track ball update.
   * \param from (in) the point from which the dragging has started
   * \param to (in) the point where the dragging has reached
   * \param speed (in) the current speed of the navigation
   *
   * This is not a real trackball implementation. It is a slightly varied
   * version of the trackball. When the mouse button is clicked, the position
   * of the cursor is stored, so is the "current" rotation matrix. Each move
   * of the cursor, the rotational transformation is calculated relative the
   * the cursors position at the time the mouse button was pressed (unlike
   * "real" trackball in which it is calculated relative to the last "segment"
   * on the screen). It is then multiplied by the rotation matrix at the time
   * the mouse button was pressed.
   *
   * Doing it this way, we avoid the weird rotational movement of the model
   * when a circular motion is performed with the cursor.
   */
  virtual void track_ball(const Vector2sh& from, const Vector2sh & to,
                          Float speed = 1.0f);

  /*! Handle a translation update.
   * \param distance (in) the distance of the translation
   * \param speed (in) the current speed of the navigation
   */
  virtual void translate(const Vector3f& distance, Float speed);

  /*! Obtain the bindable stack.
   * This function prepares the Navigation_sensor node for operation. It obtains
   * the speed factor computed by the scene graph. A valid pointer to the scene
   * graph mustbe maintained. An alternative mechanism to obtain the speed
   * factor is to pass the speed factor via events.
   */
  virtual Bindable_stack* get_stack();

  /*! Enable the bindable node. */
  virtual void enable();

  /*! Disable the bindable node. */
  virtual void disable();

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The Scene_graph */
  Scene_graph* m_scene_graph;

  /*! The output translation */
  Vector3f m_translation;

  /*! The output rotation */
  Rotation m_rotation;

  /*! Indicates that dragging is locked for this sensor */
  bool m_drag_locked;

  bool m_is_left_button_double_click;

  bool m_reset;

  Float m_min_zoom_distance;

  /*! The dragging speed */
  Float m_dragging_speed;

  /*! The dragging speed divided by 2 precalculated for efficiency */
  Float m_half_dragging_speed;

  Rotation m_base_rotation;

  bool m_reset_rotation;

  Vector2sh m_from;

  /*! Tranlation stopped */
  bool m_translation_done;

  /*! Rotation stopped */
  bool m_rotation_done;

  /*! Indicates that translation is being performed. */
  bool m_translating;

  /*! Indicates that rotation is being performed. */
  bool m_rotating;

  /*! Issue events at the end of a transformation session. */
  void transform_done();

  bool setup_cursor();

  bool allow_zoom_in(const Vector3f& trans);
};

/* \brief constructs the prototype. */
inline Navigation_sensor* Navigation_sensor::prototype()
{ return new Navigation_sensor(Vector3f(), Rotation(), true); }

/*! \brief clones. */
inline Container* Navigation_sensor::clone()
{ return new Navigation_sensor(); }

/*! \brief sets the scene graph. */
inline void Navigation_sensor::set_scene_graph(Scene_graph* sg)
{ m_scene_graph = sg; }

/*! \brief obtains the dragging speed. */
inline Float Navigation_sensor::get_dragging_speed() const
{ return m_dragging_speed; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Navigation_sensor::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
