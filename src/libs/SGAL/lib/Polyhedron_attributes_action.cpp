// Copyright (c) 2015 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/Polyhedron_attributes_action.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief applies the action to a given node.
Action::Trav_directive Polyhedron_attributes_action::apply(Shared_node node)
{
  if (!node) return TRAV_CONT;

  auto shape = boost::dynamic_pointer_cast<Shape>(node);
  if (!shape) return node->traverse(this);

  const auto geometry = shape->get_geometry();
  const auto ifs = boost::dynamic_pointer_cast<Indexed_face_set>(geometry);
  if (ifs) insert(ifs);

  return TRAV_CONT;
}

//! \brief adds some volume to the accumulated volume.
inline void Polyhedron_attributes_action::insert(Shared_indexed_face_set ifs)
{
  Size number_of_vertices(ifs->get_number_of_vertices());
  Size number_of_edges(ifs->get_number_of_edges());
  Size number_of_facets(ifs->get_number_of_facets());
  auto volume(ifs->volume());
  auto valid(ifs->is_consistent() && ifs->is_closed());
  auto surface_area(ifs->surface_area());
  Size number_of_connected_components(ifs->get_number_of_connected_components());
  const auto& bounding_box(ifs->bounding_box());
  const auto& bounding_sphere(ifs->get_bounding_sphere());
  auto& array = m_array.get_array();
  array.emplace(array.end(), valid,
                number_of_vertices, number_of_edges, number_of_facets,
                volume, surface_area, number_of_connected_components,
                bounding_box, bounding_sphere);
}

SGAL_END_NAMESPACE
