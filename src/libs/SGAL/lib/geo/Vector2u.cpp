// Copyright (c) 2012 Israel.
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
// $Revision: 10791 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector2u.hpp"

SGAL_BEGIN_NAMESPACE

/*!
 */
Vector2u::Vector2u() { m_vector[0] = m_vector[1] = 0; }

/*!
 */
Vector2u::Vector2u(Uint a, Uint b) { set(a, b); }

/*!
 */
Vector2u::Vector2u(const Vector2u& v) { set(v[0], v[1]); }

/*!
 */
Uint& Vector2u::operator[](Uint i) { return m_vector[i]; }

/*!
 */
Uint Vector2u::operator[](Uint i) const { return m_vector[i]; }

/*!
 */
void Vector2u::set(Uint a, Uint b) { m_vector[0] = a; m_vector[1] = b; }

/*!
 */
void Vector2u::get(Uint *a, Uint *b)  const
{ *a = m_vector[0]; *b = m_vector[1]; }

/*!
 */
Uint Vector2u::get(int i) const 
{
  if ((i == 0) || (i == 1)) return m_vector[i];
  else return 0;   // !!!!!!!! SHould WARN !!!!!!!!!!
}

/*!
 */
void Vector2u::set(const Vector2u &v) 
{ m_vector[0] = v.m_vector[0]; m_vector[1] = v.m_vector[1]; }

/*!
 */
void Vector2u::get(Vector2u &v) const
{ v.m_vector[0] = m_vector[0];  v.m_vector[1] = m_vector[1]; }

/*!
 */
void Vector2u::add(const Vector2u& _v) 
{ 
  m_vector[0] += _v[0]; 
  m_vector[1] += _v[1]; 
}

/*!
 */
void Vector2u::add(const Vector2sh& _v) 
{ 
  m_vector[0] += _v[0]; 
  m_vector[1] += _v[1]; 
}

/*!
 */
bool Vector2u::equal(const Vector2u&  _v) const 
{ return ((m_vector[0] == _v[0]) && (m_vector[1] == _v[1])); }

/*!
 */
bool Vector2u::operator==(const Vector2u& v) const { return equal(v); }

/*!
 */
bool Vector2u::operator!=(const Vector2u& v) const { return !equal(v); }

/*!
 */
Vector2u& Vector2u::operator=(const Vector2u& v) { set(v); return *this; }

/*!
 */
Vector2u& Vector2u::operator=(Uint v) 
{
  m_vector[0] = v; m_vector[1] = v;
  return *this;
}

/*!
 */
void Vector2u::sub(const Vector2u& v) 
{ 
  m_vector[0] -= v[0]; 
  m_vector[1] -= v[1]; 
}

/*!
 */
Uint Vector2u::dot(const Vector2u& v) const 
{ return (m_vector[0] * v[0] + m_vector[1] * v[1]); }

/*!
 */
void Vector2u::add(const Vector2u& v1, const Vector2u& v2) 
{ 
  m_vector[0] = v1[0] + v2[0]; 
  m_vector[1] = v1[1] + v2[1]; 
}

/*!
 */
void Vector2u::sub(const Vector2u& v1, const Vector2u& v2) 
{ 
  m_vector[0] = v1[0] - v2[0]; 
  m_vector[1] = v1[1] - v2[1]; 
}

SGAL_END_NAMESPACE
