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

//! \brief computes the number of segsments that approximates a circular arc.
size_t Dxf_parser::num_segments_of_circular_arc(const std::array<double, 2>& v1,
                                                const std::array<double, 2>& v2,
                                                double b)
{
  static const double min_b(0.1);

  if (0 == b) return 0;

  size_t segs_size(0);

  auto dx = v2[0] - v1[0];
  auto dy = v2[1] - v1[1];
  auto d2 = 0.5f * sqrt(dx * dx + dy * dy);
  auto d1 = b * d2;
  auto r = d2 * (1 + b*b) / (4.0 * b);

  while (b > min_b) {
    ++segs_size;

    auto d2_next = 0.5f * sqrt(d1 * d1 + d2 * d2);
    auto h_next = d2_next / b;
    auto d1_next = r - h_next;
    b = d1_next / d2_next;
    d1 = d1_next;
    d2 = d2_next;
  }
  return segs_size;
}

//! \brief add polylines.
void Dxf_parser::
add_polylines_with_bulge(const std::list<Dxf_polyline_boundary_path*>& polylines,
                         SGAL::Group* root, bool closed)
{
  size_t num_primitives(polylines.size());
  if (0 == num_primitives) return;

  std::cout << "add_polylines_with_bulge " << std::endl;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Count number of vertices:
  size_t size(0);
  for (auto* polyline : polylines) {
    auto bit = polyline->m_bulges.begin();
    auto vit_first = polyline->m_locations.begin();
    auto vit = vit_first;
    auto v = vit;
    auto b = *bit;
    size_t polyline_size(1);
    for (++vit, ++bit; vit != polyline->m_locations.end(); ++vit, ++bit) {
      size_t segs_size(1);
      // segs_size += num_segments_of_circular_arc(*v, *vit, b);
      std::cout << "    segs size: " << segs_size << std::endl;
      polyline_size += segs_size;
      v = vit;
      b = *bit;
    }

    if (closed) {
      size_t segs_size = num_segments_of_circular_arc(*v, *vit_first, b);
      std::cout << "    segs size: " << segs_size << std::endl;
      // polyline_size += segs_size;
    }

    std::cout << "  polyline size: " << polyline_size << std::endl;
    size += polyline_size;
  }
  std::cout << "hatch size: " << size << std::endl;

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
  for (auto* polyline : polylines) {
    cit = std::transform(polyline->m_locations.begin(),
                         polyline->m_locations.end(), cit,
                         [&](const std::array<double, 2>& p)
                         { return SGAL::Vector3f(p[0], p[1], 0); });
    auto it_end = it;
    std::advance(it_end, polyline->m_locations.size());
    std::iota(it, it_end, 0);
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
  size_t i(0);
  auto it = indices.begin();
  auto cit = coords->begin();
  for (auto* polyline : polylines) {
    cit = std::transform(polyline->m_locations.begin(),
                         polyline->m_locations.end(), cit,
                         [&](const std::array<double, 2>& p)
                         { return SGAL::Vector3f(p[0], p[1], 0); });
    auto it_end = it;
    std::advance(it_end, polyline->m_locations.size());
    std::iota(it, it_end, 0);
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
  std::list<Dxf_polyline_boundary_path*>  open_polylines;
  std::list<Dxf_polyline_boundary_path*>  closed_polylines;
  std::list<Dxf_polyline_boundary_path*>  open_polylines_with_bulge;
  std::list<Dxf_polyline_boundary_path*>  closed_polylines_with_bulge;
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
  //add_polylines(closed_polylines, root, true);
  //add_polylines(open_polylines, root, false);
  add_polylines_with_bulge(closed_polylines_with_bulge, root, true);
  add_polylines_with_bulge(open_polylines_with_bulge, root, false);
}

DXF_END_NAMESPACE
