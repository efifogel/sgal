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
// $Source: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <sstream>

#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Geo_set.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

std::string Tex_coord_array::s_tag = "TextureCoordinate";

/*! The node prototype */
Container_proto * Tex_coord_array::s_prototype = NULL;

typedef Element::Str_attr_iter          Str_attr_iter;

/*! Initialize the node prototype */
void Tex_coord_array::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance:
  s_prototype = new Container_proto(Container::get_prototype());
}

/*! Delete the node prototype */
void Tex_coord_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */  
Container_proto * Tex_coord_array::get_prototype()
{
  if (s_prototype == NULL) Tex_coord_array::init_prototype();
  return s_prototype;
}
  
/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Tex_coord_array, "Tex_coord_array");

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Tex_coord_array::set_attributes(Element * elem)
{
  Container::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin(); ai != elem->str_attrs_end();
       ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "point") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 2;
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0 ; i < size ; i++)
        svalue >> m_array[i][0] >> m_array[i][1];
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
Attribute_list Tex_coord_array::get_attributes()
{ 
  Attribute_list attribs;
  attribs = Container::get_attributes();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
