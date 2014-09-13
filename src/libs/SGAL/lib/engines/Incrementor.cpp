// Copyright (c) 2008 Israel.
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

/*! \file
 *
 * Increments a counter within a range
 */

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Incrementor.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Incrementor::s_tag = "Incrementor";
Container_proto* Incrementor::s_prototype(nullptr);

// Default values:
const Int32 Incrementor::s_def_min_value(0);
const Int32 Incrementor::s_def_max_value(1);

REGISTER_TO_FACTORY(Incrementor, "Incrementor");

//! Constructor
Incrementor::Incrementor(Boolean proto) :
  Node(proto),
  m_min_value(s_def_min_value),
  m_max_value(s_def_max_value),
  m_trigger(false),
  m_value(s_def_min_value)
{}

//! \brief initializes the container prototype.
void Incrementor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Incrementor::execute);

  Int32_handle_function min_value_func =
    static_cast<Int32_handle_function>(&Incrementor::min_value_handle);
  s_prototype->add_field_info(new SF_int32(MIN_VALUE, "minValue",
                                           RULE_EXPOSED_FIELD,
                                           min_value_func, exec_func));

  Int32_handle_function max_value_func =
    static_cast<Int32_handle_function>(&Incrementor::max_value_handle);
  s_prototype->add_field_info(new SF_int32(MAX_VALUE, "maxValue",
                                           RULE_EXPOSED_FIELD,
                                           max_value_func, exec_func));

  Boolean_handle_function trigger_func =
    static_cast<Boolean_handle_function>(&Incrementor::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          RULE_EXPOSED_FIELD,
                                          trigger_func,
                                          exec_func));

  Int32_handle_function value_func =
    static_cast<Int32_handle_function>(&Incrementor::value_handle);
  s_prototype->add_field_info(new SF_int32(VALUE, "value",
                                           RULE_EXPOSED_FIELD,
                                           value_func));
}

//! \brief deletes the container prototype.
void Incrementor::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Incrementor::get_prototype()
{
  if (!s_prototype) Incrementor::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Incrementor::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "minValue") {
      m_min_value = boost::lexical_cast<Int32>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "maxValue") {
      m_max_value = boost::lexical_cast<Int32>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "value") {
      m_value = boost::lexical_cast<Int32>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "trigger") {
      m_trigger = boost::lexical_cast<Boolean>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief executes the engine.
void Incrementor::execute(const Field_info* /*! field_info */)
{
  if (m_value == m_max_value) m_value = m_min_value;
  else ++m_value;
  Field* value_field = get_field(VALUE);
  if (value_field != nullptr) value_field->cascade();
}

SGAL_END_NAMESPACE
