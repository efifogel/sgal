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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Draw true type font into the texture.
 */

#ifndef SGAL_TEXTURE_FONT_HPP
#define SGAL_TEXTURE_FONT_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Font.hpp"

SGAL_BEGIN_NAMESPACE

class Texture;
class Appearance;
class Draw_action;

class Texture_font : public Font {
public:
  struct Char {
    Uint m_width;                       // Character width
    Uint m_x;                           // x positon on bitmap
    Uint m_y;                           // y positon on bitmap
  };

  typedef boost::shared_ptr<Texture>            Shared_texture;

  /*! Constructor */
  Texture_font(const std::string& name, Boolean antialias = true,
               Boolean left_to_right = true,
               Boolean top_to_bottom = true);

  /*! Destructor */
  virtual ~Texture_font();

  static  Texture_font* get_default();  // Get default Texture_font.

  /*! Draw one string.
   * \param str the string
   * \param size the nominal height, in the local coordinate system
   */
  virtual void draw_string(Context* context, const std::string& str,
                           Float size);

  /*! Obtain the width and height of the string. */
  virtual void get_string_size(const std::string& str,
                               Uint& width, Uint& height);
  
  /*! Initialize the font. */
  virtual void init();

  void set_scale(Float x, Float y);

  void get_scale(Float& x, Float& y);

  // Draw appearance must be called before calling PutString/PutChar.
  void draw_appearance(Draw_action* draw_action);

  /*! Draw a single character */
  void draw_char(float x, float y, int ch) const;
   
  const Char& get_char(int ch);

  /*! Obtain the maximum height of all symbols in the font.
   * \return the maximum height of all symbols in the font
   */
  Uint get_max_height() const { return m_max_char_height; }

  /*! Obtain the bitmap dimension.
   * \param width the bitmap width in pixels
   * \param heigh the bitmap height in pixels
   */
  void get_bitmap_size(Uint& width, Uint& height);

  /*! Obtain the font-symbol texture.
   * \return the font-symbol texture
   */
  Shared_texture get_texture() const { return m_texture; };

  void set_appearance(Appearance* app);

  Appearance* get_appearance() const { return m_appearance; };

protected:
  /*! Load the texture with the font bitmap.
   * \param name the font name
   * \param size the font size
   */
  void load_font(const std::string& name, Uint size);

  void create_texture();

  void calc_chars_dimensions();

  void* create_bitmap();

  void release_bitmap();

  void draw_chars();

  void calc_bitmap_size();

  void create_image_texture(void* pixels);

#if 0
#if (defined _MSC_VER)
  HFONT* m_hfont;
  HDC* m_hdc;
#endif
#else
#endif

  /*! The font size. */
  Uint m_size;

  /*! The maximum width of all symbols in the font. */
  Uint m_max_char_width;

  /*! The maximum height of all symbols in the font. */
  Uint m_max_char_height;

  /*! The average width of all symbols in the font. */
  Uint m_ave_char_width;
  
  //! The width of the font-symbol bitmap.
  Uint m_bitmap_width;

  //! The height of the font-symbol bitmap.
  Uint m_bitmap_height;

  /*! A scaling factor along the x axis. */
  Float m_scalex;

  /*! A scaling factor along the y axis. */
  Float m_scaley;

  Float m_width_reciprocal;

  Float m_height_reciprocal;

  /*! Space between symbols in the rendered string. */
  Int m_spacing;

  /*! A list of symbol information. */
  std::vector<Char> m_chars;

  /*! The minimum symbol index in the symbol bitmap. */
  Uint m_min_char;

  /*! The maximum symbol index in the symbol bitmap. */
  Uint m_max_char;

  /*! The font-symbol texture. */
  Shared_texture m_texture;

  /*! The font-symbol appearance. */
  Appearance* m_appearance;
};

SGAL_END_NAMESPACE

#endif
