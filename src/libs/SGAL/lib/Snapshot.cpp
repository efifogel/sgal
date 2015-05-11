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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/lexical_cast.hpp>

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
#include "SGAL/Snapshot.hpp"
#include "SGAL/File_format_2d.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Vrml_formatter.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Snapshot::s_tag = "Snapshot";
Container_proto* Snapshot::s_prototype(nullptr);

// Default values
const std::string Snapshot::s_def_dir_name(".");
const std::string Snapshot::s_def_file_name("snaphsot");
const File_format_2d::Id Snapshot::s_def_file_format(File_format_2d::ID_PPM);
const Uint Snapshot::s_def_quality(50);
const Boolean Snapshot::s_def_flip(true);

REGISTER_TO_FACTORY(Snapshot, "Snapshot");

//! \brief constructor
Snapshot::Snapshot(Boolean proto) :
  Node(proto),
  m_dir_name(s_def_dir_name),
  m_file_name(s_def_file_name),
  m_file_format(s_def_file_format),
  m_quality(s_def_quality),
  m_sequence(false),
  m_count(0),
  m_trigger(false),
  m_use_front_buffer(true),
  m_size(0),
  m_flip(s_def_flip)
{}

//! \brief destructor
Snapshot::~Snapshot()
{
  if (m_image) {
    Uchar* pixels = (Uchar *) m_image->get_pixels();
    if (pixels) delete [] pixels;
    m_image->set_pixels(nullptr);
    m_image->set_width(0);
    m_image->set_height(0);
    m_size = 0;
  }
}

//! \brief takes a snapshot and write to a file if triggered.
Action::Trav_directive Snapshot::draw(Draw_action* draw_action)
{
  if (!m_trigger) return Action::TRAV_CONT;
  m_trigger = false;

  m_use_front_buffer = draw_action->is_snap_from_front();
  if (!allocate_space(draw_action)) return Action::TRAV_CONT;
  take_snapshot();
  write_image();
  return Action::TRAV_CONT;
}

//! \brief allocates space for the image.
Boolean Snapshot::allocate_space(Draw_action* action)
{
  if (!m_image) m_image.reset(new Image);
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();

  if ((width == 0) || (height == 0)) {
    Context* context = action->get_context();
    if (!context) return false;
    Uint x0 = 0, y0 = 0;
    Uint tw, th;
    context->get_viewport(x0, y0, tw, th);
    if (width == 0) {
      m_image->set_width(tw);
      width = tw;
    }
    if (height == 0) {
      height = th;
      m_image->set_height(th);
    }
  }

  if (width == 0 || height == 0) return false;
  Uint size = Image::get_size(width, height, format);
  if (size > m_size) {
    Uchar* pixels = (Uchar *) m_image->get_pixels();
    if (pixels) delete [] pixels;
    pixels = new Uchar[size];
    if (!pixels) return false;
    m_image->set_pixels(pixels);
    m_size = size;
  }
  return true;
}

//! \brief takes a snapshot of the window.
void Snapshot::take_snapshot()
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  void* pixels = m_image->get_pixels();
  GLenum gl_format = m_image->get_format_format(format);
  GLenum gl_type = m_image->get_format_type(format);

  GLint val;
  glGetIntegerv(GL_READ_BUFFER, &val);
  GLenum read_buffer_mode = (GLenum)val;
  (m_use_front_buffer) ? glReadBuffer(GL_FRONT) : glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, gl_format, gl_type, pixels);
  glReadBuffer(read_buffer_mode);
}

//! \brief writes the image into a file.
void Snapshot::write_image()
{
  std::string file_name = m_dir_name + "/" + m_file_name;
  if (m_sequence) {
    std::ostringstream oss;
    oss << m_count++;
    file_name += "_" + oss.str();
  }
  file_name += std::string(".") + File_format_2d::get_name(m_file_format);

  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Uchar* pixels = static_cast<Uchar *>(m_image->get_pixels());
  Magick::Image image(width, height, "RGB", Magick::CharPixel, pixels);
  if (m_flip) image.flip();
  image.magick(File_format_2d::get_name(m_file_format));
  image.write(file_name);
}

//! \brief initializes the container prototype.
void Snapshot::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Node::get_prototype());

  // trigger
  Boolean_handle_function trigger_func =
    static_cast<Boolean_handle_function>(&Snapshot::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER,
                                          "trigger",
                                          Field_info::RULE_IN,
                                          trigger_func,
                                          false));

  // dirName
  String_handle_function dir_name_func =
    static_cast<String_handle_function>(&Snapshot::dir_name_handle);
  s_prototype->add_field_info(new SF_string(DIR_NAME,
                                            "dirName",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            dir_name_func,
                                            s_def_dir_name));

  // fileName
  String_handle_function file_name_func =
    static_cast<String_handle_function>(&Snapshot::file_name_handle);
  s_prototype->add_field_info(new SF_string(FILE_NAME,
                                            "fileName",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            file_name_func,
                                            s_def_file_name));

  // fileFormat
  Uint_handle_function file_format_func =
    reinterpret_cast<Uint_handle_function>(&Snapshot::file_format_handle);
  s_prototype->add_field_info(new SF_uint(FILE_FORMAT,
                                          "fileFormat",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          file_format_func,
                                          s_def_file_format));

  // Image
  Shared_container_handle_function image_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Snapshot::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE,
                                                      "image",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      image_func));
}

//! \brief deletes the container prototype.
void Snapshot::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Snapshot::get_prototype()
{
  if (!s_prototype) Snapshot::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Snapshot::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "dirName") {
      m_dir_name = strip_double_quotes(value);
      if (m_dir_name.empty()) m_dir_name = s_def_dir_name;
      Uint n = m_dir_name.size() - 1;
      if (m_dir_name[n] == '/') m_dir_name.resize(n);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileName") {
      m_file_name = strip_double_quotes(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileFormat") {
      auto str = strip_double_quotes(value);
      size_t i;
      for (i = 0; i < File_format_2d::NUM_IDS; ++i) {
        if (File_format_2d::compare_name(i, str)) {
          set_file_format(static_cast<File_format_2d::Id>(i));
          break;
        }
      }
      if (i == File_format_2d::NUM_IDS) {
        std::cerr << "Illegal file format (" << value.c_str() << ")!"
                  << std::endl;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      m_quality = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "sequence") {
      m_sequence = compare_to_true(value);
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
      m_image = boost::dynamic_pointer_cast<Image>(cont);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Snapshot::add_to_scene(Scene_graph* sg) { sg->add_snaphot(this); }

#if 0
//! \brief
Attribute_list Snapshot::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  if ((m_fileName != s_def_file_name)) {
    attrib.first = "fileName";
    attrib.second = get_file_name();
    attribs.push_back(attrib);
  }

  return attribs;
}

#endif

//! \breif writes a field of this container.
void Snapshot::write_field(const Field_info* field_info, Formatter* formatter)
{
  auto* vrml_formatter = static_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (FILE_FORMAT == field_info->get_id()) {
      vrml_formatter->single_string(field_info->get_name(),
                                    File_format_2d::get_name(m_file_format),
                                    File_format_2d::get_name(s_def_file_format));
      return;
    }
  }
  field_info->write(this, formatter);
}

SGAL_END_NAMESPACE
