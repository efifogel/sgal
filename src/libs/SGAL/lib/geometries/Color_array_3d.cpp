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

#include <iostream>
#include <sstream>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Color_array_3d.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/multi_istream_iterator.hpp"
#include "SGAL/io_vector3f.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Color_array_3d::s_tag = "Color";
Container_proto* Color_array_3d::s_prototype(nullptr);

//! Register to the container factory.
REGISTER_TO_FACTORY(Color_array_3d, "Color_array_3d");

//! \brief constructs.
Color_array_3d::Color_array_3d(Boolean proto) : Color_array(proto) {}

//! \brief constructs.
Color_array_3d::Color_array_3d(Size n) { m_array.resize(n); }

//! \brief destructs.
Color_array_3d::~Color_array_3d() { clear(); }

//! \brief initializes the node prototype.
void Color_array_3d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Color_array::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Color_array::color_changed);

  // color
  Vector3f_array_handle_function array_func =
    static_cast<Vector3f_array_handle_function>(&Color_array_3d::array_handle);
  s_prototype->add_field_info(new MF_vector3f(COLOR, "color",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              array_func, exec_func));
}

//! \brief deletes the node prototype.
void Color_array_3d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Color_array_3d::get_prototype()
{
  if (s_prototype == nullptr) Color_array_3d::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this container.
void Color_array_3d::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "color") {
      std::stringstream ss(value);
      std::transform(multi_istream_iterator<3>(ss),
                     multi_istream_iterator<3>(),
                     std::back_inserter(m_array),
                     &boost::lexical_cast<Vector3f, String>);
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
