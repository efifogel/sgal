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

#include "SGAL/Normal_array.hpp"
#include "SGAL/Geo_set.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

std::string Normal_array::s_tag = "Normal";

/*! The node prototype */
Container_proto * Normal_array::s_prototype = NULL;

/*! Register to the container factory */
REGISTER_TO_FACTORY(Normal_array, "Normal_array");

/*! Initialize the node prototype */
void Normal_array::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance:
  s_prototype = new Container_proto(Container::get_prototype());
}

/*! Delete the node prototype */
void Normal_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */  
Container_proto * Normal_array::get_prototype()
{
  if (s_prototype == NULL) Normal_array::init_prototype();
  return s_prototype;
}
  
/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Normal_array::set_attributes(Element * elem)
{ 
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "vector") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 3;
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0 ; i < size ; i++) {
        svalue >> m_array[i][0] >> m_array[i][1] >> m_array[i][2];
      }
      elem->mark_delete(ai);      
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
Attribute_list Normal_array::get_attributes()
{ 
  Attribute_list attrs; 
  attrs = Container::get_attributes();
  return attrs; 
}
#endif

SGAL_END_NAMESPACE
