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

#ifndef SCGAL_POLYHEDRON_GEO_BUILDER_HPP
#define SCGAL_POLYHEDRON_GEO_BUILDER_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

template <typename HDS>
class Polyhedron_geo_builder : public CGAL::Modifier_base<HDS> {
private:
  const Mesh_set* m_mesh_set;

public:
  /*! Constructor. */
  Polyhedron_geo_builder() {}

  void set_mesh_set(const Mesh_set* mesh_set) { m_mesh_set = mesh_set; }

  /*! */
  void operator()(HDS& hds)
  {
    // Postcondition: `hds' is a valid polyhedral surface.
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    typedef typename CGAL::Polyhedron_incremental_builder_3<HDS>::size_type
      size_type;
    const Geo_set::Shared_coord_array coord_array =
      m_mesh_set->get_coord_array();
    size_type coord_array_size = coord_array->size();
    unsigned int num_facets = m_mesh_set->get_num_primitives();
    B.begin_surface(coord_array_size, num_facets);
    typedef typename HDS::Vertex Vertex;
    typedef typename Vertex::Point Point;
    // Add the points:
    Uint i;
    for (i = 0; i < coord_array_size; i++) {
      const Vector3f& v = (*coord_array)[i];
      B.add_vertex(Point(v[0], v[1], v[2]));
    }

    // Add the faces:
    if (m_mesh_set->are_coord_indices_flat()) {
      if (m_mesh_set->get_primitive_type() == Geo_set::PT_TRIANGLES) {
        Uint j = 0;
        for (i = 0; i < num_facets; ++i) {
          B.begin_facet();
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j));
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+1));
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+2));
          B.end_facet();
          j += 3;
        }
      }
      else if (m_mesh_set->get_primitive_type() == Geo_set::PT_QUADS) {
        Uint j = 0;
        for (i = 0; i < num_facets; ++i) {
          B.begin_facet();
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j));
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+1));
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+2));
          B.add_vertex_to_facet(m_mesh_set->get_flat_coord_index(j+3));
          B.end_facet();
          j += 4;
        }
      }
      else SGAL_error();
    }
    else {
      Uint j = 0;
      for (i = 0; i < num_facets; ++i) {
        B.begin_facet();
        for (; m_mesh_set->get_coord_index(j) != (Uint) -1; ++j) {
          B.add_vertex_to_facet(m_mesh_set->get_coord_index(j));
        }
        ++j;
        B.end_facet();
      }
    }
    B.end_surface();
  }
};

SGAL_END_NAMESPACE

#endif
