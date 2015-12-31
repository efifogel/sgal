// Copyright (c) 2004 Israel.
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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/File_format_3d.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Loader::Loader() : m_multiple_shapes(false) {}

//! \brief loads a scene graph from an input stream.
Loader::Return_code Loader::load_stl(std::istream& stl_stream, size_t size,
                                     Scene_graph* sg, bool force)
{
  char str[81];
  stl_stream.read(str, 80);
  std::string title(str);
  if (!stl_stream) {
    throw Read_error(m_filename);
    return FAILURE;
  }

  if (force || (0 != title.compare(0, 5, "solid"))) {
    Vector3f color;
    auto pos = title.find("COLOR=");
    if ((pos != std::string::npos) && (pos < 70)) {
      pos += 6;
      Float red = static_cast<Float>(static_cast<Uchar>(str[pos])) / 255.0f;
      Float green = static_cast<Float>(static_cast<Uchar>(str[pos+1])) / 255.0f;
      Float blue = static_cast<Float>(static_cast<Uchar>(str[pos+2])) / 255.0f;
      Float alpha = static_cast<Float>(static_cast<Uchar>(str[pos+3])) / 255.0f;
      color.set(red, green, blue);
    }
    auto rc = read_stl(stl_stream, size, sg, color);
    if (rc < 0) return rc;
    return SUCCESS;
  }
  return RETRY;
}

//! \brief loads a scene graph from an stl file.
Loader::Return_code Loader::load_stl(char* data, size_t size,
                                     Scene_graph* sg, bool force)
{
  boost::interprocess::bufferstream stl_stream(data, size);
  if (!stl_stream.good()) {
    throw Overflow_error(m_filename);
    return FAILURE;
  }
  return load_stl(stl_stream, size, sg, force);
}

//! \brief loads a scene graph from an stl file.
Loader::Return_code Loader::load_stl(const char* filename, Scene_graph* sg,
                                     bool force)
{
  // Record the filename
  m_filename = filename ? filename : std::string();

  std::ifstream stl_stream(filename, std::ios::in|std::ios::binary);
  if (!stl_stream.good()) {
    throw Open_file_error(m_filename);
    return FAILURE;
  }
  stl_stream.seekg(0, stl_stream.end);
  size_t size = stl_stream.tellg();
  stl_stream.seekg(0, stl_stream.beg);

  auto rc = load_stl(stl_stream, size, sg, force);
  stl_stream.close();
  return rc;
}

//! \brief load a scene graph from a stream.
Loader::Return_code Loader::load(std::istream& src_stream, Scene_graph* sg)
{
  Vrml_scanner scanner(&src_stream);
  // scanner.set_debug(1);

  // Parse & export:
  bool maybe_binary_stl(false);
  Vrml_parser parser(scanner, sg, maybe_binary_stl);
  if (parser.parse()) {
    if (maybe_binary_stl) return RETRY;
    throw Parse_error(m_filename);
    return FAILURE;
  }
  return SUCCESS;
}

//! \brief loads a scene graph from a buffer.
Loader::Return_code Loader::load(char* data, size_t size,
                                 const char* filename, Scene_graph* sg)
{
  // Try to determine the file type from its extension.
  if (filename) {
    std::string file_extension = boost::filesystem::extension(filename);
    if (boost::iequals(file_extension, ".stl")) {
      auto rc = load_stl(data, size, sg);
      if (rc <= 0) return rc;
    }
  }
  return load(data, size, sg);
}

//! \brief loads a scene graph from a buffer.
Loader::Return_code Loader::load(char* data, size_t size, Scene_graph* sg)
{
  boost::interprocess::bufferstream src_stream(data, size);
  if (!src_stream.good()) {
    throw Overflow_error(m_filename);
    return FAILURE;
  }

  auto rc = load(src_stream, sg);
  if (rc < 0) {
    src_stream.clear();
    return rc;
  }

  src_stream.clear();

  if (rc == RETRY) rc = load_stl(data, size, sg, true);
  return rc;
}

//! \brief loads a scene graph from a file.
Loader::Return_code Loader::load(const char* filename, Scene_graph* sg)
{
  // Record the filename
  m_filename = filename ? filename : std::string();

  // Try to determine the file type from its extension.
  SGAL_assertion(filename);
  std::string file_extension = boost::filesystem::extension(filename);
  if (boost::iequals(file_extension, ".stl")) {
    auto rc = load_stl(filename, sg);
    if (rc <= 0) return rc;
  }

  // Open source file:
  std::ifstream src_stream(filename);
  if (!src_stream.good()) {
    throw Open_file_error(m_filename);
    return FAILURE;
  }

  auto rc = load(src_stream, sg);
  if (rc < 0) {
    src_stream.close();
    return rc;
  }

  src_stream.close();

  if (rc == RETRY) rc = load_stl(filename, sg, true);
  return rc;
}

//! \bried read a traingle (1 normal and 3 vertices)
Loader::Return_code Loader::read_triangle(std::istream& stl_stream,
                                          Triangle& triangle)
{
  Float x, y, z;

  // Read normal
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));

  // Read vertex 1:
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));
  triangle.v0.set(x, y, z);

  // Read vertex 2:
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));
  triangle.v1.set(x, y, z);

  // Read vertex 3:
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));
  triangle.v2.set(x, y, z);

  stl_stream.read((char*)&(triangle.spacer), sizeof(unsigned short));
  //std::cout << std::hex << spacer << std::endl;

  if (!stl_stream) {
    throw Read_error(m_filename);
    return FAILURE;
  }
  return SUCCESS;
}

//! \brief computes a new Indexed Face Set container.
Loader::Shared_indexed_face_set
Loader::compute_ifs(size_t count,
                    std::list<Triangle>::const_iterator begin,
                    std::list<Triangle>::const_iterator end)
{
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(count);
  Uint num_vertices = 3 * count;
  Coord_array_3d* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  ifs->set_coord_array(shared_coords);
  auto& indices = ifs->empty_triangle_coord_indices();
  indices.resize(count);

  size_t i(0);
  Uint coord_index(0);
  for (auto it = begin; it != end; ++it) {
    auto& triangle = *it;
    (*coords)[coord_index].set(triangle.v0);
    indices[i][0] = coord_index++;
    (*coords)[coord_index].set(triangle.v1);
    indices[i][1] = coord_index++;
    (*coords)[coord_index].set(triangle.v2);
    indices[i][2] = coord_index++;
    ++i;
  }
  // ifs->collapse_identical_coordinates();

  return ifs;
}

//! \brief adds a shape node to the scene.
Loader::Shared_shape
Loader::compute_shape(Scene_graph* scene_graph, Shared_transform transform,
                      const Vector3f& color)
{
  Shared_shape shape(new Shape);
  SGAL_assertion(shape);
  scene_graph->add_container(shape);
  shape->add_to_scene(scene_graph);
  transform->add_child(shape);
  Shared_appearance appearance(new Appearance);
  SGAL_assertion(appearance);
  shape->set_appearance(appearance);
  Shared_material material(new Material);
  SGAL_assertion(material);
  appearance->set_material(material);
  material->set_emissive_color(color);
  material->set_ambient_intensity(0.0f);
  return shape;
}

/*! \brief adds a shape for every sub sequence of triangles with a distinguish
 * color.
 */
void Loader::add_shapes(Scene_graph* scene_graph, Shared_transform transform,
                        std::list<Triangle>& triangles, const Vector3f& color)
{
  auto it = triangles.begin();
  auto first = it;
  Triangle& triangle = *it++;
  size_t count(1);

  Vector3f last_color(color);
  if (triangle.is_colored()) triangle.set_color(last_color);

  while (it != triangles.end()) {
    triangle = *it;

    Vector3f tmp_color(color);
    if (triangle.is_colored()) triangle.set_color(tmp_color);

    if (tmp_color != last_color) {
      auto shape = compute_shape(scene_graph, transform, last_color);
      auto ifs = compute_ifs(count, first, it);
      shape->set_geometry(ifs);

      first = it;
      count = 0;
      last_color.set(tmp_color);
    }
    ++it;
    ++count;
  }
  auto shape = compute_shape(scene_graph, transform, last_color);
  auto ifs = compute_ifs(count, first, triangles.end());
  shape->set_geometry(ifs);
}

//! \brief adds a colored shape.
void Loader::add_colored_shape(Scene_graph* scene_graph,
                               Shared_transform transform,
                               std::list<Triangle>& triangles,
                               const Vector3f& color)
{
  Uint count = triangles.size();

  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(count);

  Uint num_vertices = 3 * count;
  Coord_array_3d* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  ifs->set_coord_array(shared_coords);
  auto& indices = ifs->empty_triangle_coord_indices();
  indices.resize(count);

  Color_array* colors = new Color_array(count);
  Shared_color_array shared_colors(colors);
  ifs->set_color_array(shared_colors);
  auto& color_indices = ifs->empty_triangle_color_indices();
  color_indices.resize(count);

  ifs->set_color_per_vertex(true);

  size_t i(0);
  Uint j(0);

  for (auto &triangle: triangles) {
    Vector3f use_color(color);
    if (triangle.is_colored()) triangle.set_color(use_color);
    (*colors)[i].set(use_color);

    (*coords)[j].set(triangle.v0);
    color_indices[i][0] = i;
    indices[i][0] = j++;

    (*coords)[j].set(triangle.v1);
    color_indices[i][1] = i;
    indices[i][1] = j++;

    (*coords)[j].set(triangle.v2);
    color_indices[i][2] = i;
    indices[i][2] = j++;

    ++i;
  }
  auto shape = compute_shape(scene_graph, transform, color);
  shape->set_geometry(ifs);
}

//! \brief reads a scene graph from a stream in the STL binary format.
Loader::Return_code Loader::read_stl(std::istream& stl_stream, size_t size,
                                     Scene_graph* scene_graph,
                                     const Vector3f& color)
{
  scene_graph->set_input_format_id(File_format_3d::ID_STL);
  auto transform = scene_graph->initialize();       // initialize

  Int32 total_num_tris;
  stl_stream.read((char*)&total_num_tris, sizeof(Int32));
  // std::cout << "# size: " << size << std::endl;
  // std::cout << "# triangles: " << total_num_tris << std::endl;
  // Header---80
  // # triangles---4
  // Triangle---50
  //   normal,v0,v1,v2---12*4
  //   spacer--2
  if (size != 84 + total_num_tris * 50) {
    throw Inconsistent_error(m_filename);
    return FAILURE;
  }

  // Read all triangles while discarding collinear triangles.
  std::list<Triangle> triangles;
  size_t i(0);
  while (i != total_num_tris) {
    Triangle triangle;
    do {
      auto rc = read_triangle(stl_stream, triangle);
      if (rc < 0) return rc;
      ++i;
    } while (Vector3f::collinear(triangle.v0, triangle.v1, triangle.v2));
    triangles.push_back(triangle);
  }
  // Check end condition:
  if (!stl_stream) {
    throw Read_error(m_filename);
    return FAILURE;
  }

  // Construct shape nodes
  bool use_colors((color[0] != .0f) || (color[1] != .0f) ||(color[2] != .0f));
  if (use_colors && m_multiple_shapes) use_colors = false;

  if (! use_colors) add_shapes(scene_graph, transform, triangles, color);
  else add_colored_shape(scene_graph, transform, triangles, color);

  return SUCCESS;
}

SGAL_END_NAMESPACE
