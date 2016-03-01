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

#ifndef SGAL_NUMBER_OF_CONNECTED_COMPONENTS_VISITOR_HPP
#define SGAL_NUMBER_OF_CONNECTED_COMPONENTS_VISITOR_HPP

#include <map>
#include <boost/variant.hpp>
#include <boost/property_map/property_map.hpp>

#include <CGAL/basic.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;

SGAL_BEGIN_NAMESPACE

/*! Number of connected components polyhedron visitor counts the number of
 * connected components in a polygonal mesh.
 * An object of this class can be used as a (boost) visitor when the polyhedron
 * are stored as a variant (multi-type, single value) of several possible
 * types of polyhedrons.
 */
class Number_of_connected_components_polyhedron_visitor :
  public boost::static_visitor<Boolean>
{
public:
  /*! Count the number of connected components.
   * param[in] polyhedron the input polygonal mesh.
   * \return the number of connected components.
   */
  template <typename Polyhedron_>
  Boolean operator()(Polyhedron_& polyhedron) const
  {
    typedef Polyhedron_                 Polyhedron;
    auto index_map = CGAL::get(boost::face_external_index_t(), polyhedron);
    auto np = PMP::parameters::face_index_map(index_map);
    std::map<typename Polyhedron::Face_handle, size_t> face_ccs;
    auto fcm = boost::make_assoc_property_map(face_ccs);
    return PMP::connected_components(polyhedron, fcm, np);
  }
};

SGAL_END_NAMESPACE

#endif
