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

#ifndef PLAYER_OPTION_PARSER_HPP
#define PLAYER_OPTION_PARSER_HPP

#include <string>
#include <vector>

#include "SGAL/Option_parser.hpp"
#include "SGAL/Window_option_parser.hpp"

namespace po = boost::program_options;

SGAL_BEGIN_NAMESPACE

class scene_graph;

SGAL_END_NAMESPACE

class Player_option_parser : public SGAL::Option_parser,
                             public SGAL::Window_option_parser
{
public:
  /*! Constructor. */
  Player_option_parser();

  /*! Destructor. */
  virtual ~Player_option_parser() {}

  /*! Initialize. */
  void init();

  /*! Parse the options.
   * \param argc (in)
   * \param argv (in)
   */
  void operator()(int argc, char* argv[]);

  /*! Apply the options.
   */
  void apply();

  /*! Configure the window manager and the scene graph.
   * \param window_manage The window manager.
   * \param scene_graph The scene graph.
   */
  template <typename Window_manager>
  void configure(Window_manager* window_manage,
                 SGAL::Scene_graph* scene_graph);

  /*! Obtain the base file-name.
   * \param name the returned file name.
   * \return true if a name was specified on the command line, false otherwise.
   */
  bool get_file_name(std::string& name) const;

  /*! Determines whether to display a grid.
   */
  bool get_draw_grid() const { return m_grid; }

  /*! Determines whether to display texture information.
   */
  SGAL::Boolean get_display_texture_info() const
  { return m_display_texture_info; }

  /*! Determines whether to display geometry information.
   */
  SGAL::Boolean get_display_geometry_info() const
  { return m_display_geometry_info; }

  /*! Determines whether to perform a benchmark.
   */
  SGAL::Boolean get_bench() const { return m_bench; }

  /*! \brief Determines whether the maximum number of vertices in the index
   * array is set.
   */
  SGAL::Boolean get_sub_index_buffer_size(SGAL::Uint & size) const;

  /*! Determine whether the operation is interactive.
   * \return true if the operation is interactive; false otherwise.
   */
  SGAL::Boolean is_interactive() const;

protected:
  /*! The player option description. */
  boost::program_options::options_description m_player_opts;

  /*! Indicate whether to draw a grid. */
  SGAL::Boolean m_grid;

  /*! Indicates whether to display texture information. */
  SGAL::Boolean m_display_texture_info;

  /*! Indicates whether to display geometry information. */
  SGAL::Boolean m_display_geometry_info;

  /*! Indicate whether to perform a benchmark. */
  SGAL::Boolean m_bench;

  /*! The maximum number of vertices in the index array. */
  SGAL::Uint m_sub_index_buffer_size;
};

//! \brief configures the window manager.
template <typename Window_manager>
void Player_option_parser::configure(Window_manager* window_manager,
                                     SGAL::Scene_graph* scene_graph)
{
  Window_option_parser::configure(m_variable_map, window_manager);
  Option_parser::configure(scene_graph);
}

#endif
