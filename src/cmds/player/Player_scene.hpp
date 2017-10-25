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
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Scene.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Box.hpp"
#if defined(SGAL_USE_V8)
#include "SGAL/Array_buffer_allocator.hpp"
#endif
#if defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#endif

#include "Player_option_parser.hpp"

namespace fi = boost::filesystem;

#if defined(SGAL_USE_V8)
namespace v8 {
  class Isolate;
};
#endif

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Tick_event;
class Draw_event;
class Reshape_event;
class Keyboard_event;
class Mouse_event;
class Motion_event;
class Passive_motion_event;
class Context;
class Configuration;
class Indexed_face_set;
class Box;
class Image;
class Matrix4f;
class Polyhedron_attributes_array;

#if (defined SGAL_USE_GLUT)
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
  typedef boost::shared_ptr<SGAL::Image>          Shared_image;

#if (defined SGAL_USE_GLUT)
  typedef SGAL::Glut_window_manager               Window_manager;
  typedef SGAL::Glut_window_item                  Window_item;
#elif defined(_WIN32)
#if defined(PLAYER_CONTROL)
  typedef CPlayerControlCtrl                      Window_manager;
  typedef SGAL::Windows_window_item               Window_item;
#else
  typedef SGAL::Windows_window_manager            Window_manager;
  typedef SGAL::Windows_window_item               Window_item;
#endif
#else
  typedef SGAL::X11_window_manager                Window_manager;
  typedef SGAL::X11_window_item                   Window_item;
#endif

  /*! Construct default. */
  Player_scene();

  /*! Construct. */
  Player_scene(Player_option_parser* option_parser);

  /*! Set the player option parser.
   * \param[in] option_parser the player option parser.
   */
  void set(Player_option_parser* option_parser);

  /*! Destructor. */
  virtual ~Player_scene(void);

  /*! Create the scene. */
  virtual void create_scene(char* data, int size);

  /*! Create the scene. */
  virtual void create_scene();

  /*! Destroy the scene. */
  virtual void destroy_scene();

  /*! Initialize the secene. */
  virtual void init_scene();

  /*! Check whether the scene is initialized. */
  virtual bool is_scene_initiated();

  /*! Clear the scene */
  virtual void clear_scene();

  /*! Initialize the window. Typically used to create a context.
   * \param[in] window_item the window to initialize
   */
  virtual void init_window(SGAL::Window_item* window_item,
                           SGAL::Uint width, SGAL::Uint height);

  /*! Clear a window. Used to clear a context.
   * \param[in] window_item the window to initialize
   */
  virtual void clear_window(SGAL::Window_item* window_item);

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
   * \param[in] window_item the window to draw
   * \param[in] dont_accumulate indicates that no accumulation should be
   *                            performed
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

  /*! Handle a mouse event. */
  virtual void handle(SGAL::Mouse_event* event);

  /*! Handle a motion event. */
  virtual void handle(SGAL::Motion_event* event);

  /*! Handle a passive motion event. */
  virtual void handle(SGAL::Passive_motion_event* event);

  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify(void);

  /*! Set the window manager. */
  template <typename Window_manager>
  void set_window_manager(Window_manager* manager)
  { m_window_manager = manager; }

  /*! Set the option parser. */
  void set_option_parser(Player_option_parser* option_parser);

  /*! Obtain the scene scene-graph. */
  SGAL::Scene_graph* get_scene_graph() const;

#if defined(SGAL_USE_V8)
  /*! Obtain an isolated instance of the V8 engine.
   * \return an isolated instance of the V8 engine.
   */
  v8::Isolate* get_isolate();
#endif

  /*! Determine whether the scene does simulate something. */
  SGAL::Boolean is_simulating(void) const;

  /*! Determine whether the application has a visual.
   * \return true if the application has a visual; false otherwise.
   */
  SGAL::Boolean do_have_visual() const;

  /*! Determine whether the application renders off screen.
   * \return true if the application renders off screen.
   */
  SGAL::Boolean do_render_offscreen() const;

  /*! Determine whether the scene is interactive. A non interactive scene
   * is being rendered at most once.
   * \return false if the user has explicitly requested for a non interactive
   *         scene, or if she has requested taking a snapshot of scene or
   *         exporting the scene.
   */
  SGAL::Boolean is_interactive() const;

  /*! Obtain the camera projection matrix.
   */
  void get_proj_mat(SGAL::Matrix4f& mat);

  /*! Obtain the camera viewing matrix.
   */
  void get_view_mat(SGAL::Matrix4f& mat);

  /*! Obtain the total volume of all polyhedrons.
   */
  float volume();

  /*! Obtain the total surface area of all polyhedrons.
   */
  float surface_area();

  /*! Obtain the attributes of all polyhedrons.
   */
  void
  process_polyhedron_attributes_array(SGAL::Polyhedron_attributes_array& array)
    const;

protected:
  /*! The window manager. */
  Window_manager* m_window_manager;

  /*! The window item. */
  Window_item* m_window_item;

  /*! The width of the window. */
  SGAL::Uint m_win_width;

  /* The height of the window. */
  SGAL::Uint m_win_height;

  /*! The width of the offscreen framebuffer. */
  SGAL::Uint m_offscreen_width;

  /* The height of the offscreen framebuffer. */
  SGAL::Uint m_offscreen_height;

#if defined(SGAL_USE_V8)
  /*! An allocator. */
  SGAL::Array_buffer_allocator m_allocator;

  v8::Isolate::CreateParams m_params;

  /*! An isolated instance of the V8 engine. */
  v8::Isolate* m_isolate;
#endif

  /*! The scene graph. */
  SGAL::Scene_graph* m_scene_graph;

  /*! The configuration. */
  SGAL::Configuration* m_configuration;

  /*! The context. */
  SGAL::Context* m_context;

  /*! Option parser. */
  Player_option_parser* m_option_parser;

  SGAL::Boolean m_simulate;

  /*! Initialize. */
  void init();

  /*! Create a window.
   */
  void create_window();

  /*! Draw guides that separate the window into 4x5 rectangles. */
  void draw_grid();

  /*! Print statistic information. */
  void print_stat();

  /*! Updates directory search. */
  void update_data_dirs();

  /*! Indulge user requests from the command line. */
  void indulge_user();

  /*! Print geometry information of Index_face_set. */
  void print_geometry_info(SGAL::Indexed_face_set* ifs);

  /*! Print geometry information of Box. */
  void print_geometry_info(SGAL::Box* box);

  /*! Take a snapshot of the scene. */
  void snapshot_scene();

  /*! Export the scene. */
  void export_scene();

  /*! Create default nodes in the scene graph. */
  void create_defaults();

  /*! Create visual.
   */
  void create_visual();

  /*! Set preferred window attributes.
   */
  void set_preferred_window_attributes();

  /*! Set actual window attributes.
   */
  void set_actual_window_attributes();

  /*! Initialize offscreen rendering.
   */
  void init_offscreen_rendering();

  /*! Clear offscreen rendering.
   */
  void clear_offscreen_rendering();

private:
  typedef std::list<fi::path>                                   Path_list;
  typedef Path_list::iterator                                   Path_iter;

  struct Add_dir {
    Path_list& m_dirs;
    Add_dir(Path_list& dirs) : m_dirs(dirs) {}
    void operator()(const std::string& dir) { m_dirs.push_back(dir); }

    // The assignment operator cannot be generated (because some of the data
    // members are const pointers), so we suppress it explicitly.
    Add_dir& operator=(const Add_dir&);

    // In C++11, VC2013, the following is supported:
    // Add_dir& operator=(const Add_dir&) = delete;
  };

  enum Error_id { FILE_NOT_FOUND, ILLEGAL_EXTENSION, UNABLE_TO_LOAD };
  struct Illegal_input : public std::logic_error {
    Illegal_input(Error_id /* err */, const std::string& msg,
                  const std::string& filename) :
      std::logic_error(std::string(msg).append(" (").append(filename).append(")!"))
    {}

    Illegal_input(Error_id /* err */, const std::string& msg) :
      std::logic_error(std::string(msg).append("!"))
    {}
  };

  struct Input_file_missing_error : public std::logic_error {
    Input_file_missing_error(std::string& str) : std::logic_error(str) {}
  };

  /*! The input file full-name. */
  std::string m_fullname;

  /*! A collection of directories to search files in. */
  Path_list m_dirs;

  /*! Frame buffer object for off-screen drawing. */
  GLuint m_frame_buffer;

  /*! Render buffer object for off-screen drawing. */
  GLuint m_render_buffers[2];

  /*! The snapshot image. */
  Shared_image m_image;
};

//! \brief checks whether the scene is initialized.
inline bool Player_scene::is_scene_initiated()
{ return m_window_item != nullptr; }

//! \brief sets the option parser.
inline void Player_scene::set_option_parser(Player_option_parser* option_parser)
{ m_option_parser = option_parser; }


/*! \brief obtains the scene scene-graph. */
inline SGAL::Scene_graph* Player_scene::get_scene_graph() const
{ return m_scene_graph; }

#if defined(SGAL_USE_V8)
//! \brief obtains an isolated instance of the V8 engine.
inline v8::Isolate* Player_scene::get_isolate() { return m_isolate; }
#endif

//! \brief sets the player option parser.
inline void Player_scene::set(Player_option_parser* option_parser)
{ m_option_parser = option_parser; }

#endif
