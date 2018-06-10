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

#ifndef DXF_CIRCLE_EDGE_HPP
#define DXF_CIRCLE_EDGE_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_edge.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_circle_edge : Dxf_edge {
  /// Record members
  //@{
  double m_center[2];   // Center point (in OCS)
  double m_radius;      // Radius
  double m_start_angle; // Start angle
  double m_end_angle;   // End angle
  int16_t m_is_counterclockwise; // Is counterclockwise flag
  //@}
};

DXF_END_NAMESPACE

#endif
