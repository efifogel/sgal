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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Event_handler.hpp"
#include "SGAL/Event.hpp"
#include "SGAL/Tracer.hpp"

SGAL_BEGIN_NAMESPACE

Uint Event_handler::s_verbose_level(0);
Event_handler::Event_queue Event_handler::s_queue;

//! \brief
void Event_handler::process()
{
  while (!s_queue.empty()) {
    Event* event = s_queue.front();
    s_queue.pop();
    SGAL_TRACE_CODE(Tracer::EVENTS, event->identify(););
    event->handle();
    delete event;
  }
}

//! \brief
void Event_handler::issue(Event* event) { s_queue.push(event); }

//! \brief
Boolean Event_handler::is_empty(void) { return s_queue.empty(); }

SGAL_END_NAMESPACE
