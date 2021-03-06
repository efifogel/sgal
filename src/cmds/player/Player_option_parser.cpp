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

#if defined(PLAYER_CONTROL)
#include "stdafx.h"
#endif

#include <string>

#include "Player_option_parser.hpp"

#include "SGAL/Window_option_parser.hpp"
#if (defined SGAL_USE_GLUT)
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_manager.hpp"
#endif

namespace po = boost::program_options;

/*! Constructor */
Player_option_parser::Player_option_parser() :
  m_player_opts("Player options"),
  m_grid(false)
{
  m_player_opts.add_options()
    ("grid,G", po::value<SGAL::Boolean>(&m_grid)->default_value(false),
     "draw grid")
    ("display-texture-info",
     po::value<SGAL::Boolean>(&m_display_texture_info)->default_value(false),
     "display texture information")
    ("display-geometry-info",
     po::value<SGAL::Boolean>(&m_display_geometry_info)->default_value(false),
     "display geometry information")
    ("display-polyhedra-info",
     po::value<SGAL::Boolean>(&m_display_polyhedra_info)->default_value(false),
     "display polyhedra information")
     ("bench", po::value<SGAL::Boolean>(&m_bench)->default_value(false),
      "bench")
    ;
}

/*! Initialize */
void Player_option_parser::init()
{
  m_visible_opts.add(m_window_opts).add(m_player_opts);
  m_cmd_line_opts.add(m_window_opts).add(m_player_opts);
  m_config_file_opts.add(m_window_opts).add(m_player_opts);
  m_environment_opts.add(m_window_opts).add(m_player_opts);
}

//! \brief parses the options.
void Player_option_parser::parse(int argc, char* argv[])
{ SGAL::Option_parser::operator()(argc, argv); }

//! \brief applies the options.
void Player_option_parser::apply()
{
  SGAL::Option_parser::apply();
  SGAL::Window_option_parser::apply(m_variable_map);
}

//! \brief obtains the number of names of input files.
size_t Player_option_parser::get_num_input_files() const
{ return m_variable_map.count("input-file"); }

//! \brief obtains the base file-name.
const std::string& Player_option_parser::get_input_file(size_t id) const
{
  SGAL_assertion(id < m_variable_map.count("input-file"));
  typedef std::vector<std::string> vs;
  return m_variable_map["input-file"].as<vs>()[id];
}

/*! Is the maximum number of vertices in the index array set? */
SGAL::Boolean
Player_option_parser::get_sub_index_buffer_size(SGAL::Uint & size) const
{
  if (!m_variable_map.count("sub-index-buffer-size")) return false;
  size = m_variable_map["sub-index-buffer-size"].as<SGAL::Uint>();
  return true;
}
