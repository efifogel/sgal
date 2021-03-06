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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

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
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_type.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Field_value_applier.hpp"
#include "SGAL/Field_value_transformer.hpp"

SGAL_BEGIN_NAMESPACE

class Container;
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

  /*! The field rule. */
  Field_rule m_rule;

  /*! The function to execute when the field changes. */
  Execution_function m_exec_func;

  /*! \todo what is it? */
  Boolean m_initially_blocked;

public:
  /*! Construct.
   */
  Field_info(Uint id, const std::string& name, Field_rule rule,
             Execution_function exec_func = nullptr,
             Boolean initially_blocked = false) :
    m_id(id),
    m_name(name),
    m_rule(rule),
    m_exec_func(exec_func),
    m_initially_blocked(initially_blocked)
  {}

  /*! Destruct. */
  virtual ~Field_info() {};

  /*! Obtain the field id.
   */
  Uint get_id() const;

  /*! Obtain the field name.
   */
  const std::string& get_name() const;

  /*! Obtain the field rule.
   * \return the field rule.
   */
  Field_rule get_rule() const;

  /*! Obtain the execution function.
   */
  Execution_function execution_function() const;

  /*! Execute the execution_function.
   */
  void execute(Container* container) const;

  /*! Equality opetator.
   */
  Boolean operator==(const Field_info& other) const;

  /*! Determine whether
   */
  Boolean is_initially_blocked() const;

  /* Obtain the field info type.
   */
  virtual Field_type get_type_id() const = 0;

  /*! Obtain the name of the field info type.
   */
  const char* get_type_name() const;

  /*! Obtain the name of a rule.
   */
  const char* get_rule_name() const;

  /*! Clone the field, the (field) info of which is this object, of a source
   * container and store it in a target container.
   * \param[in] source the source container.
   * \param[in] target the target container.
   */
  virtual void clone(const Container* source, Container* target) const = 0;

  /*! Apply an operation on the field, the (field) info of which is this object,
   * of a given container.
   * \param[in] cont the container that holds the field.
   * \param[in] op the unary operator.
   */
  virtual void apply(const Container* cont, Field_value_applier& op) const = 0;

  /*! Transform the field, the (field) info of which is this object, of a source
   * container and store it in a target container.
   * \param[in] source the source container.
   * \param[in] target the target container.
   * \param[in] op the unary transform operator.
   */
  virtual void transform(const Container* source, Container* target,
                         Field_value_transformer& op) const = 0;

  /*! Move the value of the field, of a given container, associated with this
   * field-information record to the initial value of the field-information
   * record.
   * \param container the container that holds the field.
   */
  virtual void move_field_to_initial_value(Container* container) = 0;

  /*! Create an object that holds a pointer to the value of an actual field
   * with this info.
   * \param container the container of the field.
   */
  virtual Value_holder_base* create_value_holder(Container* container) const = 0;

  /*! Detach the value of a field, the (field) info of which is this object,
   * from the container that contains the field before the field is overriden.
   * \param[in] container The container that contains the field.
   */
  virtual void detach(Container* container) const = 0;

  /*! Attach the value of a field, the (field) info of which is this object,
   * to the container that contains the field after the field is overriden.
   * \param[in] container The container that contains the field.
   */
  virtual void attach(Container* container) const = 0;

  /*! Determine whether the field is a scalar
   * \return \true if the field is a scalar and false otherwise.
   */
  virtual Boolean is_scalar() const = 0;

  /*! Export a field using a given formatter.
   * \param[in] container The container that contains the field.
   * \param[in] formatter The given formatter, e.g., VRML.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  virtual void write(Container* container, Formatter* formatter,
                     Boolean declaration = false) const = 0;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, Boolean value, Boolean default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, Float value, Float default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, Uint value, Uint default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, Int32 value, Int32 default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Scene_time& value,
             const Scene_time& default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector2f& value,
             const Vector2f& default_value, Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector3f& value,
             const Vector3f& default_value, Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector4f& value,
             const Vector4f& default_value, Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Rotation& value,
             const Rotation& default_value, Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Bounding_sphere& value,
             const Bounding_sphere& default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const std::string& value,
             const std::string& default_value,
             Boolean declaration = false) const;

  /*! Export the (single) Boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, Shared_container value,
             Shared_container default_value,
             Boolean declaration = false) const;

  /*! Export the multi-boolean field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Boolean_array& value,
             const Boolean_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-float field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Float_array& value,
             const Float_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi Uint field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Uint_array& value,
             const Uint_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-int field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Int32_array& value,
             const Int32_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-scene-time field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Scene_time_array& value,
             const Scene_time_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-vector2f field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector2f_array& value,
             const Vector2f_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-vector3f field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector3f_array& value,
             const Vector3f_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-vector4f field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Vector4f_array& value,
             const Vector4f_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-rotation field.
   * \param[in] formatter The formatter.
   * \param[in] value The attribute value.
   * \param[in] default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Rotation_array& value,
             const Rotation_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multisphere-bound field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Bounding_sphere_array& value,
             const Bounding_sphere_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-string field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const String_array& value,
             const String_array& default_value,
             Boolean declaration = false) const;

  /*! Export the multi-shared container field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   * \patam[in] declaration Indicates whether the exported statement is a
   *            declaration or a definition.
   */
  void write(Formatter* formatter, const Shared_container_array& value,
             const Shared_container_array& default_value,
             Boolean declaration = false) const;
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

//! \brief equality opetator.
inline Boolean Field_info::operator==(const Field_info& other) const
{ return (m_id == other.m_id); }

//! \brief obtains the field rule.
inline Field_rule Field_info::get_rule() const { return m_rule; }

//! \brief obtains the name of the field rule.
inline const char* Field_info::get_rule_name() const
{ return get_field_rule_name(get_rule()); }

//! \brief obtains the name of the field info type.
inline const char* Field_info::get_type_name() const
{ return get_field_type_name(get_type_id()); }

//! \brief determines whether
inline Boolean Field_info::is_initially_blocked() const
{ return m_initially_blocked; }

//! \brief export a field info.
inline std::ostream& operator<<(std::ostream& os, const Field_info& fi)
{
  os << fi.get_name().c_str() << ", " << fi.get_id();
  return os;
}

SGAL_END_NAMESPACE

#endif
