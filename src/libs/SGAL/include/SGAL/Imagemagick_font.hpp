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
// $Id: $
// $Revision: 5647 $
//
// Author(s)     : Ophir Setter      <ophir.setter@gmail.com>
//                 Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGEMAGICK_FONT_HPP
#define SGAL_IMAGEMAGICK_FONT_HPP

/*! \file
 *
 */

#include <Magick++.h>

#include "SGAL/Font.hpp"
#include "SGAL/Texture_2d.hpp"

SGAL_BEGIN_NAMESPACE

class Imagemagick_font : public Font {
public:
  /*! Constructor */
  Imagemagick_font(const std::string& name, Boolean antialias = true,
                   Boolean left_to_right = true, Boolean top_to_bottom = true);

  /*! Destructor */
  virtual ~Imagemagick_font();

  /*! Draw one string
   * \param str the string
   * \param size the nominal height, in the local coordinate system
   */
  virtual void draw_string(Context* context, const std::string& str,
                           Float size);

  /*! Obtain the width and height of the string */
  virtual void get_string_size(const std::string& str,
                               Uint& width, Uint& height);

  /*! Initialize the font */
  virtual void init();

  /*! Clear the font */
  virtual void clear();

private:
  /*! The annotated image */
  Magick::Image m_magick_image;

  /*! */
  Magick::TypeMetric m_type_metrics;

  /*! */
  Uint m_point_size;

  /*! */
  Texture_2d m_texture;

  /*! */
  Image m_image;
  
  /*! */
  Uint m_text_width;

  /*! */
  Uint m_text_height;

  /*! The texture image width (number of columns) */
  Uint m_width;

  /*! The texture image height (number of rows) */
  Uint m_height;
  
  /*! Is the image dirty? */
  Boolean m_dirty;
};

SGAL_END_NAMESPACE

#endif
