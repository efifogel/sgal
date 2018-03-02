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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Proto.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Proto::Proto(Boolean proto) {}

//! \brief destructs.
Proto::~Proto() {}

//! \brief deletes the container prototype.
void Proto::delete_prototype()
{
  if (!m_prototype) return;
  delete m_prototype;
  m_prototype = nullptr;
}

//! \brief sets the attributes of the object extracted from the input file.
void Proto::set_attributes(Element* elem)
{
}

SGAL_END_NAMESPACE
