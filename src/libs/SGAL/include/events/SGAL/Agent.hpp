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
// $Revision: 4990 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_AGENT_HPP
#define SGAL_AGENT_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_event;
class Keyboard_event;
class Motion_event;
class Mouse_event;
class Passive_motion_event;
class Redraw_event;
class Reset_event;
class Reshape_event;
class Simulate_event;
class Start_event;
class Stop_event;
class Tick_event;

class SGAL_SGAL_DECL Agent {
public:
  /*! Destructor */
  virtual ~Agent() {}
  
  virtual void handle(Draw_event*) {}
  virtual void handle(Keyboard_event*) {}
  virtual void handle(Motion_event*) {}
  virtual void handle(Mouse_event*) {}
  virtual void handle(Passive_motion_event*) {}
  virtual void handle(Redraw_event*) {}
  virtual void handle(Reset_event*) {}
  virtual void handle(Reshape_event*) {}
  virtual void handle(Simulate_event*) {}
  virtual void handle(Start_event*) {}
  virtual void handle(Stop_event*) {}
  virtual void handle(Tick_event*) {}

  virtual void identify(void) = 0;
};

SGAL_END_NAMESPACE

#endif
