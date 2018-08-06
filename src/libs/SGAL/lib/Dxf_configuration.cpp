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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>

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
const Vector4f Dxf_configuration::s_def_background_color{0.9, 0.9, 0.9, 1};
const Float Dxf_configuration::s_def_min_bulge(0.1f);
const Uint Dxf_configuration::s_def_refinement_arcs_num(16);

//! \brief constructs.
Dxf_configuration::Dxf_configuration(Boolean proto) :
  Container(proto),
  m_palette_file_name(s_def_palette_file_name),
  m_background_color(s_def_background_color),
  m_min_bulge(s_def_min_bulge)
{}

//! \brief initializes the node prototype.
void Dxf_configuration::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  auto exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);

  // paletteFileName
  auto palette_file_name_func =
    static_cast<String_handle_function>
    (&Dxf_configuration::palette_file_name_handle);
  s_prototype->add_field_info(new SF_string(PALETTE_FILE_NAME, "paletteFileName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            palette_file_name_func,
                                            s_def_palette_file_name,
                                            exec_func));

  // backgroundColor
  auto background_color_func =
    static_cast<Vector4f_handle_function>
    (&Dxf_configuration::background_color_handle);
  s_prototype->add_field_info(new SF_vector4f(BACKGROUND_COLOR,
                                              "backgroundColor",
                                              Field_rule::RULE_EXPOSED_FIELD,
                                              background_color_func,
                                              s_def_background_color,
                                              exec_func));

  // minBulge
  auto min_bulge_func =
    static_cast<Float_handle_function>(&Dxf_configuration::min_bulge_handle);
  s_prototype->add_field_info(new SF_float(MIN_BULGE,
                                           "minBulge",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           min_bulge_func,
                                           s_def_min_bulge,
                                           exec_func));

  // refinementArcsNum
  auto refinement_arcs_num_func =
    static_cast<Uint_handle_function>
    (&Dxf_configuration::refinement_arcs_num_handle);
  s_prototype->add_field_info(new SF_uint(REFINMENT_ARCS_NUM,
                                           "refinementArcsNum",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           refinement_arcs_num_func,
                                           s_def_refinement_arcs_num,
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
    if (name == "backgroundColor") {
      set_background_color(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "minBulge") {
      set_min_bulge(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "refinementArcsNum") {
      set_refinement_arcs_num(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief sets defualt values.
void Dxf_configuration::reset(const String& def_palette_file_name,
                              const Vector4f& def_background_color,
                              const Float def_min_bulge,
                              const Uint def_refinement_arcs_num)
{
  m_palette_file_name = def_palette_file_name;
  m_background_color = def_background_color;
  m_min_bulge = def_min_bulge;
  m_refinement_arcs_num = def_refinement_arcs_num;
}

SGAL_END_NAMESPACE
