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

  /*! Set the triangulation of the glyph geometry.
   * \param[in] triangulation the new triangulation.
   */
  void set_triangulation(const Triangulation& triangulation);

  /*! Obtain the (const) triangulation of the glyph geometry.
   * \param[in] triangulation the new triangulation.
   */
  const Triangulation& get_triangulation() const;

  /*! Obtain the (non-const) triangulation of the glyph geometry.
   * \return the triangulation.
   */
  Triangulation& get_triangulation();

  template <typename InputIterator>
  Uint construct_triangulation(InputIterator begin, InputIterator end, Uint k);

  void mark_domains();

  /*! Set the scale factor of the glyph geometry.
   * \param[in] scale the new scale factor.
   */
  void set_scale(Float scale);

  /*! Obtain the scale factor of the glyph geometry.
   * \return the new scale factor.
   */
  Float get_scale() const;

  /*! Set the 2D vector used to advance the glyph geometry.
   * \param[in] advance the new 2D used to advance the glyph geometry.
   */
  void set_advance(const Vector2f& advance);

  /*! Set the x- and y-coordinates used to advance the glyph geometry.
   * \param[in] x the new x-coordinate used to advance the glyph geometry
   *              along the x-axis.
   * \param[in] y the new y-coordinate used to advance the glyph geometry
   *              along the y-axis.
   */
  void set_advance(Float x, Float y);

  /*! Obtain the 2D vector used to advance the glyph geometry.
   * \return the 2D vector used to advance the glyph geometry.
   */
  const Vector2f& get_advance() const;

private:
  /*! The triangulation of the glyph geometry. */
  Triangulation m_triangulation;

  /*! The scale factor of the glyph geometry. */
  Float m_scale;

  /*! The 2D vector used to advance the glyph geometry. */
  Vector2f m_advance;
};

class SGAL_SGAL_DECL Line_geometry {
public:
  /*! Construct default. */
  Line_geometry();

  /*! Obtain the begin (non-const) iterator of the glyph geometries.
   * \return the begin (non-const) iterator of the glyph geometries.
   */
  std::list<const Glyph_geometry*>::iterator begin();

  /*! Obtain the pass-the-end (non-const) iterator of the glyph geometries.
   * \return the pass-the-end (non-const) iterator of the glyph geometries.
   */
  std::list<const Glyph_geometry*>::iterator end();

  /*! Obtain the begin (const) iterator of the glyph geometries.
   * \return the begin (const) iterator of the glyph geometries.
   */
  std::list<const Glyph_geometry*>::const_iterator begin() const;

  /*! Obtain the pass-the-end (const) iterator of the glyph geometries.
   * \return the pass-the-end (const) iterator of the glyph geometries.
   */
  std::list<const Glyph_geometry*>::const_iterator end() const;

  /*! Push a glyph geometry to the glyph geometries at the back.
   * \param[in] glyph_geometry the glyph geometry to push.
   */
  void push_back(const Glyph_geometry* glyph_geometry);

  /*! Set the scale factor of the line geometry.
   * \param[in] scale the new scale factor.
   */
  void set_scale(Float scale);

  /*! Obtain the scale factor of the line geometry.
   * \return the new scale factor.
   */
  Float get_scale() const;

  /*! Set the width of the line geometry.
   * \param[in] width the new width.
   */
  void set_width(Float width);

  /*! Obtain the width of the line geometry.
   * \return the current width.
   */
  Float get_width() const;

  /*! Set height of the line geometry.
   * \param[i] height the new height.
   */
  void set_height(Float height);

  /*! Obtain the height of the line geometry.
   * \return the current height.
   */
  Float get_height() const;

private:
  std::list<const Glyph_geometry*> m_glyph_geometries;

  /*! The scale factor if the line geometry. */
  Float m_scale;

  /*! The width of the line geometry. */
  Float m_width;

  /*! The height of the line geometry. */
  Float m_height;
};

class SGAL_SGAL_DECL Text_geometry {
public:
  /*! Construct default. */
  Text_geometry();

  /*! Resize the line geometry container.
   * \param[in] the new size.
   */
  void resize(size_t size) { m_line_geometries.resize(size); }

  /*! Obtain the number of line geometries.
   * \return the number of line geometries.
   */
  size_t size() const { return m_line_geometries.size(); }

  std::vector<Line_geometry>::iterator begin()
  { return m_line_geometries.begin(); }

  std::vector<Line_geometry>::iterator end()
  { return m_line_geometries.end(); }

  std::vector<Line_geometry>::const_iterator begin() const
  { return m_line_geometries.begin(); }

  std::vector<Line_geometry>::const_iterator end() const
  { return m_line_geometries.end(); }

  std::vector<Line_geometry> m_line_geometries;

  /*! Set the scale factor of the text geometry.
   * \param[in] scale the new scale factor.
   */
  void set_scale(Float scale);

  /*! Obtain the scale factor of the text geometry.
   * \return the new scale factor.
   */
  Float get_scale() const;

  /*! Set the width of the text geometry.
   * \param[in] width the new width.
   */
  void set_width(Float width);

  /*! Obtain the width of the text geometry.
   * \return the current width.
   */
  Float get_width() const;

  /*! Set height of the text geometry.
   * \param[i] height the new height.
   */
  void set_height(Float height);

  /*! Obtain the height of the text geometry.
   * \return the current height.
   */
  Float get_height() const;

private:
  /*! The scale factor if the text geometry. */
  Float m_scale;

  /*! The width of the text geometry. */
  Float m_width;

  /*! The height of the text geometry. */
  Float m_height;
};

//! \brief construct default.
inline Glyph_geometry::Glyph_geometry() : m_scale(1.0f) {}

//! \brief sets the triangulation of the glyph geometry.
inline void
Glyph_geometry::set_triangulation(const Triangulation& triangulation)
{ m_triangulation = triangulation; }

//! \brief obtains the (const) triangulation of the glyph geometry.
inline const Glyph_geometry::Triangulation&
Glyph_geometry::get_triangulation() const
{ return m_triangulation; }

//! \brief obtains the (non-const) triangulation of the glyph geometry.
inline Glyph_geometry::Triangulation& Glyph_geometry::get_triangulation()
{ return m_triangulation; }

template <typename InputIterator>
inline Uint Glyph_geometry::construct_triangulation(InputIterator begin,
                                                    InputIterator end,
                                                    Uint k)
{ return SGAL::construct_triangulation(m_triangulation, begin, end, k); }

inline void Glyph_geometry::mark_domains()
{ SGAL::mark_domains(m_triangulation); }

//! \brief sets the scale factor of the glyph geometry.
inline void Glyph_geometry::set_scale(Float scale) { m_scale = scale; }

//! \brief obtains the scale factor of the glyph geometry.
inline Float Glyph_geometry::get_scale() const { return m_scale; }

//! \brief sets the 2D vector used to advance the glyph geometry.
inline void Glyph_geometry::set_advance(const Vector2f& advance)
{ m_advance = advance; }

//! \brief sets the x- and y-coordinates used to advance the glyph geometry.
inline void Glyph_geometry::set_advance(Float x, Float y)
{ m_advance.set(x, y); }

//! \brief obtains the 2D vector used to advance the glyph geometry.
inline const Vector2f& Glyph_geometry::get_advance() const { return m_advance; }

//! \brief construct default.
inline Line_geometry::Line_geometry() :
  m_scale(1.0f),
  m_width(0.0f),
  m_height(0.0f)
{}

//! \brief obtains the begin (non-const) iterator of the glyph geometries.
inline std::list<const Glyph_geometry*>::iterator Line_geometry::begin()
{ return m_glyph_geometries.begin(); }

//! \brief obtains the pass-the-end (non-const) iterator of the glyph geometries.
inline std::list<const Glyph_geometry*>::iterator Line_geometry::end()
{ return m_glyph_geometries.end(); }

//! \brief obtains the begin (const) iterator of the glyph geometries.
inline std::list<const Glyph_geometry*>::const_iterator Line_geometry::begin()
  const
{ return m_glyph_geometries.begin(); }

//! \brief obtains the pass-the-end (const) iterator of the glyph geometries.
inline std::list<const Glyph_geometry*>::const_iterator Line_geometry::end()
  const
{ return m_glyph_geometries.end(); }

//! \brief pushes a glyph geometry to the glyph geometries at the back.
inline void Line_geometry::push_back(const Glyph_geometry* glyph_geometry)
{ m_glyph_geometries.push_back(glyph_geometry); }

//! Set the width of the line geometry.
inline void Line_geometry::set_width(Float width) { m_width = width; }

//! Obtain the width of the line geometry.
inline Float Line_geometry::get_width() const { return m_width; }

//! Set height of the line geometry.
inline void Line_geometry::set_height(Float height) { m_height = height; }

//! Obtain the height of the line in the local coordinate system.
inline Float Line_geometry::get_height() const { return m_height; }

//! \brief sets the scale factor of the line geometry.
inline void Line_geometry::set_scale(Float scale) { m_scale = scale; }

//! Obtain the scale factor of the line geometry.
inline Float Line_geometry::get_scale() const { return m_scale; }

//! \brief construct default.
inline Text_geometry::Text_geometry() :
  m_scale(1.0f),
  m_width(0.0f),
  m_height(0.0f)
{}

//! Set the width of the text geometry.
inline void Text_geometry::set_width(Float width) { m_width = width; }

//! Obtain the width of the text geometry.
inline Float Text_geometry::get_width() const { return m_width; }

//! Set height of the text geometry.
inline void Text_geometry::set_height(Float height) { m_height = height; }

//! Obtain the height of the text in the local coordinate system.
inline Float Text_geometry::get_height() const { return m_height; }

//! \brief sets the scale factor of the text geometry.
inline void Text_geometry::set_scale(Float scale) { m_scale = scale; }

//! Obtain the scale factor of the text geometry.
inline Float Text_geometry::get_scale() const { return m_scale; }

SGAL_END_NAMESPACE

#endif
