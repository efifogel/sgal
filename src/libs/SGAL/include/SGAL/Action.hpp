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

#ifndef SGAL_ACTION_HPP
#define SGAL_ACTION_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Node;

/*! \class Action Action.hpp
 */
class SGAL_SGAL_DECL Action {
public:
  typedef enum {TRAV_CONT = 0, TRAV_PRUNE = 1, TRAV_STOP = 2} Trav_directive;

  typedef boost::shared_ptr<Node>                       Shared_node;

  /*! Construct. */
  Action();

  /*! Destruct. */
  virtual ~Action();

  /*! Apply the action to a node.
   * \param[in] node the node to apply the action to.
   */
  virtual Trav_directive apply(Shared_node node) = 0;
};

//! \brief constructs.
inline Action::Action() {}

//! \brief destructs.
inline Action::~Action() {}

SGAL_END_NAMESPACE

#endif
