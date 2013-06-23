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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_MOTION_EVENT_HPP
#define SGAL_MOTION_EVENT_HPP

/*! \file
 * A record that represents an event issued when the cursor moves while at
 * least one of the mouse buttons is pressed
 */

#include <time.h>
#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;
class Window_item;

/*!
 */
class SGAL_SGAL_DECL Motion_event: public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent*> s_set;
  
  /*! The window where the event took place */
  Window_item* m_window_item;

  /*! The x coordinate of the cursor location at the time of the event */
  int m_x;

  /*! The y coordinate of the cursor location at the time of the event */
  int m_y;

protected:
  /*! Handle any agent */
  virtual void handle(Agent* agent);

  /*! Obtain set of agents registered to process this type of event */
  virtual const std::set<Agent*>& get_set(void) const { return s_set; }

public:
  /*! Register this event for a particular agent */
  static void doregister(Agent* agent) { s_set.insert(agent); }

  /*! Unregisters this event for a particular agent */
  static void unregister(Agent* agent) { s_set.erase(agent); }

  /*! Constructor */
  Motion_event(void) :
    Event(),
    m_window_item(NULL),
    m_x(0), m_y(0)
  {}

  /*! Destructor */
  virtual ~Motion_event(void) {}

  /*! Identify the event (for debugging purposes) */
  virtual void identify(void);
  
  /*! Set the window where the event took place */
  void set_window_item(Window_item* item) { m_window_item = item; }

  /*! Obtain the window where the event took place */
  Window_item* get_window_item() const { return m_window_item; }

  /*! Set the x-coordinate of the cursor at the time of the event */
  void set_x(int x) { m_x = x; }

  /*! Obtain the x-coordinate of the cursor at the time of the event */
  int get_x() const { return m_x; }

  /*! Set the y-coordinate of the cursor at the time of the event */
  void set_y(int y) { m_y = y; }

  /*! Obtain the x-coordinate of the cursor at the time of the event */
  int get_y() const { return m_y; }
};

SGAL_END_NAMESPACE

#endif
