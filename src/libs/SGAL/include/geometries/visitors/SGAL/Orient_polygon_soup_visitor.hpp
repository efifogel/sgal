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

#ifndef SGAL_ORIENT_POLYGON_SOUP_VISITOR_HPP
#define SGAL_ORIENT_POLYGON_SOUP_VISITOR_HPP

#include <boost/variant.hpp>

#include <CGAL/basic.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;

SGAL_BEGIN_NAMESPACE

/*! Orient polygon soup visitor attempts to consistently orient a soup of
 * polygons in 3D space.
 * The polygons are gived asa mesh, that is, an array of points and an array
 * of a sequence of indices.
 * An object of this class can be used as a (boost) visitor when the indices
 * are stored as a variant (multi-type, single value) of several possible
 * types of indices.
 */
class SGAL_SGAL_DECL Orient_polygon_soup_visitor :
  public boost::static_visitor<Boolean>
{
public:
  typedef boost::shared_ptr<Coord_array>        Shared_coord_array;

  //! Construct
  Orient_polygon_soup_visitor(Shared_coord_array coords) : m_coords(coords) {}

  /*! Attempt to consistently orient a soup of polygons in 3D space.
   * \return true if the orientation operation succeded; return false if some
   *         points were duplicated, thus producing a self-intersecting
   *         polyhedron.
   */
  template <typename Indices_>
  Boolean operator()(Indices_& indices) const
  {
    auto* field_info = m_coords->get_field_info(Coord_array::POINT);
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coords);
    if (coords) {
      auto& points = *(coords->array_handle(field_info));
      return !operator()(points, indices);
    }
    auto epic_coords =
      boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coords);
    if (epic_coords) {
      auto& points = *(epic_coords->array_handle(field_info));
      return !operator()(points, indices);
    }
    auto epec_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coords);
    if (epec_coords) {
      auto& points = *(epec_coords->array_handle(field_info));
      return !operator()(points, indices);
    }
    SGAL_error();
  }

  // The "flat" representation is inapplicable.
  Boolean operator()(Flat_indices& indices) const { SGAL_error(); return false; }

private:
  template <typename Points_, typename Indices_>
  Boolean operator()(Points_& points, Indices_& indices) const
  { return CGAL::Polygon_mesh_processing::orient_polygon_soup(points, indices); }

  const Shared_coord_array m_coords;
};

SGAL_END_NAMESPACE

#endif
