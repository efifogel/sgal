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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * The native window item. It represents a window managed by the native
 * window manager
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Native_window_item.hpp"

SGAL_BEGIN_NAMESPACE

// The flag that indicates whether windows be made full screen.
Boolean Native_window_item::s_init_full_screen(false);

//! The initial width of windows.
Uint Native_window_item::s_init_width(SGAL_DEF_WINDOW_WIDTH);

//! The initial height of windows.
Uint Native_window_item::s_init_height(SGAL_DEF_WINDOW_HEIGHT);

//! The initial x-cordinate of window origins.
Uint Native_window_item::s_init_x(SGAL_DEF_WINDOW_X_POS);

//! The initial y-cordinate of window origins.
Uint Native_window_item::s_init_y(SGAL_DEF_WINDOW_Y_POS);

//! \brief constructor.
Native_window_item::Native_window_item() :
  m_width(s_init_width), m_height(s_init_height),
  m_x(s_init_x), m_y(s_init_y),
  m_red_bits(SGAL_DEF_WINDOW_COLOR_BITS),
  m_green_bits(SGAL_DEF_WINDOW_COLOR_BITS),
  m_blue_bits(SGAL_DEF_WINDOW_COLOR_BITS),
  m_alpha_bits(SGAL_DEF_WINDOW_ALPHA_BITS),
  m_accum_red_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS),
  m_accum_green_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS),
  m_accum_blue_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS),
  m_accum_alpha_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS),
  m_depth_bits(SGAL_DEF_WINDOW_DEPTH_BITS),
  m_stencil_bits(SGAL_DEF_WINDOW_STENCIL_BITS),
  m_number_of_samples(0),
  m_double_buffer(true),
  m_full_screen(s_init_full_screen)
{}

//! \brief destructor.
Native_window_item::~Native_window_item() {}

//! \brief sets the window to be double buffered.
void Native_window_item::set_double_buffer(Boolean flag)
{ m_double_buffer = flag; }

//! \brief sets the window to be full screen.
void Native_window_item::set_full_screen(Boolean flag) { m_full_screen = flag; }

//! \brief sets the width of the window.
void Native_window_item::set_width(Uint width) { m_width = width; }

//! \brief Sets the height of the window.
void Native_window_item::set_height(Uint height) { m_height = height; }

//! \brief sets the width and height of the window.
void Native_window_item::set_size(Uint width, Uint height)
{
  m_width = width;
  m_height = height;
}

//! \brief sets the origin of the window.
void Native_window_item::set_position(Uint x, Uint y)
{
  m_x = x;
  m_y = y;
}

//! \brief sets the number of RGB bits.
void Native_window_item::set_number_of_color_bits(Uint red_bits,
                                                  Uint green_bits,
                                                  Uint blue_bits)
{
  m_red_bits = red_bits;
  m_green_bits = green_bits;
  m_blue_bits = blue_bits;
}

//! \brief obtains the number of RGB bits.
void Native_window_item::get_number_of_color_bits(Uint& red_bits,
                                                  Uint& green_bits,
                                                  Uint& blue_bits) const
{
  red_bits = m_red_bits;
  green_bits = m_green_bits;
  blue_bits = m_blue_bits;
}

//! \brief sets the number of alpha bits.
void Native_window_item::set_number_of_alpha_bits(Uint alpha_bits)
{ m_alpha_bits = alpha_bits; }

//! \brief obtains the number of alpha bits.
Uint Native_window_item::get_number_of_alpha_bits() const
{ return m_alpha_bits; }

//! \brief sets the number of RGBA bits stored in the accumulation buffer.
void Native_window_item::set_number_of_accumulation_bits(Uint red_bits,
                                                         Uint green_bits,
                                                         Uint blue_bits,
                                                         Uint alpha_bits)
{
  m_accum_red_bits = red_bits;
  m_accum_green_bits = green_bits;
  m_accum_blue_bits = blue_bits;
  m_accum_alpha_bits = alpha_bits;
}

//! \brief obtains the number of RGBA bits stored in the accumulation buffer.
void Native_window_item::get_number_of_accumulation_bits(Uint& red_bits,
                                                         Uint& green_bits,
                                                         Uint& blue_bits,
                                                         Uint& alpha_bits)
  const
{
  red_bits = m_accum_red_bits;
  green_bits = m_accum_green_bits;
  blue_bits = m_accum_blue_bits;
  alpha_bits = m_accum_alpha_bits;
}

//! \brief sets the number of depth bits.
void Native_window_item::set_number_of_depth_bits(Uint depth_bits)
{ m_depth_bits = depth_bits; }

//! \brief obtains the number of depth bits.
Uint Native_window_item::get_number_of_depth_bits() const
{ return m_depth_bits; }

//! \brief sets the number of stencil bits in the accumulation buffer.
void Native_window_item::set_number_of_stencil_bits(Uint bits)
{ m_stencil_bits = bits; }

//! \brief obtains the number of stencil bits in the accumulation buffer.
Uint Native_window_item::get_number_of_stencil_bits() const
{ return m_stencil_bits; }

//! \brief sets the number of samples for multisampling.
void Native_window_item::set_number_of_samples(Uint samples)
{ m_number_of_samples = samples; }

//! Obtain the number of samples for multisampling.
Uint Native_window_item::get_number_of_samples() const
{ return m_number_of_samples; }

//! \brief sets the flag that indicates whether windows be made full screen.
void Native_window_item::set_init_full_screen(Boolean flag)
{ s_init_full_screen = flag; }

//! \brief Set the initial width of windows.
void Native_window_item::set_init_width(Uint width) { s_init_width = width; }

//! \brief Set the initial height and height of windows.
void Native_window_item::set_init_height(Uint height)
{ s_init_height = height; }

//! \brief sets the initial width and height of windows.
void Native_window_item::set_init_size(Uint width, Uint height)
{
  s_init_width = width;
  s_init_height = height;
}

//! \brief sets the initial x-coordinate of window origins.
void Native_window_item::set_init_x(unsigned int x) { s_init_x = x; }

//! \brief obtains the initial x-coordinate of window origins.
unsigned int Native_window_item::get_init_x() { return s_init_x; }

//! \brief Set the initial y-coordinate of window origins.
void Native_window_item::set_init_y(unsigned int y) { s_init_y = y; }

//! \brief Obtain the initial y-coordinate of window origins.
unsigned int Native_window_item::get_init_y() { return s_init_y; }

//! \brief sets the initial origin of windows.
void Native_window_item::set_init_position(Uint x, Uint y)
{
  s_init_x = x;
  s_init_y = y;
}

SGAL_END_NAMESPACE
