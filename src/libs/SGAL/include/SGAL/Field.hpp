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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_HPP
#define SGAL_FIELD_HPP

/*! \file
 * Holds information of a connected field in the scene graph.
 * A connected field is a field that was connected to, or connected from,
 * another node's field.
 */

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

//! \todo #include "Critical_section.h"

SGAL_BEGIN_NAMESPACE

class Value_holder;
class Container;
class Field_info;

/* Hold a pointer to the suitable field-info records and a list of connected
 * fields, which this field is connected to.
 */
class Field {
protected:
  typedef std::list<Field*> Field_list;

  /*! The containing container */
  Container * m_container;

  /*! The field info record */
  Field_info * m_field_info;

  /*! A list of connected fields */
  Field_list m_connected_fields;

  /*! Indicated whether cascading is blocked */
  Boolean m_blocked;

  /*! The value of the field */
  Value_holder * m_value_holder;

  //! \todo Critical_section m_fields_cs;

public:
  /*! Constructor */
  Field(Container * container, Field_info * field_info);

  /*! Destructor */
  virtual ~Field();

  /*! Obtain the field info */
  Field_info * get_field_info() const { return m_field_info; }

  /*! Connect this field to the given field */
  void connect(Field * field);

  /*! Disconnect this field from the given field */
  void disconnect(Field * field);

  /*! Cascade the execution flow of the connected fields */
  virtual void cascade();

  /*! Set the blocked flag to ignore cascading */
  void set_blocked(Boolean blocked) { m_blocked = blocked; }

  /*! Obtain the cascade-blocking flag */
  Boolean is_blocked() const { return m_blocked; }

  // Returns the EValue member
  virtual Value_holder * get_value_holder() { return m_value_holder; }
};

SGAL_END_NAMESPACE

#endif
