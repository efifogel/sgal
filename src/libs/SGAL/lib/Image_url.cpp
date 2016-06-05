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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4146 )
#endif
#include <boost/lexical_cast.hpp>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4244 )
#endif
#include <Magick++.h>
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/errors.hpp"
#include "SGAL/Image_url.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Image_url::s_prototype(nullptr);
const std::string Image_url::s_tag("ImageUrl");

REGISTER_TO_FACTORY(Image_url, "Image_url");

// Default values
const Boolean Image_url::s_def_flip(true);
const Float Image_url::s_def_rotation(0);
const Boolean Image_url::s_def_alpha(false);
const Float Image_url::s_def_transparency(1);

//! \brief construct default
Image_url::Image_url(Boolean proto) :
  Image(proto),
  m_flip(s_def_flip),
  m_rotation(s_def_rotation),
  m_alpha(s_def_alpha),
  m_transparency(s_def_transparency)
{}

//! \brief destructor
Image_url::~Image_url() {}

//! \brief initializess the node prototype.
void Image_url::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Image::get_prototype());

  // url
  String_handle_function url_func =
    static_cast<String_handle_function>(&Image_url::url_handle);
  s_prototype->add_field_info(new SF_string(URL, "url",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            url_func));

  Boolean_handle_function flip_func =
    static_cast<Boolean_handle_function>(&Image_url::flip_handle);
  s_prototype->add_field_info(new SF_bool(FLIP, "flip",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          flip_func,
                                          s_def_flip));

  Float_handle_function rotation_func =
    static_cast<Float_handle_function>(&Image_url::rotation_handle);
  s_prototype->add_field_info(new SF_float(ROTATION, "rotation",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           rotation_func, s_def_rotation));

  Boolean_handle_function alpha_func =
    static_cast<Boolean_handle_function>(&Image_url::alpha_handle);
  s_prototype->add_field_info(new SF_bool(ALPHA, "alpha",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          alpha_func,
                                          s_def_alpha));

  Float_handle_function transparency_func =
    static_cast<Float_handle_function>(&Image_url::transparency_handle);
  s_prototype->add_field_info(new SF_float(TRANSPARENCY, "transparency",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           transparency_func,
                                           s_def_transparency));
}

//! \brief deletes the prototype.
void Image_url::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Image_url::get_prototype()
{
  if (!s_prototype) Image_url::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the image.
void Image_url::set_attributes(Element* elem)
{
  Image::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "url") {
      set_url(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      set_rotation(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "alpha") {
      set_alpha(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "transparency") {
      set_transparency(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief cleans the image.
void Image_url::clean()
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
  try {
    image.read(fullname.c_str());
    if (m_flip) image.flip();
    if (m_rotation != 0) image.rotate(rad2deg(m_rotation));
    if (m_alpha) {
      image.matte(true);
      Float quantum_range;
      // Workaround a bug in ImageMagick.
      {
        using namespace Magick;
        quantum_range = QuantumRange;
      }
      image.opacity(boost::lexical_cast<Uint>(quantum_range * m_transparency));
      image.colorSpace(Magick::TransparentColorspace);
    }
    Image::Format format = kIllegal;
    std::string magick_map;
    Magick::StorageType magick_type = Magick::CharPixel;
    Magick::ImageType type = image.type();
    switch (type) {
     case Magick::GrayscaleType:       // Grayscale img
      // std::cout << "GrayscaleType" << std::endl;
      format = Image::kLuminance8;
      magick_map = "R";
      magick_type = Magick::CharPixel;
      break;

     case Magick::TrueColorType:       // Truecolor img
      // std::cout << "TrueColorType" << std::endl;
      format = Image::kRGB8_8_8;
      magick_map = "RGB";
      magick_type = Magick::CharPixel;
      break;

     case Magick::TrueColorMatteType:  // Truecolor img + opacity
      // std::cout << "TrueColorMatteType" << std::endl;
      format = Image::kRGBA8_8_8_8;
      magick_map = "RGBA";
      magick_type = Magick::CharPixel;
      break;

     case Magick::PaletteMatteType:    // Indexed color (palette) img + opacity
      // std::cout << "PaletteMatteType" << std::endl;
      format = Image::kRGBA8_8_8_8;
      magick_map = "RGBA";
      magick_type = Magick::CharPixel;
      break;

     case Magick::PaletteType:         // Indexed color (palette) img
      // std::cout << "PaletteType" << std::endl;
      format = Image::kRGB8_8_8;
      magick_map = "RGB";
      magick_type = Magick::CharPixel;
      break;

     case Magick::BilevelType:         // Monochrome img
      std::cout << "BilevelType" << std::endl;
      goto err;

     case Magick::GrayscaleMatteType:  // Grayscale img + opacity
      std::cout << "GrayscaleMatteType" << std::endl;
      goto err;

     case Magick::ColorSeparationType: // Cyan/Yellow/Magenta/Black (CYMK) img
      std::cout << "ColorSeparationType" << std::endl;
      goto err;

     case Magick::ColorSeparationMatteType:
      std::cout << "ColorSeparationMatteType" << std::endl;
      goto err;

     case Magick::UndefinedType:       // Unset type
      std::cout << "UndefinedType" << std::endl;
      goto err;

     case Magick::OptimizeType:
     default:
     err:
      std::cerr << "Unsupported image type (" << type
                << ") in file " << fullname.c_str() << "!" << std::endl;
      break;
    }
    Uint width = image.columns();
    Uint height = image.rows();
    m_width = width;
    m_height = height;
    m_format = format;
    Uint size = Image::get_size(width, height, format);
    m_pixels = new char[size];
    SGAL_assertion(m_pixels);
    m_owned_pixels = true;
    image.write(0, 0, width, height, magick_map, magick_type, m_pixels);
  }
  catch (Magick::Exception &error_) {
    m_dirty = false;
    std::cerr << "Caught exception: " << error_.what() << std::endl;
    return;
  }
  m_dirty = false;
}

//! \brief adds the container to a given scene.
void Image_url::add_to_scene(Scene_graph* scene_graph)
{ set_dirs(scene_graph->get_data_dirs()); }

//! \brief sets the URL.
void Image_url::set_url(const std::string& url)
{
  m_url = url;
  m_dirty = true;
}

SGAL_END_NAMESPACE
