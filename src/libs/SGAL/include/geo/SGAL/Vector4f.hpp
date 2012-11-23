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
// $Source$
// $Revision: 5690 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR4F_HPP
#define SGAL_VECTOR4F_HPP

/*! \file
 *
 */

#include <string>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Matrix4f;

class SGAL_CLASSDEF Vector4f {
private:
  float m_vector[4];

public:
  Vector4f() {};
  Vector4f(float a, float b, float c, float d);
  Vector4f(const Vector4f &v);
  Vector4f(const Vector3f &v);
  Vector4f(const std::string & text);

  void set(float a, float b, float c, float d);
  void get(float * a, float * b, float * c, float * d)  const;
  void set(int i, float f);
  float get(int i) const;
  void set(const Vector4f & v);
  void get(Vector4f & v) const;
  std::string get_text(); 

  bool equal(const Vector4f & v) const;
  float length() const;
  void xform(const Vector4f & v, const Matrix4f & m);
  float dot(const Vector4f &  v) const;
  void negate();    
  void scale(float s, const Vector4f & v);
    
  bool almost_equal(const Vector4f & v, float tol) const;
  void negate(const Vector4f & v);
  void add(const Vector4f & v1, const Vector4f & v2);
  void sub(const Vector4f & v1, const Vector4f & v2);
  void add_scaled(const Vector4f & v1, float s, const Vector4f & v2);
  void combine(float a, const Vector4f & v1, float b, const Vector4f & v2);
  float sqr_distance(const Vector4f & v) const;
  float normalize();
  float distance(const Vector4f & v) const;

  Vector4f & operator=(const Vector4f & v);
  Vector4f & operator=(float v);
  float & operator[](int i);
  float operator[](int i) const;
  bool operator==(const Vector4f & v) const;
  bool operator!=(const Vector4f & v) const;
};

inline Vector4f::Vector4f(float a, float b, float c, float d)
{
  set(a, b, c, d);
}

inline Vector4f::Vector4f(const Vector4f &v)
{
  set(v);
}

inline Vector4f::Vector4f(const Vector3f &v)
{
  set(v[0], v[1], v[2], 0);
}

inline float & Vector4f::operator[](int i)
{
  return m_vector[i];
}

inline float Vector4f::operator[](int i) const
{
  return m_vector[i];
}

inline void Vector4f::set(float a, float b, float c, float d)
{
  m_vector[0] = a; m_vector[1] = b; m_vector[2] = c; m_vector[3] = d;
}

inline void Vector4f::get(float * a, float * b, float * c, float * d) const
{
  *a = m_vector[0]; *b = m_vector[1]; *c = m_vector[2]; *d = m_vector[3];
}

inline void Vector4f::set(int i, float val)
{
  m_vector[i] = val;
}

inline float Vector4f::get(int i) const
{
  return m_vector[i];
}

inline void Vector4f::set(const Vector4f & v)
{
  m_vector[0] = v.m_vector[0]; m_vector[1] = v.m_vector[1];
  m_vector[2] = v.m_vector[2]; m_vector[3] = v.m_vector[3];
}

inline void Vector4f::get(Vector4f & v) const
{
  v.m_vector[0] = m_vector[0]; v.m_vector[1] = m_vector[1];
  v.m_vector[2] = m_vector[2]; v.m_vector[3] = m_vector[3];
}

inline Vector4f & Vector4f::operator=(const Vector4f & v)
{
  if (this != &v)
    set(v);
  return *this;
}

inline Vector4f & Vector4f::operator=(float v)
{
  m_vector[0] = v; m_vector[1] = v; m_vector[2] = v; m_vector[3] = v;
  return *this;
}

inline bool Vector4f::operator==(const Vector4f & v) const
{
  return equal(v);
}

inline bool Vector4f::operator!=(const Vector4f & v) const
{
  return !equal(v);
}

inline bool Vector4f::equal(const Vector4f & v) const
{
  return(m_vector[0] == v[0] && m_vector[1] == v[1] &&
         m_vector[2] == v[2] && m_vector[3] == v[3]);
}

inline float Vector4f::length() const
{
  return squarerootf(dot(*this));
}

inline float Vector4f::dot(const Vector4f & v) const
{
  return(m_vector[0] * v[0] + m_vector[1] * v[1] +
         m_vector[2] * v[2] + m_vector[3] * v[3]);
}
inline void Vector4f::negate()
{
  m_vector[0] = -m_vector[0];
  m_vector[1] = -m_vector[1];
  m_vector[2] = -m_vector[2];
  m_vector[3] = -m_vector[3];
}

inline void Vector4f::scale(float s, const Vector4f & v)
{
  m_vector[0] = s * v[0];
  m_vector[1] = s * v[1];
  m_vector[2] = s * v[2];
  m_vector[3] = s * v[3];
}

inline Vector4f::Vector4f(const std::string & text)
{
  std::istringstream tmp(text, std::istringstream::in);
  tmp >> m_vector[0];
  tmp >> m_vector[1];
  tmp >> m_vector[2];
  tmp >> m_vector[3];
}

inline void Vector4f::negate(const Vector4f & v)
{
  m_vector[0] = -v[0];
  m_vector[1] = -v[1];
  m_vector[2] = -v[2];
  m_vector[3] = -v[3];
}

inline void Vector4f::add(const Vector4f & v1, const Vector4f & v2)
{
  m_vector[0] = v1[0] + v2[0];
  m_vector[1] = v1[1] + v2[1];
  m_vector[2] = v1[2] + v2[2];
  m_vector[3] = v1[3] + v2[3];
}

inline void Vector4f::sub(const Vector4f & v1, const Vector4f & v2)
{
  m_vector[0] = v1[0] - v2[0];
  m_vector[1] = v1[1] - v2[1];
  m_vector[2] = v1[2] - v2[2];
  m_vector[3] = v1[3] - v2[3];
}

inline void Vector4f::add_scaled(const Vector4f & v1, float s,
                                 const Vector4f & v2)
{
  m_vector[0] = v1[0] + s * v2[0];
  m_vector[1] = v1[1] + s * v2[1];
  m_vector[2] = v1[2] + s * v2[2];
  m_vector[3] = v1[3] + s * v2[3];
}

inline void Vector4f::combine(float a, const Vector4f & v1, float b,
                              const Vector4f & v2)
{
  m_vector[0] = a * v1[0] + b * v2[0];
  m_vector[1] = a * v1[1] + b * v2[1];
  m_vector[2] = a * v1[2] + b * v2[2];
  m_vector[3] = a * v1[3] + b * v2[3];
}

inline float Vector4f::sqr_distance(const Vector4f & v) const
{
  float d0 = m_vector[0] - v.m_vector[0];
  float d1 = m_vector[1] - v.m_vector[1];
  float d2 = m_vector[2] - v.m_vector[2];
  float d3 = m_vector[3] - v.m_vector[3];
  return d0 * d0 + d1 * d1 + d2 * d2 + d3 * d3;
}

/*! Sends a vector to an output stream */
inline std::ostream & operator<<(std::ostream & os, const Vector4f & vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3];
  return os;
}  

SGAL_END_NAMESPACE

#endif
