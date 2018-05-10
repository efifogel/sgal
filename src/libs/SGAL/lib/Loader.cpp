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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/File_format_3d.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/find_file.hpp"
#include "SGAL/Dxf_parser.hpp"

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Loader::Loader() : m_multiple_shapes(false) {}

//! \brief loads a scene graph from a file.
Loader_code Loader::load(const char* filename, Scene_graph* sg)
{
  SGAL_assertion(filename);

  // Record the filename
  m_filename = filename;

  // Open source file.
  std::ifstream is(filename, std::ifstream::binary);
  if (!is.good()) {
    throw Open_file_error(m_filename);
    return Loader_code::FAILURE;
  }

  // Verify that the file is not empty.
  if (is.peek() == std::char_traits<char>::eof()) {
    throw Empty_error(m_filename);
    is.close();
    return Loader_code::FAILURE;
  }

  // Obtain the file extension.
  auto file_extension = boost::filesystem::extension(filename);

  // We do not rely on the file extension neither on the magic string (if
  // exists at all). We don't know whether the file is binary or text.
  // We read the first 80 characters into a buffer and treat it as string.
  // (We rewind the input stream indicater afterwards.)
  // We terminate the buffer with the end-of-string character replacing the
  // end-of-line character, if exists, otherwise, at the end of the buffer.
  // Obtain the first line and and examine the magic string
  char line[81];
  is.read(line, 80);
  if (!is) {
    throw Read_error(m_filename);
    return Loader_code::FAILURE;
  }
  auto it = line;
  for (; it != &line[80]; ++it) if (*it == '\n') break;
  *it = '\0';
  std::string magic(line);

  // If the first line starts with "OFF", assume that the file is in the off
  // format. If the return code of the loader is positive, the file might be i
  // n a different format. In this case, continue trying matching.
  boost::smatch what;
  boost::regex re("(ST)?(C)?(N)?(4)?(n)?OFF\\s*(\\s#.*)?");
  if (boost::regex_match(magic, what, re)) {
    // If the first line starts with "OFF" and the file extension is off,
    // assume that the file format is OFF.
    auto rc = load_off(is, sg, what);
    if (static_cast<int>(rc) <= 0) {
      is.close();
      if (rc == Loader_code::SUCCESS)
        std::cerr << "Warning: File extension " << file_extension
                  << " does not match file format (OFF)" << std::endl;
      return rc;
    }
  }

  // If the magic string is either "solid" or #VRL, assume that the file is in
  // the VRML or the text STL format, respectively.
  // If the return code of the parser is positive, the file might be in a
  // different format. In this case, continue trying matching.
  else if ((0 == magic.compare(0, 5, "solid")) ||
           (0 == magic.compare(0, 5, "#VRML")))
  {
    is.seekg(0, is.beg);        // rewind
    auto rc = parse(is, sg);
    if (static_cast<int>(rc) <= 0) {
      is.close();
      return rc;
    }
  }

  is.seekg(0, is.beg);          // rewind

  // If the extension is .obj, assume that the file is in the obj format.
  // If the return code of the loader is positive, the file might be in a
  // different format. In this case, continue trying matching.
  if (boost::iequals(file_extension, ".obj")) {
    auto rc = parse_obj(is, sg);
    is.close();
    if (static_cast<int>(rc) <= 0) {
      if (static_cast<int>(rc) < 0) throw Parse_error(m_filename);
      return rc;
    }
  }

  // If the extension is .stl, assume that the file is in the binary stl format.
  // If the return code of the loader is positive, the file might be in a
  // different format. In this case, continue trying matching.
  else if (boost::iequals(file_extension, ".stl")) {
    auto rc = load_stl(is, sg);
    is.close();
    if (static_cast<int>(rc) <= 0) {
      if (rc == Loader_code::SUCCESS) {
        if (0 == magic.compare(0, 5, "solid")) {
          std::cerr << "Warning: The file magic string " << "\"solid\""
                    << " does not match the file format (binary STL)"
                    << std::endl;
        }
      }
      return rc;
    }
  }

  // If the extension is .dxf, assume that the file is in the dxf format.
  // If the return code of the loader is positive, the file might be in a
  // different format. In this case, continue trying matching.
  if (boost::iequals(file_extension, ".dxf")) {
    Dxf_parser parser(is, sg);
    auto rc = parser();
    is.close();
    if (static_cast<int>(rc) <= 0) {
      if (static_cast<int>(rc) < 0) throw Parse_error(m_filename);
      return rc;
    }
  }

  // Assume that the file is in the binary STL format.
  is.seekg(0, is.beg);
  auto rc = load_stl(is, sg);
  is.close();
  if (rc == Loader_code::SUCCESS) {
    if (!boost::iequals(file_extension, ".stl"))
      std::cerr << "Warning: The file extension " << file_extension
                << " does not match the file format (binary STL)" << std::endl;
    if (0 == magic.compare(0, 5, "solid")) {
      std::cerr << "Warning: The file magic string " << "\"solid\""
                << " does not match the file format (binary STL)" << std::endl;
    }
  }
  return rc;
}

//! \brief loads a scene graph from an input stream.
Loader_code
Loader::load_stl(std::istream& is, size_t size, Scene_graph* sg)
{
  char str[81];
  is.read(str, 80);
  if (!is) {
    throw Read_error(m_filename);
    return Loader_code::FAILURE;
  }

  std::string title(str);
  // std::cout << "title: " << title << std::endl;
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
  auto rc = read_stl(is, size, sg, color);
  if (static_cast<int>(rc) < 0) return rc;
  return Loader_code::SUCCESS;
}

//! \brief loads a scene graph from an stl file.
Loader_code Loader::load_stl(char* data, size_t size,
                                     Scene_graph* sg)
{
  boost::interprocess::bufferstream is(data, size);
  if (!is.good()) {
    throw Overflow_error(m_filename);
    return Loader_code::FAILURE;
  }
  return load_stl(is, size, sg);
}

//! \brief loads a scene graph from an stl file.
Loader_code Loader::load_stl(std::istream& is, Scene_graph* sg)
{
  is.seekg(0, is.end);
  size_t size = is.tellg();
  is.seekg(0, is.beg);
  auto rc = load_stl(is, size, sg);
  return rc;
}

//! \brief parses a scene graph from a stream.
Loader_code Loader::parse(std::istream& its, Scene_graph* sg)
{
  Vrml_scanner scanner(&its);
  scanner.set_filename(m_filename);
  // scanner.set_debug(1);

  // Parse & export:
  bool maybe_binary_stl(false);
  auto* root = sg->initialize();
  Vrml_parser parser(scanner, sg, root, maybe_binary_stl);
  auto rc = parser.parse();
  if (0 != rc) {
    if (maybe_binary_stl) return Loader_code::RETRY;
    throw Parse_error(m_filename);
    return Loader_code::FAILURE;
  }
  return Loader_code::SUCCESS;
}

//! \brief loads a scene graph from a buffer.
Loader_code
Loader::load(char* data, size_t size, const char* filename, Scene_graph* sg)
{
  // Try to determine the file type from its extension.
  if (filename) {
    std::string file_extension = boost::filesystem::extension(filename);
    if (boost::iequals(file_extension, ".stl")) {
      auto rc = load_stl(data, size, sg);
      if (static_cast<int>(rc) <= 0) return rc;
    }
  }
  return load(data, size, sg);
}

//! \brief loads a scene graph from a buffer.
Loader_code Loader::load(char* data, size_t size, Scene_graph* sg)
{
  boost::interprocess::bufferstream is(data, size);
  if (!is.good()) {
    throw Overflow_error(m_filename);
    return Loader_code::FAILURE;
  }

  auto rc = parse(is, sg);
  if (static_cast<int>(rc) < 0) {
    is.clear();
    return rc;
  }

  is.clear();

  if (rc == Loader_code::RETRY) rc = load_stl(data, size, sg);
  return rc;
}

//! \bried read a traingle (1 normal and 3 vertices)
Loader_code Loader::read_triangle(std::istream& is, Triangle& triangle)
{
  Float x, y, z;

  // Read normal
  is.read((char*)&x, sizeof(Float));
  is.read((char*)&y, sizeof(Float));
  is.read((char*)&z, sizeof(Float));

  // Read vertex 1:
  is.read((char*)&x, sizeof(Float));
  is.read((char*)&y, sizeof(Float));
  is.read((char*)&z, sizeof(Float));
  triangle.v0.set(x, y, z);

  // Read vertex 2:
  is.read((char*)&x, sizeof(Float));
  is.read((char*)&y, sizeof(Float));
  is.read((char*)&z, sizeof(Float));
  triangle.v1.set(x, y, z);

  // Read vertex 3:
  is.read((char*)&x, sizeof(Float));
  is.read((char*)&y, sizeof(Float));
  is.read((char*)&z, sizeof(Float));
  triangle.v2.set(x, y, z);

  is.read((char*)&(triangle.spacer), sizeof(unsigned short));
  // std::cout << std::hex << triangle.spacer << std::endl;

  if (!is) {
    throw Read_error(m_filename);
    return Loader_code::FAILURE;
  }
  return Loader_code::SUCCESS;
}

//! \brief computes a new Indexed Face Set container.
Loader::Shared_indexed_face_set
Loader::create_ifs(Scene_graph* scene_graph, size_t count,
                   std::list<Triangle>::const_iterator begin,
                   std::list<Triangle>::const_iterator end)
{
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(count);
  ifs->set_make_consistent(true);

  auto num_vertices = count * 3;
  auto* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  ifs->set_coord_array(shared_coords);
  auto& indices = ifs->get_empty_triangle_coord_indices();
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
  ifs->facet_coord_indices_changed();
  ifs->collapse_identical_coordinates();
  ifs->add_to_scene(scene_graph);

  return ifs;
}

//! \brief creates a new appearance container.
Loader::Shared_appearance Loader::create_appearance(Scene_graph* sg) const
{
  Shared_appearance app(new Appearance);
  SGAL_assertion(app);
  sg->add_container(app);
  return app;
}

//! \brief creates a new appearance container.
Loader::Shared_appearance
Loader::create_appearance(Scene_graph* sg, const std::string& name) const
{
  Shared_appearance app(new Appearance);
  SGAL_assertion(app);
  sg->add_container(app, name);
  return app;
}

//! \brief creates a new Material container.
Loader::Shared_material Loader::create_material(Scene_graph* sg) const
{
  Shared_material mat(new Material);
  SGAL_assertion(mat);
  sg->add_container(mat);
  return mat;
}

//! \brief creates a new IndexedFaceSet container.
Loader::Shared_indexed_face_set Loader::create_ifs(Scene_graph* sg) const
{
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->add_to_scene(sg);
  sg->add_container(ifs);
  return ifs;
}

//! \brief creates a new shape node.
Loader::Shared_shape Loader::create_shape(Scene_graph* sg) const
{
  Shared_shape shape(new Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(sg);
  sg->add_container(shape);
  return shape;
}

//! \brief adds a shape node to the scene.
Loader::Shared_shape
Loader::create_shape(Scene_graph* scene_graph, const Vector3f& color)
{
  auto shape = create_shape(scene_graph);
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

//! \brief adds a single shape for all triangles.
void Loader::add_shape(Scene_graph* scene_graph, Group* group,
                       std::list<Triangle>& tris)
{
  Vector3f color;       // Place holder
  auto shape = create_shape(scene_graph, color);
  group->add_child(shape);
  auto ifs = create_ifs(scene_graph, tris.size(), tris.begin(), tris.end());
  shape->set_geometry(ifs);
}

/*! \brief adds a shape for every sub sequence of triangles with a distinguish
 * color.
 */
void Loader::add_shapes(Scene_graph* scene_graph, Group* group,
                        std::list<Triangle>& triangles, const Vector3f& color)
{
  auto it = triangles.begin();
  auto first = it;
  const Triangle& triangle = *it++;
  size_t count(1);

  Vector3f last_color(color);
  if (triangle.is_colored()) triangle.set_color(last_color);

  while (it != triangles.end()) {
    const Triangle& triangle = *it;

    Vector3f tmp_color(color);
    if (triangle.is_colored()) triangle.set_color(tmp_color);

    if (tmp_color != last_color) {
      auto shape = create_shape(scene_graph, last_color);
      group->add_child(shape);
      auto ifs = create_ifs(scene_graph, count, first, it);
      shape->set_geometry(ifs);

      first = it;
      count = 0;
      last_color.set(tmp_color);
    }
    ++it;
    ++count;
  }
  auto shape = create_shape(scene_graph, last_color);
  group->add_child(shape);
  auto ifs = create_ifs(scene_graph, count, first, triangles.end());
  shape->set_geometry(ifs);
}

//! \brief adds a colored shape.
void Loader::add_colored_shape(Scene_graph* scene_graph, Group* group,
                               std::list<Triangle>& triangles,
                               const Vector3f& color)
{
  auto count = triangles.size();

  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(count);

  auto num_vertices = count * 3;
  Coord_array_3d* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  ifs->set_coord_array(shared_coords);
  auto& indices = ifs->get_empty_triangle_coord_indices();
  indices.resize(count);

  Color_array* colors = new Color_array(count);
  Shared_color_array shared_colors(colors);
  ifs->set_color_array(shared_colors);
  auto& color_indices = ifs->get_empty_triangle_color_indices();
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
  auto shape = create_shape(scene_graph, color);
  group->add_child(shape);
  ifs->facet_coord_indices_changed();
  ifs->facet_color_indices_changed();
  ifs->collapse_identical_coordinates();
  ifs->add_to_scene(scene_graph);
  shape->set_geometry(ifs);
}

//! \brief reads a scene graph from a stream in the STL binary format.
Loader_code Loader::read_stl(std::istream& is, size_t size,
                                     Scene_graph* scene_graph,
                                     const Vector3f& color)
{
  scene_graph->set_input_format_id(File_format_3d::ID_STL);
  auto* root = scene_graph->initialize();

  Int32 total_num_tris;
  is.read((char*)&total_num_tris, sizeof(Int32));
  // std::cout << "# size: " << size << std::endl;
  // std::cout << "# triangles: " << total_num_tris << std::endl;
  // Header---80
  // # triangles---4
  // Triangle---50
  //   normal,v0,v1,v2---12*4
  //   spacer--2
  if (size != 84 + total_num_tris * 50) {
    throw Inconsistent_error(m_filename);
    return Loader_code::FAILURE;
  }

  // Read all triangles while discarding collinear triangles.
  std::list<Triangle> triangles;
  size_t i(0);
  while (i != total_num_tris) {
    Triangle triangle;
    do {
      auto rc = read_triangle(is, triangle);
      if (static_cast<int>(rc) < 0) return rc;
      ++i;
    } while (Vector3f::collinear(triangle.v0, triangle.v1, triangle.v2));
    triangles.push_back(triangle);
  }
  // Check end condition:
  if (!is) {
    throw Read_error(m_filename);
    return Loader_code::FAILURE;
  }

  // Construct shape nodes
  if (m_multiple_shapes) add_shapes(scene_graph, root, triangles, color);
  else {
    bool use_colors((color[0] != .0f) || (color[1] != .0f) ||(color[2] != .0f));
    if (use_colors) add_colored_shape(scene_graph, root, triangles, color);
    else add_shape(scene_graph, root, triangles);
  }

  return Loader_code::SUCCESS;
}

/*! \brief loads a scene graph represented in the off file format from a stream.
 * Observe that the magic string has been consumed.
 * \param what
 *    what[0]---entire string
 *    what[1]---texture coordinate
 *    what[2]---color
 *    what[3]---normal
 *    what[4]---4 components including a final homogeneous component
 *    what[5]---n components
 */
Loader_code Loader::load_off(std::istream& is, Scene_graph* sg,
                                     const boost::smatch& what)
{
  sg->set_input_format_id(File_format_3d::ID_OFF);
  bool has_colors = what.length(2);

  // Obtain root.
  auto* root = sg->initialize();

  // Add Shape
  Shared_shape shape(new Shape);
  SGAL_assertion(shape);
  shape->add_to_scene(sg);
  sg->add_container(shape);
  root->add_child(shape);

  // Add Appearance
  Shared_appearance app(new Appearance);
  SGAL_assertion(app);
  shape->set_appearance(app);

  // Add IndexedFaceSet
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  ifs->add_to_scene(sg);
  sg->add_container(ifs);
  shape->set_geometry(ifs);

  size_t num_vertices;
  size_t num_facets;
  size_t num_edges;
  is >> num_vertices >> num_facets >> num_edges;
  if ((num_vertices << 1) < num_facets) {
    throw Inconsistent_error(m_filename);
    return Loader_code::FAILURE;
  }
  // std::cout << num_vertices << std::endl;
  // std::cout << num_facets << std::endl;
  // std::cout << num_edges << std::endl;

  auto* coords = new Coord_array_3d(num_vertices);
  SGAL_assertion(coords);
  Shared_coord_array_3d shared_coords(coords);
  coords->add_to_scene(sg);
  sg->add_container(shared_coords);

  Color_array* colors;
  Shared_color_array shared_colors;
  float scale;
  if (has_colors) {
    colors = new Color_array(num_vertices);
    SGAL_assertion(colors);
    shared_colors = Shared_color_array(colors);
    colors->add_to_scene(sg);
    sg->add_container(shared_colors);
    scale = 1.0 / 255.0;
  }

  for (auto i = 0; i < num_vertices; ++i) {
    float x, y, z;
    is >> x >> y >> z;
    (*coords)[i].set(x, y, z);

    if (has_colors) {
      float r, g, b, a;
      is >> r >> g >> b >> a;
      (*colors)[i].set(scale * r, scale * g, scale * b);
    }
  }

  Polygon_indices indices;
  indices.resize(num_facets);
  Boolean tris(true), quads(true);
  for (auto i = 0; i < num_facets; ++i) {
    size_t n;
    is >> n;
    SGAL_assertion(2 != n);
    if (3 != n) tris = false;
    if (4 != n) quads = false;
    indices[i].resize(n);
    for (auto j = 0; j < n; ++j) is >> indices[i][j];
  }
  if (tris) {
    ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
    auto& tris = ifs->get_empty_triangle_coord_indices();
    tris.resize(num_facets);
    for (auto i = 0; i < num_facets; ++i) {
      tris[i][0] = indices[i][0];
      tris[i][1] = indices[i][1];
      tris[i][2] = indices[i][2];
      indices[i].clear();
    }
    indices.clear();
  }
  else if (quads) {
    ifs->set_primitive_type(Geo_set::PT_QUADS);
    auto& quads = ifs->get_empty_quad_coord_indices();
    quads.resize(num_facets);
    for (auto i = 0; i < num_facets; ++i) {
      quads[i][0] = indices[i][0];
      quads[i][1] = indices[i][1];
      quads[i][2] = indices[i][2];
      quads[i][3] = indices[i][3];
      indices[i].clear();
    }
    indices.clear();
  }
  else {
    ifs->set_primitive_type(Geo_set::PT_POLYGONS);
    ifs->set_facet_coord_indices(std::move(indices));
  }

  ifs->facet_coord_indices_changed();
  ifs->set_coord_array(shared_coords);
  if (has_colors) {
    ifs->set_color_array(shared_colors);
    ifs->set_color_per_vertex(true);
  }
  ifs->set_num_primitives(num_facets);
  ifs->add_to_scene(sg);

  return Loader_code::SUCCESS;
}

/*! Obtain the number represented by a symbol.
 */
template <typename Number_type>
bool get_number(const Vrml_parser::symbol_type& symbol, Number_type& number)
{
  if (symbol.token() != Vrml_parser::token::TOK_NUMBER) {
    std::cerr << "Error at " << symbol.location << ": "
              << "invalid symbol" << std::endl;
    return false;
  }
  Shared_string number_str(symbol.value.as<Shared_string>());
  number = boost::lexical_cast<Number_type>(*number_str);
  return true;
}

/*! Read a number from a scanner.
 */
template <typename Number_type>
bool read_number(Vrml_scanner& scanner, Number_type& number)
{
  Vrml_parser::symbol_type symbol(scanner.mylex());
  return get_number(symbol, number);
}

/*! Obtain the string represented by a symbol.
 */
bool get_string(const Vrml_parser::symbol_type& symbol,
                               Shared_string& str)
{
  if (symbol.token() != Vrml_parser::token::TOK_IDENTIFIER) {
    std::cerr << "Error at " << symbol.location << ": "
              << "invalid string" << std::endl;
    return false;
  }
  str = symbol.value.as<Shared_string>();
  return true;
}

/*! Read a string from a scanner.
 */
bool read_string(Vrml_scanner& scanner, Shared_string& str)
{
  Vrml_parser::symbol_type symbol(scanner.mylex());
  return get_string(symbol, str);
}

/*! Read a line.
 */
bool read_obj_line(Vrml_scanner& scanner)
{
  while (true) {
    Vrml_parser::symbol_type symbol(scanner.mylex());
    if (symbol.token() == Vrml_parser::token::TOK_K_LINE_END) break;
  }
  return true;
}

/*! Read coords and perhaps colors from an obj file.
 */
bool read_obj_coords_and_colors(Vrml_scanner& scanner, Coord_array_3d* coords,
                                Color_array* colors)
{
  Vrml_parser::location_type loc;
  std::array<float, 6> nums;
  size_t j(0);
  while (true) {
    Vrml_parser::symbol_type num_symbol(scanner.mylex());
    if (num_symbol.token() == Vrml_parser::token::TOK_K_LINE_END) break;
    loc = num_symbol.location;
    if (j == 6) {
      std::cerr << "Warning: more than 6 coordinates at " << loc
                << ". Remaining coordinates are ignored!" << std::endl;
      break;
    }
    if (! get_number(num_symbol, nums[j++])) return false;
  }

  if ((j < 3) || (j == 5)) {
    std::cerr << "Error at " << loc << ": "
              << "wrong number of coordinates" << std::endl;
    return false;
  }
  if (j == 4) {
    nums[0] /= nums[3];
    nums[1] /= nums[3];
    nums[2] /= nums[3];
  }
  Vector3f v(nums[0], nums[1], nums[2]);
  coords->push_back(v);
  if (j == 6) {
    Vector3f c(nums[3], nums[4], nums[5]);
    colors->push_back(c);
  }
  return true;
}

/*! Read texture coordinates from an obj file.
 */
bool read_obj_tex_coords(Vrml_scanner& scanner, Tex_coord_array_2d* tex_coords)
{
  Vrml_parser::location_type loc;
  std::array<float, 6> nums;
  size_t j(0);
  while (true) {
    Vrml_parser::symbol_type num_symbol(scanner.mylex());
    if (num_symbol.token() == Vrml_parser::token::TOK_K_LINE_END) break;
    loc = num_symbol.location;
    if (j == 4) {
      std::cerr << "Warning: more than 4 texture coordinates at " << loc
                << ". Remaining coordinates are ignored!" << std::endl;
      break;
    }
    if (! get_number(num_symbol, nums[j++])) return false;
  }

  if (j < 2) {
    std::cerr << "Error at " << loc << ": "
              << "insufficient number of texture coordinates" << std::endl;
    return false;
  }
  if (j == 3) {
    nums[0] /= nums[2];
    nums[1] /= nums[2];
  }
  Vector2f v(nums[0], nums[1]);
  tex_coords->push_back(v);
  return true;
}

/*! Read normals from an obj file.
 */
bool read_obj_normals(Vrml_scanner& scanner, Normal_array* normals)
{
  Vrml_parser::location_type loc;
  std::array<float, 3> nums;
  size_t j(0);
  while (true) {
    Vrml_parser::symbol_type num_symbol(scanner.mylex());
    if (num_symbol.token() == Vrml_parser::token::TOK_K_LINE_END) break;
    loc = num_symbol.location;
    if (j == 4) {
      std::cerr << "Warning: more than 4 normal coordinates at " << loc
                << ". Remaining normals coordinates are ignored!" << std::endl;
      break;
    }
    if (! get_number(num_symbol, nums[j++])) return false;
  }

  if (j < 3) {
    std::cerr << "Error at " << loc << ": "
              << "insufficient number of normal coordinates" << std::endl;
    return false;
  }
  Vector3f n(nums[0], nums[1], nums[2]);
  normals->push_back(n);
  return true;
}

/*! Read indices of coordinates from an obj file.
 */
bool read_obj_facet(Vrml_scanner& scanner,
                    Polygon_indices& coord_indices,
                    Polygon_indices& normal_indices,
                    Polygon_indices& tex_coord_indices,
                    size_t num_coords)
{
  coord_indices.resize(coord_indices.size()+1);
  auto& facet_coord_indices = coord_indices.back();
  facet_coord_indices.reserve(3);

  Index_array facet_normal_indices;
  facet_normal_indices.reserve(3);
  Index_array facet_tex_coord_indices;
  facet_tex_coord_indices.reserve(3);
  size_t num_slashes(0);
  while (true) {
    Vrml_parser::symbol_type v_symbol(scanner.mylex());
    if (v_symbol.token() == Vrml_parser::token::TOK_K_LINE_END) break;
    if (v_symbol.token() == Vrml_parser::token::TOK_SLASH) {
      ++num_slashes;
      continue;
    }
    auto loc = v_symbol.location;
    int i;
    if (! get_number(v_symbol, i)) return false;
    if (0 == i) {
      std::cerr << "Error at " << v_symbol.location << ": "
                << "zero index is illegal" << std::endl;
      return false;
    }
    size_t index = (i > 0) ? i - 1 : num_coords + i;
    if (num_slashes == 0) facet_coord_indices.push_back(index);
    else if (num_slashes == 1) facet_tex_coord_indices.push_back(index);
    else if (num_slashes == 2) {
      facet_normal_indices.push_back(index);
      num_slashes = 0;
    }
    else {
      SGAL_error();
      return false;
    }
  }
  if (!facet_normal_indices.empty())
    normal_indices.push_back(std::move(facet_normal_indices));
  if (!facet_tex_coord_indices.empty())
    tex_coord_indices.push_back(std::move(facet_tex_coord_indices));
  return true;
}

/*! Read material library.
 */
Loader_code Loader::parse_mtl(const std::string& filename, Scene_graph* sg)
{
  std::string fullname;
  std::list<fi::path> dirs;
  dirs.push_back(".");
  fi::path dir(m_filename);
  dirs.push_back(dir.parent_path());
  find_file(filename, dirs, fullname);
  if (fullname.empty()) {
    throw Find_file_error(filename);
    return Loader_code::FAILURE;
  }
  // Open source file.
  std::ifstream mtl_is(fullname);
  if (!mtl_is.good()) {
    throw Open_file_error(fullname);
    return Loader_code::FAILURE;
  }

  Vrml_scanner scanner(&mtl_is);
  scanner.push_state(6);
  Shared_appearance app;
  Shared_material mat;
  bool done(false);
  while (!done) {
    Vrml_parser::symbol_type symbol(scanner.mylex());
    switch (symbol.token()) {
     case Vrml_parser::token::TOK_END: done = true; break;
     case Vrml_parser::token::TOK_K_NEW_MATERIAL:
      {
        Shared_string name;
        if (! read_string(scanner, name)) return Loader_code::FAILURE;
        app = create_appearance(sg, *name);
        mat = create_material(sg);
        app->set_material(mat);
      }
      break;

     case Vrml_parser::token::TOK_K_SPECULAR_EXPONENT:
      {
        float number;
        if (! read_number(scanner, number)) return Loader_code::FAILURE;
        std::cerr << "Specular Exponent not supported yet!" << std::endl;
      }
      break;

     case Vrml_parser::token::TOK_K_AMBIENT_COLOR:
      {
        float r, g, b;
        if (! read_number(scanner, r)) return Loader_code::FAILURE;
        if (! read_number(scanner, g)) return Loader_code::FAILURE;
        if (! read_number(scanner, b)) return Loader_code::FAILURE;
        if ((r != 1) && (g != 1) & (b != 1)) mat->set_ambient_color(r, g, b);
      }
      break;

     case Vrml_parser::token::TOK_K_DIFFUSE_COLOR:
      {
        float r, g, b;
        if (! read_number(scanner, r)) return Loader_code::FAILURE;
        if (! read_number(scanner, g)) return Loader_code::FAILURE;
        if (! read_number(scanner, b)) return Loader_code::FAILURE;
        mat->set_diffuse_color(r, g, b);
      }
      break;

     case Vrml_parser::token::TOK_K_SPECULAR_COLOR:
      {
        float r, g, b;
        if (! read_number(scanner, r)) return Loader_code::FAILURE;
        if (! read_number(scanner, g)) return Loader_code::FAILURE;
        if (! read_number(scanner, b)) return Loader_code::FAILURE;
        mat->set_specular_color(r, g, b);
      }
      break;

     case Vrml_parser::token::TOK_K_EMISSIVE_COLOR:
      {
        float r, g, b;
        if (! read_number(scanner, r)) return Loader_code::FAILURE;
        if (! read_number(scanner, g)) return Loader_code::FAILURE;
        if (! read_number(scanner, b)) return Loader_code::FAILURE;
        mat->set_emissive_color(r, g, b);
      }
      break;

     case Vrml_parser::token::TOK_K_OPTICAL_DENSITY:
      {
        float number;
        if (! read_number(scanner, number)) return Loader_code::FAILURE;
        std::cerr << "Optical Density not supported yet!" << std::endl;
      }
      break;

     case Vrml_parser::token::TOK_K_DISSOLVE:
      {
        float number;
        if (! read_number(scanner, number)) return Loader_code::FAILURE;
        mat->set_transparency(1.0f - number);
      }
      break;

     case Vrml_parser::token::TOK_K_TRANSPARENCY:
      {
        float number;
        if (! read_number(scanner, number)) return Loader_code::FAILURE;
        mat->set_transparency(number);
      }
      break;

     case Vrml_parser::token::TOK_K_ILLUMINATION:
      {
        float number;
        if (! read_number(scanner, number)) return Loader_code::FAILURE;
        std::cerr << "illumination not supported yet!" << std::endl;
      }
      break;

     default: break;
    }
  }
  scanner.pop_state();
  mtl_is.close();
  return Loader_code::SUCCESS;
}

/*! Convert polygons to triangles and clear the polygons.
 */
void to_tri_indices(Polygon_indices& indices, Triangle_indices& tri_indices)
{
  tri_indices.resize(indices.size());
  for (auto i = 0; i < tri_indices.size(); ++i) {
    tri_indices[i][0] = indices[i][0];
    tri_indices[i][1] = indices[i][1];
    tri_indices[i][2] = indices[i][2];
    indices[i].clear();
  }
  indices.clear();
}

/*! Convert polygons to quadrilaterals and clear the polygons.
 */
void to_quad_indices(Polygon_indices& indices, Quad_indices& quad_indices)
{
  quad_indices.resize(indices.size());
  for (auto i = 0; i < quad_indices.size(); ++i) {
    quad_indices[i][0] = indices[i][0];
    quad_indices[i][1] = indices[i][1];
    quad_indices[i][2] = indices[i][2];
    quad_indices[i][3] = indices[i][3];
    indices[i].clear();
  }
  indices.clear();
}

/*! Update an IndexedFaceSet container.
 */
Loader_code
Loader::update_ifs(Scene_graph* sg,
                   Shared_indexed_face_set ifs,
                   Shared_coord_array_3d shared_coords,
                   Shared_normal_array shared_normals,
                   Shared_color_array shared_colors,
                   Shared_tex_coord_array_2d shared_tex_coords,
                   Polygon_indices& coord_indices,
                   Polygon_indices& normal_indices,
                   Polygon_indices& tex_coord_indices)
{
  auto* coords = &*shared_coords;
  auto* normals = &*shared_normals;
  auto* colors = &*shared_colors;
  auto* tex_coords = &*shared_tex_coords;

  auto num_primitives = coord_indices.size();

  coords->add_to_scene(sg);
  sg->add_container(shared_coords);
  ifs->set_coord_array(shared_coords);

  if (!normals->empty()) {
    normals->add_to_scene(sg);
    sg->add_container(shared_normals);
    ifs->set_normal_array(shared_normals);
  }

  if (!colors->empty()) {
    colors->add_to_scene(sg);
    sg->add_container(shared_colors);
    ifs->set_color_array(shared_colors);
  }

  if (!tex_coords->empty()) {
    tex_coords->add_to_scene(sg);
    sg->add_container(shared_tex_coords);
    ifs->set_tex_coord_array(shared_tex_coords);
  }

  if (!normal_indices.empty()) {
    SGAL_assertion(!normals->empty());
    ifs->set_normal_per_vertex(true);
  }
  else if (!colors->empty()) {
    SGAL_assertion(!colors->empty());
    ifs->set_color_per_vertex(true);
  }

  // Convert to triangles or quads if possible.
  Boolean tris(true), quads(true);
  for (const auto& polygon : coord_indices) {
    auto n = polygon.size();
    SGAL_assertion(2 != n);
    if (3 != n) tris = false;
    if (4 != n) quads = false;
  }
  if (tris) {
    ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
    auto& tri_coord_indices = ifs->get_empty_triangle_coord_indices();
    to_tri_indices(coord_indices, tri_coord_indices);
    if (!normal_indices.empty()) {
      auto& tri_normal_indices = ifs->get_empty_triangle_normal_indices();
      to_tri_indices(normal_indices, tri_normal_indices);
    }
    if (!tex_coord_indices.empty()) {
      auto& tri_tex_coord_indices = ifs->get_empty_triangle_tex_coord_indices();
      to_tri_indices(tex_coord_indices, tri_tex_coord_indices);
    }
  }
  else if (quads) {
    ifs->set_primitive_type(Geo_set::PT_QUADS);
    auto& quad_coord_indices = ifs->get_empty_quad_coord_indices();
    to_quad_indices(coord_indices, quad_coord_indices);
    if (!normal_indices.empty()) {
      auto& quad_normal_indices = ifs->get_empty_quad_normal_indices();
      to_quad_indices(normal_indices, quad_normal_indices);
    }
    if (!tex_coord_indices.empty()) {
      auto& quad_tex_coord_indices = ifs->get_empty_quad_tex_coord_indices();
      to_quad_indices(tex_coord_indices, quad_tex_coord_indices);
    }
  }
  else {
    ifs->set_primitive_type(Geo_set::PT_POLYGONS);
    ifs->set_facet_coord_indices(std::move(coord_indices));
    if (!normal_indices.empty())
      ifs->set_facet_normal_indices(std::move(normal_indices));
    if (!tex_coord_indices.empty())
      ifs->set_facet_tex_coord_indices(std::move(tex_coord_indices));
  }

  ifs->set_num_primitives(num_primitives);
  ifs->facet_coord_indices_changed();
  ifs->add_to_scene(sg);

  return Loader_code::SUCCESS;
}

// \brief loads a scene graph represented in the obj file format from a stream.
Loader_code Loader::parse_obj(std::istream& is, Scene_graph* sg)
{
  sg->set_input_format_id(File_format_3d::ID_OBJ);
  auto* root = sg->initialize();            // obtain root

  // Construct arrays
  Shared_coord_array_3d shared_coords(new Coord_array_3d);
  SGAL_assertion(shared_coords);
  Shared_normal_array shared_normals(new Normal_array);
  SGAL_assertion(shared_normals);
  Shared_color_array shared_colors(new Color_array);
  SGAL_assertion(shared_colors);
  Shared_tex_coord_array_2d shared_tex_coords(new Tex_coord_array_2d);
  SGAL_assertion(shared_tex_coords);

  // Start scanning
  Vrml_scanner scanner(&is);
  scanner.push_state(4);
  bool done(false);
  Shared_appearance app;                        // define a place holder
  while (!done) {
    bool done_shape(false);

    // Create new containers
    Shared_shape shape(create_shape(sg));       // create a new Shape node
    root->add_child(shape);                     // add the new Shape node
    Shared_indexed_face_set ifs(create_ifs(sg));// create a new Ifs container
    Polygon_indices coord_indices;
    Polygon_indices normal_indices;
    Polygon_indices tex_coord_indices;

    auto coords = &*shared_coords;
    auto normals = &*shared_normals;
    auto colors = &*shared_colors;
    auto tex_coords = &*shared_tex_coords;

    while (!done && !done_shape) {
      Vrml_parser::symbol_type symbol(scanner.mylex());
      switch (symbol.token()) {
       case Vrml_parser::token::TOK_END:
        if (app) shape->set_appearance(app);
        done = true;
        break;

       case Vrml_parser::token::TOK_K_VERTEX:
        if (! read_obj_coords_and_colors(scanner, coords, colors))
          return Loader_code::FAILURE;
        break;

       case Vrml_parser::token::TOK_K_TEXTURE_COORDINATE:
        if (! read_obj_tex_coords(scanner, tex_coords))
          return Loader_code::FAILURE;
        break;

       case Vrml_parser::token::TOK_K_NORMAL:
        if (! read_obj_normals(scanner, normals)) return Loader_code::FAILURE;
        break;

       case Vrml_parser::token::TOK_K_FACET:
        if (! read_obj_facet(scanner, coord_indices, normal_indices,
                             tex_coord_indices, coords->size()))
          return Loader_code::FAILURE;
        break;

       case Vrml_parser::token::TOK_K_SMOOTH:
        if (! read_obj_line(scanner)) return Loader_code::FAILURE;
        break;

       case Vrml_parser::token::TOK_K_MATERIAL_LIB:
        {
          Shared_string name;
          if (! read_string(scanner, name)) return Loader_code::FAILURE;
          auto rc = parse_mtl(*name, sg);
          if (static_cast<int>(rc) < 0) return Loader_code::FAILURE;
        }
        break;

       case Vrml_parser::token::TOK_K_USE_MATERIAL:
        {
          // If an appearance exists, set it as the appearance of the current
          // shape, and mark it done.
          if (app) {
            shape->set_appearance(app);
            done_shape = true;
          }
          // Record the new appearance.
          Shared_string name;
          if (! read_string(scanner, name)) return Loader_code::FAILURE;
          auto cont = sg->get_container(*name);
          app = boost::dynamic_pointer_cast<Appearance>(cont);
          if (!app) {
            std::cerr << "Warning: Appearance " << "\"" << *name << "\""
                      << " is not defined" << std::endl;
          }
          break;
        }
        break;

       default: break;
      }
    }
    // Finsh creating a Shape node
    auto rc = update_ifs(sg, ifs, shared_coords, shared_normals, shared_colors,
                         shared_tex_coords, coord_indices, normal_indices,
                         tex_coord_indices);
    if (static_cast<int>(rc) < 0) return Loader_code::FAILURE;
    if (!app) {
      app = create_appearance(sg);
      shape->set_appearance(app);
    }
    shape->set_geometry(ifs);
    if (done) break;
  }
  scanner.pop_state();
  return Loader_code::SUCCESS;
}

SGAL_END_NAMESPACE
