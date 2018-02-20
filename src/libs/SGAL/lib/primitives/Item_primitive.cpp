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

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Item_primitive.hpp"

SGAL_BEGIN_NAMESPACE

/*! Return the value of a given parameter. Empty string if the parameter
 * does not exist.
 * @param paramName (in) the parameter's id
 * @return the value in string format
 */
std::string Item_primitive::get_value(Uint param_id) const
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
void Item_primitive::get_value(Uint param_id, int & value) const
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
void Item_primitive::get_value(Uint param_id, float & value) const
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
void Item_primitive::get_value(Uint param_id, bool & value) const
{
  if (param_id >= m_attributes.size()) {
    SGAL_assertion(0);
    value = 0;
  }
  value = (m_attributes[param_id] == "TRUE" ? true : false);
}

//! \brief sets a value of a parameter.
void Item_primitive::set_value(Uint param_id, const std::string & value)
{
  SGAL_assertion(param_id < m_attributes.size());
  m_attributes[param_id] = value;
}

//! \brief sets a value of a parameter.
void Item_primitive::set_value(Uint param_id, int value)
{
  SGAL_assertion(param_id < m_attributes.size());
  m_attributes[param_id] = boost::lexical_cast<std::string>(value);
}

//! \brief sets a value of a parameter.
void Item_primitive::set_value(Uint param_id, float value)
{
  SGAL_assertion(param_id < m_attributes.size());
  m_attributes[param_id] = boost::lexical_cast<std::string>(value);
}

//! \brief sets a boolean value of a parameter.
void Item_primitive::set_value(Uint param_id, bool value)
{
  SGAL_assertion(param_id < m_attributes.size());
  m_attributes[param_id] = (value ? "TRUE" : "FALSE");
}

SGAL_END_NAMESPACE
