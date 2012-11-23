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
// $Source$
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Value_holder.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Field::Field(Container * container, Field_info * field_info) :
  m_container(container),
  m_field_info(field_info)
{
  if (field_info != NULL) {
    m_value_holder = field_info->create_value_holder(container);
    m_blocked = field_info->is_initially_blocked();
  } else {
    m_value_holder = NULL;
    m_blocked = false;
  }
};

/*! Destructor */
Field::~Field()
{
  delete m_value_holder;
};

/*! Connect this field to a given one
 *
 * Inserts the given field to the connected fields list
 * @param  field (in) the field to be added  
 */
void Field::connect(Field * field)
{
  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)
  //! \todo Auto_lock auto_lock(&m_fields_cs);
  m_connected_fields.insert(m_connected_fields.begin(), field);
}

/*! Disconnects this field from the given one
 * Removes the given field from the connected fields list
 * @param  field (in) the field to be removed  
 */
void Field::disconnect(Field * field)
{
  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)
  //! \todo Auto_lock auto_lock(&m_fields_cs);
  m_connected_fields.remove(field);
}

/*! Cascade the execution flow from this field to its connected ones
 * - Activate execution - if this field is an execution function.
 * - For all connected fields
 * - Set the value to the value of the current field.
 * - Activate Cascade on each one.
 */
void Field::cascade()
{
  // Activate execution if the field info has one
  if (m_field_info != NULL) {
    Execution_function execution_func = m_field_info->execution_function();
    if (execution_func) (m_container->*execution_func)(m_field_info);
  }

  // lock m_connected_fields critical section
  // (will unlock automaticaly at end of function)

  //! \todo Auto_lock auto_lock(&m_fields_cs);
  if (m_connected_fields.empty()) return;

  Field_list::iterator fi;
  // Loop over the connected fields
  for (fi = m_connected_fields.begin(); fi != m_connected_fields.end(); fi++) {
    Field * connected_field = (*fi);
    
    // Continue the cascade into a connected field only if it is not blocked
    if (!connected_field->is_blocked()) {
      // Set the connected field's value to this field's value
      (connected_field->get_value_holder())->set(m_value_holder);
      
      // Activate cascade on the connected field
      connected_field->cascade();
    }
  }
}

SGAL_END_NAMESPACE
