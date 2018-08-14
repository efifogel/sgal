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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <list>
#include <numeric>
#include <limits>
#include <cmath>
#include <fstream>
#include <iterator>

#include <boost/shared_ptr.hpp>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/approximate_circular_arc.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Color_background.hpp"
#include "SGAL/Loader_errors.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Orientation.hpp"
#include "SGAL/remove_collinear_points.hpp"
#include "SGAL/is_convex.hpp"
#include "SGAL/construct_triangulation.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"
#include "SGAL/Epic_triangulation.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_block.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"
#include "dxf/Dxf_line_entity.hpp"
#include "dxf/Dxf_circle_entity.hpp"
#include "dxf/Dxf_arc_entity.hpp"
#include "dxf/Dxf_polyline_entity.hpp"
#include "dxf/Dxf_lwpolyline_entity.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_spline_entity.hpp"
#include "dxf/Dxf_insert_entity.hpp"
#include "dxf/Dxf_solid_entity.hpp"

DXF_BEGIN_NAMESPACE

// The extrusion direction of an entity is the normal vector to the plane
// containing the entity. The handling of the extrusion direction of an entity
// is degenerate.
// We only handle the cases where the extrusion direction is the default, that
// is (0,0,1), or (0,0,-1), which, essencially, implies that the local
// coordinate system is scaled by (-1, 1, 1) (or rotated 180 deg. about the
// y-axis).
// Theoretically, the extrusion direction can be any vector of length 1, say e.
// In all cases except for the above two, the entity should be rotated by a
// rotation that rotates the vector (0,0,1) to e.

// At this point we don't handle the following
// 1. SPLINE entities.
// 2. Patterns of HATCH entities.
//      The hatches are always filled up even if they shouldn't and instead
//      they should be rendred with a pattern, probably implemented by a vertex
//      shader.
// 3. DIMENSION entities.
// 4. Probably more.

//! \brief initializes the pallete.
void Dxf_parser::init_palette(const SGAL::String& file_name)
{
  std::ifstream t(file_name);
  //! \todo move the test for file existance out of here
  if (! t) return;
  std::string line;
  s_palette.clear();
  while (t) {
    size_t x;
    t >> std::hex >> x;
    auto r = ((x >> 16) & 0xFF) / 255.0;
    auto g = ((x >> 8) & 0xFF) / 255.0;
    auto b = ((x) & 0xFF) / 255.0;
    s_palette.push_back(SGAL::Vector3f(r, g, b));
  }
  t.close();
}

//! \brief processes all layers. Create a color array for each.
void Dxf_parser::process_layers()
{
  for (auto& layer : m_data->m_layer_table.m_entries) {
    Shared_color_array shared_colors;
    size_t color = layer.m_color;
    if (color != static_cast<size_t>(-1)) {
      auto* colors = new SGAL::Color_array(size_t(1));
      colors->add_to_scene(m_scene_graph);
      auto r = ((color >> 16) & 0xFF) / 255.0;
      auto g = ((color >> 8) & 0xFF) / 255.0;
      auto b = ((color) & 0xFF) / 255.0;
      (*colors)[0] = SGAL::Vector3f(r, g, b);
      shared_colors.reset(colors);
      m_scene_graph->add_container(shared_colors);
    }
    else {
      auto color_index = layer.m_color_index;
      if (color_index >= 0) {
        auto ait = m_color_arrays.find(color_index);
        if (ait == m_color_arrays.end()) {
          auto* colors = new SGAL::Color_array(size_t(1));
          colors->add_to_scene(m_scene_graph);
          if (color_index >= s_palette.size()) color_index = 0;
          (*colors)[0] = s_palette[color_index];
          shared_colors.reset(colors);
          m_scene_graph->add_container(shared_colors);
          m_color_arrays.insert(std::make_pair(color_index, shared_colors));
        }
        else shared_colors = ait->second;
      }
    }

    layer.m_color_array = shared_colors;
  }
}

//! \brief dispatches the processing of all entities.
void Dxf_parser::process_entities(std::vector<Dxf_base_entity*>& entities,
                                  SGAL::Group* root)
{
  for (auto* entity : entities) {
    if (auto* line = dynamic_cast<Dxf_line_entity*>(entity)) {
      process_line_entity(*line, root);
      continue;
    }
    if (auto* polyline = dynamic_cast<Dxf_polyline_entity*>(entity)) {
      process_polyline_entity(*polyline, root);
      continue;
    }
    if (auto* lwpolyline = dynamic_cast<Dxf_lwpolyline_entity*>(entity)) {
      process_lwpolyline_entity(*lwpolyline, root);
      continue;
    }
    if (auto* circle = dynamic_cast<Dxf_circle_entity*>(entity)) {
      process_circle_entity(*circle, root);
      continue;
    }
    if (auto* arc = dynamic_cast<Dxf_arc_entity*>(entity)) {
      process_arc_entity(*arc, root);
      continue;
    }
    if (auto* hatch = dynamic_cast<Dxf_hatch_entity*>(entity)) {
      process_hatch_entity(*hatch, root);
      continue;
    }
    if (auto* solid = dynamic_cast<Dxf_solid_entity*>(entity)) {
      process_solid_entity(*solid, root);
      continue;
    }
    if (auto* spline = dynamic_cast<Dxf_spline_entity*>(entity)) {
      process_spline_entity(*spline, root);
      continue;
    }
    if (auto* insert = dynamic_cast<Dxf_insert_entity*>(entity)) {
      process_insert_entity(*insert, root);
      continue;
    }
  }
}

//! \brief Obtain the color array of an entity.
Dxf_parser::Shared_color_array
Dxf_parser::get_color_array(int32_t color, int16_t color_index,
                            const SGAL::String& layer)
{
  Shared_color_array shared_colors;
  if (color != static_cast<int32_t>(-1)) {
    auto* colors = new SGAL::Color_array(size_t(1));
    colors->add_to_scene(m_scene_graph);
    auto r = ((color >> 16) & 0xFF) / 255.0;
    auto g = ((color >> 8) & 0xFF) / 255.0;
    auto b = ((color) & 0xFF) / 255.0;
    (*colors)[0] = SGAL::Vector3f(r, g, b);
    shared_colors.reset(colors);
    m_scene_graph->add_container(shared_colors);
    return shared_colors;
  }

  if (color_index == static_cast<int16_t>(By::BYLAYER)) {
    auto it = m_data->m_layer_table.find(layer);
    SGAL_assertion(it != m_data->m_layer_table.m_entries.end());
    shared_colors = it->m_color_array;
    SGAL_assertion(shared_colors);
    return shared_colors;
  }

  if (color_index < 0) return shared_colors;

  auto ait = m_color_arrays.find(color_index);
  if (ait == m_color_arrays.end()) {
    auto* colors = new SGAL::Color_array(size_t(1));
    colors->add_to_scene(m_scene_graph);
    if (color_index >= s_palette.size()) color = 0;
    (*colors)[0] = s_palette[color_index];
    shared_colors.reset(colors);
    m_scene_graph->add_container(shared_colors);
    m_color_arrays.insert(std::make_pair(color_index, shared_colors));
    return shared_colors;
  }

  return ait->second;
}

//! \brief obtains the lighting-disabled appearance.
// Also, assuming that this appearance is used for paper-space, we also disable
// the hidden-surface-removal, and draw the entities in the order they appear.
Dxf_parser::Shared_appearance Dxf_parser::get_fill_appearance()
{
  if (! m_fill_appearance) {
    m_fill_appearance.reset(new SGAL::Appearance);
    m_fill_appearance->add_to_scene(m_scene_graph);
    m_fill_appearance->set_light_enable(false);

    m_fill_appearance->set_depth_mask(false);
    m_fill_appearance->set_depth_enable(false);

    m_scene_graph->add_container(m_fill_appearance);
  }
  return m_fill_appearance;
}

//! \brief obtains the lighting-disabled appearance.
// Also, assuming that this appearance is used for paper-space, we also disable
// the hidden-surface-removal, and draw the entities in the order they appear.
Dxf_parser::Shared_appearance
Dxf_parser::get_pattern_appearance()
{
  Shared_appearance app(new SGAL::Appearance);
  app->add_to_scene(m_scene_graph);
  app->set_light_enable(false);

  app->set_depth_mask(false);
  app->set_depth_enable(false);

  //! \todo add a (vertex) shader that draws the pattern.

  m_scene_graph->add_container(app);

  m_pattern_appearances.push_back(app);

  return app;
}

//! \brief add a default background color
void Dxf_parser::add_background(SGAL::Group* root)
{
  typedef boost::shared_ptr<SGAL::Color_background>    Shared_color_background;

  Shared_color_background bg(new SGAL::Color_background);
  SGAL_assertion(bg);
  bg->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(bg);
  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  bg->set_color(dxf_conf->get_background_color());
  root->add_child(bg);
}

/*! Approximate a polyline with buldges in-place
 * \param[in,out] the sequence of points.
 * \param[in] bulges_begin the begin iterator of the bulges
 * \param[in] min_bulge the minimum bulge (used for approximating a circular
 *            arc).
 * \param[in] closed indicates whether the polyline is closed.
 */
template <typename BulgeInputIterator>
void approximate(std::list<SGAL::Vector2f>& points,
                 BulgeInputIterator bulges_begin,
                 double min_bulge, bool closed = true)
{
  typedef std::list<SGAL::Vector2f>             Point_list;
  typedef std::insert_iterator<Point_list>      Insert_point_iterator;

  auto bit = bulges_begin;
  auto vit2 = points.begin();
  auto vit1 = vit2;

  for (++vit2; vit2 != points.end(); ++vit2, ++bit) {
    SGAL::approximate_circular_arc(*vit1, *vit2, *bit, min_bulge,
                                   Insert_point_iterator(points, vit2));
    vit1 = vit2;
  }

  if (! closed) return;

  SGAL::approximate_circular_arc(*vit1, *(points.begin()), *bit, min_bulge,
                                 Insert_point_iterator(points, vit2));
}

//! \brief prints out hatch information.
void Dxf_parser::print_hatch_information(const Dxf_hatch_entity& hatch)
{
  std::cout << "style: " << hatch.m_style << std::endl;
  std::cout << "pattern name: " << hatch.m_pattern_name << std::endl;
  std::cout << "pattern type: " << hatch.m_pattern_type << std::endl;
  std::cout << "pattern angle: " << hatch.m_pattern_angle << std::endl;
  std::cout << "pattern scale: " << hatch.m_pattern_scale << std::endl;
  std::cout << "pattern double flag: "
            << hatch.m_pattern_double_flag << std::endl;
  std::cout << "seeds: " << hatch.m_seed_points.size() << std::endl;
  for (auto& p : hatch.m_seed_points) std::cout << "  " << p << std::endl;
  std::cout << "pattern data: " << hatch.m_pattern_line.size() << std::endl;
  for (auto& datum : hatch.m_pattern_line) {
    std::cout << "  Pattern line: " << std::endl;
    std::cout << "    Angle: " << datum.m_angle << std::endl;
    std::cout << "    Base point: " << datum.m_base_point[0] << ", "
              << datum.m_base_point[1] << std::endl;
    std::cout << "    Offset: " << datum.m_offset[0] << ", "
              << datum.m_offset[1] << std::endl;
    std::cout << "    lengths: " << datum.m_dash_lengths.size() << std::endl;
    for (auto& length : datum.m_dash_lengths)
      std::cout << "      " << length << std::endl;
  }
}

//! \brief add polylines.
void Dxf_parser::
process_polyline_boundaries
(const Dxf_hatch_entity& hatch,
 const std::list<Dxf_polyline_boundary_path*>& polylines,
 SGAL::Group* root)
{
  if (0 == polylines.size()) return;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(hatch.m_color, hatch.m_color_index, hatch.m_layer);
  if (! shared_colors) return;

  // Check whether mirroring is required
  bool mirror(false);
  if ((hatch.m_extrusion_direction[0] == 0) &&
      (hatch.m_extrusion_direction[1] == 0) &&
      (hatch.m_extrusion_direction[2] < 0))
    mirror = true;

  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto min_bulge = dxf_conf->get_min_bulge();

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Indexed_face_set>   Shared_indexed_face_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  auto app = (hatch.m_flags) ? get_fill_appearance() : get_pattern_appearance();
  shape->set_appearance(app);

  // Handle pattern
  /// if (! hatch.m_flags) {...}

  // Add geometry
  Shared_indexed_face_set ifs(new SGAL::Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ifs);
  shape->set_geometry(ifs);

  // Compute orientation
  // Note, reorienting the polygons to be counterclockwise is unnecessary,
  // because we triangulate the polygons anyhow.
  // polyline->m_orientation = SGAL::orientation(polyline->m_locations.begin(),
  //                                             polyline->m_locations.end());

  // Construct triangulations
  std::list<SGAL::Epic_triangulation> tris;
  for (auto* polyline : polylines) {
    if (polyline->m_locations.size() < 3) continue;
    std::list<SGAL::Vector2f> poly;
    if (polyline->m_has_bulge) {
      std::list<double> bulges;
      SGAL::remove_collinear_points(polyline->m_locations.begin(),
                                    polyline->m_locations.end(),
                                    polyline->m_bulges.begin(),
                                    polyline->m_bulges.end(), true,
                                    std::back_inserter(poly),
                                    std::back_inserter(bulges));
      if (poly.size() < 3) continue;

      approximate(poly, bulges.begin(), min_bulge);
      bulges.clear();
    }
    else {
      SGAL::remove_collinear_points(polyline->m_locations.begin(),
                                    polyline->m_locations.end(), true,
                                    std::back_inserter(poly));
      if (poly.size() < 3) continue;
    }
    tris.emplace_back();
    auto& tri = tris.back();
    SGAL::construct_triangulation(tri, poly.begin(), poly.end(), 0);
    SGAL::mark_domains(tri);
    poly.clear();
  }

  // Count number of primitives & vertices:
  size_t num_primitives(0);
  size_t num_indices(0);
  size_t size(0);
  for (auto& tri : tris) {
    size += tri.number_of_vertices();
    for (auto fit = tri.finite_faces_begin(); fit != tri.finite_faces_end();
         ++fit)
    {
      if (! fit->info().in_domain()) continue;

      ++num_primitives;
      num_indices += 4;
    }
  }
  // std::cout << "# primitives: " << num_primitives << std::endl;
  // std::cout << "# indices: " << num_indices << std::endl;
  // std::cout << "size: " << size << std::endl;

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ifs->get_coord_indices();
  indices.resize(num_indices);

  // Assign the coordinates & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (auto& tri : tris) {
    // Assign coordinates
    for (auto vit = tri.finite_vertices_begin();
         vit != tri.finite_vertices_end(); ++vit)
    {
      // std::cout << polyline->m_locations[it->info()] << std::endl;
      auto x = (mirror) ? -static_cast<SGAL::Float>(vit->point().x()) :
        static_cast<SGAL::Float>(vit->point().x());
      auto y = static_cast<SGAL::Float>(vit->point().y());
      (*cit++).set(x, y, 0);
    }

    // Assign indices:
    for (auto fit = tri.finite_faces_begin(); fit != tri.finite_faces_end();
         ++fit)
    {
      if (! fit->info().in_domain()) continue;
      *it++ = fit->vertex(0)->info() + i;
      *it++ = fit->vertex(1)->info() + i;
      *it++ = fit->vertex(2)->info() + i;
      *it++ = -1;
    }
    i += tri.number_of_vertices();
  }
  tris.clear();

  ifs->set_coord_array(shared_coords);
  ifs->set_color_array(shared_colors);
  ifs->set_num_primitives(num_primitives);
  ifs->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes a hatch entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_hatch_entity(const Dxf_hatch_entity& hatch,
                                      SGAL::Group* root)
{
  ++m_hatches_num;

  std::list<Dxf_polyline_boundary_path*> polylines;
  for (Dxf_base_boundary_path* path : hatch.m_boundary_paths) {
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline) {
      SGAL_warning_msg(true, "Unsupported boundary path!");
      continue;
    }

    // A polyline defined in a hatch entity must be closed!
    SGAL_assertion(polyline->m_is_closed);

    polylines.push_back(polyline);
  }

  process_polyline_boundaries(hatch, polylines, root);
}

//! \brief processes a spline entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_spline_entity(const Dxf_spline_entity& spline,
                                       SGAL::Group* root)
{
  //! \todo Add support for splines.
  return;

  ++m_splines_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(spline.m_color, spline.m_color_index, spline.m_layer);
  if (! shared_colors) return;

  size_t number_of_poles(spline.m_control_points.size());
  size_t degree(spline.m_degree);
  bool periodic(spline.is_periodic());
  bool planar(spline.is_planar());

  std::cout << "Normal: " << spline.m_normal << std::endl;
  std::cout << "Flags: " << spline.m_flags << std::endl;
  if (spline.m_flags & Dxf_spline_entity::CLOSED)
    std::cout << "  CLOSED" << std::endl;
  if (spline.m_flags & Dxf_spline_entity::PERIODIC)
    std::cout << "  PERIODIC" << std::endl;
  if (spline.m_flags & Dxf_spline_entity::RATIONAL)
    std::cout << "  RATIONAL" << std::endl;
  if (spline.m_flags & Dxf_spline_entity::PLANAR)
    std::cout << "  PLANAR" << std::endl;
  if (spline.m_flags & Dxf_spline_entity::LINEAR)
    std::cout << "  LINEAR" << std::endl;
  std::cout << "Degree: " << spline.m_degree << std::endl;
  std::cout << "Knot tolerance: " << spline.m_knot_tolerance << std::endl;
  std::cout << "Control Point tolerance: "
            << spline.m_control_point_tolerance << std::endl;
  std::cout << "Fit tolerance: " << spline.m_fit_tolerance << std::endl;
  std::cout << "Start tangent: " << spline.m_start_tangent[0] << ", "
            << spline.m_start_tangent[1] << ", "
            << spline.m_start_tangent[2] << std::endl;
  std::cout << "End tangent: " << spline.m_end_tangent << std::endl;

  std::cout << "Knots: " << spline.m_knots.size() << std::endl;
  for (const auto& knot : spline.m_knots)
    std::cout << "  " << knot << std::endl;

  std::cout << "Control points: " << number_of_poles << std::endl;
  for (size_t i = 0; i < number_of_poles; ++i)
    std::cout << "  " << spline.m_control_points[i] << ", "
              << spline.m_weights[i] << std::endl;

  std::cout << "Fit points: " << spline.m_fit_points.size() << std::endl;
  for (const auto& point : spline.m_fit_points)
    std::cout << "  " << point << std::endl;

  if (number_of_poles < 2)
    throw SGAL::Parse_error(filename(), "less than 2 control points!");

  size_t number_of_knots(0);
  size_t sum_of_mults(0);
  std::vector<size_t> mults;
  std::vector<double> knots;
  if (!spline.m_knots.empty()) {
    double current_knot;
    size_t mult(0);
    for (auto knot : spline.m_knots) {
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
  if (spline.m_weights.size() != number_of_poles)
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

//! \brief processes a line entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_line_entity(const Dxf_line_entity& line,
                                     SGAL::Group* root)
{
  ++m_lines_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(line.m_color, line.m_color_index, line.m_layer);
  if (! shared_colors) return;

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
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Allocate vertices:
  size_t size(2);
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size+1);

  // Check whether mirroring is required
  bool mirror(false);
  if ((line.m_extrusion_direction[0] == 0) &&
      (line.m_extrusion_direction[1] == 0) &&
      (line.m_extrusion_direction[2] < 0))
    mirror = true;

  // Assign the vertices & indices:
  auto cit = coords->begin();
  auto x1 = (mirror) ? -(line.m_start[0]) : line.m_start[0];
  *cit++ = SGAL::Vector3f(x1, line.m_start[1], line.m_start[2]);
  auto x2 = (mirror) ? -(line.m_end[0]) : line.m_end[0];
  *cit++ = SGAL::Vector3f(x2, line.m_end[1], line.m_end[2]);

  auto it = indices.begin();
  *it++ = 0;
  *it++ = 1;
  *it = -1;

  ils->set_primitive_type(SGAL::Geo_set::PT_LINES);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(1);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes a polyline entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_polyline_entity(const Dxf_polyline_entity& polyline,
                                         SGAL::Group* root)
{
  ++m_polylines_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(polyline.m_color, polyline.m_color_index, polyline.m_layer);
  if (! shared_colors) return;

  auto closed = polyline.is_closed();
  auto has_bulge = polyline.has_bulge();

  if ((polyline.m_vertex_entities.size() < 2) ||
      (closed && !has_bulge && (polyline.m_vertex_entities.size() < 3))) return;

  //! \todo Add support for 3D.
  if (polyline.is_3d()) return;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto min_bulge = dxf_conf->get_min_bulge();

  // Check whether mirroring is required
  bool mirror(false);
  if ((polyline.m_extrusion_direction[0] == 0) &&
      (polyline.m_extrusion_direction[1] == 0) &&
      (polyline.m_extrusion_direction[2] < 0))
    mirror = true;

  std::list<SGAL::Vector2f> poly;
  if (has_bulge) {
    std::list<double> bulges;
    // Remove duplicate and collinear points:
    SGAL::remove_collinear_points(polyline.points_begin(),
                                  polyline.points_end(),
                                  polyline.bulges_begin(),
                                  polyline.bulges_end(), closed,
                                  std::back_inserter(poly),
                                  std::back_inserter(bulges));
    if ((poly.size() < 2) || (closed && ! has_bulge && (poly.size() < 3)))
      return;

    // Approximate bulges:
    approximate(poly, bulges.begin(), min_bulge, closed);
    bulges.clear();
  }
  else {
    // Remove duplicate and collinear points:
    SGAL::remove_collinear_points(polyline.points_begin(),
                                  polyline.points_end(), closed,
                                  std::back_inserter(poly));
    if ((poly.size() < 2) || (closed && (poly.size() < 3))) return;
  }
  // for (auto& p : poly) std::cout << p << std::endl;

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Count number of vertices:
  size_t size(poly.size());
  size_t num_primitives(1);

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size + num_primitives);

  // Assign the vertices & indices:
  std::transform(poly.begin(), poly.end(), coords->begin(),
                 [&](const SGAL::Vector2f& v)
                 {
                   auto x = (mirror) ? -v[0] : v[0];
                   return SGAL::Vector3f(x, v[1], 0);
                 });
  poly.clear();
  auto it = indices.begin();
  std::advance(it, size);
  std::iota(indices.begin(), it, 0);
  *it++ = -1;

  auto type = closed ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes a light weight polyline entity.
void Dxf_parser::process_lwpolyline_entity(const Dxf_lwpolyline_entity& polyline,
                                           SGAL::Group* root)
{
  ++m_lwpolylines_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(polyline.m_color, polyline.m_color_index, polyline.m_layer);
  if (! shared_colors) return;

  auto closed = polyline.is_closed();
  auto has_bulge = polyline.has_bulge();

  if ((polyline.m_vertices.size() < 2) ||
      (closed && ! has_bulge && (polyline.m_vertices.size() < 3))) return;

  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);
  auto min_bulge = dxf_conf->get_min_bulge();

  // Check whether mirroring is required
  bool mirror(false);
  if ((polyline.m_extrusion_direction[0] == 0) &&
      (polyline.m_extrusion_direction[1] == 0) &&
      (polyline.m_extrusion_direction[2] < 0))
    mirror = true;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  std::list<SGAL::Vector2f> poly;
  if (has_bulge) {
    std::list<double> bulges;
    // Remove duplicate and collinear points:
    SGAL::remove_collinear_points(polyline.m_vertices.begin(),
                                  polyline.m_vertices.end(),
                                  polyline.m_bulges.begin(),
                                  polyline.m_bulges.end(), closed,
                                  std::back_inserter(poly),
                                  std::back_inserter(bulges));
    if ((poly.size() < 2) || (closed && !has_bulge && (poly.size() < 3))) return;

    // Approximate bulges:
    approximate(poly, bulges.begin(), min_bulge, closed);
    bulges.clear();
  }
  else {
    // Remove duplicate and collinear points:
    SGAL::remove_collinear_points(polyline.m_vertices.begin(),
                                  polyline.m_vertices.end(), closed,
                                  std::back_inserter(poly));
    if ((poly.size() < 2) || (closed && (poly.size() < 3))) return;
  }

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet:
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  m_scene_graph->add_container(ils);
  ils->add_to_scene(m_scene_graph);
  shape->set_geometry(ils);

  // Count number of vertices:
  size_t size(poly.size());
  size_t num_primitives(1);

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ils->get_coord_indices();
  indices.resize(size + num_primitives);

  // Assign the vertices & indices:
  std::transform(poly.begin(), poly.end(), coords->begin(),
                 [&](const SGAL::Vector2f& v)
                 {
                   auto x = (mirror) ? -v[0] : v[0];
                   return SGAL::Vector3f(x, v[1], 0);
                 });
  poly.clear();
  auto it = indices.begin();
  std::advance(it, size);
  std::iota(indices.begin(), it, 0);
  *it++ = -1;

  auto type = closed ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes a circle entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_circle_entity(const Dxf_circle_entity& circle,
                                       SGAL::Group* root)
{
  ++m_circles_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(circle.m_color, circle.m_color_index, circle.m_layer);
  if (! shared_colors) return;

  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);

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
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Count number of vertices:
  const double pi = std::acos(-1);
  auto num = dxf_conf->get_refinement_arcs_num();
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

  // Check whether mirroring is required
  bool mirror(false);
  if ((circle.m_extrusion_direction[0] == 0) &&
      (circle.m_extrusion_direction[1] == 0) &&
      (circle.m_extrusion_direction[2] < 0))
    mirror = true;

  // Assign the vertices & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  std::generate(coords->begin(), coords->end(),
                [&] () mutable
                {
                  auto angle = (delta_angle * i++) * pi / 180;
                  auto x = circle.m_center[0] + circle.m_radius * std::cos(angle);
                  if (mirror) x = -x;
                  auto y = circle.m_center[1] + circle.m_radius * std::sin(angle);
                  return SGAL::Vector3f(x, y, 0);
                });
  auto it_start = it;
  std::advance(it, size);
  std::iota(it_start, it, 0);
  *it++ = -1;

  auto type = SGAL::Geo_set::PT_LINE_LOOPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes an arc entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_arc_entity(const Dxf_arc_entity& arc,
                                    SGAL::Group* root)
{
  ++m_arcs_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(arc.m_color, arc.m_color_index, arc.m_layer);
  if (! shared_colors) return;

  auto conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto dxf_conf = conf->get_dxf_configuration();
  SGAL_assertion(dxf_conf);

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
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Count number of vertices:
  auto diff_angle = arc.m_end_angle - arc.m_start_angle;
  if (diff_angle < 0.0) diff_angle += 360.0;
  size_t num = dxf_conf->get_refinement_arcs_num() * diff_angle / 360.0;
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

  // Check whether mirroring is required
  bool mirror(false);
  if ((arc.m_extrusion_direction[0] == 0) &&
      (arc.m_extrusion_direction[1] == 0) &&
      (arc.m_extrusion_direction[2] < 0))
    mirror = true;

  // Assign the vertices & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  const double pi = std::acos(-1);
  std::generate(coords->begin(), coords->end(),
                [&] () mutable
                {
                  auto angle = (arc.m_start_angle + delta_angle * i++) * pi / 180;
                  auto x = arc.m_center[0] + arc.m_radius * std::cos(angle);
                  auto y = arc.m_center[1] + arc.m_radius * std::sin(angle);
                  if (mirror) x = -x;
                  return SGAL::Vector3f(x, y, 0);
                });
  auto it_start = it;
  std::advance(it, size);
  std::iota(it_start, it, 0);
  *it++ = -1;

  auto type = SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
  ils->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes all insert entities.
void Dxf_parser::process_insert_entity(const Dxf_insert_entity& insert,
                                       SGAL::Group* root)
{
  ++m_inserts_num;

  auto it = std::find_if(m_data->m_blocks.begin(), m_data->m_blocks.end(),
                         [&](Dxf_block& block)
                         { return insert.m_name == block.m_name; });
  auto& block = *it;
  if (! block.m_group) {
    auto* group = new SGAL::Group;
    group->add_to_scene(m_scene_graph);
    process_entities(block.m_entities, group);
    block.m_group.reset(group);
    m_scene_graph->add_container(block.m_group);
  }

  typedef boost::shared_ptr<SGAL::Transform>    Shared_transform;
  Shared_transform transform(new SGAL::Transform);
  transform->add_to_scene(m_scene_graph);
  transform->add_child(block.m_group);
  m_scene_graph->add_container(transform);

  transform->set_translation(insert.m_location[0], insert.m_location[1],
                             insert.m_location[2]);
  transform->set_scale(insert.m_x_scale_factor,
                       insert.m_y_scale_factor,
                       insert.m_z_scale_factor);
  transform->set_rotation(0, 0, 1, SGAL::deg2rad(insert.m_rotation));

  // Check whether mirroring is required
  bool mirror(false);
  if ((insert.m_extrusion_direction[0] == 0) &&
      (insert.m_extrusion_direction[1] == 0) &&
      (insert.m_extrusion_direction[2] < 0))
    mirror = true;

  if (mirror) {
    const auto& mat = transform->get_matrix();
    SGAL::Matrix4f extrusion_mat;
    extrusion_mat.make_scale(-1, 1, 1);
    // extrusion_mat.make_rot(0, 1, 0, 3.14);
    extrusion_mat.pre_mult(mat);
    transform->set_matrix(extrusion_mat);
  }

  root->add_child(transform);
}

//! \brief processes a solid entity.
void Dxf_parser::process_solid_entity(const Dxf_solid_entity& solid,
                                      SGAL::Group* root)
{
  ++m_solids_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(solid.m_color, solid.m_color_index, solid.m_layer);
  if (! shared_colors) return;

  // Check whether mirroring is required
  bool mirror(false);
  if ((solid.m_extrusion_direction[0] == 0) &&
      (solid.m_extrusion_direction[1] == 0) &&
      (solid.m_extrusion_direction[2] < 0))
    mirror = true;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Indexed_face_set>   Shared_indexed_face_set;
  typedef boost::shared_ptr<SGAL::Coord_array_3d>     Shared_coord_array_3d;

  // Add Shape
  Shared_shape shape(new SGAL::Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add geometry
  Shared_indexed_face_set ifs(new SGAL::Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ifs);
  shape->set_geometry(ifs);

  // We assume that the solid is convex and the order of points is 1, 2, 4, 3
  // If this assumption turns out to be erroneous, then compute the convex hull,
  // the triangulate.

  // Count number of primitives & vertices:
  size_t num_primitives(1);
  size_t num_indices(5);
  size_t size(4);

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = ifs->get_coord_indices();
  indices.resize(num_indices);

  // Assign the coordinates & indices:
  auto cit = coords->begin();
  *cit++ = SGAL::Vector3f(solid.m_corner1[0], solid.m_corner1[1], solid.m_corner1[2]);
  *cit++ = SGAL::Vector3f(solid.m_corner2[0], solid.m_corner2[1], solid.m_corner2[2]);
  *cit++ = SGAL::Vector3f(solid.m_corner4[0], solid.m_corner4[1], solid.m_corner4[2]);
  *cit++ = SGAL::Vector3f(solid.m_corner3[0], solid.m_corner3[1], solid.m_corner3[2]);

  auto it = indices.begin();
  *it++ = 0;
  *it++ = 1;
  *it++ = 2;
  *it++ = 3;
  *it++ = -1;

  ifs->set_coord_array(shared_coords);
  ifs->set_color_array(shared_colors);
  ifs->set_num_primitives(num_primitives);
  ifs->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

DXF_END_NAMESPACE
