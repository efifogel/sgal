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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_OPEN_GL_OS_INIT_HPP
#define SGAL_OPEN_GL_OS_INIT_HPP

/*! \file
 * A base class (abstract) for window related operations.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

class Window_handle;

class SGAL_SGAL_DECL Open_gl_os_init {

public:
  Open_gl_os_init() { };
  virtual ~Open_gl_os_init() {  };

  virtual void set_window(Window_handle * win_handle) = 0;
  virtual void destroy_current() = 0;
  virtual int make_current() = 0;
  virtual int make_current_null() = 0;
  virtual void release_current() = 0;
  virtual void swap_buffers() = 0;

  Gfx::Color_mode get_color_mode() const;

protected:
  Gfx::Color_mode m_color_mode;
};

SGAL_END_NAMESPACE

#endif
