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
// Author(s): Efi Fogel         <efifogel@gmail.com>

/*! Implements Vector3f interpolator with asynchronic loading of compressed
 * data
 */

#include "SGAL/Comp_vector3f_interpolator.hpp"
#include "SGAL/Tracer.hpp"

SGAL_BEGIN_NAMESPACE

std::string Comp_vector3f_interpolator::s_tag =
  "efiCompressedVector3fInterpolator";

// Default values:
const float Comp_vector3f_interpolator::m_default_key_bin = (float)1e-4;
const float Comp_vector3f_interpolator::m_value_bin_factor = (float)2e-4;

REGISTER_TO_FACTORY(Comp_vector3f_interpolator, "Comp_vector3f_interpolator");

/*! Constructor */
Comp_vector3f_interpolator::Comp_vector3f_interpolator(Boolean flag, Boolean proto) :
  Vector3f_interpolator(flag, proto),
  m_pInterpolator_data(0),
  m_pExecutionCoordinator(0)
{
  m_tag = "sgalCompressedVector3fInterpolator";
}

/*! Destructor */
Comp_vector3f_interpolator::~Comp_vector3f_interpolator()
{
  TRACE_MSG(Tracer::DESTRUCTOR, "~Comp_vector3f_interpolator ...");
  if (m_pInterpolator_data) delete m_pInterpolator_data;
  TRACE_MSG(Tracer::DESTRUCTOR, " completed\n");
}

// Overiding base interpolator get_attributes function
// Get all attributes except the key and key_value
Attribute_list Comp_vector3f_interpolator::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  attribs = Node::get_attributes();

  // Set the interpolate flag
  attrib.first = "interpolate_flag";
  attrib.second = (m_interpolate_flag) ? TRUE_STR : FALSE_STR;
  attribs.push_back(attrib);

  return attribs;
}

// Load_int methods - used only by the player
// ---------------

Item_primitive * Comp_vector3f_interpolator::get_data()
{
  m_pInterpolator_data = new Interpolator_data(3);  // Scalar
  assert(m_pInterpolator_data);
  return m_pInterpolator_data;
}

void Comp_vector3f_interpolator::Update(Boolean is_last)
{
  if (m_pInterpolator_data == 0) {
    assert(0);
    return;
  }

  m_no_keys = m_pInterpolator_data->get_size();
  assert((m_no_keys > 0) && (m_no_keys < 10e6));

  float * pKey = m_pInterpolator_data->get_keyData();
  assert(pKey);
  m_keys = new float [m_no_keys];
  assert(m_keys);
  ::memcpy((void *)m_keys,(const void *)pKey,m_no_keys * sizeof(Float));

  float ** Values = m_pInterpolator_data->get_value_data();
  assert(Values);
  m_values = new Vector3f[m_no_keys];
  assert(m_values);
  Vector3f *pValueVec = 0;
  for (int valueIdx = 0; valueIdx < 3 ; valueIdx++) {
    pValueVec = m_values;
    float *pValues = Values[valueIdx];
    for (int i = 0; i < m_no_keys; i++, pValues++, pValueVec++) {
      (*pValueVec)[valueIdx] = *pValues;
    }
  }

  // Update execution coordinator
  if (m_pExecutionCoordinator) {
    m_pExecutionCoordinator->UpdateCompInterpolator();
  }
}

// Interpolator_int methods - used only by the convertor
// -----------------------
Interpolator_data * Comp_vector3f_interpolator::get_interpolator_data()
{
  if (m_pInterpolator_data != 0) {
    return m_pInterpolator_data;
  }

  m_pInterpolator_data = new Interpolator_data(3); // Scalar

  // Allocate data memory
  assert((m_no_keys > 0) && (m_no_keys < 10e6));
  m_pInterpolator_data->allocate(m_no_keys);

  // Set the data
  float *pKey = m_pInterpolator_data->get_keyData();
  assert(pKey);
  ::memcpy((void *)pKey,(const void *)m_keys,m_no_keys * sizeof(Float));

  float ** Values = m_pInterpolator_data->get_value_data();
  assert(Values);
  Vector3f * pValueVec = 0;
  for (int valueIdx = 0; valueIdx < 3 ; valueIdx++) {
    pValueVec = m_values;
    float * pValues = Values[valueIdx];
    for (int i = 0; i < m_no_keys; i++, pValues++, pValueVec++) {
      *pValues = (*pValueVec)[valueIdx];
    }
  }

  // Set the keys compression attributes
  Key_attribute * pKeyAttr = m_pInterpolator_data->get_key_attr();
  assert(pKeyAttr);
  pKeyAttr->m_bin = get_key_bin();

  // Set the values compression attributes
  Key_attribute *pValAttr = 0;
  for (valueIdx = 0; valueIdx < 3 ; valueIdx++) {
    pValAttr = m_pInterpolator_data->get_value_attr(valueIdx);
    assert(pValAttr);
    pValAttr->m_bin =
      get_value_bin_factor() * get_valueRange(Values[valueIdx], m_no_keys);
  }

  return m_pInterpolator_data;
}

SGAL_END_NAMESPACE
