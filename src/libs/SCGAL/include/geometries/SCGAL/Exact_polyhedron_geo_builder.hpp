// Copyright (c) 2004 Israel.
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

#ifndef SCGAL_EXACT_POLYHEDRON_GEO_BUILDER_HPP
#define SCGAL_EXACT_POLYHEDRON_GEO_BUILDER_HPP

#include <CGAL/Cartesian.h>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_coord_array.hpp"

SGAL_BEGIN_NAMESPACE

template <typename HDS>
class Exact_polyhedron_geo_builder : public Polyhedron_geo_builder<HDS> {
protected:
  /*! Insert the vertices.
   */
  virtual void insert_vertices(CGAL::Polyhedron_incremental_builder_3<HDS>& B)
  {
    typedef typename HDS::Vertex Vertex;
    typedef typename Vertex::Point Point;

    boost::shared_ptr<Coord_array> coord_array =
      this->m_mesh_set->get_coord_array();

    boost::shared_ptr<Exact_coord_array> exact_coords =
      boost::static_pointer_cast<Exact_coord_array>(coord_array);
    if (exact_coords) {
      for (auto it = exact_coords->begin(); it != exact_coords->end(); ++it)
        B.add_vertex(Point(*it));
      return;
    }
    boost::shared_ptr<Coord_array_3d> coords =
      boost::static_pointer_cast<Coord_array_3d>(coord_array);
    if (coords) {
      for (auto it = coords->begin(); it != coords->end(); ++it)
        B.add_vertex(Point((*it)[0], (*it)[1], (*it)[2]));
      return;
    }
    SGAL_error();
  }

// public:
//   /*! Build the polyhedral surface.
//    * \param hds (out) the halfedge data structure, which stores the incidence
//    *            relations between the cells of the polyhedral surface.
//    */
//   void operator()(HDS& hds)
//   {
//     // Postcondition: `hds' is a valid polyhedral surface.
//     CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
//     B.begin_surface(this->m_mesh_set->get_coord_array()->size(),
//                     this->m_mesh_set->get_num_primitives());
//     insert_vertices(B);
//     insert_faces(B);
//     B.end_surface();
//   }
};

SGAL_END_NAMESPACE

#endif
