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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file Manage windows
 */

#ifndef SGAL_WINDOW_FACTORY_HPP
#define SGAL_WINDOW_FACTORY_HPP

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Window_item;

class Window_factory {
public:
  /*! Destructor */
  ~Window_factory() {}

  /*! Obtain the factory singletone */
  static Window_factory * get_instance();  

  /*! Obtain the current window */
  static Window_item * get_current();

  /*! Process window system events for all Windows that have been created */
  static void main_loop();

  /*! Create a new window */
  Window_item * create();

  /*! Detsroy a window */
  void destroy(Window_item * win);

  /*! */
  void idle_func();

private:
  /*! The window-factory singletone */
  static Window_factory * m_instance;

  /*! The list of windows */
  std::list<Window_item *> m_windows;

  /*! Push a new window onto the list of windows */
  void push_back(Window_item * win) { m_windows.push_back(win); }

  /*! */
  static void sgal_set_window(Uint id);
  static Uint sgal_get_window();
  static void * sgal_get_user_data(Uint id);

  friend class Window_item;
};

SGAL_END_NAMESPACE

#endif
