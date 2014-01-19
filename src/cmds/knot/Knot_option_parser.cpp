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
// $Id: Knot_option_parser.cpp 5029 2007-09-03 08:43:47Z efif $
// $Revision: 5029 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <stdlib.h>

#include "Knot_option_parser.hpp"

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
Knot_option_parser::Knot_option_parser() :
  m_knot_opts("Knot options"),
  m_solve(false),
  m_export(false),
  m_cycle_interval(1)
{
  m_knot_opts.add_options()
    ("solve,S", po::value<Boolean>(&m_solve)->default_value(false),
     "solve the puzzle")
    ("export,E",  po::value<Boolean>(&m_export)->default_value(false),
     "export in VRML format")
    ("cycle,C",  po::value<Scene_time>(&m_cycle_interval)->default_value(1),
     "animation cycle interval")
    ("head-pad-x", po::value<Uint>(), "Head padding along x-coordinate")
    ("head-pad-y", po::value<Uint>(), "Head padding along y-coordinate")
    ("head-pad-z", po::value<Uint>(), "Head padding along z-coordinate")
    ("tail-pad-x", po::value<Uint>(), "Tail padding along x-coordinate")
    ("tail-pad-y", po::value<Uint>(), "Tail padding along y-coordinate")
    ("tail-pad-z", po::value<Uint>(), "Tail padding along z-coordinate")
    ;
}

/*! Initialize */
void Knot_option_parser::init()
{
  m_visible_opts.add(m_window_opts).add(m_knot_opts);
  m_cmd_line_opts.add(m_window_opts).add(m_knot_opts);
  m_config_file_opts.add(m_window_opts).add(m_knot_opts);
  m_environment_opts.add(m_window_opts).add(m_knot_opts);
}

/*! \brief parses the options */
void Knot_option_parser::operator()(int argc, char* argv[])
{ SGAL::Option_parser::operator()(argc, argv); }

/*! \brief applies the options */
void Knot_option_parser::apply()
{
  SGAL::Option_parser::apply();
  SGAL::Window_option_parser::apply(m_variable_map);
}

/*! Obtain the head padding of the pieces within the volume along the
 * x-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_head_pad_x(Uint & head_pad_x)
{
  if (!m_variable_map.count("head-pad-x")) return false;
  head_pad_x = m_variable_map["head-pad-x"].as<Uint>();
  return true;
}

/*! Obtain the head padding of the pieces within the volume along the
 * y-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_head_pad_y(Uint & head_pad_y)
{
  if (!m_variable_map.count("head-pad-y")) return false;
  head_pad_y = m_variable_map["head-pad-y"].as<Uint>();
  return true;
}

/*! Obtain the head padding of the pieces within the volume along the
 * z-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_head_pad_z(Uint & head_pad_z)
{
  if (!m_variable_map.count("head-pad-z")) return false;
  head_pad_z = m_variable_map["head-pad-z"].as<Uint>();
  return true;
}

/*! Obtain the tail padding of the pieces within the volume along the
 * x-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_tail_pad_x(Uint & tail_pad_x)
{
  if (!m_variable_map.count("tail-pad-x")) return false;
  tail_pad_x = m_variable_map["tail-pad-x"].as<Uint>();
  return true;
}

/*! Obtain the tail padding of the pieces within the volume along the
 * y-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_tail_pad_y(Uint & tail_pad_y)
{
  if (!m_variable_map.count("tail-pad-y")) return false;
  tail_pad_y = m_variable_map["tail-pad-y"].as<Uint>();
  return true;
}

/*! Obtain the tail padding of the pieces within the volume along the
 * z-coordinate, and return true only if appears on the command line
 */
SGAL::Boolean Knot_option_parser::get_tail_pad_z(Uint & tail_pad_z)
{
  if (!m_variable_map.count("tail-pad-z")) return false;
  tail_pad_z = m_variable_map["tail-pad-z"].as<Uint>();
  return true;
}
