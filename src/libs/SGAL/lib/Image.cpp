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
#include "SGAL/Math_defs.hpp"
#include "SGAL/errors.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image::s_prototype(NULL);
const std::string Image::s_tag = "Image";

REGISTER_TO_FACTORY(Image, "Image");

/*! Constructor */
Image::Image(Boolean proto) : Image_base(proto) {}
  
/*! Destructor */
Image::~Image() {}

/*! \brief initializess the node prototype. */
void Image::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Image_base::get_prototype());

  // Add the field-info records to the prototype:
  //! \todo set and use the exec_func
  // Execution_function exec_func;

  s_prototype->add_field_info(new SF_string(URL, "url",
                                            get_member_offset(&m_url)));
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
  if (m_url.empty()) return;
  
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
  if (m_rotation != 0) image.rotate(rad2deg(m_rotation));
  image.matte(m_alpha);
  if (m_alpha) {
    image.opacity(MaxRGB * m_transparency);
    image.colorSpace(Magick::TransparentColorspace);
  }
  Image_base::Format format = kIllegal;
  std::string magick_map;
  Magick::StorageType magick_type;
  Magick::ImageType type = image.type();
  switch (type) {
   case Magick::GrayscaleType:       // Grayscale img
    format = Image_base::kIntensity8;
    magick_map = "A";
    magick_type = Magick::CharPixel;
    break;

   case Magick::TrueColorType:       // Truecolor img
    format = Image_base::kRGB8_8_8;
    magick_map = "RGB";
    magick_type = Magick::CharPixel;
    break;

   case Magick::TrueColorMatteType:  // Truecolor img + opacity
    format = Image_base::kRGBA8_8_8_8;
    magick_map = "RGBA";
    magick_type = Magick::CharPixel;
    break;

   case Magick::UndefinedType:       // Unset type
   case Magick::BilevelType:         // Monochrome img
   case Magick::GrayscaleMatteType:  // Grayscale img + opacity
   case Magick::PaletteType:         // Indexed color (palette) img
   case Magick::PaletteMatteType:    // Indexed color (palette) img + opacity
   case Magick::ColorSeparationType: // Cyan/Yellow/Magenta/Black (CYMK) img
   case Magick::ColorSeparationMatteType:
   case Magick::OptimizeType:
   default:
    std::cerr << "Unsupported image type (" << type
              << ") in file " << fullname.c_str() << "!" << std::endl;
    break;
  }
  Uint width = image.columns();
  Uint height = image.rows();
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
