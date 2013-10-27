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
// $Id $
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EVENT_HANDLER_HPP
#define SGAL_EVENT_HANDLER_HPP

/*! \file
 * \brief event handling.
 */

#include <time.h>
#include <queue>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Event;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*!
 */
class SGAL_SGAL_DECL Event_handler {
public:
  void process();

  static void issue(Event* event);

  /*! Set the verbosity level.
   * \param level the verbosity level.
   */
  static void set_verbose_level(Uint level);

  /*! Determine whether the event queue is empty.
   * \return true if the event queue is empty and false otherwise.
   */
  static Boolean is_empty(void);

private:
  /*! The verbosity level. */
  static Uint s_verbose_level;

  typedef std::queue<Event*> Event_queue;
  static Event_queue s_queue;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

/*! \brief sets the verbosity level. */
inline void Event_handler::set_verbose_level(Uint verbose_level)
{ s_verbose_level = verbose_level; }

SGAL_END_NAMESPACE

#endif
