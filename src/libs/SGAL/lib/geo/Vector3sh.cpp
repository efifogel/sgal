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

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3sh.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor from string.
Vector3sh::Vector3sh(const std::string& str)
{
  if (!str.empty()) {
    std::istringstream tmp(str, std::istringstream::in);
    tmp >> m_vector[0];
    tmp >> m_vector[1];
    tmp >> m_vector[2];
    return;
  }
  init();
}

//! \brief
void Vector3sh::negate(const Vector3sh& v)
{
  m_vector[0] = -v[0];
  m_vector[1] = -v[1];
  m_vector[2] = -v[2];
}

//! \brief
void Vector3sh::negate()
{
  m_vector[0] = -m_vector[0];
  m_vector[1] = -m_vector[1];
  m_vector[2] = -m_vector[2];
}

//! \brief
void Vector3sh::clamp(const Vector3sh& v,
                      const Vector3sh& min, const Vector3sh& max)
{
  m_vector[0] = (v[0] < min[0]) ? min[0] : (max[0] < v[0]) ? max[0] : v[0];
  m_vector[1] = (v[1] < min[1]) ? min[1] : (max[1] < v[1]) ? max[1] : v[1];
  m_vector[2] = (v[2] < min[2]) ? min[2] : (max[2] < v[2]) ? max[2] : v[2];
}

//! \brief exports.
std::ostream& operator<<(std::ostream& os, const Vector3sh& vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2];
  return os;
}

SGAL_END_NAMESPACE
