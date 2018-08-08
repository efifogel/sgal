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

#include<iostream>

#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Event.hpp"
#include "SGAL/Agent.hpp"
#include "SGAL/Tracer.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief
void Event::handle(void)
{
  const auto& my_set = get_set();

  // Copy the set to a temporary place holder, because the handlers may
  // register and unregister agents to the original set.
  std::set<Agent *> tmp = my_set;
  for (auto it = tmp.begin(); it != tmp.end(); ++it) {
    SGAL_TRACE_CODE(Tracer::EVENTS, (*it)->identify(););
    handle(*it);
  }
  tmp.clear();
}

SGAL_END_NAMESPACE
