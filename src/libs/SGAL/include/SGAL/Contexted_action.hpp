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

#ifndef SGAL_CONTEXTED_ACTION_HPP
#define SGAL_CONTEXTED_ACTION_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Node;
class Context;

/*! \class Contexted_action Contexted_action.hpp
 */
class SGAL_SGAL_DECL Contexted_action : public Action {
public:
  /*! Construct. */
  Contexted_action();

  /*! Destruct. */
  virtual ~Contexted_action();

  /*! Set the context. */
  virtual void set_context(Context* context);

  /*! Obtain the (const) context. */
  const Context* get_context() const;

  /*! Obtain the (mutable) context. */
  Context* get_context();

  /*! Apply node. */
  virtual Trav_directive apply(Shared_node node) = 0;

protected:
  /*! The current context. */
  Context* m_context;
};

//! \brief constructs.
inline Contexted_action::Contexted_action() : m_context(nullptr) {}

//! \brief destructs.
inline Contexted_action::~Contexted_action() {}

//! \brief sets the context.
inline void Contexted_action::set_context(Context* context)
{ m_context = context; }

//! \brief obtains the (const) context.
inline const Context* Contexted_action::get_context() const { return m_context; }

//! \brief obtains the (mutable) context.
inline Context* Contexted_action::get_context() { return m_context; }

SGAL_END_NAMESPACE

#endif
