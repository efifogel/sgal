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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

std::string Cube_environment::s_tag = "CubeEnvironment";
Container_proto* Cube_environment::s_prototype = NULL;

// Default values:

/*! Constructor */
Cube_environment::Cube_environment(Boolean proto) :
  Texture(proto)
{}

/*! Destructor */
Cube_environment::~Cube_environment() {}

/*! \brief initializes the prototype. */
void Cube_environment::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype != NULL) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto(Texture::get_prototype());
}

/*! \brief deletes the prototype. */
void Cube_environment::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Cube_environment::get_prototype() 
{  
  if (s_prototype == NULL) Cube_environment::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the cubical environment map. */
void Cube_environment::set_attributes(Element* elem)
{
  Texture::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;

  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    // const std::string& name = elem->get_name(ai);
    // const std::string& value = elem->get_value(ai);
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
