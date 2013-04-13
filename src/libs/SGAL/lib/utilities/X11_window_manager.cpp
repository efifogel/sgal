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
// $Id: $
// $Revision: 12533 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * X11 window system component of the window manager
 */

#include <string.h>
#include <time.h>
/* default header and core functionallity */
#include <X11/Xlib.h>
/* Functionallity for creating new protocol messages */
#include <X11/Xatom.h>
/* keysymbols used to resolv what keys that are being pressed */
#include <X11/keysym.h>

/* the XF86 Video Mode extension allows us to change the displaymode of the
 * server this allows us to set the display to fullscreen and also read
 * videomodes and other information.
 */
#include <X11/extensions/xf86vmode.h>
 
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/X11_window_manager.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Scene.hpp"
#include "SGAL/Trace.hpp"

// #include "X11_event_names.hpp"

SGAL_BEGIN_NAMESPACE

/*! The single instance of a window manager */
X11_window_manager* X11_window_manager::s_instance = NULL;

/*! Constructor */
X11_window_manager::X11_window_manager() :
  m_display(NULL),
  m_screen(0),
  m_created(false)
{}

/*! Destructor */
X11_window_manager::~X11_window_manager() { clear(); }

/*! \brief obtains a pointer to the manager */
X11_window_manager* X11_window_manager::instance() 
{
  if (!s_instance) s_instance = new X11_window_manager();
  return s_instance;
}

/*! \brief initializes the window manager */
void X11_window_manager::init(Uint /* argc */, char* /* argv */[])
{
  m_display = XOpenDisplay(0);
  m_screen = DefaultScreen(m_display);
  // Enable CreateNotify events of new windows
  XSelectInput(m_display, RootWindow(m_display, m_screen),
               SubstructureNotifyMask);
}

/*! brief clears the window manager */
void X11_window_manager::clear()
{
  if (!m_display) return;
  XCloseDisplay(m_display);
  m_display = NULL;
}

/*! \brief creates a new window */
void X11_window_manager::create_window(X11_window_item* window_item)
{
  window_item->create(m_display, m_screen);
  m_current_window = window_item;
}
 
/*! \brief destroys an existing window */
void X11_window_manager::destroy_window(X11_window_item* window_item)
{
  window_item->destroy();
  // Switch back to original desktop resolution if we were in fullscreen:
  if (window_item->is_full_screen()) {
    XF86VidModeSwitchToMode(m_display, m_screen, &m_desktop_mode);
    XF86VidModeSetViewPort(m_display, m_screen, 0, 0);
  }
}

/*! \brief runs the main event loop */
void X11_window_manager::event_loop(Boolean simulating)
{
  Boolean done = false;

  // Handle events while not done:
  do {
    // Chech whether simulation is required: 
    if (simulating) {
      // Measure the ellapsed time:
      clock_t end_tick_time = clock();
      clock_t tick_duration = end_tick_time - m_start_tick_time;
      long sleep_time = m_required_tick_duration - tick_duration;
      // Slow down if necessary:
      if (sleep_time > 0) {
        // Draw all visibile windows:
        Windows_iter it;
        for (it = this->begin_windows(); it != this->end_windows(); ++it) {
          X11_window_item* window_item = *it;
          window_item->set_redraw(true);
        }
      }
      else {
        // If any window is being accumulated, do not advance in the time line.
        bool accumulating = false;
        Windows_iter it;
        for (it = this->begin_windows(); it != this->end_windows(); ++it) {
          X11_window_item* window_item = *it;
          if (window_item->is_accumulating()) {
            accumulating = true;
            break;
          }
        }
        if (!accumulating) {
          // Continue along the time line - issue a tick event:
          m_start_tick_time = end_tick_time;
          m_sim_time += tick_duration;
          SGAL::Tick_event* event = new SGAL::Tick_event;
          event->set_est_tick_duration(m_est_tick_duration);
          event->set_sim_time(m_sim_time);
          m_event_handler.issue(event);
          m_required_tick_duration = 0;

          /*! \todo Do not force the redraw globally. Instead set the redraw
           * only when really needed.
           */
          Windows_iter it;
          for (it = this->begin_windows(); it != this->end_windows(); ++it) {
            X11_window_item* window_item = *it;
            window_item->set_redraw(true);
          }
        }
      }
    }

    /* If
     * 1. the event queue is empty, and
     * 2. none of the windows has to be redrawn, then
     * wait for an X event:
     */
    Boolean wait_for_xevent = false;
    if (m_event_handler.is_empty()) {
      wait_for_xevent = true;
      Windows_iter it;
      for (it = this->begin_windows(); it != this->end_windows(); ++it) {
        X11_window_item* window_item = *it;
        if (window_item->do_redraw()) {
          wait_for_xevent = false;
          break;
        }
      }
    }

    if (wait_for_xevent) {
      XEvent event;
      XNextEvent(m_display, &event);
      
      process_xevent(event);
      if (m_created && (this->size_windows() == 0)) done = true;
    }

    // Process events from the X queue if there are any:
    while (XPending(m_display) > 0) {
      XEvent event;
      XNextEvent(m_display, &event);
      process_xevent(event);
      if (m_created && (this->size_windows() == 0)) {
        done = true;
        break;
      }
    }
    // Process events:
    m_event_handler.process();

    // Draw all windows that need to be drawn:
    Windows_iter it;
    for (it = this->begin_windows(); it != this->end_windows(); ++it) {
      Window_item* window_item = *it;
      if (window_item->do_redraw() && window_item->is_visible()) {
        // Reset the redraw flag, so that the user can re-set it:
        window_item->set_redraw(false);
        window_item->make_current();
        m_scene->draw_window(window_item, m_button_state != 0);
      }
    }
  } while (!done);
}

/*! \brief processes a single X event */
void X11_window_manager::process_xevent(XEvent& event)
{
  KeySym keysym;
  char buffer[31];
  Uint button_id;
  Mouse_event::Mouse_button button;
  Mouse_event* mouse_event;
  Motion_event* motion_event;
  Passive_motion_event* passive_motion_event;
  Keyboard_event* keyboard_event;
  XKeyboardControl kc_values;
  Boolean pressed;
  XComposeStatus status_in_out;
  Windows_iter it;
  
  kc_values.key = -1;         // ALL keys
  
  // std::cout << "event type: " << event_names[event.type] << std::endl;
  switch (event.type) {
   case Expose:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "Expose" << std::endl;);
    // Draw only if the number of Expose events that are to follow vanishes:
    if (event.xexpose.count != 0) break;
    for (it = this->begin_windows(); it != this->end_windows(); ++it) {
      X11_window_item* window_item = *it;
      if (window_item->m_window == event.xexpose.window) {
#if 0
        window_item->set_redraw(true);
#else
        window_item->set_redraw(false);
        window_item->make_current();
        m_scene->draw_window(window_item, m_button_state != 0);
#endif
        break;
      }
    }
    break;

   case ConfigureNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "ConfigureNotify" << std::endl;);
    for (it = this->begin_windows(); it != this->end_windows(); ++it) {
      X11_window_item* window_item = *it;
      if (window_item->m_window == event.xconfigure.window) {
        // Reshape only if the window-size changed:
        Uint width = event.xconfigure.width;
        Uint height = event.xconfigure.height;
        if ((static_cast<Uint>(event.xconfigure.width) !=
             window_item->m_width) ||
            (static_cast<Uint>(event.xconfigure.height) !=
             window_item->m_height))
        {
          window_item->set_size(width, height);
#if 0
          window_item->set_reshape(true);
#else
          window_item->make_current();
          m_scene->reshape_window(window_item, width, height);
#endif
        }
        break;
      }
    }
    break;

   case ButtonPress:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "ButtonPress" << std::endl;);
    button_id = (event.xbutton.button == Button1) ? 0 :
      (event.xbutton.button == Button2) ? 1 :
      (event.xbutton.button == Button3) ? 2 : 3;
    m_button_state |= (0x1 << button_id);
    mouse_event = new Mouse_event;
    mouse_event->set_window_item(m_current_window);
    button = (Mouse_event::Mouse_button)(0x1 << button_id);
    mouse_event->set_button(button);
    mouse_event->set_state(Mouse_event::DOWN);
    mouse_event->set_x(event.xbutton.x);
    mouse_event->set_y(m_current_window->m_height - event.xbutton.y);
    m_event_handler.issue(mouse_event);
    m_current_window->set_redraw(true);
    break;

   case ButtonRelease:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "ButtonRelease" << std::endl;);
    button_id = (event.xbutton.button == Button1) ? 0 :
      (event.xbutton.button == Button2) ? 1 :
      (event.xbutton.button == Button3) ? 2 : 3;
    m_button_state &= ~(0x1 << button_id);
    mouse_event = new Mouse_event;
    mouse_event->set_window_item(m_current_window);
    button = (Mouse_event::Mouse_button)(0x1 << button_id);
    mouse_event->set_button(button);
    mouse_event->set_state(Mouse_event::UP);
    mouse_event->set_x(event.xbutton.x);
    mouse_event->set_y(m_current_window->m_height - event.xbutton.y);
    m_event_handler.issue(mouse_event);
    m_current_window->set_redraw(true);
    break;
        
   case KeyPress:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "KeyPress" << std::endl;);
    /* Break out if the window has been destroyed. This is possible, for
     * example, if auto-repeat for the key is on. In this case alternating
     * KeyPress and KeyRelease events are generated while the key is still
     * pressed:
     */
    if (!m_current_window) break;
    XLookupString(&event.xkey, buffer, 30, &keysym, &status_in_out);
    pressed = true;
  process_key:
    
    keyboard_event = new Keyboard_event;
    keyboard_event->set_window_item(m_current_window);
    keyboard_event->set_key(buffer[0]);
    keyboard_event->set_x(event.xkey.x);
    keyboard_event->set_y(m_current_window->m_height - event.xkey.y);
    keyboard_event->set_pressed(pressed);
    m_event_handler.issue(keyboard_event);
    m_current_window->set_redraw(true);
    break;

   case KeyRelease:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "KeyRelease" << std::endl;);
    // Break out if the window has been destroyed:
    if (!m_current_window) break;
    XLookupString(&event.xkey, buffer, 30, &keysym, &status_in_out);
    switch(keysym) {
      // case XK_Escape:
      // destroy_window(m_current_window);
      // break;

     case XK_F1:            // toggle full screen
      destroy_window(m_current_window);
      m_current_window->set_full_screen(!m_current_window->is_full_screen());
      create_window(m_current_window);
      break;
     default:
      pressed = false;
      goto process_key;
      break;
    }
    break;

   case MotionNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "MotionNotify" << std::endl;);
    if (!m_current_window) {
      for (it = this->begin_windows(); it != this->end_windows(); ++it) {
        X11_window_item* window_item = *it;
        if (window_item->m_window == event.xfocus.window) {
          m_current_window = window_item;
          break;
        }
      }
    }

    // Break out if the window has been destroyed:
    if (!m_current_window) break;
    SGAL_assertion(m_current_window);
    if (m_button_state) {
      motion_event = new Motion_event;
      motion_event->set_window_item(m_current_window);
      motion_event->set_x(event.xbutton.x);
      motion_event->set_y(m_current_window->m_height - event.xbutton.y);
      m_event_handler.issue(motion_event);
    }
    else {
      passive_motion_event = new Passive_motion_event;
      passive_motion_event->set_window_item(m_current_window);
      passive_motion_event->set_x(event.xbutton.x);
      passive_motion_event->set_y(m_current_window->m_height - event.xbutton.y);
      m_event_handler.issue(passive_motion_event);
    }
    m_current_window->set_redraw(true);
    break;
    
   case ClientMessage:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "ClientMessage" << std::endl;);
    if (event.xclient.window == RootWindow(m_display, m_screen)) break;
    if (!m_current_window) break;
    if (static_cast<Atom>(event.xclient.data.l[0]) ==
        m_current_window->m_wm_delete)
    {
      this->remove_window(m_current_window);
      kc_values.auto_repeat_mode = 1;
      XChangeKeyboardControl(m_display, KBAutoRepeatMode, &kc_values);
    }
    break;

   case FocusIn:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "FocusIn" << std::endl;);
    kc_values.auto_repeat_mode = 0;
    XChangeKeyboardControl(m_display, KBAutoRepeatMode, &kc_values);
    for (it = this->begin_windows(); it != this->end_windows(); ++it) {
      X11_window_item* window_item = *it;
      if (window_item->m_window == event.xfocus.window) {
        m_current_window = window_item;
        break;
      }
    }
    break;

   case FocusOut:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "FocusOut" << std::endl;);
    m_current_window = NULL;
    kc_values.auto_repeat_mode = 1;
    XChangeKeyboardControl(m_display, KBAutoRepeatMode, &kc_values);
    break;

   case CreateNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "CreateNotify" << std::endl;);
    if (event.xcreatewindow.override_redirect) break;
    // Check whether the event is associated with our window.
    if (!m_current_window) break;
    m_created = true;
    this->insert_window(m_current_window);
    break;
    
   case DestroyNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "DestroyNotify" << std::endl;);
    if (event.xdestroywindow.event != event.xdestroywindow.window) break;
    for (it = this->begin_windows(); it != this->end_windows(); ++it) {
      X11_window_item* window_item = *it;
      if (window_item->m_window == event.xdestroywindow.window) {
        window_item->reset_window();
        this->remove_window(window_item);
        m_current_window = NULL;
        kc_values.auto_repeat_mode = 1;
        XChangeKeyboardControl(m_display, KBAutoRepeatMode, &kc_values);
        break;
      }
    }
    break;

   case MapNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "MapNotify" << std::endl;);
    if (event.xunmap.event != event.xunmap.window) break;
    if (!m_current_window) {
      for (it = this->begin_windows(); it != this->end_windows(); ++it) {
        X11_window_item* window_item = *it;
        if (window_item->m_window == event.xmap.window) {
          m_current_window = window_item;
          break;
        }
      }
    }
    if (!m_current_window) break;
    m_current_window->set_visible(true);
    break;
    
   case UnmapNotify:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "UnmapNotify" << std::endl;);
    if (event.xunmap.event != event.xunmap.window) break;
    if (!m_current_window) {
      for (it = this->begin_windows(); it != this->end_windows(); ++it) {
        X11_window_item* window_item = *it;
        if (window_item->m_window == event.xunmap.window) {
          m_current_window = window_item;
          break;
        }
      }
    }
    if (!m_current_window) break;
    m_current_window->set_visible(false);
    break;
    
   default:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "default" << std::endl;);
    break;
  }
}

SGAL_END_NAMESPACE
