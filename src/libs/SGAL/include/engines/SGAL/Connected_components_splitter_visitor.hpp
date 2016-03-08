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

#ifndef SGAL_CONNECTED_COMPONENTS_SPLITTER_VISITOR_HPP
#define SGAL_CONNECTED_COMPONENTS_SPLITTER_VISITOR_HPP

#include <boost/variant.hpp>
#include <boost/foreach.hpp>

#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;

SGAL_BEGIN_NAMESPACE

/*! Connected components splitter visitor splits the connected components
 * of a polyhedral surface into separets polyhedral surfaces.
 * An object of this class can be used as a (boost) visitor when the polyhedron
 * are stored as a variant (multi-type, single value) of several possible
 * types of polyhedrons.
 */
class SGAL_SGAL_DECL Connected_components_splitter_visitor :
  public boost::static_visitor<Size>
{
private:
  Size m_id;

public:
  //! Construct
  Connected_components_splitter_visitor(Size id) : m_id(id) {}

  //! Operator
  template <typename Polyhedron_, typename Indices_>
  Size operator()(const Polyhedron_& polyhedron, Indices_& indices) const
  { SGAL_error(); return 0; }

  template <typename Polyhedron_>
  Size operator()(const Polyhedron_& polyhedron,
                  Triangle_indices& indices) const
  {
    Size count(0);
    BOOST_FOREACH(auto f, faces(polyhedron)) {
      if (f->id() != m_id) continue;
      ++count;
    }
    indices.resize(count);
    auto it = indices.begin();
    BOOST_FOREACH(auto f, faces(polyhedron)) {
      if (f->id() != m_id) continue;
      auto& tri = *it++;
      auto hh = f->facet_begin();
      tri[0] = hh++->vertex()->id();
      tri[1] = hh++->vertex()->id();
      tri[2] = hh->vertex()->id();
    }
    return count;
  }

  // template <typename Polyhedron_>
  // void operator()(const Polyhedron_& polyhedron,
  //                 Quad_indices& indices) const
  // {
  //   SGAL_error_msg("Not implemented yet!");
  // }

  // template <typename Polyhedron_>
  // void operator()(const Polyhedron_& polyhedron,
  //                 Polygon_indices& indices) const
  // {
  //   SGAL_error_msg("Not implemented yet!");
  // }

  // template <typename Polyhedron_>
  // void operator()(const Polyhedron_& polyhedron,
  //                 const Mesh_set::Flat_indices& indices) const
  // { SGAL_error(); }
};

SGAL_END_NAMESPACE

#endif
