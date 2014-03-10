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

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Bindable_node.hpp"
#include "SGAL/Bindable_stack.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"

SGAL_BEGIN_NAMESPACE

std::string Bindable_node::s_tag = "BindableNode";
Container_proto * Bindable_node::s_prototype(nullptr);

//! \brief constructor.
Bindable_node::Bindable_node(Boolean proto) :
  Container(proto),
  m_set_bind(false),
  m_is_bound(false)
{}

//! \brief destructor.
Bindable_node::~Bindable_node() {}

//! \brief initializes the node prototype.
void Bindable_node::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype:
  // set_bind
  Execution_function exec_func =
    static_cast<Execution_function>(&Bindable_node::set_bind);
  Boolean_handle_function bind_func =
    static_cast<Boolean_handle_function>(&Bindable_node::bind_handle);
  s_prototype->add_field_info(new SF_bool(SET_BIND, "set_bind",
                                          RULE_EXPOSED_FIELD,
                                          bind_func,
                                          exec_func));

  // isBound
  Boolean_handle_function is_bound_func =
    static_cast<Boolean_handle_function>(&Bindable_node::is_bound_handle);
  s_prototype->add_field_info(new SF_bool(IS_BOUND, "isBound",
                                          RULE_EXPOSED_FIELD,
                                          is_bound_func));
}

//! \brief
void Bindable_node::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief
Container_proto * Bindable_node::get_prototype()
{
  if (!s_prototype) Bindable_node::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this node.
void Bindable_node::set_attributes(Element * elem)
{
  Container::set_attributes(elem);
}

//! \brief obtains the Navigation_info at the top os the stack.
Bindable_node * Bindable_node::top_stack() { return get_stack()->top(); }

//! \brief erases the Navigation_info.
void Bindable_node::erase_stack(Bindable_node * bindable)
{
  get_stack()->erase(bindable);
}

//! \brief pops the Navigation_info at the top of the stack.
void Bindable_node::pop_stack() { get_stack()->pop(); }

//! \brief pushes a Navigation_info onto the stack.
void Bindable_node::push_stack(Bindable_node * bindable)
{
  get_stack()->push(bindable);
}

//! \brief inserts a Navigation_info at the back of the stack.
void Bindable_node::insert_stack(Bindable_node * bindable)
{
  //! \todo insert(this)????
  get_stack()->insert(bindable);
}

//! \brief binds this node (used during initialization).
void Bindable_node::bind()
{
  if (m_is_bound) return;
  m_set_bind = true;
  set_is_bound(true);
}

/*! \brief sets the flag that indicates whether this node is bound
 * \param is_bound indicates whether the node is bound
 */
void Bindable_node::set_is_bound(Boolean is_bound)
{
  (is_bound) ? enable() : disable();

  m_is_bound = is_bound;
  Field * is_bound_field = get_field(IS_BOUND);
  if (is_bound_field) is_bound_field->cascade();
}

/*! \brief binds or unbinds this node.
 * \param field_info
 * If set_bind is true, push this node onto the top of the stack
 */
void Bindable_node::set_bind(Field_info * /* field_info */)
{
  if (m_set_bind) {
    Bindable_node * top = top_stack();
    if (this == top) return;
    top->set_is_bound(false);

    /*! \todo the dragging speed is the product of the radius of the bounding
     * sphere of the connected transform and the speed factor extracted set
     * in the Configuration container.
     * Need to the bounding sphere (or at least the sphere radius and the
     * speed factor as input and computes the dragging speed as output
     */
    // set_dragging_speed(top->get_dragging_speed());

    erase_stack(this);
    push_stack(this);
    set_is_bound(true);
    return;
  }

  // set_bind is false:
  Bindable_node * top = top_stack();
  if (this == top) {
    set_is_bound(false);
    pop_stack();

    top = top_stack();
    top->set_is_bound(true);
    return;
  }
  erase_stack(this);
}

SGAL_END_NAMESPACE
