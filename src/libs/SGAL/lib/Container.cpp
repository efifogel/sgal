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

/*!
 * \brief - implementation
 */

#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Container::Container(Boolean /* proto */) :
  m_base(0),
  m_execution_coordinator(0)
{}

//! \brief destructor.
Container::~Container()
{
  for (auto fi = m_fields.begin(); fi != m_fields.end(); ++fi)
    delete (*fi).second;
  m_fields.clear();
  m_name.clear();
  m_observers.clear();
};

//! \brief adds a field with the given id.
Field* Container::add_field(Uint id)
{
  const auto* field_info = get_field_info(id);
  return add_field(field_info);
}

//! \brief adds a field with the given name.
Field* Container::add_field(const std::string& name)
{
  const auto* field_info = get_field_info(name);
  return add_field(field_info);
}

//! \brief obtains the field with the given id.
Field* Container::get_field(Uint id)
{
  const auto* field_info = get_field_info(id);
  return get_field(field_info);
}

//! \brief obtains the field with the given name.
Field* Container::get_field(const std::string& name)
{
  const auto* field_info = get_field_info(name);
  return get_field(field_info);
}

//! \brief obtains a field by a field info.
Field* Container::get_field(const Field_info* field_info)
{
  // Lock the fields critical section (will unlock autonmaticaly at the end of
  // function
  //! \todo Auto_lock auto_lock(&m_fields_cs);

  auto fi = m_fields.find(field_info);
  if (!(fi == m_fields.end())) return (*fi).second;
  return NULL;
}

//! \brief adds a field given its field-info.
Field* Container::add_field(const Field_info* field_info)
{
  if (!field_info) return nullptr;

  // Check if the field already exists -> dont add it again
  auto* field = get_field(field_info);
  if (field) return field;

  // If the field does not exist - create a new one and add it
  // mem allocated here deleted in container distr'.
  field = new Field(this, field_info);

  // Lock the fields critical section (will unlock autonmaticaly at the end of
  // function
  //! \todo Auto_lock auto_lock(&m_fields_cs);
  m_fields.insert(Field_map::value_type(field_info, field));
  return field;
}

//! \brief obtains a field info by id.
Field_info* Container::get_field_info(Uint id)
{ return (get_prototype()) ? get_prototype()->get_field_info(id) : NULL; }

//! \brief obtains a field info by name.
Field_info* Container::get_field_info(const std::string& name)
{
  // Use the node's prototype to get the field info
  return (get_prototype()) ? get_prototype()->get_field_info(name) : NULL;
}

#if 0
//! \brief obtains  the attributes.
Attribute_list Container::get_attributes()
{
  Attribute_list attribs;
  // attribs = Background::get_attributes();
  Attribue attrib;

  if (!m_name.empty()) {
    attrib.first = "DEF";
    attrib.second = get_name();
    attribs.push_back(attrib);
  }
  return attribs;
}

//! \brief adds a child object to the scene.
void Container::add_to_scene(Scene_graph* sg, XML_entity* parent)
{ m_execution_coordinator = sg->get_execution_coordinator(); }
#endif

//! \brief obtains a source field with a given name.
Field* Container::get_source_field(const std::string& src_field_name)
{
  auto* src_field = add_field(src_field_name);
  if (src_field) return src_field;

  Uint start = src_field_name.size() - 8;
  if (src_field_name.compare(start, 8, "_changed") == 0) {
    std::string tmp = src_field_name;
    tmp.resize(start);
    src_field = add_field(tmp);
    if (src_field) return src_field;
  }

  //! \todo handle errors properly!
  std::cerr << "Failed to find field " << src_field_name
            << " in source node" << std::endl;
  return nullptr;
}

//! \brief obtains a destination field with a given name.
Field* Container::get_destination_field(const std::string& dst_field_name)
{
  auto* dst_field = add_field(dst_field_name);
  if (dst_field) return dst_field;
  if (dst_field_name.compare(0, 4, "set_") == 0) {
    std::string tmp = dst_field_name;
    tmp.erase(0, 4);
    dst_field = add_field(tmp);
    if (dst_field) return dst_field;
  }

  //! \todo handle errors properly!
  std::cerr << "Failed to find field " << dst_field_name
            << " in destination node" << std::endl;
  return nullptr;
}

//! \brief writes all fields of this container.
void Container::write_fields(Formatter* formatter)
{
  auto* proto = get_prototype();
  for (auto it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if (field_info->get_rule() == Field_info::RULE_EXPOSED_FIELD)
      field_info->write(this, formatter);
  }
}

//! \brief writes this container.
void Container::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::WRITING,
                  std::cout << "Container: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  formatter->container_begin(get_tag());
  write_fields(formatter);
  formatter->container_end();
}

//! \breif processes change of content.
void Container::process_content_changed()
{
  for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
    auto* observer = (*it).first;
    const auto* field_info = (*it).second;
    observer->field_changed(field_info);
  }
}

//! \brief Processes change of field.
void Container::field_changed(const Field_info* /* field_info */)
{ process_content_changed(); }

SGAL_END_NAMESPACE
