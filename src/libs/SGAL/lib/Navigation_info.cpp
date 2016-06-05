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

#include <string.h>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Navigation_info.hpp"
#include "SGAL/Navigation_info_types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Attribute_error.hpp"

SGAL_BEGIN_NAMESPACE

std::string Navigation_info::s_tag = "NavigationInfo";
Container_proto* Navigation_info::s_prototype(nullptr);

REGISTER_TO_FACTORY(Navigation_info, "Navigation_info");

// Default values
const String_array Navigation_info::s_def_types =
  boost::assign::list_of(static_cast<std::string>("WALK"));

const char* Navigation_info::s_type_strings[] =
  {"NONE", "EXAMINE", "FLY", "WALK", "TRANSFORM"};

//! \brief constructor.
Navigation_info::Navigation_info(Boolean proto) :
  Navigation_sensor(Vector3f(), Rotation(), proto),
  m_any(false),
  m_types(s_def_types),
  m_type(SGAL::NONE)
{}

//! Destructor.
Navigation_info::~Navigation_info() {}

//! \brief parses the type string-attribute.
void Navigation_info::set_type(const std::string& type)
{
  auto trimmed_type = type;
  boost::algorithm::trim(trimmed_type);

  m_types.clear();

  // Compare with supported types:
  for (size_t j = 0; j < SGAL::NUM_TYPES; ++j) {
    const auto* nav_type = s_type_strings[j];
    if (trimmed_type.compare(0, strlen(nav_type), nav_type) == 0) {
      m_types.push_back(nav_type);
      m_type = (Navigation_info_type) j;
      return;
    }
  }

  // Compare with "ANY":
  const auto* nav_type = "ANY";
  if (trimmed_type.compare(0, strlen(nav_type), nav_type) == 0) {
    m_any = true;
    return;
  }

  throw Attribute_error(std::string("Unrecognized navigation type \"").append(type).append("\"!"));
}

//! \brief sets the attributes of this node.
void Navigation_info::set_attributes(Element* elem)
{
  Navigation_sensor::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "type") {
      set_type(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Navigation_info::add_to_scene(Scene_graph* sg)
{
  Navigation_sensor::add_to_scene(sg);

  // Push this bindable Navigation_info node onto the respective stack:
  insert_stack(this);

  // Route this node properly:
  sg->route_navigation_info(this, m_type);
}

//! \brief initializes the node prototype.
void Navigation_info::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Navigation_sensor::get_prototype());

  // types string
  auto types_func =
    static_cast<String_array_handle_function>(&Navigation_info::types_handle);
  s_prototype->add_field_info(new MF_string(TYPES, "type",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            types_func,
                                            s_def_types));
}

//! \brief deletes the prototype.
void Navigation_info::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Navigation_info::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
