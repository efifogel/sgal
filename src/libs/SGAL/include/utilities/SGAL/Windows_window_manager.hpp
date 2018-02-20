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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WINDOWS_WINDOW_MANAGER_HPP
#define SGAL_WINDOWS_WINDOW_MANAGER_HPP

/*! \file
 * The Windows window manager. It manages the creation and destruction of
 * multiple Windows windows
 */

#include <windows.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Native_window_manager.hpp"
#include "SGAL/Windows_window_item.hpp"
#include "SGAL/Event_handler.hpp"

SGAL_BEGIN_NAMESPACE

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class Scene;

class SGAL_SGAL_DECL Windows_window_manager :
  public Native_window_manager<Windows_window_item>
{
private:
  /*! The module instance. */
  HINSTANCE m_hInstance;

  /*! The window class name. */
  char* m_window_class_name;

  /*! The window-manager singleton. */
  static Windows_window_manager* s_instance;

  /*! The event handler. */
  static Event_handler s_event_handler;

  /*! Has the shift key pressed. */
  static Boolean s_capital;

  /* Process messages sent to a window (the event handler).
   * \param hWnd
   * \param uMsg
   * \param wParam
   * \param lParam
   */
  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg,
                                     WPARAM wParam, LPARAM lParam);

  /*! Register a window class for this application.
   */
  void register_window_class();

public:
  /*! Obtain the window-manager singleton.
   * \return the window-manager.
   */
  static Windows_window_manager* instance();

  /*! Constructor */
  Windows_window_manager();

  /*! Initialize the window manager.
   * \param argc the number of arguments.
   * \param argv the array of ASCII arrguments.
   */
  void init();

  /*! Clear the window manager */
  void clear();

  /*! Destructor */
  virtual ~Windows_window_manager() {}

  /*! Create a new window.
   * \param window_item the window item that holds the window to create .
   */
  virtual void create_window(Windows_window_item* window_item);

  /*! Destroy an existing window.
   * \param window_item the window item that holds the window to destroy.
   */
  virtual void destroy_window(Windows_window_item* window_item);

  /*! Run the main event loop.
   * \param simulating indicates whether the application is simulating.
   */
  virtual void event_loop(Boolean simulating = false);
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
