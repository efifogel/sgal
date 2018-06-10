// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <numeric>

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array_3d.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

void Dxf_parser::add_polylines(const Dxf_hatch_entity& hatch_entity,
                               SGAL::Group* root)
{
  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Count number of vertices:
  size_t size(0);
  size_t num_primitives(0);
  for (Dxf_boundary_path* path : hatch_entity.m_boundary_paths) {
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline || polyline->m_has_bulge) continue;
    size += polyline->m_locations.size();
    ++num_primitives;
  }

  if (0 == num_primitives) return;

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  Shared_appearance app(new SGAL::Appearance);
  SGAL_assertion(app);
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);

  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size + num_primitives);

  // Assign the vertices & indices:
  size_t i(0);
  auto it = indices.begin();
  auto cit = coords->begin();
  for (auto* path : hatch_entity.m_boundary_paths) {
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline || polyline->m_has_bulge) continue;
    cit = std::transform(polyline->m_locations.begin(),
                         polyline->m_locations.end(), cit,
                         [&](const std::array<double, 3>& p)
                         { return SGAL::Vector3f(p[0], p[1], p[2]); });
    auto it_end = it;
    std::advance(it_end, polyline->m_locations.size());
    std::iota(it, it_end, 0);
    it = it_end;
    *it++ = -1;
  }

  ils->set_primitive_type(SGAL::Geo_set::PT_LINE_LOOPS);
  ils->set_coord_array(shared_coords);
  ils->set_num_primitives(num_primitives);
}

DXF_END_NAMESPACE
