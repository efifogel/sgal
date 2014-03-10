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
// Author(s)     : Ophir Setter      <ophir.setter@gmail.com>
//                 Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning( disable: 4244 )
#pragma warning( disable: 4251 )
#endif

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4244 )
#endif
#include <Magick++.h>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/Image.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Imagemagick_font.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Imagemagick_font::Imagemagick_font(const std::string & name, Boolean antialias,
                                   Boolean left_to_right,
                                   Boolean top_to_bottom) :
  Font(name, antialias, left_to_right, top_to_bottom),
  m_magick_image(Magick::Geometry(0, 0), Magick::ColorMono()),
  m_point_size(120),
  m_text_width(0),
  m_text_height(0),
  m_width(0),
  m_height(0),
  m_dirty(true)
{
  m_magick_image.fillColor(Magick::Color("white"));
  m_magick_image.strokeColor(Magick::Color("white"));
  // mimage.font();
  // mimage.density("72x72");
  m_magick_image.fontPointsize(m_point_size);
  m_magick_image.antiAlias(false);

  m_texture.set_image(&m_image);
  m_texture.set_min_filter(Texture::NEAREST_MIN);
  m_texture.set_mag_filter(Texture::NEAREST_MAG);
}

/*! Destructor */
Imagemagick_font::~Imagemagick_font()
{
}

/*! Draw one string */
void Imagemagick_font::draw_string(Context * context, const std::string & str,
                                   Float m_size)
{
  if (m_dirty) {
    m_magick_image.fontTypeMetrics(str, &m_type_metrics);
    Double text_width_d = m_type_metrics.textWidth();
    Double text_height_d = m_type_metrics.textHeight();
    // std::cout << "text width: " << text_width_d << std::endl;
    // std::cout << "text height: " << text_height_d << std::endl;
    m_text_width = static_cast<Uint>(text_width_d);
    m_text_height = static_cast<Uint>(text_height_d);

    // Create an image the width and height of which are powers of 2, as some
    // graphics systems cannot map otherwise textures:
    m_width = 2;
    m_height = 2;
    while (m_width < m_text_width) m_width *= 2;
    while (m_height < m_text_height) m_height *= 2;
    // mimage.extent(Magick::Geometry(width, height));
    m_magick_image.size(Magick::Geometry(m_width, m_height));

    // Annotate the text:
    m_magick_image.annotate(str, Magick::SouthWestGravity);

    // m_width = mimage.columns();
    // m_height = mimage.rows();
    // std::cout << "width: " << m_width << std::endl;
    // std::cout << "height: " << m_height << std::endl;

    // Create the texture:
    m_image.set_width(m_width);
    m_image.set_height(m_height);
    m_image.set_format(Image::kIntensity8);
    Uchar * pixels = new Uchar[m_image.get_size()];
    m_image.set_pixels(pixels);
    m_magick_image.write(0, 0, m_width, m_height, "R", Magick::CharPixel, pixels);

    //   Uint i;
    //   for (i = 0; i < m_image->get_size(); ++i) {
    //     Uchar alpha = pixels[i];
    //     pixels[i] = (alpha < 0x7f) ? 0 : 0xff;
    //   }
    m_dirty = false;
  }

  m_texture.draw(context);

  context->draw_tex_enable(true);
  context->draw_tex_env(Gfx::MODULATE_TENV);
  context->draw_alpha_func(Gfx::GREATER_AFUNC);
  context->draw_alpha_ref(0);

  // Map the texture on a rectangle:
  Float height_rect = m_size;
  Float width_rect = height_rect * m_text_width / m_text_height;
  // std::cout << "width_rect: " << width_rect << std::endl;
  // std::cout << "height_rect: " << height_rect << std::endl;
  Float x = 0;
  Float y = 0;
  Float z = 0;

  // Notice that the image is flipped. Invert the u texture cooridinate to
  // compensate:
  Float u = static_cast<Float>(m_text_width) / m_width;
  Float v = static_cast<Float>(m_text_height) / m_height;
  // std::cout << "u: " << u << std::endl;
  // std::cout << "v: " << v << std::endl;
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 1); glVertex3f(x, y, z);
  glTexCoord2f(u, 1); glVertex3f(x + width_rect, y, z);
  glTexCoord2f(u, 1-v); glVertex3f(x + width_rect, y + height_rect, z);
  glTexCoord2f(0, 1-v); glVertex3f(x, y + height_rect, z);
  glEnd();

  //! \todo restore old state correctly.
  context->draw_tex_enable(false);
  context->draw_alpha_func(Gfx::ALWAYS_AFUNC);
}

/*! Obtain the width and height of the string */
void Imagemagick_font::get_string_size(const std::string & str,
                                       Uint & width, Uint & height)
{
  m_magick_image.fontTypeMetrics(str, &m_type_metrics);

  width = static_cast<Uint>(m_type_metrics.textWidth());
  height = static_cast<Uint>(m_type_metrics.textHeight());
}

/*! Initialize the font */
void Imagemagick_font::init()
{

}

/*! Clear the font */
void Imagemagick_font::clear()
{
}

SGAL_END_NAMESPACE
