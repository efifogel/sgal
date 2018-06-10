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

#ifndef DXF_SPLINE_EDGE_HPP
#define DXF_SPLINE_EDGE_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_edge.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_spline_edge : Dxf_edge {
  /// Record members
  //@{
#if 0
  94 m_degree; // Degree
  73 m_rational; // Rational
  74 m_periodic; // Periodic
  95 m_knots_num; // Number of knots
  96 m_control_points_num; // Number of control points
  40 m_knot_values; // Knot values (multiple entries)
  10 m_control_points; // Control point (in OCS)
  20 m_control_points; // Control point (in OCS)
  42 m_weights; // Weights (optional, default = 1)
#endif
  //@}

  /// Record handlers
  //@{

  //@}
};

DXF_END_NAMESPACE

#endif
