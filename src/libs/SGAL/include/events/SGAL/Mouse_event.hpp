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

#ifndef SGAL_MOUSE_EVENT_HPP
#define SGAL_MOUSE_EVENT_HPP

/*! \file
 * A record that represents an event issued when one of the mouse buttons
 * is pressed or released
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
class SGAL_SGAL_DECL Mouse_event : public Event {
public:
  enum Press {
    DOWN = 1,
    UP =   0
  };

  enum Mouse_button {
    LEFT_BUTTON =   0x1,
    MIDDLE_BUTTON = 0x2,
    RIGHT_BUTTON =  0x4
  };

private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent*> s_set;

  /*! The window where the event took place */
  Window_item* m_window_item;

  /*! The mouse button pressed or released */
  Mouse_button m_button;

  /*! The state of the button */
  Press m_state;

  /*! The x-coordinate of the cursor at the time of the event  */
  Uint m_x;

  /*! The y-coordinate of the cursor at the time of the event */
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
  Mouse_event(void);

  /*! Destructor */
  virtual ~Mouse_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /*! Set the window where the event took place */
  void set_window_item(Window_item* item);

  /*! Obtain the window where the event took place */
  Window_item* get_window_item() const;

  /*! Set the mouse button pressed or released */
  void set_button(Mouse_button button);

  /*! Obtain the mouse button pressed or released */
  Mouse_button get_button() const;

  /*! Set the state of the button */
  void set_state(Press state);

  /*! Obtain the state of the button */
  Press get_state() const;

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

/*! \brief obtains the set of agents registered to process this type of event.
 */
inline const std::set<Agent*>& Mouse_event::get_set(void) const
{ return s_set; }

/*! \brief sets the window where the event took place */
inline void Mouse_event::set_window_item(Window_item* item)
{ m_window_item = item; }

/*! \brief obtains the window where the event took place */
inline Window_item* Mouse_event::get_window_item() const
{ return m_window_item; }

/*! \brief sets the mouse button pressed or released */
inline void Mouse_event::set_button(Mouse_button button) { m_button = button; }

/*! \brief obtains the mouse button pressed or released */
inline Mouse_event::Mouse_button Mouse_event::get_button() const
{ return m_button; }

/*! \brief sets the state of the button */
inline void Mouse_event::set_state(Press state) { m_state = state; }

/*! \brief obtains the state of the button */
inline Mouse_event::Press Mouse_event::get_state() const { return m_state; }

/*! \brief sets the x-coordinate of the cursor at the time of the event */
inline void Mouse_event::set_x(Uint x) { m_x = x; }

/*! \brief obtains the x-coordinate of the cursor at the time of the event */
inline Uint Mouse_event::get_x() const { return m_x; }

/*! \brief sets the y-coordinate of the cursor at the time of the event */
inline void Mouse_event::set_y(Uint y) { m_y = y; }

/*! \brief obtains the x-coordinate of the cursor at the time of the event */
inline Uint Mouse_event::get_y() const { return m_y; }

SGAL_END_NAMESPACE

#endif
