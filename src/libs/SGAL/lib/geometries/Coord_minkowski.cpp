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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <string>

#include "SGAL/Coord_minkowski.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Coord_minkowski::s_prototype(nullptr);
const std::string Coord_minkowski::s_tag = "CoordinateMinkowski";

Boolean Coord_minkowski::s_def_enabled(true);

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Coord_minkowski, "Coord_minkowski");

//! constructs from proto.
Coord_minkowski::Coord_minkowski(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_execute(false)
{}

//! \brief sets the attributes of this object.
void Coord_minkowski::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "enabled") {
      m_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    Shared_container cont = elem->get_value(cai);
    if (name == "coord1") {
      auto coord_array = boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array1(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "coord2") {
      auto coord_array = boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array2(coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the attributes of this node.
void Coord_minkowski::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  Execution_function exec_func;

  // Add the field-info records to the prototype:
  // enabled
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Coord_minkowski::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          enabled_func));

  // execute
  exec_func = static_cast<Execution_function>(&Coord_minkowski::execute);
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>(&Coord_minkowski::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXECUTE, "execute",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          execute_func,
                                          exec_func));

  // coord1
  Shared_container_handle_function coord1_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_minkowski::coord_array1_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD1, "coord1",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      coord1_func, exec_func));

  // coord2
  Shared_container_handle_function coord2_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_minkowski::coord_array2_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD2, "coord2",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      coord2_func, exec_func));

  // coord
  Shared_container_handle_function coord_changed_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Coord_minkowski::coord_array_changed_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_CHANGED,
                                                      "coord_changed",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      coord_changed_func));
}

//! \brief deletes the node prototype.
void Coord_minkowski::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the node prototype.
Container_proto* Coord_minkowski::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief transforms the input vertices and store the results in the output
 * vertices
 */
void Coord_minkowski::execute(const Field_info* /* field_info */)
{
  if (!m_enabled) return;
  if (!m_coord_array1) return;
  if (!m_coord_array2) return;

  auto size1 = m_coord_array1->size();
  auto size2 = m_coord_array2->size();
  auto size = size1 * size2;

  if (!m_coord_array_changed) {
    m_coord_array_changed.reset(new Coord_array_3d(size));
    SGAL_assertion(m_coord_array_changed);
  }
  else m_coord_array_changed->resize(size);

  boost::shared_ptr<Coord_array_3d> coord_array_changed =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array_changed);
  SGAL_assertion(coord_array_changed);
  boost::shared_ptr<Coord_array_3d> coord_array1 =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array1);
  SGAL_assertion(coord_array1);
  boost::shared_ptr<Coord_array_3d> coord_array2 =
    boost::static_pointer_cast<Coord_array_3d>(m_coord_array2);
  SGAL_assertion(coord_array2);

  size_t k(0);
  for (auto i = 0; i < size1; ++i) {
    for (auto j = 0; j < size2; ++j) {
      (*coord_array_changed)[k++].add((*coord_array1)[i], (*coord_array2)[j]);
    }
  }

  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();
  m_coord_array_changed->process_content_changed();
}

SGAL_END_NAMESPACE
