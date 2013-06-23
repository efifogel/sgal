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

#ifndef SGAL_EVENT_HANDLER_HPP
#define SGAL_EVENT_HANDLER_HPP

/*! \file
 * \brief event handling.
 */

#include <time.h>
#include <queue>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Event;

/*!
 */ 
class SGAL_SGAL_DECL Event_handler {
public:
  void process();
  static void issue(Event* event);

  /*!
   */
  static void set_verbose_level(int verbose_level)
  { s_verbose_level = verbose_level; }

  /*!
   */
  static bool is_empty(void);
  
private:
  static int s_verbose_level;

  typedef std::queue<Event*> Event_queue;
  static Event_queue s_queue;
};

SGAL_END_NAMESPACE

#endif
