// Copyright (c) 2014 Israel.
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

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief the names of the field rules.
const char* Field_info::s_rule_names[] = {
  "eventIn", "eventOut", "field", "exposedField"
};

//! \brief the names of the field types.
const char* Field_info::s_type_names[] = {
  "SFBool", "SFFloat", "SFUint32", "SFInt32",
  "SFTime", "SFVec2f", "SFVec3f", "SFColor",
  "SFVec4f", "SFRotation", "SFSphereBound", "SFImage",
  "SFString", "SFSharedContainer", "MFBool", "MFFloat",
  "MFUint32", "MFInt32", "MFTime", "MFVec2f",
  "MFVec3f", "MFColor", "MFVec4f", "MFRotation",
  "MFSphereBound", "MFImage", "MFString", "MFSharedContainer",
};

//! \brief exports the (single) Boolean field.
void Field_info::write(Formatter* formatter, Boolean value,
                       Boolean default_value, Boolean declaration) const
{
  const auto& name = get_name();
  if (declaration) {
    const auto& rule_name = get_rule_name();
    const auto& type_name = get_type_name();
    if ((RULE_FIELD == get_rule()) || (RULE_EXPOSED_FIELD == get_rule()))
      formatter->single_boolean(rule_name, type_name, name, default_value);
    else formatter->declaration(rule_name, type_name, name);
  }
  else formatter->single_boolean(name, value, default_value);
}

//! \brief exports the (single) Float field.
void Field_info::write(Formatter* formatter, Float value,
                       Float default_value, Boolean declaration) const
{
  if (declaration)
    formatter->single_float(get_rule_name(), get_type_name(), get_name(),
                            default_value);
  else formatter->single_float(get_name(), value, default_value);
}

//! \brief exports the (single) Uint field.
void Field_info::write(Formatter* formatter, Uint value,
                       Uint default_value, Boolean declaration) const
{
  if (declaration)
    formatter->single_uint(get_rule_name(), get_type_name(), get_name(),
                           default_value);
  else formatter->single_uint(get_name(), value, default_value);
}

//! \brief exports the (single) Int field.
void Field_info::write(Formatter* formatter, Int32 value,
                       Int32 default_value, Boolean declaration) const
{
  if (declaration)
    formatter->single_int32(get_rule_name(), get_type_name(), get_name(),
                            default_value);
  else formatter->single_int32(get_name(), value, default_value);
}

//! \brief exports the (single) Scene-time field.
void Field_info::write(Formatter* formatter, const Scene_time& value,
                       const Scene_time& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->single_scene_time(get_rule_name(), get_type_name(), get_name(),
                                 default_value);
  else formatter->single_scene_time(get_name(), value, default_value);
}

//! \brief exports the (single) Vector2f field.
void Field_info::write(Formatter* formatter, const Vector2f& value,
                       const Vector2f& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->single_vector2f(get_rule_name(), get_type_name(), get_name(),
                               default_value);
  else formatter->single_vector2f(get_name(), value, default_value);
}

//! \brief exports the (single) Vector3f field.
void Field_info::write(Formatter* formatter, const Vector3f& value,
                       const Vector3f& default_value, Boolean declaration)
  const
{
  if (declaration)
    formatter->single_vector3f(get_rule_name(), get_type_name(), get_name(),
                               default_value);
  else formatter->single_vector3f(get_name(), value, default_value);
}

//! \brief exports the (single) Vector4f field.
void Field_info::write(Formatter* formatter, const Vector4f& value,
                       const Vector4f& default_value, Boolean declaration)
  const
{
  if (declaration)
    formatter->single_vector4f(get_rule_name(), get_type_name(), get_name(),
                               default_value);
  else formatter->single_vector4f(get_name(), value, default_value);
}

//! \brief exports the (single) rotation field.
void Field_info::write(Formatter* formatter, const Rotation& value,
                       const Rotation& default_value, Boolean declaration)
  const
{
  if (declaration)
    formatter->single_rotation(get_rule_name(), get_type_name(), get_name(),
                               default_value);
  else formatter->single_rotation(get_name(), value, default_value);
}

//! \brief exports the (single) sphere-bound field.
void Field_info::write(Formatter* formatter, const Sphere_bound& value,
                       const Sphere_bound& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->single_sphere_bound(get_rule_name(), get_type_name(), get_name(),
                                   default_value);
  else formatter->single_sphere_bound(get_name(), value, default_value);
}

//! \brief exports the (single) string field.
void Field_info::write(Formatter* formatter, const std::string& value,
                       const std::string& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->single_string(get_rule_name(), get_type_name(), get_name(),
                             default_value);
  else formatter->single_string(get_name(), value, default_value);
}

//! \brief exports the (single) shared-container field.
void Field_info::write(Formatter* formatter, Shared_container value,
                       Shared_container default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->declaration(get_rule_name(), get_type_name(), get_name());
  else {
    if (!value) return;
    formatter->single_container_begin(get_name());
    formatter->write(value);
    formatter->single_container_end();
  }
}

//! \brief exports the multi-Boolean field.
void Field_info::write(Formatter* formatter, const Boolean_array& value,
                       const Boolean_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_boolean(get_rule_name(), get_type_name(), get_name(),
                             default_value);
  else formatter->multi_boolean(get_name(), value, default_value);
}

//! \brief exports the multi-Float field.
void Field_info::write(Formatter* formatter, const Float_array& value,
                       const Float_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_float(get_rule_name(), get_type_name(), get_name(),
                           default_value);
  else formatter->multi_float(get_name(), value, default_value);
}

//! \brief exports the multi-Uint field.
void Field_info::write(Formatter* formatter, const Uint_array& value,
                       const Uint_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_uint(get_rule_name(), get_type_name(), get_name(),
                          default_value);
  else formatter->multi_uint(get_name(), value, default_value);
}

//! \brief exports the multi-Int field.
void Field_info::write(Formatter* formatter, const Int32_array& value,
                       const Int32_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_int32(get_rule_name(), get_type_name(), get_name(),
                           default_value);
  else formatter->multi_int32(get_name(), value, default_value);
}

//! \brief exports the multi-Scene_time field.
void Field_info::write(Formatter* formatter, const Scene_time_array& value,
                       const Scene_time_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_scene_time(get_rule_name(), get_type_name(), get_name(),
                                default_value);
  else formatter->multi_scene_time(get_name(), value, default_value);
}

//! \brief exports the multi-Vector2f field.
void Field_info::write(Formatter* formatter, const Vector2f_array& value,
                       const Vector2f_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_vector2f(get_rule_name(), get_type_name(), get_name(),
                              default_value);
  else formatter->multi_vector2f(get_name(), value, default_value);
}

//! \brief exports the multi-Vector3f field.
void Field_info::write(Formatter* formatter, const Vector3f_array& value,
                       const Vector3f_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_vector3f(get_rule_name(), get_type_name(), get_name(),
                              default_value);
  else formatter->multi_vector3f(get_name(), value, default_value);
}

//! \brief exports the multi-Vector4f field.
void Field_info::write(Formatter* formatter, const Vector4f_array& value,
                       const Vector4f_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_vector4f(get_rule_name(), get_type_name(), get_name(),
                              default_value);
  else formatter->multi_vector4f(get_name(), value, default_value);
}

//! \brief exports the multi-Rotation field.
void Field_info::write(Formatter* formatter, const Rotation_array& value,
                       const Rotation_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_rotation(get_rule_name(), get_type_name(), get_name(),
                              default_value);
  else formatter->multi_rotation(get_name(), value, default_value);
}

//! \brief exports the multi-Sphere_bound field.
void Field_info::write(Formatter* formatter, const Sphere_bound_array& value,
                       const Sphere_bound_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_sphere_bound(get_rule_name(), get_type_name(), get_name(),
                                  default_value);
  else formatter->multi_sphere_bound(get_name(), value, default_value);
}

//! \brief exports the multi-string field.
void Field_info::write(Formatter* formatter, const String_array& value,
                       const String_array& default_value,
                       Boolean declaration) const
{
  if (declaration)
    formatter->multi_string(get_rule_name(), get_type_name(), get_name(),
                            default_value);
  else formatter->multi_string(get_name(), value, default_value);
}

//! \brief exports the multi-shared-container field.
void Field_info::write(Formatter* formatter,
                       const Shared_container_array& value,
                       const Shared_container_array& default_value,
                       Boolean declaration) const
{
  if (declaration) {
    formatter->declaration(get_rule_name(), get_type_name(), get_name());
    return;
  }

  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Field_info: " << ", name: " << get_name()
                  << ": " << value.size()
                  << std::endl;);
  formatter->multi_container_begin(get_name());
  for (auto it = value.begin(); it != value.end(); ++it) formatter->write(*it);
  formatter->multi_container_end();
}

SGAL_END_NAMESPACE
