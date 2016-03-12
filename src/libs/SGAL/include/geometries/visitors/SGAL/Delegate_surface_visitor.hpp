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

#ifndef SGAL_DELEGATE_SURFACE_VISITOR_HPP
#define SGAL_DELEGATE_SURFACE_VISITOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Delegate_surface_visitor :
  public boost::static_visitor<Boolean>
{
public:
  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;

  /*! Construct.
   */
  Delegate_surface_visitor(Geo_set::Primitive_type primitive_type,
                           Size num_primitives,
                           Shared_coord_array coord_array,
                           const Mesh_set::Facet_indices& coord_indices) :
    m_primitive_type(primitive_type),
    m_num_primitives(num_primitives),
    m_coord_array(coord_array),
    m_coord_indices(coord_indices)
  {}

  template <typename Polyhedron_>
  Boolean operator()(Polyhedron_& polyhedron) const
  {
    typedef Polyhedron_                         Polyhedron;
    typedef typename Polyhedron_::HalfedgeDS    HalfedgeDS;
    typedef Polyhedron_geo_builder<HalfedgeDS>  Builder;
    Builder surface(m_primitive_type, m_num_primitives, m_coord_array,
                    m_coord_indices);
    polyhedron.delegate(surface);
    return surface.is_consistent();
  }

private:
  /*! The primitive type of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Geo_set::Primitive_type m_primitive_type;

  /*! The number of facets of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Size m_num_primitives;

  /*! The coordinates of the mesh for which a polyhedron data structure is
   * constructed.
   */
  Shared_coord_array m_coord_array;

  /*! The coordinate indices of the mesh for which a polyhedron data structure
   * is constructed.
   */
  const Mesh_set::Facet_indices m_coord_indices;
};

SGAL_END_NAMESPACE

#endif
