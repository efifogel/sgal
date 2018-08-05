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

#ifndef DXF_POLYLINE_ENTITY_HPP
#define DXF_POLYLINE_ENTITY_HPP

#include <vector>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"
#include "dxf/Dxf_vertex_entity.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_polyline_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;
  typedef std::vector<Dxf_vertex_entity>        Vertices;

  /// Record members
  //@{

  enum {
    CLOSED = 1,
    INCLUDES_CURVE_FIT_VERTICES = 2,
    INCLUDES_SPLINE_FIT_VERTICES = 4,
    IS_3D_POLYLINE = 8,
    IS_3D_POLYGON_MESH = 16,
    IS_3D_POLYGON_MESH_CLOSED = 32,
    IS_POLY_FACE_MESH = 64,
    HAS_CONTINUOUS_LINE_TYPE_PATTERN = 128
  };

  double m_elevation[3];// APP: a "dummy" point; the X and Y values are always
                        // 0, and the Z value is the polyline's elevation (in
                        // OCS when 2D, WCS when 3D)
  double m_thickness;   // Thickness (optional; default = 0)
  int16_t m_flag;       // Polyline flag (bit-coded); default is 0:
                        //   1 = This is a closed polyline (or a polygon mesh
                        //       closed in the M direction).
                        //   2 = Curve-fit vertices have been added.
                        //   4 = Spline-fit vertices have been added.
                        //   8 = This is a 3D polyline.
                        //  16 = This is a 3D polygon mesh.
                        //  32 = The polygon mesh is closed in the N direction.
                        //  64 = The polyline is a polyface mesh.
                        // 128 = The linetype pattern is generated continuously
                        //       around the vertices of this polyline.
  double m_start_width; // Default start width (optional; default = 0)
  double m_end_width;   // Default end width (optional; default = 0)
  int16_t m_m_count;    // Polygon mesh M vertex count (optional; default = 0)
  int16_t m_n_count;    // Polygon mesh N vertex count (optional; default = 0)
  int16_t m_m_density;  // Smooth surface M density (optional; default = 0)
  int16_t m_n_density;  // Smooth surface N density (optional; default = 0)
  int16_t m_type;       // Curves and smooth surface type (optional;
                        // default = 0); integer codes, not bit-coded:
                        // 0 = No smooth surface fitted
                        // 5 = Quadratic B-spline surface
                        // 6 = Cubic B-spline surface
                        // 8 = Bezier surface
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)

  std::vector<Dxf_vertex_entity> m_vertex_entities;

  //@}

  /*! Determine whether the polyline is closed.
   */
  bool is_closed() const { return m_flag & CLOSED; }

  /*! Determine whether the polyline is in 3d.
   */
  bool is_3d() const { return m_flag & IS_3D_POLYLINE; }

  /*! Determine whether the polyline has a bulge.
   * \return true if the polyline has a bulge and false otherwise.
   */
  bool has_bulge() const
  {
    for (auto& vertex : m_vertex_entities)
      if (vertex.m_bulge != 0.0) return true;
    return false;
  }

  /*! Construct (set default values).
   */
  Dxf_polyline_entity() :
    m_thickness(0),
    m_flag(0),
    m_start_width(0),
    m_end_width(0),
    m_m_count(0),
    m_n_count(0),
    m_m_density(0),
    m_n_density(0),
    m_type(0),
    m_extrusion_direction{0, 0, 1}
  {}

  /// Member handlers
  //@{

  /*! Handle obsolete.
   */
  void handle_obsolete(int16_t value) {}

  //@}

  /*! Iterator to a point.
   */
  class Point_const_iterator :
    public std::iterator<std::bidirectional_iterator_tag, const double*>
  {
  private:
    typedef Point_const_iterator        iterator;

    //! The internal itertor.
    std::vector<Dxf_vertex_entity>::const_iterator m_it;

  public:
    Point_const_iterator() {}
    Point_const_iterator(Vertices::const_iterator it) : m_it(it) {}
    ~Point_const_iterator() {}

    iterator& operator++() /* prefix */ { ++m_it; return *this; }
    iterator& operator--() /* prefix */ { --m_it; return *this; }
    const double* operator* () const { return &(m_it->m_location[0]); }
    // pointer operator->() const { return &(m_it->m_location); }
    bool operator==(const iterator& rhs) const { return m_it == rhs.m_it; }
    bool operator!=(const iterator& rhs) const { return m_it != rhs.m_it; }
  };

  /*! Iterator to a bulge.
   */
  class Bulge_const_iterator :
    public std::iterator<std::bidirectional_iterator_tag, const double>
  {
  private:
    typedef Bulge_const_iterator        iterator;

    //! The internal itertor.
    std::vector<Dxf_vertex_entity>::const_iterator m_it;

  public:
    Bulge_const_iterator() {}
    Bulge_const_iterator(Vertices::const_iterator it) : m_it(it) {}
    ~Bulge_const_iterator() {}

    iterator& operator++() /* prefix */ { ++m_it; return *this; }
    iterator& operator--() /* prefix */ { --m_it; return *this; }
    reference operator* () const { return m_it->m_bulge; }
    pointer operator->() const { return &(m_it->m_bulge); }
    bool operator==(const iterator& rhs) const { return m_it == rhs.m_it; }
    bool operator!=(const iterator& rhs) const { return m_it != rhs.m_it; }
  };

  /*! Obtain the begin iterator of points.
   * Note that the value type is a pointer to a double.
   */
  Point_const_iterator points_begin() const
  { return Point_const_iterator(m_vertex_entities.begin()); }

  /*! Obtain the past-the-end iterator of points.
   * Note that the value type is a pointer to a double.
   */
  Point_const_iterator points_end() const
  { return Point_const_iterator(m_vertex_entities.end()); }

  /*! Obtain the begin iterator of bulges.
   */
  Bulge_const_iterator bulges_begin() const
  { return Bulge_const_iterator(m_vertex_entities.begin()); }

  /*! Obtain the past-the-end iterator of bulges.
   */
  Bulge_const_iterator bulges_end() const
  { return Bulge_const_iterator(m_vertex_entities.end()); }
};

DXF_END_NAMESPACE

#endif
