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

#ifndef SGAL_BINDABLE_STACK_HPP
#define SGAL_BINDABLE_STACK_HPP

#include <vector>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Bindable_node;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Bindable_stack {
private:
  typedef std::vector<Bindable_node*>           Bindable_vector;
  typedef Bindable_vector::iterator             Bindable_iter;

  /*! The bindable stack. */
  Bindable_vector m_bindable_stack;

public:
  /*! Bind the top-stack bindable element.
   * This is typically used during intialization, to activate a bindable the
   * first time
   */
  void bind_top();

  /*! Obtain the mutable bindable at the top os the stack.
   */
  Bindable_node* top();

  /*! Obtain the const bindable at the top os the stack.
   */
  const Bindable_node* top() const;

  /*! Erase the bindable. */
  void erase(const Bindable_node* bindable);

  /*! Pop the bindable at the top of the stack.
   */
  void pop();

  /*! Push a bindable onto the stack.
   */
  void push(Bindable_node* bindable);

  /*! Obtain the size of the stack.
   * \return the size of the stack.
   */
  size_t size() const;

  /*! Insert a bindable at the back of the stack.
   */
  void insert(Bindable_node* bindable);

  /*! Apply a unary operation to all bindable nodes in the stack.
   */
  template <typename UnaryFunc>
  void delegate(UnaryFunc& uf);
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the mutable bindable at the top os the stack.
inline Bindable_node* Bindable_stack::top()
{ return m_bindable_stack.empty() ? nullptr : m_bindable_stack.back(); }

//! \brief obtains the const bindable at the top os the stack.
inline const Bindable_node* Bindable_stack::top() const
{ return m_bindable_stack.empty() ? nullptr : m_bindable_stack.back(); }

//! \rief pops the bindable at the top of the stack.
inline void Bindable_stack::pop() { m_bindable_stack.pop_back(); }

//! \brief pushes a bindable onto the stack.
inline void Bindable_stack::push(Bindable_node* bindable)
{ m_bindable_stack.push_back(bindable); }

//! \brief inserts a bindable at the back of the stack.
inline void Bindable_stack::insert(Bindable_node* bindable)
{ m_bindable_stack.insert(m_bindable_stack.begin(), bindable); }

//! \brief applies a unary operation to all bindable nodes in the stack.
template <typename UnaryFunc>
inline void Bindable_stack::delegate(UnaryFunc& uf)
{ uf = std::for_each(m_bindable_stack.begin(), m_bindable_stack.end(), uf); }

//! \brief obtains the size of the stack.
inline size_t Bindable_stack::size() const { return m_bindable_stack.size(); }

SGAL_END_NAMESPACE

#endif
