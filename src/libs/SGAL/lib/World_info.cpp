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

#include "SGAL/World_info.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

std::string World_info::s_tag("WorldInfo");
Container_proto* World_info::s_prototype(NULL);

/*! \brief constructor. */
World_info::World_info(Boolean proto) : Container(proto) {}

/*! \brief initializes the node prototype */
void World_info::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

void World_info::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

Container_proto* World_info::get_prototype()
{
  if (!s_prototype) World_info::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this container. */
void World_info::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
}

SGAL_END_NAMESPACE
