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

class SGAL_SGAL_DECL Native_window_item : public Window_item {
protected:
  /*! The window width. */
  Uint m_width;

  /*! The window height. */
  Uint m_height;

  /*! The window origin X position. */
  Uint m_x;

  /*! The window origin X position. */
  Uint m_y;

  /*! Number of red bits. */
  Uint m_red_bits;

  /*! Number of green bits. */
  Uint m_green_bits;

  /*! Number of blue bits. */
  Uint m_blue_bits;

  /*! Number of alpha bits. */
  Uint m_alpha_bits;

  /*! Number of red bits stored in the accumulation buffer. */
  Uint m_accum_red_bits;

  /*! Number of green bits stored in the accumulation buffer. */
  Uint m_accum_green_bits;

  /*! Number of blue bits stored in the accumulation buffer. */
  Uint m_accum_blue_bits;

  /*! Number of alpha bits stored in the accumulation buffer. */
  Uint m_accum_alpha_bits;

  /*! The number of depth bits. */
  Uint m_depth_bits;

  /*! Number of stencil bits */
  Uint m_stencil_bits;

  /*! Number of (multi)samples. */
  Uint m_number_of_samples;

  /*! The window is double buffered. */
  Boolean m_double_buffer;

  /*! The window is full screen. */
  Boolean m_full_screen;

  /* The flag that indicates whether windows be made full screen. */
  static Boolean s_init_full_screen;

  /*! The initial width of windows. */
  static Uint s_init_width;

  /*! The initial height of windows. */
  static Uint s_init_height;

  /*! The initial x-cordinate of window origins. */
  static Uint s_init_x;

  /*! The initial y-cordinate of window origins. */
  static Uint s_init_y;

public:
  /*! Constructor. */
  Native_window_item();

  /*! Destructor. */
  virtual ~Native_window_item();

  /*! Determine whether the window full screen. */
  virtual Boolean is_full_screen() const;

  /*! Set the window to be full screen. */
  virtual void set_full_screen(Boolean flag);

  /*! Set the width of the window.
   * \param width the new width of the window.
   */
  virtual void set_width(Uint width);

  /*! Set the height of the window.
   * \param height the new height of the window.
   */
  virtual void set_height(Uint height);
  
  /*! Set the width and height of the window.
   * \param width the new width of the window.
   * \param height the new height of the window.
   */
  virtual void set_size(Uint width, Uint height);

  /*! Set the origin of the window.
   * \param x the x-coordinate of the window origin.
   * \param y the y-coordinate of the window origin.
   */
  virtual void set_position(Uint x, Uint y);

  /*! Set the number of RGB bits.
   * \param red_bits the number of red bits.
   * \param green_bits the number of green bitsr.
   * \param blue_bits the number of blue bits.
   */
  virtual void set_number_of_color_bits(Uint red_bits,
                                        Uint green_bits,
                                        Uint blue_bits);

  /*! Set the number of alpha bits.
   * \param alpha_bits the number of alpha bits. 
   */
  virtual void set_number_of_alpha_bits(Uint alpha_bits);

  /*! Obtain the number of alpha bits.
   * \return the number of alpha bits.
   */
  virtual Uint get_number_of_alpha_bits() const;

  /*! Obtain the number of RGB bits.
   * \param red_bits the number of red bits.
   * \param green_bits the number of green bits.
   * \param blue_bits the number of blue bits.
   */
  virtual void get_number_of_color_bits(Uint& red_bits,
                                        Uint& green_bits,
                                        Uint& blue_bits) const;

  /*! Set the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer.
   * \param green_bits the number of green bits stored in the accumulation
   *                   buffer.
   * \param blue_bits the number of blue bits stored in the accumulation buffer.
   * \param alpha_bits the number of alpha bits stored in the accumulation
   *                   buffer.
   */
  virtual void set_number_of_accumulation_bits(Uint red_bits,
                                               Uint green_bits,
                                               Uint blue_bits,
                                               Uint alpha_bits);

  /*! Obtain the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer.
   * \param green_bits the number of green bits stored in the accumulation
   *                   buffer.
   * \param blue_bits the number of blue bits stored in the accumulation buffer.
   * \param alpha_bits the number of alpha bits stored in the accumulation
   *                   buffer.
   */
  virtual void get_number_of_accumulation_bits(Uint& red_bits,
                                               Uint& green_bits,
                                               Uint& blue_bits,
                                               Uint& alpha_bits) const;

  /*! Set the number of depth bits.
   * \param depth the number of depth bits.
   */
  virtual void set_number_of_depth_bits(Uint depth);

  /*! Obtain the number of depth bits.
   * \return the number of depth bits.
   */
  virtual Uint get_number_of_depth_bits() const;

  /*! Set the number of stencil bits.
   * \param stencil_bits the number of stencil bits.
   */
  virtual void set_number_of_stencil_bits(Uint stencil_bits);
  
  /*! Obtain the number of stencil bits.
   * \return the number of stencil bits.
   */
  virtual Uint get_number_of_stencil_bits() const;

  /*! Set the number of samples for multisampling.
   * \param samples the number of samples.
   */
  virtual void set_number_of_samples(Uint samples);

  /*! Obtain the number of samples for multisampling.
   * \return the number of samples.
   */
  virtual Uint get_number_of_samples() const;

  /*! Set the flag that indicates whether windows be made full screen
   * \param flag indicates whether windows be made full screen
   */
  static void set_init_full_screen(Boolean flag);

  /*! Set the initial width of windows.
   * \param width the new width of the window.
   */
  static void set_init_width(Uint width);

  /*! Set the initial height and height of windows.
   * \param height the new height of the window.
   */
  static void set_init_height(Uint height);

  /*! Set the initial width and height of windows.
   * \param width the new width of the window.
   * \param height the new height of the window.
   */
  static void set_init_size(Uint width, Uint height);
  
  /*! Set the initial x-position of window otigins.
   * \param x the x-coordinate of the window origin.
   */
  static void set_init_x(Uint x);

  /*! Obtain the initial x-coordinate of window origins.
   * \return the origin x-coordinate.
   */
  static Uint get_init_x();

  /*! Set the initial y-position of window otigins.
   * \param y the y-coordinate of the window origin.
   */
  static void set_init_y(Uint y);

  /*! Obtain the initial y-coordinate of window origins.
   * \param y the origin y-coordinate.
   */
  static Uint get_init_y();

  /*! Set the initial origin of windows.
   * \param x the x-coordinate of the window origin.
   * \param y the y-coordinate of the window origin.
   */
  static void set_init_position(Uint x, Uint y);

#if defined(_WIN32)
  friend class Windows_window_manager;
#else
  friend class X11_window_manager;
#endif
};

/*! \brief determines whether the window full screen. */
inline Boolean Native_window_item::is_full_screen() const
{ return m_full_screen; }

SGAL_END_NAMESPACE

#endif
