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
#include <fstream>

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
#include "SGAL/is_convex.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Face_nesting_level.hpp"
#include "SGAL/construct_triangulation.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_line_entity.hpp"
#include "dxf/Dxf_circle_entity.hpp"
#include "dxf/Dxf_arc_entity.hpp"
#include "dxf/Dxf_polyline_entity.hpp"
#include "dxf/Dxf_hatch_entity.hpp"
#include "dxf/Dxf_spline_entity.hpp"
#include "dxf/Dxf_insert_entity.hpp"
#include "dxf/Dxf_block.hpp"
#include "dxf/Dxf_polyline_boundary_path.hpp"

DXF_BEGIN_NAMESPACE

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
  for (auto& layer : m_layer_table.m_entries) {
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
    auto it = m_layer_table.find(layer);
    SGAL_assertion(it != m_layer_table.m_entries.end());
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
  if (conf) {
    auto dxf_conf = conf->get_dxf_configuration();
    if (dxf_conf) bg->set_color(dxf_conf->get_background_color());
  }
  root->add_child(bg);
}

//! \brief add polylines that have bulge factors.
void Dxf_parser::
add_polylines_with_bulge(const Dxf_hatch_entity& hatch,
                         const std::list<Dxf_polyline_boundary_path*>& polylines,
                         SGAL::Group* root)
{
  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(hatch.m_color, hatch.m_color_index, hatch.m_layer);
  if (! shared_colors) return;

  static const double min_bulge(0.1);

  size_t num_primitives(polylines.size());
  if (0 == num_primitives) return;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Indexed_line_set>   Shared_indexed_line_set;
  typedef boost::shared_ptr<SGAL::Indexed_face_set>   Shared_indexed_face_set;
  typedef boost::shared_ptr<SGAL::Geo_set>            Shared_geo_set;
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

  // Add Geometry
  Shared_geo_set geom;

  if (hatch.m_flags) {
    Shared_indexed_face_set ifs(new SGAL::Indexed_face_set);
    SGAL_assertion(ifs);
    ifs->add_to_scene(m_scene_graph);
    m_scene_graph->add_container(ifs);
    ifs->set_primitive_type(SGAL::Geo_set::PT_POLYGONS);
    geom = ifs;
  }
  else {
    Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
    SGAL_assertion(ils);
    ils->add_to_scene(m_scene_graph);
    m_scene_graph->add_container(ils);
    ils->set_primitive_type(SGAL::Geo_set::PT_LINE_LOOPS);
    geom = ils;
  }
  shape->set_geometry(geom);

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

    SGAL::Vector2f v1((*v)[0], (*v)[1]);
    SGAL::Vector2f v2((*vit_first)[0], (*vit_first)[1]);
    SGAL::approximate_circular_arc(v1, v2, b, min_bulge,
                                   std::back_inserter(segs));
  }

  // Count the number of segments and allocate the vertices and indices
  size_t size(0);
  for (const auto& segs : polylines_segs) size += segs.size();
  coords->resize(size);
  auto& indices = geom->get_coord_indices();
  indices.resize(size + num_primitives);

  // Assign the vertices & indices:
  auto it = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (const auto& segs : polylines_segs) {
    cit = std::transform(segs.begin(), segs.end(), cit,
                         [&](const SGAL::Vector2f& p)
                         { return SGAL::Vector3f(p[0], p[1], 0); });
    auto it_start = it;
    std::advance(it, segs.size());
    std::iota(it_start, it, i);
    i += segs.size();
    *it++ = -1;
  }

  geom->set_coord_array(shared_coords);
  geom->set_color_array(shared_colors);
  geom->set_num_primitives(num_primitives);
  geom->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief add polylines.
void Dxf_parser::
add_polylines(const Dxf_hatch_entity& hatch,
              const std::list<Dxf_polyline_boundary_path*>& polylines,
              SGAL::Group* root)
{
  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(hatch.m_color, hatch.m_color_index, hatch.m_layer);
  if (! shared_colors) return;

  if (0 == polylines.size()) return;

  typedef boost::shared_ptr<SGAL::Shape>              Shared_shape;
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
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
  if (! hatch.m_flags) {

#if 0
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
#endif
  }

  // Add geometry
  Shared_indexed_face_set geom(new SGAL::Indexed_face_set);
  shape->set_geometry(geom);

  // Construct triangulations
  typedef SGAL::Inexact_kernel                                          Kernel;
  typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, Kernel>   VB;
  typedef CGAL::Triangulation_face_base_with_info_2<SGAL::Face_nesting_level,
                                                    Kernel>             FBI;
  typedef CGAL::Constrained_triangulation_face_base_2<Kernel, FBI>      FB;
  typedef CGAL::Triangulation_data_structure_2<VB, FB>                  TDS;
  typedef CGAL::No_intersection_tag                                     Itag;
  // typedef CGAL::Exact_predicates_tag                                 Itag;
  typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>
    Triangulation;
  std::vector<Triangulation> tris(polylines.size());
  auto tit = tris.begin();
  for (auto* polyline : polylines) {
    if (! SGAL::is_convex(polyline->m_locations.begin(),
                          polyline->m_locations.end()))
    {
      SGAL::construct_triangulation(*tit, polyline->m_locations.begin(),
                                    polyline->m_locations.end(), 0);
      SGAL::mark_domains(*tit);
    }
    ++tit;
    break;
  }

  // Count number of primitives & vertices:
  size_t num_primitives(0);
  size_t num_indices(0);
  size_t size(0);
  tit = tris.begin();
  for (auto* polyline : polylines) {
    if (0 == tit->number_of_faces()) {
      size += polyline->m_locations.size();
      ++num_primitives;
      num_indices += size + 1;
    }
    else {
      size += tit->number_of_vertices();
      for (auto fit = tit->finite_faces_begin(); fit != tit->finite_faces_end();
           ++fit)
        if (fit->info().in_domain()) {
          ++num_primitives;
          num_indices += 4;
        }
    }
    ++tit;
    break;
  }

  // Allocate vertices:
  auto* coords = new SGAL::Coord_array_3d(size);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(shared_coords);

  // Allocate indices:
  auto& indices = geom->get_coord_indices();
  indices.resize(num_indices);

  // Check whether mirroring is required
  bool mirror(false);
  if ((hatch.m_extrusion_direction[0] == 0) &&
      (hatch.m_extrusion_direction[1] == 0) &&
      (hatch.m_extrusion_direction[2] < 0))
    mirror = true;

  // Assign the coordinates & indices:
  tit = tris.begin();
  auto iit = indices.begin();
  auto cit = coords->begin();
  size_t i(0);
  for (auto* polyline : polylines) {
    if (0 == tit->number_of_faces()) {
      // Assign coordinates
      cit = std::transform(polyline->m_locations.begin(),
                           polyline->m_locations.end(), cit,
                           [&](const SGAL::Vector2f& p)
                           {
                             auto x = (mirror) ? -p[0] : p[0];
                             return SGAL::Vector3f(x, p[1], 0);
                           });

      // Assign indices:
      auto iit_start = iit;
      std::advance(iit, polyline->m_locations.size());
      std::iota(iit_start, iit, i);
      *iit++ = -1;
      i += polyline->m_locations.size();
    }
    else {
      // Assign coordinates
      for (auto it = tit->finite_vertices_begin();
           it != tit->finite_vertices_end(); ++it)
      {
        auto x = (mirror) ? -static_cast<SGAL::Float>(it->point().x()) :
          static_cast<SGAL::Float>(it->point().x());
        auto y = static_cast<SGAL::Float>(it->point().y());
        (*cit++).set(x, y, 0);
      }

      // Assign indices:
      for (auto fit = tit->finite_faces_begin(); fit != tit->finite_faces_end();
           ++fit)
      {
        if (! fit->info().in_domain()) continue;
        *iit++ = fit->vertex(0)->info() + i;
        *iit++ = fit->vertex(1)->info() + i;
        *iit++ = fit->vertex(2)->info() + i;
        *iit++ = -1;
      }
      i += tit->number_of_vertices();
    }
    break;
  }
  tris.clear();

  geom->set_coord_array(shared_coords);
  geom->set_color_array(shared_colors);
  geom->set_num_primitives(num_primitives);
  geom->set_color_attachment(SGAL::Geo_set::AT_PER_MESH);
}

//! \brief processes a hatch entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_hatch_entity(const Dxf_hatch_entity& hatch,
                                      SGAL::Group* root)
{
  ++m_hatches_num;

  std::list<Dxf_polyline_boundary_path*> polylines;
  std::list<Dxf_polyline_boundary_path*> polylines_with_bulge;
  for (Dxf_base_boundary_path* path : hatch.m_boundary_paths) {
    if (! polylines.empty()) break;
    // Observe that the following statement obtains a non-const polyline.
    // A non-const struct is required to repair the polyline below.
    // The repairing, probably, should be taken out of here.
    auto* polyline = dynamic_cast<Dxf_polyline_boundary_path*>(path);
    if (! polyline) {
      SGAL_warning_msg(true, "Unsupported boundary path!");
      continue;
    }

    // A polyline defined in a hatch entity must be closed!
    SGAL_assertion(polyline->m_is_closed);

    // // Remove repeated points.
    // auto end =
    //   std::unique(polyline->m_locations.begin(), polyline->m_locations.end());
    // polyline->m_locations.erase(end, polyline->m_locations.end());
    // auto last = std::prev(polyline->m_locations.end());
    // if (polyline->m_locations.front() == *last)
    //   polyline->m_locations.erase(last);

    // Remove collinear coordinates:
    SGAL::simplify(polyline->m_locations);

    // Bail out if there are insufficient points.
    if (polyline->m_locations.size() < 3) return;

    // Orient counterclockwise
    auto orient = SGAL::orientation(polyline->m_locations.begin(),
                                    polyline->m_locations.end());
    if (SGAL::Orientation::Clockwise == orient)
      std::reverse(polyline->m_locations.begin(), polyline->m_locations.end());

    if (polyline->m_has_bulge) polylines_with_bulge.push_back(polyline);
    else polylines.push_back(polyline);
  }

  add_polylines(hatch, polylines, root);
  add_polylines_with_bulge(hatch, polylines_with_bulge, root);
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

//! \brief processes a spline entity. Construct Indexed_line_set as necessary.
void Dxf_parser::process_spline_entity(const Dxf_spline_entity& spline,
                                       SGAL::Group* root)
{
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

//! \brief processes a light weight polyline entity.
void Dxf_parser::process_lwpolyline_entity(const Dxf_lwpolyline_entity& polyline,
                                           SGAL::Group* root)
{
  ++m_lwpolylines_num;

  // Obtain the color array:
  Shared_color_array shared_colors =
    get_color_array(polyline.m_color, polyline.m_color_index, polyline.m_layer);
  if (! shared_colors) return;

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
  auto app = get_fill_appearance();
  shape->set_appearance(app);

  // Add IndexedLineSet
  Shared_indexed_line_set ils(new SGAL::Indexed_line_set);
  SGAL_assertion(ils);
  ils->add_to_scene(m_scene_graph);
  m_scene_graph->add_container(ils);
  shape->set_geometry(ils);

  // Count number of vertices:
  size_t size(polyline.m_vertices.size());

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
  cit = std::transform(polyline.m_vertices.begin(),
                       polyline.m_vertices.end(), cit,
                       [&](const SGAL::Vector2f& v)
                       { return SGAL::Vector3f(v[0], v[1], 0); });
  auto it_start = it;
  std::advance(it, polyline.m_vertices.size());
  std::iota(it_start, it, i);
  i += polyline.m_vertices.size();
  *it++ = -1;

  auto type = polyline.is_closed() ?
    SGAL::Geo_set::PT_LINE_LOOPS : SGAL::Geo_set::PT_LINE_STRIPS;
  ils->set_primitive_type(type);
  ils->set_coord_array(shared_coords);
  ils->set_color_array(shared_colors);
  ils->set_num_primitives(num_primitives);
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
  auto app = get_fill_appearance();
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
  auto it_start = it;
  std::advance(it, polyline.m_vertex_entities.size());
  std::iota(it_start, it, i);
  i += polyline.m_vertex_entities.size();
  *it++ = -1;

  auto type = polyline.is_closed() ?
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
  size_t num = m_arcs_refinement_num;
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
  auto diff_angle = arc.m_end_angle - arc.m_start_angle;
  if (diff_angle < 0.0) diff_angle += 360.0;
  size_t num = m_arcs_refinement_num * diff_angle / 360.0;
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

  auto it = std::find_if(m_blocks.begin(), m_blocks.end(),
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

  root->add_child(transform);
}

DXF_END_NAMESPACE
