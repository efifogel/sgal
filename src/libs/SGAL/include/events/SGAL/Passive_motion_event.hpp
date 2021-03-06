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

#ifndef SGAL_PASSIVE_MOTION_EVENT_HPP
#define SGAL_PASSIVE_MOTION_EVENT_HPP

/*! \file
 * A record that represents an event issued when the cursor moves while none
 * of the mouse buttons are pressed
 */

#include <time.h>
#include <set>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;
class Window_item;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*!
 */
class SGAL_SGAL_DECL Passive_motion_event : public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent*> s_set;

  /*! The window where the event took place */
  Window_item* m_window_item;

  /*! The x coordinate of the cursor location at the time of the event */
  Uint m_x;

  /*! The y coordinate of the cursor location at the time of the event */
  Uint m_y;

protected:
  /*! Deligate the handling of the current event to the given agent.
   * \param agent the agent.
   */
  virtual void handle(Agent* agent);

  /*! Obtain the set of agents registered to process this type of event.
   * \return the set of agents.
   */
  virtual const std::set<Agent*>& get_set(void) const;

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
  Passive_motion_event(void);

  /*! Destructor */
  virtual ~Passive_motion_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /*! Set the window where the event took place */
  void set_window_item(Window_item* item);

  /*! Obtain the window where the event took place */
  Window_item* get_window_item() const;

  /*! Set the x-coordinate of the cursor at the time of the event */
  void set_x(Uint x);

  /*! Obtain the x-coordinate of the cursor at the time of the event */
  Uint get_x() const;

  /*! Set the y-coordinate of the cursor at the time of the event */
  void set_y(Uint y);

  /*! Obtain the x-coordinate of the cursor at the time of the event */
  Uint get_y() const;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the set of agents registered to process this type of event.
inline const std::set<Agent*>& Passive_motion_event::get_set(void) const
{ return s_set; }

//! \brief sets the window where the event took place.
inline void Passive_motion_event::set_window_item(Window_item* item)
{ m_window_item = item; }

//! \brief obtains the window where the event took place.
inline Window_item* Passive_motion_event::get_window_item() const
{ return m_window_item; }

//! \brief sets the x-coordinate of the cursor at the time of the event.
inline void Passive_motion_event::set_x(Uint x) { m_x = x; }

//! \brief obtains the x-coordinate of the cursor at the time of the event.
inline Uint Passive_motion_event::get_x() const { return m_x; }

//! \brief sets the y-coordinate of the cursor at the time of the event.
inline void Passive_motion_event::set_y(Uint y) { m_y = y; }

//! \brief obtains the x-coordinate of the cursor at the time of the event.
inline Uint Passive_motion_event::get_y() const { return m_y; }

SGAL_END_NAMESPACE

#endif
