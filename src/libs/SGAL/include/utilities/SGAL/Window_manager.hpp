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
// $Revision: 4978 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WINDOW_MANAGER_HPP
#define SGAL_WINDOW_MANAGER_HPP

/*! \file
 * A pure window manager, which manages the creation and destruction of
 * multiple windows
 */

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Window_manager.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Window_item>
class Window_manager {
protected:
  /*! The created windows */
  std::list<Window_item *> m_windows;
  
  /*! Constructor */
  Window_manager() {}

public:
  /*! Destructor */
  virtual ~Window_manager() { m_windows.clear(); }
  
  /*! Create a new window item
   * \return the newly created window item
   */
  virtual Window_item * create_window_item()
  {
    Window_item * window_item = new Window_item;
    m_windows.push_back(window_item);
    return window_item;
  }

  /*! Destroy an existing window item
   * \param window_item the window item to destroy
   */
  virtual void destroy_window_item(Window_item * window_item)
  {
    m_windows.remove(window_item);
    delete window_item;
  }
};

SGAL_END_NAMESPACE

#endif
