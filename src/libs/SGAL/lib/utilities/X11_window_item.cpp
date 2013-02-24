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
// $Source: $
// $Revision: 12532 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * X11 window system component of the window manager
 */

#include <stdio.h>
#include <string.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/X11_window_item.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
X11_window_item::X11_window_item() :
  m_display(NULL),
  m_screen(0),
  m_window(0)
{
  memset(&m_desk_mode, 0, sizeof(m_desk_mode));
}

/*! \brief creates a window */
void X11_window_item::create(Display * display, int screen)
{
  int rc = 0;
  bool brc;
  
  m_display = display;              // retain the display for further actions
  m_screen = screen;                // retain the screen for further actions

  // Window winDummy;
  // unsigned int borderDummy;
    
  // Get an appropriate visual:
  /* attributes for a visual in RGBA format with at least 
   * 4 bits per color and a 24 bit depth buffer
   */
  int attributes[32];
  Uint index = 0;
  attributes[index++] = GLX_RGBA;
  if (m_double_buffer) attributes[index++] = GLX_DOUBLEBUFFER;
  attributes[index++] = GLX_RED_SIZE;
  attributes[index++] = SGAL_DEF_WINDOW_COLOR_BITS;
  attributes[index++] = GLX_GREEN_SIZE;
  attributes[index++] = SGAL_DEF_WINDOW_COLOR_BITS;
  attributes[index++] = GLX_BLUE_SIZE;
  attributes[index++] = SGAL_DEF_WINDOW_COLOR_BITS;
  // Do not allocate alpha for now.
  // attributes[index++] = GLX_ALPHA_SIZE;
  // attributes[index++] = SGAL_DEF_WINDOW_COLOR_BITS;
  attributes[index++] = GLX_DEPTH_SIZE;
  attributes[index++] = SGAL_DEF_WINDOW_DEPTH_BITS;
  if (m_stencil_bits > 0) {
    attributes[index++] = GLX_STENCIL_SIZE;
    attributes[index++] = SGAL_DEF_WINDOW_STENCIL_BITS;
  }
  if (m_number_of_samples > 0) {
    attributes[index++] = GLX_SAMPLE_BUFFERS;
    attributes[index++] = 1;
    attributes[index++] = GLX_SAMPLES;
    attributes[index++] = m_number_of_samples;
  }
  if (m_accum_red_bits > 0) {
    attributes[index++] = GLX_ACCUM_RED_SIZE;
    attributes[index++] = m_accum_red_bits;
  }
  if (m_accum_green_bits > 0) {
    attributes[index++] = GLX_ACCUM_GREEN_SIZE;
    attributes[index++] = m_accum_green_bits;
  }
  if (m_accum_blue_bits > 0) {
    attributes[index++] = GLX_ACCUM_BLUE_SIZE;
    attributes[index++] = m_accum_blue_bits;
  }
  if (m_accum_alpha_bits > 0) {
    attributes[index++] = GLX_ACCUM_ALPHA_SIZE;
    attributes[index++] = m_accum_alpha_bits;
  }
  attributes[index++] = None;

  XVisualInfo * vi = glXChooseVisual(display, screen, attributes);
  if (vi == NULL) {
    std::cerr << "Failed to choose visual!" << std::endl;
    return;
  }

  if (m_stencil_bits > 0) {
    rc = glXGetConfig(display, vi, GLX_STENCIL_SIZE,
                      reinterpret_cast<int*>(&m_stencil_bits));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  if (m_accum_red_bits > 0) {
    rc = glXGetConfig(display, vi, GLX_ACCUM_RED_SIZE,
                      reinterpret_cast<int*>(&m_accum_red_bits));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  if (m_accum_green_bits > 0) {
    rc = glXGetConfig(display, vi, GLX_ACCUM_GREEN_SIZE,
                      reinterpret_cast<int*>(&m_accum_green_bits));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  if (m_accum_blue_bits > 0) {
    rc = glXGetConfig(display, vi, GLX_ACCUM_BLUE_SIZE,
                      reinterpret_cast<int*>(&m_accum_blue_bits));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  if (m_accum_alpha_bits > 0) {
    rc = glXGetConfig(display, vi, GLX_ACCUM_ALPHA_SIZE,
                      reinterpret_cast<int*>(&m_accum_alpha_bits));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  if (m_number_of_samples > 0) {
    rc = glXGetConfig(display, vi, GLX_SAMPLES,
                      reinterpret_cast<int*>(&m_number_of_samples));
    if (rc != 0) {
      std::cerr << "Failed to configure visual!" << std::endl;
      return;
    }
  }
  
//   int glx_major, glx_minor;
//   brc glXQueryVersion(display, &glx_major, &glx_minor);
//   if (!brc)
//     std::cerr << "Failed to obtain GLX version!" << std::endl;
//   else
//     std::cout << "GLX-Version " << glx_major << "." << glx_minor << std::endl;

  // Create a GLX context:
  m_context = glXCreateContext(display, vi, 0, GL_TRUE);

  // Create a color map:
  Colormap cmap = XCreateColormap(display, RootWindow(display, vi->screen),
                                  vi->visual, AllocNone);
  m_win_attr.colormap = cmap;
  m_win_attr.border_pixel = 0;
 
  if (m_full_screen) {
    // Determine the version:
//     int vm_major, vm_minor;
//     brc = XF86VidModeQueryVersion(display, &vm_major, &vm_minor);
//     if (!brc)
//       std::cerr << "Failed to connect to the X server!" << std::endl;
//     else
//       std::cout << "XF86 VideoMode extension version "
//                 << vm_major << "." << vm_minor << std::endl;

    XF86VidModeModeInfo ** modes;
    int number_of_modes;
    brc = XF86VidModeGetAllModeLines(display, screen, &number_of_modes, &modes);
    // std::cout << "number_of_modes: " << number_of_modes << std::endl;

    int dpy_width = m_width;
    int dpy_height = m_height;
    if (brc) {
      int best_mode = 0;                  // set best mode to current

      // Save desktop-resolution before switching modes:
      m_desk_mode = *modes[0];

      // Look for mode with requested resolution:
      int i;
      for (i = 0; i < number_of_modes; ++i) {
        if ((modes[i]->hdisplay == m_width) && (modes[i]->vdisplay == m_height))
          best_mode = i;
      }
      // std::cout << "best_mode: " << best_mode << std::endl;

      // Switch to fullscreen:
      XF86VidModeSwitchToMode(display, screen, modes[best_mode]);
      XF86VidModeSetViewPort(display, screen, 0, 0);
      dpy_width = modes[best_mode]->hdisplay;
      dpy_height = modes[best_mode]->vdisplay;
      XFree(modes);
    }
    
    // Set window attributes:
    m_win_attr.override_redirect = True;
    m_win_attr.event_mask =
      ExposureMask |
      KeyPressMask | KeyReleaseMask |
      ButtonPressMask | ButtonReleaseMask |
      StructureNotifyMask |
      PointerMotionMask;
    m_window = XCreateWindow(display, RootWindow(display, vi->screen),
                             0, 0, dpy_width, dpy_height, 0, vi->depth,
                             InputOutput, vi->visual,
                             CWBorderPixel | CWColormap | CWEventMask |
                             CWOverrideRedirect, &m_win_attr);
    XWarpPointer(display, None, m_window, 0, 0, 0, 0, 0, 0);
    XMapRaised(display, m_window);
    XGrabKeyboard(display, m_window, True, GrabModeAsync, GrabModeAsync,
                  CurrentTime);
    XGrabPointer(display, m_window, True, ButtonPressMask,
                 GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
  } else {
    // Create a window in window mode:
    m_win_attr.event_mask = 
      ExposureMask |
      KeyPressMask | KeyReleaseMask |
      ButtonPressMask | ButtonReleaseMask |
      StructureNotifyMask |
      PointerMotionMask |
      FocusChangeMask;
    m_window = XCreateWindow(display, RootWindow(display, vi->screen),
                             0, 0, m_width, m_height, 1, vi->depth, InputOutput,
                             vi->visual,
                             CWBorderPixel | CWColormap | CWEventMask,
                             &m_win_attr);
    /* only set window title and handle wm_delete_events if in windowed mode */
    Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(display, m_window, &wm_delete, 1);
    XSetStandardProperties(display, m_window,
                           m_title.c_str(), m_title.c_str(),
                           None, NULL, 0, NULL);
    XMapRaised(display, m_window);
  }
  XSelectInput(display, m_window, m_win_attr.event_mask);
  m_width = 0;
  m_height = 0;

  make_current();           // make the context the current rendering context
}

/*! \brief destroys the window */
void X11_window_item::destroy()
{
  set_accumulating(SGAL_FALSE);
  set_redraw(SGAL_FALSE);
  if (m_window) {
    if (m_context) {
      if (m_full_screen) {
        if ((m_desk_mode.hdisplay != 0) && (m_desk_mode.vdisplay != 0))
          XF86VidModeSwitchToMode(m_display, m_screen, &m_desk_mode);
      }
      
      if (!glXMakeCurrent(m_display, None, NULL)) {
        printf("Could not release drawing context.\n");
      }
      glXDestroyContext(m_display, m_context);
      m_context = NULL;
    }
    XUnmapWindow(m_display, m_window);
    XDestroyWindow(m_display, m_window);
    m_window = 0;
  }
}

/*! \brief swaps the window frame-buffer */
void X11_window_item::swap_buffers()
{
  if (m_double_buffer) glXSwapBuffers(m_display, m_window);
}

/*! \brief shows the window. Make the window current if it is not already. */
void X11_window_item::show() {}

/*! \brief hides the window. Make the window current if it is not already. */
void X11_window_item::hide() {}

/*! \brief makes the context of the window item the current rendering context of
 * the calling thread
 */
void X11_window_item::make_current()
{
  glXMakeCurrent(m_display, m_window, m_context);
}

SGAL_END_NAMESPACE
