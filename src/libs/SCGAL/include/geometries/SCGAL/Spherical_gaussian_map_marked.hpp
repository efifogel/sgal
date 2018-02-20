// Copyright (c) 2008 Israel.
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
// $Id: Spherical_gaussian_map_marked.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_SPHERICAL_GAUSSIAN_MAP_MARKED_HPP
#define SCGAL_SPHERICAL_GAUSSIAN_MAP_MARKED_HPP

/*! \file
 */

#include <CGAL/Cartesian.h>
// #include <CGAL/IO/Arr_iostream.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_traits.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epec_kernel.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Spherical_gaussian_map.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <typename Point_2>
class Sgm_marked_arr_vertex : public Sgm_arr_vertex<Point_2> {
private:
  /*! Indicates whether it is a marked polyheron vertex (aos face) */
  Boolean m_marked;

public:
  /*! Constructor */
  Sgm_marked_arr_vertex() : m_marked(false) {}

  /*! Obtain the "marked" flag */
  Boolean marked() const { return m_marked; }

  /*! Set the "marked" flag */
  void set_marked(Boolean marked) { m_marked = marked; }
};

/*! Extend the arrangement halfedge */
template <typename X_monotone_curve_2>
class Sgm_marked_arr_halfedge : public Sgm_arr_halfedge<X_monotone_curve_2> {
private:
  /*! Indicates whether it is a marked (aos) halfedge */
  Boolean m_marked;

public:
  /*! Constructor */
  Sgm_marked_arr_halfedge() : m_marked(false) {}

  /*! Obtain the "marked" flag */
  Boolean marked() const { return m_marked; }

  /*! Set the "marked" flag */
  void set_marked(Boolean marked) { m_marked = marked; }
};

/*! Extend the arrangement face */
class Sgm_marked_arr_face : public Sgm_arr_face {
  /*! Indicates whether it is a marked (aos) face */
  Boolean m_marked;

public:
  /*! Constructor */
  Sgm_marked_arr_face() : m_marked(false) {}

  Boolean marked() const { return m_marked; }
  void set_marked(Boolean marked) { m_marked = marked; }
};

/*! A new dcel builder with extended features */
template <typename Traits>
class Sgm_marked_arr_dcel :
  public CGAL::Arr_dcel_base<Sgm_marked_arr_vertex<typename Traits::Point_2>,
                             Sgm_marked_arr_halfedge<typename Traits::X_monotone_curve_2>,
                             Sgm_marked_arr_face>
{
public:
  Sgm_marked_arr_dcel() {}
};

typedef CGAL::Arr_polyhedral_sgm_traits<Epec_kernel>
  Sgm_marked_geometry_traits_2;
typedef CGAL::Arr_polyhedral_sgm<Sgm_marked_geometry_traits_2,
                                 Sgm_marked_arr_dcel>
  Spherical_gaussian_map_marked;

typedef Sgm_marked_geometry_traits_2::Point_2          Sgm_marked_point_2;
typedef Sgm_marked_geometry_traits_2::Curve_2          Sgm_marked_curve_2;
typedef Sgm_marked_geometry_traits_2::X_monotone_curve_2
  Sgm_marked_x_monotone_curve_2;

typedef Spherical_gaussian_map_marked::Vertex          Sgm_marked_vertex;
typedef Spherical_gaussian_map_marked::Halfedge        Sgm_marked_halfedge;
typedef Spherical_gaussian_map_marked::Face            Sgm_marked_face;

typedef Spherical_gaussian_map_marked::Vertex_handle   Sgm_marked_vertex_handle;
typedef Spherical_gaussian_map_marked::Halfedge_handle
  Sgm_marked_halfedge_handle;
typedef Spherical_gaussian_map_marked::Face_handle     Sgm_marked_face_handle;

typedef Spherical_gaussian_map_marked::Vertex_const_handle
  Sgm_marked_vertex_const_handle;
typedef Spherical_gaussian_map_marked::Halfedge_const_handle
  Sgm_marked_halfedge_const_handle;
typedef Spherical_gaussian_map_marked::Face_const_handle
  Sgm_marked_face_const_handle;

typedef Spherical_gaussian_map_marked::Vertex_iterator
  Sgm_marked_vertex_iterator;
typedef Spherical_gaussian_map_marked::Halfedge_iterator
  Sgm_marked_halfedge_iterator;
typedef Spherical_gaussian_map_marked::Edge_iterator   Sgm_marked_edge_iterator;
typedef Spherical_gaussian_map_marked::Face_iterator   Sgm_marked_face_iterator;

typedef Spherical_gaussian_map_marked::Vertex_const_iterator
  Sgm_marked_vertex_const_iterator;
typedef Spherical_gaussian_map_marked::Edge_const_iterator
  Sgm_marked_edge_const_iterator;
typedef Spherical_gaussian_map_marked::Halfedge_const_iterator
  Sgm_marked_halfedge_const_iterator;
typedef Spherical_gaussian_map_marked::Face_const_iterator
  Sgm_marked_face_const_iterator;

typedef Spherical_gaussian_map_marked::Halfedge_around_vertex_circulator
  Sgm_marked_halfedge_around_vertex_circulator;
typedef Spherical_gaussian_map_marked::Halfedge_around_vertex_const_circulator
  Sgm_marked_halfedge_around_vertex_const_circulator;

typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map_marked,
                                              Epec_kernel>
  Sgm_marked_polyhedron;
typedef CGAL::Arr_polyhedral_sgm_initializer<Spherical_gaussian_map_marked,
                                             Sgm_marked_polyhedron>
  Sgm_marked_initializer;

SGAL_END_NAMESPACE

#endif
