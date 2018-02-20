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

#ifndef SGAL_TICK_EVENT_HPP
#define SGAL_TICK_EVENT_HPP

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

/*!
 */
class SGAL_SGAL_DECL Tick_event: public Event {
private:
  /*! A set of agents registered to process this type of event. */
  static std::set<Agent*> s_set;

  /*! Total number of ticks. */
  static Uint s_num_ticks;

  /*! Estimated tick duration in seconds. */
  Scene_time m_est_tick_duration;

  /*! Accumulated Simulation time in seconds. */
  Scene_time m_sim_time;

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
  Tick_event(void);

  /*! Destructor */
  virtual ~Tick_event(void) {}

  /*! Export an identification message to standard output. */
  virtual void identify(void);

  /* Set the estimated tick duration.
   * \param est_tick_duration (in) the estimated tick duration.
   */
  void set_est_tick_duration(Scene_time est_tick_duration);

  /*! Obtain the estimated tick duration.
   * \return the estimated tick duration.
   */
  Scene_time get_est_tick_duration() const;

  /*! Set the accumulated simulation time.
   * \param sim_time (in) the accumulated simulation time.
   */
  void set_sim_time(Scene_time sim_time);

  /*! Obtain the accumulated simulation time.
   * \return the accumulated simulation time.
   */
  Scene_time get_sim_time(void) const;

  /*! Obtain the total number of ticks.
   * \return the total number of ticks.
   */
  unsigned int get_num_ticks() const;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the total number of ticks.
inline Uint Tick_event::get_num_ticks() const { return s_num_ticks; }

//! \brief obtains the set of agents registered to process this type of event.
inline const std::set<Agent*>& Tick_event::get_set(void) const
{ return s_set; }

//! \brief sets the estimated tick duration.
inline void Tick_event::set_est_tick_duration(Scene_time est_tick_duration)
{ m_est_tick_duration = est_tick_duration; }

//! \brief obtains the estimated tick duration.
inline Scene_time Tick_event::get_est_tick_duration() const
{ return m_est_tick_duration; }

//! \brief sets the accumulated simulation time.
inline void Tick_event::set_sim_time(Scene_time sim_time)
{ m_sim_time = sim_time; }

//! \brief obtains the accumulated simulation time.
inline Scene_time Tick_event::get_sim_time(void) const { return m_sim_time; }

SGAL_END_NAMESPACE

#endif
