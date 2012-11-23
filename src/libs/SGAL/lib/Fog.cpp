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

#include "SGAL/basic.hpp"
#include "SGAL/Fog.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

REGISTER_TO_FACTORY(Fog, "Fog");

/*! Constructor */
Fog::Fog(Boolean proto) : Node(proto) {}

/*! Destructor */
Fog::~Fog() {}

void Fog::SetOn(Boolean on)
{
}

Boolean Fog::GetOn()
{
  return SGAL_TRUE;
}

void Fog::Push()
{
}

void Fog::Pop(Fog * outgoingFog)
{
}

Trav_directive Fog::Draw(Draw_action * draw_action)
{
  return Trav_cont;
}

SGAL_END_NAMESPACE
