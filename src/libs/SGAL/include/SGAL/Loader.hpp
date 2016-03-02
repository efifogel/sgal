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

#ifndef SGAL_LOADER_HPP
#define SGAL_LOADER_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Loader_errors.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Transform;
class Shape;
class Indexed_face_set;
class Coord_array_3d;
class Color_array;
class Appearance;
class Material;

class SGAL_SGAL_DECL Loader {
public:
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

  enum Return_code {RETRY = 1, SUCCESS = 0, FAILURE = -1};

  /*! Construct */
  Loader();

  /*! Load a scene graph from a stream.
   */
  Return_code load(std::istream& src_stream, Scene_graph* sg);

  /*! Load a scene graph from a buffer.
   */
  Return_code load(char* data, size_t size, Scene_graph* sg);

  /*! Load a scene graph from a buffer.
   */
  Return_code load(char* data, size_t size, const char* filename,
                   Scene_graph* sg);

  /*! Load a scene graph from a file.
   * \param[in] filename the name of the file to load.
   */
  Return_code load(const char* filename, Scene_graph* sg);

  /*! Load a scene graph from an stl stream.
   * \param[in] stl_stream the stream to load.
   * \param[in] sg the scene graph
   * \param[in] force indicates whether to force reading, assuming that the
   *            file is in the STL binary format. (Do not check whether the
   *            file starts with the token "solid".)
   */
  Return_code load_stl(std::istream& stl_stream, size_t size, Scene_graph* sg,
                       bool force = false);

  /*! Load a scene graph from an stl buffer.
   * \param[in] data the buffer that contains a model in the stl format.
   * \param[in] size the size of the buffer.
   * \param[in] sg the scene graph
   * \param[in] force indicates whether to force reading, assuming that the
   *            file is in the STL binary format. (Do not check whether the
   *            file starts with the token "solid".)
   */
  Return_code load_stl(char* data, size_t size, Scene_graph* sg,
                       bool force = false);

  /*! Load a scene graph from an stl file.
   * \param[in] filename the name of the STL file to load.
   * \param[in] sg the scene graph
   * \param[in] force indicates whether to force reading, assuming that the
   *            file is in the STL binary format. (Do not check whether the
   *            file starts with the token "solid".)
   */
  Return_code load_stl(const char* filename, Scene_graph* sg,
                       bool force = false);

  /*! Read a scene graph from a file in the STL binary format.
   */
  Return_code read_stl(std::istream& stl_stream, size_t size, Scene_graph* sg,
                       const Vector3f& color);

  /*! Read a traingle (1 normal and 3 vertices)
   */
  Return_code read_triangle(std::istream& stl_stream, Triangle& triangle);

private:
  typedef boost::shared_ptr<Transform>              Shared_transform;
  typedef boost::shared_ptr<Shape>                  Shared_shape;
  typedef boost::shared_ptr<Indexed_face_set>       Shared_indexed_face_set;
  typedef boost::shared_ptr<Coord_array_3d>         Shared_coord_array_3d;
  typedef boost::shared_ptr<Color_array>            Shared_color_array;
  typedef boost::shared_ptr<Appearance>             Shared_appearance;
  typedef boost::shared_ptr<Material>               Shared_material;

  /*! Indicates whether multiple shape nodes should represent the entire mesh
   * when colors are present. When colors are not present this flag has no
   * effect.
   * By default a single Shape node represents the entire mesh. When colors are
   * present, we store the colors in the color array. When m_multiple_shapes is
   * set to false and colors are present, we split the mesh into as many Shape
   * nodes as different colors.
   * \todo use a shader to combine the colors, when present, and phong shading.
   */
  Boolean m_multiple_shapes;

  /*! The filename if exists. */
  std::string m_filename;

  /*! Compute a new Indexed Face Set container. */
  Shared_indexed_face_set compute_ifs(Scene_graph* scene_graph, size_t count,
                                      std::list<Triangle>::const_iterator begin,
                                      std::list<Triangle>::const_iterator end);

  /*! Add a shape node to the scene. */
  Shared_shape compute_shape(Scene_graph* scene_graph,
                             Shared_transform transform,
                             const Vector3f& color);

  /*! Add a shape for every sub sequence of triangles with a distinguish color.
   */
  void add_shapes(Scene_graph* scene_graph, Shared_transform transform,
                  std::list<Triangle>& triangles, const Vector3f& color);

  /*! Add a colored shape.
   */
  void add_colored_shape(Scene_graph* scene_graph, Shared_transform transform,
                         std::list<Triangle>& triangles, const Vector3f& color);
};

SGAL_END_NAMESPACE

#endif
