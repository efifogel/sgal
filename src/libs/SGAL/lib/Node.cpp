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
  m_dirty_bounding_sphere(true),
  m_locked_bounding_sphere(false)
{}

//! \brief initializes the node prototype.
void Node::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  // Execution_function exec_func;
  // exec_func = static_cast<Execution_function>(&Transform::parts_changed);
  Bounding_sphere_handle_function bounding_sphere_func =
    static_cast<Bounding_sphere_handle_function>(&Node::bounding_sphere_handle);
  s_prototype->add_field_info(new SF_bounding_sphere(BOUNDING_SPHERE,
                                                     "boundingSphere",
                                                     Field_info::RULE_OUT,
                                                     bounding_sphere_func));
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
void Node::bounding_sphere_changed(const Field_info* /* field_info */)
{ m_dirty_bounding_sphere = true; }

//! \brief obtains the sphere bound.
const Bounding_sphere& Node::get_bounding_sphere()
{
  if (m_dirty_bounding_sphere) clean_bounding_sphere();
  return m_bounding_sphere;
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
