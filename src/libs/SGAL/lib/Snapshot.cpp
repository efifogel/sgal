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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

/*! This file and its content, namely the class Snapshot, are obsolete.
 * They are retained for backward compatibility.
 * Use Snapshotter instead.
 * Snapshot takes a snapshot of the 3D scene and save it in an image file.
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Snapshot.hpp"
#include "SGAL/File_format_2d.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Image_writer.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Snapshot::s_tag("Snapshot");
Container_proto* Snapshot::s_prototype(nullptr);

// Default values
const Boolean Snapshot::s_def_sequence(false);

REGISTER_TO_FACTORY(Snapshot, "Snapshot");

//! \brief constructor
Snapshot::Snapshot(Boolean proto) :
  Snapshotter(proto),
  m_dir_name(Image_writer::s_def_dir_name),
  m_file_name(Image_writer::s_def_file_name),
  m_file_format(Image_writer::s_def_file_format),
  m_quality(Image_writer::s_def_quality),
  m_sequence(s_def_sequence),
  m_count(0),
  m_dirty_image_writer(true),
  m_dirty_file_name(false),
  m_dirty_dir_name(false),
  m_dirty_file_format(false)
{}

//! \brief destructor
Snapshot::~Snapshot() {}

//! \brief takes a snapshot and write to a file if triggered.
Action::Trav_directive Snapshot::draw(Draw_action* draw_action)
{
  if (!m_trigger) return Action::TRAV_CONT;
  m_trigger = false;

  if (m_dirty_image_writer) clean_image_writer();
  Snapshotter::set_size(draw_action);
  // If the writter happens to have an image, override it!
  m_image_writer->set_image(get_image());
  Snapshotter::execute();
  if (m_sequence) {
    std::ostringstream oss;
    oss << m_count++;
    auto file_name = m_file_name + "_" + oss.str();
    m_image_writer->set_file_name(file_name);
  }
  m_image_writer->execute();    // save the image
  return Action::TRAV_CONT;
}

//! \brief initializes the container prototype.
void Snapshot::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Snapshotter::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Snapshot::field_changed);

  // dirName
  String_handle_function dir_name_func =
    static_cast<String_handle_function>(&Snapshot::dir_name_handle);
  s_prototype->add_field_info(new SF_string(DIR_NAME,
                                            "dirName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            dir_name_func,
                                            Image_writer::s_def_dir_name,
                                            exec_func));

  // fileName
  String_handle_function file_name_func =
    static_cast<String_handle_function>(&Snapshot::file_name_handle);
  s_prototype->add_field_info(new SF_string(FILE_NAME,
                                            "fileName",
                                            Field_rule::RULE_EXPOSED_FIELD,
                                            file_name_func,
                                            Image_writer::s_def_file_name,
                                            exec_func));

  // fileFormat
  Uint_handle_function file_format_func =
    reinterpret_cast<Uint_handle_function>(&Snapshot::file_format_handle);
  s_prototype->add_field_info(new SF_uint(FILE_FORMAT,
                                          "fileFormat",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          file_format_func,
                                          Image_writer::s_def_file_format,
                                          exec_func));

  // Image
  Shared_container_handle_function image_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Snapshot::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE_WRITER,
                                                      "imageWriter",
                                                      Field_rule::RULE_EXPOSED_FIELD,
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
  Snapshotter::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "dirName") {
      m_dir_name = value;
      if (m_dir_name.empty()) m_dir_name = Image_writer::s_def_dir_name;
      Uint n = m_dir_name.size() - 1;
      if (m_dir_name[n] == '/') m_dir_name.resize(n);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileName") {
      set_file_name(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileFormat") {
      size_t i;
      for (i = 0; i < File_format_2d::NUM_CODES; ++i) {
        if (File_format_2d::compare_name(i, value)) {
          set_file_format(static_cast<File_format_2d::Code>(i));
          break;
        }
      }
      if (i == File_format_2d::NUM_CODES) {
        std::cerr << "Illegal file format (" << value.c_str() << ")!"
                  << std::endl;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      set_quality(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "sequence") {
      set_sequence(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "imageWriter") {
      set_image_writer(boost::dynamic_pointer_cast<Image_writer>(cont));
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief sets the name of the file where the snapshot is written to.
void Snapshot::set_file_name(const std::string& file_name)
{
  m_file_name = file_name;
  m_dirty_image_writer = true;
  m_dirty_file_name = false;
}

//! \brief obtains the name of the file where the snapshot is written to.
const std::string& Snapshot::get_file_name()
{
  if (m_dirty_file_name) clean_attributes();
  return m_file_name;
}

//! \brief sets the name of the dir where the snapshot is written to.
void Snapshot::set_dir_name(std::string dir_name)
{
  m_dir_name = dir_name;
  m_dirty_image_writer = true;
  m_dirty_dir_name = false;
}

//! \brief obtains the name of the dir where the snapshot is written to.
std::string Snapshot::get_dir_name()
{
  if (m_dirty_dir_name) clean_attributes();
  return m_dir_name;
}

//! \brief sets the file format.
void Snapshot::set_file_format(File_format_2d::Code format)
{
  m_file_format = format;
  m_dirty_image_writer = true;
  m_dirty_file_format = false;
}

//! \brief obtains the file format.
File_format_2d::Code Snapshot::get_file_format()
{
  if (m_dirty_file_format) clean_attributes();
  return m_file_format;
}

//! \brief sets the quality.
void Snapshot::set_quality(Uint quality)
{
  m_quality = quality;
  m_dirty_image_writer = true;
}

//! \brief sets the image.
void Snapshot::set_image_writer(Shared_image_writer image_writer)
{
  m_image_writer = image_writer;
  m_dirty_image_writer = false;
  m_dirty_file_name = true;
  m_dirty_dir_name = true;
  m_dirty_file_format = true;
}

//! \brief obtains the image.
Snapshot::Shared_image_writer Snapshot::get_image_writer()
{
  if (m_dirty_image_writer) clean_image_writer();
  return m_image_writer;
}

//! \brief adds the container to a given scene.
void Snapshot::add_to_scene(Scene_graph* sg) { sg->add_snaphotter(this); }

//! \breif writes a field of this container.
void Snapshot::write_field(const Field_info* field_info, Formatter* formatter)
{
  auto* vrml_formatter = static_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (FILE_FORMAT == field_info->get_id()) {
      vrml_formatter->single_string(field_info->get_name(),
                                    File_format_2d::get_name(m_file_format),
                                    File_format_2d::get_name(Image_writer::s_def_file_format));
      return;
    }
  }
  field_info->write(this, formatter);
}

//! \brief cleans the image writer.
void Snapshot::clean_image_writer()
{
  m_dirty_image_writer = false;
  if (!m_image_writer) m_image_writer.reset(new Image_writer);
  m_image_writer->set_dir_name(get_dir_name());
  m_image_writer->set_file_name(get_file_name());
  m_image_writer->set_file_format(get_file_format());
  m_image_writer->set_quality(get_quality());
}

//! \brief cleans the attributes.
void Snapshot::clean_attributes()
{
  m_dirty_file_name = false;
  m_dirty_dir_name = false;
  m_dirty_file_format = false;
  if (m_image_writer) {
    m_file_name = m_image_writer->get_file_name();
    m_dir_name = m_image_writer->get_dir_name();
    m_file_format = m_image_writer->get_file_format();
  }
}

//! \brief processes change of field.
void Snapshot::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case IMAGE_WRITER:
    m_dirty_image_writer = false;
    m_dirty_file_name = true;
    m_dirty_dir_name = true;
    m_dirty_file_format = true;
    break;

   case FILE_NAME:
    m_dirty_file_name = false;
    m_dirty_image_writer = true;
    break;

   case DIR_NAME:
    m_dirty_file_name = false;
    m_dirty_image_writer = true;
    break;

   case FILE_FORMAT:
    m_dirty_file_name = false;
    m_dirty_image_writer = true;
    break;

   default: break;
  }
  Snapshotter::field_changed(field_info);
}

SGAL_END_NAMESPACE
