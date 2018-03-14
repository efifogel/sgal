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

#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_type.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/multi_istream_iterator.hpp"
#include "SGAL/io_vector2f.hpp"
#include "SGAL/io_vector3f.hpp"
#include "SGAL/io_rotation.hpp"
#include "SGAL/to_boolean.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Proto::Proto(Boolean proto) : Group(proto), m_prototype(nullptr) {}

//! \brief destructs.
Proto::~Proto() {}

//! \brief deletes the container prototype.
void Proto::delete_prototype()
{
  //! \todo only the prototype container should delete the container prototype.
  // if (!m_prototype) return;
  // delete m_prototype;
  // m_prototype = nullptr;
}

//! \brief Creates a new container of this type (virtual copy constructor)..
Container* Proto::create()
{
  auto proto = new Proto();
  SGAL_assertion(proto);
  proto->set_prototype(get_prototype());
  return proto;
}

//! \brief initializes the container prototype.
void Proto::init_prototype()
{
  if (m_prototype) return;
  m_prototype = new Container_proto(Group::get_prototype());

  // The container-prototype does not have static fields.
  // If there had been, they would have been defined here.
}

//! \brief obtains the container prototype.
Container_proto* Proto::get_prototype()
{
  if (!m_prototype) Proto::init_prototype();
  return m_prototype;
}

//! \brief \todo.
void Proto::execute(const Field_info* field_info) {}

//! \brief adds a field information record to the prototype.
void Proto::add_field_info(Field_rule rule, Field_type type,
                           const String& name, const String& value)
{
#if !defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(Trace::PROTO)) {
    std::cout << "add_field_info() " << name << ", " << value << std::endl;
  }
#endif

  auto* prototype = get_prototype();
  SGAL_assertion(prototype);
  Execution_function exec_func = (rule == Field_rule::RULE_IN) ?
    static_cast<Execution_function>(&Proto::execute) : nullptr;

  Variant_field variant_field;
  auto id = LAST + m_fields.size();
  Field_info* field_info(nullptr);
  switch (type) {
   case Field_type::SF_BOOL:
    {
      auto initial_value = value.empty() ? false : to_boolean(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_BOOL>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::SF_FLOAT:
    {
      auto initial_value = value.empty() ? 0 : boost::lexical_cast<Float>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_FLOAT>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::SF_TIME:
    {
      auto initial_value =
        value.empty() ? 0 : boost::lexical_cast<Scene_time>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_TIME>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::SF_INT32:
    {
      auto initial_value = value.empty() ? 0 : boost::lexical_cast<Int32>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_INT32>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::SF_VEC2F:
    {
      Vector2f initial_value;
      if (! value.empty()) initial_value = boost::lexical_cast<Vector2f>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_VEC2F>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::SF_VEC3F:
    {
      Vector3f initial_value;
      if (! value.empty()) initial_value = boost::lexical_cast<Vector3f>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_VEC3F>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::SF_COLOR:
    {
      Vector3f initial_value;
      if (! value.empty()) initial_value = boost::lexical_cast<Vector3f>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_COLOR>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::SF_ROTATION:
    {
      Rotation initial_value;
      if (! value.empty()) initial_value = boost::lexical_cast<Rotation>(value);
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_ROTATION>(id, name, rule,
                                                      initial_value, exec_func);
    }
    break;

   case Field_type::SF_STR:
    {
      variant_field = value;
      field_info = create_fi<Field_type::SF_STR>(id, name, rule, value,
                                                 exec_func);
    }
    break;

   case Field_type::SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case Field_type::SF_SHARED_CONTAINER:
    {
      Shared_container initial_value;
      variant_field = initial_value;
      field_info = create_fi<Field_type::SF_SHARED_CONTAINER>(id, name, rule,
                                                              initial_value,
                                                              exec_func);
    }
    break;

   case Field_type::MF_BOOL:
    {
      Boolean_array initial_value;
      std::stringstream ss(value);
      std::transform(std::istream_iterator<std::string>(ss),
                     std::istream_iterator<std::string>(),
                     std::back_inserter(initial_value), to_boolean);
      variant_field = initial_value;
      field_info = create_fi<Field_type::MF_BOOL>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::MF_FLOAT:
    {
      Float_array initial_value;
      std::stringstream ss(value);
      std::copy(std::istream_iterator<Float>(ss),
                std::istream_iterator<Float>(),
                std::back_inserter(initial_value));
      variant_field = initial_value;
      field_info = create_fi<Field_type::MF_FLOAT>(id, name, rule,
                                                   initial_value, exec_func);
    }
    break;

   case Field_type::MF_TIME:
    {
      Scene_time_array initial_value;
      std::stringstream ss(value);
      std::copy(std::istream_iterator<Scene_time>(ss),
                std::istream_iterator<Scene_time>(),
                std::back_inserter(initial_value));
      variant_field = initial_value;
      field_info = create_fi<Field_type::MF_TIME>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::MF_INT32:
    {
      Int32_array initial_value;
      std::stringstream ss(value);
      std::copy(std::istream_iterator<Int32>(ss),
                std::istream_iterator<Int32>(),
                std::back_inserter(initial_value));
      variant_field = initial_value;
      field_info = create_fi<Field_type::MF_INT32>(id, name, rule,
                                                   initial_value, exec_func);
    }
     break;

   case Field_type::MF_VEC2F:
    {
     Vector2f_array initial_value;
     std::stringstream ss(value);
     std::transform(multi_istream_iterator<2>(ss),
                    multi_istream_iterator<2>(),
                    std::back_inserter(initial_value),
                    &boost::lexical_cast<Vector2f, String>);
     variant_field = initial_value;
     field_info = create_fi<Field_type::MF_VEC2F>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::MF_VEC3F:
   case Field_type::MF_COLOR:
    {
     Vector3f_array initial_value;
     std::stringstream ss(value);
     std::transform(multi_istream_iterator<3>(ss),
                    multi_istream_iterator<3>(),
                    std::back_inserter(initial_value),
                    &boost::lexical_cast<Vector3f, String>);
     variant_field = initial_value;
     field_info = create_fi<Field_type::MF_VEC3F>(id, name, rule, initial_value,
                                                  exec_func);
    }
    break;

   case Field_type::MF_ROTATION:
    {
     Rotation_array initial_value;
     std::stringstream ss(value);
     std::transform(multi_istream_iterator<4>(ss),
                    multi_istream_iterator<4>(),
                    std::back_inserter(initial_value),
                    &boost::lexical_cast<Rotation, String>);
     variant_field = initial_value;
     field_info = create_fi<Field_type::MF_ROTATION>(id, name, rule,
                                                     initial_value, exec_func);
    }
    break;

   case Field_type::MF_STR:
    {
     String_array initial_value;
      std::stringstream ss(value);
      std::copy(std::istream_iterator<std::string>(ss),
                std::istream_iterator<std::string>(),
                std::back_inserter(initial_value));
     variant_field = initial_value;
     field_info = create_fi<Field_type::MF_STR>(id, name, rule, initial_value,
                                                exec_func);
    }
    break;

   case Field_type::MF_SHARED_CONTAINER:
    {
     Shared_container_array initial_value;
     SGAL_error_msg("Not supported yet!");
     variant_field = initial_value;
     field_info = create_fi<Field_type::MF_SHARED_CONTAINER>(id, name, rule,
                                                             initial_value,
                                                             exec_func);
    }
    break;

   default:
    std::cerr << "Unrecognized type (" << get_field_type_name(type) << ")!"
              << std::endl;
    break;
  }

  if (field_info) {
    prototype->add_field_info(field_info);
    m_fields.push_back(variant_field);
  }
}

//! \brief sets all field info records of a prototype container.
void Proto::set_field_infos(Element* elem)
{
  for (auto fi = elem->field_attrs_begin(); fi != elem->field_attrs_end(); ++fi)
  {
    auto rule = elem->get_rule(fi);
    auto type = elem->get_type(fi);
    const auto& name = elem->get_name(fi);
    const auto& value = elem->get_value(fi);
    add_field_info(rule, type, name, value);
  }

  // Remove all the marked attributes:
  //! \todo need to remove all. Actually, replace the use of Element with
  // something more suitable for field-infos.
  elem->delete_marked();
}

//! \brief sets the attributes of the object extracted from the input file.
void Proto::set_attributes(Element* elem)
{
  // Find the children default value and clone it.
  auto* field_info = get_field_info(CHILDREN);
  SGAL_assertion(field_info);
  auto* childs_field_info = reinterpret_cast<MF_shared_container*>(field_info);
  SGAL_assertion(childs_field_info);
  const auto& proto_childs = childs_field_info->get_initial_value();

  Container_array& childs = get_childs();
  childs.resize(proto_childs.size());

  /*! \todo Instead of simply cloning, we need to replace the fields in the
   * source sub-graphs with the formal values of the corresponding fields given
   * in the declaration section. These values are stored as the initial values
   * of field-information records (in the prototype). Make sure to skip the
   * 'children' field-information record when the list of information records
   * are processed.
   * \todo need to call:
   * 1. cont->add_scene_graph(scene_graph), and
   * 2. scene_graph->add_container(cont)
   * where cont is the cloned container, for every cloned container.
   * It implies, that we need to pass the scene graph to the clone() function
   * as an argument.
   */
  std::transform(proto_childs.begin(), proto_childs.end(), childs.begin(),
                 [] (Shared_container cont)
                 { return Shared_container(cont->clone()); });

  // Group::set_attributes(elem);
  // SGAL_error_msg("Not implemented yet!");
}

SGAL_END_NAMESPACE
