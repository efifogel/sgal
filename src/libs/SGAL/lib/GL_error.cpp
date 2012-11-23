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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <string>

#include "SGAL/basic.hpp"
#include "GL_error.h"
#include "App_error_handle_int.h"

using namespace SGAL;

GL_error * GL_error::m_instance = 0;

/*!
 */
GL_error::GL_error() : m_execution_coordinator(0)
{
    REGISTER_DEBUG_CLASS(DBG_GLERROR, "GL_error")
}

/*!
 */
GL_error * GL_error::get_instance() 
{
  if (!m_instance) 
    m_instance = new GL_error();
  return m_instance;
}

/*!
 */
void GL_error::report_error(const std::string & where)
{
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) return;

  const Uchar * error_str = gluErrorString(error);
  if (m_execution_coordinator) {
    std::string msg;
    msg.format("Error: occured in %s - %s", where.c_str(), error_str);
    m_execution_coordinator->SendErrorMessage(etGLError, msg);
  }
}
