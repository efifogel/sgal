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
 * Defines the type Polyhedron, which is a polyhedral surface representation
 * of a polyhedron. It uses CGAL and as such it uses a halfedge data
 * structure to represent the incidence relations between the cells, namely,
 * vertices, halfedges, and facets. It uses an inexact Cartesian kernel.
 * Each facet contains the normal to the facet (and not the equiation of the
 * underlying plane). This is convenient, as it can directly be used to
 * render the facet.
 */

#ifndef SGAL_POLYHEDRON_HPP
#define SGAL_POLYHEDRON_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include "SGAL/basic.hpp"

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Refs>
class Polyhedron_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
public:
  /*! Constructor */
  Polyhedron_halfedge() {}

  /*! Obtain the index of the index of the incident vertex.
   * \return the index of the incident vertex.
   */
  Uint get_index() const { return m_index; }

  /*! The index of the index of the incident vertex. */
  Uint m_index;

  Boolean m_creased;
};

// An items type using my vertex and edge.
struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef Polyhedron_halfedge<Refs> Halfedge;
  };
};

typedef CGAL::Cartesian<Float>                     Inexact_kernel;
typedef CGAL::Polyhedron_traits_with_normals_3<Inexact_kernel>
                                                   Inexact_polyhedron_traits;
typedef CGAL::Polyhedron_3<Inexact_polyhedron_traits, Polyhedron_items>
                                                   Inexact_polyhedron;

SGAL_END_NAMESPACE

#endif
