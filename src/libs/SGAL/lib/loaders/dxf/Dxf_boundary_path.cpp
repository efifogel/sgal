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

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_boundary_path.hpp"
#include "SGAL/Dxf_parser.hpp"
#include "SGAL/Dxf_line_edge.hpp"
#include "SGAL/Dxf_circle_edge.hpp"
#include "SGAL/Dxf_ellipse_edge.hpp"
#include "SGAL/Dxf_spline_edge.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_boundary_path>   Dxf_boundary_path_wrapper;

//! record members initialization.
template <>
const std::map<int, Dxf_boundary_path_wrapper::Record_member>
Dxf_boundary_path_wrapper::s_record_members = {
  // {93, Number of edges in this boundary path (only if boundary is not a
                        // polyline)
};

//! record handlers initialization.
template <>
const std::map<int, Dxf_boundary_path_wrapper::Record_handler_type>
Dxf_boundary_path_wrapper::s_record_handlers = {
  {93, &Dxf_boundary_path::handle_edges_num},
};

//! \brief handles the number of edges.
void Dxf_boundary_path::handle_edges_num(int32_t size)
{
  m_edges.resize(size);
  for (auto& edge : m_edges) {
    int code;
    m_parser->import_code(code);
    SGAL_assertion(72 == code);
    auto ct = m_parser->code_type(code);

    int16_t type;
    m_parser->import_value(type);
    if (type == Dxf_edge::LINE) {
      auto* line_edge = new Dxf_line_edge;
      line_edge->set_parser(m_parser);
      m_parser->parse_line_edge(*line_edge);
      edge = line_edge;
      continue;
    }

    if (type == Dxf_edge::CIRCULAR_ARC) {
      auto* circle_edge = new Dxf_circle_edge;
      circle_edge->set_parser(m_parser);
      m_parser->parse_circle_edge(*circle_edge);
      edge = circle_edge;
      continue;
    }

    if (type == Dxf_edge::ELLIPTIC_ARC) {
      auto* ellipse_edge = new Dxf_ellipse_edge;
      ellipse_edge->set_parser(m_parser);
      m_parser->parse_ellipse_edge(*ellipse_edge);
      edge = ellipse_edge;
      continue;
    }

    if (type == Dxf_edge::SPLINE) {
      auto* spline_edge = new Dxf_spline_edge;
      spline_edge->set_parser(m_parser);
      m_parser->parse_spline_edge(*spline_edge);
      edge = spline_edge;
      continue;
    }
  }
}

//! \brief initializes
void Dxf_boundary_path::init()
{
  for (auto* edge : m_edges) delete edge;
  m_edges.clear();
}

SGAL_END_NAMESPACE
