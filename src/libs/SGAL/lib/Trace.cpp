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
// $Id: $
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

Trace * Trace::s_instance = 0;

/*! Returns a pointer to the factory and makes sure only one instance exits */
Trace * Trace::get_instance() 
{
  if (!s_instance) s_instance = new Trace();
  return s_instance;
}

void Trace::enable(unsigned int my_signature) { m_signature |= my_signature; }

SGAL_END_NAMESPACE
