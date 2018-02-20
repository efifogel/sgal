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

#ifndef SGAL_DRAG_SENSOR_HPP
#define SGAL_DRAG_SENSOR_HPP

/*! \file
 * Handles dragging for inheriting sensors (such as touch sensor...)
 *
 * Implements higher level virtual functions for handling draging events.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector2sh.hpp"

SGAL_BEGIN_NAMESPACE

#if 0
class Window_handle;
#endif

class SGAL_SGAL_DECL Drag_sensor {
public:
  // Constructor
  Drag_sensor();

  // Destructor
  virtual ~Drag_sensor();

protected:
  virtual void left_button_down(int x, int y);
  virtual void left_button_up(int x, int y);
  virtual void right_button_down(int x, int y);
  virtual void right_button_up(int x, int y);
  virtual void mouse_move(int x, int y);

  // Virtual functions that are activated by this class to handle dragging

  // Activated when dragging with the left mouse button is started
  virtual void start_left_dragging(const Vector2sh& /* point */) {};
  virtual void start_right_dragging(const Vector2sh& /* point */) {};

  // Activated for mouse move during left button dragging
  virtual void left_mouse_drag(const Vector2sh& /* from */,
                               const Vector2sh& /* to */) {};
  virtual void right_mouse_drag(const Vector2sh& /* from */,
                                const Vector2sh& /* to */) {};

  // Activated when dragging with left button stops
  virtual void left_dragging_done(const Vector2sh& /* point */) {};
  virtual void right_dragging_done(const Vector2sh& /* point */) {};

  // Activated when dragging with left or right mouse button is started
  virtual void start_dragging(const Vector2sh& /* point */) {};

  // Activated for mouse move during left or right button dragging
  virtual void mouse_drag(const Vector2sh& /* from */,
                          const Vector2sh& /* to */)
  {};

  // Activated when dragging with left or right button stops
  virtual void dragging_done(const Vector2sh& /* point */) {};

  // Query about the state (down or up) of buttons
  virtual bool is_left_mouse_down() const;
  virtual bool is_right_mouse_down() const;
  virtual bool is_control_down() const;
  virtual bool is_shift_down() const;

  // Returns the point from which dragging has started
  Vector2sh& get_from_point();
  const Vector2sh& get_from_point() const;

  // Returns the point from which dragging has finished
  Vector2sh& get_to_point();
  const Vector2sh& get_to_point() const;

public:
  virtual bool is_mouse_drag() const { return m_is_mouse_drag; }

  virtual void set_is_mouse_drag(Boolean is_mouse_drag)
  {
    m_is_mouse_drag = is_mouse_drag;
  }

protected:
  // Data members that holds the current state of the user actions:

  bool m_is_control_down;

  bool m_is_shift_down;

  bool m_is_left_mouse_down;

  bool m_is_right_mouse_down;

  bool m_is_mouse_drag;

  Vector2sh m_from_point;

  Vector2sh m_to_point;
};

// Inline implementation of the simple query functions

inline bool Drag_sensor::is_left_mouse_down() const
{ return m_is_left_mouse_down; }

inline bool Drag_sensor::is_right_mouse_down() const
{ return m_is_right_mouse_down; }

inline bool Drag_sensor::is_control_down() const
{ return m_is_control_down; }

inline bool Drag_sensor::is_shift_down() const
{ return m_is_shift_down; }

inline Vector2sh& Drag_sensor::get_from_point()
{ return m_from_point; }

inline const Vector2sh& Drag_sensor::get_from_point() const
{ return m_from_point; }

inline Vector2sh& Drag_sensor::get_to_point()
{ return m_to_point; }

inline const Vector2sh& Drag_sensor::get_to_point() const
{ return m_to_point; }

SGAL_END_NAMESPACE

#endif
