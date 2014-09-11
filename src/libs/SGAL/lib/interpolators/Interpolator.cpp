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

/*! \file
 * Pure interpolator engine
 */

#include "SGAL/Interpolator.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Interpolator::s_prototype(nullptr);

//! \brief constructor.
Interpolator::Interpolator(Boolean interpolate_flag, Boolean proto) :
  Node(proto),
  m_fraction(0),
  m_interpolate_flag(interpolate_flag)
{}

//! \brief destructor.
Interpolator::~Interpolator() { m_keys.clear(); }

//! \brief initializes the container prototype.
void Interpolator::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  // fraction
  Execution_function exec_func =
    static_cast<Execution_function>(&Interpolator::execute);
  Float_handle_function fraction_func =
    static_cast<Float_handle_function>(&Interpolator::fraction_handle);
  s_prototype->add_field_info(new SF_float(FRACTION,
                                           "set_fraction",
                                           RULE_EXPOSED_FIELD,
                                           fraction_func, exec_func));
}

//! \brief deletes the container prototype.
void Interpolator::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Interpolator::get_prototype()
{
  if (!s_prototype) Interpolator::init_prototype();
  return s_prototype;
}


//! \brief sets the attributes of the object extracted from the input file.
void Interpolator::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "key") {
      Uint size = get_num_tokens(value);
      m_keys.resize(size);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < size; ++i) svalue >> m_keys[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "interpolateFlag") {
      m_interpolate_flag = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! \brief obtains a list of attributes in this object.
Attribute_list Vector3f_interpolator::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  // Set the keys array
  attrib.first = "key";
  String key("");
  char buf[30];

  for (int i = 0; i < m_num_keys; ++i) {
    sprintf(buf, "%g ", m_keys[i]);
    key = key + String(buf);
  }
  attrib.second = key;
  attribs.push_back(attrib);

  // Set the interpolate flag
  attrib.first = "interpolateFlag";
  attrib.second = (m_interpolate_flag) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE
