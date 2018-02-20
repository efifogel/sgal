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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_RESHAPE_EVENT_HPP
#define SGAL_RESHAPE_EVENT_HPP

/*! \file
 * A record that represents an window reshape event
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
class SGAL_SGAL_DECL Reshape_event : public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent*> s_set;

  /*! The window that is being reshaped */
  Window_item* m_window_item;

  /*! The width of the window being reshaped */
  Uint m_width;

  /*! The height of the window being reshaped */
  Uint m_height;

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

  /*! Unregister this event for a particular agent */
  static void unregister(Agent* agent);

  /*! Constructor */
  Reshape_event(void);

  /*! Destructor */
  virtual ~Reshape_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /*! Set the window item */
  void set_window_item(Window_item* item);

  /*! Obtain the window where the event took place */
  Window_item* get_window_item() const;

  /*! Set the window width */
  void set_width(Uint width);

  /*! Obtain the window width */
  Uint get_width() const;

  /*! Set the window height */
  void set_height(Uint height);

  /*! Obtain the window height */
  Uint get_height() const;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the set of agents registered to process this type of event.
inline const std::set<Agent*>& Reshape_event::get_set(void) const
{ return s_set; }

//! \brief sets the window width.
inline void Reshape_event::set_width(Uint width) { m_width = width; }

//! \brief obtains the window width.
inline Uint Reshape_event::get_width() const { return m_width; }

//! \brief sets the window height.
inline void Reshape_event::set_height(Uint height) { m_height = height; }

//! \brief obtains the window height.
inline Uint Reshape_event::get_height() const { return m_height; }

SGAL_END_NAMESPACE

#endif
