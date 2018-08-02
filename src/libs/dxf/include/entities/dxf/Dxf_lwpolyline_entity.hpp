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

#ifndef DXF_LWPOLYLINE_ENTITY_HPP
#define DXF_LWPOLYLINE_ENTITY_HPP

#include <vector>
#include <array>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_lwpolyline_entity : public Dxf_base_entity {

  enum {
    CLOSED = 1,
    PLINEGEN = 128
  };

  /// Record members
  //@{

  typedef Dxf_base_entity                       Base;

  int16_t m_flags;      // Polyline flag (bit-coded); default is 0:
                        // 1 = Closed; 128 = Plinegen
  double m_constant_width; // Constant width (optional; default = 0). Not
                        // used if variable width (codes 40 and/or 41) is set
  double m_elevation;   // Elevation (optional; default = 0)
  double m_thickness;   // Thickness (optional; default = 0)
  std::vector<SGAL::Vector2f> m_vertices; // Vertex coordinates (in OCS),
                        // multiple entries; one entry for each vertex
  int32_t m_identifier; // Vertex identifier
  double m_start_width; // Starting width (multiple entries; one entry for
                        // each vertex) (optional; default = 0; multiple
                        // entries). Not used if constant width (code 43) is set
  double m_end_width;   // End width (multiple entries; one entry for each
                        // vertex) (optional; default = 0; multiple entries).
                        // Not used if constant width (code 43) is set
  std::vector<double> m_bulges; // Bulge (multiple entries; one entry for each
                        // vertex) (optional; default = 0)
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)

  //@}

  /*! Construct (set default values).
   */
  Dxf_lwpolyline_entity() :
    m_flags(CLOSED),
    m_constant_width(0),
    m_elevation(0),
    m_thickness(0),
    m_start_width(0),
    m_end_width(0),
    m_extrusion_direction{0, 0, 1}
  {}

  /*! Determine whether the polyline is closed.
   * \return true if the polyline is closed and false otherwise.
   */
  bool is_closed() const { return m_flags & CLOSED; }

  /*! Determine whether the polyline has a bulge.
   * \return true if the polyline has a bulge and false otherwise.
   */
  bool has_bulge()
  {
    for (auto& bulge : m_bulges) if (bulge != 0.0) return true;
    return false;
  }

  /// Record handlers
  //@{

  //! Handle the x-coordinate of a vertex.
  void handle_vertex_x(double x)
  {
    m_vertices.resize(m_vertices.size() + 1);
    m_vertices.back()[0] = x;
    m_bulges.resize(m_bulges.size() + 1);
    m_bulges.back() = 0;
  }

  //! Handle the y-coordinate of a vertex.
  void handle_vertex_y(double y)
  {
    SGAL_assertion(! m_vertices.empty());
    m_vertices.back()[1] = y;
  }

  //! Handle a bulge.
  void handle_bulge(double bulge)
  {
    m_bulges.back() = bulge;
  }

  //! Handle the number of vertices.
  void handle_vertices_num(int32_t size)
  {
    m_vertices.reserve(size);
    m_bulges.reserve(size);
  }

  //@}
};

DXF_END_NAMESPACE

#endif
