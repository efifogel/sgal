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
// $Revision: 12531 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * Windows window system component of the window manager
 */

#include "SGAL/basic.hpp"
#include "SGAL/Windows_window_item.hpp"
#include "SGAL/Gfx_conf.hpp"

#include "SGAL/Reshape_event.hpp"

SGAL_BEGIN_NAMESPACE

/*! Is it the first window? */
Boolean Windows_window_item::s_first(true);

/*! \brief shows the window. Make the window current if it is not already. */
void Windows_window_item::show() { ShowWindow(m_hWnd, SW_SHOW); }

/*! \brief hides the window. Make the window current if it is not already. */
void Windows_window_item::hide() { ShowWindow(m_hWnd, SW_HIDE); }

/*! \brief swaps the window frame-buffer */
void Windows_window_item::swap_buffers()
{ if (m_double_buffer) SwapBuffers(m_hDC); }

/*! \brief makes the context of the window item the current rendering context
 * of the calling thread
 */
void Windows_window_item::make_current()
{ wglMakeCurrent(m_hDC, m_hRC); }

/*! \brief queries the multisample frequencies */
Boolean Windows_window_item::init_multisample(HINSTANCE& hInstance)
{
  if (!Gfx_conf::get_instance()->is_multisample_supported()) return false;
  float fAttributes[] = {0,0};

  // These Attributes Are The Bits We Want To Test For In Our Sample
  // Everything Is Pretty Standard, The Only One We Want To 
  // Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
  // These Two Are Going To Do The Main Testing For Whether Or Not
  // We Support Multisampling On This Hardware.
  Uint color_bits = m_red_bits + m_green_bits + m_blue_bits;
  Uint accum_bits = m_accum_red_bits + m_accum_green_bits +
    m_accum_blue_bits + m_accum_alpha_bits;
  int iAttributes[] = {
    WGL_DRAW_TO_WINDOW_ARB,             GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB,             GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB,              GL_TRUE,
    WGL_ACCELERATION_ARB,               WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB,                 color_bits,
    WGL_ACCUM_BITS_ARB,                 accum_bits,
    WGL_ALPHA_BITS_ARB,                 m_alpha_bits,
    WGL_DEPTH_BITS_ARB,                 m_depth_bits,
    WGL_STENCIL_BITS_ARB,               m_stencil_bits,
    WGL_SAMPLE_BUFFERS_ARB,             GL_TRUE,
    WGL_SAMPLES_ARB,                    m_number_of_samples,
    0,0
  };

  // First We Check To See If We Can Get A Pixel Format For 4 Samples
  Uint num_formats;
  int valid = wglChoosePixelFormatARB(m_hDC, iAttributes, fAttributes, 1,
                                      &m_arb_multisample_format, &num_formats);
 
  // If We Returned True, And Our Format Count Is Greater Than 1
  if (valid && (num_formats >= 1)) return true;
 
  return false;
}

/*! \brief creates the window (or use an existing window handle). */
void Windows_window_item::create_base(HINSTANCE& hInstance, char* wc_name,
                                      HWND hWnd)
{
  if (hWnd == NULL) {
    m_bOwnWnd = true;

    // Create a window:
    DWORD dw_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;   // window style
    DWORD dw_ex_style = WS_EX_APPWINDOW;                  // window ext. style
    RECT rect = {0, 0, m_width, m_height};                // window rectangle
    if (m_full_screen) {
      dw_ex_style = WS_EX_TOPMOST;                        // cover everything
      dw_style |= WS_POPUP;
    } else {
      dw_ex_style |= WS_EX_WINDOWEDGE;
      dw_style |= WS_OVERLAPPEDWINDOW;
      AdjustWindowRectEx(&rect, dw_style, FALSE, dw_ex_style);
    }
    m_hWnd = CreateWindowEx(dw_ex_style,
                            wc_name,                      // window class name
                            m_title.c_str(),              // title (caption)
                            dw_style,                     // style
                            m_x, m_y,                     // origin
                            rect.right - rect.left,       // window width
                            rect.bottom - rect.top,       // window height
                            HWND_DESKTOP,                 // desktop is parent
                            0,                            // no menu
                            hInstance,                    // instance
                            this);                        // pass to WM_CREATE
  }
  else {
    m_bOwnWnd = false;
    m_hWnd = hWnd;
  }

  // Make sure we got a window:
  if (m_hWnd == NULL) {
    std::cerr << "CreateWindow() failed:  Cannot create a window!"
              << std::endl;
    return;
  }

  m_hDC = GetDC(m_hWnd);                        // grab the device context
  if (m_hDC == 0) {                             // did we get a device context?
    if (m_bOwnWnd) DestroyWindow(m_hWnd);       // Destroy The Window
    m_hWnd = 0;                                 // Zero The Window Handle
    return;                                     // Return failure
  }

  // Fill in the pixel format descriptor:
  PIXELFORMATDESCRIPTOR pfd;
  Uint color_bits = m_red_bits + m_green_bits + m_blue_bits;
  Uint accum_bits = m_accum_red_bits + m_accum_green_bits +
    m_accum_blue_bits + m_accum_alpha_bits;
  pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion        = 1;                   // version number (should be 1)
  pfd.dwFlags         = PFD_DRAW_TO_WINDOW | // draw to window (not bitmap)
                        PFD_SUPPORT_OPENGL | // draw using opengl
                        PFD_DOUBLEBUFFER;    // double buffered
  pfd.iPixelType      = PFD_TYPE_RGBA;       // PFD_TYPE_RGBA or COLORINDEX
  pfd.cColorBits      = color_bits;          // color bitplanes excluding alpha
  pfd.cRedBits        = m_red_bits;          // ignored
  pfd.cRedShift       = 0;                   // ignored
  pfd.cGreenBits      = m_green_bits;        // ignored
  pfd.cGreenShift     = 0;                   // ignored
  pfd.cBlueBits       = m_blue_bits;         // ignored
  pfd.cBlueShift      = 0;                   // ignored
  pfd.cAlphaBits      = m_alpha_bits;        // alpha bitplanes
  pfd.cAccumBits      = accum_bits;          // total accumulation bitplanes
  pfd.cAccumRedBits   = m_accum_red_bits;    // ignored
  pfd.cAccumGreenBits = m_accum_green_bits;  // ignored
  pfd.cAccumBlueBits  = m_accum_blue_bits;   // ignored
  pfd.cAccumAlphaBits = m_accum_alpha_bits;  // ignored
  pfd.cDepthBits      = m_depth_bits;        // depth bitplanes
  pfd.cStencilBits    = m_stencil_bits;      // stencil bitplanes
  pfd.cAuxBuffers     = 0;
  pfd.iLayerType      = PFD_MAIN_PLANE;      // main drawing layer

  /*! If multisample is not required, continue as usual.
   * if multisample is required, but it's the first window, also create
   * a window normally. In this case if multisample is supported, destroy
   * the window and create a new one with the appropriate pixel format
   */
  if (s_first || (m_number_of_samples == 0)) {
    // Find A Compatible Pixel Format
    m_pixel_format = ChoosePixelFormat(m_hDC, &pfd);
    if (m_pixel_format == 0) {                  // found a compatible format?
      std::cerr << "ChoosePixelFormat() failed:  Cannot find format specified!"
                << std::endl;
      ReleaseDC(m_hWnd, m_hDC);                 // release the device context
      m_hDC = 0;                                // zero the device context
	  if (m_bOwnWnd) DestroyWindow(m_hWnd);     // destroy the window
      m_hWnd = 0;                               // zero the window handle
      return;                                   // return failure
    }
  }
  else {
    m_pixel_format = m_arb_multisample_format;
  }
  // Set the pixel format:
  if (SetPixelFormat(m_hDC, m_pixel_format, &pfd) == FALSE) {
    std::cerr << "SetPixelFormat() failed:  Cannot set format specified!"
              << std::endl;
    ReleaseDC(m_hWnd, m_hDC);                   // release the device context
    m_hDC = 0;                                  // zero the device context
	if (m_bOwnWnd) DestroyWindow(m_hWnd);       // destroy the window
    m_hWnd = 0;                                 // zero the window handle
    return;                                     // return failure
  }

  // Create an OpenGL context:
  m_hRC = wglCreateContext(m_hDC);
  if (m_hRC == 0) {                         // did we get a rendering context?
    std::cerr << "wglCreateContext() failed: Cannot create context!"
              << std::endl;
    ReleaseDC(m_hWnd, m_hDC);                   // release the device context
    m_hDC = 0;                                  // zero the device context
	if (m_bOwnWnd) DestroyWindow(m_hWnd);       // destroy the window
    m_hWnd = 0;                                 // zero the window handle
    return;                                     // return failure
  }

  // Make the rendering context the current rendering context:
  if (wglMakeCurrent(m_hDC, m_hRC) == FALSE) {
    std::cerr << "wglMakeCurrent() failed: Cannot make current!" << std::endl;
    wglDeleteContext(m_hRC);                    // delete the rendering context
    m_hRC = 0;                                  // zero the rendering context
    ReleaseDC(m_hWnd, m_hDC);                   // release the device context
    m_hDC = 0;                                  // zero the device context
	if (m_bOwnWnd) DestroyWindow(m_hWnd);       // destroy the window
    m_hWnd = 0;                                 // zero the window handle
    return;                                     // return failure
  }
}

/*! \brief creates the window */
void Windows_window_item::create(HINSTANCE& hInstance, char* wc_name,
                                 HWND hWnd)
{
  create_base(hInstance, wc_name, hWnd);
  if (s_first) {
    s_first = false;
    if (m_number_of_samples > 0) {
      if (init_multisample(hInstance)) {
        destroy();
        create_base(hInstance, wc_name, hWnd);
      }
    }
  }

  // get the current pixel format index 
  int iPixelFormat = GetPixelFormat(m_hDC); 
    
  // obtain a detailed description of that pixel format 
  PIXELFORMATDESCRIPTOR  pfd;
  DescribePixelFormat(m_hDC, iPixelFormat,
                      sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  m_accum_red_bits = pfd.cAccumRedBits; 
  m_accum_green_bits = pfd.cAccumGreenBits; 
  m_accum_blue_bits = pfd.cAccumBlueBits; 
  m_accum_alpha_bits = pfd.cAccumAlphaBits;
  m_depth_bits = pfd.cDepthBits; 
  m_stencil_bits = pfd.cStencilBits; 

  if (Gfx_conf::get_instance()->is_multisample_supported()) {
    Int piAttributes[] = {WGL_SAMPLES_ARB};
    Int piValues[2];
    if (wglGetPixelFormatAttribivARB(m_hDC, m_pixel_format,
                                     0,             // main plane
                                     1, piAttributes, piValues) == FALSE)
    {
      std::cerr << "wglGetPixelFormatAttribivARB() failed: Cannot obtain pixel format!"
                << std::endl;
      return;
    }
    m_number_of_samples = piValues[0];
  }
  
  UpdateWindow(m_hWnd);                 // expose the window
                                        // ... send an initial WM_PAINT message
  /* The window is showed only when the application calls show() to let the
   * application create the context first
   */
}

/*! \brief destroys the window */
void Windows_window_item::destroy()
{
  if (m_hWnd != NULL) {
    if (m_hDC != 0) {
      wglMakeCurrent(m_hDC, NULL);      // make our context 'un-'current
      if (m_hRC != 0) {
        wglDeleteContext(m_hRC);        // delete the rendering context
        m_hRC = 0;
      }
      ReleaseDC(m_hWnd, m_hDC);         // release handle to DC
      m_hDC = 0;
    }
	if (m_bOwnWnd)
	  DestroyWindow(m_hWnd);              // destroy the window
    m_hWnd = NULL;
  }
}
  
SGAL_END_NAMESPACE
