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

SGAL_BEGIN_NAMESPACE

const std::string Exporter::s_tag = "Exporter";
Container_proto* Exporter::s_prototype(nullptr);

// Default values
const std::string Exporter::s_def_file_name = "sgal";
const File_format::Id Exporter::s_def_file_format(File_format::ID_WRL);

REGISTER_TO_FACTORY(Exporter, "Exporter");

//! \brief constructor.
Exporter::Exporter(Boolean proto) :
  Node(proto),
  m_file_name(s_def_file_name),
  m_file_format(s_def_file_format),
  m_trigger(false)
{}

//! \brief destructor.
Exporter::~Exporter() {}

//! \brief executes the engine---writes the content of the scene graph to a file.
void Exporter::execute(const Field_info* /* field_info */)
{
  if (!m_scene_graph) return;
  std::string filename =
    m_file_name + std::string(".") + File_format::get_name(m_file_format);
  m_scene_graph->write(filename, m_file_format);
}

//! \brief initializes the container prototype.
void Exporter::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Node::get_prototype());

  // trigger
  Execution_function exec_func =
    static_cast<Execution_function>(&Exporter::execute);

  Boolean_handle_function trigger_func =
    static_cast<Boolean_handle_function>(&Exporter::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          RULE_EXPOSED_FIELD,
                                          trigger_func,
                                          false, exec_func));

  // fileName
  String_handle_function file_name_func =
    static_cast<String_handle_function>(&Exporter::file_name_handle);
  s_prototype->add_field_info(new SF_string(FILE_NAME, "fileName",
                                            RULE_EXPOSED_FIELD,
                                            file_name_func, s_def_file_name));

  // fileFormat
  Uint_handle_function file_format_func =
    reinterpret_cast<Uint_handle_function>(&Exporter::file_format_handle);
  s_prototype->add_field_info(new SF_uint(FILE_FORMAT, "fileFormat",
                                          RULE_EXPOSED_FIELD,
                                          file_format_func, s_def_file_format));
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

  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
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
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Exporter::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

SGAL_END_NAMESPACE
