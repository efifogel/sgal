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
// $Id: player.cpp 6010 2008-03-05 13:37:35Z ophirset $
// $Revision: 6010 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#if (defined USE_GLUT)
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_manager.hpp"
#endif

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"

#if (defined USE_CGAL)
SGAL_BEGIN_NAMESPACE
extern void scgal_init();
SGAL_END_NAMESPACE
#endif

/*! Main entry */
int main(int argc, char * argv[])
{
  // SGAL::initialize(argc, argv);
#if (defined USE_CGAL)
  SGAL::scgal_init();
#endif

  // Create a window manager:
#if (defined USE_GLUT)
  SGAL::Glut_window_manager * wm = SGAL::Glut_window_manager::instance();
#elif defined(_WIN32)
  SGAL::Windows_window_manager * wm = SGAL::Windows_window_manager::instance();
#else
  SGAL::X11_window_manager * wm = SGAL::X11_window_manager::instance();
#endif

  // Parse program options:
  Player_option_parser option_parser;
  option_parser.set_window_manager(wm);
  option_parser.init();
  try {
    option_parser(argc, argv);
    option_parser.apply();
  } catch(Player_option_parser::Generic_option_exception & /* e */) {
    return 0;
  } catch(std::exception & e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Initialize the scene:
  Player_scene scene(&option_parser);
  scene.set_window_manager(wm);
  wm->set_scene(&scene);
  
  try {
    scene.create_scene();
  } catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  try {
    wm->init(static_cast<SGAL::Uint>(argc), argv);
    scene.init_scene();
  } catch(std::exception & e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  wm->event_loop(scene.is_simulating());
  wm->clear();
  return 0;
}
