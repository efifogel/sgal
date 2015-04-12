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

SGAL_BEGIN_NAMESPACE

//! \brief writes the (single) Boolean field.
void Field_info::write(Formatter* formatter, Boolean value,
                       Boolean default_value) const
{ formatter->single_boolean(get_name(), value, default_value); }

//! \brief writes the (single) Float field.
void Field_info::write(Formatter* formatter, Float value,
                       Float default_value) const
{ formatter->single_float(get_name(), value, default_value); }

//! \brief writes the (single) Uint field.
void Field_info::write(Formatter* formatter, Uint value,
                       Uint default_value) const
{ formatter->single_uint(get_name(), value, default_value); }

//! \brief writes the (single) Int field.
void Field_info::write(Formatter* formatter, Int32 value,
                       Int32 default_value) const
{ formatter->single_int32(get_name(), value, default_value); }

//! \brief writes the (single) Scene-time field.
void Field_info::write(Formatter* formatter, const Scene_time& value,
                       const Scene_time& default_value) const
{ formatter->single_scene_time(get_name(), value, default_value); }

//! \brief writes the (single) Vector2f field.
void Field_info::write(Formatter* formatter, const Vector2f& value,
                       const Vector2f& default_value) const
{ formatter->single_vector2f(get_name(), value, default_value); }

//! \brief writes the (single) Vector3f field.
void Field_info::write(Formatter* formatter, const Vector3f& value,
                       const Vector3f& default_value) const
{ formatter->single_vector3f(get_name(), value, default_value); }

//! \brief writes the (single) Vector4f field.
void Field_info::write(Formatter* formatter, const Vector4f& value,
                       const Vector4f& default_value) const
{ formatter->single_vector4f(get_name(), value, default_value); }

//! \brief writes the (single) rotation field.
void Field_info::write(Formatter* formatter, const Rotation& value,
                       const Rotation& default_value) const
{ formatter->single_rotation(get_name(), value, default_value); }

//! \brief writes the (single) sphere-bound field.
void Field_info::write(Formatter* formatter, const Sphere_bound& value,
                       const Sphere_bound& default_value) const
{ formatter->single_sphere_bound(get_name(), value, default_value); }

//! \brief writes the (single) string field.
void Field_info::write(Formatter* formatter, const std::string& value,
                       const std::string& default_value) const
{ formatter->single_string(get_name(), value, default_value); }

//! \brief writes the (single) shared-container field.
void Field_info::write(Formatter* formatter, Shared_container value,
                       Shared_container default_value) const
{
  if (!value) return;
  formatter->single_container_begin(get_name());
  formatter->write(value);
  formatter->single_container_end();
}

//! \brief writes the multi-Boolean field.
void Field_info::write(Formatter* formatter, const Boolean_array& value,
                       const Boolean_array& default_value) const
{ formatter->multi_boolean(get_name(), value, default_value); }

//! \brief writes the multi-Float field.
void Field_info::write(Formatter* formatter, const Float_array& value,
                       const Float_array& default_value) const
{ formatter->multi_float(get_name(), value, default_value); }

//! \brief writes the multi-Uint field.
void Field_info::write(Formatter* formatter, const Uint_array& value,
                       const Uint_array& default_value) const
{ formatter->multi_uint(get_name(), value, default_value); }

//! \brief writes the multi-Int field.
void Field_info::write(Formatter* formatter, const Int32_array& value,
                       const Int32_array& default_value) const
{ formatter->multi_int32(get_name(), value, default_value); }

//! \brief writes the multi-Scene_time field.
void Field_info::write(Formatter* formatter, const Scene_time_array& value,
                       const Scene_time_array& default_value) const
{ formatter->multi_scene_time(get_name(), value, default_value); }

//! \brief writes the multi-Vector2f field.
void Field_info::write(Formatter* formatter, const Vector2f_array& value,
                       const Vector2f_array& default_value) const
{ formatter->multi_vector2f(get_name(), value, default_value); }

//! \brief writes the multi-Vector3f field.
void Field_info::write(Formatter* formatter, const Vector3f_array& value,
                       const Vector3f_array& default_value) const
{ formatter->multi_vector3f(get_name(), value, default_value); }

//! \brief writes the multi-Vector4f field.
void Field_info::write(Formatter* formatter, const Vector4f_array& value,
                       const Vector4f_array& default_value) const
{ formatter->multi_vector4f(get_name(), value, default_value);}

//! \brief writes the multi-Rotation field.
void Field_info::write(Formatter* formatter, const Rotation_array& value,
                       const Rotation_array& default_value) const
{ formatter->multi_rotation(get_name(), value, default_value); }

//! \brief writes the multi-Sphere_bound field.
void Field_info::write(Formatter* formatter, const Sphere_bound_array& value,
                       const Sphere_bound_array& default_value) const
{ formatter->multi_sphere_bound(get_name(), value, default_value); }

//! \brief writes the multi-string field.
void Field_info::write(Formatter* formatter, const String_array& value,
                       const String_array& default_value) const
{ formatter->multi_string(get_name(), value, default_value); }

//! \brief writes the multi-shared-container field.
void Field_info::write(Formatter* formatter,
                       const Shared_container_array& value,
                       const Shared_container_array& default_value) const
{
  formatter->multi_container_begin(get_name());
  for (auto it = value.begin(); it != value.end(); ++it) {
    Shared_container cont = *it;
    formatter->write(cont);
  }
  formatter->multi_container_end();
}

SGAL_END_NAMESPACE
