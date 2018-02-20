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
// $Id: merge_coplanar_facets.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_MERGE_COPLANAR_FACETS_HPP
#define SCGAL_MERGE_COPLANAR_FACETS_HPP

/*! \file
 * This file contains a few function that accepts a polyhedron. It merges
 * cooplanar facets.
 */

#include <list>
#include <boost/type_traits.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Merge coplanar facets */
template <typename Polyhedron, typename Equal>
void merge_coplanar_facets(Polyhedron& polyhedron, Equal& eq)
{
  typedef typename Polyhedron::Halfedge_handle              Halfedge_handle;
  typedef std::list<Halfedge_handle>                        Edge_container;

  // Insert all edges incident to coplanar faces to the coplanar edge container
  Edge_container edges;
  for (auto eit = polyhedron.edges_begin(); eit != polyhedron.edges_end();
       ++eit)
  {
    auto ohe = eit->opposite();
    if (eq(eit->facet()->plane(), ohe->facet()->plane())) edges.push_back(eit);
  }

  // Traverse cooplanar edges and remove them.
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    auto he = *it;
    auto ohe = he->opposite();
    if ((he->vertex()->degree() > 2) && (ohe->vertex()->degree() > 2))
      polyhedron.join_facet(he);
    else if (he->vertex()->degree() == 2) polyhedron.join_vertex(ohe);
    else polyhedron.join_vertex(he);
  }

  // Traverse all vertices and remove vertices of degree 2.
  for (auto vit = polyhedron.vertices_begin(); vit != polyhedron.vertices_end();
       ++vit)
  {
    if (vit->degree() == 2) {
      auto he = vit->vertex_begin();
      auto ohe = he->opposite();
      polyhedron.join_vertex(ohe);
    }
  }
}

template <typename Kernel>
struct Direction_equal {
  Kernel& m_kernel;
  Direction_equal(Kernel& kernel) : m_kernel(kernel) {}
  template <typename Vector>
  bool operator()(Vector& v1, Vector& v2)
  {
    typename Kernel::Equal_3 eq = m_kernel.equal_3_object();
    return eq(v1.direction(), v2.direction());
  }
};

template <typename Kernel, typename Polyhedron>
void merge_coplanar_facets(Kernel& kernel, Polyhedron& polyhedron,
                           boost::false_type)
{
  Direction_equal<Kernel> eq(kernel);
  merge_coplanar_facets(polyhedron, eq);
}

template <typename Kernel, typename Polyhedron>
void merge_coplanar_facets(Kernel& kernel, Polyhedron& polyhedron,
                           boost::true_type)
{
  typename Kernel::Equal_3 eq = kernel.equal_3_object();
  merge_coplanar_facets(polyhedron, eq);
}

SGAL_END_NAMESPACE

#endif
