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
#include "SGAL/Execution_function.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
class Element;
class Field_info;
class SAI_field_services;
class Value_holder_base;
class Field;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Field_info {
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
    m_exec_func(exec_func),
    m_initially_blocked(initially_blocked)
  { }

  /*! Destructor */
  virtual ~Field_info() {};

  /*! Obtain the field id.
   */
  Uint get_id() const;

  /*! Obtain the field name.
   */
  const std::string& get_name() const;

  /*! Obtain the execution function */
  Execution_function execution_function() const;

  /*! Equality opetator.
   */
  Boolean operator==(const Field_info& other) const;

  /*! Determine whether
   */
  Boolean is_initially_blocked() const;

  /* Obtain the field info type.
   */
  virtual Uint get_type_id() const = 0;

  /*! Create an object that holds a pointer to the value of an actual field
   * with this info.
   * \param container the container of the field.
   */
  virtual Value_holder_base* create_value_holder(Container* container) = 0;

  /*! Obtain the attribute of a field in a given container, where this field
   * info contains the information of the field.
   */
  virtual void get_attribute(Container* container, Element* element) const = 0;

  /*! Obtain the attribute of a field in a given container, where this field
   * info contains the information of the field.
   */
  void add_attribute(Container* container, Element* element,
                     const Boolean& value) const;
  void add_attribute(Container* container, Element* element,
                     const Float& value) const;
  void add_attribute(Container* container, Element* element,
                     const Uint& value) const;
  void add_attribute(Container* container, Element* element,
                     const Int& value) const;
  void add_attribute(Container* container, Element* element,
                     const Scene_time& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector2f& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector3f& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector4f& value) const;
  void add_attribute(Container* container, Element* element,
                     const Rotation& value) const;
  void add_attribute(Container* container, Element* element,
                     const Sphere_bound& value) const;
  void add_attribute(Container* container, Element* element,
                     const std::string& value) const;
  void add_attribute(Container* container, Element* element,
                     const Boolean_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Float_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Uint_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Int_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Scene_time_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector2f_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector3f_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Vector4f_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Rotation_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Sphere_bound_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const String_array& value) const;
  void add_attribute(Container* container, Element* element,
                     const Shared_container_array& value) const;

  /*! \todo
  // Creates an SAI_field of the same type id and name
  // as this field info.
  // The function also create a new SAI_field_info of the same type
  // and initializes the new SAI_field with it.
  virtual SAI_field_services * create_sai_field() = 0;
  */
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the field id.
inline Uint Field_info::get_id() const { return m_id; }

//! \brief obtains the field name.
inline const std::string& Field_info::get_name() const { return m_name; }

//! \brief obtains the execution function.
inline Execution_function Field_info::execution_function() const
{ return m_exec_func; }

//*! \brief equality opetator.
inline Boolean Field_info::operator==(const Field_info& other) const
{ return (m_id == other.m_id); }

//! \brief determines whether
inline Boolean Field_info::is_initially_blocked() const
{ return m_initially_blocked; }

/*!
 */
inline std::ostream& operator<<(std::ostream& os, const Field_info& fi)
{
  os << fi.get_name().c_str() << ", " << fi.get_id();
  return os;
}

SGAL_END_NAMESPACE

#endif
