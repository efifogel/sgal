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

#ifndef SGAL_REDRAW_EVENT_HPP
#define SGAL_REDRAW_EVENT_HPP

/*! \file
 *
 */

#include <time.h>
#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Redraw event */
class SGAL_SGAL_DECL Redraw_event : public Event {
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

  /*! Constructor */
  Redraw_event(void) : Event() {}

  /*! Destructor */
  virtual ~Redraw_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the set of agents registered to process this type of event.
inline const std::set<Agent*>& Redraw_event::get_set(void) const
{ return s_set; }

SGAL_END_NAMESPACE

#endif
