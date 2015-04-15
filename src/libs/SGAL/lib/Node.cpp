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

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Node::s_prototype(nullptr);

//! Constructor.
Node::Node(Boolean proto) :
  Container(proto),
  m_dirty_sphere_bound(true),
  m_locked_sphere_bound(false)
{}

//! \brief initializes the node prototype.
void Node::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  // Execution_function exec_func;
  // exec_func = static_cast<Execution_function>(&Transform::parts_changed);
  Sphere_bound_handle_function sphere_bound_func =
    static_cast<Sphere_bound_handle_function>(&Node::sphere_bound_handle);
  s_prototype->add_field_info(new SF_sphere_bound(SPHERE_BOUND, "sphereBound",
                                                  Field_info::RULE_OUT,
                                                  sphere_bound_func));
}

//! \brief deletes the node prototype.
void Node::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Node::get_prototype()
{
  if (!s_prototype) Node::init_prototype();
  return s_prototype;
}

//! \brief sets the flag that indicates that the sphere bound should be cleaned.
void Node::sphere_bound_changed(const Field_info* /* field_info */)
{ m_dirty_sphere_bound = true; }

//! \brief obtains the sphere bound.
const Sphere_bound& Node::get_sphere_bound()
{
  if (m_dirty_sphere_bound) clean_sphere_bound();
  return m_sphere_bound;
}

//! \brief sets the attributes of the node.
void Node::set_attributes(Element* elem) { Container::set_attributes(elem); }

#if 0
//! \brief gets the attributes of the node.
Attribute_list Node::get_attributes()
{
  Attribute_list attribs;
  attribs = Container::get_attributes();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
