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
#include "SGAL/Vector2f.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_polyline_boundary_path : Dxf_boundary_path {
  int16_t m_has_bulge;  // Has bulge flag
  int16_t m_is_closed;  // Is closed flag
  std::vector<SGAL::Vector2f> m_locations; // Vertex location (in OCS)
  std::vector<double> m_bulges; // Bulge (optional, default = 0)

  /// Handlers
  //@{
  void number_handler(int32_t size);
  void location_0_handler(double coord);
  void location_1_handler(double coord);
  void bulge_handler(double bulge);
  //@}

  //! Construct default
  Dxf_polyline_boundary_path();
};

//! \brief constructs
inline Dxf_polyline_boundary_path::Dxf_polyline_boundary_path() :
  m_has_bulge(false),
  m_is_closed(false)
{}

DXF_END_NAMESPACE

#endif
