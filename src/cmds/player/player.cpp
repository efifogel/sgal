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
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/function.hpp>

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

namespace fi = boost::filesystem;
namespace ex = boost::extensions;

/*! \breif */
struct Shared_library_error : public std::logic_error {
  Shared_library_error(std::string& str) :
    std::logic_error(str.append(": ").append(dlerror())) {}
};

/*! \breif */
struct Shared_library_open_error : public Shared_library_error {
  Shared_library_open_error(std::string& library_name) :
    Shared_library_error(std::string("Cannot open library ").
                         append(library_name)) {}
};

/*! \breif */
struct Shared_library_symbol_error : public Shared_library_error {
  Shared_library_symbol_error(std::string& symbol_name) :
    Shared_library_error(std::string("Cannot retrieve symbol ").
                         append(symbol_name)) {}
};

/*! \breif */
void load_shared_library(std::string& library_name, std::string& function_name)
{
  // Create shared_library object with the absolute path to the shared library.
  ex::shared_library lib(library_name);

  // Attempt to open the shared library.
  if (!lib.open()) {
    throw Shared_library_open_error(library_name);
    return;
  }

  // Obtain the function
  boost::function<void()> init(lib.get<void>(function_name));

  // Check that the function was found.
  if (!init) {
    throw Shared_library_symbol_error(function_name);
    return;
  }

  // Call the function from the shared library 
  init();  
}

/*! Main entry */
int main(int argc, char* argv[])
{
  // SGAL::initialize(argc, argv);
  
#if (defined USE_CGAL)
  // try {
  //   std::string library_name = "libSCGAL.so";
  //   std::string function_name = "scgal_init";
  //   load_shared_library(library_name, function_name);
  // }
  // catch(std::exception& e) {
  //   std::cerr << e.what() << std::endl;
  //   return -1;
  // }
  SGAL::scgal_init();
#endif
  
#if (defined USE_EGO)
  try {
    std::string library_name = "libSEGO.so";
    std::string function_name = "sego_init";
    load_shared_library(library_name, function_name);
  }
  catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
#endif

  // Create a window manager:
#if (defined USE_GLUT)
  SGAL::Glut_window_manager* wm = SGAL::Glut_window_manager::instance();
#elif defined(_WIN32)
  SGAL::Windows_window_manager* wm = SGAL::Windows_window_manager::instance();
#else
  SGAL::X11_window_manager* wm = SGAL::X11_window_manager::instance();
#endif

  // Parse program options:
  Player_option_parser option_parser;
  option_parser.set_window_manager(wm);
  option_parser.init();
  try {
    option_parser(argc, argv);
    option_parser.apply();
  }
  catch(Player_option_parser::Generic_option_exception& /* e */) {
    return 0;
  }
  catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Initialize the scene:
  Player_scene scene(&option_parser);
  scene.set_window_manager(wm);
  wm->set_scene(&scene);
  
  try {
    scene.create_scene();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  try {
    wm->init(static_cast<SGAL::Uint>(argc), argv);
    scene.init_scene();
  }
  catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  wm->event_loop(scene.is_simulating());
  wm->clear();
  return 0;
}
