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

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;

class SGAL_SGAL_DECL Loader {
public:
  /*! Construct */
  Loader();

  /*! Load a scene graph from a file.
   */
  int load(const char* filename, Scene_graph* sg);

  /*! Load a scene graph from an stl file.
   * \param[in] filename the file name to parse
   * \param[in] sg the scene graph
   * \param[in] force indicates whether to force reading, assuming that the
   *            file is in the STL binary format. (Do not check whether the
   *            file starts with the token "solid".)
   */
  int load_stl(const char* filename, Scene_graph* sg, bool force = false);

  /*! Read a scene graph from a file in the STL binary format.
   */
  int read_stl(std::ifstream& stl_stream, Scene_graph* sg,
               const Vector3f& color);

  /*! Read a traingle (1 normal and 3 vertices)
   */
  void read_triangle(std::ifstream& stl_stream,
                     Vector3f& v0, Vector3f& v1, Vector3f& v2, Ushort& spacer);

private:
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
};

SGAL_END_NAMESPACE

#endif
