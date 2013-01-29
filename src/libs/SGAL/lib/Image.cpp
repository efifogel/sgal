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

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/errors.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

std::string Image::s_tag = "Image";
Container_proto* Image::s_prototype(NULL);

REGISTER_TO_FACTORY(Image, "Image");

/*! Constructor */
Image::Image(Boolean proto) :
  Image_base(proto),
  m_flip(true)
{}
  
/*! Destructor */
Image::~Image() {}

/*! \brief initializess the node prototype. */
void Image::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Image_base::get_prototype());
}

/*! \brief deletes the prototype. */
void Image::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Image::get_prototype() 
{  
  if (!s_prototype) Image::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the image. */
void Image::set_attributes(Element* elem)
{
  Image_base::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief obtains the image width. */
Uint Image::get_width()
{
  if (m_dirty) clean();
  return m_width;
}

/*! \brief obtains the image height. */
Uint Image::get_height()
{
  if (m_dirty) clean();
  return m_height;
}

/*! \brief obtains the image format. */
Image_base::Format Image::get_format()
{
  if (m_dirty) clean();
  return m_format;
}

/*! \brief obtains the image pixel data. */
void* Image::get_pixels()
{
  if (m_dirty) clean();
  return m_pixels;
}

/*! \brief cleans the image. */
void Image::clean()
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
  Image_base::Format format = kIllegal;
  std::string magick_map;
  Magick::StorageType magick_type;
  switch (color_space) {
   case Magick::RGBColorspace:
    format = Image_base::kRGB8_8_8;
    magick_map = "RGB";
    magick_type = Magick::CharPixel;
    break;
   case Magick::GRAYColorspace:
    format = Image_base::kIntensity8;
    magick_map = "A";
    magick_type = Magick::CharPixel;
    break;
   case Magick::TransparentColorspace:
    format = Image_base::kRGBA8_8_8_8;
    magick_map = "RGBA";
    magick_type = Magick::CharPixel;
    break;
   default:
    std::cerr << "Unsupported color space (" << color_space
              << ") in file " << fullname.c_str() << "!" << std::endl;
  }

  m_width = width;
  m_height = height;
  m_format = format;
  Uint size = Image_base::get_size(width, height, format);
  m_pixels = new char[size];
  SGAL_assertion(m_pixels);
  m_owned_pixels = true;
  image.write(0, 0, width, height, magick_map, magick_type, m_pixels);
  
  m_dirty = false;
}

/*! \brief sets the URL. */
void Image::set_url(const std::string& url)
{
  m_url = url;
  m_dirty = true;
}

SGAL_END_NAMESPACE
