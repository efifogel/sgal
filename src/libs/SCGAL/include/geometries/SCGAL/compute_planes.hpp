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
// $Id: compute_planes.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_COMPUTE_PLANES_HPP
#define SCGAL_COMPUTE_PLANES_HPP

/*! \file
 * This file contains a free function that computes the equesions of or the
 * normal to the planes containing the facets of a given polyhedron.
 */

#include <algorithm>
#include <boost/type_traits.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Compute the equation of the normal to a facet */
template <typename Kernel>
class Normal_equation {
private:
  const Kernel& m_kernel;

public:
  Normal_equation(const Kernel& kernel) : m_kernel(kernel) {}
  template <typename Facet>
  typename Facet::Plane_3 operator()(Facet& f) {
    typename Facet::Halfedge_handle h = f.halfedge();
    typename Facet::Vertex::Point_3& p1 = h->vertex()->point();
    typename Facet::Vertex::Point_3& p2 = h->next()->vertex()->point();
    h = h->next()->next();
    typename Kernel::Collinear_3 collinear = m_kernel.collinear_3_object();
    while (collinear(p1, p2, h->vertex()->point())) h = h->next();
    typename Kernel::Construct_cross_product_vector_3 cross_product =
      m_kernel.construct_cross_product_vector_3_object();
    return cross_product(p2 - p1, h->vertex()->point() - p2);
  }
};

template <typename Kernel, typename T_Polyhedron>
void compute_planes(Kernel& kernel, T_Polyhedron& polyhedron, boost::false_type)
{
  std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                 polyhedron.planes_begin(), Normal_equation<Kernel>(kernel));
}

/*! Compute the equation of the undelying plane of a facet.
 * This function allows for consecutive vertices to be collinear
 */
template <typename Kernel>
class Plane_equation {
private:
  const Kernel& m_kernel;

public:
  Plane_equation(const Kernel& kernel) : m_kernel(kernel) {}
  template <typename Facet>
  typename Facet::Plane_3 operator()(Facet& f) {
    typename Facet::Halfedge_handle h = f.halfedge();
    typename Facet::Vertex::Point_3& p1 = h->vertex()->point();
    typename Facet::Vertex::Point_3& p2 = h->next()->vertex()->point();
    h = h->next()->next();
    typename Kernel::Collinear_3 collinear = m_kernel.collinear_3_object();
    while (collinear(p1, p2, h->vertex()->point())) h = h->next();
    return m_kernel.construct_plane_3_object()(p1, p2, h->vertex()->point());
  }
};

template <typename Kernel, typename T_Polyhedron>
void compute_planes(Kernel& kernel, T_Polyhedron& polyhedron, boost::true_type)
{
  std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                 polyhedron.planes_begin(), Plane_equation<Kernel>(kernel));
}

SGAL_END_NAMESPACE

#endif
