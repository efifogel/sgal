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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Vector3f_interpolator - implementation
 */

#include "SGAL/Vector3f_interpolator.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

std::string Vector3f_interpolator::s_tag = "Vector3fInterpolator";
Container_proto* Vector3f_interpolator::s_prototype(NULL);

REGISTER_TO_FACTORY(Vector3f_interpolator, "Vector3f_interpolator");

/*! Constructor
 * @param interpolate_flag (in) initialization value for the interpolate flag
 * field
*/
Vector3f_interpolator::Vector3f_interpolator(Boolean interpolate_flag,
                                             Boolean proto) :
  Interpolator(interpolate_flag, proto),
  m_last_location(0)
{}

/*! Destructor */
Vector3f_interpolator::~Vector3f_interpolator()
{
  m_values.clear();
}

/*! initializes the node prototype */
void Vector3f_interpolator::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Interpolator::get_prototype());

  // value
  Vector3f_handle_function value_func =
    static_cast<Vector3f_handle_function>(&Vector3f_interpolator::value_handle);
  s_prototype->add_field_info(new SF_vector3f(VALUE,"value_changed",
                                              value_func));
}

/*! \brief delete the prototype */
void Vector3f_interpolator::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtain the prototype */
Container_proto* Vector3f_interpolator::get_prototype()
{
  if (!s_prototype) Vector3f_interpolator::init_prototype();
  return s_prototype;
}

/*! The interpolation execution function.
 * Activated through the cascade of the m_fraction field.
 * The function calculates m_value, updates it and activate cascade on it
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Vector3f_interpolator::execute(Field_info* /* field_info */)
{
  Field* value = get_field(VALUE);
  // if there is no connection to the value field there is no need to execute
  // the interpolation
  if (value == NULL) return;

  // if there are no array set the value to zero and return
  if ((m_keys.size() == 0) || m_values.size() == 0) {
    m_value = Vector3f(0.0f, 0.0f, 0.0f);
    value->cascade();
    return;
  }

  // If there is only one key/value pair, set it and return
  if (m_keys.size() == 1) {
    m_value = m_values[0];
    value->cascade();
    return;
  }

  // If the fraction is larger than the last key, set the value to the last one
  if (m_fraction >= m_keys[m_keys.size()-1]) {
    m_value = m_values[m_keys.size()-1];
    value->cascade();
    return;
  }

  // If the fraction is smaller than the first key - set the value to the
  // first one
  if (m_fraction <= m_keys[0]) {
    m_value = m_values[0];
    value->cascade();
    return;
  }

  // Start looking for the fraction location in the keys array
  // from the last fraction location place
  Uint location = m_last_location;

  // Loop up the array to find the closest key which is bigger than fraction
  while (location + 1 < m_keys.size() && m_fraction > m_keys[location+1])
    ++location;

  // Loop down the array to find the closest key which is smaller than fraction
  while (location > 0 && m_fraction < m_keys[location]) location--;

  // Set the last location to the current one for next execution
  m_last_location = location;

  // Set m_value by interpolating between two values or
  // just return the first one (if interpolate flag is false)
  if (m_interpolate_flag) {
    const Vector3f& location_value = m_values[location];
    const Vector3f& next_location_value = m_values[location+1];
    Float key_delta = m_keys[location+1] - m_keys[location];
    Float fration_delta = m_fraction - m_keys[location];

    for (int i = 0; i < 3; ++i) {
      Float delta = next_location_value[i] - location_value[i];
      m_value[i] = location_value[i] + (fration_delta / key_delta) * delta;
    }
  }
  else
    m_value = m_values[location];

  // Cascade the updated value
  value->cascade();
}

/*! \brief sets the attributes of this container. */
void Vector3f_interpolator::set_attributes(Element* elem)
{
  Interpolator::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "keyValue") {
      Uint num_values = get_num_tokens(value);
      Uint num_vecs = num_values / 3;
      m_values.resize(num_vecs);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_vecs; ++i) svalue >> m_values[i];
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*! Get a list of attributes in this object. This method is called only
 * from the Builder side.
 *
 * \return a list of attributes
 */
Attribute_list Vector3f_interpolator::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Interpolator::get_attributes();

  // Set the keys array
  attrib.first = "key";
  String key("");
  char buf[30];

  for (int i=0;i<m_num_keys;i++) {
    sprintf(buf, "%g ", m_keys[i]);
    key = key + String(buf);
  }
  attrib.second = key;
  attribs.push_back(attrib);

  // Set the values array
  attrib.first = "keyValue";
  String key_value("");
  for (i = 0; i < m_num_keys; i++) {
    for (int j = 0; j < 3; j++) {
      const Vector3f & cur_value = m_values[i];
      sprintf(buf, "%g ", cur_value[j]);
      key_value = key_value + std::string(buf);
    }
  }
  attrib.second = key_value;
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE
