// Copyright (c) 2015 Israel.
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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FONT_GEOMETRY_HPP
#define SGAL_FONT_GEOMETRY_HPP

#include <list>
#include <vector>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Bounding_box.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Face_nesting_level.hpp"
#include "SGAL/construct_triangulation.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Glyph_geometry {
public:
  /*! Construct default. */
  Glyph_geometry();

  // Triangulation.
  typedef Inexact_kernel                                                Kernel;
  typedef CGAL::Triangulation_vertex_base_with_info_2<Uint, Kernel>     VB;
  typedef CGAL::Triangulation_face_base_with_info_2<Face_nesting_level, Kernel>
                                                                        FBI;
  typedef CGAL::Constrained_triangulation_face_base_2<Kernel, FBI>      FB;
  typedef CGAL::Triangulation_data_structure_2<VB, FB>                  TDS;
  typedef CGAL::No_intersection_tag                                     Itag;
  // typedef CGAL::Exact_predicates_tag                                    Itag;
  typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>
    Triangulation;

  void set_triangulation(const Triangulation& triangulation)
  { m_triangulation = triangulation; }

  const Triangulation& get_triangulation() const { return m_triangulation; }

  Triangulation& get_triangulation() { return m_triangulation; }

  template <typename InputIterator>
  Uint construct_triangulation(InputIterator begin, InputIterator end, Uint k)
  { return SGAL::construct_triangulation(m_triangulation, begin, end, k); }

  void mark_domains() { SGAL::mark_domains(m_triangulation); }

  void set_scale(Float scale) { m_scale = scale; }

  Float get_scale() const { return m_scale; }

  void set_advance(const Vector2f& advance) { m_advance = advance; }

  void set_advance(Float x, Float y) { m_advance.set(x, y); }

  const Vector2f& get_advance() const { return m_advance; }

private:
  Triangulation m_triangulation;

  Float m_scale;

  Vector2f m_advance;
};

class SGAL_SGAL_DECL Line_geometry {
public:
  /*! Construct default. */
  Line_geometry();

  std::list<const Glyph_geometry*>::iterator begin()
  { return m_glyph_geometries.begin(); }

  std::list<const Glyph_geometry*>::iterator end()
  { return m_glyph_geometries.end(); }

  std::list<const Glyph_geometry*>::const_iterator begin() const
  { return m_glyph_geometries.begin(); }

  std::list<const Glyph_geometry*>::const_iterator end() const
  { return m_glyph_geometries.end(); }

  void push_back(const Glyph_geometry* glyph_geometry)
  { m_glyph_geometries.push_back(glyph_geometry); }

  void set_line_position(const Vector2f& position)
  {  m_line_position = position; }

  const Vector2f& get_line_position() const { return m_line_position; }

  void add_bbox(const Bounding_box& bbox) { m_bbox += bbox; }

  const Bounding_box& get_bbox() const { return m_bbox; }

  Float xmin() const;
  Float ymin() const;
  Float xmax() const;
  Float ymax() const;

  void set_scale(Float scale) { m_scale = scale; }

  Float get_scale() const { return m_scale; }

private:
  std::list<const Glyph_geometry*> m_glyph_geometries;

  Vector2f m_line_position;

  Bounding_box m_bbox;

  Float m_scale;
};

class SGAL_SGAL_DECL Text_geometry {
public:
  /*! Constructor. */
  void resize(size_t num_line_geometries)
  { m_line_geometries.resize(num_line_geometries); }

  std::vector<Line_geometry>::iterator begin()
  { return m_line_geometries.begin(); }

  std::vector<Line_geometry>::iterator end()
  { return m_line_geometries.end(); }

  std::vector<Line_geometry>::const_iterator begin() const
  { return m_line_geometries.begin(); }

  std::vector<Line_geometry>::const_iterator end() const
  { return m_line_geometries.end(); }

  std::vector<Line_geometry> m_line_geometries;

  void add_bbox(const Bounding_box& bbox) { m_bbox += bbox; }

  const Bounding_box& get_bbox() const { return m_bbox; }

  Float xmin() const;
  Float ymin() const;
  Float xmax() const;
  Float ymax() const;

  void set_scale(Float scale) { m_scale = scale; }

  Float get_scale() const { return m_scale; }

private:
  Bounding_box m_bbox;

  Float m_scale;
};

//! \brief construct default.
inline Glyph_geometry::Glyph_geometry() : m_scale(1.0f) {}

//! \brief construct default.
inline Line_geometry::Line_geometry() : m_scale(1.0f) {}

inline Float Line_geometry::xmin() const { return m_bbox.xmin(); }
inline Float Line_geometry::ymin() const { return m_bbox.ymin(); }
inline Float Line_geometry::xmax() const { return m_bbox.xmax(); }
inline Float Line_geometry::ymax() const { return m_bbox.ymax(); }

inline Float Text_geometry::xmin() const { return m_bbox.xmin(); }
inline Float Text_geometry::ymin() const { return m_bbox.ymin(); }
inline Float Text_geometry::xmax() const { return m_bbox.xmax(); }
inline Float Text_geometry::ymax() const { return m_bbox.ymax(); }

SGAL_END_NAMESPACE

#endif
