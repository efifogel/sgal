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

#ifndef SGAL_ACTION_HPP
#define SGAL_ACTION_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"

SGAL_BEGIN_NAMESPACE

class Node;
class Context;

class SGAL_CLASSDEF Action {
public:
  typedef enum {TRAV_CONT = 0, TRAV_PRUNE = 1, TRAV_STOP = 2} Trav_directive;

  /*! Constructor */
  Action() : m_context(NULL) {}

  /*! Destructor */
  virtual ~Action() {}

  /*! Set the context */
  virtual void set_context(Context *context) { m_context = context; }

  /*! Obtain the context */
  Context * get_context() { return m_context; }

  /*! Apply node */
  virtual Trav_directive apply(Node * node) = 0;
   
protected:
  /*! The current context */
  Context * m_context;
};

SGAL_END_NAMESPACE

#endif
