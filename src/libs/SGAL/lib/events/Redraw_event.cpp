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

#include <iostream>

#include "SGAL/Redraw_event.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

std::set<Agent*> Redraw_event::s_set;

//! \brief deligates the handling of the current event to the given agent.
void Redraw_event::handle(Agent* agent) { agent->handle(this); }

//! \brief exports an identification message to standard output.
void Redraw_event::identify(void) { std::cout << "Redraw" << std::endl; }

//! \brief registers this event for a particular agent.
void Redraw_event::doregister(Agent* agent) { s_set.insert(agent); }

//! \brief unregisters this event for a particular agent.
void Redraw_event::unregister(Agent* agent) { s_set.erase(agent); }

SGAL_END_NAMESPACE
