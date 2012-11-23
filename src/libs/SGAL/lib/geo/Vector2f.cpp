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
// $Revision: 10791 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

/*!
 */
Vector2f::Vector2f()
{
  m_vector[0] = m_vector[1] = 0.0f;
}

/*!
 */
Vector2f::Vector2f(Float a, Float b)
{
  set(a, b);
}

/*!
 */
Vector2f::Vector2f(const Vector2f & v)
{
  set(v[0], v[1]);
}

/*!
 */
Float & Vector2f::operator[](int i) 
{
  return m_vector[i];
}

/*!
 */
Float Vector2f::operator[](int i) const 
{
  return m_vector[i];
}

/*!
 */
void Vector2f::set(Float a, Float b) 
{
  m_vector[0] = a; m_vector[1] = b; 
}

/*!
 */
void Vector2f::get(Float *a, Float *b)  const 
{
  *a = m_vector[0]; *b = m_vector[1];
}

/*!
 */
Float Vector2f::get(int i) const 
{
  if (i == 0 || i == 1) return m_vector[i];
  else return 0.0f;   // !!!!!!!! SHould WARN !!!!!!!!!!
}

/*!
 */
void Vector2f::set(const Vector2f &v) 
{
  m_vector[0] = v.m_vector[0]; m_vector[1] = v.m_vector[1];
}

/*!
 */
void Vector2f::get(Vector2f &v) const
{
  v.m_vector[0] = m_vector[0];  v.m_vector[1] = m_vector[1];
}

/*!
 */
void Vector2f::add(const Vector2f & _v) 
{ 
  m_vector[0] += _v[0]; 
  m_vector[1] += _v[1]; 
}

/*!
 */
void Vector2f::add(const Vector2sh & _v) 
{ 
  m_vector[0] += _v[0]; 
  m_vector[1] += _v[1]; 
}

/*!
 */
void Vector2f::scale(Float _s) 
{ 
  m_vector[0] *= _s ;
  m_vector[1] *= _s ;
}

/*!
 */
void Vector2f::scale(Float _s, const Vector2f & _v) 
{ 
  m_vector[0] = _s * _v[0];
  m_vector[1] = _s * _v[1];
}

/*!
 */
bool Vector2f::equal(const Vector2f &  _v) const 
{ 
  return (m_vector[0] == _v[0] && m_vector[1] == _v[1]);
}

/*!
 */
bool Vector2f::operator==(const Vector2f & v) const
{
  return equal(v);
}

/*!
 */
bool Vector2f::operator!=(const Vector2f & v) const 
{
  return !equal(v);
}

/*!
 */
Vector2f & Vector2f::operator=(const Vector2f & v) 
{
  set(v); return *this;
}

/*!
 */
Vector2f & Vector2f::operator=(Float v) 
{
  m_vector[0] = v; m_vector[1] = v;
  return *this;
}

/*!
 */
void Vector2f::round()
{
  m_vector[0] = (float) ((int)m_vector[0]); 
  m_vector[1] = (float) ((int)m_vector[1]); 
  
}

/*!
 */
void Vector2f::div(float c, const Vector2f & v)
{
  m_vector[0] = c / v[0];
  m_vector[1] = c / v[1];
}

/*!
 */
void Vector2f::sub(const Vector2f & v) 
{ 
  m_vector[0] -= v[0]; 
  m_vector[1] -= v[1]; 
}

/*!
 */
void Vector2f::negate(const Vector2f & v) 
{ 
  m_vector[0] = -v[0];
  m_vector[1] = -v[1]; 
}

/*!
 */
Float Vector2f::dot(const Vector2f & v) const 
{
  return (m_vector[0] * v[0] + m_vector[1] * v[1]);
}

/*!
 */
void Vector2f::add(const Vector2f & v1, const Vector2f & v2) 
{ 
  m_vector[0] = v1[0] + v2[0]; 
  m_vector[1] = v1[1] + v2[1]; 
}

/*!
 */
void Vector2f::sub(const Vector2f & v1, const Vector2f & v2) 
{ 
  m_vector[0] = v1[0] - v2[0]; 
  m_vector[1] = v1[1] - v2[1]; 
}

void Vector2f::add_scaled(const Vector2f & v1, Float s,
                          const Vector2f & v2) 
{ 
  m_vector[0] = v1[0] + s * v2[0]; 
  m_vector[1] = v1[1] + s * v2[1]; 
}

/*!
 */
void Vector2f::combine(Float a, const Vector2f & v1,
                       Float b, const Vector2f & v2) 
{ 
  m_vector[0] = a * v1[0] + b * v2[0]; 
  m_vector[1] = a * v1[1] + b * v2[1]; 
}

/*!
 */
Float Vector2f::sqr_distance(const Vector2f & v) const 
{ 
  Float d0, d1;
  d0 = m_vector[0] - v.m_vector[0];
  d1 = m_vector[1] - v.m_vector[1];
  return d0 * d0 + d1 * d1;
}

/*!
 */
Float Vector2f::length() const
{
  return squarerootf(dot(*this));
}

float Vector2f::normalize()
{
  float my_length = length();
  if (my_length == 0.0f) return my_length;
  float length_reciprocal = 1.0f / my_length;
  m_vector[0] *= length_reciprocal;
  m_vector[1] *= length_reciprocal;
  return my_length;
}

SGAL_END_NAMESPACE
