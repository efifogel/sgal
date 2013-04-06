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
// Id: $
// $Revision: 12554 $
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
#include "SGAL/Coord_array.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Coord_minkowski::s_prototype = NULL;
const std::string Coord_minkowski::s_tag = "CoordinateMinkowski";

Boolean Coord_minkowski::s_def_enabled(true);

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Coord_minkowski, "Coord_minkowski");

/*! A parameter-less constructor. */
Coord_minkowski::Coord_minkowski(Boolean proto) :
  Container(proto), 
  m_enabled(s_def_enabled),
  m_changed(false),
  m_execute(false)
{}

/*! \brief Sets the attributes of this object. */
void Coord_minkowski::set_attributes(Element* elem)
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
    Container* cont = elem->get_value(cai);
    if (name == "coord1") {
      Coord_array* coord_array = dynamic_cast<Coord_array*>(cont);
      Shared_coord_array shared_coord_array;
      shared_coord_array.reset(coord_array);
      set_coord_array1(shared_coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "coord2") {
      Coord_array* coord_array = dynamic_cast<Coord_array*>(cont);
      Shared_coord_array shared_coord_array;
      shared_coord_array.reset(coord_array);
      set_coord_array2(shared_coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief sets the attributes of this node. */
void Coord_minkowski::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);

  Execution_function exec_func;
  
  // Add the field-info records to the prototype:
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          get_member_offset(&m_enabled)));

  s_prototype->add_field_info(new SF_bool(CHANGED, "changed",
                                          get_member_offset(&m_changed)));

  exec_func = static_cast<Execution_function>(&Coord_minkowski::execute);  

  s_prototype->add_field_info(new SF_bool(EXECUTE, "execute",
                                          get_member_offset(&m_execute),
                                          exec_func));

  SF_shared_container* field;
  field = new SF_shared_container(COORD1, "coord1",
                                  get_member_offset(&m_coord_array1),
                                  exec_func);
  s_prototype->add_field_info(field);

  field = new SF_shared_container(COORD2, "coord2",
                                  get_member_offset(&m_coord_array2),
                                  exec_func);
  s_prototype->add_field_info(field);

  field = new SF_shared_container(COORD_CHANGED, "coord_changed",
                                  get_member_offset(&m_coord_array_changed));
  s_prototype->add_field_info(field);
}

/*! \brief deletes the node prototype. */
void Coord_minkowski::delete_prototype()
{
  delete s_prototype;
  s_prototype = 0;
}

/*! \brief obtains the node prototype. */
Container_proto* Coord_minkowski::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief transforms the input vertices and store the results in the output
 * vertices
 */
void Coord_minkowski::execute(Field_info* /* field_info */)
{
  if (!m_enabled) return;
  if (!m_coord_array1) return;
  if (!m_coord_array2) return;

  unsigned int size1 = m_coord_array1->size();
  unsigned int size2 = m_coord_array2->size();
  unsigned int size = size1 * size2;

  if (!m_coord_array_changed) 
    m_coord_array_changed.reset(new Coord_array(size));
  else
    m_coord_array_changed->resize(size);

  Uint k = 0;
  for (Uint i = 0; i < size1; ++i) {
    for (Uint j = 0; j < size2; ++j) {
      (*m_coord_array_changed)[k++].add((*m_coord_array1)[i],
                                        (*m_coord_array2)[j]);
    }
  }
  
  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field) coord_changed_field->cascade();

  m_changed = true;
  Field* changed_field = get_field(CHANGED);
  if (changed_field) changed_field->cascade();

  m_coord_array_changed->process_content_changed();
}

SGAL_END_NAMESPACE
