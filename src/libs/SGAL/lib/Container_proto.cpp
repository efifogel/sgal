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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

/*!
 * Container_proto - implementation
 */

#include "SGAL/basic.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_info.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Container_proto::Container_proto(Container_proto* ancestor) :
  m_ancestor(ancestor),
  m_first_id(0)
{
  if (ancestor)
    m_first_id = ancestor->m_first_id + ancestor->m_field_info_ids.size();
}

//! \brief destructs.
Container_proto::~Container_proto()
{
  for (auto fi = m_field_info_ids.begin(); fi != m_field_info_ids.end(); ++fi) {
    auto* field_info = (*fi).second;
    delete field_info;
  }
  m_field_info_ids.clear();
  m_field_info_names.clear();
}

/*! Add a field-information record to the container-type prototype
 * It is assumed that, if (the prototype of) container A inherits the field
 * information records from (the prototype of) container B, the identification
 * numbers of the field-information records of A succeed the the
 * identification numbers of the field-information records of B.
 * Insert the id of the new field-information record into the container of ids,
 * and its name into the name container.
 * \param field_info (in) a pointer to a field-information record to be added
 */
void Container_proto::add_field_info(Field_info* field_info)
{
  SGAL_assertion(m_first_id <= field_info->get_id());

  // Insert the new field info into the map
  SGAL_assertion(m_field_info_ids.find(field_info->get_id()) ==
                 m_field_info_ids.end());
  m_field_info_ids.insert
    (Field_info_id_map::value_type(field_info->get_id(), field_info));
  SGAL_assertion(m_field_info_names.find(field_info->get_name()) ==
                 m_field_info_names.end());
  m_field_info_names.insert
    (Field_info_name_map::value_type(field_info->get_name(), field_info));
}

/*! \brief obtains a field info given by its id from the current prototype or
 * from one of its ancestors. If the given id is in the current prototype
 * field-infos range, find it there. Otherwise, search for the record in the
 * ancestor prototype recursively.
 */
Field_info* Container_proto::get_field_info(Uint id) const
{
  auto fi = m_field_info_ids.find(id);
  if (fi != m_field_info_ids.end()) return (*fi).second;
  if (m_ancestor) return m_ancestor->get_field_info(id);
  return nullptr;
}

/*! \brief obtains a field info by its name from the current prototype or from
 * one of its ancestors.
 */
Field_info* Container_proto::get_field_info(const std::string& name) const
{
  // Try to find the name on the current map
  // Since in VRML/X3D a field name may appear with "set_XXXX" or "XXXX_changed"
  // (when the field is referenced as an event) try to match any of the three
  // posible names
  Field_info_name_const_iter fi;

  for (fi = m_field_info_names.begin(); fi != m_field_info_names.end(); ++fi) {
    if ((*fi).first == name) break;
    //! \todo add (optional?) "set_" to all input fields and (optional?)
    // "_changed" to all output fields; first, need to fix the rule types of
    // all fields.
  }

  // Return the id - if it is found
  if (fi != m_field_info_names.end()) return (*fi).second;

  // if not found till now activate on ancestor if exists
  if (m_ancestor) return m_ancestor->get_field_info(name);

  // if ancestor is null there is no field info with the requested info
  // in this proto nor in its ancestors -> return NULL
  return nullptr;
}

//! \brief obtains the begin iterator of the extended range of field info ids.
Container_proto::Id_const_iterator
Container_proto::ids_begin(const Container_proto* prototype) const
{
  while ((field_info_ids_begin() == prototype->field_info_ids_end()) &&
         (prototype->m_ancestor != nullptr))
    prototype = prototype->m_ancestor;
  return Id_const_iterator(prototype, prototype->field_info_ids_begin());
}

/*! \brief obtains the past-the-end iterator of the extended range of field
 * info ids.
 */
Container_proto::Id_const_iterator
Container_proto::ids_end(const Container_proto* prototype) const
{
  while (prototype->m_ancestor != nullptr) prototype = prototype->m_ancestor;
  return Id_const_iterator(prototype, prototype->field_info_ids_end());
}

SGAL_END_NAMESPACE
