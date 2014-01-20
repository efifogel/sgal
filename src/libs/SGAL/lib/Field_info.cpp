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
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief obtains the attribute of a field in a given container, where this
 * field info contains the information of the field.
 */
void Field_info::add_attribute(Container* container, Element* element,
                               const Boolean& value) const
{
  std::string* value_str = new std::string;
  *value_str = boost::lexical_cast<std::string>(value);
  Element::Str_attr attr(&(get_name()), value_str);
  element->add_attribute(attr);
}

void Field_info::add_attribute(Container* container, Element* element,
                               const Float& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Uint& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Int& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Scene_time& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector2f& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector3f& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector4f& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Rotation& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Sphere_bound& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const std::string& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Boolean_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Float_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Uint_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Int_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Scene_time_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector2f_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector3f_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Vector4f_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Rotation_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Sphere_bound_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const String_array& value) const
{}

void Field_info::add_attribute(Container* container, Element* element,
                               const Shared_container_array& value) const
{}

SGAL_END_NAMESPACE
