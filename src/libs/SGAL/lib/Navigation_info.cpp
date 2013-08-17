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
// $Id: $
// $Revision: 7773 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <string.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Navigation_info.hpp"
#include "SGAL/Navigation_info_types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

std::string Navigation_info::s_tag = "NavigationInfo";
Container_proto* Navigation_info::s_prototype(NULL);
const char* Navigation_info::s_type_strings[] =
  {"NONE", "EXAMINE", "FLY", "WALK", "TRANSFORM"};

REGISTER_TO_FACTORY(Navigation_info, "Navigation_info");

/* Constructor */
Navigation_info::Navigation_info(Boolean proto) :
  Navigation_sensor(Vector3f(), Rotation(), proto),
  m_any(false),
  m_type(SGAL::NONE)
{}

/*! Destructor */
Navigation_info::~Navigation_info() {}

/*! \brief parses the type string-attribute. */
int Navigation_info::parse_type(const std::string& type)
{
  Uint i;
  for (i = 0; i < type.size(); ++i) {
    // Skip white space:
    while (i < type.size() && type[i] == ' ') i++;
    if (i == type.size()) return 0;

    // Match open double-quote (")
    if (type[i++] != '\"') {
      std::cerr << "Invalid type field!" << std::endl;
      return -1;
    }

    // Compare with supported types:
    unsigned int j;
    Boolean found = false;
    
    for (j = 0; j < SGAL::NUM_TYPES; ++j) {
      const char* nav_type = s_type_strings[j];
      if (type.compare(i, strlen(nav_type), nav_type) == 0) {
        m_type = (SGAL::Navigation_info_type) j;
        found = true;
        break;
      }
    }

    // Compare with "ANY":
    if (!found && !m_any) {
      const char* nav_type = "ANY";
      if (type.compare(i, strlen(nav_type), nav_type) == 0) m_any = true;
    }

    // Advance to close double-quote (")
    while (i < type.size() && type[i] != '\"') i++;
    if (i == type.size()) {
      return -1;
      std::cerr << "Invalid type field!" << std::endl;
    }

    if (found || m_any) break;
  }

  return 0;
}

/*! \brief sets the attributes of this node. */
void Navigation_info::set_attributes(Element* elem) 
{
  Navigation_sensor::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "type") {
      parse_type(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */  
void Navigation_info::add_to_scene(Scene_graph* sg)
{
  Navigation_sensor::add_to_scene(sg);
  
  // Push this bindable Navigation_info node onto the respective stack:
  insert_stack(this);

  // Route this node properly:
  sg->route_navigation_info(this, m_type);
}

/*! \brief initializes the node prototype. */
void Navigation_info::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Navigation_sensor::get_prototype());
}

/*! \brief deletes the prototype. */
void Navigation_info::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief Obtains the prototype. */
Container_proto* Navigation_info::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
