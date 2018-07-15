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

#ifndef DXF_SPLINE_ENTITY_HPP
#define DXF_SPLINE_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

DXF_BEGIN_NAMESPACE

class Dxf_parser;

struct Dxf_spline_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;
  typedef std::array<double, 3>                 Double_3;

  enum Type {
    CLOSED = 0x1,
    PERIODIC = 0x2,
    RATIONAL = 0x4,
    PLANAR = 0x8,
    LINEAR = 0x10
  };

  /// \name Data members
  //@{

  double m_normal[3];   // Normal vector (omitted if the spline is nonplanar)
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
  double m_start_tangent[3]; // Start tangent; may be omitted (in WCS)
  double m_end_tangent[3]; // End tangent; may be omitted (in WCS)
  std::vector<double> m_knot_values; // Knot value (one entry per knot)
  std::vector<SGAL::Vector3f> m_control_points; // Control points (in WCS); one
                        // entry per control point
  std::vector<double> m_weights; // Weight (if not 1); with multiple group pairs,
                        // they are present if all are not 1
  std::vector<SGAL::Vector3f> m_fit_points; // Fit points (in WCS); one entry
                        // per fit point

  //@}

  /*! Construct (set default values).
   */
  Dxf_spline_entity() :
    m_knot_tolerance(0.0000001),
    m_control_point_tolerance(0.0000001),
    m_fit_tolerance(0.0000000001)
  {}

  /*! Handle the number of knot values.
   */
  void handle_knot_values_num(int16_t size) { m_knot_values.reserve(size); }

  /*! Handle the number of control points.
   */
  void handle_control_points_num(int16_t size)
  {
    m_control_points.reserve(size);
    m_weights.reserve(size);
  }

  /*! Handle the number of fit points.
   */
  void handle_fit_points_num(int16_t size) { m_fit_points.reserve(size); }

  /*! Handle a knot value.
   */
  void handle_knot_value(double value)
  {
    m_knot_values.resize(m_knot_values.size() + 1);
    m_knot_values.back() = value;
  }

  /*! Handle the x-coordinate of a control point.
   */
  void handle_control_point_x(double value)
  {
    m_control_points.resize(m_control_points.size() + 1);
    m_control_points.back()[0] = value;
    m_weights.resize(m_weights.size() + 1);
    m_weights.back() = 1;
  }

  /*! Handle the y-coordinate of a control point.
   */
  void handle_control_point_y(double value)
  {
    SGAL_assertion(! m_control_points.empty());
    m_control_points.back()[1] = value;
  }

  /*! Handle the z-coordinate of a control point.
   */
  void handle_control_point_z(double value)
  {
    SGAL_assertion(! m_control_points.empty());
    m_control_points.back()[2] = value;
  }

  /*! Handle the weight of a control point.
   */
  void handle_weight(double value)
  {
    SGAL_assertion(! m_weights.empty());
    m_weights.back() = value;
  }

  /*! Handle the x-coordinate of a fit point.
   */
  void handle_fit_point_x(double value)
  {
    m_fit_points.resize(m_fit_points.size() + 1);
    m_fit_points.back()[0] = value;
  }

  /*! Handle the y-coordinate of a fit point.
   */
  void handle_fit_point_y(double value)
  {
    SGAL_assertion(! m_fit_points.empty());
    m_fit_points.back()[1] = value;
  }

  /*! Handle the z-coordinate of a fit point.
   */
  void handle_fit_point_z(double value)
  {
    SGAL_assertion(! m_fit_points.empty());
    m_fit_points.back()[2] = value;
  }
};

DXF_END_NAMESPACE

#endif
