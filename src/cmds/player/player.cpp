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
#include <vector>
#include <exception>

#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#if (defined _MSC_VER)
#define NOMINMAX 1
#endif
#include <boost/extension/shared_library.hpp>

#include "SGAL/sgal.hpp"
#include "SGAL/Loader_errors.hpp"

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

// The following symbols are defined, possibly by the header file X.h
// It is also defined by v8...
#undef True
#undef False
#undef None

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"
#include "Player.hpp"

#if (defined SGAL_USE_SCGAL)
SGAL_BEGIN_NAMESPACE
extern void scgal_init();
SGAL_END_NAMESPACE
#endif

namespace fi = boost::filesystem;
namespace ex = boost::extensions;

//! \breif
static std::string& error_message(std::string& str)
{
  str.append(": ");
#if (defined _MSC_VER)
  DWORD err;
  LPVOID lpMsgBuf;
  if ((err = ::GetLastError()) != NO_ERROR &&
       ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM,
                        nullptr,
                        err,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (LPSTR) &lpMsgBuf,
                        0,
                        nullptr) != 0)
  {
    str.append((LPSTR) lpMsgBuf);
    ::LocalFree(lpMsgBuf);
  }
#else
  str.append(dlerror());
#endif
  return str;
}

struct Shared_library_error : public std::logic_error {
  Shared_library_error(std::string& str) :
    std::logic_error(error_message(str)) {}
};

//! \breif
struct Shared_library_open_error : public Shared_library_error {
  Shared_library_open_error(std::string& library_name) :
    Shared_library_error(std::string("Cannot open library ").
                         append(library_name)) {}
};

//! \breif
struct Shared_library_symbol_error : public Shared_library_error {
  Shared_library_symbol_error(std::string& symbol_name) :
    Shared_library_error(std::string("Cannot retrieve symbol ").
                         append(symbol_name)) {}
};

//! \breif dynamically loads a shared library.
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

//! Main entry used only for debugging.
// When building python bindings, the player is build as a shared library
// without a main() function.
#if 0
int main(int argc, char* argv[])
{
  std::ifstream is("tetrahedron.wrl", std::ifstream::binary);
  if (!is.good()) {
    std::cerr << "Error: Failed to open tetrahedron.wrl!" << std::endl;
    return -1;
  }
  is.seekg(0, is.end);
  auto size = is.tellg();
  if (size < 0) {
    std::cerr << "Error: Failed to size tetrahedron.wrl!" << std::endl;
    return -1;
  }
  is.seekg (0, is.beg);
  auto* data = new char [size];
  if (!data) {
    std::cerr << "Error: Failed to allocate!" << std::endl;
    return -1;
  }
  is.read(data, size);    // read data as a block
  if (!is) std::cout << "Error: only " << is.gcount() << " could be read";
  is.close();
  auto rc = play(data, size, argc, argv);
  delete[] data;
  return rc;
}
#endif

/*! Player type. */
void Player::visualize()
{
  // Create a window manager:
#if (defined SGAL_USE_GLUT)
  SGAL::Glut_window_manager* wm = SGAL::Glut_window_manager::instance();
#elif defined(_WIN32)
  SGAL::Windows_window_manager* wm = SGAL::Windows_window_manager::instance();
#else
  SGAL::X11_window_manager* wm = SGAL::X11_window_manager::instance();
#endif

  m_scene.set_window_manager(wm);
  wm->set_scene(&m_scene);

  wm->init();
  m_scene.init_scene();

  wm->event_loop(m_scene.is_simulating());
  wm->clear();
  m_scene.clear_scene();
}

//! \brief constructs default
Player::Player() : m_option_parser(nullptr) {}

//! \brief constructs
Player::Player(int argc, char* argv[]) : m_option_parser(nullptr)
{ init(argc, argv); }

//! \brief constructs.
Player::Player(Arguments arguments) : m_option_parser(nullptr)
{ init(arguments); }

//! \brief destructs.
Player::~Player()
{
  if (m_option_parser) {
    delete m_option_parser;
    m_option_parser = nullptr;
  }
}

//! \brief initializes the player.
void Player::init(int argc, char* argv[])
{
  SGAL::initialize(argc, argv);
#if (defined SGAL_USE_SCGAL)
  SGAL::scgal_init();
#endif

  // Parse program options:
  m_option_parser = new Player_option_parser;
  m_option_parser->init();
  m_option_parser->parse(argc, argv);
  m_option_parser->apply();

  // Load plugins
  m_option_parser->for_each_plugin([](const std::string& plugin)
                                   {
                                     std::vector<std::string> strs;
                                     boost::split(strs, plugin,
                                                  boost::is_any_of(","));
                                     if (strs.size() < 2) {
                                       std::cerr << "Illegal plugin" << std::endl;
                                       return;
                                     }
                                     load_shared_library(strs[0], strs[1]);
                                   });
}

//! \brief initializes the player.
void Player::init(Arguments args)
{
  int argc = static_cast<int>(args.size()) + 1;
  char** argv = new char*[argc];
  char const * progname = "player";
  argv[0] = const_cast<char*>(progname);
  size_t i(1);
  std::for_each(args.begin(), args.end(),
                [&](const std::string& arg)
                { argv[i++] = const_cast<char*>(arg.c_str()); });

  init(argc, argv);

  if (argv) {
    delete [] argv;
    argv = nullptr;
  }
  argc = 0;
}

//! \brief cleans the player.
void Player::clean() { SGAL::clean(); }

//! \brief creates the player.
void Player::create()
{
  m_scene.set(m_option_parser);
  m_scene.create_scene();
}

//! \brief creates the player.
void Player::create(char* data, int size)
{
  m_scene.set(m_option_parser);
  m_scene.create_scene(data, size);
}

//! \brief destroys
void Player::destroy() { m_scene.destroy_scene(); }

//! operator()
void Player::operator()()
{
  create();                                     // create the scene
  if (m_scene.do_have_visual()) visualize();    // visualize the scene
  destroy();                                    // destroy the scene
}

/*! Operator */
void Player::operator()(char* data, int size)
{
  create(data, size);                           // create the scene
  if (m_scene.do_have_visual()) visualize();    // visualize the scene
  destroy();                                    // destroy the scene
}

//! \brief obtains the accumulated volume of all polyhedrons in the scene.
float Player::volume() { return m_scene.volume(); }

//! \brief obtains the accumulated surface area of all polyhedrons in the scene.
float Player::surface_area() { return m_scene.surface_area(); }

//! \brief obtains the attributes of all polyhedrons.
const SGAL::Polyhedron_attributes_array&
Player::get_polyhedron_attributes_array()
{
  m_scene.process_polyhedron_attributes_array(m_polyhedron_attributes_array);
  return m_polyhedron_attributes_array;
}

//! \brief main entry.
int main(int argc, char* argv[])
{
  Player player;
  try {
    player.init(argc, argv);
  }
  catch(Player_option_parser::Generic_option_exception& /* e */) {
    return 0;
  }
  catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  try {
    player();
  }
  catch(SGAL::Loader_error& e) {
    player.destroy();
    std::cerr << e.what();
    if (!e.filename().empty()) std::cerr << " (" << e.filename() << ")";
    std::cerr << std::endl;
    return -1;
  }
  catch(std::exception& e) {
    player.destroy();
    std::cerr << e.what() << std::endl;
    return -1;
  }
  player.clean();
  return 0;
}
