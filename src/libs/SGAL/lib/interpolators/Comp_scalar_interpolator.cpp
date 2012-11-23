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
// $Source: $
// $Revision: 4971 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Implements scalar interpolator with asynchronic loading of compressed data  
 */

#include "SGAL/Comp_scalar_interpolator.hpp"
#include "SGAL/Trace.hpp"
 
SGAL_BEGIN_NAMESPACE

std::string Comp_scalar_interpolator::s_tag =
"sgalCompressedScalarInterpolator";

// Default values:
const float Comp_scalar_interpolator::m_default_key_bin = (float)1e-4;
const float Comp_scalar_interpolator::m_value_bin_factor = (float)5e-4;      

REGISTER_TO_FACTORY(Comp_scalar_interpolator, "Comp_scalar_interpolator");

/*! Constructor */
Comp_scalar_interpolator::Comp_scalar_interpolator(Boolean flag, Boolean proto) :
  Scalar_interpolator(flag, proto),
  m_pInterpolator_data(0),
  m_pExecution_coordinator(0)
{
}

/*! Destructor */
Comp_scalar_interpolator::~Comp_scalar_interpolator()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Comp_scalar_interpolator ...");
  if (m_pInterpolator_data) delete m_pInterpolator_data;
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

// Overiding base interpolator get_attributes function
// Get all attributes except the key and key_value
Attribute_list Comp_scalar_interpolator::get_attributes()
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

Item_primitive * Comp_scalar_interpolator::get_data()
{
  m_pInterpolator_data = new Interpolator_data(1);  // Scalar
  assert(m_pInterpolator_data);
  return m_pInterpolator_data;
}

void Comp_scalar_interpolator::update(Boolean is_last)
{
  if (m_pInterpolator_data == 0) {  
    assert(0);
    return;
  }

  m_no_keys = m_pInterpolator_data->get_size();  
  assert((m_no_keys > 0) && (m_no_keys < 10e6));

  float *pKey = m_pInterpolator_data->get_keyData();
  assert(pKey);
  m_keys = new float [m_no_keys];
  assert(m_keys);
  ::memcpy((void *)m_keys, (const void *)pKey, m_no_keys * sizeof(Float));

  float **Values = m_pInterpolator_data->get_value_data();
  assert(Values);
  m_values = new float [m_no_keys];
  assert(m_values);
  ::memcpy((void *)m_values,(const void *)Values[0],m_no_keys * sizeof(Float));

  // Update execution coordinator
  if (m_pExecutionCoordinator) {
    m_pExecutionCoordinator->UpdateCompInterpolator();
  }
}

// Interpolator_int methods - used only by the convertor
// -----------------------
Interpolator_data * Comp_scalar_interpolator::get_interpolator_data()
{
  if (m_pInterpolator_data != 0) {
    return m_pInterpolator_data; 
  }

  m_pInterpolator_data = new Interpolator_data(1); // Scalar
  
  // Allocate data memory
  assert((m_no_keys > 0) && (m_no_keys < 10e6));
  m_pInterpolator_data->Allocate(m_no_keys);

  // Set the data
  float * pKey = m_pInterpolator_data->get_keyData();
  assert(pKey);
  ::memcpy((void *)pKey,(const void *)m_keys,m_no_keys * sizeof(Float));

  float ** Values = m_pInterpolator_data->get_value_data();
  assert(Values);
  ::memcpy((void *)Values[0], (const void *)m_values,
           m_no_keys * sizeof(Float));

  // Set the keys compression attributes
  Key_attribute * pKeyAttr = m_pInterpolator_data->get_key_attr();
  assert(pKeyAttr);
  pKeyAttr->m_bin = get_key_bin();

  // Set the values compression attributes
  Key_attribute * pValAttr = m_pInterpolator_data->get_value_attr(0);
  assert(pValAttr);
  pValAttr->m_bin =
    get_value_bin_factor() * get_valueRange(Values[0], m_no_keys);
  
  return m_pInterpolator_data;
}

SGAL_END_NAMESPACE
