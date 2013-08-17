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
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Item_primitive.hpp"

SGAL_BEGIN_NAMESPACE

/*! Return the value of a given parameter. Empty string if the parameter
 * does not exist.
 * @param paramName (in) the parameter's id
 * @return the value in string format
 */
std::string Item_primitive::get_value(unsigned int param_id) const
{
  if (param_id >= m_attributes.size()) {
    return std::string("");
  }
  return m_attributes[param_id];
}

/*! Get the integer value of a given paramter. If the parameter
 * is invalid returns 0.
 * @param (in) the parameter's id.
 * param (out) the integer value.
 */
void Item_primitive::get_value(unsigned int param_id, int & value) const
{
  if (m_attributes.empty() || (param_id >= m_attributes.size())) {
    SGAL_assertion(0);
    value = 0;
  } else {
    value = atoi(m_attributes[param_id].c_str());
  }
}

/*! Get the float value of a given paramter. If the parameter
 * is invalid returns 0.
 * @param (in) the parameter's id.
 * param (out) the float value.
 */
void Item_primitive::get_value(unsigned int param_id, float & value) const 
{
  if (param_id >= m_attributes.size()) {
    SGAL_assertion(0);
    value = 0;
  }
  value = atoff(m_attributes[param_id].c_str());
}

/*! Get the boolean value of a given paramter. If the parameter
 * is invalid returns 0.
 * @param (in) the parameter's id.
 * param (out) the float value.
 */
void Item_primitive::get_value(unsigned int param_id, bool & value) const 
{
  if (param_id >= m_attributes.size()) {
    SGAL_assertion(0);
    value = 0;
  }
  value = (m_attributes[param_id] == "TRUE" ? true : false);
}

/*! Set a value of a parameter.
 * @param paramName (in) the id of the parameter
 * @param value (in) the value of the parameter in string format
 */
void Item_primitive::set_value(unsigned int param_id, const std::string & value)
{
  if (param_id < m_attributes.size()) {
    m_attributes[param_id] = value;
  } else {
    SGAL_assertion(0);
  }
}

/*! Set a value of a parameter.
 * @param paramName (in) the id of the parameter.
 * @param value (in) the value of an integer parameter.
 */
void Item_primitive::set_value(unsigned int param_id, int value)
{
  char buf[32];
  sprintf(buf, "%d", value);
  if (param_id < m_attributes.size()) {
    m_attributes[param_id] = buf;
  } else {
    SGAL_assertion(0);
  }
}

/*! Set a value of a parameter.
 * @param paramName (in) the id of the parameter.
 * @param value (in) the value of an float parameter.
 */
void Item_primitive::set_value(unsigned int param_id, float value)
{
  char buf[32];
  sprintf(buf, "%g", value);
  if (param_id < m_attributes.size()) {
    m_attributes[param_id] = buf;
  } else {
    SGAL_assertion(0);
  }
}

/*! Set a boolean value of a parameter.
 * @param paramName (in) the id of the parameter.
 * @param value (in) the value of a boolean parameter.
 */
void Item_primitive::set_value(unsigned int param_id, bool value)
{
  if (param_id < m_attributes.size()) {
    m_attributes[param_id] = (value ? "TRUE" : "FALSE");
  } else {
    SGAL_assertion(0);
  }
}

SGAL_END_NAMESPACE
