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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Windows window system component of the window manager
 */

#include <Windows.h>
#include <time.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Windows_window_manager.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Scene.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

/*! Has the shift key pressed */
Boolean Windows_window_manager::s_capital(false);

/*! The single instance of a window manager */
Windows_window_manager* Windows_window_manager::s_instance = NULL;

/*! The event handler */
Event_handler Windows_window_manager::s_event_handler;

/*! Constructor */
Windows_window_manager::Windows_window_manager() :
  m_window_class_name("SGAL")
{}

/*! \brief obtains a pointer to the manager */
Windows_window_manager* Windows_window_manager::instance() 
{
  if (!s_instance) s_instance = new Windows_window_manager();
  return s_instance;
}

/*! \brief registers a window class for this application */
void Windows_window_manager::register_window_class()
{
  // Fill in the window class structure:
  WNDCLASSEX wc;
  ::ZeroMemory(&wc, sizeof(wc));                    // clear memory
  wc.cbSize        = sizeof(WNDCLASSEX);            // size of structure
  // Redraws The Window For Any Movement / Resizing:
  // wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  
  wc.style         = 0;                             // no special styles
  wc.lpfnWndProc   = (WNDPROC)(WindowProc);         // event handler
  wc.cbClsExtra    = 0;                             // no extra class data
  wc.cbWndExtra    = 0;                             // no extra window data
  wc.hInstance     = m_hInstance;                   // instance
  wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);   // load a default icon
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);   // load a default cursor
  wc.hbrBackground = NULL;                          // redraw our own bg
  wc.lpszMenuName  = NULL;                          // no menu
  wc.lpszClassName = m_window_class_name;           // use a special class

  /* register the window class */
  if (!::RegisterClassEx(&wc)) {
    std::cerr << "RegisterClass() failed:  Cannot register window class,"
              << std::endl;
    return;
  }
}

/*! \brief initializes the window manager */
void Windows_window_manager::init(Uint argc, char* argv[])
{
  // Get this module instance:
  m_hInstance = GetModuleHandle(NULL);
  register_window_class();
}

/*! brief clears the window manager */
void Windows_window_manager::clear()
{
}

/* Process messages sent to a window
 * \param hWnd
 * \param uMsg
 * \param wParam
 * \param lParam
 */
/* \brief processes messages sent to a window */
LRESULT CALLBACK
Windows_window_manager::WindowProc(HWND hWnd, UINT uMsg,
                                   WPARAM wParam, LPARAM lParam)
{
  SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                  std::cout << "WindowProc()" << std::endl;);
    
  // Get The Window Context
  Windows_window_manager* wm = Windows_window_manager::instance();
  Windows_window_item* current_window;
  
  LONG lRet = 1;
  PAINTSTRUCT ps;
  Mouse_event::Mouse_button button;
  Mouse_event::Press state;
  Boolean pressed;
  Mouse_event* mouse_event;
  Keyboard_event* keyboard_event;
  Motion_event* motion_event;
  Passive_motion_event* passive_motion_event;
  CREATESTRUCT* creation;
  
  switch (uMsg) {
   case WM_CREATE:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_CREATE" << std::endl;);
    // Store Window Structure Pointer:
    creation = (CREATESTRUCT*)(lParam);
    current_window = (Windows_window_item*)(creation->lpCreateParams);
    SetWindowLong(hWnd, GWL_USERDATA,(LONG)(current_window));
    wm->set_current_window(current_window);
    wm->insert_window(current_window);
    break;

   case WM_DESTROY:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_DESTROY" << std::endl;);
    current_window =
      (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->remove_window(current_window);
    wm->set_current_window(NULL);
    break;

   case WM_SIZE:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_SIZE" << std::endl;);
    current_window =
      (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    switch (wParam) {                   // Evaluate Size Action
     case SIZE_MINIMIZED:               // Was Window Minimized?
      current_window->set_visible(false);
      break;
     case SIZE_MAXIMIZED:               // Was Window Maximized?
    process_size:
      current_window->set_visible(true);
      // Don't delay the reshape:
#if 0
      current_window->set_reshape(true);
#else
      current_window->make_current();
      wm->m_scene->reshape_window(current_window,
                                  LOWORD(lParam), HIWORD(lParam));
#endif
      break;
     case SIZE_RESTORED:                // Was Window Restored?
      goto process_size;
      break;
    }
    break;
    
   case WM_PAINT:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_PAINT" << std::endl;);
    BeginPaint(hWnd, &ps);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    // Don't delay the redraw:
#if 0
    current_window->set_redraw(true);
#else
    current_window->set_redraw(false);
    current_window->make_current();
    wm->m_scene->draw_window(current_window, wm->m_button_state != 0);
#endif
    EndPaint(hWnd, &ps);
    break;

   case WM_KEYDOWN:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_KEYDOWN" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    switch (wParam) {
     case VK_SHIFT: s_capital = !s_capital; break;
     case VK_CAPITAL: s_capital = true; break;
     default:
      pressed = true;

     process_key:
      keyboard_event = new Keyboard_event;
      keyboard_event->set_window_item(current_window);
      keyboard_event->set_key((s_capital) ? (wParam + 32) : wParam);
      keyboard_event->set_x(LOWORD(lParam));
      keyboard_event->set_y(current_window->m_height - (HIWORD(lParam)));
      keyboard_event->set_pressed(pressed);
      s_event_handler.issue(keyboard_event);
      current_window->set_redraw(true);
      break;
    }
    break;

   case WM_KEYUP:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_KEYUP" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    switch (wParam) {
      // case VK_ESCAPE:
      //  Windows_window_manager::instance()->destroy_window(current_window);
      //  break;

     case VK_SHIFT: s_capital = !s_capital; break;
     case VK_CAPITAL: s_capital = false; break;
     default:
      pressed = false;
      goto process_key;
    }
    break;

   case WM_LBUTTONDOWN:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_LBUTTONDOWN" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::LEFT_BUTTON;
    state = Mouse_event::DOWN;
    wm->m_button_state |= (0x1 << 1);

  process_mouse:
    mouse_event = new Mouse_event;
    mouse_event->set_window_item(current_window);
    mouse_event->set_x(LOWORD(lParam));
    mouse_event->set_y(current_window->m_height - (HIWORD(lParam)));
    mouse_event->set_button(button);
    mouse_event->set_state(state);
    s_event_handler.issue(mouse_event);
    current_window->set_redraw(true);
    break;

   case WM_LBUTTONUP:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_LBUTTONUP" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::LEFT_BUTTON;
    state = Mouse_event::UP;
    wm->m_button_state &= ~(0x1 << 1);
    goto process_mouse;
    
   case WM_MBUTTONDOWN:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_MBUTTONDOWN" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::MIDDLE_BUTTON;
    state = Mouse_event::DOWN;
    wm->m_button_state |= (0x1 << 2);
    goto process_mouse;

   case WM_MBUTTONUP:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_MBUTTONUP" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::MIDDLE_BUTTON;
    state = Mouse_event::UP;
    wm->m_button_state &= ~(0x1 << 2);
    goto process_mouse;

   case WM_RBUTTONDOWN:
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::RIGHT_BUTTON;
    state = Mouse_event::DOWN;
    wm->m_button_state |= (0x1 << 3);
    goto process_mouse;

   case WM_RBUTTONUP:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_RBUTTONUP" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    button = Mouse_event::RIGHT_BUTTON;
    state = Mouse_event::UP;
    wm->m_button_state &= ~(0x1 << 3);
    goto process_mouse;

   case WM_MOUSEMOVE:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_MOUSEMOVE" << std::endl;);
    current_window = (Windows_window_item*)(GetWindowLong(hWnd, GWL_USERDATA));
    wm->set_current_window(current_window);
    if (wm->m_button_state == 0) {
      passive_motion_event = new Passive_motion_event;
      passive_motion_event->set_window_item(current_window);
      passive_motion_event->set_x(LOWORD(lParam));
      passive_motion_event->set_y(current_window->m_height - (HIWORD(lParam)));
      s_event_handler.issue(passive_motion_event);
    } else {
      motion_event = new Motion_event;
      motion_event->set_window_item(current_window);
      motion_event->set_x(LOWORD(lParam));
      motion_event->set_y(current_window->m_height - (HIWORD(lParam)));
      s_event_handler.issue(motion_event);
    }
    current_window->set_redraw(true);
    break;

   case WM_CAPTURECHANGED:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "WM_CAPTURECHANGED" << std::endl;)
    break;
    
   default:
    SGAL_TRACE_CODE(Trace::WINDOW_MANAGER,
                    std::cout << "default" << std::endl;)
    // Pass it on if unproccessed:
    lRet = DefWindowProc(hWnd, uMsg, wParam, lParam); 
    break; 
  }

  return lRet;
} 

/*! \brief creates a new window */
void Windows_window_manager::create_window(Windows_window_item* window_item)
{
  window_item->create(m_hInstance, m_window_class_name);
  m_current_window = window_item;
}

/*! \brief destroys an existing window */
void Windows_window_manager::destroy_window(Windows_window_item* window_item)
{
  window_item->destroy();
  if (window_item == m_current_window) m_current_window = NULL;
}

/*! \brief performs the main event loop */
void Windows_window_manager::event_loop(Boolean simulating)
{
  Boolean done = false;
  
  // Handle events while not done:
  do {
    // Stage 1 - Generate Tick event:

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
          Windows_window_item* window_item = *it;
          window_item->set_redraw(true);
        }
      } else {
        // If any window is being accumulated, do not advance in the time line.
        bool accumulating = false;
        Windows_iter it;
        for (it = this->begin_windows(); it != this->end_windows(); ++it) {
          Windows_window_item* window_item = *it;
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
          s_event_handler.issue(event);
          m_required_tick_duration = 0;

          /*! \todo Do not force the redraw globally. Instead set the redraw
           * only when really needed.
           */
          Windows_iter it;
          for (it = this->begin_windows(); it != this->end_windows(); ++it) {
            Windows_window_item* window_item = *it;
            window_item->set_redraw(true);
          }
        }
      }
    }

    // Stage 2 - Process Windows window events:
    
    /* If
     * 1. the event queue is empty, and
     * 2. none of the windows has to be redrawn, then
     * wait for an Windows event:
     */
    Boolean wait_for_event = false;
    if (s_event_handler.is_empty()) {
      wait_for_event = true;
      Windows_iter it;
      for (it = this->begin_windows(); it != this->end_windows(); ++it) {
        Windows_window_item* window_item = *it;
        if (window_item->do_redraw()) {
          wait_for_event = false;
          break;
        }
      }
    }

    if (wait_for_event) {
      BOOL rc = WaitMessage();
      if (rc == 0) {
        DWORD dw = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      dw,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR) &lpMsgBuf,
                      0, NULL);
        std::cerr << "WaitMessage failed with error " << dw << ": "
                  << (char *)lpMsgBuf << std::endl;
        return;
      }
    }
    
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, TRUE) != 0) {
      if (msg.message == WM_QUIT) {
        done = true;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (this->size_windows() == 0) PostQuitMessage(0);
    }
    if (!s_event_handler.is_empty()) s_event_handler.process();

    // Stage 3 - Draw all windows that require drawing:

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
  // return msg.wParam;
}

SGAL_END_NAMESPACE
