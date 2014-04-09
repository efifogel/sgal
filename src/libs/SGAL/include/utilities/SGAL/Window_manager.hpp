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

#ifndef SGAL_WINDOW_MANAGER_HPP
#define SGAL_WINDOW_MANAGER_HPP

/*! \file
 * A pure window manager, which manages the creation and destruction of
 * multiple windows.
 */

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Window_item>
class Window_manager {
public:
  typedef std::list<Window_item*>                       Windows;
  typedef typename Windows::iterator                    Windows_iter;
  typedef typename Windows::const_iterator              Windows_const_iter;

  /*! Constructor */
  Window_manager() {}

  /*! Destructor */
  virtual ~Window_manager() {}

  /*! Create a new window
   * \param window_item the window item that holds the window to create
   */
  virtual void create_window(Window_item* window_item) = 0;

  /*! Destroy an existing window
   * \param window_item the window item that holds the window to destroy
   */
  virtual void destroy_window(Window_item * window_item) = 0;

  /*! Run the main event loop
   * \param simulating indicates whether the application is simulating
   */
  virtual void event_loop(Boolean simulating = false) = 0;

protected:
  /*! Insert a new window item to the container of window items.
   * \param window_item the window item to insert.
   */
  void insert_window(Window_item* window_item);

  /*! Remove a window item from the container of window items.
   * \param window_item the window item to remove.
   */
  void remove_window(Window_item* window_item);

  /*! Obtain the iteator to the window container first element. */
  Windows_iter begin_windows();

  /*! Obtain the iterator to the Array past-the-end element. */
  Windows_iter end_windows();

  /*! Obtain the iteator to the window container first element. */
  Windows_const_iter begin_windows() const;

  /*! Obtain the iterator to the window container past-the-end element. */
  Windows_const_iter end_windows() const;

  /*! Obtain the number of windows. */
  Uint size_windows() const;

private:
  /*! The window items */
  Windows m_windows;
};

/*! \brief inserts a new window item to the container of window items. */
template <typename Window_item>
inline void Window_manager<Window_item>::insert_window(Window_item* window_item)
{ m_windows.push_back(window_item); }

/*! \brief removes a window item from the container of window items. */
template <typename Window_item>
inline void Window_manager<Window_item>::remove_window(Window_item* window_item)
{ m_windows.remove(window_item); }

/*! \brief obtains the iteator to the window container first element. */
template <typename Window_item>
inline typename Window_manager<Window_item>::Windows_iter
Window_manager<Window_item>::begin_windows()
{ return m_windows.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element. */
template <typename Window_item>
inline typename Window_manager<Window_item>::Windows_iter
Window_manager<Window_item>::end_windows()
{ return m_windows.end(); }

/*! \brief obtains the iteator to the window container first element. */
template <typename Window_item>
inline typename Window_manager<Window_item>::Windows_const_iter
Window_manager<Window_item>::begin_windows() const
{ return m_windows.begin(); }

/*! \brief obtains the iterator to the window container past-the-end element. */
template <typename Window_item>
inline typename Window_manager<Window_item>::Windows_const_iter
Window_manager<Window_item>::end_windows() const
{ return m_windows.end(); }

/*! \brief obtains the number of windows. */
template <typename Window_item>
inline Uint Window_manager<Window_item>::size_windows() const
{ return m_windows.size(); }

SGAL_END_NAMESPACE

#endif
