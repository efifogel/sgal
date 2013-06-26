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
// $Id: Arrangement_color_dcel.hpp 5825 2008-02-06 10:25:09Z efif $
// $Revision: 5825 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_ARRANGEMENT_COLOR_DCEL_HPP
#define SCGAL_ARRANGEMENT_COLOR_DCEL_HPP

/*! \file
 * An arrangement DCEL, the vertex, halfedge, and face of which are extended
 * each with an (RGB) color. 
 */

#include <CGAL/basic.h>
#include <CGAL/Arr_dcel_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <typename Point_2>
class Arrangement_color_vertex :
  public CGAL::Arr_vertex_base<Point_2>
{
private:
  /*! An arbitrary color */
  Vector3f m_color;

public:
  /*! Constructor */
  Arrangement_color_vertex() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }

  /*! Assign from another vertex.
   * \param v the other vertex
   */
  virtual void assign(const Arrangement_color_vertex& v)
  {
    CGAL::Arr_vertex_base<Point_2>::assign(v);
    m_color = v.m_color;
  }
};

/*! Extend the arrangement halfedge */
template <typename X_monotone_curve_2>
class Arrangement_color_halfedge :
  public CGAL::Arr_halfedge_base<X_monotone_curve_2> {
private:
  /*! An arbitrary color */
  Vector3f m_color;

public:
  /*! Constructor */
  Arrangement_color_halfedge() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }

  /*! Assign from another halfedge.
   * \param he the other halfedge
   */
  virtual void assign(const Arrangement_color_halfedge& he)
  {
    CGAL::Arr_halfedge_base<X_monotone_curve_2>::assign(he);
    m_color = he.m_color;
  }
};
  
/*! Extend the arrangement face */
class Arrangement_color_face : public CGAL::Arr_face_base {
private:
  /*! An arbitrary color */
  Vector3f m_color;
  
public:
  /*! Constructor */
  Arrangement_color_face() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }

  /*! Assign from another face.
   * \param f the other face
   */
  virtual void assign(const Arrangement_color_face& f)
  {
    CGAL::Arr_face_base::assign(f);
    m_color = f.m_color;
  }
};

/*! A new dcel builder with extended features */
template <typename Traits>
class Arrangement_color_dcel :
  public CGAL::Arr_dcel_base<Arrangement_color_vertex<typename Traits::
                                                      Point_2>,
                             Arrangement_color_halfedge<typename Traits::
                                                        X_monotone_curve_2>,
                             Arrangement_color_face>
{
public:
  Arrangement_color_dcel() {}
};

SGAL_END_NAMESPACE

#endif
