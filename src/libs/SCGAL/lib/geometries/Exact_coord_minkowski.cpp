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
#include <sstream>
#include <string>

#include <CGAL/basic.h>
#include <CGAL/number_utils_classes.h>

#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"

#include "SCGAL/Exact_coord_minkowski.hpp"
#include "SCGAL/Exact_coord_array.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_coord_minkowski::s_tag = "ExactCoordinateMinkowski";
Container_proto* Exact_coord_minkowski::s_prototype(nullptr);

const Boolean Exact_coord_minkowski::s_def_enabled(true);

REGISTER_TO_FACTORY(Exact_coord_minkowski, "Exact_coord_minkowski");

//! \brief constructor.
Exact_coord_minkowski::Exact_coord_minkowski(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_changed(false),
  m_execute(false)
{}

//! \brief initializes the node prototype.
void Exact_coord_minkowski::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  Execution_function exec_func =
    static_cast<Execution_function>(&Exact_coord_minkowski::execute);

  // exactExecute
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>
    (&Exact_coord_minkowski::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXACT_EXECUTE, "exactExecute",
                                          RULE_EXPOSED_FIELD,
                                          execute_func, exec_func));

  Shared_container_handle_function coord1_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Exact_coord_minkowski::coord_array1_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD1, "coord1",
                                                      RULE_EXPOSED_FIELD,
                                                      coord1_func, exec_func));

  // coord2
  Shared_container_handle_function coord2_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Exact_coord_minkowski::coord_array2_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD2, "coord2",
                                                      RULE_EXPOSED_FIELD,
                                                      coord2_func, exec_func));

  // coord
  Shared_container_handle_function coord_changed_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Exact_coord_minkowski::coord_array_changed_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_CHANGED,
                                                      "coord_changed",
                                                      RULE_EXPOSED_FIELD,
                                                      coord_changed_func));
}

//! \brief deletes the node prototype.
void Exact_coord_minkowski::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Exact_coord_minkowski::get_prototype()
{
  if (!s_prototype) Exact_coord_minkowski::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Exact_coord_minkowski::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      m_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Shared_container cont = elem->get_value(cai);
    if (name == "coord1") {
      Shared_exact_coord_array coord_array =
        boost::dynamic_pointer_cast<Exact_coord_array>(cont);
      set_coord_array1(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "coord2") {
      Shared_exact_coord_array coord_array =
        boost::dynamic_pointer_cast<Exact_coord_array>(cont);
      set_coord_array2(coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief transforms the input vertices.
void Exact_coord_minkowski::execute(const Field_info* field_info)
{
  if (!m_enabled) return;
  if (!m_coord_array1) return;
  if (!m_coord_array2) return;

  boost::shared_ptr<Exact_coord_array> exact_coord_array1 =
    boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array1);
  SGAL_assertion(exact_coord_array1);
  boost::shared_ptr<Exact_coord_array> exact_coord_array2 =
    boost::dynamic_pointer_cast<Exact_coord_array>(m_coord_array2);
  SGAL_assertion(exact_coord_array2);

  Uint size1 = exact_coord_array1->size();
  Uint size2 = exact_coord_array2->size();
  Uint size = size1 * size2;

  if (!m_coord_array_changed) {
    m_coord_array_changed.reset(new Exact_coord_array(size));
    SGAL_assertion(m_coord_array_changed);
  }
  else
    m_coord_array_changed->resize(size);

  boost::shared_ptr<Exact_coord_array> coord_array_changed =
    boost::static_pointer_cast<Exact_coord_array>(m_coord_array_changed);
  SGAL_assertion(coord_array_changed);

  Exact_coord_array::Exact_point_iter pi1 = exact_coord_array1->begin();
  for (; pi1 != exact_coord_array1->end(); ++pi1) {
    const Exact_point_3& p1 = *pi1;
    Exact_vector_3 v(CGAL::ORIGIN, p1);
    Exact_coord_array::Exact_point_iter pi2 = exact_coord_array2->begin();
    for (; pi2 != exact_coord_array2->end(); ++pi2) {
      const Exact_point_3& p2 = *pi2;
      coord_array_changed->push_back(p2 + v);
    }
  }

  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  Field* changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();
}

SGAL_END_NAMESPACE
