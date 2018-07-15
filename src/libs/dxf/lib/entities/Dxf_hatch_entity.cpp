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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <map>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_boundary_path.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_hatch_entity>  Dxf_hatch_entity_wrapper;

//! Record members
template <>
const std::map<int, Dxf_hatch_entity_wrapper::Record_member>
Dxf_hatch_entity_wrapper::s_record_members = {
  {10, {&Dxf_hatch_entity::m_elevation_point, 3, 0}},
  {20, {&Dxf_hatch_entity::m_elevation_point, 3, 1}},
  {30, {&Dxf_hatch_entity::m_elevation_point, 3, 2}},
  {210, {&Dxf_hatch_entity::m_extrusion_direction, 3, 0}},
  {220, {&Dxf_hatch_entity::m_extrusion_direction, 3, 1}},
  {230, {&Dxf_hatch_entity::m_extrusion_direction, 3, 2}},
  {2, {&Dxf_hatch_entity::m_hatch_pattern_name, 1, 0}},
  {70, {&Dxf_hatch_entity::m_flags, 1, 0}},
  {63, {&Dxf_hatch_entity::m_pattern_fill_color, 1, 0}},
  {71, {&Dxf_hatch_entity::m_associativity_flags, 1, 0}},
  // {91, // Number of boundary paths (loops)
  // varies m_boundary_path data; // Boundary path data. Repeats number of
  {75, {&Dxf_hatch_entity::m_hatch_style, 1, 0}},
  {76, {&Dxf_hatch_entity::m_hatch_pattern_type, 1, 0}},
  {52, {&Dxf_hatch_entity::m_hatch_pattern_angle, 1, 0}},
  {41, {&Dxf_hatch_entity::m_hatch_pattern_scale, 1, 0}},
  {73, {&Dxf_hatch_entity::m_boundary_annotation_flag, 1, 0}},
  {77, {&Dxf_hatch_entity::m_hatch_pattern_double_flag, 1, 0}},
  // {78, {&Dxf_hatch_entity::m_num_pattern_definition_lines, 1, 0}},
  // varies // Pattern line data. Repeats number of times specified by code 78.
  {47, {&Dxf_hatch_entity::m_pixel_size, 1, 0}},
  {98, {&Dxf_hatch_entity::m_num_sed_points, 1, 0}},
  {11, {&Dxf_hatch_entity::m_offset_vetor, 1, 0}},
  {99, {&Dxf_hatch_entity::m_num_loops, 1, 0}},
  {10, {&Dxf_hatch_entity::m_seed_point, 3, 0}},
  {20, {&Dxf_hatch_entity::m_seed_point, 3, 1}},
  {30, {&Dxf_hatch_entity::m_seed_point, 3, 2}},
  {450, {&Dxf_hatch_entity::m_solid_hatch, 1, 0}},
  {451, {&Dxf_hatch_entity::m_reserved1, 1, 0}},
  {452, {&Dxf_hatch_entity::m_color_defined, 1, 0}},
  {453, {&Dxf_hatch_entity::m_num_colors, 1, 0}},
  {460, {&Dxf_hatch_entity::m_rotation_angle, 1, 0}},
  {461, {&Dxf_hatch_entity::m_gradient, 1, 0}},
  {462, {&Dxf_hatch_entity::m_color_tint_value, 1, 0}},
  {463, {&Dxf_hatch_entity::m_reserved2, 1, 0}},
  {470, {&Dxf_hatch_entity::m_string, 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_hatch_entity_wrapper::Record_handler_type>
Dxf_hatch_entity_wrapper::s_record_handlers = {
  {91, &Dxf_hatch_entity::handle_boundary_paths},
  {78, &Dxf_hatch_entity::handle_pattern_definition_lines_num}
};

//! \brief handles boundary paths.
void Dxf_hatch_entity::handle_boundary_paths(int32_t size)
{
  m_boundary_paths.resize(size);
  for (auto& boundary_path : m_boundary_paths) {
    int code;
    m_parser->import_code(code);
    SGAL_assertion(92 == code);

    int32_t type;
    m_parser->import_value(type);
    if (type & Dxf_boundary_path::POLYLINE) {
      auto* path = new Dxf_polyline_boundary_path;
      path->set_parser(m_parser);
      m_parser->parse_polyline_boundary_path(*path);
      boundary_path = path;
      continue;
    }

    auto* path = new Dxf_boundary_path;
    path->set_parser(m_parser);
    m_parser->parse_boundary_path(*path);
    boundary_path = path;
  }
}

//! \brief handles a value that requires special handling.
void Dxf_hatch_entity::handle_pattern_definition_lines_num(int16_t size)
{
  m_pattern_line.resize(size);
  for (auto& pattern_data : m_pattern_line)
    m_parser->parse_pattern_data(pattern_data);
}

DXF_END_NAMESPACE
