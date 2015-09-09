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
Loader::Return_code Loader::load_stl(std::istream& stl_stream, Scene_graph* sg,
                                     bool force)
{
  char str[81];
  stl_stream.read(str, 80);
  std::string title(str);
  if (!stl_stream) return ERROR_READ;

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
    auto rc = read_stl(stl_stream, sg, color);
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
  if (!stl_stream.good()) return ERROR_OVERFLOW;
  return load_stl(stl_stream, sg, force);
}

//! \brief loads a scene graph from an stl file.
Loader::Return_code Loader::load_stl(const char* filename, Scene_graph* sg,
                                     bool force)
{
  std::ifstream stl_stream(filename, std::ios::in|std::ios::binary);
  if (!stl_stream.good()) return ERROR_OPEN;

  auto rc = load_stl(stl_stream, sg, force);
  stl_stream.close();
  return rc;
}

//! \brief load a scene graph from a stream.
Loader::Return_code  Loader::load(std::istream& src_stream, Scene_graph* sg)
{
  Vrml_scanner scanner(&src_stream);
  // scanner.set_debug(1);

  // Parse & export:
  bool maybe_binary_stl(false);
  Vrml_parser parser(scanner, sg, maybe_binary_stl);
  if (parser.parse()) {
    if (maybe_binary_stl) return RETRY;
    return ERROR_PARSE;
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
  if (!src_stream.good()) return ERROR_OVERFLOW;

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
  // Try to determine the file type from its extension.
  SGAL_assertion(filename);
  std::string file_extension = boost::filesystem::extension(filename);
  if (boost::iequals(file_extension, ".stl")) {
    auto rc = load_stl(filename, sg);
    if (rc <= 0) return rc;
  }

  // Open source file:
  std::ifstream src_stream(filename);
  if (!src_stream.good()) return ERROR_OPEN;

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
                                          Vector3f& v0, Vector3f& v1,
                                          Vector3f& v2,
                                          Ushort& spacer)
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
  v0.set(x, y, z);

  // Read vertex 2:
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));
  v1.set(x, y, z);

  // Read vertex 2:
  stl_stream.read((char*)&x, sizeof(Float));
  stl_stream.read((char*)&y, sizeof(Float));
  stl_stream.read((char*)&z, sizeof(Float));
  v2.set(x, y, z);

  stl_stream.read((char*)&spacer, sizeof(unsigned short));
  //std::cout << std::hex << spacer << std::endl;

  if (!stl_stream) return ERROR_READ;
  return SUCCESS;
}

//! \brief reads a scene graph from a stream in the STL binary format.
Loader::Return_code Loader::read_stl(std::istream& stl_stream,
                                     Scene_graph* scene_graph,
                                     const Vector3f& color)
{
  Boolean use_colors((color[0] != .0f) || (color[1] != .0f) ||
                     (color[2] != .0f));
  if (use_colors && m_multiple_shapes) use_colors = false;

  typedef boost::shared_ptr<Shape>                Shared_shape;
  typedef boost::shared_ptr<Indexed_face_set>     Shared_indexed_face_set;
  typedef boost::shared_ptr<Coord_array_3d>       Shared_coord_array_3d;
  typedef boost::shared_ptr<Color_array>          Shared_color_array;
  typedef boost::shared_ptr<Appearance>           Shared_appearance;
  typedef boost::shared_ptr<Material>             Shared_material;

  scene_graph->set_input_format_id(File_format_3d::ID_STL);
  auto transform = scene_graph->initialize();       // initialize

  Int32 total_num_tris;
  stl_stream.read((char*)&total_num_tris, sizeof(Int32));
  // std::cout << "# triangles: " << total_num_tris << std::endl;
  Uint total_num_vertices = total_num_tris * 3;

  Boolean new_shape(true);
  Vector3f last_color(color);

  // read first triangle
  Int32 i(0);

  Vector3f v0, v1, v2;
  Ushort spacer = 0;
  // Discard collinear faces:
  do {
    auto rc = read_triangle(stl_stream, v0, v1, v2, spacer);
    if (rc < 0) return rc;
    ++i;
  } while (Vector3f::collinear(v0, v1, v2));
  Boolean colored = spacer & 0x0001;
  if (colored) {
    Float red = static_cast<Float>((spacer >> 1) & 0x001f) / 31.0f;
    Float green = static_cast<Float>((spacer >> 6) & 0x001f) / 31.0f;
    Float blue = static_cast<Float>((spacer >> 11) & 0x001f) / 31.0f;
    last_color.set(red, green, blue);
  }

  while (new_shape) {
    new_shape = false;
    Uint coord_index(0);
    Uint color_index(0);

    Shared_shape shape(new Shape);
    SGAL_assertion(shape);
    scene_graph->add_container(shape);
    shape->add_to_scene(scene_graph);
    Shared_indexed_face_set ifs(new Indexed_face_set);
    SGAL_assertion(ifs);
    Shared_appearance appearance(new Appearance);
    SGAL_assertion(appearance);
    Shared_material material(new Material);
    SGAL_assertion(material);

    ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
    Coord_array_3d* coords = new Coord_array_3d(total_num_vertices);
    Shared_coord_array_3d shared_coords(coords);
    auto& indices = ifs->get_flat_coord_indices();
    indices.resize(total_num_vertices);
    ifs->set_coord_indices_flat(true);

    Shared_color_array colors;
    auto& color_indices = ifs->get_flat_color_indices();
    if (use_colors) {
      colors.reset(new Color_array(total_num_vertices));
      color_indices.resize(total_num_vertices);
      ifs->set_color_indices_flat(true);
      ifs->set_color_per_vertex(true);
      ifs->set_color_array(colors);

      (*colors)[color_index].set(last_color);
    }
    else {
      material->set_emissive_color(last_color);
      material->set_ambient_intensity(0.0f);
    }

    ifs->set_coord_array(shared_coords);
    appearance->set_material(material);
    shape->set_appearance(appearance);
    shape->set_geometry(ifs);
    transform->add_child(shape);

    Uint num_tris(0);

    (*coords)[coord_index].set(v0);
    if (use_colors) color_indices[coord_index] = color_index;
    indices[coord_index] = coord_index++;

    (*coords)[coord_index].set(v1);
    if (use_colors) color_indices[coord_index] = color_index;
    indices[coord_index] = coord_index++;

    (*coords)[coord_index].set(v2);
    if (use_colors) color_indices[coord_index] = color_index;
    indices[coord_index] = coord_index++;

    ++num_tris;

    while (i < total_num_tris) {
      // Discard collinear faces:
      do {
        auto rc = read_triangle(stl_stream, v0, v1, v2, spacer);
        if (rc < 0) return rc;
        ++i;
      } while (Vector3f::collinear(v0, v1, v2));
      Float red = color[0];
      Float green = color[1];
      Float blue = color[2];
      Boolean colored = spacer & 0x0001;
      if (colored) {
        red = static_cast<Float>((spacer >> 1) & 0x001f) / 31.0f;
        green = static_cast<Float>((spacer >> 6) & 0x001f) / 31.0f;
        blue = static_cast<Float>((spacer >> 11) & 0x001f) / 31.0f;
      }
      if ((red != last_color[0]) || (green != last_color[1]) ||
          (blue != last_color[2]))
      {
        last_color.set(red, green, blue);
        if (!use_colors) {
          new_shape = true;
          break;
        }
        (*colors)[++color_index].set(last_color);
      }
      (*coords)[coord_index].set(v0);
      if (use_colors) color_indices[coord_index] = color_index;
      indices[coord_index] = coord_index++;

      (*coords)[coord_index].set(v1);
      if (use_colors) color_indices[coord_index] = color_index;
      indices[coord_index] = coord_index++;

      (*coords)[coord_index].set(v2);
      if (use_colors) color_indices[coord_index] = color_index;
      indices[coord_index] = coord_index++;

      ++num_tris;
    }

    if (use_colors) colors->resize(++color_index);
    Uint num_vertices = coord_index;
    if (num_vertices != total_num_vertices) {
      coords->resize(num_vertices);
      indices.resize(num_vertices);
      if (use_colors) color_indices.resize(num_vertices);
    }
    ifs->set_num_primitives(num_tris);
    ifs->collapse_identical_coordinates();
    total_num_tris -= i;
    total_num_vertices -= 3 * i;
  }

  if (!stl_stream) return ERROR_READ;
  return SUCCESS;
}

SGAL_END_NAMESPACE
