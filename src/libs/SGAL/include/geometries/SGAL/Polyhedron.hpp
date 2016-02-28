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
#include "SGAL/Types.hpp"
#include "SGAL/Inexact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Refs, typename Traits>
struct Polyhedron_vertex :
  public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true,
                                      typename Traits::Point_3>
{
  typedef typename Traits::Point_3    Point;

  /*! A uniqe index of vertices. */
  Uint m_index;

  Polyhedron_vertex() {}
  Polyhedron_vertex(const Point& p) :
    CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point>(p)
  {}
};

template <typename Refs>
class Polyhedron_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
public:
  /*! Default constructor */
  Polyhedron_halfedge() : m_creased(false) {}

  /*! Obtain the index of the index of the incident vertex.
   * \return the index of the incident vertex.
   */
  Uint get_index() const { return m_index; }

  /*! Set the general-purpose flag.
   * \param[in] flag the new Boolean value of the flag.
   */
  void set_flag(Boolean flag) const { m_flag = flag; }

  /*! Obtain the general-purpose flag.
   * \return the Boolean value of the flag.
   */
  Boolean is_flag() const { return m_flag; }

  /*! The index of the index of the incident vertex. */
  Uint m_index;

  Boolean m_creased;

  /*! A general-purpose Boolean flag. */
  mutable Boolean m_flag;
};

// An items type using my vertex and edge.
struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Vertex_wrapper {
    typedef Polyhedron_vertex<Refs, Traits> Vertex;
  };

  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef Polyhedron_halfedge<Refs> Halfedge;
  };
};

typedef CGAL::Polyhedron_traits_with_normals_3<Inexact_kernel>
                                                   Inexact_polyhedron_traits;
typedef CGAL::Polyhedron_3<Inexact_polyhedron_traits, Polyhedron_items>
                                                   Inexact_polyhedron;

SGAL_END_NAMESPACE

#endif
