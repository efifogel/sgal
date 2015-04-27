// Copyright (c) 2014 Israel.
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

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Exporter.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/File_format.hpp"
#include "SGAL/Vrml_formatter.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exporter::s_tag = "Exporter";
Container_proto* Exporter::s_prototype(nullptr);

// Default values
const std::string Exporter::s_def_dir_name(".");
const std::string Exporter::s_def_file_name("sgal");
const File_format::Id Exporter::s_def_file_format(File_format::ID_WRL);
const Boolean Exporter::s_def_separate(false);

REGISTER_TO_FACTORY(Exporter, "Exporter");

//! \brief constructor.
Exporter::Exporter(Boolean proto) :
  Node(proto),
  m_dir_name(s_def_dir_name),
  m_file_name(s_def_file_name),
  m_file_format(s_def_file_format),
  m_separate(s_def_separate),
  m_count(0),
  m_trigger(false)
{}

//! \brief destructor.
Exporter::~Exporter() {}

//! \brief executes the engine---writes the content of the scene graph to a file.
void Exporter::execute(const Field_info* /* field_info */)
{
  if (!m_scene_graph) return;
  std::string file_name = m_dir_name + "/" + m_file_name;
  if (m_separate) {
    std::ostringstream oss;
    oss << m_count++;
    file_name += "_" + oss.str();
  }
  file_name += std::string(".") + File_format::get_name(m_file_format);
  m_scene_graph->write(file_name, m_file_format);
}

//! \brief initializes the container prototype.
void Exporter::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Node::get_prototype());

  // trigger
  Execution_function exec_func =
    static_cast<Execution_function>(&Exporter::execute);

  auto trigger_func =
    static_cast<Boolean_handle_function>(&Exporter::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_info::RULE_IN,
                                          trigger_func,
                                          false, exec_func));

  // dirName
  String_handle_function dir_name_func =
    static_cast<String_handle_function>(&Exporter::dir_name_handle);
  s_prototype->add_field_info(new SF_string(DIR_NAME,
                                            "dirName",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            dir_name_func,
                                            s_def_dir_name));

  // fileName
  auto file_name_func =
    static_cast<String_handle_function>(&Exporter::file_name_handle);
  s_prototype->add_field_info(new SF_string(FILE_NAME, "fileName",
                                            Field_info::RULE_EXPOSED_FIELD,
                                            file_name_func, s_def_file_name));

  // fileFormat
  auto file_format_func =
    reinterpret_cast<Uint_handle_function>(&Exporter::file_format_handle);
  s_prototype->add_field_info(new SF_uint(FILE_FORMAT, "fileFormat",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          file_format_func, s_def_file_format));

  // separate
  auto separate_func =
    reinterpret_cast<Boolean_handle_function>(&Exporter::separate_handle);
  s_prototype->add_field_info(new SF_bool(SEPARATE, "separate",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          separate_func, s_def_separate));
}

//! \brief deletes the container prototype.
void Exporter::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Exporter::get_prototype()
{
  if (!s_prototype) Exporter::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Exporter::set_attributes(Element* elem)
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
      std::string str = strip_double_quotes(value);
      Uint i;
      for (i = 0; i < File_format::NUM_IDS; ++i) {
        if (str == File_format::get_name(i)) {
          set_file_format(static_cast<File_format::Id>(i));
          break;
        }
      }
      if (i == File_format::NUM_IDS) {
        std::cerr << "Illegal file format (" << value.c_str() << ")!"
                  << std::endl;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "separate") {
      m_separate = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Exporter::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

//! \breif writes a field of this container.
void Exporter::write_field(const Field_info* field_info, Formatter* formatter)
{
  auto* vrml_formatter = static_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (FILE_FORMAT == field_info->get_id()) {
      vrml_formatter->single_string(field_info->get_name(),
                                    File_format::get_name(m_file_format),
                                    File_format::get_name(s_def_file_format));
      return;
    }
  }
  field_info->write(this, formatter);
}

SGAL_END_NAMESPACE
