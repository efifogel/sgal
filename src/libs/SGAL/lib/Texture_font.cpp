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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/gl.h>
#include <assert.h>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_font.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Material.hpp"

SGAL_BEGIN_NAMESPACE

static Texture_font* s_default_font = NULL;

static const Int FIRST_CHAR = 32;
static const Int LAST_CHAR = 128;

/*! Constructor */
Texture_font::Texture_font(const std::string& name, Boolean antialias,
                           Boolean left_to_right, Boolean top_to_bottom) :
  Font(name, antialias, left_to_right, top_to_bottom),
  m_texture(NULL),
  m_appearance(NULL)
{
  m_min_char = FIRST_CHAR;
  m_max_char = LAST_CHAR;

  m_scalex = 1.0f;
  m_scaley = 1.0f;
  m_spacing = 0;

  m_chars.resize(256);

#if 0
#if (defined _MSC_VER)  
  m_hfont = NULL;
  HWND hwnd = GetDesktopWindow();
  m_hdc = CreateCompatibleDC(GetDC(hwnd));
#endif
#endif
}

/*! Destructor */
Texture_font::~Texture_font()
{
  if (m_texture) delete m_texture;

#if 0
#if (defined _MSC_VER)    
  DeleteObject(m_hfont);
  DeleteDC((HDC)m_hdc);
#endif
#endif
}

/*! \brief initializes the font. */
void Texture_font::init()
{
  load_font(m_name, m_size);
  create_texture();
}

/*! \brief loads the texture with the font bitmap. */
void Texture_font::load_font(const std::string& /* name */, Uint /* size */)
{
#if 0
#if (defined _MSC_VER)
  // Get number of pixels per inch
  int pixPerInch = GetDeviceCaps((HDC)m_hdc, LOGPIXELSY);

  // height is in pixels, size in points (1 point = 1/72 inch)
  int height = -MulDiv(size, pixPerInch, 72);

  int weight = m_bold ? FW_BOLD : FW_NORMAL;
  DWORD italic = m_italic ? TRUE : FALSE;
  DWORD quality = m_antialias ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;

  if (m_hfont) DeleteObject(m_hfont);

  m_hfont = CreateFont(height, 0, 0, 0, weight,italic, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       ANTIALIASED_QUALITY /* quality */,
                       DEFAULT_PITCH | FF_DONTCARE, m_name);

  SelectObject((HDC)m_hdc, m_hfont);

  TEXTMETRIC tm;
  GetTextMetrics((HDC)m_hdc, &tm);
  m_max_char_height = tm.tmHeight;
  m_max_char_width  = tm.tmMaxCharWidth;

  // Calc chars width.
  for (int c = m_min_char; c<m_max_char + 1; ++c) {
    Char& ch = m_chars[c];
    int w;
    GetCharWidth((HDC)m_hdc, c, c, &w);
    ch.width = w;
    m_chars[c].width = w;
  }
#endif
#endif

  for (Uint c = m_min_char; c < m_max_char + 1; ++c) {
    Char& ch = m_chars[c];
    ch.m_width = 0;
    ch.m_x = 0;
    ch.m_y = 0;
  }
  m_max_char_height = 0;
  m_max_char_width  = 0;
}

/*! \breif creates a default texture font named "fixedsys".
 * \todo keep a static structure of fonts
 */
Texture_font* Texture_font::get_default()
{
  if (!s_default_font) s_default_font = new Texture_font("fixedsys");
  return s_default_font;
}

/*! \breif obtains the character given by its ascii code. */
const Texture_font::Char& Texture_font::get_char(int c)
{
  SGAL_assertion(c >= 0 && c < (int) m_chars.size());
  return m_chars[c];
}

/*! \breif creates the texture. */
void Texture_font::create_texture()
{
  calc_bitmap_size();
  calc_chars_dimensions();
  void* pixels = create_bitmap();
  draw_chars();
  create_image_texture(pixels);
  release_bitmap();

#if 0
#if (defined _MSC_VER)
  if (!m_left_to_right)
    SetTextAlign((HDC)m_hdc, TA_RTLREADING); // Set right to left reading order
#endif
#endif
}

/*! \brief calculates the dimensions of all characters. */
void Texture_font::calc_chars_dimensions()
{
  Uint x = 0, y = 0;
  for (Uint c = m_min_char; c <= m_max_char; ++c) {
    Char& ch = m_chars[c];
    float dw = m_max_char_width * 0.035f;
    Uint chw = (int) (ch.m_width + dw);
    //int chw = ch.width;

    if (x + chw >= m_bitmap_width) {
      x = 0;
      y += m_max_char_height;
    }

    ch.m_x = x;
    ch.m_y = y;
    x += chw;
  }
}

/*! \brief */
void Texture_font::draw_chars()
{
#if 0
#if (defined _MSC_VER)
   // Set text appearance properties
  SetBkMode((HDC)m_hdc,TRANSPARENT);
  SetBkColor((HDC)m_hdc, RGB(0,0,0));
  SetTextAlign((HDC)m_hdc,TA_LEFT | TA_TOP);
  SetTextColor((HDC)m_hdc,RGB(255,255,255));

  int x = 0;
  int y = 0;
  char str[2] = {0,0};
  for (int c = m_min_char; c < m_max_char + 1; ++c) {
    str[0] = c;
    RECT r;
    Char& ch = m_chars[c];
    r.top = ch.y, r.left = ch.x;
    r.right = m_bitmap_width;
    r.bottom = m_bitmap_height;

    // Drawing the text into the bitmap. 
    // TextOut() is not good for this purpose since it has problems with
    // antialiasing
    DrawText((HDC)m_hdc, str, 1, &r, DT_NOCLIP | DT_NOPREFIX | DT_SINGLELINE);
  }
#endif
#endif
}

/*! \brief obtains the bitmap dimension. */
void Texture_font::get_bitmap_size(Uint& x, Uint& y)
{
  x = m_bitmap_width;
  y = m_bitmap_height;
}

/*! \brief sets the appearance of this object.
 * \todo Handle text that always appears - the material must be transparent and
 * the depth test must be disabled.
 */
void Texture_font::set_appearance(Appearance* app) { 
  m_appearance = app; 
  if (m_appearance) {
    m_appearance->set_light_enable(SGAL_FALSE);
    m_appearance->set_depth_enable(SGAL_FALSE);
    m_appearance->set_poly_mode(Gfx::FILL_PMODE);
    m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
  }
}

/*! \brief calculates minimal possible dimensions (width & height)
 * for bitmap of cached characters (bitmap dimensions.
 * must be 2 to the power of 3 to 8).
 */
void  Texture_font::calc_bitmap_size()
{
  Uint dim[] = {8,16,32,64,128,256,512,1024,2048}; // possible bitmap dimensions
  Uint max_dim = 1024;

  // Check if renderer returned a valid value
  Uint  max_dim_idx;
  bool valid_max_dim = false;

  // Find index to max_dim in array of valid dimensions
  for (Uint j = 0; j < 9; ++j) {
    if (max_dim == dim[j]) {
      valid_max_dim = true;
      max_dim_idx = j;
      break;
    }
  }

  // Check whether renderer returned a valid dimension
  if (!valid_max_dim) {
    SGAL_assertion(0); // invalid texture dimensions.
    return;
  }

  // If font is too big recalc bitmap size (with a smaller font)
  if(m_max_char_height > max_dim || m_max_char_width > max_dim) {
    load_font(m_name, m_size-10);
    calc_bitmap_size();
    return;
  }

  Uint width = 0;
  float dw = m_max_char_width * 0.035f;

  // Find base value for width
  for (Uint c = m_min_char; c < m_max_char + 1; ++c)
    width += Uint(m_chars[c].m_width + dw);

  // Obtain optimal width and height
  Uint height = 0;

  // Find base value for height
  for (Uint i = 0; i <= max_dim_idx; ++i) {
    if (m_max_char_height <= dim[i]) {
      height = dim[i];
      break;
    }
  }

  // Calculate final width and height
  if (width > max_dim) {
    Uint min = (Uint) (height * ceil((float)(width - max_dim) / max_dim));
  
    while ((height *= 2) < min+1);

    width = max_dim;
  
    if (height > max_dim) {
      load_font(m_name, m_size - 10);
      calc_bitmap_size();
      return;
    }
  }
  else {
    Uint idx = 0;

    // Find base value for width
    for (Uint i = 0; i <= max_dim_idx; ++i) {
      if (width <= dim[i]) {
        idx = i;
        break;
      }
    }

    while ((width = dim[idx++]) < m_max_char_width);
  }

  m_bitmap_width = width;
  m_bitmap_height = height;

  m_width_reciprocal = 1.0f / m_bitmap_width;
  m_height_reciprocal = 1.0f / m_bitmap_height;

}

/*! \brief creates the texture.
 * First, create a bitmap using the calculated dimensions.
 */
void* Texture_font::create_bitmap()
{
#if 0
#if (defined _MSC_VER)
  // Initialize bitmap info structure
  BITMAPINFO bi;
  bi.bmiHeader.biSize   = sizeof(bi.bmiHeader);
  bi.bmiHeader.biWidth  =  m_bitmap_width;
  // bitmap is a top-down DIB (origin is upper-left corner):
  bi.bmiHeader.biHeight = -m_bitmap_height;
  bi.bmiHeader.biPlanes =  1;
  bi.bmiHeader.biBitCount    = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage   = 0;
  bi.bmiHeader.biXPelsPerMeter = m_bitmap_width;
  bi.bmiHeader.biYPelsPerMeter = m_bitmap_height;
  bi.bmiHeader.biClrUsed       = 0;
  bi.bmiHeader.biClrImportant  = 0;

  // Create GDI bitmap
  void* data;
  HBITMAP bmp = CreateDIBSection((HDC)m_hdc, &bi, DIB_RGB_COLORS, &data, 0, 0);
  SelectObject((HDC)m_hdc, bmp);
  SelectObject((HDC)m_hdc,(HFONT)m_hfont);

  // Initialize all bitmap pixels to black
  BitBlt((HDC)m_hdc, 0, 0, m_bitmap_width, m_bitmap_height, (HDC)m_hdc, 0, 0,
         BLACKNESS);
  return data;
#endif
#endif
  return NULL;
}

/*! \brief */
void Texture_font::release_bitmap()
{
#if 0
#if (defined _MSC_VER)
  // Destroy current bitmap.
  HBITMAP bmp = (HBITMAP) GetCurrentObject((HDC)m_hdc, OBJ_BITMAP);
  HBITMAP tmp = ::CreateBitmap(0, 0, 0, 0, 0);
  SelectObject((HDC)m_hdc, tmp);
  DeleteObject(bmp);
#endif
#endif
}

/*! \brief */
void Texture_font::create_image_texture(void* pixels)
{
  if (!m_appearance) return;

  /* Handle Alpha Values for Texture.
   * For each pixel in the surface buffer, copy the 8 red bits 
   * (pixel is ABGR) to the alpha bits, by a left shift of 24 bits.
   * Consequently all pixels in texture become transparent (Alpha=0)
   * except those that had a non zero value for red (red is chosen arbitrarily)
   * This allows for a transparent background for the text.
   */

  if (m_texture) delete m_texture;
  m_texture = new Image_texture;
  Image* image = new Image;
  image->set_width(m_bitmap_width);
  image->set_height(m_bitmap_height);
  image->set_format(Image_base::kRGB8_8_8);     // default format
  //! \todo Move to Image.cpp
  Uint size =
    Image::get_size(m_bitmap_width, m_bitmap_height, Image_base::kRGB8_8_8);
  void* new_pixels = new char[size];
  image->set_pixels(new_pixels);
  
#if 0
  // Instantiate a new surface for the texture object
  {
    Uint* src = (Uint*)data;
    
    int sz = srf->sizeY*srf->sizeX;
    for (int i = 0; i < sz; ++i, ++src, ++buf) 
      *buf = (((*src >> 4) & 0xF) << 12) | 0xFFF;
  }
#endif
  
  Uint* src = (Uint *) pixels;
  Uint* trg = (Uint *) new_pixels;
  for (Uint i = 0; i < m_bitmap_width * m_bitmap_height; ++i) {
    //trg[i] = 0xFF000000 + src[i];
    //pi[i] = (((*src >> 4) & 0xF) << 24) | 0xFFF;
    trg[i] = 0xFFFFFF | (src[i] << 24);
//    trg[i] = 0xFFFFFF | 0xFF000000;
  }

  m_texture->clean();
  m_appearance->set_texture(m_texture);
  m_appearance->set_tex_enable(true);
}

/*! \brief obtains the width and height of the string. */
void Texture_font::get_string_size(const std::string& /* str */,
                                   Uint& /* width */, Uint& /* height */)
{
#if 0
#if (defined _MSC_VER)
  SIZE size;
  if (!m_hfont) return;
  SelectObject((HDC)m_hdc, m_hfont);
  GetTextExtentPoint32((HDC)m_hdc, str.c_str(), str.size(), &size);
  width = size.cx;
  height = size.cy;
#endif
#endif
};

/*! \brief */
void Texture_font::set_scale(Float x, Float y)
{
  m_scalex = x;
  m_scaley = y;
}
  
/*! \brief */
void Texture_font::get_scale(Float& x, Float& y)
{
  x = m_scalex;
  y = m_scaley;
}

/*! \brief */
void Texture_font::draw_appearance(Draw_action* draw_action)
{
  m_appearance->draw(draw_action);
}

/*! \brief */
void Texture_font::draw_char(float x, float y, int ch) const
{
  x = ceilf(x);
  y = ceilf(y);

  const Char& c = m_chars[ch];

  float u1 = m_width_reciprocal  * (c.m_x);
  float v1 = m_height_reciprocal * (c.m_y);
  float u2 = m_width_reciprocal  * (c.m_x + c.m_width);
  float v2 = m_height_reciprocal * (c.m_y + m_max_char_height);

  // Texel offset.
  Vector3f color;
  Material* mat = m_appearance->get_material();
  if (mat) mat->get_diffuse_color(color);
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(u1,v1); glVertex2f(x, y);
  glTexCoord2f(u2,v1); glVertex2f(x + m_scalex * c.m_width, y);
  glTexCoord2f(u2,v2); glVertex2f(x + m_scalex * c.m_width,y -
                                  m_scaley * m_max_char_height);
  glTexCoord2f(u1,v2); glVertex2f(x, y - m_scaley * m_max_char_height);
  glEnd();
}

/*! \brief draws one string. */
void Texture_font::draw_string(Context* /* context */,
                               const std::string& text, Float /* size */)
{
  Float x = 0, y = 0;
#if 0
#if (defined _MSC_VER)
  if (!m_left_to_right) {
    char strBuf[4096];
    strBuf[len] = '\0';
    GCP_RESULTS gcp;
    gcp.lStructSize = sizeof(GCP_RESULTS);
    gcp.lpOutString = strBuf;
    gcp.lpDx = NULL;
    gcp.lpCaretPos = NULL;
    gcp.lpOrder = NULL;
    gcp.lpClass = NULL;
    gcp.lpGlyphs = NULL;
    gcp.nGlyphs = 256;

    GetCharacterPlacement((HDC)m_hdc, text.c_str(), len, 0, &gcp, GCP_REORDER);
    str = strBuf;
  }
#endif
#endif
  Uint len = text.size();
  const char* str = text.c_str();

  float px = x;
  float py = y;

  for (Uint i = 0; i < len; ++i) {
    Uint c = (unsigned char) str[i];
    if (c < 32) {
      if (c == '\n') {
        px = x;
        py += m_scaley * m_max_char_height;
      }
      continue;
    }
    Char& ch = m_chars[c];
    draw_char(px, py, c);
    px += (ch.m_width + m_spacing) * m_scalex;
  }
}

SGAL_END_NAMESPACE
