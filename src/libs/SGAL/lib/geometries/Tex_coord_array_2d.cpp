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

#include <iostream>
#include <sstream>

#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Tex_coord_array_2d::s_tag = "TextureCoordinate";
Container_proto* Tex_coord_array_2d::s_prototype(nullptr);

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Tex_coord_array_2d, "Tex_coord_array_2d");

//! \brief Constructs.
Tex_coord_array_2d::Tex_coord_array_2d(Boolean proto) : Tex_coord_array(proto)
{}

//! \brief Constructs.
Tex_coord_array_2d::Tex_coord_array_2d(Size n) { m_array.resize(n); }

//! \brief Destructs.
Tex_coord_array_2d::~Tex_coord_array_2d() {}

//! \brief initializes the node prototype.
void Tex_coord_array_2d::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Tex_coord_array::get_prototype());
}

//! \brief deletes the node prototype.
void Tex_coord_array_2d::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;

  // point
  Vector2f_array_handle_function array_func =
    static_cast<Vector2f_array_handle_function>
    (&Tex_coord_array_2d::array_handle);
  s_prototype->add_field_info(new MF_vector2f(POINT, "point",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              array_func));
}

//! \brief obtains the node prototype.
Container_proto* Tex_coord_array_2d::get_prototype()
{
  if (s_prototype == NULL) Tex_coord_array_2d::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Tex_coord_array_2d::set_attributes(Element* elem)
{
  Tex_coord_array::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "point") {
      auto num_values = get_num_tokens(value);
      auto size = num_values / 2;
      m_array.resize(size);
      //! svalue.seekg(0); why this doesn't work?
      std::istringstream svalue(value, std::istringstream::in);
      for (auto i = 0; i < size; ++i) svalue >> m_array[i][0] >> m_array[i][1];
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! \brief
Attribute_list Tex_coord_array_2d::get_attributes()
{
  Attribute_list attribs;
  attribs = Tex_coord_array::get_attributes();
  return attribs;
}
#endif

SGAL_END_NAMESPACE
