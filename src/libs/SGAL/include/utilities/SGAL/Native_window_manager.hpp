// Copyright (c) 2004  Israel.
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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_NATIVE_WINDOW_MANAGER_HPP
#define SGAL_NATIVE_WINDOW_MANAGER_HPP

/*! \file
 * The native window manager, which manages the creation and destruction of
 * multiple windows.
 */

#include <time.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Window_manager.hpp"
#include "SGAL/Native_window_item.hpp"

SGAL_BEGIN_NAMESPACE

class Scene;

template <typename Window_item>
class Native_window_manager : public Window_manager<Window_item> {
protected:
  /*! The attached scene. */
  Scene* m_scene;

  /*! The current window. */
  Window_item* m_current_window;

  /*! Mouse button states. */
  Uint m_button_state;

  /*! The time when the current tick starts in clock units. */
  long int m_start_tick_time;

  /*! The accumulated simulation time. */
  Scene_time m_sim_time;

  /*! The estimated tick duration. */
  Scene_time m_est_tick_duration;

  /*! The required tick duration. */
  Scene_time m_required_tick_duration;

public:
  typedef Window_manager<Window_item>    Base_window_manager;

  /*! Constructor */
  Native_window_manager() :
    m_scene(NULL),
    m_current_window(NULL),
    m_start_tick_time(0),
    m_sim_time(0),
    m_est_tick_duration(0),
    m_required_tick_duration(0)
  {
    // Mouse button states:
    m_button_state = 0;
    m_est_tick_duration = 1.0f / 30;
  }

  /*! Destructor */
  virtual ~Native_window_manager() {}

  /*! Set the current window.
   * \param window_item the window to set.
   */
  void set_current_window(Window_item* window_item)
  { m_current_window = window_item; }

  /*! Obtain the current window.
   * \return the current window.
   */
  Window_item* get_current_window() const
  { return m_current_window; }

  /*! Set the current window and make the window's Context current as well.
   * \param window_item the window to make current.
   */
  void make_current_window(Window_item* window_item)
  { m_current_window = window_item; }

  /*! Set the flag that indicates whether the next window be made full.
   * screen
   */
  void set_full_screen(Boolean flag)
  { Native_window_item::set_init_full_screen(flag); }

  /*! Set the initial width of windows.
   * \param width the new width of the windows.
   */
  void set_window_width(Uint width)
  { Native_window_item::set_init_width(width); }

  /*! Set the initial height of windows.
   * \param heigh the height of the windows.
   */
  void set_window_height(Uint height)
  { Native_window_item::set_init_height(height); }

  /*! Set the initial size of windows.
   * \param width the new width of the windows.
   * \param heigh the new height of the windows.
   */
  void set_window_size(Uint width, Uint height)
  { Native_window_item::set_init_size(width, height); }

  /*! Set the initial x-position of window otigins.
   * \param x the screen x-coordinate of the window origin.
   */
  void set_window_x(Uint x) { Native_window_item::set_init_x(x); }

  /*! Set the initial y-position of window otigins.
   * \param x the screen y-coordinate of the window origin.
   */
  void set_window_y(Uint y) { Native_window_item::set_init_y(y); }

  /*! Set the initial position of window otigins.
   * \param x the screen X coordinate of the window origin.
   * \param y the screen Y coordinate of the window origin.
   */
  void set_window_position(Uint x, Uint y)
  { Native_window_item::set_init_position(x, y); }

  /*! Attach a scene to the window manager. */
  void set_scene(Scene* scene) { m_scene = scene; }
};

SGAL_END_NAMESPACE

#endif
