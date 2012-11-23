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
// $Id: Spherical_gaussian_map_colored.hpp 13463 2012-06-04 15:22:36Z efif $
// $Revision: 13463 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SPHERICAL_GAUSSIAN_MAP_COLORED_HPP
#define SGAL_SPHERICAL_GAUSSIAN_MAP_COLORED_HPP

/*! \file
 */

#include <CGAL/Cartesian.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_traits.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>

#include "SGAL/basic.hpp"

#include "SCGAL/Spherical_gaussian_map.hpp"
#include "SCGAL/Exact_number_type.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <class Point_2>
class Sgm_colored_arr_vertex : public Sgm_arr_vertex<Point_2> {
private:
  /*! The color of the (aos) vertex) */
  Vector3f m_color;

public:
  /*! Constructor */
  Sgm_colored_arr_vertex() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }
};

/*! Extend the arrangement halfedge */
template <class X_monotone_curve_2>
class Sgm_colored_arr_halfedge : public Sgm_arr_halfedge<X_monotone_curve_2> {
private:
  /*! The color of the (aos) halfedge */
  Vector3f m_color;

public:
  /*! Constructor */
  Sgm_colored_arr_halfedge() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }
};
  
/*! Extend the arrangement face */
class Sgm_colored_arr_face : public Sgm_arr_face {
private:
  /*! Indicates whether it is a colored (aos) face */
  Vector3f m_color;

public:
  /*! Constructor */
  Sgm_colored_arr_face() {}

  /*! Obtain the color */
  const Vector3f& color() const { return m_color; }

  /*! Set the color */
  void set_color(const Vector3f& color) { m_color = color; }
};

/*! A new dcel builder with extended features */
template <class Traits>
class Sgm_colored_arr_dcel :
  public CGAL::Arr_dcel_base<Sgm_colored_arr_vertex<typename Traits::Point_2>,
                             Sgm_colored_arr_halfedge<typename Traits::X_monotone_curve_2>,
                             Sgm_colored_arr_face>
{
public:
  Sgm_colored_arr_dcel() {}
};

typedef CGAL::Arr_polyhedral_sgm_traits<Exact_kernel>
  Sgm_colored_geometry_traits_2;
typedef CGAL::Arr_polyhedral_sgm<Sgm_colored_geometry_traits_2,
                                 Sgm_colored_arr_dcel>
  Spherical_gaussian_map_colored;

typedef Sgm_colored_geometry_traits_2::Point_2          Sgm_colored_point_2;
typedef Sgm_colored_geometry_traits_2::Curve_2          Sgm_colored_curve_2;
typedef Sgm_colored_geometry_traits_2::X_monotone_curve_2
  Sgm_colored_x_monotone_curve_2;

typedef Spherical_gaussian_map_colored::Vertex          Sgm_colored_vertex;
typedef Spherical_gaussian_map_colored::Halfedge        Sgm_colored_halfedge;
typedef Spherical_gaussian_map_colored::Face            Sgm_colored_face;

typedef Spherical_gaussian_map_colored::Vertex_handle   Sgm_colored_vertex_handle;
typedef Spherical_gaussian_map_colored::Halfedge_handle
  Sgm_colored_halfedge_handle;
typedef Spherical_gaussian_map_colored::Face_handle     Sgm_colored_face_handle;

typedef Spherical_gaussian_map_colored::Vertex_const_handle
  Sgm_colored_vertex_const_handle;
typedef Spherical_gaussian_map_colored::Halfedge_const_handle
  Sgm_colored_halfedge_const_handle;
typedef Spherical_gaussian_map_colored::Face_const_handle
  Sgm_colored_face_const_handle;
  
typedef Spherical_gaussian_map_colored::Vertex_iterator
  Sgm_colored_vertex_iterator;
typedef Spherical_gaussian_map_colored::Halfedge_iterator
  Sgm_colored_halfedge_iterator;
typedef Spherical_gaussian_map_colored::Edge_iterator   Sgm_colored_edge_iterator;
typedef Spherical_gaussian_map_colored::Face_iterator   Sgm_colored_face_iterator;

typedef Spherical_gaussian_map_colored::Vertex_const_iterator
  Sgm_colored_vertex_const_iterator;
typedef Spherical_gaussian_map_colored::Edge_const_iterator
  Sgm_colored_edge_const_iterator;
typedef Spherical_gaussian_map_colored::Halfedge_const_iterator
  Sgm_colored_halfedge_const_iterator;
typedef Spherical_gaussian_map_colored::Face_const_iterator
  Sgm_colored_face_const_iterator;

typedef Spherical_gaussian_map_colored::Halfedge_around_vertex_circulator
  Sgm_colored_halfedge_around_vertex_circulator;
typedef Spherical_gaussian_map_colored::Halfedge_around_vertex_const_circulator
  Sgm_colored_halfedge_around_vertex_const_circulator;

typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map_colored,
                                              Exact_kernel>
  Sgm_colored_polyhedron;
typedef CGAL::Arr_polyhedral_sgm_initializer<Spherical_gaussian_map_colored,
                                             Sgm_colored_polyhedron>
  Sgm_colored_initializer;

SGAL_END_NAMESPACE

#endif
