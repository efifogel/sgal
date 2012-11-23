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

#ifndef SGAL_COMPUTE_PLANES_HPP
#define SGAL_COMPUTE_PLANES_HPP

/*! \file
 * This file contains a free function that computes the equesions of or the
 * normal to the planes containing the facets of a given polyhedron.
 */

#include <algorithm>
#include <boost/type_traits.hpp>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Compute the equation of the normal to a facet */
struct Normal_equation {
  template <typename Facet>
  typename Facet::Plane_3 operator()(Facet & f) {
    typename Facet::Halfedge_handle h = f.halfedge();
    return CGAL::cross_product(h->next()->vertex()->point() -
                               h->vertex()->point(),
                               h->next()->next()->vertex()->point() -
                               h->next()->vertex()->point());
  }
};

template <typename T_Polyhedron>
void compute_planes(T_Polyhedron & polyhedron, boost::false_type)
{
  std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                 polyhedron.planes_begin(), Normal_equation());
}

/*! Compute the equation of the undelying plane of a facet */
struct Plane_equation {
  template <typename Facet>
  typename Facet::Plane_3 operator()(Facet & f) {
    typename Facet::Halfedge_handle h = f.halfedge();
    return typename Facet::Plane_3(h->vertex()->point(),
                                   h->next()->vertex()->point(),
                                   h->next()->next()->vertex()->point());
  }
};

template <typename T_Polyhedron>
void compute_planes(T_Polyhedron & polyhedron, boost::true_type)
{
  std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                 polyhedron.planes_begin(), Plane_equation());
}

SGAL_END_NAMESPACE

#endif
