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
// $Id: Player_option_parser.cpp 6010 2008-03-05 13:37:35Z ophirset $
// $Revision: 6010 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(PLAYER_CONTROL)
#include "stdafx.h"
#endif

#include <string>

#include "Player_option_parser.hpp"

#include "SGAL/Window_option_parser.hpp"
#if (defined USE_GLUT)
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
  m_window_manager(NULL),
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

/*! \brief parse the options */
void Player_option_parser::operator()(int argc, char * argv[])
{
  SGAL::Option_parser::operator()(argc, argv);
}

/*! \brief applies the options */
void Player_option_parser::apply()
{
  SGAL::Option_parser::apply();
  SGAL::Window_option_parser::apply(m_variable_map, m_window_manager);
}

/*! \brief obtains the base file-name */
bool Player_option_parser::get_file_name(std::string & name) const
{
  if (!m_variable_map.count("input-file")) return false;
  typedef std::vector<std::string> vs;
  name = m_variable_map["input-file"].as<vs>()[0];
  return true;
}

/*! Is the maximum number of vertices in the index array set? */
SGAL::Boolean
Player_option_parser::get_sub_index_buffer_size(SGAL::Uint & size) const
{
  if (!m_variable_map.count("sub-index-buffer-size")) return false;
  size = m_variable_map["sub-index-buffer-size"].as<SGAL::Uint>();
  return true;
}
