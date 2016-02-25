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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_SPHERICAL_GAUSSIAN_MAP_HPP
#define SCGAL_SPHERICAL_GAUSSIAN_MAP_HPP

/*! \file
 */

#include <CGAL/Cartesian.h>
// #include <CGAL/IO/Arr_iostream.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_traits.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_arr_dcel.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_polyhedron_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epec_kernel.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <typename Point_2>
class Sgm_arr_vertex : public CGAL::Arr_polyhedral_sgm_arr_vertex<Point_2> {
private:
  /*! The (approximate) normal of an aos face */
  Vector3f m_rendered_normal;

public:
  /*! Constructor */
  Sgm_arr_vertex() {}

  /*! Obtain the normal of the aos face */
  const Vector3f& get_rendered_normal() const { return m_rendered_normal; }

  /*! Set the normal of the aos face */
  void set_rendered_normal(const Vector3f& normal)
  { m_rendered_normal = normal; }
};

/*! Extend the arrangement halfedge */
template <class X_monotone_curve_2>
class Sgm_arr_halfedge :
  public CGAL::Arr_polyhedral_sgm_arr_halfedge<X_monotone_curve_2> {
public:
  /*! Constructor */
  Sgm_arr_halfedge() {}
};

/*! Extend the arrangement face */
class Sgm_arr_face : public CGAL::Arr_polyhedral_sgm_arr_face<Epec_point_3> {
private:
  Vector3f m_rendered_point;

public:
  /*! Constructor */
  Sgm_arr_face() {}

  /*! Obtain the rendered point */
  const Vector3f& get_rendered_point() const { return m_rendered_point; }

  /*! Set the rendered point */
  void set_rendered_point(const Vector3f& point) { m_rendered_point = point; }
};

/*! A new dcel builder with extended features */
template <typename Traits>
class Sgm_arr_dcel :
  public CGAL::Arr_dcel_base<Sgm_arr_vertex<typename Traits::Point_2>,
                             Sgm_arr_halfedge<typename Traits::X_monotone_curve_2>,
                             Sgm_arr_face>
{
public:
  Sgm_arr_dcel() {}
};

typedef CGAL::Arr_polyhedral_sgm_traits<Epec_kernel>   Sgm_geometry_traits_2;
typedef CGAL::Arr_polyhedral_sgm<Sgm_geometry_traits_2, Sgm_arr_dcel>
                                                        Spherical_gaussian_map;

typedef Sgm_geometry_traits_2::Point_2                  Sgm_point_2;
typedef Sgm_geometry_traits_2::Curve_2                  Sgm_curve_2;
typedef Sgm_geometry_traits_2::X_monotone_curve_2       Sgm_x_monotone_curve_2;

typedef Spherical_gaussian_map::Vertex                  Sgm_vertex;
typedef Spherical_gaussian_map::Halfedge                Sgm_halfedge;
typedef Spherical_gaussian_map::Face                    Sgm_face;

typedef Spherical_gaussian_map::Vertex_handle           Sgm_vertex_handle;
typedef Spherical_gaussian_map::Halfedge_handle         Sgm_halfedge_handle;
typedef Spherical_gaussian_map::Face_handle             Sgm_face_handle;

typedef Spherical_gaussian_map::Vertex_const_handle     Sgm_vertex_const_handle;
typedef Spherical_gaussian_map::Halfedge_const_handle
  Sgm_halfedge_const_handle;
typedef Spherical_gaussian_map::Face_const_handle       Sgm_face_const_handle;

typedef Spherical_gaussian_map::Vertex_iterator         Sgm_vertex_iterator;
typedef Spherical_gaussian_map::Halfedge_iterator       Sgm_halfedge_iterator;
typedef Spherical_gaussian_map::Edge_iterator           Sgm_edge_iterator;
typedef Spherical_gaussian_map::Face_iterator           Sgm_face_iterator;

typedef Spherical_gaussian_map::Vertex_const_iterator
  Sgm_vertex_const_iterator;
typedef Spherical_gaussian_map::Edge_const_iterator     Sgm_edge_const_iterator;
typedef Spherical_gaussian_map::Halfedge_const_iterator
  Sgm_halfedge_const_iterator;
typedef Spherical_gaussian_map::Face_const_iterator     Sgm_face_const_iterator;

typedef Spherical_gaussian_map::Halfedge_around_vertex_circulator
  Sgm_halfedge_around_vertex_circulator;
typedef Spherical_gaussian_map::Halfedge_around_vertex_const_circulator
  Sgm_halfedge_around_vertex_const_circulator;

typedef CGAL::Arr_polyhedral_sgm_polyhedron_3<Spherical_gaussian_map,
                                              Epec_kernel>
  Sgm_polyhedron;
typedef CGAL::Arr_polyhedral_sgm_initializer<Spherical_gaussian_map,
                                             Sgm_polyhedron>
  Sgm_initializer;

SGAL_END_NAMESPACE

#endif
