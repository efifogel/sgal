// Copyright (c) 2014 Israel.
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

/*! \file
 * Defines the type Exact_polyhedron, which is a polyhedral surface
 * representation of a polyhedron. It uses CGAL and as such it uses a
 * halfedge data structure to represent the incidence relations between the
 * cells, namely, vertices, halfedges, and facets. It uses an exact Cartesian
 * kernel. Each facet contains the equiation of the underlying plane.
 */

#ifndef SCGAL_EXACT_POLYHEDRON_HPP
#define SCGAL_EXACT_POLYHEDRON_HPP

#include <CGAL/basic.h>
#include <CGAL/Polyhedron_3.h>

#include "SGAL/basic.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

/*! An extended vertex */
template <typename Refs, typename Traits>
struct My_vertex :
  public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true,
                                      typename Traits::Point_3>
{
  typedef typename Traits::Point_3    Point;
  Vector3f m_vertex;
  My_vertex() {}
  My_vertex(const Point& p) :
    CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point>(p)
  {}
};

/*! An extended halfedge */
template <typename Refs>
struct My_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
  bool m_flag;
  My_halfedge() : m_flag(false) {}
};

/*! A face type with the face normal data member */
template <typename Refs>
struct My_face :
  public CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Exact_plane_3>
{
  Vector3f m_normal;
  My_face() {}
  My_face(const Exact_plane_3& pln) :
    CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Exact_plane_3>(pln)
  {}
};

/*! An items type using extended features */
struct Exact_polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Vertex_wrapper {
    typedef My_vertex<Refs, Traits> Vertex;
  };

  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef My_halfedge<Refs> Halfedge;
  };

  template <typename Refs, typename Traits>
  struct Face_wrapper {
    typedef My_face<Refs> Face;
  };
};

typedef CGAL::Polyhedron_3<Exact_kernel, Exact_polyhedron_items>
  Exact_polyhedron;

SGAL_END_NAMESPACE

#endif
