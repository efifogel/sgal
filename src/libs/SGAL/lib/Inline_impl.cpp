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
// Author(s) : Efi Fogel        <efifogel@gmail.com>

#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#include "SGAL/Inline.hpp"
#include "SGAL/Loader_errors.hpp"

#include "Vrml_scanner.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief adds the container to the given scene.
void Inline::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

//! \brief cleans (generate) the children.
void Inline::clean_childs()
{
  Group::clean_childs();
  const auto& filename = m_url[0];

  // Open source file.
  std::ifstream is(filename);
  if (!is.good()) {
    throw Open_file_error(filename);
    return;
  }

  Vrml_scanner scanner(&is);
  // scanner.set_debug(1);
  Boolean maybe_stl;
  Vrml_parser parser(scanner, m_scene_graph, this, maybe_stl);
  auto rc = parser.parse();
  if (0 != rc) {
    throw Parse_error(filename);
    return;
  }

  return;
}

SGAL_END_NAMESPACE
