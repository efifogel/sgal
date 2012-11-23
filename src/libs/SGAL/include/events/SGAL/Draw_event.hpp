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
// $Source: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DRAW_EVENT_HPP
#define SGAL_DRAW_EVENT_HPP

/*! \file
 * A record of an event that indicates that the scene should be drawn 
 */

#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;
class Window_item;

/*!
 */
class Draw_event: public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent *> s_set;

  /*! The window where the event took place */
  Window_item * m_window_item;

  /*! Do suppress accumulation? */
  Boolean m_suppress_accumulation;
  
protected:
  /*! Handle any agent */
  virtual void handle(Agent * agent);
  
  /*! Obtain set of agents registered to process this type of event */
  virtual const std::set<Agent *> & get_set(void) const { return s_set; }

public:
  /*! Register this event for a particular agent */
  static void doregister(Agent * agent) { s_set.insert(agent); }

  /*! Unregister this event for a particular agent */
  static void unregister(Agent * agent) { s_set.erase(agent); }

  /*! Constructor */
  Draw_event(void) :
    Event(),
    m_window_item(NULL),
    m_suppress_accumulation(SGAL_FALSE)
  {}

  /*! Destructor */
  virtual ~Draw_event(void) {}

  /*! Identify the event (for debugging purposes) */
  virtual void identify(void);

  /*! Set the window where the event took place */
  void set_window_item(Window_item * item) { m_window_item = item; }

  /*! Obtain the window where the event took place */
  Window_item * get_window_item() const { return m_window_item; }

  /*! Set the flag that indicates whether to suppress accumulation */
  void set_suppress_accumulation(Boolean flag) { m_suppress_accumulation = flag; }

  /*! Obtain the flag that indicates whether to suppress accumulation */
  Boolean get_suppress_accumulation() const { return m_suppress_accumulation; }
};

SGAL_END_NAMESPACE

#endif
