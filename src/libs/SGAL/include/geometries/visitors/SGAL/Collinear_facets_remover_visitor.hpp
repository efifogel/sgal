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

#ifndef SGAL_COLLINEAR_FACETS_REMOVER_VISITOR_HPP
#define SGAL_COLLINEAR_FACETS_REMOVER_VISITOR_HPP

#include <algorithm>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Epic_coord_array_3d.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

/*! Collinear_facets_remover_visitor removes collienar facets.
 */
class SGAL_SGAL_DECL Collinear_facets_remover_visitor :
  public boost::static_visitor<Size>
{
public:
  typedef boost::shared_ptr<Coord_array>        Shared_coord_array;

  //! Construct
  Collinear_facets_remover_visitor(Shared_coord_array coords) :
    m_coords(coords)
  {}

  /*!
   */
  template <typename Indices_>
  Size operator()(Indices_& indices) const
  {
    auto* field_info = m_coords->get_field_info(Coord_array::POINT);
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coords);
    if (coords) {
      auto& points = *(coords->array_handle(field_info));
      remove_collinear_facets(points, indices);
      return indices.size();
    }
    auto epic_coords =
      boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coords);
    if (epic_coords) {
      auto& points = *(epic_coords->array_handle(field_info));
      remove_collinear_facets(points, indices);
      return indices.size();;
    }
    auto epec_coords =
      boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coords);
    if (epec_coords) {
      auto& points = *(epec_coords->array_handle(field_info));
      remove_collinear_facets(points, indices);
      return indices.size();;
    }
    SGAL_error();
  }

  // The "flat" representation is inapplicable.
  Size operator()(Flat_indices& indices) const { SGAL_error(); return 0; }

private:
  template <typename Points_, typename Indices_>
  void remove_collinear_facets(Points_& points, Indices_& indices) const
  {
    Is_facet_collinear<Points_> is_facet_collinear(points);
    auto it = std::remove_if(indices.begin(), indices.end(), is_facet_collinear);
    indices.erase(it, indices.end());
  }

  template <typename Points_>
  class Is_facet_collinear {
  public:
    typedef Points_                     Points;

    /*! Construct.
     */
    Is_facet_collinear(const Points& points) : m_points(points) {}

    /*! Operator.
     */
    template <typename Facet_>
    Boolean operator()(const Facet_& facet)
    {
      auto it = facet.begin();
      const auto& p0 = m_points[*it++];
      const auto& p1 = m_points[*it++];
      Boolean col(true);
      do {
        const auto& p = m_points[*it++];
        col = col && is_collinear(p0, p1, p);
        if (!col) break;
      } while (it != facet.end());
      return col;
    }

  private:
    template <typename Point>
    Boolean is_collinear(const Point& p0, const Point& p1, const Point& p2)
      const
    { return CGAL::collinear(p0, p1, p2); }

    Boolean is_collinear(const Vector3f& p0, const Vector3f& p1,
                         const Vector3f& p2) const
    { return Vector3f::collinear(p0, p1, p2); }

    const Points& m_points;
  };

  const Shared_coord_array m_coords;
};

SGAL_END_NAMESPACE

#endif
