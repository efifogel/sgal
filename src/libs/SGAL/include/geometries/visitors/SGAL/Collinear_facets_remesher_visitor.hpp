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

#ifndef SGAL_COLLINEAR_FACETS_REMESHER_VISITOR_HPP
#define SGAL_COLLINEAR_FACETS_REMESHER_VISITOR_HPP

#include <boost/variant.hpp>
#include <boost/foreach.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Geo_set.hpp"
#include "SGAL/Inexact_polyhedron.hpp"
#include "SGAL/Epic_polyhedron.hpp"
#include "SGAL/Epec_polyhedron.hpp"

SGAL_BEGIN_NAMESPACE

/*! Collinear_facets_remesher_visitor remeshes collinear facets.
 */
class SGAL_SGAL_DECL Collinear_facets_remesher_visitor :
  public boost::static_visitor<Size>
{
public:
  typedef Geo_set::Primitive_type                       Primitive_type;

  /*! Construct.
   */
  Collinear_facets_remesher_visitor(Primitive_type type) :
    m_primitive_type(type)
  {}

  Size operator()(Inexact_polyhedron& polyhedron) const
  { return remesher<Inexact_kernel>(polyhedron); }

  Size operator()(Epic_polyhedron& polyhedron) const
  { return remesher<Epic_kernel>(polyhedron); }

  Size operator()(Epec_polyhedron& polyhedron) const
  { return remesher<Epec_kernel>(polyhedron); }

private:
  template <typename Kernel_, typename Polyhedron_>
  Size remesher(Polyhedron_& polyhedron) const
  {
    std::cout << "# border: " << polyhedron.size_of_border_edges() << std::endl;
    // Do not remesh at all if the facet are not triangles.
    if (m_primitive_type != Geo_set::PT_TRIANGLES)
      return polyhedron.size_of_facets();

    typedef Kernel_                           Kernel;
    typedef Polyhedron_                       Polyhedron;
    typedef typename Polyhedron::Halfedge_around_facet_circulator
                                              Halfedge_around_facet_circulator;
    typedef typename Polyhedron::Halfedge_around_vertex_circulator
                                              Halfedge_around_vertex_circulator;
    auto it = polyhedron.facets_begin();
    for (; it != polyhedron.facets_end(); ++it) {
      auto j = it->facet_begin();
      auto he0 = j++;
      auto he1 = j++;
      auto he2 = j++;
      const auto& p0 = he0->vertex()->point();
      const auto& p1 = he1->vertex()->point();
      const auto& p2 = he2->vertex()->point();
      if (!CGAL::collinear(p0, p1, p2)) continue;

      typename Kernel::Segment_3 s2(p0, p1);
      if (s2.has_on(p2)) {
        process(polyhedron, he0, he1, he2);
        continue;
      }
      typename Kernel::Segment_3 s0(p1, p2);
      if (s0.has_on(p0)) {
        process(polyhedron, he1, he2, he0);
        continue;
      }
      typename Kernel::Segment_3 s1(p2, p0);
      SGAL_assertion(s1.has_on(p1));
      process(polyhedron, he2, he0, he1);
    }
    return polyhedron.size_of_facets();
  }

  template <typename Polyhedron_, typename HalfedgeHandle_>
  void process(Polyhedron_& polyhedron,
               HalfedgeHandle_ he0, HalfedgeHandle_ he1, HalfedgeHandle_ he2)
    const
  {
    if ((he1->opposite()->is_border()) ||
        (he0->opposite()->is_border() && he2->opposite()->is_border()))
    {
      polyhedron.erase_facet(he1);
      return;
    }
    if ((3 == CGAL::circulator_size(he2->vertex()->vertex_begin())) &&
        (!he0->opposite()->is_border() && !he2->opposite()->is_border()))
    {
      polyhedron.erase_center_vertex(he2);
      return;
    }
    polyhedron.join_facet(he1);
    polyhedron.split_facet(he2, he1->opposite()->next());
  }

  template <typename Facet_>
  Boolean is_facet_collinear(const Facet_& facet) const
  {
    auto j = facet.facet_begin();
    const auto& p0 = j++->vertex()->point();
    const auto& p1 = j++->vertex()->point();
    Boolean col(true);
    do {
      const auto& p = j++->vertex()->point();
      col = col && CGAL::collinear(p0, p1, p);
      if (!col) break;
    } while (j != facet.facet_begin());
    return col;
  }

  /*! The facet type.
   */
  Primitive_type m_primitive_type;
};

SGAL_END_NAMESPACE

#endif
