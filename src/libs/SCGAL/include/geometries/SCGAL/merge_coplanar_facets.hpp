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

#ifndef SGAL_MERGE_COPLANAR_FACETS_HPP
#define SGAL_MERGE_COPLANAR_FACETS_HPP

/*! \file
 * This file contains a few function that accepts a polyhedron. It merges
 * cooplanar facets.
 */

#include <vector>
#include <boost/type_traits.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Merge coplanar facets */
template <typename Polyhedron, typename Equal>
void merge_coplanar_facets(Polyhedron & polyhedron, Equal & eq)
{
  typedef typename Polyhedron::Vertex_handle            Vertex_handle;
  typedef typename Polyhedron::Halfedge_handle          Halfedge_handle;
  typedef typename Polyhedron::Halfedge_around_vertex_circulator
    Halfedge_around_vertex_circulator;
  typedef std::vector<Vertex_handle>                    Vertex_container;

  // Copy all vertex handles to a temporary container:
  Vertex_container vertices(polyhedron.size_of_vertices());
  typename Vertex_container::iterator vit = vertices.begin();
  typename Polyhedron::Vertex_iterator it;
  for (it = polyhedron.vertices_begin(); it != polyhedron.vertices_end();
       ++it, ++vit)
    *vit = it;

  // Traverse all vertices:
  for (vit = vertices.begin(); vit != vertices.end(); ++vit) {
    // Traverse all outgoing halfedges:
    Halfedge_around_vertex_circulator start_he = (*vit)->vertex_begin();
    Halfedge_around_vertex_circulator he = start_he;
    Halfedge_around_vertex_circulator next_he;
    bool end_vertex = false;
    for (++he; he != start_he; he = next_he) {
      next_he = he;
      ++next_he;
      Halfedge_handle ohe = he->opposite();

      if ((*vit)->degree() > 2) {
        if (eq(he->facet()->plane(), ohe->facet()->plane())) {
          if (ohe->vertex()->degree() > 2) {
            polyhedron.join_facet(he);
          }
        }
      } else {
        if (eq(he->facet()->plane(), ohe->facet()->plane())) {
          polyhedron.erase_center_vertex(he);
        } else {
          polyhedron.join_vertex(ohe);
        }
        end_vertex = true;
        break;
      }
    }

    if (!end_vertex) {
      Halfedge_handle ohe = he->opposite();
      if ((*vit)->degree() > 2) {
        if (eq(he->facet()->plane(), ohe->facet()->plane())) {
          if (ohe->vertex()->degree() > 2) {
            polyhedron.join_facet(he);
          }
        }
      }
    }
  }
}

template <typename Kernel>
struct Direction_equal {
  Kernel & m_kernel;
  Direction_equal(Kernel & kernel) : m_kernel(kernel) {}
  template <typename Vector>
  bool operator()(Vector & v1, Vector & v2)
  {
    typename Kernel::Equal_3 eq = m_kernel.equal_3_object();
    return eq(v1.direction(), v2.direction());
  }
};

template <typename Kernel, typename Polyhedron>
void merge_coplanar_facets(Kernel & kernel, Polyhedron & polyhedron,
                           boost::false_type)
{
  Direction_equal<Kernel> eq(kernel);
  merge_coplanar_facets(polyhedron, eq); 
}

template <typename Kernel, typename Polyhedron>
void merge_coplanar_facets(Kernel & kernel, Polyhedron & polyhedron,
                           boost::true_type)
{
  typename Kernel::Equal_3 eq = kernel.equal_3_object();
  merge_coplanar_facets(polyhedron, eq);
}

SGAL_END_NAMESPACE

#endif
