// Copyright (c) 2004  Israel.
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
// $Revision: 1808 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Context.hpp"
#include "SGAL/Window_obj.hpp"

#if defined(__GNUC__) && !defined(_WIN32)
#include "/usr/include/GL/glx.h"
#endif

SGAL_BEGIN_NAMESPACE

unsigned int Window_obj::s_init_x = static_cast<unsigned int>(-1);
unsigned int Window_obj::s_init_y = static_cast<unsigned int>(-1);
unsigned int Window_obj::s_init_width = DEF_WIN_WIDTH;
unsigned int Window_obj::s_init_height = DEF_WIN_HEIGHT;
Window_obj::Window_mode s_init_display_mode =
  static_cast<Window_obj::Window_mode>
  (Window_obj::RGB | Window_obj::DOUBLE |
   Window_obj::DEPTH | Window_obj::MULTISAMPLE);
unsigned int Window_obj::s_mouse_x = 0;
unsigned int Window_obj::s_mouse_y = 0;
unsigned int Window_obj::s_mouse_buttons = 0;
unsigned int Window_obj::s_screen_height = 0, Window_obj::s_screen_width = 0;

Window_obj::Render_callback Window_obj::s_render_func = NULL;

void * Window_obj::s_render_user_data = NULL;
bool Window_obj::s_free_run = false;

/*! Initialize the window
 * \param name the window name
 */
void Window_obj::init(const std::string & name)
{
  m_title = name;
  m_frame_func = NULL;
  m_event_func = NULL;
  
#if 0
  m_window_list.append(_item = new Window_item(this));
#endif
  m_closing = false;
  m_delete_on_close = true;

  m_is_RGB = (s_init_display_mode & INDEX) == 0;

  m_timer_enabled = 0;
  m_timer_interval = 0;

  if (s_screen_height == 0) {
    s_screen_width = _sgal_glut_get(SCREEN_WIDTH);
    s_screen_height = _sgal_glut_get(SCREEN_HEIGHT);
  }
  _sgal_gluts_init_display_mode(s_init_display_mode);
  _sgal_glut_init_window_position(s_init_x, s_init_y);
  _sgal_glut_init_window_size(s_init_width, m_height = s_init_height);

  m_id = _sgal_glut_create_window(m_title);
  _sgal_glut_set_user_data(m_id, this);

  if (name.empty()) _sgal_glut_set_borders(false);
  if (s_init_x == 0 && s_init_y == 0 && 
      s_init_width == s_screen_width && s_init_height == s_screen_height)
    _sgal_glut_full_screen();

  _sgal_glut_reshape_func(&Window_obj::reshape_func);
  _sgal_glut_close_func(&Window_obj::close_func);
  _sgal_glut_mouse_func(&Window_obj::mouse_func);
  _sgal_glut_motion_func(&Window_obj::motion_func);
  _sgal_glut_passive_motion_func(&Window_obj::passive_motion_func);
  _sgal_glut_display_func(&Window_obj::display_func);
  _sgal_glut_special_func(&Window_obj::special_func);
  _sgal_glut_special_up_func(&Window_obj::special_up_func);
  _sgal_glut_keyboard_func(&Window_obj::keyboard_func);
  _sgal_glut_keyboard_up_func(&Window_obj::keyboard_up_func);
    
#if defined(_WIN32)
  m_hwnd = _sgal_glut_get_HWND();
  m_hdc = _sgal_glut_get_HDC();
  m_context = new Context(m_hdc);
#endif

#if 0
#if defined(__GNUC__) && !defined(_WIN32)
  // clone the X connection so each window will have it's own
  // to avoid multi-thread X access
  ::Display * dpy = _sgal_glut_get_display();
  XSync(dpy, false);
  m_display = XOpenDisplay(DisplayString(dpy));
  m_window = _sgal_glut_get_X_window();
  m_visual = _sgal_glut_get_visual_info();
  m_context = new Context(m_display, m_visual);
#endif
#endif

  _sgal_glut_process_work_list();

  make_current(this);
}

/*! Constructor */
Window_obj::Window_obj(const std::string & name)
#if 0
  : 
  m_event_list(16),
  m_free_events(16)
#endif
{
  init(name);
}

/*! Destructor */
Window_obj::~Window_obj()
{
#if 0
  m_window_list.unlink(_item);
  delete m_item;
#endif

  m_title.clear();
  m_icon_title.clear();

  _sgal_glut_destroy_window(m_id);

#if !defined(_WIN32)
  // release our X connection clone
  XCloseDisplay(m_display);
#endif
}

//================================ Constructors ==============================

/*! Set the window title of the current window to title. */
void Window_obj::set_window_title(const std::string & title)
{
  if (title.empty())
    _sgal_glut_set_borders(false);
  else {
    _sgal_glut_set_borders(true);
    _sgal_glut_set_window_title(title);
  }

  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win) {
    cur_win->m_title.clear();
    cur_win->m_title = title;
  }
}

/*! Set the icon title of the current window to title. */
void Window_obj::set_icon_title(const std::string & title)
{
  _sgal_glut_set_icon_title(title);
  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win) {
    cur_win->m_icon_title.clear();
    cur_win->m_icon_title = title;
  }
}

/*! Controls whether or not the window is automatically deleted when it is
 * closed by the window manager. The default behavior is to delete the
 * window when it is closed. If enable is FALSE, the window will not be
 * deleted and the WINDOW_CLOSE event can be used to define the desired
 * behavior by using set_event_func(). If enable is TRUE the default behavior
 * will be restored.
 */
void Window_obj::delete_on_close(bool enable)
{
  m_delete_on_close = enable;
}

#if 0
Event * Window_obj::add_event()
{
  Event * event;

  // Pop event off free list if available
  if (m_free_events.getCount() > 0)
  {
    unsigned int n = m_free_events.get_count() - 1;
    event = m_free_events[n];
    m_free_events.set_count(n);
  }
  else        // Create new event
    event = new Event;

  event->target = this;
  m_event_list.append(event);
  return event;
}
#endif

/*! Clear the saved array of Event objects for this window. */
void Window_obj::reset_events()
{
#if 0
  // Copy events from event_list to free list
  unsigned int n = m_event_list.get_count();
  for (unsigned int i = 0; i < n; ++i)
    m_free_events.append(m_event_list[i]);

  m_event_list.set_count(0);
#endif
}

void Window_obj::idle_func()
{
#if 0
#ifdef SOLARIS_251
  DList_iter<Window_item, DLink> iter(m_window_list);
#else
  DList_iter<Window_item> iter(n_window_list);
#endif
#endif
#if 0
  Window_item * item;
#endif
  
#if 0
  //! \todo Handle time sensors
  Time_sensor::update_sensors();
#endif
  
#if 0
  while (item = iter.next()) {
    Window_obj * window = item->get_window();
    _sgal_glut_set_window(window->m_id);
    window->process_events();
  }
#endif
  
  if (s_render_func) (this->*s_render_func)(s_render_user_data);
}


void Window_obj::process_events()
{
#if 0
  //! \todo Handle time sensors
  if (!s_free_run) Time_sensor::update_sensors();
  
  if (m_frame_func) {
    set_free_run(m_frame_func(m_frame_user_data) == CONTINUE);
    reset_events();
  } else if (m_event_func && m_event_list.get_count()) {
    m_event_func(m_event_user_data);
    reset_events();
    if (s_render_func && !s_free_run) s_render_func(s_render_user_data);
  }

  if (m_closing) delete this;
#endif
}

/*! Reshape the window */
void Window_obj::reshape_func(unsigned int w, unsigned int h)
{
  Window_obj * cur_win = Window_obj::get_current();
  cur_win->m_height = _sgal_glut_get(WINDOW_HEIGHT);
    
  // do the reshape for the user if the context is current
  Context * ctx = cur_win->m_context;
  if (ctx == Context::get_current()) ctx->set_viewport(0, 0, w, h);

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::WINDOW_RESIZED;
  event->x = w;
  event->y = h;
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

bool Window_obj::close_func()
{
  Window_obj * cur_win = Window_obj::get_current();
  cur_win->m_closing = cur_win->m_delete_on_close;

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::WINDOW_CLOSE;
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
    
  return cur_win->m_delete_on_close;
}

void Window_obj::keyboard_func(unsigned char key, unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();
  y = cur_win->m_height-1 - y;

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::KEY_PRESS;
  event->key = key;
  event->x = x;
  event->y = y;
  event->button = s_mouse_buttons;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::keyboard_up_func(unsigned char key, unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();
  y = cur_win->m_height-1 - y;

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::KEY_RELEASE;
  event->key = key;
  event->x = x;
  event->y = y;
  event->button = s_mouse_buttons;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::display_func()
{
  Window_obj * cur_win = Window_obj::get_current();
  cur_win->m_height = _sgal_glut_get(WINDOW_HEIGHT);

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::WINDOW_REDRAW;
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::mouse_func(unsigned int button, Press press, unsigned int x,
                            unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();

#if 0
  static Mouse_button map_button[] = {
    Window_obj::LEFT_BUTTON,
    Window_obj::MIDDLE_BUTTON,
    Window_obj::RIGHT_BUTTON
  };
        
  Event * event = cur_win->add_event();
  s_mouse_x = x;
  s_mouse_y = cur_win->m_height-1 - y;
  if (press == UP) {
    s_mouse_buttons &= ~map_button[button];
    event->id = Event::MOUSE_UP;
  } else {
    s_mouse_buttons |= map_button[button];
    event->id = Event::MOUSE_DOWN;
  }
  event->x = s_mouse_x;
  event->y = s_mouse_y;
  event->button = map_button[button];
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::motion_func(unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();
#if 0
  s_mouse_x = x;
  s_mouse_y = cur_win->m_height-1 - y;

  Event * event = cur_win->add_event();
  event->id = Event::MOUSE_DRAG;        // ??? MOUSE_MOVE ???
  event->x = s_mouse_x;
  event->y = s_mouse_y;
  event->button = s_mouse_buttons;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::passive_motion_func(unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();
#if 0
  s_mouse_x = x;
  s_mouse_y = cur_win->m_height-1 - y;

  Event * event = cur_win->add_event();
  event->id = Event::MOUSE_MOVE;
  event->x = s_mouse_x;
  event->y = s_mouse_y;
  event->button = 0;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::special_func(int key, unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::KEY_ACTION_PRESS;
  event->key = key;
  event->x = x;
  event->y = cur_win->m_height-1 - y;
  event->button = s_mouse_buttons;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::special_up_func(int key, unsigned int x, unsigned int y)
{
  Window_obj * cur_win = Window_obj::get_current();

#if 0
  Event * event = cur_win->add_event();
  event->id = Event::KEY_ACTION_RELEASE;
  event->key = key;
  event->x = x;
  event->y = cur_win->m_height-1 - y;
  event->button = s_mouse_buttons;
  event->modifier = _sgal_glut_get_modifiers();
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) cur_win->process_events();
}

void Window_obj::swap_buffers()
{
  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win != NULL) cur_win->swap_this_window_buffers();
}

void Window_obj::swap_this_window_buffers()
{
#if defined(_WIN32)
  SwapBuffers(m_hdc);
#else
  glXSwapBuffers(m_display, m_window);
#endif
}

/*! Set the current window to win but does not make the window's Context
 * current. This is not a thread safe method; all threads share the same
 * notion of what the current window is. Also, note that the current window
 * will be automatically set when the event callbacks are invoked.
 */
 void Window_obj::set_current(Window_obj * win)
{
  if (win) _sgal_glut_set_window(win->m_id);
}

/*! Set the current window to win and makes the window's Context current
 * as well. Use set_current() to avoid making the Context current.
 */
void Window_obj::make_current(Window_obj *win)
{
  if (win) {
    _sgal_glut_set_window(win->m_id);
#if defined(_WIN32)
    win->m_context->make_current(win->m_hdc);
#endif
#if 0
#if defined(__GNUC__) && !defined(_WIN32)
    win->m_context->make_current(win->m_display, win->m_window);
#endif
#endif
  }
}

/*! Set the screen position of the current window to x and y. */
void Window_obj::position_window(unsigned int x, unsigned int y)
{
  _sgal_glut_position_window(x, y);
}

/*! Set the screen size of the current window to width and height. */
void Window_obj::reshape_window(unsigned int width, unsigned int height)
{
  _sgal_glut_reshape_window(width, height);
}

void Window_obj::pop_window(void)
{
  _sgal_glut_pop_window();
}

/*! Lower the current window to the bottom of the window stack. */
void Window_obj::push_window(void)
{
  _sgal_glut_push_window();
}

/*! Change the current window to iconic state. */
void Window_obj::iconify_window(void)
{
  _sgal_glut_iconify_window();
}

void Window_obj::show_window(void)
{
  _sgal_glut_show_window();
}

/*! Hide the current window from view. */
void Window_obj::hide_window(void)
{
  _sgal_glut_hide_window();
}

int Window_obj::get(Query what)
{
  return _sgal_glut_get((GLenum) what);
}

int Window_obj::get_device(Query_device what)
{
  return _sgal_glut_device_get((GLenum) what);
}

void Window_obj::main_loop()
{
  _sgal_glut_main_loop();
}

Window_obj * Window_obj::get_current()
{
  return (Window_obj*)_sgal_glut_get_user_data(_sgal_glut_get_window());
}

void Window_obj::set_frame_func(Frame_callback * frame_func, void * user_data)
{
  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win) {
    cur_win->m_frame_func = frame_func;
    cur_win->m_frame_user_data = user_data;
  }
}

void Window_obj::set_event_func(Event_callback * event_func, void * user_data)
{
  m_event_func = event_func;
  m_event_user_data = user_data;
}

void Window_obj::set_render_func(Render_callback render_func,
                                 void * user_data)
{
  s_render_func = render_func;
  s_render_user_data = user_data;
}

void Window_obj::set_free_run(bool enable)
{
  s_free_run = enable;
  _sgal_glut_idle_func(enable ? &Window_obj::idle_func : NULL);
}

/*! Return true if the window is in TGB mode, and false if in index mode */
bool Window_obj::is_RGB()
{
  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win == NULL) return 1;
  return cur_win->m_is_RGB;
}

void Window_obj::set_display(const char * name)
{
  _sgal_glut_set_display(name);
}

/*! Return true if mode is supported for window creation on this system,
 * false otherwise. Possible values of mode are a bitwise OR of the
 * Window_obj::Window_mode enumerated type. This method is intended to
 * allow chosing a valid mode without running into fatal errors from the
 * Window_obj() constructor.
 */
bool Window_obj::check_display_mode(Window_obj::Window_mode mode)
{
  return _sgal_glut_check_display_mode(mode);
}

/*! Set the mode to be used for subsequent window creations to mode.
 * Possible values of mode are a bitwise OR of the
 * Window_obj::WindowModeEnum enumerated type. If the mode is not
 * supported, the window contructor may abort the calling program. Use
 * check_display_mode() to avoid this behavior. If the double-buffer or
 * multi-sample modes can not be satisfied, they will be discarded and no
 * error is generated.
 */
void Window_obj::init_display_mode(Window_obj::Window_mode mode)
{
  s_init_display_mode = mode;
}

/*! Set the screen position to be used for subsequent window creations to x
 * and y. Initially, no window position is specified. setting x and y to
 * negative values goes back to the initial state
 */
void Window_obj::init_position(unsigned int x, unsigned int y)
{
  s_init_x = x;
  s_init_y = y;
}

/*! Set the screen size to be used for subsequent window creations to width
 * and height.
 */
void Window_obj::init_size(unsigned int width, unsigned int height)
{
  s_init_width = width;
  s_init_height = height;
}

/*! \brief returns the most recent position of the mouse in X. */
unsigned int Window_obj::get_mouse_x() { return s_mouse_x; }

/*! \brief returns the most recent position of the mouse in Y. */
unsigned int Window_obj::get_mouse_y() { return s_mouse_y; }

/*! \brief returns the most recent state of the mouse buttons. */
unsigned int Window_obj::get_mouse_buttons() { return s_mouse_buttons; }

#if 0
const Event_array & Window_obj::get_events() { return _event_list; }
#endif

void Window_obj::enable_timer(unsigned int msecs)
{
  _sgal_glut_timer_func(msecs, &Window_obj::timer_callback_func, this);
  m_timer_enabled = 1;
  m_timer_interval = msecs;
}

void Window_obj::disable_timer()
{
  m_timer_enabled = 0;
}

void Window_obj::timer_callback_func(void * value)
{
  Window_obj * win = (Window_obj *) value;

  if (!win->get_timer_enabled()) return;

  // register next callback
  _sgal_glut_timer_func(win->get_timer_interval(),
                        &Window_obj::timer_callback_func,
                        static_cast<Window_obj*>(value));

#if 0
  Event * event = win->add_event();
  event->id = Event::TIMER;
#endif
  
  // Notify app if in BLOCK mode
  if (!s_free_run) win->process_events();
}

bool Window_obj::get_timer_enabled()
{
  return m_timer_enabled;
}

unsigned int Window_obj::get_timer_interval()
{
  return m_timer_interval;
}

void Window_obj::update_sensors(Node * root, Camera * cam)
{
  Window_obj * cur_win = Window_obj::get_current();
  if (cur_win == NULL || cam == NULL || root == NULL)
    return;

#if 0
  unsigned int i;

  static int mouse_down = 0;
  int do_update;
  
  // See if mouse button is pressed or released.
  const Event_array & events = cur_win->_event_list;
  for (i = 0; i < events.get_count(); ++i) {
    do_update = 0;
    if (events[i]->id == Event::MOUSE_MOVE ||
        events[i]->id == Event::MOUSE_DRAG)
    {
      do_update = 1;
    }
    else if (events[i]->id == Event::MOUSE_DOWN) {
      // Make sure this mouse event corresponds to the pick x, y
      mouse_down = 1;
      do_update = 1;
    }
    else if (events[i]->id == Event::MOUSE_UP)
    {
      mouse_down = 0;
      do_update = 1;
    }
    if (do_update)
    {
      Pick_sensor::update_sensors(root, cam, events[i]->x, events[i]->y, 
                                  mouse_down);
    }
  }
#endif
}

Context * Window_obj::get_this_window_context() { return m_context; }

/*! Return the Context for the current window. */
Context * Window_obj::get_context()
{
  Window_obj * cur_win = Window_obj::get_current();
  return cur_win == NULL ? NULL : cur_win->m_context;
}

#if defined(_WIN32)
/* Return window handle for this window. */
HWND Window_obj::get_HWND() { return m_hwnd; }

/* Return device context handle for this window. */
HDC Window_obj::get_HDC() { return m_hdc; }
#endif

#if defined(__GNUC__) && !defined(_WIN32)
/*! Return the X Display for this window. */
::Display * Window_obj::get_X_display() { return m_display; }

/*! The X Window for this window. */
::Window Window_obj::get_X_window() { return m_window; }
#endif

SGAL_END_NAMESPACE

