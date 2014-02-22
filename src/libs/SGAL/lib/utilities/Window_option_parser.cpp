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
// $Revision: 4989 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Window_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Window_option_parser::Window_option_parser() :
  m_window_opts("Window options"),
  m_win_width(720), m_win_height(576),
  m_win_x(0), m_win_y(0),
  m_full_screen(false)
{
  // Generic options:
  m_window_opts.add_options()
    ("window-width,W", po::value<SGAL::Uint>(&m_win_width), "window width")
    ("window-height,H", po::value<SGAL::Uint>(&m_win_height), "window height")
    ("window-x,X", po::value<SGAL::Uint>(&m_win_x),
     "window origin X position")
    ("window-y,Y", po::value<SGAL::Uint>(&m_win_y),
     "window origin Y position")
    ("full-screen,F", po::value<SGAL::Boolean>(&m_full_screen),
     "full screen")
    ;
}

/*! Destructor */
Window_option_parser::~Window_option_parser() {}

/*! \brief obtains the requested window width */
Boolean
Window_option_parser::get_window_width(const po::variables_map& variable_map,
                                       Uint& width) const
{
  if (variable_map.count("window-width") <= 0) return false;
  width = m_win_width;
  return true;
}

/*! \brief obtains the requested window height */
Boolean
Window_option_parser::get_window_height(const po::variables_map& variable_map,
                                        Uint& height) const
{
  if (variable_map.count("window-height") <= 0) return false;
  height = m_win_height;
  return true;
}

/*! \breif obtains the requested x-position of the window origin */
Boolean Window_option_parser::get_window_x(const po::variables_map& variable_map,
                                           Uint& x) const
{
  if (variable_map.count("window-x")) return false;
  x = m_win_x;
  return true;
}

/*! \breif obtains the requested y-position of the window origin */
Boolean Window_option_parser::get_window_y(const po::variables_map& variable_map,
                                           Uint& y) const
{
  if (variable_map.count("window-y")) return false;
  y = m_win_y;
  return true;
}

/*! \breif obtains the requested position of the window origin */
Boolean Window_option_parser::get_window_position(const po::variables_map&
                                                  variable_map,
                                                  Uint& x, Uint &y) const
{
  if ((variable_map.count("window-x") <= 0) &&
      (variable_map.count("window-y") <= 0))
    return false;
  x = m_win_x;
  y = m_win_y;
  return true;
}

/*! \breif obtains the flag that indicates whether full screen is requested */
Boolean
Window_option_parser::get_window_full_screen(const po::variables_map&
                                               variable_map,
                                             Boolean& full_sreen) const
{
  if (variable_map.count("full-screen") <= 0) return false;
  full_sreen = m_full_screen;
  return true;
}

SGAL_END_NAMESPACE
