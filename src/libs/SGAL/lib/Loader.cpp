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
#include <boost/algorithm/string.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/File_format_3d.hpp"

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief loads a scene graph from an stl file.
int Loader::load_stl(const char* filename, Scene_graph* sg, bool force)
{
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
  if (force || (0 != title.compare(0, 5, "solid"))) {
    int rc = read_stl(stl_stream, sg);
    stl_stream.close();
    if (rc < 0) {
      std::cerr << "Error: Failed to read " << filename << "!" << std::endl;
      return -1;
    }
    return 0;
  }
  stl_stream.close();
  return 1;
}

//! \brief loads a scene graph from a file.
int Loader::load(const char* filename, Scene_graph* sg)
{
  // Try to determine whether the file is binary or ascii
  std::string file_extension = boost::filesystem::extension(filename);
  if (boost::iequals(file_extension, ".stl")) {
    int rc = load_stl(filename, sg);
    if (rc <= 0) return rc;
  }

  // Open source file:
  std::ifstream src_stream(filename);

  Vrml_scanner scanner(&src_stream);
  // scanner.set_debug(1);

  // Parse & export:
  bool maybe_binary_stl(false);
  Vrml_parser parser(scanner, sg, maybe_binary_stl);
  if (parser.parse()) {
    if (maybe_binary_stl) {
      int rc = load_stl(filename, sg, true);
      if (rc == 0) return rc;
    }
    std::cerr << "Error: failed to parse " << filename << "!" << std::endl;
    return -1;
  }
  return 0;
}

//! \brief reads a scene graph from a file in the STL binary format.
int Loader::read_stl(std::ifstream& stl_stream, Scene_graph* scene_graph)
{
  scene_graph->set_input_format_id(File_format_3d::ID_STL);
  Shared_shape shape(new Shape);
  SGAL_assertion(shape);
  scene_graph->initialize(shape);       // initialize

  /*! Add IndexedFaceSet */
  Shared_indexed_face_set ifs(new Indexed_face_set);
  shape->set_geometry(ifs);

  Int32 num_tris;
  stl_stream.read((char*)&num_tris, sizeof(Int32));
  std::cout << "# triangles: " << num_tris << std::endl;
  ifs->set_num_primitives(num_tris);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  Uint num_vertices = num_tris * 3;

  Coord_array_3d* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  ifs->set_coord_array(shared_coords);
  auto& indices = ifs->get_flat_coord_indices();
  ifs->set_coord_indices_flat(true);
  indices.resize(num_vertices);

  Uint index(0);

  for (Int32 i = 0; i < num_tris; ++i) {
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

  ifs->collapse_identical_coordinates();
  return 0;
}

SGAL_END_NAMESPACE
