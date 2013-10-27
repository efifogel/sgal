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

#ifndef SGAL_EVENT_HPP
#define SGAL_EVENT_HPP

/*! \file
 *
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <set>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Agent;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*!
 */
class SGAL_SGAL_DECL Event {
private:
  int m_verbose_level;

  virtual void handle(Agent* agent) = 0;
  virtual const std::set<Agent*>& get_set(void) const = 0;

public:
  /*!
   */
  Event() : m_verbose_level(0) {}

  /*!
   */
  virtual ~Event() {}

  /*!
   */
  virtual void identify(void) = 0;

  /*!
   */
  void handle(void);

  /*!
   */
  void set_verbose_level(int verbose_level) { m_verbose_level = verbose_level; }
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
