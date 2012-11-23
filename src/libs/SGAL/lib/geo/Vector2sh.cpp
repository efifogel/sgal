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

#include "SGAL/Vector2sh.hpp"

using namespace SGAL;

/*!
 */
short & Vector2sh::operator[](int i) 
{
  return m_vector[i];
}

/*!
 */
short Vector2sh::operator[](int i) const 
{
  return m_vector[i];
}

/*!
 */
void Vector2sh::set(short a, short b) 
{
  m_vector[0] = a; m_vector[1] = b; 
}

/*!
 */
void Vector2sh::get(short *a, short *b)  const 
{
  *a = m_vector[0]; *b = m_vector[1];
}

/*!
 */
short Vector2sh::get(int i) const 
{
  if (i == 0 || i == 1) return m_vector[i];
  //! \todo should warn!
  else return 0;
}

/*!
 */
void Vector2sh::set(const Vector2sh & v) 
{
  m_vector[0] = v.m_vector[0]; m_vector[1] = v.m_vector[1];
}

/*!
 */
void Vector2sh::get(Vector2sh & v) const
{
  v.m_vector[0] = m_vector[0];  v.m_vector[1] = m_vector[1];
}

/*!
 */
void Vector2sh::add(const Vector2sh & v1, const Vector2sh & v2) 
{ 
  m_vector[0] = v1[0] + v2[0]; 
  m_vector[1] = v1[1] + v2[1]; 
}

/*!
 */
Vector2sh::Vector2sh()
{
  m_vector[0] = m_vector[1] = 0;
}

/*!
 */
Vector2sh::Vector2sh(short a, short b)
{
  set(a, b);
}

/*!
 */
Vector2sh::Vector2sh(const Vector2sh & v)
{
  set(v[0], v[1]);
}

/*!
 */
void Vector2sh::sub(const Vector2sh & v1, const Vector2sh & v2) 
{ 
  m_vector[0] = v1[0] - v2[0]; 
  m_vector[1] = v1[1] - v2[1]; 
}

/*!
 */
void Vector2sh::sub(const Vector2sh & v) 
{ 
  m_vector[0] -= v[0]; 
  m_vector[1] -= v[1]; 
}

/*!
 */
void Vector2sh::add(const Vector2sh & v) 
{ 
  m_vector[0] += v[0]; 
  m_vector[1] += v[1]; 
}

/*!
 */
bool Vector2sh::equal(const Vector2sh & v) const 
{ 
  return (m_vector[0] == v[0] && m_vector[1] == v[1]);
}

/*!
 */
void Vector2sh::negate(const Vector2sh & v) 
{ 
  m_vector[0] = -v[0];
  m_vector[1] = -v[1]; 
}

/*!
 */
short Vector2sh::dot(const Vector2sh & v) const 
{
  return (m_vector[0] * v[0] + m_vector[1] * v[1]);
}

/*!
 */
short Vector2sh::sqr_distance(const Vector2sh & v) const 
{ 
  short d0, d1;
  d0 = m_vector[0] - v.m_vector[0];
  d1 = m_vector[1] - v.m_vector[1];
  return d0 * d0 + d1 * d1;
}

/*!
 */
Vector2sh & Vector2sh::operator=(const Vector2sh & v) 
{
  set(v); return *this;
}

/*!
 */
Vector2sh & Vector2sh::operator=(short v) 
{
  m_vector[0] = v; m_vector[1] = v;
  return *this;
}

/*!
 */
bool Vector2sh::operator==(const Vector2sh & v) const
{
  return equal(v);
}

/*!
 */
bool Vector2sh::operator!=(const Vector2sh & v) const 
{
  return !equal(v);
}

/*!
 */
inline void Vector2sh::mul(const Vector2sh & v1, const Vector2sh & v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
}
