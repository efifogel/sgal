// Copyright (c) 2015 Israel.
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

#if defined(_WIN32)
#pragma warning( disable : 4996 )
#endif

#include <iostream>
#include <sstream>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Coord_array_1d.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Coord_array_1d::s_tag = "Coordinate1D";
Container_proto* Coord_array_1d::s_prototype(nullptr);

//! Register to the container factory.
REGISTER_TO_FACTORY(Coord_array_1d, "Coord_array_1d");

//! \brief constructs.
Coord_array_1d::Coord_array_1d(Boolean proto) : Coord_array(proto) {}

//! \brief constructs.
Coord_array_1d::Coord_array_1d(Size n) { m_array.resize(n); }

//! \brief destructs.
Coord_array_1d::~Coord_array_1d() { clear(); }

//! \brief initializes the node prototype.
void Coord_array_1d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Coord_array::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Coord_array::point_changed);

  // point
  Float_array_handle_function array_func =
    static_cast<Float_array_handle_function>(&Coord_array_1d::array_handle);
  s_prototype->add_field_info(new MF_float(POINT, "point",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           array_func, exec_func));
}

//! \brief deletes the node prototype.
void Coord_array_1d::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Coord_array_1d::get_prototype()
{
  if (s_prototype == nullptr) Coord_array_1d::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from an input file.
void Coord_array_1d::set_attributes(Element* elem)
{
  Coord_array::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "point") {
      auto size = get_num_tokens(value);
      m_array.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      for (auto i = 0; i < size; ++i) svalue >> m_array[i];
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
