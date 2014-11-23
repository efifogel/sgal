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
#include <vector>
#include <boost/filesystem.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

int Loader::load(const char* filename, Scene_graph* sg)
{
  // Try to determine whether the file is binary or ascii
  std::string file_extension = boost::filesystem::extension(filename);
  if (file_extension == ".stl") {
    int c;
    std::ifstream stl_stream(filename, std::ios::in|std::ios::binary);
    if (!stl_stream.good()) {
      std::cerr << "Error: failed to open " << filename << "!" << std::endl;
      return -1;
    }
    char str[81];
    stl_stream.read(str, 80);
    std::string title(str);
    if (!stl_stream) {
      std::cout << "Error: only " << stl_stream.gcount() << " could be read!"
                << std::endl;
      stl_stream.close();
      return -1;
    }

    if (0 != title.compare(0, 5, "solid")) {
      int rc = read_stl(stl_stream, sg);
      stl_stream.close();
      if (rc < 0) {
        std::cerr << "Error: Failed to read " << filename << "!" << std::endl;
        return -1;
      }
      return 0;
    }
    stl_stream.close();
  }

  // Open source file:
  std::ifstream src_stream(filename);

  Vrml_scanner scanner(&src_stream);
  // scanner.set_debug(1);

  // Parse & export:
  Vrml_parser parser(scanner, sg);
  if (parser.parse()) {
    std::cerr << "Error: failed to parse " << filename << "!" << std::endl;
    return -1;
  }
  return 0;
}

int Loader::read_stl(std::ifstream& stl_stream, Scene_graph* scene_graph)
{
  scene_graph->set_input_format_id(File_format::ID_STL);

  Group* group = new Group;
  scene_graph->set_root(Shared_group(group));
  Transform* transform = new Transform;
  Shared_transform shared_transform(transform);
  scene_graph->add_container(shared_transform, g_navigation_root_name);
  scene_graph->set_navigation_root(shared_transform);
  group->add_child(shared_transform);
  Shape* shape = new Shape;
  Shared_shape shared_shape(shape);
  transform->add_child(shared_shape);
  Indexed_face_set* indexed_face_set = new Indexed_face_set;
  Shared_indexed_face_set shared_ifs(indexed_face_set);
  shape->set_geometry(shared_ifs);
  shape->add_to_scene(scene_graph);
  scene_graph->add_container(shared_shape);

  Int32 num_tris;
  stl_stream.read((char*)&num_tris, sizeof(Int32));
  std::cout << "# triangles: " << num_tris << std::endl;
  shared_ifs->set_num_primitives(num_tris);
  shared_ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  Uint num_vertices = num_tris * 3;

  Coord_array_3d* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  shared_ifs->set_coord_array(shared_coords);
  auto& indices = shared_ifs->get_flat_coord_indices();
  shared_ifs->set_coord_indices_flat(true);
  indices.resize(num_vertices);

  Uint index(0);

  for (size_t i = 0; i < num_tris; ++i) {
    Float x, y, z;
    // Read normal
    stl_stream.read((char*)&x, sizeof(Float));
    stl_stream.read((char*)&y, sizeof(Float));
    stl_stream.read((char*)&z, sizeof(Float));

    // Read vertex 1:
    stl_stream.read((char*)&x, sizeof(Float));
    stl_stream.read((char*)&y, sizeof(Float));
    stl_stream.read((char*)&z, sizeof(Float));
    (*coords)[index].set(x, y, z);
    indices[index] = index++;

    // Read vertex 2:
    stl_stream.read((char*)&x, sizeof(Float));
    stl_stream.read((char*)&y, sizeof(Float));
    stl_stream.read((char*)&z, sizeof(Float));
    (*coords)[index].set(x, y, z);
    indices[index] = index++;

    // Read vertex 2:
    stl_stream.read((char*)&x, sizeof(Float));
    stl_stream.read((char*)&y, sizeof(Float));
    stl_stream.read((char*)&z, sizeof(Float));
    (*coords)[index].set(x, y, z);
    indices[index] = index++;

    unsigned short spacer;
    stl_stream.read((char*)&spacer, sizeof(unsigned short));
  }

  shared_ifs->collapse_identical_coordinates();
  return 0;
}

SGAL_END_NAMESPACE
