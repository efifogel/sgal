// Copyright (c) 2004 Israel.
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
// $Id: Lower_envelope_sphere.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOWER_ENVELOPE_SPHERE_HPP
#define SGAL_LOWER_ENVELOPE_SPHERE_HPP

/*! \file
 */

#include <CGAL/basic.h>

#include <list>

#include <CGAL/Envelope_3/Envelope_pm_dcel.h>

/*! Extend the envelope-diagram vertex */
template <typename Point_2, typename Data>
class Rendered_envelope_diagram_vertex :
  public CGAL::Envelope_3::Envelope_pm_vertex<Point_2, Data>
{
private:
  typedef Rendered_envelope_diagram_vertex<Point_2, Data>   Self;
  typedef CGAL::Envelope_3::Envelope_pm_vertex<Point_2, Data>
    Base;
};

typedef std::pair<double, double>                           Double_pair;
typedef std::list<Double_pair>                              Double_point_list;
typedef Double_point_list::iterator                         Double_point_iter;
typedef Double_point_list::const_iterator
  Double_point_const_iter;
typedef Double_point_list::reverse_iterator                 Double_point_riter;
typedef Double_point_list::const_reverse_iterator
  Double_point_const_riter;

/*! Extend the planar-map halfedge */
template <typename X_monotone_curve_2, typename Data>
class Rendered_envelope_diagram_halfedge :
  public CGAL::Envelope_3::Envelope_pm_halfedge<X_monotone_curve_2, Data>
{
private:
  typedef Rendered_envelope_diagram_halfedge<X_monotone_curve_2, Data>  Self;
  typedef CGAL::Envelope_3::Envelope_pm_halfedge<X_monotone_curve_2, Data>
    Base;

  /*! A sequence of points that approximate the x-monotone curve */
  Double_point_list * m_points;
  
public:
  /*! Constructor */
  Rendered_envelope_diagram_halfedge() : m_points(NULL) {}

  /*! Destructor */
  virtual ~Rendered_envelope_diagram_halfedge() { clear_points(); }

  /*! Clear the sequence of points that approximate the x-monotone curve */
  void clear_points()
  {
    if (!m_points) return;
    m_points->clear();
    delete m_points;
    m_points = NULL;
  }
  
  /*! Assign from another halfedge.
   * \param h the other halfedge.
   */
  virtual void assign(const Self & h)
  {
    Base::assign(h);
    set_points(h.m_points);
  }

  /*! Set the sequence of points that approximate the x-monotone curve */
  void set_points(Double_point_list * points)
  {
    clear_points();
    m_points = points;
  }

  /*! Obtain the sequence of points that approximate the x-monotone curve */
  Double_point_list * get_points() { return m_points; }

  /*! Obtain the sequence of points that approximate the x-monotone curve */
  const Double_point_list * get_points() const { return m_points; }
};

/*! Extend the planar-map face */
template <typename Data>
class Rendered_envelope_diagram_face :
  public CGAL::Envelope_3::Envelope_pm_face<Data>
{
private:
  typedef Rendered_envelope_diagram_face<Data>          Self;
  typedef CGAL::Envelope_3::Envelope_pm_face<Data>      Base;
};

/*! A new dcel builder with full Envelope features */
template <typename Traits, typename Data>
class Rendered_Envelope_diagram_dcel :
  public CGAL::Arr_dcel_base<Rendered_envelope_diagram_vertex
                               <typename Traits::Point_2, Data>,
                             Rendered_envelope_diagram_halfedge
                               <typename Traits::X_monotone_curve_2, Data>,
                             Rendered_envelope_diagram_face<Data> >
{
public:
  typedef Data                                    Face_data;
  typedef typename CGAL::Envelope_3::Envelope_pm_face<Data>::Data_iterator
                                                  Face_data_iterator;
  typedef typename CGAL::Envelope_3::Envelope_pm_face<Data>::Data_const_iterator
                                                  Face_data_const_iterator;

  typedef Data                                    Edge_data;
  typedef Face_data_iterator                      Edge_data_iterator;
  typedef Face_data_const_iterator                Edge_data_const_iterator;

  typedef Data                                    Vertex_data;
  typedef Face_data_iterator                      Vertex_data_iterator;
  typedef Face_data_const_iterator                Vertex_data_const_iterator;

  typedef CGAL::Envelope_3::Dcel_info<Data>       Dcel_elem_with_data;

  typedef Data                                    Dcel_data;
  typedef Face_data_iterator                      Dcel_data_iterator;
  typedef Face_data_const_iterator                Dcel_data_const_iterator;
};

#endif
