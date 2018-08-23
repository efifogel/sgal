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

#include "SGAL/Color_array.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Color_array::s_prototype(nullptr);

//! \brief initializes the node prototype.
void Color_array::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());
}

//! \brief deletes the node prototype.
void Color_array::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Color_array::get_prototype()
{
  if (s_prototype == nullptr) Color_array::init_prototype();
  return s_prototype;
}

//! \brief processes change of points.
void Color_array::color_changed(const Field_info* field_info)
{
  set_rendering_required();
  field_changed(field_info);
}

//! \brief sets the attributes of this container.
void Color_array::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
