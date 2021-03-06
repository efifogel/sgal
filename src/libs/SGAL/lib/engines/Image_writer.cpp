// Copyright (c) 2015 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4244 )
#endif
#include <Magick++.h>

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

#if defined(_MSC_VER)
#define __WIN32__
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Image_writer.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Image_format.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Image_writer::s_tag("ImageWriter");
Container_proto* Image_writer::s_prototype(nullptr);

// Default values
const std::string Image_writer::s_def_dir_name(".");
const std::string Image_writer::s_def_file_name("snaphsot");
const Uint Image_writer::s_def_file_format(Image_format::PPM);
const Uint Image_writer::s_def_quality(50);
const Boolean Image_writer::s_def_flip(true);

REGISTER_TO_FACTORY(Image_writer, "Image_writer");

//! \brief constructs.
Image_writer::Image_writer(Boolean proto) :
  Node(proto),
  m_trigger(false),
  m_dir_name(s_def_dir_name),
  m_file_name(s_def_file_name),
  m_file_format(s_def_file_format),
  m_quality(s_def_quality),
  m_flip(s_def_flip)
{}

//! \brief destructs.
Image_writer::~Image_writer() {}

//! \brief initializes the container prototype.
void Image_writer::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>(&Image_writer::trigger_changed);

  // trigger
  auto trigger_func =
    static_cast<Boolean_handle_function>(&Image_writer::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_rule::RULE_IN,
                                          trigger_func, exec_func));

  // dirName
  auto dir_name_func =
    static_cast<String_handle_function>(&Image_writer::dir_name_handle);
  s_prototype->add_field_info(new SF_string(DIR_NAME,
                                            "dirName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            dir_name_func,
                                            s_def_dir_name));

  // fileName
  auto file_name_func =
    static_cast<String_handle_function>(&Image_writer::file_name_handle);
  s_prototype->add_field_info(new SF_string(FILE_NAME,
                                            "fileName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            file_name_func,
                                            s_def_file_name));

  // fileFormat
  auto file_format_func =
    reinterpret_cast<Uint_handle_function>(&Image_writer::file_format_handle);
  s_prototype->add_field_info(new SF_uint(FILE_FORMAT,
                                          "fileFormat",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          file_format_func,
                                          s_def_file_format));

  // flip
  auto flip_func =
    static_cast<Boolean_handle_function>(&Image_writer::flip_handle);
  s_prototype->add_field_info(new SF_bool(FLIP, "flip",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          flip_func,
                                          s_def_flip));

  // image
  auto image_func = reinterpret_cast<Shared_container_handle_function>
    (&Image_writer::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE,
                                                      "image",
                                                      Field_rule::RULE_OUT,
                                                      image_func));
}

//! \brief deletes the container prototype.
void Image_writer::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Image_writer::get_prototype()
{
  if (!s_prototype) Image_writer::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Image_writer::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "dirName") {
      m_dir_name = value;
      if (m_dir_name.empty()) m_dir_name = s_def_dir_name;
      Uint n = m_dir_name.size() - 1;
      if (m_dir_name[n] == '/') m_dir_name.resize(n);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileName") {
      m_file_name = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileFormat") {
      auto code = Image_format::get_instance()->find_code(value);
      if (code != Image_format::INVALID) set_file_format(code);
      else std::cerr << "Illegal file format (" << value.c_str() << ")!"
                     << std::endl;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      m_quality = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "image") {
      set_image(boost::dynamic_pointer_cast<Image>(cont));
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief executes the engine.
void Image_writer::execute()
{
  if (!m_image) return;
  m_trigger = false;
  auto* file_format = Image_format::get_instance();

  boost::filesystem::path file_path(m_file_name);
  if (file_path.is_relative() && !m_dir_name.empty())
    file_path = boost::filesystem::path(m_dir_name) / file_path;
  auto file_name = file_path.string();
  file_name += std::string(".") + file_format->find_name(m_file_format);

  auto width = m_image->get_width();
  auto height = m_image->get_height();
  Uchar* pixels = static_cast<Uchar*>(m_image->get_pixels());
  if (!pixels) return;
  auto format = m_image->get_format();
  std::string name =
    (format == Image::kRGB8_8_8) ? "RGB" :
    (format == Image::kRGBA8_8_8_8) ? "RGBA" :
    "unknown";
  Magick::Image image(width, height, name.c_str(), Magick::CharPixel, pixels);
  if (m_flip) image.flip();
  image.magick(file_format->find_name(m_file_format));
  image.write(file_name);
}

//! \breif writes a field of this container.
void Image_writer::write(Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (!vrml_formatter) return;
  Node::write(vrml_formatter);
}

//! \brief sets the image.
void Image_writer::set_image(Shared_image image) { m_image = image; }

/*! \brief triggers the execution of the engine as a response to change in one
 * of the input fields.
 */
void Image_writer::trigger_changed(const Field_info* /* field_info */)
{ execute(); }

SGAL_END_NAMESPACE
