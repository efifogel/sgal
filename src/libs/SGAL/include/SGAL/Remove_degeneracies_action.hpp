// Copyright (c) 2016 Israel.
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

#ifndef SGAL_REMOVE_DEGENERACIES_ACTION_HPP
#define SGAL_REMOVE_DEGENERACIES_ACTION_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Node;
class Shape;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Remove_degeneracies_action
 * Remove_degeneracies_action.hpp
 */
class SGAL_SGAL_DECL Remove_degeneracies_action : public Action {
public:
  typedef boost::shared_ptr<Node>       Shared_node;

  /*! Construct.
   */
  Remove_degeneracies_action();

  /*! Destruct. */
  virtual ~Remove_degeneracies_action();

  /*! Apply the action to a given node.
   * \param[in] node  the node to apply the action on.
   */
  virtual Trav_directive apply(Shared_node node);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
