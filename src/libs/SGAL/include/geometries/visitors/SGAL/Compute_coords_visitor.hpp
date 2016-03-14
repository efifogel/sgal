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

#ifndef SGAL_COMPUTE_COORDS_VISITOR_HPP
#define SGAL_COMPUTE_COORDS_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

/*! Compute coords visitor. */
class Compute_coords_visitor : public boost::static_visitor<> {
public:
  typedef boost::shared_ptr<Coord_array>        Shared_coord_array;

  Compute_coords_visitor(Shared_coord_array coords) : m_coords(coords) {}

  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron) const
  {
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coords);
    if (coords) {
      operator()(polyhedron, coords->begin());
      return;
    }

    // auto epic_coords =
    //   boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coords);
    // if (epic_coords) {
    //   operator()(polyhedron, epic_coords->begin());
    //   return;
    // }

    auto epec_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coords);
    if (epec_coords) {
      operator()(polyhedron, epec_coords->begin());
      return;
    }

    SGAL_error();
  }

private:
  // General case, covers:
  // Epic <= {Inexact, Epec} and
  // Epec <= {Inexact, Epic}
  template <typename Polyhedron_, typename InputIterator>
  void operator()(Polyhedron_& polyhedron, InputIterator begin) const
  { SGAL_error(); }

  // Inexact <= {Inexact, Epic, Epec}
  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron, Coord_array_3d::iterator begin) const
  {
    auto cit = begin;
    Size index(0);
    auto vit = polyhedron.vertices_begin();
    for (; vit != polyhedron.vertices_end(); ++vit) {
      vit->set_id(index++);
      *cit++ = to_vector3f(vit->point());
    }
  }

  // Epic <= Epic
  void operator()(Epic_polyhedron& polyhedron,
                  Epic_coord_array_3d::iterator begin) const
  { compute_coords(polyhedron, begin); }

  void operator()(Epec_polyhedron& polyhedron,
                  Epec_coord_array_3d::iterator begin) const
  // Epec <= Epec
  { compute_coords(polyhedron, begin); }

  template <typename Polyhedron_, typename InputIterator>
  void compute_coords(Polyhedron_& polyhedron, InputIterator begin) const
  {
    auto cit = begin;
    Size index(0);
    auto vit = polyhedron.vertices_begin();
    for (; vit != polyhedron.vertices_end(); ++vit) {
      vit->set_id(index++);
      *cit++ = vit->point();
    }
  }

  Shared_coord_array m_coords;
};

SGAL_END_NAMESPACE

#endif
