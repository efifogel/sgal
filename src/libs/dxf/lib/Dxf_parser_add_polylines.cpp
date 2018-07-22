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
#include <limits>
#include <cmath>

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/approximate_circular_arc.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Loader_errors.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_polyline_entity.hpp"
#include "dxf/Dxf_line_entity.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

//! \brief add polylines that have bulge factors.
void Dxf_parser::
add_polylines_with_bulge(const Dxf_hatch_entity& hatch,
                         const std::list<Dxf_polyline_boundary_path*>& polylines,
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

  // Check whether mirroring is required
  bool mirror(false);
  if ((hatch.m_extrusion_direction[0] == 0) &&
      (hatch.m_extrusion_direction[1] == 0) &&
      (hatch.m_extrusion_direction[2] < 0))
    mirror = true;

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
    auto x = (mirror) ? -p[0] : p[0];
    segs.push_back(SGAL::Vector2f(x, p[1]));

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
add_polylines(const Dxf_hatch_entity& hatch,
              const std::list<Dxf_polyline_boundary_path*>& polylines,
              SGAL::Group* root, bool closed)
{
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

  // Count number of vertices:
  size_t size(0);
  for (auto* polyline : polylines) size += polyline->m_locations.size();

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size + num_primitives);

  // Check whether mirroring is required
  bool mirror(false);
  if ((hatch.m_extrusion_direction[0] == 0) &&
      (hatch.m_extrusion_direction[1] == 0) &&
      (hatch.m_extrusion_direction[2] < 0))
    mirror = true;

  // Assign the vertices & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (auto* polyline : polylines) {
    cit = std::transform(polyline->m_locations.begin(),
                         polyline->m_locations.end(), cit,
                         [&](const SGAL::Vector2f& p)
                         {
                           auto x = (mirror) ? -p[0] : p[0];
                           return SGAL::Vector3f(x, p[1], 0);
                         });
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
void Dxf_parser::add_polylines(const Dxf_hatch_entity& hatch, SGAL::Group* root)
{
  std::list<Dxf_polyline_boundary_path*> open_polylines;
  std::list<Dxf_polyline_boundary_path*> closed_polylines;
  std::list<Dxf_polyline_boundary_path*> open_polylines_with_bulge;
  std::list<Dxf_polyline_boundary_path*> closed_polylines_with_bulge;
  for (Dxf_base_boundary_path* path : hatch.m_boundary_paths) {
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline) {
      SGAL_warning_msg(true, "Unsupported boundary path!");
      continue;
    }
    if (polyline->m_has_bulge) {
      if (polyline->m_is_closed) closed_polylines_with_bulge.push_back(polyline);
      else open_polylines_with_bulge.push_back(polyline);
    }
    else {
      if (polyline->m_is_closed) closed_polylines.push_back(polyline);
      else open_polylines.push_back(polyline);
    }
  }

  m_polylines_num += closed_polylines.size() + open_polylines.size() +
    closed_polylines_with_bulge.size() + open_polylines_with_bulge.size();

  add_polylines(hatch, closed_polylines, root, true);
  add_polylines(hatch, open_polylines, root, false);
  add_polylines_with_bulge(hatch, closed_polylines_with_bulge, root, true);
  add_polylines_with_bulge(hatch, open_polylines_with_bulge, root, false);
}

//! \brief adds lines provided in line entities.
void Dxf_parser::add_polylines(const Dxf_line_entity& line, SGAL::Group* root)
{
  auto* parser = line.m_parser;

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

  // Allocate colors:
  auto color = line.m_color;
  if (color == static_cast<int16_t>(By::BYLAYER)) {
    auto it = parser->m_layer_table.find(line.m_layer);
    color = it->m_color;
  }

  auto ait = parser->m_color_arrays.find(color);
  if (ait == parser->m_color_arrays.end()) {
    auto* colors = new SGAL::Color_array(size_t(1));
    Shared_color_array shared_colors(colors);
    colors->add_to_scene(m_scene_graph);
    m_scene_graph->add_container(shared_colors);
    if (color >= s_palette.size()) color = 0;
    (*colors)[0] = s_palette[color];
    auto rc = parser->m_color_arrays.insert(std::make_pair(color, shared_colors));
    ait = rc.first;
  }
  auto shared_colors = ait->second;

  // Allocate vertices:
  size_t size(2);
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size);

  // Assign the vertices & indices:
  auto cit = coords->begin();
  *cit++ = SGAL::Vector3f(line.m_start[0], line.m_start[1], line.m_start[2]);
  *cit++ = SGAL::Vector3f(line.m_end[0], line.m_end[1], line.m_end[2]);

  auto it = indices.begin();
  *it++ = 0;
  *it++ = 1;

  ils->set_primitive_type(SGAL::Geo_set::PT_LINES);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(1);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief adds polylines provided in spline entities.
void Dxf_parser::add_polylines(const Dxf_spline_entity& spline_entity,
                               SGAL::Group* root)
{
  return;

  size_t number_of_poles(spline_entity.m_control_points.size());
  size_t degree(spline_entity.m_degree);
  bool periodic(spline_entity.is_periodic());
  bool planar(spline_entity.is_planar());

  std::cout << "Normal: " << spline_entity.m_normal << std::endl;
  std::cout << "Flags: " << spline_entity.m_flags << std::endl;
  if (spline_entity.m_flags & Dxf_spline_entity::CLOSED)
    std::cout << "  CLOSED" << std::endl;
  if (spline_entity.m_flags & Dxf_spline_entity::PERIODIC)
    std::cout << "  PERIODIC" << std::endl;
  if (spline_entity.m_flags & Dxf_spline_entity::RATIONAL)
    std::cout << "  RATIONAL" << std::endl;
  if (spline_entity.m_flags & Dxf_spline_entity::PLANAR)
    std::cout << "  PLANAR" << std::endl;
  if (spline_entity.m_flags & Dxf_spline_entity::LINEAR)
    std::cout << "  LINEAR" << std::endl;
  std::cout << "Degree: " << spline_entity.m_degree << std::endl;
  std::cout << "Knot tolerance: " << spline_entity.m_knot_tolerance << std::endl;
  std::cout << "Control Point tolerance: "
            << spline_entity.m_control_point_tolerance << std::endl;
  std::cout << "Fit tolerance: " << spline_entity.m_fit_tolerance << std::endl;
  std::cout << "Start tangent: " << spline_entity.m_start_tangent[0] << ", "
            << spline_entity.m_start_tangent[1] << ", "
            << spline_entity.m_start_tangent[2] << std::endl;
  std::cout << "End tangent: " << spline_entity.m_end_tangent << std::endl;

  std::cout << "Knots: " << spline_entity.m_knots.size() << std::endl;
  for (const auto& knot : spline_entity.m_knots)
    std::cout << "  " << knot << std::endl;

  std::cout << "Control points: " << number_of_poles << std::endl;
  for (size_t i = 0; i < number_of_poles; ++i)
    std::cout << "  " << spline_entity.m_control_points[i] << ", "
              << spline_entity.m_weights[i] << std::endl;

  std::cout << "Fit points: " << spline_entity.m_fit_points.size() << std::endl;
  for (const auto& point : spline_entity.m_fit_points)
    std::cout << "  " << point << std::endl;

  if (number_of_poles < 2)
    throw SGAL::Parse_error(filename(), "less than 2 control points!");

  size_t number_of_knots(0);
  size_t sum_of_mults(0);
  std::vector<size_t> mults;
  std::vector<double> knots;
  if (!spline_entity.m_knots.empty()) {
    double current_knot;
    size_t mult(0);
    for (auto knot : spline_entity.m_knots) {
      if (mult == 0) {
        mult = 1;
        current_knot = knot;
        continue;
      }
      if (current_knot == knot) {
        ++mult;
        continue;
      }
      knots.push_back(current_knot);
      mults.push_back(mult);
      sum_of_mults += mult;
      mult = 1;
      current_knot = knot;
    }
    knots.push_back(current_knot);
    mults.push_back(mult);
    if (! periodic) sum_of_mults += mult;

    number_of_knots = knots.size();
  }
  else {
    if (periodic) {
      if (number_of_poles < degree) degree = number_of_poles + 1;
      number_of_knots = number_of_poles + 1;
    }
    else {
      if (number_of_poles <= degree) degree = number_of_poles - 1;
      number_of_knots = number_of_poles - degree + 1;
    }
    // Uniformly distribute knots between 0..1 if not given
    knots.resize(number_of_knots);
    mults.resize(number_of_knots);
    for (int i = 0; i < number_of_knots; ++i) {
      knots[i] = i / (number_of_knots-1);
      mults[i] = 1;
    }
    if (periodic) sum_of_mults = number_of_knots - 1;
    else {
      mults.front() = mults.back() = degree + 1;
      sum_of_mults = number_of_knots + degree + degree;
    }
  }
  std::cout << "degree: " << degree << std::endl;
  std::cout << "sum_of_mults: " << sum_of_mults << std::endl;

  // The folowing check is redundant, given the current handling of weights.
  if (spline_entity.m_weights.size() != number_of_poles)
    throw SGAL::Parse_error(filename(),
                            "number of poles and weights mismatch!");

  // Check whether the number of poles matches the sum of mults
  if ((periodic && (sum_of_mults != number_of_poles)) ||
      (! periodic && ((sum_of_mults - degree - 1) != number_of_poles)))
    throw SGAL::Parse_error(filename(),
                            "number of poles and sum of mults mismatch");

  std::cout << "Knots: " << knots.size() << std::endl;
  for (const auto& knot : knots)
    std::cout << "  " << knot << std::endl;
  std::cout << "Mults: " << mults.size() << std::endl;
  for (const auto& mult : mults)
    std::cout << "  " << mult << std::endl;
}

//! \brief adds polylines provided in polyline entities.
void Dxf_parser::add_polylines(const Dxf_polyline_entity& polyline,
                               SGAL::Group* root)
{
  ++m_polylines_num;

  size_t num_primitives(1);

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

  // Count number of vertices:
  size_t size(polyline.m_vertex_entities.size());

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
  cit = std::transform(polyline.m_vertex_entities.begin(),
                       polyline.m_vertex_entities.end(), cit,
                       [&](const Dxf_vertex_entity& vertex)
                       {
                         auto& p = vertex.m_location;
                         return SGAL::Vector3f(p[0], p[1], 0);
                       });
  auto it_end = it;
  std::advance(it_end, polyline.m_vertex_entities.size());
  std::iota(it, it_end, i);
  i += polyline.m_vertex_entities.size();
  it = it_end;
  *it++ = -1;

  auto type = polyline.is_closed() ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_num_primitives(num_primitives);
}

//! \brief adds polylines provided in circle entities.
void Dxf_parser::add_polylines(const Dxf_circle_entity& circle,
                               SGAL::Group* root)
{
  auto* parser = circle.m_parser;

  size_t num_primitives(1);

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  if (! parser->m_appearance) {
    parser->m_appearance.reset(new SGAL::Appearance);
    parser->m_appearance->set_light_enable(false);
  }
  auto app = parser->m_appearance;
  SGAL_assertion(app);
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Allocate colors:
  auto color = circle.m_color;
  if (color == static_cast<int16_t>(By::BYLAYER)) {
    auto it = parser->m_layer_table.find(circle.m_layer);
    color = it->m_color;
  }

  auto ait = parser->m_color_arrays.find(color);
  if (ait == parser->m_color_arrays.end()) {
    auto* colors = new SGAL::Color_array(size_t(1));
    Shared_color_array shared_colors(colors);
    colors->add_to_scene(m_scene_graph);
    m_scene_graph->add_container(shared_colors);
    if (color >= s_palette.size()) color = 0;
    (*colors)[0] = s_palette[color];
    auto rc = parser->m_color_arrays.insert(std::make_pair(color, shared_colors));
    ait = rc.first;
  }
  auto shared_colors = ait->second;

  // Count number of vertices:
  const double pi = std::acos(-1);
  size_t num = m_arcs_num;
  auto delta_angle = 360.0 / num;
  size_t size(num);

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
  std::generate(coords->begin(), coords->end(),
                [&] () mutable
                {
                  auto angle = (delta_angle * i++) * pi / 180;
                  auto x = circle.m_center[0] + circle.m_radius * std::cos(angle);
                  auto y = circle.m_center[1] + circle.m_radius * std::sin(angle);
                  return SGAL::Vector3f(x, y, 0);
                });
  auto it_end = it;
  std::advance(it_end, size);
  std::iota(it, it_end, 0);
  it = it_end;
  *it++ = -1;

  auto type = SGAL::Geo_set::PT_LINE_LOOPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief adds polylines provided in arc entities.
void Dxf_parser::add_polylines(const Dxf_arc_entity& arc, SGAL::Group* root)
{
  auto* parser = arc.m_parser;

  size_t num_primitives(1);

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

  // Allocate colors:
  auto color = arc.m_color;
  if (color == static_cast<int16_t>(By::BYLAYER)) {
    auto it = parser->m_layer_table.find(arc.m_layer);
    color = it->m_color;
  }

  auto ait = parser->m_color_arrays.find(color);
  if (ait == parser->m_color_arrays.end()) {
    auto* colors = new SGAL::Color_array(size_t(1));
    Shared_color_array shared_colors(colors);
    colors->add_to_scene(m_scene_graph);
    m_scene_graph->add_container(shared_colors);
    if (color >= s_palette.size()) color = 0;
    (*colors)[0] = s_palette[color];
    auto rc = parser->m_color_arrays.insert(std::make_pair(color, shared_colors));
    ait = rc.first;
  }
  auto shared_colors = ait->second;

  // Count number of vertices:
  const double pi = std::acos(-1);
  auto diff_angle = arc.m_end_angle - arc.m_start_angle;
  if (diff_angle < 0.0) diff_angle += 360.0;
  size_t num = m_arcs_num * diff_angle / 360.0;
  if (num == 0) num = 1;
  auto delta_angle = diff_angle / num;
  size_t size(num+1);

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
  std::generate(coords->begin(), coords->end(),
                [&] () mutable
                {
                  auto angle = (arc.m_start_angle + delta_angle * i++) * pi / 180;
                  auto x = arc.m_center[0] + arc.m_radius * std::cos(angle);
                  auto y = arc.m_center[1] + arc.m_radius * std::sin(angle);
                  return SGAL::Vector3f(x, y, 0);
                });
  auto it_end = it;
  std::advance(it_end, size);
  std::iota(it, it_end, 0);
  it = it_end;
  *it++ = -1;

  auto type = SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

DXF_END_NAMESPACE
