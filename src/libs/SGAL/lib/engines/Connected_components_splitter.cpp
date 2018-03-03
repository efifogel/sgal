// Copyright (c) 2016 Israel.
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

#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Connected_components_splitter.hpp"
#include "SGAL/Connected_components_splitter_visitor.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Modeling.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Connected_components_splitter::s_tag =
  "ConnectedComponentsSplitter";
Container_proto* Connected_components_splitter::s_prototype(nullptr);

// Default values:

REGISTER_TO_FACTORY(Connected_components_splitter,
                    "Connected_components_splitter");

//! \brief constructs
Connected_components_splitter::Connected_components_splitter(Boolean proto) :
  Node(proto),
  m_trigger(false)
{}

//! \brief destruct.
Connected_components_splitter::~Connected_components_splitter() {}

/*! \brief triggers the execution of the engine as a response to change in one
 * of the input fields.
 */
void Connected_components_splitter::
trigger_changed(const Field_info* /* field_info */)
{ execute(); }

//! \brief initializes the container prototype.
void Connected_components_splitter::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>
    (&Connected_components_splitter::trigger_changed);

  auto trigger_func =
    static_cast<Boolean_handle_function>
    (&Connected_components_splitter::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_rule::RULE_IN,
                                          trigger_func,
                                          exec_func));

  // operand
  auto operand_func = reinterpret_cast<Shared_container_handle_function>
    (&Connected_components_splitter::operand_handle);
  s_prototype->add_field_info(new SF_shared_container(OPERAND, "operand",
                                                      Field_rule::RULE_IN,
                                                      operand_func,
                                                      exec_func));

  // components
  auto components_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Connected_components_splitter::components_handle);
  s_prototype->add_field_info(new MF_shared_container(COMPONENTS, "components",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      components_func));
}

//! \brief deletes the container prototype.
void Connected_components_splitter::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Connected_components_splitter::get_prototype()
{
  if (!s_prototype) Connected_components_splitter::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Connected_components_splitter::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  // for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
  //   const auto& name = elem->get_name(ai);
  //   const auto& value = elem->get_value(ai);
  // }

  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "operand") {
      auto mesh = boost::dynamic_pointer_cast<Indexed_face_set>(cont);
      set_operand(mesh);
      elem->mark_delete(cai);
      continue;
    }
    //! \todo
    // if (name == "components") {
    //   auto components = boost::dynamic_pointer_cast<Indexed_face_set>(cont);
    //   set_components(components);
    //   elem->mark_delete(cai);
    //   continue;
    // }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief executes the engine.
void Connected_components_splitter::execute()
{
  if (!m_operand) return;

  auto operand = boost::dynamic_pointer_cast<Indexed_face_set>(m_operand);
  SGAL_assertion(operand);

  auto num_ccs = operand->get_number_of_connected_components();
  m_components.resize(num_ccs);
  if (num_ccs == 1) {
    m_components[0] = operand;
  }
  else {
    const auto& polyhedron = operand->get_polyhedron();
    auto type = operand->get_primitive_type();
    for (auto i = 0; i != num_ccs; ++i) {
      m_components[i].reset(new Indexed_face_set);
      m_components[i]->set_coord_array(operand->get_coord_array());
      m_components[i]->set_primitive_type(type);
      auto& indices = m_components[i]->get_empty_facet_coord_indices();
      Connected_components_splitter_visitor visitor(i);
      auto num = boost::apply_visitor(visitor, polyhedron, indices);
      m_components[i]->set_num_primitives(num);
      m_components[i]->facet_coord_indices_changed();
      m_components[i]->add_to_scene(m_scene_graph);
    }
  }

  // Cascade the components field:
  auto* field = get_field(COMPONENTS);
  if (field) field->cascade();
}

//! \brief writes this container.
void Connected_components_splitter::write(Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) Node::write(formatter);
}

//! \brief adds the container to a given scene.
void Connected_components_splitter::add_to_scene(Scene_graph* sg)
{ m_scene_graph = sg; }

SGAL_END_NAMESPACE
