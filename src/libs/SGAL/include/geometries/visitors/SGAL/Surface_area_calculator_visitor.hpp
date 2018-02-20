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

#ifndef SGAL_SURFACE_AREA_CALCULATOR_VISITOR_HPP
#define SGAL_SURFACE_AREA_CALCULATOR_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"

SGAL_BEGIN_NAMESPACE

/*! A visitor that calculates the surface area of a polyhedron. */
class Surface_area_calculator_visitor : public boost::static_visitor<Float> {
public:
  Float operator()(const Inexact_polyhedron& polyhedron) const
  { return surface_area<Inexact_kernel>(polyhedron); }

  Float operator()(const Epic_polyhedron& polyhedron) const
  { return surface_area<Epic_kernel>(polyhedron); }

  Float operator()(const Epec_polyhedron& polyhedron) const
  { return surface_area<Epec_kernel>(polyhedron); }

private:
  template <typename Kernel_, typename Polyhedron_>
  Float surface_area(const Polyhedron_& polyhedron) const
  {
    typedef Kernel_                           Kernel;
    typedef Polyhedron_                       Polyhedron;
    typedef typename Polyhedron::Facet        Polyhedron_facet;

    Float surface_area = 0;
    std::for_each(polyhedron.facets_begin(), polyhedron.facets_end(),
                  [&](const Polyhedron_facet& facet)
                  {
                    SGAL_assertion(3 == CGAL::circulator_size(facet.facet_begin()));
                    auto h = facet.halfedge();
                    const auto& p1 = h->vertex()->point();
                    const auto& p2 = h->next()->vertex()->point();
                    const auto& p3 = h->next()->next()->vertex()->point();
                    // m_surface_area += CGAL::area(p1, p2, p3);
                    typename Kernel::Triangle_3 tri(p1, p2, p3);
                    surface_area += sqrtf(to_float(tri.squared_area()));
                  });
    return surface_area;
  }
};

SGAL_END_NAMESPACE

#endif
