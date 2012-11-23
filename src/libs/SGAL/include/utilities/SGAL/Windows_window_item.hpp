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
// $Source: $
// $Revision: 6004 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_Windows_WINDOW_ITEM_HPP
#define SGAL_Windows_WINDOW_ITEM_HPP

/*! \file
 * The native window item. It represents a window managed by the native
 * window manager
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Native_window_item.hpp"

SGAL_BEGIN_NAMESPACE

class Windows_window_item : public Native_window_item {
private:
  bool m_bOwnWnd;

  HWND m_hWnd;
  
  HDC m_hDC;
  
  HGLRC m_hRC;

  //! The pixel format
  Int m_pixel_format;
  
  Int m_arb_multisample_format;

  /*! Is it the first window? */
  static Boolean s_first;
  
  /*! Create the window
   * \param hInstance the module instance
   * \param window_class_name the window-class name
   * \param hWnd if we don't want to create a new window, the handle of an existing window.
   */
  void create(HINSTANCE & hInstance, char * window_class_name, HWND hWnd = NULL);

  /*! Create the window
   * \param hInstance the module instance
   * \param window_class_name the window-class name
   * \param hWnd if we don't want to create a new window, the handle of an existing window.
   */
  void create_base(HINSTANCE & hInstance, char * window_class_name, HWND hWnd = NULL);

  /*! Destroy the window */
  void destroy();

  /*! Query the multisample frequencies
   * \param hInstance
   * \param pfd
   */
  Boolean init_multisample(HINSTANCE & hInstance);

public:
  /*! Constructor */
  Windows_window_item() :
    m_arb_multisample_format(0)
  {}

  /*! Destructor */
  virtual ~Windows_window_item() {}

  /*! Show the window. Make the window current if it is not already. */
  virtual void show();

  /*! Hide the window. Make the window current if it is not already. */
  virtual void hide();

  /*! Swap the window frame-buffer. It is assumed that this window is the
   * current window, which has the GL context
   */
  virtual void swap_buffers();
  
  /*! Make the context of the window item the current rendering context of
   * the calling thread
   */
  virtual void make_current();

  friend class Windows_window_manager;
  friend class CPlayerControlCtrl;
};

SGAL_END_NAMESPACE

#endif
