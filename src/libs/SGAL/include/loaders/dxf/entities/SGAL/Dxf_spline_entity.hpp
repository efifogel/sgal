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

#ifndef SGAL_DXF_SPLINE_ENTITY_HPP
#define SGAL_DXF_SPLINE_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_spline_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;
  typedef std::array<double, 3>                 Double_3;

  /// \name Data members
  //@{

  double m_normal;      // Normal vector (omitted if the spline is nonplanar)
  int16_t m_flags;      // Spline flag (bit coded):
                        // 1 = Closed spline
                        // 2 = Periodic spline
                        // 4 = Rational spline
                        // 8 = Planar
                        // 16 = Linear (planar bit is also set)
  int16_t m_degree_of_curve; // Degree of the spline curve
  double m_knot_tolerance; // Knot tolerance (default = 0.0000001)
  double m_control_point_tolerance; // Control-point tolerance (def = 0.0000001)
  double m_fit_tolerance; // Fit tolerance (default = 0.0000000001)
  double m_start_tangent[3]; // Start tangent—may be omitted (in WCS)
  double m_end_tangent[3]; // End tangent—may be omitted (in WCS)
  std::vector<double> m_knot_values; // Knot value (one entry per knot)
  double m_weight;      // Weight (if not 1); with multiple group pairs,
                        // they are present if all are not 1
  std::vector<Double_3> m_control_points; // Control points (in WCS); one entry
                        // per control point
  std::vector<Double_3> m_fit_points; // Fit points (in WCS); one entry per
                        // fit point

  //@}

  // Construct (set default values).
  Dxf_spline_entity() :
    m_knot_tolerance(0.0000001),
    m_control_point_tolerance(0.0000001),
    m_fit_tolerance(0.0000000001)
  {}

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(int code, int16_t value);

  /*! Handle a value that requires special handling (as opposed to only storing).
   */
  bool handle_value(int code, double value);
};

SGAL_END_NAMESPACE

#endif
