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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_KEYBOARD_EVENT_HPP
#define SGAL_KEYBOARD_EVENT_HPP

/*! \file
 * A record that represents an event issued when one of the keyboard keys
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

/*! The keyboard event. Holds data related to the event of pressing or
 * releasing a key by the user.
 */
class SGAL_SGAL_DECL Keyboard_event : public Event {
public:
  enum Modifier {
    SHIFT_KEY = 0x1,
    CTRL_KEY =  0x2,
    ALT_KEY =   0x4
  };

private:
  /*! The set of agents registered to process this type of event. */
  static std::set<Agent*> s_set;

  /*! The window where the event took place. */
  Window_item* m_window_item;

  /*! The key pressed or released. */
  Uint m_key;

  /*! The x-coordinate of the cursor at the time of the event. */
  Int m_x;

  /*! The y-coordinate of the cursor at the time of the event. */
  Int m_y;

  /*! Indicates whether the key is pressed or released. */
  Boolean m_pressed;

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
  Keyboard_event(void);

  /*! Destructor */
  virtual ~Keyboard_event(void);

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /*! Set the window where the event took place. */
  void set_window_item(Window_item* item);

  /*! Obtain the window where the event took place. */
  Window_item* get_window_item() const;

  /*! Set the key pressed or released. */
  void set_key(Uint key);

  /*! Obtain the key pressed or released. */
  Uint get_key() const;

  /*! Set the x-coordinate of the cursor at the time of the event. */
  void set_x(Int x);

  /*! Obtain the x-coordinate of the cursor at the time of the event. */
  Int get_x() const;

  /*! Set the y-coordinate of the cursor at the time of the event. */
  void set_y(Int y);

  /*! Obtain the y-coordinate of the cursoor at the time of the event. */
  Int get_y() const;

  /*! Set the flag that indicates whether the key is pressed or released. */
  void set_pressed(Boolean pressed);

  /*! Determine whether the key is pressed or released. */
  Boolean get_pressed() const;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

/*! \brief destructor */
inline Keyboard_event::~Keyboard_event(void) {}

/*! \brief obtains set of agents registered to process this type of event. */
inline const std::set<Agent*>& Keyboard_event::get_set(void) const
{ return s_set; }

/*! \brief sets the window where the event took place. */
inline void Keyboard_event::set_window_item(Window_item* item)
{ m_window_item = item; }

/*! \brief obtains the window where the event took place. */
inline Window_item* Keyboard_event::get_window_item() const
{ return m_window_item; }

/*! \brief sets the key pressed or released. */
inline void Keyboard_event::set_key(Uint key) { m_key = key; }

/*! \brief obtains the key pressed or released. */
inline Uint Keyboard_event::get_key() const { return m_key; }

/*! \brief sets the x-coordinate of the cursor at the time of the event. */
inline void Keyboard_event::set_x(Int x) { m_x = x; }

/*! \brief obtains the x-coordinate of the cursor at the time of the event. */
inline Int Keyboard_event::get_x() const { return m_x; }

/*! \brief sets the y-coordinate of the cursor at the time of the event. */
inline void Keyboard_event::set_y(Int y) { m_y = y; }

/*! \brief obtains the y-coordinate of the cursoor at the time of the event. */
inline Int Keyboard_event::get_y() const { return m_y; }

/*! \brief sets the flag that indicates whether the key is pressed or released.
 */
inline void Keyboard_event::set_pressed(Boolean pressed)
{ m_pressed = pressed; }

/*! \brief determines whether the key is pressed or released. */
inline Boolean Keyboard_event::get_pressed() const { return m_pressed; }

SGAL_END_NAMESPACE

#endif
