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
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#include "SGAL/Motion_event.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

std::set<Agent *> Motion_event::s_set;

/*! Constructor */
Motion_event::Motion_event(void) :
  Event(),
  m_window_item(NULL),
  m_x(0), m_y(0)
{}

/*! \brief deligates the handling of the current event to the given agent. */
void Motion_event::handle(Agent * agent) { agent->handle(this); }

/*! \brief exports an identification message to standard output. */
void Motion_event::identify(void) { std::cout << "Event: Motion" << std::endl; }

/*! \brief registers this event for a particular agent. */
void Motion_event::doregister(Agent* agent) { s_set.insert(agent); }

/*! \brief unregisters this event for a particular agent. */
void Motion_event::unregister(Agent* agent) { s_set.erase(agent); }

SGAL_END_NAMESPACE
