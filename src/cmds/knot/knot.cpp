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

#include <iostream>
#include <fstream>

#include "SGAL/sgal.hpp"
#if (defined SGAL_USE_GLUT)
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_manager.hpp"
#endif

// The preprocessor symbol 'Success' is defined, possibly by the header file X.h
// It is also defined by Eigen3...
#undef Success

#include "Knot_scene.hpp"
#include "Knot_option_parser.hpp"

/*! Main entry */
int main(int argc, char* argv[])
{
  SGAL::initialize(argc, argv);

  // Parse program options:
  Knot_option_parser option_parser;
  option_parser.init();
  try {
    option_parser(argc, argv);
    option_parser.apply();
  }
  catch(Knot_option_parser::Generic_option_exception & /* e */) {
    return 0;
  }
  catch(std::exception & e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Create the scene:
  Knot_scene scene(option_parser);
  try {
    scene.create_scene();
  }
  catch (Knot_scene::Illegal_input & e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  // Create a window manager:
#if (defined SGAL_USE_GLUT)
  SGAL::Glut_window_manager * wm = SGAL::Glut_window_manager::instance();
#elif defined(_WIN32)
  SGAL::Windows_window_manager * wm = SGAL::Windows_window_manager::instance();
#else
  SGAL::X11_window_manager * wm = SGAL::X11_window_manager::instance();
#endif

  // Initialize the visual:
  scene.set_window_manager(wm);
  wm->set_scene(&scene);

  try {
    wm->init();
    scene.init_scene();
  }
  catch(std::exception & e) {
    std::cerr << e.what() << std::endl;
    scene.destroy_scene();
    return -1;
  }
  wm->event_loop(scene.is_simulating());
  scene.clear_scene();
  scene.destroy_scene();
  return 0;
}
