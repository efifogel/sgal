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
// $Source$
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WINDOW_OBJ_HPP
#define SGAL_WINDOW_OBJ_HPP

/*! \file
 * Window is used to create and manage windows and events.
 *
 * A number of methods of this class are static and operate on the current
 * window. The current window is set by the Window() constructor or by
 * make_current(). It is also automatically set when processing any callback
 * functions. The notion of current window is not maintained in a
 * thread-safe manner.
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <string>
#if defined(__GNUC__) && !defined(_WIN32)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Node;
class Context;
class Camera;

class Window_obj {
public:
  enum Window_mode {
    RGB =                       0,
    RGBA =                      0,
    INDEX =                     1,
    SINGLE =                    0,
    DOUBLE =                    2,
    ACCUM =                     4,
    ALPHA =                     8,
    DEPTH =                     16,
    STENCIL =                   32,
    MULTISAMPLE =               128,
    STEREO =                    256
  };

  enum Query {
    WINDOW_X =                  100,
    WINDOW_Y =                  101,
    WINDOW_WIDTH =              102,
    WINDOW_HEIGHT =             103,
    WINDOW_BUFFER_SIZE =        104,
    WINDOW_STENCIL_SIZE =       105,
    WINDOW_DEPTH_SIZE =         106,
    WINDOW_RED_SIZE =           107,
    WINDOW_GREEN_SIZE =         108,
    WINDOW_BLUE_SIZE =          109,
    WINDOW_ALPHA_SIZE =         110,
    WINDOW_ACCUM_RED_SIZE =     111,
    WINDOW_ACCUM_GREEN_SIZE =   112,
    WINDOW_ACCUM_BLUE_SIZE =    113,
    WINDOW_ACCUM_ALPHA_SIZE =   114,
    WINDOW_DOUBLEBUFFER =       115,
    WINDOW_RGBA =               116,
    WINDOW_PARENT =             117,
    WINDOW_NUM_CHILDREN =       118,
    WINDOW_COLORMAP_SIZE =      119,
    WINDOW_STEREO =             121,
    SCREEN_WIDTH =              200,
    SCREEN_HEIGHT =             201,
    SCREEN_WIDTH_MM =           202,
    SCREEN_HEIGHT_MM =          203,
    MENU_NUM_ITEMS =            300,
    DISPLAY_MODE_POSSIBLE =     400,
    INIT_WINDOW_X =             500,
    INIT_WINDOW_Y =             501,
    INIT_WINDOW_WIDTH =         502,
    INIT_WINDOW_HEIGHT =        503,
    INIT_DISPLAY_MODE =         504,
    ELAPSED_TIME =              700
  };

  enum Query_device {
    HAS_KEYBOARD =              600,
    HAS_MOUSE =                 601,
    HAS_SPACEBALL =             602,
    HAS_DIAL_AND_BUTTON_BOX =   603,
    HAS_TABLET =                604,
    NUM_MOUSE_BUTTONS =         605,
    NUM_SPACEBALL_BUTTONS =     606,
    NUM_BUTTON_BOX_BUTTONS =    607,
    NUM_DIALS =                 608,
    NUM_TABLET_BUTTONS =        609
  };

  enum Press {
    DOWN,
    UP
  };

  enum Mouse_button {
    LEFT_BUTTON =               0x1,
    MIDDLE_BUTTON =             0x2,
    RIGHT_BUTTON =              0x4
  };

  enum Modifier {
    SHIFT_KEY_PRESSED =         0x1,
    CTRL_KEY_PRESSED =          0x2,
    ALT_KEY_PRESSED =           0x4
  };

  enum Special_key {
    /* function keys */
    KEY_F1  =                   1,
    KEY_F2  =                   2,
    KEY_F3  =                   3,
    KEY_F4  =                   4,
    KEY_F5  =                   5,
    KEY_F6  =                   6,
    KEY_F7  =                   7,
    KEY_F8  =                   8,
    KEY_F9  =                   9,
    KEY_F10  =                  10,
    KEY_F11  =                  11,
    KEY_F12  =                  12,
    /* directional keys */
    KEY_LEFT =                  100,
    KEY_UP  =                   101,
    KEY_RIGHT =                 102,
    KEY_DOWN =                  103,
    KEY_PAGE_UP =               104,
    KEY_PAGE_DOWN =             105,
    KEY_HOME =                  106,
    KEY_END  =                  107,
    KEY_INSERT =                108
  };

  enum Frame_mode {
    CONTINUE,
    BLOCK
  };
  
  /*! Construct a new window with the title indicated by name. If name is
   * NULL, then a borderless window will be created if possible.
   */
  Window_obj(const std::string & name);

  /*! Destroys the window and all associated system resources */
  virtual ~Window_obj();

  /*! Return true if mode is supported for window creation on this system,
   * false otherwise. Possible values of mode are a bitwise OR of the
   * Window_obj::Window_mode enumerated type. This method is intended to
   * allow chosing a valid mode without running into fatal errors from the
   * Window_obj() constructor.
   */
  bool check_display_mode(Window_mode mode);

  /*! Controls whether or not the window is automatically deleted when it is
   * closed by the window manager. The default behavior is to delete the
   * window when it is closed. If enable is FALSE, the window will not be
   * deleted and the WINDOW_CLOSE event can be used to define the desired
   * behavior by using set_event_func(). If enable is TRUE the default
   * behavior will be restored.
   */
   void delete_on_close(bool enable);

  /*! Disables the timer for this window */
  void disable_timer();

  /*! Enables a timer that generates the event Event::TIMER every "msecs"
   * milliseconds for this window.
   */
  void enable_timer(unsigned int msecs = 16);

  /*! Return various attributes of the current window as enumerated by
   * Query.
   */
  static int get(Query what);

  /*! Return the Context for the current window. */
  static Context * get_context();

  /*! Return the current window. */
  static Window_obj * get_current();

  /*! Return various attributes of the input devices as enumerated by
   * Query.
   */
  static int get_device(Query_device what);

#if 0
  /*! Return an array of Event objects for this window, one for each
   * event that has occured since the last call to resetEvents().
   */
  const Event_array & get_events();
#endif
  
  /*! Return the most recent state of the mouse buttons. */
  static unsigned int get_mouse_buttons();

  /*! Return the most recent position of the mouse in X. */
  static unsigned int get_mouse_x();

  /*! Return the most recent position of the mouse in Y. */
  static unsigned int get_mouse_y();

  /*! Return the Context for this window. */
  Context * get_this_window_context();

  /*! Return true if the timer is enabled, false otherwise. */
  bool get_timer_enabled();

  /*! Return the timer interval in milliseconds for this window. */
  unsigned int get_timer_interval();

#if defined(_WIN32)
  /* Return window handle for this window. */
  HWND get_HWND();

  /* Return device context handle for this window. */
  HDC get_HDC();
#endif

#if defined(__GNUC__) && !defined(_WIN32)
  /*! Return the X Display for this window. */
  ::Display * get_X_display();

  /*! Return the X Window for this window. */
  ::Window get_X_window();
#endif

  /*! Hide the current window from view. */
  static void hide_window();

  /*! Change the current window to iconic state. */
  static void iconify_window();

  /*! Set the mode to be used for subsequent window creations to mode.
   * Possible values of mode are a bitwise OR of the
   * Window_obj::WindowModeEnum enumerated type. If the mode is not
   * supported, the window contructor may abort the calling program. Use
   * check_display_mode() to avoid this behavior. If the double-buffer or
   * multi-sample modes can not be satisfied, they will be discarded and no
   * error is generated.
   */
  static void init_display_mode(Window_mode mode);

  /*! sets the screen position to be used for subsequent window creations */
  static void init_position(unsigned int x, unsigned int y);

  /*! Sets the screen size to be used for subsequent window creations */
  static void init_size(unsigned int width, unsigned int height);

  /*! Processe window system events for all Windows that have been created.
   * Will return when no more Windows exist.
   */
  static void main_loop();

  /*! Set the current window to win and makes the window's Context current
   * as well. Use set_current() to avoid making the Context current.
   */
  static void make_current(Window_obj * win);

  /*! Raises the current window to the top of the window stack. */
  void pop_window();

  /*! Set the screen position of the current window to x and y. */
  void position_window(unsigned int x, unsigned int y);

  /*! Lower the current window to the bottom of the window stack. */
  void push_window();

  /*! Clear the saved array of Event objects for this window. */
  void reset_events();

  /*! Set the screen size of the current window to width and height. */
  void reshape_window(unsigned int width, unsigned int height);

  /*! Set the current window to win but does not make the window's Context
   * current. This is not a thread safe method; all threads share the same
   * notion of what the current window is. Also, note that the current window
   * will be automatically set when the event callbacks are invoked.
   */
  static void set_current(Window_obj * win);

  /*! Set the display to be used for subsequent window creations to name.
   * This is currently not implemented for NT.
   */
  static void set_display(const char * name);

  typedef void (Window_obj::* Event_callback)(void);
  typedef void (Window_obj::* Frame_callback)(void);
  typedef void (Window_obj::* Render_callback)(void *);
  typedef void (Window_obj::* Timer_callback)(void *);
  
  /*! Set a callback, eventFunc, that is called from mainLoop() while
   * processing events for this window. The callback has the following
   * prototype:
   *
   * void eventFunc(void * user_data)
   * 
   * The user_data parameter supplied to this method will be passed to the
   * callback. This callback will be invoked either for every event that is
   * generated on this window if set_free_run() was last called with FALSE,
   * otherwise it is invoked each time the event loop becomes idle and there
   * are events stored on this window.
   */
  void set_event_func(Event_callback * event_func, void * user_data = NULL);
  
  /*! Set a callback, frameFunc, that is called from mainLoop() while
   * processing events for this window. The callback has the following
   * prototype:
   * 
   * int frame_func(void * user_data)
   * 
   * The user_data parameter supplied to this method will be passed to the
   * callback. The return value from the callback can be either
   * Window_obj::CONTINUE or Window_obj::BLOCK.
   * 
   * When BLOCK is returned, the event processing loop will block until
   * another event occurs on this window. When CONTINUE is returned the
   * callback will be called every time the event loop becomes idle. Thus, in
   * BLOCK mode the callback is called for every event but not at all when no
   * events are being generated, while in CONTINUE mode, the events are
   * batched up and delivered when things go idle and it will continue to be
   * called even if no events are being generated.
   * 
   * This method overrides the set_event_func() and set_render_func()
   * callbacks.
   */
  static void set_frame_func(Frame_callback * frame_func,
                             void * user_data = NULL);
  
  /*! Control the behavior of the render and event callbacks as described for
   * set_event_func() and set_render_func().
   * set_free_run().
   */
  static void set_free_run(bool enable);

  /*! Set the icon title of the current window to title. */
  void set_icon_title(const std::string & title);

  /*! Set a callback, renderFunc, that is called from mainLoop()
   * when a new frame should be rendered. The callback has the following
   * prototype:
   * 
   * void render_func(void * user_data)
   * 
   * The user_data parameter supplied to this method will be passed to the
   * callback. This callback will be invoked for every event that is
   * generated on any window if setFreeRun() was last called with FALSE,
   * otherwise it is invoked each time the event loop becomes idle.
   * set_frame_func().
   */
  static void set_render_func(Render_callback renderFunc,
                              void * user_data = NULL);
  
  /*! Set the window title of the current window to title. */
  void set_window_title(const std::string & title);

  /*! Change the current window from iconic or hidden state to visible state.
   */
  static void show_window(void);

  /*! Swap the window buffers(if double buffered) for the current window. */
  static void swap_buffers();

  /*! Swap the window buffers(if double buffered) for this window. It is
   * assumed that an appropriate Context is current.
   */
  void swap_this_window_buffers();

  /*! Update the pick sensors for the current window. */
  static void update_sensors(Node * root, Camera * cam);

private:
#define DEF_WIN_WIDTH   720
#define DEF_WIN_HEIGHT  576

  static unsigned int s_init_x;
  static unsigned int s_init_y;
  static unsigned int s_init_width;
  static unsigned int s_init_height;
  static Window_mode s_init_display_mode;
  static unsigned int s_mouse_x;
  static unsigned int s_mouse_y;
  static unsigned int s_mouse_buttons;
  static unsigned int s_screen_height;
  static unsigned int s_screen_width;

  static Render_callback s_render_func;
  static void * s_render_user_data;
  static bool s_free_run;
  
  /*! The current gl context */
  Context * m_context;

#if defined(_WIN32)
  /*! The window handle for this window */
  HWND m_hwnd;

  /*! The device context handle for this window. */
  HDC m_hdc;
#endif

#if defined(__GNUC__) && !defined(_WIN32)
  /*! The X Display for this window. */
  ::Display * m_display;
  
  /*! The X Window for this window. */
  ::Window m_window;

  /*! The X visual for this window */
  ::XVisualInfo * m_visual;
#endif

  /*! The window id */
  unsigned int m_id;

  /*! The window display mode. Bitwise or of the Window_mode enumerated */

  Window_mode m_display_mode;

  /*! The window title */ 
  std::string m_title;
  
  /*! The window icon title */ 
  std::string m_icon_title;

  /*! The window height */
  unsigned int m_height;

  /*! Is the window RGB or index mode */
  bool m_is_RGB;

  Frame_callback * m_frame_func;
  
  void * m_frame_user_data;

  Event_callback * m_event_func;
  
  void * m_event_user_data;

#if 0
  Event_array m_free_events;
  Event_array m_event_list;
#endif
  
  bool m_timer_enabled;
  unsigned int m_timer_interval;

#if 0
  class Window_item * m_item;
#endif
  
  bool m_closing;
  bool m_delete_on_close;

#if 0
  Event * add_event();
#endif
  
  /*! Initializ the window */
  void init(const std::string & name);

  /*! Process events */
  void process_events();

  /*! Return true if the window is in TGB mode, and false if it is in
   * index mode
   */
  bool is_RGB();

  void reshape_func(unsigned int w, unsigned int h);
  bool close_func();
  void keyboard_func(unsigned char key, unsigned int x, unsigned int y);
  void keyboard_up_func(unsigned char key, unsigned int x, unsigned int y);
  void display_func();
  void mouse_func(unsigned int button, Press press, unsigned int x,
                  unsigned int y);
  void motion_func(unsigned int x, unsigned int y);
  void passive_motion_func(unsigned int x, unsigned int y);
  void special_func(int key, unsigned int x, unsigned int y);
  void special_up_func(int key, unsigned int x, unsigned int y);
  void idle_func();
  void timer_callback_func(void * value);
  
#if defined(_WIN32)
  const HWND & _sgal_glut_get_HWND();
  const HDC & _sgal_glut_get_HDC();
#endif
  
#if defined(__GNUC__) && !defined(_WIN32)
  static ::Display * _sgal_glut_get_display();
  ::Window _sgal_glut_get_X_window();
  ::XVisualInfo * _sgal_glut_get_visual_info();
#endif

  typedef void (Window_obj::* Idle_func)(void);
  typedef void (Window_obj::* Reshape_func)(unsigned int, unsigned int);
  typedef bool (Window_obj::* Close_func)(void);
  typedef void (Window_obj::* Mouse_func)(unsigned int, Press, unsigned int,
                                          unsigned int);
  typedef void (Window_obj::* Motion_func)(unsigned int, unsigned int);
  typedef void (Window_obj::* Passive_motion_func)(unsigned int, unsigned int);
  typedef void (Window_obj::* Display_func)(void);
  typedef void (Window_obj::* Special_func)(int, unsigned int, unsigned int);
  typedef void (Window_obj::* Special_up_func)(int, unsigned int,
                                               unsigned int);
  typedef void (Window_obj::* Keyboard_func)(unsigned char, unsigned int,
                                             unsigned int);
  typedef void (Window_obj::* Keyboard_up_func)(unsigned char, unsigned int,
                                                unsigned int);
  
  void _sgal_glut_reshape_func(Reshape_func);
  void _sgal_glut_close_func(Close_func);
  void _sgal_glut_mouse_func(Mouse_func);
  void _sgal_glut_motion_func(Motion_func);
  void _sgal_glut_passive_motion_func(Passive_motion_func);
  void _sgal_glut_display_func(Display_func);
  void _sgal_glut_special_func(Special_func);
  void _sgal_glut_special_up_func(Special_up_func);
  void _sgal_glut_keyboard_func(Keyboard_func);
  void _sgal_glut_keyboard_up_func(Keyboard_up_func);

  void _sgal_gluts_init_display_mode(Window_mode mode);
  void _sgal_glut_init_window_position(unsigned int init_x, unsigned int init_y);
  void _sgal_glut_init_window_size(unsigned int width, unsigned int height);
  unsigned int _sgal_glut_create_window(const std::string & title);
  void _sgal_glut_set_user_data(unsigned int id, Window_obj * win);
  void _sgal_glut_set_borders(bool flag);
  void _sgal_glut_full_screen();
  int _sgal_glut_get_modifiers();

  void _sgal_glut_timer_func(unsigned int, Timer_callback callback,
                          Window_obj *);  
  void _sgal_glut_process_work_list();
  void _sgal_glut_destroy_window(unsigned int m_id);
  void _sgal_glut_set_window_title(const std::string & title);
  void _sgal_glut_set_icon_title(const std::string & title);
  bool _sgal_glut_check_display_mode(Window_mode mode);
  void _sgal_glut_push_window();
  void _sgal_glut_pop_window();
  void _sgal_glut_reshape_window(unsigned int, unsigned int);
  void _sgal_glut_position_window(unsigned int x, unsigned int y);

  static void * _sgal_glut_get_user_data(void *);
  static void * _sgal_glut_get_window();
  static int _sgal_glut_device_get(GLenum);
  static unsigned int _sgal_glut_get(unsigned int);
  static void _sgal_glut_hide_window();
  static void _sgal_glut_show_window();
  static void _sgal_glut_iconify_window();
  static void _sgal_glut_set_window(unsigned int m_id);
  static void _sgal_glut_main_loop();
  static void _sgal_glut_set_display(const char * name);
  static void _sgal_glut_idle_func(Idle_func);
  
  friend class Material;
};

SGAL_END_NAMESPACE

#endif
