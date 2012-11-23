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
// $Revision: 1308 $
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
#include "SGAL/Event.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;

/*!
 */
class Tick_event: public Event {
private:
  /*! A set of agents registered to process this type of event */
  static std::set<Agent *> s_set;
  static unsigned int s_num_ticks;
  
  clock_t m_est_tick_duration;
  clock_t m_sim_time;

protected:
  virtual void handle(Agent * agent);
  
public:
  static void doregister(Agent * agent) { s_set.insert(agent); }
  static void unregister(Agent * agent) { s_set.erase(agent); }
  virtual const std::set<Agent *> & get_set(void) const { return s_set; }

  Tick_event(void) : Event(), m_est_tick_duration(0), m_sim_time(0)
  { s_num_ticks++; }
  virtual ~Tick_event(void) {}
  
  virtual void identify(void);

  /*!
   */
  void set_est_tick_duration(clock_t est_tick_duration)
  { m_est_tick_duration = est_tick_duration; }

  /*!
   */
  clock_t get_est_tick_duration() const { return m_est_tick_duration; }

  /*!
   */
  void set_sim_time(clock_t sim_time) { m_sim_time = sim_time; }

  /*!
   */
  clock_t get_sim_time(void) const { return m_sim_time; }

  /*!
   */
  unsigned int get_num_ticks() const;
};

/*!
 */
inline unsigned int Tick_event::get_num_ticks() const { return s_num_ticks; }

SGAL_END_NAMESPACE

#endif
