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
// $Source: $
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Container - implementation                              
 */

#include "SGAL/Drag_sensor.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Drag_sensor::Drag_sensor() :
  m_is_control_down(false),
  m_is_shift_down(false),
  m_is_left_mouse_down(false),
  m_is_right_mouse_down(false),
  m_is_mouse_drag(false)
{
};

/*! Desctructor */
Drag_sensor::~Drag_sensor() {}

/*! Handles the left button down event
 * @param event (in) the activated event
 */
void Drag_sensor::left_button_down(int x, int y)
{
  m_is_left_mouse_down = true;
  m_from_point[0] = m_to_point[0] = x;
  m_from_point[1] = m_to_point[1] = y;

  /*! \todo
   * get_win_handle()->set_capture();
   * get_win_handle()->set_focus();
   */
  
  // Activates the virtual start dragging functions
  start_dragging(m_to_point);
  start_left_dragging(m_to_point);
}

/*! Handles the left button up event
 * @param event (in) the activated event
 */
void Drag_sensor::left_button_up(int x, int y)
{
  m_from_point = m_to_point;
  m_to_point[0] = x;
  m_to_point[1] = y;

  // Activates the virtual dragging done functions
  //! \todo get_win_handle ()->release_capture();
  dragging_done(m_to_point);
  left_dragging_done(m_to_point);
  m_is_left_mouse_down = false;
}

/*! Handles the right button down event
 * @param event (in) the activated event
 */
void Drag_sensor::right_button_down(int x, int y)
{
  m_is_right_mouse_down = true;
  m_from_point[0] = m_to_point[0] = x;
  m_from_point[1] = m_to_point[1] = y;

  /*! \todo
   * get_win_handle()->set_capture();
   * get_win_handle()->set_focus();
   */
  
  // Activates the virtual start dragging function
  start_dragging(m_to_point);
  start_right_dragging(m_to_point);
}

/*! Handles the right button up event
 * @param event (in) the activated event
 */
void Drag_sensor::right_button_up(int x, int y)
{
  m_from_point = m_to_point;
  m_to_point[0] = x;
  m_to_point[1] = y;

  // Activates the virtual dragging done function
  //! \todo get_win_handle()->release_capture();
  dragging_done(m_to_point);
  right_dragging_done(m_to_point);
  m_is_right_mouse_down = false;
}

/*! Handles the mouse event - 
 * @param event (in) the activated event
 */
void Drag_sensor::mouse_move(int x, int y)
{
  // updates the points between which the mouse move was performed
  m_from_point = m_to_point;

  m_to_point[0] = x;
  m_to_point[1] = y;

  // Activates dragging if left or right button is down
  if (is_left_mouse_down() || is_right_mouse_down()) {
    m_is_mouse_drag = true;

    // Activated the virtual mouse dragging function
    mouse_drag(m_from_point, m_to_point);

    // if the left button is pressed - activates the virtual left button
    // mouse dragging function
    if (is_left_mouse_down()) left_mouse_drag(m_from_point, m_to_point);
    if (is_right_mouse_down()) right_mouse_drag(m_from_point, m_to_point);
  } 
}

SGAL_END_NAMESPACE
