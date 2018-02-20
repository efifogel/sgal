// Copyright (c) 2007 Israel.
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

#ifndef SGAL_X11_WINDOW_ITEM_HPP
#define SGAL_X11_WINDOW_ITEM_HPP

/*! \file
 * The native window item. It represents a window managed by the native
 * window manager
 */

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/glx.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Native_window_item.hpp"

SGAL_BEGIN_NAMESPACE

class X11_window_item : public Native_window_item {
private:
  /* Information about the X server communicated with. */
  Display* m_display;

  /*! The screen identifier. */
  int m_screen;

  /* The window instance. */
  Window m_window;

  /*! The glx context. */
  GLXContext m_context;

  /*! Window attributes. */
  XSetWindowAttributes m_win_attr;

  /*! The original video mode. */
  XF86VidModeModeInfo m_desk_mode;

  /*! The delete atom. */
  Atom m_wm_delete;

  /*! Create the window.
   * \param display.
   * \param screen.
   * \param full_screen.
   */
  void create(Display* display, Int32 screen);

  /*! Destroy the window. */
  void destroy();

public:
  /*! Constructor */
  X11_window_item();

  /*! Destructor */
  virtual ~X11_window_item() {}

  /*! Show the window. Make the window current if it is not already. */
  virtual void show();

  /*! Hide the window. Make the window current if it is not already. */
  virtual void hide();

  /*! Swap the window frame-buffer. It is assumed that this window is the
   * current window, which has the GL context.
   */
  virtual void swap_buffers();

  /*! Make the context of the window item the current rendering context of
   * the calling thread.
   */
  virtual void make_current();

  void reset_window();

  friend class X11_window_manager;
};

//! \brief reset the window identifier.
inline void X11_window_item::reset_window() { m_window = 0; }

SGAL_END_NAMESPACE

#endif
