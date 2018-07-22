// Copyright (c) 2016 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_configuration.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Dxf_configuration::s_tag = "DxfConfiguration";
Container_proto* Dxf_configuration::s_prototype(nullptr);

// Default values
const String Dxf_configuration::s_def_palette_file_name("dxf_palette.txt");

//! \brief constructs.
Dxf_configuration::Dxf_configuration(Boolean proto) :
  Container(proto),
  m_palette_file_name(s_def_palette_file_name)
{}

//! \brief initializes the node prototype.
void Dxf_configuration::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);

  // makeConsistent
  auto palette_file_name_func =
    static_cast<String_handle_function>(&Dxf_configuration::palette_file_name_handle);
  s_prototype->add_field_info(new SF_string(PALETTE_FILE_NAME, "paletteFileName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            palette_file_name_func,
                                            s_def_palette_file_name,
                                            exec_func));
}

//! \brief deletes the node prototype.
void Dxf_configuration::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Dxf_configuration::get_prototype()
{
  if (!s_prototype) Dxf_configuration::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Dxf_configuration::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);

    if (name == "paletteFileName") {
      set_palette_file_name(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief sets defualt values.
void Dxf_configuration::reset(const String& def_palette_file_name)
{
  m_palette_file_name = def_palette_file_name;
}

SGAL_END_NAMESPACE
