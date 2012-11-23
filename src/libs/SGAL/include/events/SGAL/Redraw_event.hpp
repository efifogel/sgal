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

#ifndef SGAL_REDRAW_EVENT_HPP
#define SGAL_REDRAW_EVENT_HPP

/*! \file
 *
 */

#include <time.h>
#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;

/*! Redraw event */
class Redraw_event: public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent *> s_set;

protected:
  virtual void handle(Agent * agent);
  
public:
  static void doregister(Agent * agent) { s_set.insert(agent); }
  static void unregister(Agent * agent) { s_set.erase(agent); }
  virtual const std::set<Agent *> & get_set(void) const { return s_set; }

  /*! Constructor */
  Redraw_event(void) : Event() {}

  /*! Destructor */
  virtual ~Redraw_event(void) {}
  
  virtual void identify(void);
};

SGAL_END_NAMESPACE

#endif
