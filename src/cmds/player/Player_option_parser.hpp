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
  /*! Construct. */
  Player_option_parser();

  /*! Destruct. */
  virtual ~Player_option_parser() {}

  /*! Initialize. */
  void init();

  /*! Parse the options.
   * \param argc (in)
   * \param argv (in)
   */
  void parse(int argc, char* argv[]);

  /*! Apply the options.
   */
  void apply();

  /*! Configure the scene graph.
   * \param[in] scene_graph The scene graph.
   */
  void configure_scene_graph(SGAL::Scene_graph* scene_graph);

  /*! Configure the window manager
   * \param[in] window_manage The window manager.
   */
  template <typename WindowManager>
  void configure_window_manager(WindowManager* window_manage);

  /*! Obtain the number of names of input files.
   * \return the number of names of input files.
   */
  size_t get_num_input_files() const;

  /*! Obtain the name of an input file.
   * \param[in] the id of the file name to return;
   * \return the name of an input file.
   */
  const std::string& get_input_file(size_t id) const;

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

//! \brief configures the scene graph.
inline void
Player_option_parser::configure_scene_graph(SGAL::Scene_graph* scene_graph)
{ Option_parser::configure(scene_graph); }

//! \brief configures the window manager.
template <typename WindowManager>
void
Player_option_parser::configure_window_manager(WindowManager* window_manager)
{ Window_option_parser::configure(m_variable_map, window_manager); }

#endif
