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

#include "SGAL/Collision.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto * Collision::s_prototype = 0;
std::string Collision::s_tag = "Collision";

REGISTER_TO_FACTORY(Collision, "Collision");

/*! Constructor */
Collision::Collision(Boolean proto) :
  Group(proto)
{
}

/*! Destructor */
Collision::~Collision() {}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Collision::set_attributes(Element * elem) 
{
  Group::set_attributes(elem);
}

/*! Initializes the node prototype */
void Collision::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto(Group::get_prototype());
}

/*! Deletes the node prototype */
void Collision::delete_prototype()
{
  delete s_prototype;
}

/*!
 */
Container_proto * Collision::get_prototype() 
{  
  if (!s_prototype) Collision::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
