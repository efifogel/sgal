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
// $Id: $
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

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*!
 */
class SGAL_SGAL_DECL Draw_event : public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent*> s_set;

  /*! The window where the event took place */
  Window_item* m_window_item;

  /*! Do suppress accumulation? */
  Boolean m_suppress_accumulation;

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
  /*! Constructor */
  Draw_event(void);

  /*! Destructor */
  virtual ~Draw_event(void) {}

  /*! Register this event for a particular agent.
   * \param agent the agent.
   */
  static void doregister(Agent* agent);

  /*! Unregister this event for a particular agent.
   * \param agent the agent.
   */
  static void unregister(Agent* agent);

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /*! Set the window where the event took place. */
  void set_window_item(Window_item* item);

  /*! Obtain the window where the event took place. */
  Window_item* get_window_item() const;

  /*! Set the flag that indicates whether to suppress accumulation. */
  void set_suppress_accumulation(Boolean flag);

  /*! Obtain the flag that indicates whether to suppress accumulation. */
  Boolean get_suppress_accumulation() const;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains set of agents registered to process this type of event.
inline const std::set<Agent*>& Draw_event::get_set(void) const
{ return s_set; }

//! \brief sets the window where the event took place.
inline void Draw_event::set_window_item(Window_item* item)
{ m_window_item = item; }

//! \brief obtains the window where the event took place.
inline Window_item* Draw_event::get_window_item() const
{ return m_window_item; }

//! \brief sets the flag that indicates whether to suppress accumulation.
inline void Draw_event::set_suppress_accumulation(Boolean flag)
{ m_suppress_accumulation = flag; }

//! \brief obtains the flag that indicates whether to suppress accumulation.
inline Boolean Draw_event::get_suppress_accumulation() const
{ return m_suppress_accumulation; }

SGAL_END_NAMESPACE

#endif
