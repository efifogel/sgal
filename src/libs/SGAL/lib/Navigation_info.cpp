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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <string.h>

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Navigation_info.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/io_navigation_type.hpp"

SGAL_BEGIN_NAMESPACE

std::string Navigation_info::s_tag = "NavigationInfo";
Container_proto* Navigation_info::s_prototype(nullptr);
const std::list<Navigation_type> Navigation_info::s_supported_types =
  {Navigation_type::EXAMINE};

REGISTER_TO_FACTORY(Navigation_info, "Navigation_info");

const char* s_navigation_types[] =
  {"NONE", "ANY", "EXAMINE", "FLY", "WALK", "TRANSFORM"};

// Default values
const Navigation_type Navigation_info::s_def_type(Navigation_type::WALK);
const String_array Navigation_info::s_def_types = {std::string("WALK")};

//! \brief constructor.
Navigation_info::Navigation_info(Boolean proto) :
  Navigation_sensor(Vector3f(), Rotation(), proto),
  m_types(s_def_types),
  m_type(s_def_type)
{}

//! Destructor.
Navigation_info::~Navigation_info() {}

//! \brief sets the type.
bool Navigation_info::is_supported(Navigation_type type)
{
  // Compare with supported types:
  for (auto t : s_supported_types) if (t == type) return true;
  return false;
}

//! \brief sets the navigation paradigm list.
inline void Navigation_info::set_types(const String_array& types)
{
  m_types = types;
  types_changed(get_field_info(TYPES));
}

//! \brief parses the type string-attribute.
void Navigation_info::add_type(const std::string& type)
{
  m_types.push_back(type);
  types_changed(get_field_info(TYPES));
}

//! \brief sets the attributes of this node.
void Navigation_info::set_attributes(Element* elem)
{
  Navigation_sensor::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "type") {
      add_type((value));
      elem->mark_delete(ai);
      continue;
    }
  }

  auto msai = elem->multi_str_attrs_begin();
  for (; msai != elem->multi_str_attrs_end(); ++msai) {
    const auto& name = elem->get_name(msai);
    auto& value = elem->get_value(msai);
    if (name == "type") {
      m_types.resize(value.size());
      auto tit = m_types.begin();
      for (auto sit : value) *tit++ = std::move(*sit);
      types_changed(get_field_info(TYPES));
      elem->mark_delete(msai);
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
  auto exec_func =
    static_cast<Execution_function>(&Navigation_info::types_changed);
  auto types_func =
    static_cast<String_array_handle_function>(&Navigation_info::types_handle);
  s_prototype->add_field_info(new MF_string(TYPES, "type",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            types_func,
                                            s_def_types, exec_func));
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

//! \brief processes change of types.
void Navigation_info::types_changed(const Field_info* field_info)
{
  // Find the first supported type:
  for (const auto& type_str : m_types) {
    auto type = boost::lexical_cast<Navigation_type>(type_str);
    if (type == Navigation_type::ANY) {
      set_type(Navigation_type::EXAMINE);
      break;
    }
    if (is_supported(type)) {
      set_type(type);
      break;
    }
  }
  field_changed(field_info);
}

SGAL_END_NAMESPACE
