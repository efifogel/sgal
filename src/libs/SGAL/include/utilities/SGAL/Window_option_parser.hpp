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
// $Revision: 4988 $
//
// Author(s)     : Efi Fogel

#ifndef SGAL_WINDOW_OPTION_PARSER_HPP
#define SGAL_WINDOW_OPTION_PARSER_HPP

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
#endif
#include <boost/program_options.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class SGAL_SGAL_DECL Window_option_parser {
public:
  /*! Constructor */
  Window_option_parser();

  /*! Destructor */
  virtual ~Window_option_parser();

  /*! Obtain the window-option description */
  inline const po::options_description& get_window_opts() const
  { return m_window_opts; }

  /*! Apply the options
   * \param variable_map
   */
  void apply(po::variables_map& variable_map);

  /*! Configure the given window manager.
   */
  template <typename Window_manager>
  void configure(const po::variables_map& variable_map,
                 Window_manager* window_manager)
  {
    Uint width;
    if (get_window_width(variable_map, width))
      window_manager->set_window_width(width);

    Uint height;
    if (get_window_height(variable_map, height))
      window_manager->set_window_height(height);

    Uint x;
    if (get_window_x(variable_map, x)) window_manager->set_window_x(x);

    Uint y;
    if (get_window_y(variable_map, y)) window_manager->set_window_y(y);

    Boolean full_screen;
    if (get_window_full_screen(variable_map, full_screen))
      window_manager->set_full_screen(full_screen);
  }

  /*! Obtain the requested window width
   * \param width the requested width
   * \return true if a particular width is requested, and false otherwise.
   */
  Boolean get_window_width(const po::variables_map& variable_map,
                           Uint& width) const;

  /*! Obtain the requested window height
   * \param height the requested height
   * \return true if a particular height is requested, and false otherwise.
   */
  Boolean get_window_height(const po::variables_map& variable_map,
                            Uint& height) const;

  /*! Obtain the requested x-position of the window origin
   * \param x the requested x-coordinate of the window lower-left corner
   * \return true if a particular position is requested, and false otherwise.
   */
  Boolean get_window_x(const po::variables_map& variable_map, Uint& x) const;

  /*! Obtain the requested y-position of the window origin
   * \param y the requested y-coordinate of the window lower-left corner
   * \return true if a particular position is requested, and false otherwise.
   */
  Boolean get_window_y(const po::variables_map& variable_map, Uint& y) const;

  /*! Obtain the requested position of the window origin
   * \param x the requested x-coordinate of the window lower-left corner
   * \param y the requested y-coordinate  of the window lower-left corner
   * \return true if a particular position is requested, and false otherwise.
   */
  Boolean get_window_position(const po::variables_map& variable_map,
                              Uint& x, Uint& y) const;

  /*! Obtain the boolean flag that indicates whether full screen is requested
   * \param full_sreen indicates whether full screen is requested
   * \return true if a prticular value is requested, and false otherwise.
   */
  Boolean get_window_full_screen(const po::variables_map& variable_map,
                                 Boolean& full_sreen) const;

protected:
  /*! The window options */
  po::options_description m_window_opts;

private:
  /*! The window width */
  Uint m_win_width;

  /*! The window height */
  Uint m_win_height;

  /*! The window origin X position */
  Uint m_win_x;

  /*! The window origin Y position */
  Uint m_win_y;

  /*! Indicates to open the application in full screen */
  Boolean m_full_screen;

  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Window_option_parser& operator=(const Window_option_parser&);
  Window_option_parser(const Window_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Window_option_parser& operator=(const Window_option_parser&) = delete;
  // Window_option_parser(const Window_option_parser&) = delete;
};

//! \brief applies the options
inline void Window_option_parser::apply(po::variables_map& /* variable_map */)
{}

SGAL_END_NAMESPACE

#endif
