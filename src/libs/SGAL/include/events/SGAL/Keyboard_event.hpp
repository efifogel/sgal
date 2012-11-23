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

/*! The keyboard event. Holds data related to the event of pressing or
 * releasing a key by the user
 */
class Keyboard_event: public Event {
public:
  enum Modifier {
    SHIFT_KEY = 0x1,
    CTRL_KEY =  0x2,
    ALT_KEY =   0x4
  };

private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent *> s_set;

  /*! The window where the event took place */
  Window_item * m_window_item;

  /*! The key pressed or released */
  Uint m_key;

  /*! The x-coordinate of the cursor at the time of the event */
  Int m_x;

  /*! The y-coordinate of the cursor at the time of the event */
  Int m_y;

  /*! Indicates whether the key is pressed or released */
  Boolean m_pressed;
  
protected:
  /*! Handle any agent */
  virtual void handle(Agent * agent);
  
  /*! Obtain set of agents registered to process this type of event */
  virtual const std::set<Agent *> & get_set(void) const { return s_set; }

public:
  /*! Register this event for a particular agent */
  static void doregister(Agent * agent) { s_set.insert(agent); }

  /*! Unregisters this event for a particular agent */
  static void unregister(Agent * agent) { s_set.erase(agent); }

  /*! Constructor */
  Keyboard_event(void) :
    Event(),
    m_window_item(NULL),
    m_key('\0'), m_x(0), m_y(0), m_pressed(SGAL_FALSE)
  {}

  /*! Destructor */
  virtual ~Keyboard_event(void) {}

  /*! Identify the event (for debugging purposes) */
  virtual void identify(void);

  /*! Set the window where the event took place */
  void set_window_item(Window_item * item) { m_window_item = item; }

  /*! Obtain the window where the event took place */
  Window_item * get_window_item() const { return m_window_item; }
  
  /*! Set the key pressed or released */
  void set_key(Uint key) { m_key = key; }

  /*! Obtain the key pressed or released */
  Uint get_key() const { return m_key; }

  /*! Set the x-coordinate of the cursor at the time of the event  */
  void set_x(Int x) { m_x = x; }

  /*! Obtains the x-coordinate of the cursor at the time of the event  */
  Int get_x() const { return m_x; }

  /*! Sets the y-coordinate of the cursor at the time of the event  */
  void set_y(Int y) { m_y = y; }

  /*! Obtains the y-coordinate of the cursoor at the time of the event */
  Int get_y() const { return m_y; }

  /*! Set the flag that indicates whether the key is pressed or released */
  void set_pressed(Boolean pressed) { m_pressed = pressed; }

  /*! Return true if the key is pressed, and false if the key is released */
  Boolean get_pressed() const { return m_pressed; }
};

SGAL_END_NAMESPACE

#endif
