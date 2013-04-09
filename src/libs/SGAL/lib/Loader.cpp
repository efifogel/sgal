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
// $Id: $
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Scene_graph.hpp"

#include "wrlFlexLexer.hpp"

extern SGAL::Scene_graph* scene_graph;

SGAL_BEGIN_NAMESPACE

int Loader::load(const char* filename, Scene_graph* sg)
{
  // Open source file:
  std::ifstream src_stream(filename);

  wrlFlexLexer flexLexer(&src_stream);
  flexLexer.m_lineno = 1;
  // flexLexer.set_debug(1);

  // Parse & export:
  scene_graph = sg;

  Vrml_parser parser;  
  if (parser.parse()) {
    std::cerr << "Failed to parse " << filename << "!" << std::endl;
    return -1;
  }
  return 0;
}

SGAL_END_NAMESPACE
