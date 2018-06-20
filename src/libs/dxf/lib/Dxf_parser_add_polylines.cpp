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
#include "SGAL/approximate_circular_arc.hpp"
#include "SGAL/Vector2f.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

//! \brief add polylines that have bulge factors.
void Dxf_parser::
add_polylines_with_bulge(const std::list<Dxf_polyline_boundary_path*>& polylines,
                         SGAL::Group* root, bool closed)
{
  static const double min_bulge(0.1);

  size_t num_primitives(polylines.size());
  if (0 == num_primitives) return;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

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
  auto* coords = new SGAL::Coord_array_3d();
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Compute the circular arc approximation.
  typedef std::list<SGAL::Vector2f>             Polyline;
  std::vector<Polyline> polylines_segs(num_primitives);
  auto sit = polylines_segs.begin();
  for (auto* polyline : polylines) {
    auto& segs = *sit++;

    auto bit = polyline->m_bulges.begin();
    auto vit_first = polyline->m_locations.begin();
    auto vit = vit_first;
    auto v = vit;
    auto b = *bit;

    const auto& p = *v;
    segs.push_back(SGAL::Vector2f(p[0], p[1]));

    for (++vit, ++bit; vit != polyline->m_locations.end(); ++vit, ++bit) {
      SGAL::Vector2f v1((*v)[0], (*v)[1]);
      SGAL::Vector2f v2((*vit)[0], (*vit)[1]);
      SGAL::approximate_circular_arc(v1, v2, b, min_bulge,
                                     std::back_inserter(segs));
      segs.push_back(v2);

      v = vit;
      b = *bit;
    }

    if (closed) {
      SGAL::Vector2f v1((*v)[0], (*v)[1]);
      SGAL::Vector2f v2((*vit_first)[0], (*vit_first)[1]);
      SGAL::approximate_circular_arc(v1, v2, b, min_bulge,
                                     std::back_inserter(segs));
    }
  }

  // Count the number of segments and allocate the vertices and indices
  size_t size(0);
  for (const auto& segs : polylines_segs) size += segs.size();
  coords->resize(size);
  auto& indices = ils->get_coord_indices();
  indices.resize(size + num_primitives);

  // Assign the vertices & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (const auto& segs : polylines_segs) {
    cit = std::transform(segs.begin(), segs.end(), cit,
                         [&](const SGAL::Vector2f& p)
                         { return SGAL::Vector3f(p[0], p[1], 0); });
    auto it_end = it;
    std::advance(it_end, segs.size());
    std::iota(it, it_end, i);
    i += segs.size();
    it = it_end;
    *it++ = -1;
  }

  auto type = closed ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_num_primitives(num_primitives);
}

//! \brief add polylines.
void Dxf_parser::
add_polylines(const std::list<Dxf_polyline_boundary_path*>& polylines,
              SGAL::Group* root, bool closed)
{
  size_t num_primitives(polylines.size());
  if (0 == num_primitives) return;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Count number of vertices:
  size_t size(0);
  for (auto* polyline : polylines) size += polyline->m_locations.size();

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
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (auto* polyline : polylines) {
    cit = std::transform(polyline->m_locations.begin(),
                         polyline->m_locations.end(), cit,
                         [&](const SGAL::Vector2f& p)
                         { return SGAL::Vector3f(p[0], p[1], 0); });
    auto it_end = it;
    std::advance(it_end, polyline->m_locations.size());
    std::iota(it, it_end, i);
    i += polyline->m_locations.size();
    it = it_end;
    *it++ = -1;
  }

  auto type = closed ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_num_primitives(num_primitives);
}

//! \brief add polylines provided in hatch entities.
void Dxf_parser::add_polylines(const Dxf_hatch_entity& hatch_entity,
                               SGAL::Group* root)
{
  std::list<Dxf_polyline_boundary_path*> open_polylines;
  std::list<Dxf_polyline_boundary_path*> closed_polylines;
  std::list<Dxf_polyline_boundary_path*> open_polylines_with_bulge;
  std::list<Dxf_polyline_boundary_path*> closed_polylines_with_bulge;
  for (Dxf_boundary_path* path : hatch_entity.m_boundary_paths) {
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline) continue;
    if (polyline->m_has_bulge) {
      if (polyline->m_is_closed) closed_polylines_with_bulge.push_back(polyline);
      else open_polylines_with_bulge.push_back(polyline);
    }
    else {
      if (polyline->m_is_closed) closed_polylines.push_back(polyline);
      else open_polylines.push_back(polyline);
    }
  }

  add_polylines(closed_polylines, root, true);
  add_polylines(open_polylines, root, false);
  add_polylines_with_bulge(closed_polylines_with_bulge, root, true);
  add_polylines_with_bulge(open_polylines_with_bulge, root, false);
}

DXF_END_NAMESPACE
