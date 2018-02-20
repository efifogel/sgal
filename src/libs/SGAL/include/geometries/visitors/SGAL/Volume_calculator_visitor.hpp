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

#ifndef SGAL_VOLUME_CALCULATOR_VISITOR_HPP
#define SGAL_VOLUME_CALCULATOR_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"

SGAL_BEGIN_NAMESPACE

/*! A visitor that calculates the volume of a polyhedron. */
class Volume_calculator_visitor : public boost::static_visitor<Float> {
public:
  Float operator()(const Inexact_polyhedron& polyhedron) const
  { return volume<Inexact_kernel>(polyhedron); }

  Float operator()(const Epic_polyhedron& polyhedron) const
  { return volume<Epic_kernel>(polyhedron); }

  Float operator()(const Epec_polyhedron& polyhedron) const
  { return volume<Epec_kernel>(polyhedron); }

private:
  template <typename Kernel_, typename Polyhedron_>
  Float volume(const Polyhedron_& polyhedron) const
  {
    typedef Kernel_                           Kernel;
    typedef Polyhedron_                       Polyhedron;
    typedef typename Polyhedron::Facet        Polyhedron_facet;

    Float volume = 0;
    typename Kernel::Point_3 origin(CGAL::ORIGIN);
    //! \todo Fix CGAL::volume() to accept CGAL::ORIGIN as an argument.
    std::for_each(polyhedron.facets_begin(), polyhedron.facets_end(),
                  [&](const Polyhedron_facet& facet)
                  {
                    SGAL_assertion(3 ==
                                   CGAL::circulator_size(facet.facet_begin()));
                    auto h1 = facet.halfedge();
                    auto h2 = h1->next();
                    auto h3 = h2->next();
                    volume += to_float(CGAL::volume(origin,
                                                    h1->vertex()->point(),
                                                    h2->vertex()->point(),
                                                    h3->vertex()->point()));
                  });
    return volume;
  }
};

SGAL_END_NAMESPACE

#endif
