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
// $Id: $
// $Revision: $
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

#include "SCGAL/compute_planes.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"
#include "SCGAL/Boolean_operation.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Boolean_operation::s_tag = "BooleanOperation";
Container_proto* Boolean_operation::s_prototype = NULL;

/*! Operation names */
const char* Boolean_operation::s_operation_names[] =
  { "nop", "intersection" };

// Default values:
REGISTER_TO_FACTORY(Boolean_operation, "Boolean_operation");

/*! Constructor */
Boolean_operation::Boolean_operation(Boolean proto) :
  Node(proto),
  m_operation(OP_NOP),
  m_trigger(false)
{}

/*! Destructor */
Boolean_operation::~Boolean_operation() {}

/*! \brief . */
void Boolean_operation::trigger_changed(Field_info* /* field_info */)
{ execute(); }

/*! \brief executes the engine. */
void Boolean_operation::execute()
{
  typedef Exact_polyhedron_geo::Polyhedron          Polyhedron;
  typedef CGAL::Nef_polyhedron_3<Exact_kernel, CGAL::SNC_indexed_items>
                                                    Nef_polyhedron;
  typedef Nef_polyhedron::Volume_const_iterator     Volume_const_iterator;

  Exact_polyhedron_geo geometry1;
  geometry1.set_coord_array(m_operand1->get_coord_array());
  geometry1.set_coord_indices(m_operand1->get_coord_indices());
  geometry1.set_num_primitives(m_operand1->get_num_primitives());
  geometry1.set_primitive_type(m_operand1->get_primitive_type());
  Polyhedron& polyhedron1 = geometry1.get_polyhedron();
  Nef_polyhedron nef_polyhedron1 = Nef_polyhedron(polyhedron1);

  Exact_polyhedron_geo geometry2;
  geometry2.set_coord_array(m_operand2->get_coord_array());
  geometry2.set_coord_indices(m_operand2->get_coord_indices());
  geometry2.set_num_primitives(m_operand2->get_num_primitives());
  geometry2.set_primitive_type(m_operand2->get_primitive_type());
  Polyhedron& polyhedron2 = geometry2.get_polyhedron();
  Nef_polyhedron nef_polyhedron2 = Nef_polyhedron(polyhedron2);

  // Compute Boolean operation:
  Nef_polyhedron nef_polyhedron(nef_polyhedron1 * nef_polyhedron2);

  m_result.reset(new Exact_polyhedron_geo);
  SGAL_assertion(nef_polyhedron.is_simple());
  Polyhedron p;
  nef_polyhedron.convert_to_polyhedron(p);
  m_result->set_polyhedron(p);

  // Cascade the result field:
  Field* field = get_field(RESULT);
  if (field) field->cascade();
}

/*! \brief sets the attributes of this object. */
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

/*! \brief initilalizes the container prototype. */
void Boolean_operation::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());
  
  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Boolean_operation::trigger_changed);

  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          get_member_offset(&m_trigger),
                                          exec_func));

  // We use SF_int (instead of SF_uint) to allow connecting the value
  // field of an Incrementor, which is of int type (and not Uint) to this
  // field.
  s_prototype->add_field_info(new SF_int(OPERATION, "operation",
                                         get_member_offset(&m_operation),
                                         exec_func));

  SF_shared_container* field;
  field = new SF_shared_container(OPERAND1, "operand1",
                                  get_member_offset(&m_operand1), exec_func);
  s_prototype->add_field_info(field);    

  field = new SF_shared_container(OPERAND2, "operand2",
                                  get_member_offset(&m_operand2), exec_func);
  s_prototype->add_field_info(field);    

  field = new SF_shared_container(RESULT, "result",
                                  get_member_offset(&m_result));
  s_prototype->add_field_info(field);    
}

/*! \brief deletes the container prototype. */
void Boolean_operation::delete_prototype() 
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Boolean_operation::get_prototype() 
{
  if (!s_prototype) Boolean_operation::init_prototype();
  return s_prototype;
} 

SGAL_END_NAMESPACE
