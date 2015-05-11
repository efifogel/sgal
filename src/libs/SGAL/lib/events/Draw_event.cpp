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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include<iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Draw_event.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

std::set<Agent*> Draw_event::s_set;

/*! Constructor */
Draw_event::Draw_event(void) :
  Event(),
  m_window_item(NULL),
  m_suppress_accumulation(false)
{}

//! deligates the handling of the current event to the given agent.
void Draw_event::handle(Agent* agent) { agent->handle(this); }

//! exports an identification message to standard output.
void Draw_event::identify(void) { std::cout << "Event: Draw" << std::endl; }

//! \brief registers this event for a particular agent.
void Draw_event::doregister(Agent* agent) { s_set.insert(agent); }

//! \brief unregisters this event for a particular agent.
void Draw_event::unregister(Agent* agent) { s_set.erase(agent); }

SGAL_END_NAMESPACE
