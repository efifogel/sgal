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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_INFO_HPP
#define SGAL_FIELD_INFO_HPP

/*! \file
 * Holds information about a field in the scene graph:
 *      - The way it's data is held
 *      - Optional pointer to a function that should be executed
 *        when the field is changed
 *          
 * Hold the fields id, name of the a field 
 * within its node class, and a pointer to a possible function.
 * The function is used by fields when cascade is activated.
 *
 * Field_info - The basic abstract field info - used by containers ...
 * 
 * Field_infoTemplate - a specific field info template for handling the
 * specific type of the field's value
 * 
 * Specific field infos (like SFFloat, SFInt...) are
 * declared by type instantiation of Field_infoTemplate.
 */

#include <string>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Member_offset_type.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
class Field_info;
class SAI_field_services;
class Value_holder;

class Field_info {  
protected:
  /*! The field id (e.g, Group::ISVISIBLE). */
  Uint m_id;

  /*! The field name. */
  std::string m_name;

  /*! The function to execute when the field changes. */
  Execution_function m_exec_func;

  /*! \todo what is it? */
  Boolean m_initially_blocked;

public:
  /*! Constructor. */
  Field_info(Uint id, const std::string& name,
             Execution_function exec_func = NULL,
             Boolean initially_blocked = false) :
    m_id(id),
    m_name(name),
    m_initially_blocked(initially_blocked)
  {
    m_exec_func = exec_func;
  }

  /*! Destructor */
  virtual ~Field_info() {};

  /*! Obtaint the field id */
  Uint get_id() const { return m_id; }

  /*! Obtaint the field name */
  const std::string& get_name() const { return m_name; }

  /*! Obtain the execution function */
  Execution_function execution_function() const { return m_exec_func; }

  // opetator
  Boolean operator==(const Field_info & other)
  { return (m_id == other.m_id); }

  /*! Obtain */
  Boolean is_initially_blocked() { return m_initially_blocked; }

  // Get the field info type.
  virtual Uint get_type_id() const = 0;

  // Returns an Value_holder object of the field info for the given container.
  virtual Value_holder* create_value_holder(Container* container) = 0;

  /*! \todo
  // Creates an SAI_field of the same type id and name 
  // as this field info.
  // The function also create a new SAI_field_info of the same type
  // and initializes the new SAI_field with it.
  virtual SAI_field_services * create_sai_field() = 0;
  */
};

/*!
 */
inline std::ostream& operator<<(std::ostream& os, const Field_info& fi)
{
  os << fi.get_name().c_str() << ", " << fi.get_id();
  return os;
}

SGAL_END_NAMESPACE

#endif
