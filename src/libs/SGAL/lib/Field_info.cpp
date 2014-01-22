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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief obtains the attribute of a field in a given container, where this
 * field info contains the information of the field.
 */
void Field_info::write(Formatter* formatter, Boolean value) const
{ formatter->single_boolean(get_name(), value, false); }

void Field_info::write(Formatter* formatter, const Float& value) const
{ formatter->single_float(get_name(), value, 0); }

void Field_info::write(Formatter* formatter, const Uint& value) const
{ formatter->single_uint(get_name(), value, 0); }

void Field_info::write(Formatter* formatter, const Int& value) const
{ formatter->single_int(get_name(), value, 0); }

void Field_info::write(Formatter* formatter, const Scene_time& value) const
{}

void Field_info::write(Formatter* formatter, const Vector2f& value) const
{}

void Field_info::write(Formatter* formatter, const Vector3f& value) const
{ formatter->single_vector3f(get_name(), value, Vector3f()); }

void Field_info::write(Formatter* formatter, const Vector4f& value) const
{}

void Field_info::write(Formatter* formatter, const Rotation& value) const
{}

void Field_info::write(Formatter* formatter, const Sphere_bound& value) const
{}

void Field_info::write(Formatter* formatter, const std::string& value) const
{}

void Field_info::write(Formatter* formatter, Shared_container value) const
{
  if (!value) return;
  formatter->single_container_begin(get_name());
  formatter->write(&*value);
  formatter->single_container_end();
}

void Field_info::write(Formatter* formatter, const Boolean_array& value) const
{}

void Field_info::write(Formatter* formatter, const Float_array& value) const
{}

void Field_info::write(Formatter* formatter, const Uint_array& value) const
{}

void Field_info::write(Formatter* formatter, const Int_array& value) const
{}

void Field_info::write(Formatter* formatter, const Scene_time_array& value) const
{}

void Field_info::write(Formatter* formatter, const Vector2f_array& value) const
{}

void Field_info::write(Formatter* formatter, const Vector3f_array& value) const
{}

void Field_info::write(Formatter* formatter, const Vector4f_array& value) const
{}

void Field_info::write(Formatter* formatter, const Rotation_array& value) const
{}

void Field_info::write(Formatter* formatter, const Sphere_bound_array& value) const
{}

void Field_info::write(Formatter* formatter, const String_array& value) const
{}

void Field_info::write(Formatter* formatter, const Shared_container_array& value) const
{}

SGAL_END_NAMESPACE
