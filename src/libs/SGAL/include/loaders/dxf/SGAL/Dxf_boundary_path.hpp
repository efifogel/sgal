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

#ifndef SGAL_DXF_BOUNDARY_PATH_HPP
#define SGAL_DXF_BOUNDARY_PATH_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_boundary_path {
  enum Path_type {
    DEFAULT = 0x0,
    EXTERNAL = 0x1,
    POLYLINE = 0x2,
    DERIVED = 0x4,
    TEXTBOX = 0x8,
    OUTERMOST = 0x10
  };

  /*! Desctruct.
   * This vrtual destructor is necessary to make this struct polymorphic.
   * Note, that it is used as a base class for several boundary-path types
   * dynamically allocated.
   */
  virtual ~Dxf_boundary_path() = default;

  /// Record members
  //@{

  int32_t m_flags;      // Boundary path type flag (bit coded):
                        // 0 = Default
                        // 1 = External
                        // 2 = Polyline
                        // 4 = Derived
                        // 8 = Textbox
                        // 16 = Outermost
  // varies Polyline boundary type data (only if boundary = polyline).
                        // See Polyline boundary data table below
  // Number of edges in this boundary path (only if boundary is not a
                        // polyline)
  int16_t m_edge_type;  // Edge type (only if boundary is not a polyline):
                        // 1 = Line
                        // 2 = Circular arc
                        // 3 = Elliptic arc
                        // 4 = Spline
  // varies Edge type data (only if boundary is not a polyline).
                        // See appropriate Edge data table below
  // Number of source boundary objects
  std::vector<Uint> m_source_objects; // Reference to source boundary objects
                        // (multiple entries)

  //@}

  /// Record handlers
  //@{
  void number_handler(int32_t size);
  void source_objects_handler(Uint handler);
  //@}

#if 0
  // EF: Probably good to rename Dxf_boundary_path to Dxf_base_boundary_path
  // and then introduce:
  // 1. Dxf_line_boundary_path
  // 2. Dxf_circular_arc_boundary_path
  // 3. Dxf_elliptic_boundary_path
  // 4. Dxf_spline_boundary_path
Line edge data group codes
Group code
Description
10 Start point (in OCS)
DXF: X value; APP: 2D point
20 DXF: Y value of start point (in OCS)
11 Endpoint (in OCS)
DXF: X value; APP: 2D point
21 DXF: Y value of endpoint (in OCS)
Arc edge data group codes
Group code
Description
10 Center point (in OCS)
DXF: X value; APP: 2D point
20 DXF: Y value of center point (in OCS)
40 Radius
50 Start angle
51 End angle
73 Is counterclockwise flag
#endif
};

SGAL_END_NAMESPACE

#endif
