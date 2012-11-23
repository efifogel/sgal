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
// $Id: Player_option_parser.hpp 6155 2008-04-06 12:37:15Z efif $
// $Revision: 6155 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef PLAYER_OPTION_PARSER_HPP
#define PLAYER_OPTION_PARSER_HPP

#include <string>
#include <vector>

#include "SGAL/Option_parser.hpp"
#include "SGAL/Window_option_parser.hpp"

namespace po = boost::program_options;

SGAL_BEGIN_NAMESPACE

#if (defined USE_GLUT)
class Glut_window_manager;
#elif defined(_WIN32)
class Windows_window_manager;
#else
class X11_window_manager;
#endif

SGAL_END_NAMESPACE

class Player_option_parser : public SGAL::Option_parser,
                             public SGAL::Window_option_parser
{
public:
  /*! Constructor */
  Player_option_parser();

  /*! Destructor */
  virtual ~Player_option_parser() {}

  /*! Set the window manager */
  template <typename Window_manager>
  void set_window_manager(Window_manager * window_manager)
  { m_window_manager = window_manager; }
  
  /*! Initialize */
  void init();

  /*! Parse the options
   * \param argc
   * \param argv
   */
  void operator()(int argc, char * argv[]);

  /*! Apply the options */
  void apply();
  
  /*! Obtain the base file-name
   * \param name the returned file name
   * \return true if a name was specified on the command line, false otherwise
   */
  bool get_file_name(std::string & name) const;

  typedef std::vector<std::string>      Input_path;
  typedef Input_path::const_iterator    Input_path_const_iterator;
  
  Input_path_const_iterator dirs_begin()
  { return m_variable_map["input-path"].as<Input_path>().begin(); }

  Input_path_const_iterator dirs_end()
  { return m_variable_map["input-path"].as<Input_path>().end(); }

  template <class UnaryFunction>
  UnaryFunction for_each_dir(UnaryFunction func)
  {
    if (!m_variable_map.count("input-path")) return func;
    return std::for_each(dirs_begin(), dirs_end(), func);
  }
  
  /*! Display a grid? */
  bool get_draw_grid() const { return m_grid; }

  /*! Do display texture information? */
  SGAL::Boolean get_display_texture_info() const
  { return m_display_texture_info; }

  /*! Do display geometry information? */
  SGAL::Boolean get_display_geometry_info() const
  { return m_display_geometry_info; }
  
  /*! Do perform a benchmark? */
  SGAL::Boolean get_bench() const { return m_bench; }
  
  /*! \brief is the maximum number of vertices in the index array set? */
  SGAL::Boolean get_sub_index_buffer_size(SGAL::Uint & size) const;
  
protected:
  /*! The player option description */
  boost::program_options::options_description m_player_opts;
  
private:
#if (defined USE_GLUT)
  typedef SGAL::Glut_window_manager             Window_manager;
#elif defined(_WIN32)
  typedef SGAL::Windows_window_manager          Window_manager;
#else
  typedef SGAL::X11_window_manager              Window_manager;
#endif

  /*! The window manager */
  Window_manager * m_window_manager;

  /*! Indicate whether to draw a grid */
  SGAL::Boolean m_grid;

  /*! Indicates whether to display texture information */
  SGAL::Boolean m_display_texture_info;

  /*! Indicates whether to display geometry information */
  SGAL::Boolean m_display_geometry_info;

  /*! Indicate whether to perform a benchmark */
  SGAL::Boolean m_bench;

  /*! The maximum number of vertices in the index array */
  SGAL::Uint m_sub_index_buffer_size;
};

#endif
