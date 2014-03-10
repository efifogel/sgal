// Copyright (c) 2013 Israel.
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

#include <iostream>
#include <sstream>

#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

/*! The node prototype */
Container_proto* Tex_coord_array::s_prototype(NULL);

/*! Initialize the node prototype */
void Tex_coord_array::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

/*! Delete the node prototype */
void Tex_coord_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */
Container_proto* Tex_coord_array::get_prototype()
{
  if (s_prototype == NULL) Tex_coord_array::init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 */
void Tex_coord_array::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
