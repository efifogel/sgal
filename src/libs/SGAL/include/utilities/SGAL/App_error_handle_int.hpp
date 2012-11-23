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
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_APP_ERROR_HANDLE_INT_HPP
#define SGAL_APP_ERROR_HANDLE_INT_HPP

/*! \file
 * The application provide this interface to any contained manager that need
 * informing the plugin user about internal errors
 */

#include <string>

enum Error_type {
  etDbgInfo = 0,      // <debug message>
  etFatal,            // <error message>
  etInvalidName,      // <invalid name>|<description>
  etNodeRoute,        // <node1 name>|<node2 name>|<description>
  etFieldRoute,       // <field1 name>|<field2 name>|<description>
  etGLError           // <where>|<description>
};

class App_error_handle_int {
public :
  App_error_handle_int() {}
  virtual ~App_error_handle_int() {}
	
  virtual void Send_error_message(Error_type type, std::string & error) = 0;
};

#endif
