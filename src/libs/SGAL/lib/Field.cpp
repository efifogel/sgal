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

#include "SGAL/basic.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Value_holder.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Field::Field(Container* container, const Field_info* field_info) :
  m_container(container),
  m_field_info(field_info)
{
  if (field_info) {
    m_value_holder = field_info->create_value_holder(container);
    m_blocked = field_info->is_initially_blocked();
  }
  else {
    m_value_holder = nullptr;
    m_blocked = false;
  }
};

//! \brief destructs.
Field::~Field() { delete m_value_holder; };

//! \brief connects this field to a given one.
void Field::connect(Field* field)
{
  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)
  //! \todo Auto_lock auto_lock(&m_fields_cs);
  m_connected_fields.insert(m_connected_fields.begin(), field);
}

//! \brief disconnects this field from the given one.
void Field::disconnect(Field* field)
{
  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)
  //! \todo Auto_lock auto_lock(&m_fields_cs);
  m_connected_fields.remove(field);
}

//! \brief delegates the value.
void Field::delegate(Field* target)
{ (target->get_value_holder())->copy(*m_value_holder); }

//! \brief copies the value.
void Field::copy(const Field* source)
{ get_value_holder()->copy(*(source->get_value_holder())); }

/*! \brief propagates the data from this field to the fields connected to this
 * field, generating a cascading of events effect.
 */
void Field::cascade()
{
  // Activate execution if the field info has one:
  if (m_field_info) {
    Execution_function execution_func = m_field_info->execution_function();
    if (execution_func) (m_container->*execution_func)(m_field_info);
  }

  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)

  //! \todo Auto_lock auto_lock(&m_fields_cs);

  // Loop over the connected fields:
  for (auto* connected_field : m_connected_fields) {
    // Continue the cascade into a connected field only if it is not blocked:
    if (connected_field->is_blocked()) continue;

    // Detach the old field value from the container
    if (m_field_info) m_field_info->detach(m_container);

    // Set the connected field's value to this field's value:
    // delegate(connected_field);
    connected_field->copy(this);

    // Apply cascade on the connected field:
    connected_field->cascade();
  }
}

SGAL_END_NAMESPACE
