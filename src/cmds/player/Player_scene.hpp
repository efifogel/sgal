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
// $Id: Player_scene.hpp 6745 2008-08-24 12:13:21Z efif $
// $Revision: 6745 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef PLAYER_SCENE_HPP
#define PLAYER_SCENE_HPP

/*! The concrete player scene. The create_scene() and destroy_scene() virtual
 * member functions create and destory the scene respectively. These functions
 * must not issue window or graphics related commands. The init_scene() and
 * clear_scene() are responssible for initializing the wndow and graphics
 * contexts and clear them when not needed any longer.
 */

#include <string>
#include <list>
#include <stdexcept>

#include <boost/filesystem/path.hpp>

#include "SGAL/Types.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Scene.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Box.hpp"
#if defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#endif

namespace fi = boost::filesystem;

#include "Player_option_parser.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Tick_event;
class Draw_event;
class Reshape_event;
class Keyboard_event;
class Context;
class Configuration;
class Indexed_face_set;
class Box;

#if (defined USE_GLUT)
class Glut_window_manager;
class Glut_window_item;
#elif defined(_WIN32)
#if defined(PLAYER_CONTROL)
class Windows_window_item;
#else
//class Windows_window_manager;
class Windows_window_item;
#endif
#else
class X11_window_manager;
class X11_window_item;
#endif

SGAL_END_NAMESPACE

// CPlayerControlCtrl not in SGAL namespace
#if defined(_WIN32) && defined(PLAYER_CONTROL)
class CPlayerControlCtrl;
#endif

class Player_scene : public SGAL::Scene {
public:
#if (defined USE_GLUT)
  typedef SGAL::Glut_window_manager             Window_manager;
  typedef SGAL::Glut_window_item                Window_item;
#elif defined(_WIN32)
#if defined(PLAYER_CONTROL)
  typedef CPlayerControlCtrl                    Window_manager;
  typedef SGAL::Windows_window_item             Window_item;
#else
  typedef SGAL::Windows_window_manager          Window_manager;
  typedef SGAL::Windows_window_item             Window_item;
#endif
#else
  typedef SGAL::X11_window_manager              Window_manager;
  typedef SGAL::X11_window_item                 Window_item;
#endif

  /*! Constructor */
  Player_scene(Player_option_parser* option_parser);

  /*! Constructor */
  Player_scene();

  /*! Initialize */
  void init();

  /*! Destructor */
  virtual ~Player_scene(void);

  /*! Create the scene. */
  virtual void create_scene();

  /*! Destroy the scene. */
  virtual void destroy_scene();
  
  /*! Initialize the secene. */
  virtual void init_scene();

  /*! Check if the scene is initialized. */
  virtual bool is_scene_initiated() { return m_window_item != NULL; }

  /*! Clear the scene */  
  virtual void clear_scene();
  
  /*! Reshape the viewport of a window of the scene.
   * It is assumed that the window context is the current context.
   * \param window_item the window to reshape
   * \param width the new width of the window
   * \param height the new height of the window
   */
  virtual void reshape_window(SGAL::Window_item* window_item,
                              SGAL::Uint width, SGAL::Uint height);

  /*! Draw into a window of the scene.
   * It is assumed that the window context is the current context.
   * \param window_item the window to draw
   * \param dont_accumulate indicates that no accumulation should be performed
   */
  virtual void draw_window(SGAL::Window_item* window_item,
                           SGAL::Boolean dont_accumulate);

  /*! Handle a reshape event. */
  virtual void handle(SGAL::Reshape_event* event);
  
  /*! Handle a draw event. */
  virtual void handle(SGAL::Draw_event* event);
    
  /*! Handle a tick event. */
  virtual void handle(SGAL::Tick_event* event);

  /*! Handle a keyboard event. */
  virtual void handle(SGAL::Keyboard_event* event);
  
  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify(void);

  /*! Set the window manager. */
  template <typename Window_manager>
  void set_window_manager(Window_manager* manager)
  { m_window_manager = manager; }
  
  /*! Set the option parser. */
  void set_option_parser(Player_option_parser *option_parser)
  { m_option_parser = option_parser; }

  /*! Obtain the scene scene-graph. */
  SGAL::Scene_graph* get_scene_graph() { return m_scene_graph; }

  /*! Determine whether the scene does simulate something. */
  SGAL::Boolean is_simulating(void);

protected:
  /*! The window manager. */
  Window_manager* m_window_manager;

  /*! The window item. */
  Window_item* m_window_item;
  
  /*! The width of the window. */
  SGAL::Uint m_win_width;

  /* The height of the window. */
  SGAL::Uint m_win_height;

  /*! Work space scene graph. */
  SGAL::Scene_graph* m_scene_graph;

  /*! The context. */
  SGAL::Context* m_context;  

  /*! Option parser. */
  Player_option_parser* m_option_parser;

  SGAL::Boolean m_simulate;
  
  /*! \brief draws guides that separate the window into 4x5 rectangles. */
  void draw_grid();

  /*! \brief prints statistic information. */
  void print_stat();

  /*! \brief updates directory search. */
  void update_data_dirs();

  /*! \brief indulges user requests from the command line. */
  void indulge_user();

  /*! \brief print geometry information of Index_face_set. */
  void print_geometry_info(SGAL::Indexed_face_set* ifs);

  /*! \brief print geometry information of Box. */
  void print_geometry_info(SGAL::Box* box);

private:
  typedef std::list<fi::path>                                   Path_list;
  typedef Path_list::iterator                                   Path_iter;
    
  struct Add_dir {
    Path_list& m_dirs;
    Add_dir(Path_list& dirs) : m_dirs(dirs) {}
    void operator()(const std::string& dir) { m_dirs.push_back(dir); }
  };

  enum Error_id { FILE_NOT_FOUND, ILLEGAL_EXTENSION, UNABLE_TO_LOAD };
  struct Illegal_input : public std::logic_error {
    Illegal_input(Error_id err, const std::string& msg,
                  const std::string& filename) :
      std::logic_error(std::string(msg).append(" (").append(filename).append(")!"))       
    {}
  };

  struct Input_file_missing_error : public std::logic_error {
    Input_file_missing_error(std::string& str) : std::logic_error(str) {}
  };
  
  /*! The input file full-name. */
  std::string m_fullname;
  
  /*! A collection of directories to search files in. */
  Path_list m_dirs;
  
  /*! Valid file format names */
  static const char* s_file_format_names[];
};

#endif
