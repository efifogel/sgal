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

SGAL_BEGIN_NAMESPACE

class Container;
class Value_holder_base;
class Field;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Field_info {
public:
  enum Field_type {
    SF_BOOL = 0,
    SF_FLOAT,
    SF_UINT32,
    SF_INT32,
    SF_TIME,
    SF_VEC2F,
    SF_VEC3F,
    SF_COLOR,
    SF_VEC4F,
    SF_ROTATION,
    SF_SB,
    SF_IMAGE,
    SF_STR,
    SF_SHARED_CONTAINER,
    MF_BOOL,
    MF_FLOAT,
    MF_UINT32,
    MF_INT32,
    MF_TIME,
    MF_VEC2F,
    MF_VEC3F,
    MF_COLOR,
    MF_VEC4F,
    MF_ROTATION,
    MF_SB,
    MF_IMAGE,
    MF_STR,
    MF_SHARED_CONTAINER,
    NUM_FIELD_TYPES
  };

  enum Field_rule {
    RULE_IN = 0,
    RULE_OUT,
    RULE_FIELD,
    RULE_EXPOSED_FIELD,
    NUM_FIELD_RULES
  };

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

  /*! Obtain the name of the field rule.
   * \return the name of field rule.
   */
  const char* get_rule_name() const;

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

  /*! Obtain the name of the field info type.
   */
  const char* get_type_name() const;

  /*! Create an object that holds a pointer to the value of an actual field
   * with this info.
   * \param container the container of the field.
   */
  virtual Value_holder_base* create_value_holder(Container* container) const = 0;

  /*! Detach the value of a field, the (field) info of which is this object,
   * from the container that contains the field before the field is overriden.
   * \param container (in) The container that contains the field.
   */
  virtual void detach(Container* container) const = 0;

  /*! Determine whether the field is a scalar
   * \return \true if the field is a scalar and false otherwise.
   */
  virtual Boolean is_scalar() const = 0;

  /*! Write a field using a given formatter.
   * \param container (in) The container that contains the field.
   * \param formatter (in) The given formatter, e.g., VRML.
   */
  virtual void write(Container* container, Formatter* formatter,
                     Boolean declaration = false) const = 0;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, Boolean value, Boolean default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, Float value, Float default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, Uint value, Uint default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, Int32 value, Int32 default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Scene_time& value,
             const Scene_time& default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector2f& value,
             const Vector2f& default_value, Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector3f& value,
             const Vector3f& default_value, Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector4f& value,
             const Vector4f& default_value, Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Rotation& value,
             const Rotation& default_value, Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Sphere_bound& value,
             const Sphere_bound& default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const std::string& value,
             const std::string& default_value,
             Boolean declaration = false) const;

  /*! Write the (single) Boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, Shared_container value,
             Shared_container default_value,
             Boolean declaration = false) const;

  /*! Write the multi-boolean field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Boolean_array& value,
             const Boolean_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-float field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Float_array& value,
             const Float_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi Uint field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Uint_array& value,
             const Uint_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-int field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Int32_array& value,
             const Int32_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-scene-time field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Scene_time_array& value,
             const Scene_time_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-vector2f field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector2f_array& value,
             const Vector2f_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-vector3f field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector3f_array& value,
             const Vector3f_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-vector4f field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Vector4f_array& value,
             const Vector4f_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-rotation field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Rotation_array& value,
             const Rotation_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multisphere-bound field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Sphere_bound_array& value,
             const Sphere_bound_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-string field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const String_array& value,
             const String_array& default_value,
             Boolean declaration = false) const;

  /*! Write the multi-shared container field.
   * \param formatter The formatter.
   * \param value The attribute value.
   * \param default_value The attribute default value.
   */
  void write(Formatter* formatter, const Shared_container_array& value,
             const Shared_container_array& default_value,
             Boolean declaration = false) const;

  /*! Obtain the name of a rule.
   */
  static const char* get_rule_name(Field_rule id);

  /*! Obtain the name of a type.
   */
  static const char* get_type_name(Uint id);

private:
  /*! The names of the field rules. */
  const static char* s_rule_names[];

  /*! The names of the field types. */
  const static char* s_type_names[];
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
inline Field_info::Field_rule Field_info::get_rule() const { return m_rule; }

//! \brief obtains the name of the field rule.
inline const char* Field_info::get_rule_name() const
{ return get_rule_name(get_rule()); }

//! \brief obtains the name of the field info type.
inline const char* Field_info::get_type_name() const
{ return get_type_name(get_type_id()); }

//! \brief determines whether
inline Boolean Field_info::is_initially_blocked() const
{ return m_initially_blocked; }

//! \brief export a field info.
inline std::ostream& operator<<(std::ostream& os, const Field_info& fi)
{
  os << fi.get_name().c_str() << ", " << fi.get_id();
  return os;
}

//! \brief obtains the name of a rule.
inline const char* Field_info::get_rule_name(Field_rule id)
{ return s_rule_names[id]; }

//! \brief obtains the name of a type.
inline const char* Field_info::get_type_name(Uint id)
{ return s_type_names[id]; }

SGAL_END_NAMESPACE

#endif
