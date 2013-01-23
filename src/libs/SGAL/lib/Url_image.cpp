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
// $Source: $
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>

#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#if (defined _MSC_VER)
#define __WIN32__
#endif

#include <Magick++.h>

#include "SGAL/errors.hpp"
#include "SGAL/Url_image.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Url_image::Url_image(Boolean proto) :
  Image(proto),
  m_dirty(true),
  m_current_image(NULL),
  m_flip(true)
{ m_current_image = &m_images[0]; }
  
/*! Destructor */
Url_image::~Url_image() { deallocate(); }

/*! \brief obtains the image width. */
Uint Url_image::get_width()
{
  if (m_dirty) clean();
  return m_current_image->get_width();
}

/*! \brief obtains the image height. */
Uint Url_image::get_height()
{
  if (m_dirty) clean();
  return m_current_image->get_height();
}

/*! \brief obtains the image format. */
Image::Format Url_image::get_format()
{
  if (m_dirty) clean();
  return m_current_image->get_format();
}

/*! \brief obtains the image pixel data. */
void* Url_image::get_pixels()
{
  if (m_dirty) clean();
  return m_current_image->get_pixels();
}

/*! \brief cleans the image. */
void Url_image::clean()
{
  std::string fullname;
  
#if BOOST_VERSION >= 103400
  fi::path file_path(m_url);
#else
  fi::path file_path(m_url, fi::native);
#endif
  if (file_path.is_complete()) {
#if BOOST_VERSION >= 103400
    if (fi::exists(file_path)) fullname = file_path.string();
#else
    if (fi::exists(file_path)) fullname = file_path.native_file_string();
#endif
  }
  else {
    for (Path_iter pi = m_dirs.begin(); pi != m_dirs.end(); ++pi) {
      fi::path full_file_path = *pi / file_path;
      if (!fi::exists(full_file_path)) continue;
      // As of Boost 1.34?
      fullname = full_file_path.string();
      // fullname = full_file_path.native_file_string();
      break;
    }
  }
  if (fullname.empty()) {
    throw File_not_found_error(m_url);
    return;
  }
  
  Magick::Image image;
  image.read(fullname);
  if (m_flip) image.flip();
  Uint width = image.columns();
  Uint height = image.rows();
  Magick::ColorspaceType color_space = image.colorSpace();
  Image::Format format = kIllegal;
  std::string magick_map;
  Magick::StorageType magick_type;
  switch (color_space) {
   case Magick::RGBColorspace:
    format = Image::kRGB8_8_8;
    magick_map = "RGB";
    magick_type = Magick::CharPixel;
    break;
   case Magick::GRAYColorspace:
    format = Image::kIntensity8;
    magick_map = "A";
    magick_type = Magick::CharPixel;
    break;
   case Magick::TransparentColorspace:
    format = Image::kRGBA8_8_8_8;
    magick_map = "RGBA";
    magick_type = Magick::CharPixel;
    break;
   default:
    std::cerr << "Unsupported color space (" << color_space
                      << ") in file " << fullname.c_str() << "!" << std::endl;
  }
  
  m_current_image->set_width(width);
  m_current_image->set_height(height);
  m_current_image->set_format(format);
  Uint size = Image::get_size(width, height, format);
  allocate(size);
  void* pixels = m_current_image->get_pixels();
  image.write(0, 0, width, height, magick_map, magick_type, pixels);
  
  m_dirty = false;
}

/*! \brief allocates memory that holds the image. */
void Url_image::allocate(Uint size)
{
  void* pixels = new char[size];
  m_current_image->set_pixels(pixels);
}

/*! \brief deallocates the memory that holds the image. */
void Url_image::deallocate()
{
  void* pixels = m_current_image->get_pixels();
  if (pixels) delete [] (char*) pixels;
}

/*! \brief swaps between the image buffer being decoded and the image buffer
 * being output. Retun true on success and false otherwise.
 */
Boolean Url_image::swap_image()
{
#if 0
  /*! \todo The following values are suppose to be provided by the
   * asyncroneous decoder. At this point we resort to a syncroneous one
   * and obtain the values right here
   */
  Uint width = atoi(m_decoder_image.get_value(Image_data::WIDTH).c_str());
  Uint height = atoi(m_decoder_image.get_value(Image_data::HEIGHT).c_str());
  Image::Format format = m_image_buffers[m_current_image_id]->get_format();
  
  // if we are running on Voodoo 3, we do not allow showing textures that are 
  // larger than 256x256
  if ((Gfx_conf::get_instance()->get_renderer() == Gfx_conf::reVoodoo3) &&
      (width > 256 || height > 256) && pixels != 0)
  {
    return false;
  }

  /*! Set the decoder pixel-buffer to be the curent pixel-buffer */
  m_decoder_image.set_pixels(m_image_buffers[m_current_image_id]->get_pixels());

  /*! Switch the current buffer */
  m_current_image_id = (m_current_image_id == 0 ? 1 : 0);
  m_current_image = &m_images[m_current_image_id];
#endif
  
  return true;
}

SGAL_END_NAMESPACE
