// Copyright (c) 2015 Israel.
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

#ifndef SGAL_MODELING_OPTION_PARSER_HPP
#define SGAL_MODELING_OPTION_PARSER_HPP

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
#include "SGAL/Array_types.hpp"

SGAL_BEGIN_NAMESPACE

namespace po = boost::program_options;

class SGAL_SGAL_DECL Modeling_option_parser {
public:
  /*! Construct default.
   */
  Modeling_option_parser();

  /*! Destruct.
   */
  virtual ~Modeling_option_parser();

  /*! Obtain the modeling-option description.
   * \return the modeling-option description.
   */
  inline const po::options_description& get_modeling_opts() const;

  /*! Apply the options
   * \param variable_map
   */
  void apply(po::variables_map& variable_map);

  /*! Configure the given window manager.
   */
//   template <typename Font_manager>
//   void configure(const po::variables_map& variable_map,
//                  Font_manager* window_manager)
//   {
//     Uint width;
//     if (get_window_width(variable_map, width))
//       window_manager->set_window_width(width);

//     Uint height;
//     if (get_window_height(variable_map, height))
//       window_manager->set_window_height(height);

//     Uint x;
//     if (get_window_x(variable_map, x)) window_manager->set_window_x(x);

//     Uint y;
//     if (get_window_y(variable_map, y)) window_manager->set_window_y(y);

//     Boolean full_screen;
//     if (get_window_full_screen(variable_map, full_screen))
//       window_manager->set_full_screen(full_screen);
//   }

  /*! Obtain the font paths.
   * \param variable_map (in)
   * \return the font paths.
   */
  const String_array& get_paths(const po::variables_map& variable_map)
    const;

protected:
  /*! The modeling options. */
  po::options_description m_modeling_opts;

private:
  // The assignment operator cannot be generated (because some of the data
  // members are const pointers), so we suppress it explicitly.
  // We also suppress the copy constructor.
  Modeling_option_parser& operator=(const Modeling_option_parser&);
  Modeling_option_parser(const Modeling_option_parser&);

  // In C++11, VC2013, the following is supported:
  // Modeling_option_parser& operator=(const Modeling_option_parser&) = delete;
  // Modeling_option_parser(const Modeling_option_parser&) = delete;
};

//! \brief obtains the window-option description.
inline const po::options_description&
Modeling_option_parser::get_modeling_opts() const { return m_modeling_opts; }

//! \brief applies the options.
inline void Modeling_option_parser::apply(po::variables_map& /* variable_map */)
{}

SGAL_END_NAMESPACE

#endif
