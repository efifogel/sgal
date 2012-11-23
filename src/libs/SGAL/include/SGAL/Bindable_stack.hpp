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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BINDABLE_STACK_HPP
#define SGAL_BINDABLE_STACK_HPP

/*! \file
 */

#include <vector>

#include "SGAL/Bindable_node.hpp"

SGAL_BEGIN_NAMESPACE

class Bindable_stack {
private:
  typedef std::vector<Bindable_node *>          Bindable_vector;
  typedef Bindable_vector::iterator             Bindable_iter;

  /*! The bindable stack */
  Bindable_vector m_bindable_stack;

public:
  /*! Bind the top-stack bindable element */
  void bind_top();
  
  /*! Obtain the bindable at the top os the stack */
  Bindable_node * top()
  {
    return m_bindable_stack.empty() ? NULL : m_bindable_stack.back();
  }

  /*! Erase the bindable */
  void erase(Bindable_node * bindable);
  
  /*! Pop the bindable at the top of the stack */
  void pop() { m_bindable_stack.pop_back(); }

  /*! Pushe a bindable onto the stack */
  void push(Bindable_node * bindable) { m_bindable_stack.push_back(bindable); }

  /*! Insert a bindable at the back of the stack */
  void insert(Bindable_node * bindable)
  {
    m_bindable_stack.insert(m_bindable_stack.begin(), bindable);
  }

  /*! Apply a unary operation to all bindable nodes in the stack */
  template <class Unary_func>
  void delegate(Unary_func & uf)
  {
    uf = std::for_each(m_bindable_stack.begin(), m_bindable_stack.end(), uf);
  }
};

SGAL_END_NAMESPACE

#endif
