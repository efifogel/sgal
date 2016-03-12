// Copyright (c) 2016 Israel.
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

#ifndef SGAL_CLEAN_FACET_INDICES_FROM_POLYHEDRON_VISITOR_HPP
#define SGAL_CLEAN_FACET_INDICES_FROM_POLYHEDRON_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clean (compute) the indices of the facets from a polyhedral surface. */
class Clean_facet_indices_from_polyhedron_visitor :
  public boost::static_visitor<>
{
private:
  Size m_num_primitives;

public:
  /*! Construct.
   */
  Clean_facet_indices_from_polyhedron_visitor(Size num_primitives) :
    m_num_primitives(num_primitives)
  {}

  /*! Compute the indices.
   */
  template <typename Polyhedron_, typename Indices_>
  void operator()(Polyhedron_& polyhedron, Indices_& indices) const
  {
    typedef Indices_                                    Indices;
    typedef boost::is_same<Indices, Polygon_indices>    Is_polygon;

    indices.resize(m_num_primitives);
    size_t index(0);
    auto it = indices.begin();
    auto fit = polyhedron.facets_begin();
    for (; fit != polyhedron.facets_end(); ++fit, ++it) {
      auto hh = fit->facet_begin();
      initialize_polygon(*it, hh, Is_polygon());
      size_t j(0);
      do {
        (*it)[j++] = hh->vertex()->id();
        hh->set_id(index++);
      } while (++hh != fit->facet_begin());
    }
  }

  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron, Flat_indices& indices) const
  { SGAL_error_msg("Not implemented!"); }

private:
  /*! Resize the polygon.
   * \param[in] is_not_fixed the polygon is not a triangle nor a quad.
   * In this case, where the polygon is not a triangle nor a quad, we need
   * to resize the structure with the number of vertices.
   */
  template <typename Polygon_, typename HalfedgeHandle_>
  void initialize_polygon(Polygon_& polygon, HalfedgeHandle_ hh,
                          boost::true_type /*! is_not_fixed */) const
  { polygon.resize(CGAL::circulator_size(hh)); }

  /*! Resize the polygon.
   * \param[in] is_fixed the polygon is either a triangle or a quad.
   * In this case, where the polygon is a triangle or a quad, there is
   * nothing to do.
   */
  template <typename Polygon_, typename HalfedgeHandle_>
  void initialize_polygon(Polygon_& polygon, HalfedgeHandle_ hh,
                          boost::false_type /*! is_fixed */) const
  {}
};

SGAL_END_NAMESPACE

#endif
