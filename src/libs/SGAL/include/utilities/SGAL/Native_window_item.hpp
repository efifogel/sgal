// Copyright (c) 2007 Israel.
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
// $Id: Native_window_item.hpp 7919 2009-09-01 15:47:29Z efif $
// $Revision: 7919 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_NATIVE_WINDOW_ITEM_HPP
#define SGAL_NATIVE_WINDOW_ITEM_HPP

/*! \file
 * The native window item. It represents a window managed by the native
 * window manager
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Window_item.hpp"

SGAL_BEGIN_NAMESPACE

class Native_window_item : public Window_item {
protected:
  /*! The window width */
  unsigned int m_width;

  /*! The window height */
  unsigned int m_height;

  /*! The window origin X position */
  unsigned int m_x;

  /*! The window origin X position */
  unsigned int m_y;

  /*! Number of (multi)samples */
  unsigned int m_number_of_samples;

  /*! Number of stencil bits */
  unsigned int m_stencil_bits;

  /*! Number of red bits stored in the accumulation buffer */
  unsigned int m_accum_red_bits;

  /*! Number of red bits stored in the accumulation buffer */
  unsigned int m_accum_green_bits;

  /*! Number of red bits stored in the accumulation buffer */
  unsigned int m_accum_blue_bits;

  /*! Number of red bits stored in the accumulation buffer */
  unsigned int m_accum_alpha_bits;

  /*! The window is double buffered */
  Boolean m_double_buffer;

  /*! The window is full screen */
  Boolean m_full_screen;

  /* The flag that indicates whether windows be made full screen */
  static Boolean s_init_full_screen;

  /*! The initial width of windows */
  static unsigned int s_init_width;

  /*! The initial height of windows */
  static unsigned int s_init_height;

  /*! The initial x-cordinate of window origins */
  static unsigned int s_init_x;

  /*! The initial y-cordinate of window origins */
  static unsigned int s_init_y;

public:
  /*! Constructor */
  Native_window_item();

  /*! Destructor */
  virtual ~Native_window_item();

  /*! Is the window full screen? */
  virtual Boolean is_full_screen() const { return m_full_screen; }

  /*! Set the window to be full screen */
  virtual void set_full_screen(Boolean flag);

  /*! Set the width of the window
   * \param width the new width of the window
   */
  virtual void set_width(unsigned int width);

  /*! Set the height of the window
   * \param height the new height of the window
   */
  virtual void set_height(unsigned int height);
  
  /*! Set the width and height of the window
   * \param width the new width of the window
   * \param height the new height of the window
   */
  virtual void set_size(unsigned int width, unsigned int height);

  /*! Set the origin of the window
   * \param x the x-coordinate of the window origin
   * \param y the y-coordinate of the window origin
   */
  virtual void set_position(unsigned int x, unsigned int y);

  /*! Set the number of samples for multisampling
   * \param samples the number of samples 
   */
  virtual void set_number_of_samples(unsigned int samples);

  /*! Obtain the number of samples for multisampling
   * \return the number of samples
   */
  virtual unsigned int get_number_of_samples() const;

  /*! Set the number of stencil bits in the accumulation buffer
   * \param stencil_bits the number of stencil bits 
   */
  virtual void set_number_of_stencil_bits(unsigned int stencil_bits);
  
  /*! Obtain the number of stencil bits in the accumulation buffer
   * \return the number of stencil bits
   */
  virtual unsigned int get_number_of_stencil_bits() const;

  /*! Set the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void set_number_of_accumulation_bits(unsigned int red_bits,
                                               unsigned int green_bits,
                                               unsigned int blue_bits,
                                               unsigned int alpha_bits);

  /*! Obtain the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void get_number_of_accumulation_bits(unsigned int & red_bits,
                                               unsigned int & green_bits,
                                               unsigned int & blue_bits,
                                               unsigned int & alpha_bits) const;

  /*! Set the flag that indicates whether windows be made full screen
   * \param flag indicates whether windows be made full screen
   */
  static void set_init_full_screen(Boolean flag);

  /*! Set the initial width of windows
   * \param width the new width of the window
   */
  static void set_init_width(unsigned int width);

  /*! Set the initial height and height of windows
   * \param height the new height of the window
   */
  static void set_init_height(unsigned int height);

  /*! Set the initial width and height of windows
   * \param width the new width of the window
   * \param height the new height of the window
   */
  static void set_init_size(unsigned int width, unsigned int height);
  
  /*! Set the initial x-position of window otigins
   * \param x the x-coordinate of the window origin
   */
  static void set_init_x(unsigned int x);

  /*! Obtain the initial x-coordinate of window origins
   * \return the origin x-coordinate
   */
  static unsigned int get_init_x();

  /*! Set the initial y-position of window otigins
   * \param y the y-coordinate of the window origin
   */
  static void set_init_y(unsigned int y);

  /*! Obtain the initial y-coordinate of window origins
   * \param y the origin y-coordinate
   */
  static unsigned int get_init_y();

  /*! Set the initial origin of windows
   * \param x the x-coordinate of the window origin
   * \param y the y-coordinate of the window origin
   */
  static void set_init_position(unsigned int x, unsigned int y);

#if defined(_WIN32)
  friend class Windows_window_manager;
#else
  friend class X11_window_manager;
#endif
};

SGAL_END_NAMESPACE

#endif
