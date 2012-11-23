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
// $Id: Draw_event.cpp 5012 2007-09-03 07:24:36Z efif $
// $Revision: 5012 $
// $Name:  $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include<iostream>

#include "SGAL/Draw_event.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

std::set<Agent *> Draw_event::s_set;

/*! calls the appropriate agent to handle the current specific event */
void Draw_event::handle(Agent * agent) { agent->handle(this); }

/*!
 */
void Draw_event::identify(void) { std::cout << "Event: Draw" << std::endl; }

SGAL_END_NAMESPACE

