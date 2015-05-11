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

#ifndef KNOT_OPTION_PARSER_HPP
#define KNOT_OPTION_PARSER_HPP

#include <string>

#include "SGAL/Option_parser.hpp"
#include "SGAL/Window_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

#if (defined USE_GLUT)
class Glut_window_manager;
#elif defined(_WIN32)
class Windows_window_manager;
#else
class X11_window_manager;
#endif

SGAL_END_NAMESPACE

class Knot_option_parser : public SGAL::Option_parser,
                           public SGAL::Window_option_parser
{
public:
  typedef SGAL::Uint                    Uint;
  typedef SGAL::Boolean                 Boolean;
  typedef SGAL::Scene_time              Scene_time;

  /*! Constructor */
  Knot_option_parser();

  /*! Destructor */
  virtual ~Knot_option_parser() {}

  /*! Initialize */
  void init();

  /*! Parse the options
   * \param argc
   * \param argv
   */
  void operator()(int argc, char * argv[]);

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

  /*! Solve the puzzle? */
  Boolean solve() const { return m_solve; }

  /*! Export in VRML format ? */
  Boolean export_vrml() const { return m_export; }

  /*! Obtain the animation cycle-interval */
  Scene_time get_cycle_interval() const { return m_cycle_interval; }

  /*! \brief obtains the head padding of the pieces within the volume along
   * the x-coordinate, and returns true only if appears on the command line
   */
  Boolean get_head_pad_x(Uint & head_pad_x);

  /*! \brief obtains the head padding of the pieces within the volume along
   * the y-coordinate, and returns true only if appears on the command line
   */
  Boolean get_head_pad_y(Uint & head_pad_y);

  /*! \brief obtains the head padding of the pieces within the volume along
   * the z-coordinate, and returns true only if appears on the command line
   */
  Boolean get_head_pad_z(Uint & head_pad_z);

  /*! \brief obtains the tail padding of the pieces within the volume along
   * the x-coordinate, and returns true only if appears on the command line
   */
  Boolean get_tail_pad_x(Uint & tail_pad_x);

  /*! \brief obtains the tail padding of the pieces within the volume along
   * the y-coordinate, and returns true only if appears on the command line
   */
  Boolean get_tail_pad_y(Uint & tail_pad_y);

  /*! \brief obtains the tail padding of the pieces within the volume along
   * the z-coordinate, and returns true only if appears on the command line
   */
  Boolean get_tail_pad_z(Uint & tail_pad_z);

protected:
  /*! The knot option description */
  boost::program_options::options_description m_knot_opts;

private:
  /*! Indicates whether to solve the puzzle */
  Boolean m_solve;

  /*! Indicates whether to export in VRML format */
  Boolean m_export;

  /*! The cycle interval of the time sensors */
  Scene_time m_cycle_interval;
};

//! \brief configures the window manager.
template <typename Window_manager>
void Knot_option_parser::configure(Window_manager* window_manager,
                                   SGAL::Scene_graph* scene_graph)
{
  Window_option_parser::configure(m_variable_map, window_manager);
  Option_parser::configure(scene_graph);
}

#endif
