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
// $Source: $
// $Revision: 7919 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WINDOW_ITEM_HPP
#define SGAL_WINDOW_ITEM_HPP

/*! \file
 * A pure window item. It represents a window managed by the window manager
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

#define SGAL_DEF_WINDOW_WIDTH                   720
#define SGAL_DEF_WINDOW_HEIGHT                  576
#define SGAL_DEF_WINDOW_X_POS                   10
#define SGAL_DEF_WINDOW_Y_POS                   10
#define SGAL_DEF_WINDOW_COLOR_BITS              4
#define SGAL_DEF_WINDOW_DEPTH_BITS              24
#define SGAL_DEF_WINDOW_STENCIL_BITS            1
#define SGAL_DEF_WINDOW_ACCUM_COLOR_BITS        16

class Window_item {
protected:
  /*! The window requires redrawing */
  Boolean m_redraw;

  /*! Is the window visible */
  Boolean m_visible;

  /*! Is the window color frame-buffer being accumulated? */
  Boolean m_accumulating;
    
  /*! The window title */
  std::string m_title;

public:
  /*! Constructor */
  Window_item() :
    m_redraw(SGAL_FALSE),
    m_visible(SGAL_FALSE),
    m_accumulating(SGAL_FALSE)
  {}

  /*! Destructor */
  virtual ~Window_item() {}

  /*! Do the window requires redrawing?
   * \return a Boolean indicates whether the window requires redrawing
   */
  Boolean do_redraw() const { return m_redraw; }
  
  /*! Set the redraw flag
   * \param redraw indicates whether the window requires redrawing
   */
  void set_redraw(Boolean redraw) { m_redraw = redraw; }
  
  /*! Is the window visible */
  virtual Boolean is_visible() const { return m_visible; }

  /*! Set the flag that indicates whether the window is visible */
  virtual void set_visible(Boolean flag) { m_visible = flag; }

  /*! Is the window color frame-buffer being accumulated */
  virtual Boolean is_accumulating() const { return m_accumulating; }

  /*! Set the flag that indicates whether the window color frame-buffer is
   * being accumulated */
  virtual void set_accumulating(Boolean flag) { m_accumulating = flag; }
  
  /*! Show the window. Make the window current if it is not already. */
  virtual void show() = 0;

  /*! Hide the window. Make the window current if it is not already. */
  virtual void hide() = 0;

  /*! Swap the window frame-buffer */
  virtual void swap_buffers() = 0;

  /*! Make the context of the window item the current rendering context of
   * the calling thread
   */
  virtual void make_current() = 0;
  
  /*! Set the window title */
  void set_title(const std::string & title) { m_title = title; }

  /*! Obtain the window title */
  const std::string & get_title() const { return m_title; }

  /*! Set the window to be full screen */
  virtual void set_full_screen(Boolean flag) = 0;

  /*! Set the width of the window
   * \param width the new width of the window
   */
  virtual void set_width(Uint width) = 0;

  /*! Set the height of the window
   * \param height the new height of the window
   */
  virtual void set_height(Uint height) = 0;
  
  /*! Set the width and height of the window
   * \param width the new width of the window
   * \param height the new height of the window
   */
  virtual void set_size(Uint width, Uint height) = 0;

  /*! Set the origin of the window
   * \param x the x-coordinate of the window origin
   * \param y the y-coordinate of the window origin
   */
  virtual void set_position(Uint x, Uint y) = 0;

  /*! Set the number of samples for multisampling
   * \param samples the number of samples 
   */
  virtual void set_number_of_samples(Uint samples) = 0;

  /*! Obtain the number of samples for multisampling
   * \return the number of samples
   */
  virtual Uint get_number_of_samples() const = 0;

  /*! Set the number of stencil bits in the accumulation buffer
   * \param stencil_bits the number of stencil bits 
   */
  virtual void set_number_of_stencil_bits(Uint stencil_bits) = 0;
  
  /*! Obtain the number of stencil bits in the accumulation buffer
   * \return the number of stencil bits
   */
  virtual Uint get_number_of_stencil_bits() const = 0;

  /*! Set the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void set_number_of_accumulation_bits(Uint red_bits,
                                               Uint green_bits,
                                               Uint blue_bits,
                                               Uint alpha_bits) = 0;

  /*! Obtain the number of RGBA bits stored in the accumulation buffer.
   * \param red_bits the number of red bits stored in the accumulation buffer
   * \param green_bits the number of red bits stored in the accumulation buffer
   * \param blue_bits the number of red bits stored in the accumulation buffer
   * \param alpha_bits the number of red bits stored in the accumulation buffer
   */
  virtual void get_number_of_accumulation_bits(Uint & red_bits,
                                               Uint & green_bits,
                                               Uint & blue_bits,
                                               Uint & alpha_bits) const = 0;
};

SGAL_END_NAMESPACE

#endif
