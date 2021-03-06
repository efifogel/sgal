// Copyright (c) 2018 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_simple_record_wrapper.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

//! Default values
const int16_t Dxf_base_entity::s_def_is_in_paper_space(0);
const SGAL::String Dxf_base_entity::s_def_line_type_name("BYLAYER");
const int16_t Dxf_base_entity::s_def_color_index(static_cast<int16_t>(By::BYLAYER));
const double Dxf_base_entity::s_def_line_type_scale(-1);
const int16_t Dxf_base_entity::s_def_is_visible(0);
const int32_t Dxf_base_entity::s_def_color(static_cast<int32_t>(-1));
const int32_t Dxf_base_entity::s_def_transparency(0);

//!
typedef Dxf_simple_record_wrapper<Dxf_base_entity>      Base_entity_wrapper;
template <>
const std::map<int, Base_entity_wrapper::Record_member_type>
Base_entity_wrapper::s_record_members = {
  {5, &Dxf_base_entity::m_handle},
  {330, &Dxf_base_entity::m_owner_handle},
  {360, &Dxf_base_entity::m_owner_dict},
  // {330, &Dxf_base_entity::m_owner_block}, reuse of same code ???
  {67, &Dxf_base_entity::m_is_in_paper_space},
  {410, &Dxf_base_entity::m_layout_tab_name},
  {8, &Dxf_base_entity::m_layer},
  {6, &Dxf_base_entity::m_line_type_name},
  {62, &Dxf_base_entity::m_color_index},
  {370, &Dxf_base_entity::m_lineweight_enum_value},
  {48, &Dxf_base_entity::m_line_type_scale},
  {60, &Dxf_base_entity::m_is_visible},
  {92, &Dxf_base_entity::m_image_byte_count},
   // {310, &Dxf_base_entity::m_preview_image_data},
  {420, &Dxf_base_entity::m_color},
  {430, &Dxf_base_entity::m_color_name},
  {440, &Dxf_base_entity::m_transparency},
  {390, &Dxf_base_entity::m_plot_style},
  {284, &Dxf_base_entity::m_shadow_mode}
};

//! \brief constructs.
Dxf_base_entity::Dxf_base_entity() :
  m_is_in_paper_space(s_def_is_in_paper_space),
  m_line_type_name(s_def_line_type_name),
  m_color_index(s_def_color_index),
  m_line_type_scale(s_def_line_type_scale),
  m_is_visible(s_def_is_visible),
  m_color(s_def_color),
  m_transparency(s_def_transparency),
  m_parser(nullptr)
{}

DXF_END_NAMESPACE
