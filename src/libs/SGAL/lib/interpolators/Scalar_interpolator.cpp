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
 * Scalar_interpolator - implementation
 */

#include "SGAL/Scalar_interpolator.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Scalar_interpolator::s_tag = "ScalarInterpolator";
Container_proto* Scalar_interpolator::s_prototype(nullptr);

REGISTER_TO_FACTORY(Scalar_interpolator, "Scalar_interpolator");

/*! Constructor
 * \param interpolate_flag (in) initialization value for the interpolate flag
 * field
 */
Scalar_interpolator::Scalar_interpolator(Boolean interpolate_flag,
                                         Boolean proto) :
  Interpolator(interpolate_flag, proto),
  m_last_location(0)
{}

//! \brief destructor.
Scalar_interpolator::~Scalar_interpolator() {}

//! initializes the node prototype.
void Scalar_interpolator::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Interpolator::get_prototype());

  // value
  Float_handle_function value_func =
    static_cast<Float_handle_function>(&Scalar_interpolator::value_handle);
  s_prototype->add_field_info(new SF_float(VALUE,
                                           "value_changed",
                                           RULE_EXPOSED_FIELD,
                                           value_func));
}

//!
void Scalar_interpolator::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//!
Container_proto* Scalar_interpolator::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! The interpolation execution function.
 * Activated through the cascade of the m_fraction field.
 * The function calculates m_value, updates it and activate cascade on it
 * \param pointer (in) to the cascaded field's field info - not used for now
 */
void Scalar_interpolator::execute(const Field_info* /* field_info */)
{
  Field* value = get_field(VALUE);
  // if there is no connection to the value field there is no need to execute
  // the interpolation
  if (value == nullptr) return;

  auto no_keys = get_keys().size();

  // if there are no array set the value to zero and return
  if ((m_keys.size() == 0) || (m_values.size() == 0) || (no_keys == 0)) {
    m_value = 0.0;
    value->cascade();
    return;
  }

  // If there is only one value - set it and return
  if (no_keys == 1) {
    m_value = m_values[0];
    value->cascade();
    return;
  }

  // If the fraction is bigger than the last key - set the value to the last one
  if (m_fraction >= m_keys[no_keys-1]) {
    m_value = m_values[no_keys-1];
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
  auto location = m_last_location;

  // Loop up the array to find the closest key which is bigger than fraction
  while (location+1 < no_keys && m_fraction>m_keys[location+1]) location++;

  // Loop down the array to find the closest key which is smaller than fraction
  while (location>0 && m_fraction<m_keys[location]) location--;

  // Set the last location to the current one for next execution
  m_last_location = location;

  // Set m_value by interpolating between two values or
  // just return the first one (if interpolate flag is false)
  if (m_interpolate_flag) {
    Float delta = m_values[location+1] - m_values[location];
    Float key_delta = m_keys[location+1] - m_keys[location];
    Float fration_delta = m_fraction - m_keys[location];
    m_value = m_values[location] + (fration_delta/key_delta)*delta;
  }
  else
    m_value = m_values[location];

  // Cascade the updated value
  value->cascade();
};

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Scalar_interpolator::set_attributes(Element* elem)
{
  Interpolator::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "keyValue") {
      std::vector<Float> values;
      std::istringstream svalue(value, std::istringstream::in);

      Uint num_values = get_num_tokens(value);
      m_values.resize(num_values);

      for (Uint i = 0; i < num_values; ++i) svalue >> m_values[i];
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/**
 * Get a list of attributes in this object. This method is called only
 * from the Builder side.
 *
 * @return a list of attributes
 */
Attribute_list Scalar_interpolator::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  attribs = Interpolator::get_attributes();

  // Set the values array
  attrib.first = "keyValue";
  String key_value("");
  for (i = 0; i < m_keys.size(); i++) {
    sprintf(buf, "%g ", m_values[i]);
    key_value = key_value + String(buf);
  }
  attrib.second = key_value;
  attribs.push_back(attrib);


  // Set the interpolate flag
  attrib.first = "interpolate_flag";
  attrib.second = (m_interpolate_flag) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
}
#endif

SGAL_END_NAMESPACE
