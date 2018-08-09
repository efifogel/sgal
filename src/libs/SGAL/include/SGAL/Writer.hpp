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

#ifndef SGAL_WRITER_HPP
#define SGAL_WRITER_HPP

#include <string>
#include <map>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Base_writer;

class SGAL_SGAL_DECL Writer {
public:
  /*! Destruct.
   */
  ~Writer();

  /*! Obtain the loader singleton.
   * \return the loader singleton.
   */
  static Writer* get_instance();

  /*! Register a writer.
   * \param[in] the new loader.
   */
  void doregister(size_t format, Base_writer* writer);

  /*! Write a scene to a file.
   * \param[in] scene_graph the scene graph that represents the scene.
   * \param[in] filename the output file name.
   * \param[in] format the format of the output stream.
   * \param[in] is_binary indicates whether the format is binary.
   */
  void operator()(Scene_graph* scene_graph, const std::string& filename,
                  size_t format, bool is_binary = false);

private:
  /*! Construct.
   */
  Writer();

  //! Write a scene to a file.
  void operator()(Scene_graph* sg, const std::string& filename,
                  std::ostream& os, size_t format, bool is_binary);

  //! Writers.
  std::map<size_t, Base_writer*> m_writers;

  //! The singleton.
  static Writer* s_instance;
};

SGAL_END_NAMESPACE

#endif
