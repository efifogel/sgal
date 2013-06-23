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
                                        public Bindable_node {
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

  /*! Constructor */
  Navigation_sensor(const Vector3f & translation = Vector3f(),
                    const Rotation & rotation = Rotation(),
                    Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Navigation_sensor();

  /* Construct the prototype */
  static Navigation_sensor* prototype()
  { return new Navigation_sensor(Vector3f(), Rotation(), SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Navigation_sensor(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();
  
  /*! Set the rotation field */
  void set_rotation(const Rotation& rotation);  

  /*! Set the rotation field */
  void set_rotation(float v0, float v1, float v2, float v3);

  /*! Set the translation field */
  void set_translation(const Vector3f& translation);

  /*! Set the translation field */
  void set_translation(float v0, float v1, float v2);
  
  /*! Set the scene graph */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }
  
#if 0
  Window_handle* get_win_handle();
  Window_handle* get_win_handle() const;
#endif
  
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

  /*! Set the dragging speed  */
  void set_dragging_speed(float dragging_speed);

  /*! Obtain the dragging speed  */
  float get_dragging_speed() const { return m_dragging_speed; }
  
  void set_min_zoom_distance(float val);
  virtual bool is_left_button_double_click();

  /*! Reset the sensor to the default translation and rotation */
  void reset();
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  // functions that implement the virtual functions of Event_filter and
  // Drag_sensor, and implement the navigation sensor mechanism
  virtual bool left_button_double_click();
  virtual void start_dragging(const Vector2sh& point);
  virtual void mouse_drag(const Vector2sh& from, const Vector2sh & to);

  virtual void dragging_done(const Vector2sh& point); 

  virtual void track_ball(const Vector2sh& from, const Vector2sh & to,
                          float speed = 1.0f); 
  virtual void translate(const Vector3f& distance, float speed);

  /*! Obtain the bindable stack */
  virtual Bindable_stack* get_stack();

  /*! Enable the bindable node */
  virtual void enable();

  /*! Disable the bindable node */
  virtual void disable();
  
private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
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

  float m_min_zoom_distance;

  /*! The dragging speed */
  float m_dragging_speed;

  /*! The dragging speed divided by 2 precalculated for efficiency */
  float m_half_dragging_speed;

  Rotation m_base_rotation;

  bool m_reset_rotation;

  Vector2sh m_from;

  /*! Tranlation stopped */
  bool m_translation_done;

  /*! Rotation stopped */
  bool m_rotation_done;

  /*! Indicates that translation is being performed */
  bool m_translating;

  /*! Indicates that rotation is being performed */
  bool m_rotating;

  /*! Issue events at the end of a transformation session */
  void transform_done();
  
  bool setup_cursor();
  bool allow_zoom_in(const Vector3f& trans);
};

SGAL_END_NAMESPACE

#endif
