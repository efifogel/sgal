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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOADER_HPP
#define SGAL_LOADER_HPP

#include <map>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Loader_code.hpp"
#include "SGAL/Loader_errors.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Indices_types.hpp"

namespace ba = boost::algorithm;

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Group;
class Shape;
class Indexed_face_set;
class Coord_array_3d;
class Color_array_3d;
class Normal_array;
class Tex_coord_array_2d;
class Appearance;
class Material;
class Base_loader;

/*! \class Loader Loader.hpp
 * Loader is a function object that loads a scene from a file or an input
 * stream. It is a singleton that utilizes loaders of sepecific input formats.
 * Several input formats are supported, i.e., VRML, STL test & binary, OFF, and
 * OBJ. Also, external libraries can register additional specific loaders; for
 * example, the SGAL_dxf library, registers a specific loader that supports the
 * dxf format.
 *
 * The loaded scene is represented as a scene graph, which is a directed acyclic
 * graph of nodes. The root of the constructed graph is provided as a mandatory
 * argument of the operator(s) of the loader. In addition, the operator(s) of
 * the loader accept as input the input file name, the input buffer, or the
 * input stream, or a combination of the above. They also accept an argument,
 * referred to as Scene_graph, which holds several parameters of the scene graph
 * including the root of the graph that represents the entire scene.
 *
 * The loader is reentrant; that is, it can be invoked several times with
 * different input values to construct several sub graphs, respectively, that
 * represent corresponding pieces of one large scene. These sub-graphs can be
 * combined together to represent one large scene.
 */

class SGAL_SGAL_DECL Loader {
public:
  /*! Destruct.
   */
  ~Loader();

  /*! Obtain the loader singleton.
   * \return the loader singleton.
   */
  static Loader* get_instance();

  /*! Load a scene from a file.
   * \param[in] filename the name of the file to load.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code operator()(const char* filename,
                         Scene_graph* scene_graph, Group* root);

  /*! Load a scene from a buffer.
   * \param[in] data the buffer that contains the model description.
   * \param[in] size the size of the buffer.
   * \param[in] filename the name of the source file of the data.
   * \param[in] scene_graph the scene graph
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code operator()(char* data, size_t size, const char* filename,
                         Scene_graph* scene_graph, Group* root);

  /*! Load a scene from a buffer.
   * \param[in] data the buffer that contains the model description.
   * \param[in] size the size of the buffer.
   * \param[in] scene_graph the scene graph
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code operator()(char* data, size_t size,
                         Scene_graph* scene_graph, Group* root);

  /*! Load a scene graph from an stl file.
   * \param[in] is the stream to load.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code load_stl(std::istream& is, Scene_graph* scene_graph, Group* root);

  /*! Load a scene graph from an stl stream.
   * \param[in] is the stream to load.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code load_stl(std::istream& is, size_t size,
                       Scene_graph* scene_graph, Group* root);

  /*! Load a scene graph from an stl buffer.
   * \param[in] data the buffer that contains a model in the STL format.
   * \param[in] size the size of the buffer.
   * \param[in] scene_graph the scene graph
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code load_stl(char* data, size_t size,
                       Scene_graph* scene_graph, Group* root);

  /*! Read a scene graph from a stream in the STL binary format.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code read_stl(std::istream& is, size_t size,
                       Scene_graph* scene_graph, Group* root,
                       const Vector3f& color);

  /*! Load a scene graph represented in the off file format from a stream.
   * Observe that the magic string has been consumed.
   * \param[in] is the stream to load.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code load_off(std::istream& is, const boost::smatch& what,
                       Scene_graph* scene_graph, Group* root);

  /*! Parse a scene from a text stream.
   * \param[in] is the text stream to parse.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed sub graph.
   */
  Loader_code parse(std::istream& is, Scene_graph* scene_graph, Group* root);

  /*! Load a scene graph represented in the obj file format from a stream.
   * \param[in] is the stream to load.
   * \param[in] scene_graph the scene graph.
   * \param[in] root the root of the constructed (sub) graph.
   */
  Loader_code parse_obj(std::istream& is, Scene_graph* scene_graph, Group* root);

  /*! Prase a material library file, which is part of the obj format.
   */
  Loader_code parse_mtl(const std::string& filename, Scene_graph* sg);

  /*! Set the flag that indicates whether to construct multiple shapes when
   * colors are present (one shape per color).
   */
  void set_multiple_shapes(bool multiple_shapes);

  /*! Determine whether to construct multiple shapes when colors are present
   * (one shape per color).
   */
  bool multiple_shapes() const;

  /*! Register a loader.
   * \param[in] the new loader.
   */
  void doregister(const String& extension, Base_loader* loader);

protected:
  class Triangle {
  public:
    Triangle() : spacer(0) {}
    Boolean is_colored() const { return spacer & 0x0001; }
    void set_color(Vector3f& color) const
    {
      auto red = static_cast<Float>((spacer >> 1) & 0x001f) / 31.0f;
      auto green = static_cast<Float>((spacer >> 6) & 0x001f) / 31.0f;
      auto blue = static_cast<Float>((spacer >> 11) & 0x001f) / 31.0f;
      color.set(red, green, blue);
    }

    Vector3f v0, v1, v2;
    Ushort spacer;
  };

  /*! Read a traingle (1 normal and 3 vertices)
   */
  Loader_code read_triangle(std::istream& is, Triangle& triangle);

private:
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Indexed_face_set>       Shared_indexed_face_set;
  typedef boost::shared_ptr<Coord_array_3d>         Shared_coord_array_3d;
  typedef boost::shared_ptr<Color_array_3d>         Shared_color_array_3d;
  typedef boost::shared_ptr<Normal_array>           Shared_normal_array;
  typedef boost::shared_ptr<Tex_coord_array_2d>     Shared_tex_coord_array_2d;
  typedef boost::shared_ptr<Appearance>             Shared_appearance;
  typedef boost::shared_ptr<Material>               Shared_material;

  /*! Construct.
   */
  Loader();

  /*! Indicates whether multiple shape nodes should represent the entire mesh
   * when colors are present. When colors are not present this flag has no
   * effect.
   * By default a single Shape node represents the entire mesh. When colors are
   * present, we store the colors in the color array. When m_multiple_shapes is
   * set to true and colors are present, we split the mesh into as many Shape
   * nodes as different colors.
   * \todo use a shader to combine the colors, when present, and phong shading.
   */
  Boolean m_multiple_shapes;

  /*! The filename if exists. */
  std::string m_filename;

  /*! Create a new Indexed Face Set container. */
  Shared_indexed_face_set create_ifs(Scene_graph* scene_graph, size_t count,
                                      std::list<Triangle>::const_iterator begin,
                                      std::list<Triangle>::const_iterator end);

  /*! Create a new Shape node.
   */
  Shared_shape create_shape(Scene_graph* scene_graph) const;

  /*! Create a shape node.
   */
  Shared_shape create_shape(Scene_graph* scene_graph, const Vector3f& color);

  /*! Add a shape for every sub sequence of triangles with a distinguish color.
   */
  void add_shapes(Scene_graph* scene_graph, Group* group,
                  std::list<Triangle>& triangles, const Vector3f& color);

  /*! Add a single shape for all triangles.
   */
  void add_shape(Scene_graph* scene_graph, Group* group,
                 std::list<Triangle>& triangles);

  /*! Add a colored shape.
   */
  void add_colored_shape(Scene_graph* scene_graph, Group* group,
                         std::list<Triangle>& triangles, const Vector3f& color);

  /*! Create a new Appearance container.
   */
  Shared_appearance create_appearance(Scene_graph* scene_graph) const;

  /*! Create a new Appearance container.
   */
  Shared_appearance create_appearance(Scene_graph* scene_graph,
                                      const std::string& name) const;

  /*! Create a new Material container.
   */
  Shared_material create_material(Scene_graph* scene_graph) const;

  /*! Create a new IndexedFaceSet container.
   */
  Shared_indexed_face_set create_ifs(Scene_graph* scene_graph) const;

  /*! Update an IndexedFaceSet container.
   */
  Loader_code update_ifs(Scene_graph* scene_graph,
                         Shared_indexed_face_set ifs,
                         Shared_coord_array_3d shared_coords,
                         Shared_normal_array shared_normals,
                         Shared_color_array_3d shared_colors,
                         Shared_tex_coord_array_2d shared_tex_coords,
                         Polygon_indices& coord_indices,
                         Polygon_indices& normal_indices,
                         Polygon_indices& tex_coord_indices);

  //! case-independent (ci) compare-less binary function
  struct ci_less : std::binary_function<std::string, std::string, bool> {
    bool operator() (const String& s1, const String& s2) const {
      return ba::lexicographical_compare(s1, s2, boost::is_iless());
    }
  };

  //! Loaders.
  std::map<String, Base_loader*, ci_less> m_loaders;

  //! The singleton.
  static Loader* s_instance;
};

/*! \brief sets the flag that indicates whether to construct multiple shapes when
 * colors are present.
 */
inline void Loader::set_multiple_shapes(bool multiple_shapes)
{ m_multiple_shapes = multiple_shapes; }

/*! \brief determines whether to construct multiple shapes when colors are
 * present.
 */
inline bool Loader::multiple_shapes() const { return m_multiple_shapes; }

SGAL_END_NAMESPACE

#endif
