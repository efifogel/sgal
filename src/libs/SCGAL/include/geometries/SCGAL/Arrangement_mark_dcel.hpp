// Copyright (c) 2007 Israel.
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

#ifndef SCGAL_ARRANGEMENT_MARK_DCEL_HPP
#define SCGAL_ARRANGEMENT_MARK_DCEL_HPP

/*! \file
 * An arrangement DCEL, the vertex, halfedge, and face of which are extended
 * each with an (unsigned int) mark.
 */

#include <CGAL/basic.h>
#include <CGAL/Arr_dcel_base.h>
#include <CGAL/Boolean_set_operations_2/Gps_default_dcel.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <typename Point_2>
class Arrangement_mark_vertex : public CGAL::Arr_vertex_base<Point_2> {
private:
  /*! An arbitrary mark */
  Boolean m_mark;

public:
  /*! Constructor */
  Arrangement_mark_vertex() : m_mark(0) {}

  /*! Obtain the mark */
  Boolean mark() const { return m_mark; }

  /*! Set the mark */
  void set_mark(Boolean mark) { m_mark = mark; }

  /*! Assign from another vertex.
   * \param v the other vertex
   */
  virtual void assign(const Arrangement_mark_vertex& v)
  {
    CGAL::Arr_vertex_base<Point_2>::assign(v);
    m_mark = v.m_mark;
  }
};

/*! Extend the arrangement halfedge */
template <typename X_monotone_curve_2>
class Arrangement_mark_halfedge :
  public CGAL::Gps_halfedge_base<X_monotone_curve_2> {
private:
  /*! An arbitrary mark */
  Boolean m_mark;

public:
  /*! Constructor */
  Arrangement_mark_halfedge() : m_mark(0) {}

  /*! Obtain the mark */
  Boolean mark() const { return m_mark; }

  /*! Set the mark */
  void set_mark(Boolean mark) { m_mark = mark; }

  /*! Assign from another halfedge.
   * \param he the other halfedge
   */
  virtual void assign(const Arrangement_mark_halfedge& he)
  {
    CGAL::Arr_halfedge_base<X_monotone_curve_2>::assign(he);
    m_mark = he.m_mark;
  }
};

/*! Extend the arrangement face */
//  We use Gps_face_base as the base of our face so we could use Boolean
//  set operations in the node Polygon_set_on_sphere_geo and still have
//  Polygon_set_on_sphere_geo inherit from the Arrangement_on_sphere_marked_geo.
//  This is a temporary solution that should be replaced with a better solution
//  that will allow decoupling of the DCEL from the Boolean set operations.
//  (like using BOOST property maps).
//  In the meantime, we use the Gps' "contain" flag instead of the "mark" flag.
class Arrangement_mark_face : public CGAL::Gps_face_base {
private:
  /*! An arbitrary mark */
  // Boolean m_mark;

public:
  /*! Constructor */
  Arrangement_mark_face() //: m_mark(0)
  {}

  /*! Obtain the "mark" flag */
  Boolean mark() const {
    // return m_mark;
    return contained() ? true : false;
  }

  /*! Set the mark */
  void set_mark(Boolean mark) {
    // m_mark = mark;
    (mark == true) ? set_contained(true) : set_contained(false);
  }

  /*! Assign from another face.
   * \param f the other face
   */
  virtual void assign(const Arrangement_mark_face& f)
  {
    CGAL::Gps_face_base::operator= (f);
    CGAL::Arr_face_base::assign(f);
    // m_mark = f.m_mark;
  }
};

/*! A new dcel builder with extended features */
template <typename Traits>
class Arrangement_mark_dcel :
  public CGAL::Arr_dcel_base<Arrangement_mark_vertex<typename Traits::
                                                     Point_2>,
                             Arrangement_mark_halfedge<typename Traits::
                                                       X_monotone_curve_2>,
                             Arrangement_mark_face>
{
public:
  Arrangement_mark_dcel() {}
};

SGAL_END_NAMESPACE

#endif
