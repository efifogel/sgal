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

#ifndef SGAL_DXF_LWPOLYLINE_ENTITY_HPP
#define SGAL_DXF_LWPOLYLINE_ENTITY_HPP

#include <vector>
#include <array>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_lwpolyline_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  int16_t m_flags;      // Polyline flag (bit-coded); default is 0:
                        // 1 = Closed; 128 = Plinegen
  double m_constant_width; // Constant width (optional; default = 0). Not
                        // used if variable width (codes 40 and/or 41) is set
  double m_elevation;   // Elevation (optional; default = 0)
  double m_thickness;   // Thickness (optional; default = 0)
  std::vector<std::array<double, 3> >m_vertices; // Vertex coordinates (in OCS),
                        // multiple entries; one entry for each vertex
  int32_t m_identifier; // Vertex identifier
  double m_starting_width; // Starting width (multiple entries; one entry for
                        // each vertex) (optional; default = 0; multiple
                        // entries). Not used if constant width (code 43) is set
  double m_end_width;   // End width (multiple entries; one entry for each
                        // vertex) (optional; default = 0; multiple entries).
                        // Not used if constant width (code 43) is set
  std::vector<double> m_bulges; // Bulge (multiple entries; one entry for each
                        // vertex) (optional; default = 0)
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)

  //! \brief handles a value that requires special handling.
  bool handle_value(Dxf_parser& parser, int code, double value);

  //! \brief handles a value that requires special handling.
  bool handle_value(Dxf_parser& parser, int code, int32_t value);
};

SGAL_END_NAMESPACE

#endif
