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

#ifndef SGAL_START_EVENT_HPP
#define SGAL_START_EVENT_HPP

/*! \file
 *
 */

#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;

/*!
 */
class SGAL_SGAL_DECL Start_event : public Event {
private:
  /*! A set of agents registered to process this type of event. */
  static std::set<Agent*> s_set;

  /*! Obtain the set of agents registered to process this type of event.
   * \return the set of agents.
   */
  virtual const std::set<Agent*>& get_set(void) const;

protected:
  /*! Deligate the handling of the current event to the given agent.
   * \param agent the agent.
   */
  virtual void handle(Agent* agent);
  
public:
  /*! Register this event for a particular agent.
   * \param agent the agent.
   */
  static void doregister(Agent* agent);

  /*! Unregister this event for a particular agent.
   * \param agent the agent.
   */
  static void unregister(Agent* agent);

  /*! Destructor */
  virtual ~Start_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);
};

/*! \brief obtains the set of agents registered to process this type of event.
 */
inline const std::set<Agent*>& Start_event::get_set(void) const
{ return s_set; }

SGAL_END_NAMESPACE

#endif
