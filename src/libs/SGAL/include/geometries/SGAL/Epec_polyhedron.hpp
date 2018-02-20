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
 * Defines the type Epec_polyhedron, which is a polyhedral surface
 * representation of a polyhedron. It uses CGAL and as such it uses a
 * halfedge data structure to represent the incidence relations between the
 * cells, namely, vertices, halfedges, and facets. It uses an exact Cartesian
 * kernel. Each facet contains the equiation of the underlying plane.
 */

#ifndef SGAL_EPEC_POLYHEDRON_HPP
#define SGAL_EPEC_POLYHEDRON_HPP

#include <CGAL/basic.h>
#include <CGAL/Polyhedron_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/HalfedgeDS_vertex_with_id.hpp"
#include "SGAL/HalfedgeDS_halfedge_with_id_and_flag.hpp"
#include "SGAL/HalfedgeDS_face_with_id_and_normal.hpp"

SGAL_BEGIN_NAMESPACE

/*! An items type using extended features */
struct Epec_polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Vertex_wrapper {
    typedef typename Traits::Point_3 Point;
    typedef HalfedgeDS_vertex_with_id<Refs, Point, Size> Vertex;
  };

  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef HalfedgeDS_halfedge_with_id_and_flag<Refs, Size> Halfedge;
  };

  template <typename Refs, typename Traits>
  struct Face_wrapper {
    typedef typename Traits::Plane_3 Plane;
    typedef HalfedgeDS_face_with_id_and_normal<Refs, Plane, Size> Face;
  };
};

typedef CGAL::Polyhedron_3<Epec_kernel, Epec_polyhedron_items> Epec_polyhedron;

SGAL_END_NAMESPACE

#endif
