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

#ifndef SGAL_GL_ERROR_HPP
#define SGAL_GL_ERROR_HPP

/*! \file
 *
 */

#include <string>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Execution_coordinator;

class GL_error {
public:
  virtual ~GL_error(){}
  static GL_error * get_instance();

  void report_error(const std::string & msg);
  void set_ec(Execution_coordinator * coordinator)
  {
    m_execution_coordinator = coordinator;
  }

private:
  static GL_error * m_instance;

  Execution_coordinator * m_execution_coordinator;

  GL_error();
  GL_error(const GL_error &) {}
};

SGAL_END_NAMESPACE

#endif
