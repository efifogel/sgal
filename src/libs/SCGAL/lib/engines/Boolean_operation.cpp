// Copyright (c) 2012 Israel.
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

#if defined(_WIN32)
#pragma warning( disable : 4503)
#endif

/*! \file
 * An engine that computes a Boolean operation.
 */

#include <string>
#include <boost/lexical_cast.hpp>

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_indexed_items.h>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Stl_formatter.hpp"

#include "SCGAL/Exact_polyhedron.hpp"
#include "SCGAL/compute_planes.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"
#include "SCGAL/Boolean_operation.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Boolean_operation::s_tag = "BooleanOperation";
Container_proto* Boolean_operation::s_prototype(NULL);

/*! Operation names */
const char* Boolean_operation::s_operation_names[] =
  { "nop", "intersection" };

// Default values:
REGISTER_TO_FACTORY(Boolean_operation, "Boolean_operation");

//! \brief constructor.
Boolean_operation::Boolean_operation(Boolean proto) :
  Node(proto),
  m_operation(OP_NOP),
  m_trigger(false)
{}

//! \brief destructor.
Boolean_operation::~Boolean_operation() {}

//! \brief .
void Boolean_operation::trigger_changed(const Field_info* /* field_info */)
{ execute(); }

//! \brief executes the engine.
void Boolean_operation::execute()
{
  if (!m_operand1 || !m_operand2) return;

  typedef CGAL::Nef_polyhedron_3<Exact_kernel, CGAL::SNC_indexed_items>
                                                    Nef_polyhedron;

  Exact_polyhedron_geo geometry1;
  geometry1.set_coord_array(m_operand1->get_coord_array());
  std::vector<Uint>& indices1 = m_operand1->get_flat_coord_indices();
  /* Observe that the call to get_flat_coord_indices() may trigger the
   * "cleaning" of the coord indices. Thus, the following query must succeed
   * the previous statement.
   */
  if (m_operand1->are_coord_indices_flat())
    geometry1.set_flat_coord_indices(indices1);
  else {
    std::vector<Uint>& indices1 = m_operand1->get_coord_indices();
    Geo_set& geo_set1 = geometry1;
    geo_set1.set_coord_indices(indices1);
  }
  geometry1.set_num_primitives(m_operand1->get_num_primitives());
  geometry1.set_primitive_type(m_operand1->get_primitive_type());
  const Exact_polyhedron_geo::Polyhedron& polyhedron1 =
    geometry1.get_polyhedron();
  /*! \todo Allow passing a const polyhedron to the constructor of
   * Nef_polyhedron
   */
  auto tmp1 = const_cast<Exact_polyhedron_geo::Polyhedron&>(polyhedron1);
  Nef_polyhedron nef_polyhedron1 = Nef_polyhedron(tmp1);

  Exact_polyhedron_geo geometry2;
  geometry2.set_coord_array(m_operand2->get_coord_array());
  std::vector<Uint>& indices2 = m_operand2->get_flat_coord_indices();
  /* Observe that the call to get_flat_coord_indices() may trigger the
   * "cleaning" of the coord indices. Thus, the following query must succeed
   * the previous statement.
   */
  if (m_operand2->are_coord_indices_flat())
    geometry2.set_flat_coord_indices(indices2);
  else {
    std::vector<Uint>& indices2 = m_operand2->get_coord_indices();
    Geo_set& geo_set2 = geometry2;
    geo_set2.set_coord_indices(indices2);
  }
  geometry2.set_num_primitives(m_operand2->get_num_primitives());
  geometry2.set_primitive_type(m_operand2->get_primitive_type());
  const Exact_polyhedron_geo::Polyhedron& polyhedron2 =
    geometry2.get_polyhedron();
  /*! \todo Allow passing a const polyhedron to the constructor of
   * Nef_polyhedron
   */
  auto tmp2 = const_cast<Exact_polyhedron_geo::Polyhedron&>(polyhedron2);
  Nef_polyhedron nef_polyhedron2 = Nef_polyhedron(tmp2);

  // Compute Boolean operation:
  Nef_polyhedron nef_polyhedron(nef_polyhedron1 * nef_polyhedron2);

  if (!m_result) {
    m_result.reset(new Exact_polyhedron_geo);
    SGAL_assertion(m_result);
  }
  else m_result->clear();

  SGAL_assertion(nef_polyhedron.is_simple());
  Exact_polyhedron p;
  nef_polyhedron.convert_to_polyhedron(p);
  m_result->set_polyhedron(p);

  // Cascade the result field:
  Field* field = get_field(RESULT);
  if (field) field->cascade();
}

//! \brief sets the attributes of this object.
void Boolean_operation::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "operation") {
      Uint num = sizeof(s_operation_names) / sizeof(char*);
      const char** found = std::find(s_operation_names,
                                     &s_operation_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_operation_names;
      if (index < num) set_operation(static_cast<Operation>(index));
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Element::Shared_container cont = elem->get_value(cai);

    if (name == "operand1") {
      boost::shared_ptr<Mesh_set> poly =
        boost::dynamic_pointer_cast<Mesh_set>(cont);
      if (poly != NULL) {
        set_operand1(poly);
        elem->mark_delete(cai);
        continue;
      }
    }

    if (name == "operand2") {
      boost::shared_ptr<Mesh_set> mesh =
        boost::dynamic_pointer_cast<Mesh_set>(cont);
      if (mesh->is_dirty()) mesh->clean();
      set_operand2(mesh);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initilalizes the container prototype.
void Boolean_operation::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Boolean_operation::trigger_changed);

  // trigger
  Boolean_handle_function trigger_func =
    static_cast<Boolean_handle_function>(&Boolean_operation::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          RULE_EXPOSED_FIELD,
                                          trigger_func, exec_func));

  // operation
  Uint_handle_function operation_func =
    reinterpret_cast<Uint_handle_function>
    (&Boolean_operation::operation_handle);
  s_prototype->add_field_info(new SF_uint(OPERATION, "operation",
                                          RULE_EXPOSED_FIELD,
                                          operation_func, exec_func));

  // operand1
  Shared_container_handle_function operand1_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Boolean_operation::operand1_handle);
  s_prototype->add_field_info(new SF_shared_container(OPERAND1, "operand1",
                                                      RULE_EXPOSED_FIELD,
                                                      operand1_func,
                                                      exec_func));

  // operand2
  Shared_container_handle_function operand2_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Boolean_operation::operand2_handle);
  s_prototype->add_field_info(new SF_shared_container(OPERAND2, "operand2",
                                                      RULE_EXPOSED_FIELD,
                                                      operand2_func,
                                                      exec_func));

  // result
  Shared_container_handle_function result_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Boolean_operation::result_handle);
  s_prototype->add_field_info(new SF_shared_container(RESULT, "result",
                                                      RULE_EXPOSED_FIELD,
                                                      result_func));
}

//! \brief deletes the container prototype.
void Boolean_operation::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Boolean_operation::get_prototype()
{
  if (!s_prototype) Boolean_operation::init_prototype();
  return s_prototype;
}

//! \brief writes this container.
void Boolean_operation::write(Formatter* formatter)
{
  Stl_formatter* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  if (stl_formatter) return;
}

SGAL_END_NAMESPACE
