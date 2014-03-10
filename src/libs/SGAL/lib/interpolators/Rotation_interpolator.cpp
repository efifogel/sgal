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

/*!
 * Rotation_interpolator - implementation
 */

#include "SGAL/Rotation_interpolator.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Rotation_interpolator::s_tag = "OrientationInterpolator";
Container_proto* Rotation_interpolator::s_prototype(NULL);

REGISTER_TO_FACTORY(Rotation_interpolator, "Rotation_interpolator");

/*! Constructor
 * \param interpolate_flag (in) initialization value for the interpolate flag
 * field
 */
Rotation_interpolator::Rotation_interpolator(Boolean flag, Boolean proto) :
  Interpolator(proto),
  m_interpolate_flag(flag),
  m_last_location(0)
{}

/*! Destructor - Deletes m_keys and m_values arrays */
Rotation_interpolator::~Rotation_interpolator()
{
  m_values.clear();
}

/*! \brief initializes the node prototype */
void Rotation_interpolator::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Interpolator::get_prototype());

  // value
  Rotation_handle_function value_func =
    static_cast<Rotation_handle_function>
    (&Rotation_interpolator::value_handle);
  s_prototype->add_field_info(new SF_rotation(VALUE, "value_changed",
                                              RULE_EXPOSED_FIELD,
                                              value_func));
}

/*! \brief deletes the node prototype */
void Rotation_interpolator::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype */
Container_proto* Rotation_interpolator::get_prototype()
{
  if (!s_prototype) Rotation_interpolator::init_prototype();
  return s_prototype;
}

/*! The interpolation execution function.
 * Activated through the cascade of the m_fraction field.
 * The function calculates m_value, updates it and activate cascade on it.
 * \param pointer (in) to the cascaded field's field info - not used for now
 */
void Rotation_interpolator::execute(Field_info*)
{
  Field* value = get_field(VALUE);
  // if there is no connection to the value field there is no need to execute
  // the interpolation
  if (value == NULL) return;

  // if there are no array set the value to zero and return
  if ((m_keys.size() == 0) || m_values.size() == 0) {
    m_value = Rotation(0.0, 0.0, 1.0, 0.0);
    value->cascade();
    return;
  }

  // If there is only one value - set it and return
  if (m_keys.size() == 1) {
    m_value = m_values[0];
    value->cascade();
    return;
  }

  // If the fraction is bigger than the last key - set the value to the last
  // one
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
    Rotation location_value = m_values[location];
    Rotation next_location_value = m_values[location+1];
    Float key_delta = m_keys[location+1] - m_keys[location];
    Float fration_delta = m_fraction - m_keys[location];

    // check if the angle between the vectors is greater than PI/2 inverse
    // the first vector
    if (next_location_value.get_axis().dot(location_value.get_axis()) < 0) {
      location_value[0] = -location_value[0];
      location_value[1] = -location_value[1];
      location_value[2] = -location_value[2];
      location_value[3] = SGAL_TWO_PI - location_value[3];
    }

    // make sure the angle between the two angles is less than PI
    while (abs(next_location_value[3]-location_value[3]) > SGAL_PI) {
      if (location_value[3]<next_location_value[3]) {
        location_value[3] += SGAL_TWO_PI;
      } else {
        next_location_value[3] += SGAL_TWO_PI;
      }
    }
    m_value.slerp(fration_delta / key_delta, location_value,
                  next_location_value);
  } else
    m_value = m_values[location];

  // Cascade the updated value
  value->cascade();
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Rotation_interpolator::set_attributes(Element * elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;

  Interpolator::set_attributes(elem);

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "keyValue") {
      Uint num_values = get_num_tokens(value);
      Uint num_rotations = num_values / 4;
      m_values.resize(num_rotations);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_rotations; ++i) svalue >> m_values[i];
      elem->mark_delete(ai);
      continue;
    }
    if ( name == "interpolateFlag" ) {
      m_interpolate_flag = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*! Get a list of atytributes in this object. This method is called only
 * from the Builder side.
 *
 * \return a list of attributes
 */
Attribute_list Rotation_interpolator::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  attribs = Interpolator::get_attributes();

  // Set the values array
  attrib.first = "keyValue";
  String key_value("");
  for (i = 0; i < m_num_keys;i++)
    for (int j = 0; j < 4; j++) {
      Rotation & curValue = m_values[i];
      sprintf(buf, "%g ", curValue[j]);
      key_value = key_value + std::string(buf);
    }
  attrib.second = key_value;
  attribs.push_back(attrib);

  // Set the interpolate flag
  attrib.first = "interpolateFlag";
  attrib.second = (m_interpolate_flag) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE
