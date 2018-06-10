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

#ifndef DXF_POLYLINE_BOUNDARY_PATH_HPP
#define DXF_POLYLINE_BOUNDARY_PATH_HPP

#include <vector>
#include <array>
#include <map>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_polyline_boundary_path : Dxf_boundary_path {
  int16_t m_has_bulge;  // Has bulge flag
  int16_t m_is_closed;  // Is closed flag
  std::vector<std::array<double, 3> > m_locations; // Vertex location (in OCS)
  double m_bulge;       // Bulge (optional, default = 0)

  /// Handlers
  //@{
  void number_handler(int32_t size);
  void m_location_0_handler(double coord);
  void m_location_1_handler(double coord);
  void m_location_2_handler(double coord);
  //@}
};

DXF_END_NAMESPACE

#endif
