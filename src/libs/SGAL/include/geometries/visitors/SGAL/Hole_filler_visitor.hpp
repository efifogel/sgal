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

#ifndef SGAL_HOLE_FILLER_VISITOR_HPP
#define SGAL_HOLE_FILLER_VISITOR_HPP

#include <boost/variant.hpp>
#include <boost/foreach.hpp>

#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;

SGAL_BEGIN_NAMESPACE

/*! Hole filler visitor fills triangulated holes in a polyhedron that has
 * holes (in other words, the polyhedron has border edges).
 * An object of this class can be used as a (boost) visitor when the polyhedron
 * are stored as a variant (multi-type, single value) of several possible
 * types of polyhedrons.
 */
class SGAL_SGAL_DECL Hole_filler_visitor : public boost::static_visitor<> {
private:
  /*! Indicates whether to refine the triangulation of a hole by applying
   * local averaging rules.
   */
  Boolean m_refine;

  /*! Indicates whether to smooth the triangulation of a hole to obtain
   * as-smooth-as-possible shape deformation.
   */
  Boolean m_fair;

public:
/*! Construct. */
  Hole_filler_visitor(Boolean refine, Boolean fair) :
    m_refine(refine),
    m_fair(fair)
  {}

  void operator()(Inexact_polyhedron& polyhedron) const
  { SGAL_error_msg("Not implemented yet!"); }

  void operator()(Epic_polyhedron& polyhedron) const
  {
    Epic_kernel kernel;
    fill_holes(polyhedron, kernel);
  }

  void operator()(Epec_polyhedron& polyhedron) const
  { SGAL_error_msg("Not implemented yet!"); }

private:
  // Incrementally fill the holes
  template <typename Polyhedron, typename Kernel>
  void fill_holes(Polyhedron& poly, Kernel& kernel) const
  {
    typedef typename Polyhedron::Halfedge_handle    Halfedge_handle;
    typedef typename Polyhedron::Facet_handle       Facet_handle;
    typedef typename Polyhedron::Vertex_handle      Vertex_handle;

    Size nb_holes = 0;
    BOOST_FOREACH(Halfedge_handle h, halfedges(poly)) {
      if (!h->is_border()) continue;

      std::vector<Facet_handle>  patch_facets;
      std::vector<Vertex_handle> patch_vertices;
      bool success = CGAL::cpp11::get<0>
        (PMP::triangulate_refine_and_fair_hole
         (poly,
          h,
          std::back_inserter(patch_facets),
          std::back_inserter(patch_vertices),
          PMP::parameters::vertex_point_map(get(CGAL::vertex_point, poly)).
          geom_traits(kernel)));
      // std::cout << " Number of facets in constructed patch: "
      //           << patch_facets.size() << std::endl;
      // std::cout << " Number of vertices in constructed patch: "
      //           << patch_vertices.size() << std::endl;
      // std::cout << " Fairing : " << (success ? "succeeded" : "failed")
      //           << std::endl;
      ++nb_holes;
    }
    poly.normalize_border();
    // std::cout << std::endl;
    // std::cout << nb_holes << " holes have been filled" << std::endl;
  }
};

SGAL_END_NAMESPACE

#endif
