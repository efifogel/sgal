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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Node::Node(Boolean proto) :
  Container(proto),
  m_sphere_bound_dirty(SGAL_TRUE),
  m_sphere_bound_locked(SGAL_FALSE)
{}

/*! Set the flag to calculate bounding sphere and rendering modfied */
void Node::set_sphere_bound_modified(Field_info* /* field_info */)
{
  m_sphere_bound_dirty = true;
#if 0
  m_execution_coordinator->set_rendering_required();
  m_execution_coordinator->set_bounding_sphere_modified(SGAL_TRUE);
#endif
}

/*! Obtain the sphere bound */
const Sphere_bound& Node::get_sphere_bound()
{
  calculate_sphere_bound();
  return m_sphere_bound;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Node::set_attributes(Element * elem)
{
  Container::set_attributes(elem);
}

#if 0
/*! Get the attributes of the box
 */
Attribute_list Node::get_attributes() 
{ 
  Attribute_list attribs; 
  attribs = Container::get_attributes();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
