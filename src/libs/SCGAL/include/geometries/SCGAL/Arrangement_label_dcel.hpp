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
// $Id: Arrangement_label_dcel.hpp 5969 2008-02-25 12:43:19Z efif $
// $Revision: 5969 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_LABEL_DCEL_HPP
#define SGAL_ARRANGEMENT_LABEL_DCEL_HPP

/*! \file
 * An arrangement DCEL, the vertex, halfedge, and face of which are extended
 * each with an (unsigned int) label. 
 */

#include <CGAL/basic.h>
#include <CGAL/Arr_dcel_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <class Point_2>
class Arrangement_label_vertex : public CGAL::Arr_vertex_base<Point_2> {
private:
  /*! An arbitrary label */
  Uint m_label;

public:
  /*! Constructor */
  Arrangement_label_vertex() : m_label(0) {}

  /*! Obtain the label */
  Uint label() const { return m_label; }

  /*! Set the label */
  void set_label(Uint label) { m_label = label; }

  /*! Assign from another vertex.
   * \param v the other vertex
   */
  virtual void assign(const Arrangement_label_vertex & v)
  {
    CGAL::Arr_vertex_base<Point_2>::assign(v);
    m_label = v.m_label;
  }
};

/*! Extend the arrangement halfedge */
template <class X_monotone_curve_2>
class Arrangement_label_halfedge :
  public CGAL::Arr_halfedge_base<X_monotone_curve_2> {
private:
  /*! An arbitrary label */
  Uint m_label;

public:
  /*! Constructor */
  Arrangement_label_halfedge() : m_label(0) {}

  /*! Obtain the label */
  Uint label() const { return m_label; }

  /*! Set the label */
  void set_label(Uint label) { m_label = label; }

  /*! Assign from another halfedge.
   * \param he the other halfedge
   */
  virtual void assign(const Arrangement_label_halfedge & he)
  {
    CGAL::Arr_halfedge_base<X_monotone_curve_2>::assign(he);
    m_label = he.m_label;
  }
};
  
/*! Extend the arrangement face */
class Arrangement_label_face : public CGAL::Arr_face_base {
private:
  /*! An arbitrary label */
  Uint m_label;
    
public:
  /*! Constructor */
  Arrangement_label_face() : m_label(0) {}

  /*! Obtain the "label" flag */
  Uint label() const { return m_label; }

  /*! Set the label */
  void set_label(Uint label) { m_label = label; }

  /*! Assign from another face.
   * \param f the other face
   */
  virtual void assign(const Arrangement_label_face & f)
  {
    CGAL::Arr_face_base::assign(f);
    m_label = f.m_label;
  }
};

/*! A new dcel builder with extended features */
template <class Traits>
class Arrangement_label_dcel :
  public CGAL::Arr_dcel_base<Arrangement_label_vertex<typename Traits::
                                                      Point_2>,
                             Arrangement_label_halfedge<typename Traits::
                                                        X_monotone_curve_2>,
                             Arrangement_label_face>
{
public:
  Arrangement_label_dcel() {}
};

SGAL_END_NAMESPACE

#endif
