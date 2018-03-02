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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

/*!
 * Coordinate_interpolator - implementation
 */

#include "SGAL/Coordinate_interpolator.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/io_vector3f.hpp"

SGAL_BEGIN_NAMESPACE

std::string Coordinate_interpolator::s_tag = "CoordinateInterpolator";
Container_proto* Coordinate_interpolator::s_prototype(nullptr);

REGISTER_TO_FACTORY(Coordinate_interpolator, "Coordinate_interpolator");

//! \brief constructor.
Coordinate_interpolator::Coordinate_interpolator(Boolean proto) :
  Interpolator(proto),
  m_last_location(0)
{}

//! \brief destructor.
Coordinate_interpolator::~Coordinate_interpolator()
{
  m_keys.clear();
  m_values.clear();
  m_value.clear();
}

//! \brief initializes the container prototype.
void Coordinate_interpolator::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Interpolator::get_prototype());

  // value
  Vector3f_array_handle_function value_func =
    static_cast<Vector3f_array_handle_function>
    (&Coordinate_interpolator::value_handle);
  s_prototype->add_field_info(new MF_vector3f(VALUE,
                                              "value_changed",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              value_func));
}

//! \brief deletes the container prototype.
void Coordinate_interpolator::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto * Coordinate_interpolator::get_prototype()
{
  if (!s_prototype) Coordinate_interpolator::init_prototype();
  return s_prototype;
}

/*! The interpolation execution function.
 * Activated through the cascade of the m_fraction field.
 * The function calculates m_value, updates it and activate cascade on it
 * @param pointer (in) to the cascaded field's field info - not used for now
 */
void Coordinate_interpolator::execute(const Field_info* /* field_info */)
{
  Field* value = get_field(VALUE);

  // if there is no connection to the value field there is no need to execute
  // the interpolation
  if (value == nullptr) return;

  // if there are no array set the value to zero and return
  if ((m_keys.size() == 0) || (m_values.size() == 0)) {
    m_value = Vector3f_array(1);
    value->cascade();
    return;
  }

  // If there is only one key/value pair, set it and return
  if (m_keys.size() == 1) {
    m_value = get_value(0);
    value->cascade();
    return;
  }

  // If the fraction is larger than the last key, set the value to the last one
  if (m_fraction >= m_keys[m_keys.size() - 1]) {
    m_value = get_value(m_keys.size() - 1);
    value->cascade();
    return;
  }

  // If the fraction is smaller than the first key - set the value to the
  // first one
  if (m_fraction <= m_keys[0]) {
    m_value = get_value(0);
    value->cascade();
    return;
  }

  // Start looking for the fraction location in the keys array
  // from the last fraction location place
  Uint location = m_last_location;

  // Loop up the array to find the closest key which is bigger than fraction
  while (((location + 1) < m_keys.size()) && (m_fraction > m_keys[location+1]))
    location++;

  // Loop down the array to find the closest key which is smaller than fraction
  while ((location > 0) && (m_fraction < m_keys[location])) --location;

  // Set the last location to the current one for next execution
  m_last_location = location;

  // Set m_value by interpolating between two values
  Uint size_of_array = m_values.size() / m_keys.size();
  std::vector<Vector3f>::iterator location_value = m_values.begin();
  std::advance(location_value, location * size_of_array);
  std::vector<Vector3f>::iterator next_location_value = m_values.begin();
  std::advance(next_location_value, (location + 1) * size_of_array);

  Float key_delta = m_keys[location + 1] - m_keys[location];
  Float fration_delta = m_fraction - m_keys[location];

  m_value = Vector3f_array(size_of_array);
  for (Uint j = 0; j < size_of_array; ++j) {
    for (Uint i = 0; i < 3; ++i) {
      Float delta = (*next_location_value)[i] - (*location_value)[i];
      m_value[j][i] =
        (*location_value)[i] + (fration_delta * delta) / key_delta;
    }
    ++next_location_value;
    ++location_value;
  }

  // Cascade the updated value
  value->cascade();
}

//! \brief obtains a copy of the value in a specific location.
Vector3f_array Coordinate_interpolator::get_value(Uint location)
{
  auto size_of_array = m_values.size() / m_keys.size();
  auto ret = Vector3f_array(size_of_array);
  auto start = location * size_of_array;
  for (size_t i = 0; i < size_of_array; ++i) ret[i] = m_values[start + i];
  return ret;
}


/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Coordinate_interpolator::set_attributes(Element* elem)
{
  Interpolator::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
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


  // Set the interpolate flag
  attrib.first = "interpolateFlag";
  attrib.second = (m_interpolate_flag) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE
