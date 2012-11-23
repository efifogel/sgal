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
// $Revision: 6746 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_X11_WINDOW_MANAGER_HPP
#define SGAL_X11_WINDOW_MANAGER_HPP

/*! \file
 * The X11 window manager. It manages the creation and destruction of
 * multiple X11 windows
 */

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Native_window_manager.hpp"
#include "SGAL/X11_window_item.hpp"
#include "SGAL/Event_handler.hpp"

SGAL_BEGIN_NAMESPACE

class Scene;

class X11_window_manager : public Native_window_manager<X11_window_item> {
private:
  /*! The window-manager singleton */
  static X11_window_manager * s_instance;

  /*! The event handler */
  Event_handler m_event_handler;
  
  /* Information about the X server communicated with */
  Display * m_display;

  /*! The screen */
  Int m_screen;

  /*! Has any window been created already? */
  Boolean m_created;
  
  /* Original desktop mode saved for future restoration */
  XF86VidModeModeInfo m_desktop_mode;

  /*! Process a single X event
   * \param event the X event
   */
  void process_xevent(XEvent & event);
  
public:
  /*! Obtain the window-manager singleton */
  static X11_window_manager * instance();  
  
  /*! Constructor */
  X11_window_manager();

  /*! Initialize the window manager
   * \param argc the number of arguments
   * \param argv the array of ASCII arrguments
   */
  void init(Uint argc, char * argv[]);

  /*! Clear the window manager */
  void clear();
  
  /*! Destructor */
  virtual ~X11_window_manager();
  
  /*! Create a new window
   * \param window_item the window item that holds the window to create 
   */
  virtual void create_window(X11_window_item * window_item);

  /*! Destroy an existing window
   * \param window_item the window item that holds the window to destroy
   */
  virtual void destroy_window(X11_window_item * window_item);

  /*! Run the main event loop
   * \param simulating indicates whether the application is simulating
   */
  virtual void event_loop(Boolean simulating = SGAL_FALSE);
};

SGAL_END_NAMESPACE

#endif
