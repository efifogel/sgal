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
// $Revision: 6515 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR3F_HPP
#define SGAL_VECTOR3F_HPP

/*! \file
 */

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Vector3sh.hpp"

SGAL_BEGIN_NAMESPACE

class Matrix4f;

class SGAL_CLASSDEF Vector3f {
private:
  float m_vector[3];

public:
  Vector3f();
  Vector3f(float a, float b, float c);
  Vector3f(const Vector3f & v);
  Vector3f(const std::string & text);

  void set(float a, float b, float c);
  void get(float * a, float * b, float * c)  const;
  void set(int i, float f);
  float get(int i) const;
  void set(const Vector3f & v);
  void get(Vector3f & v) const;
  std::string get_text();

  float get_max_comp();
  bool equal(const Vector3f &  v) const;
  void sub(const Vector3f & v1, const Vector3f & v2);
  void sub(const Vector3sh & v1, const Vector3sh & v2);
  void sub(const Vector3f & v);
  void cross(const Vector3f &  v1, const Vector3f &  v2);
  void scale(float s, const Vector3f & v);
  void scale(float s);
  void add(const Vector3f & v1, const Vector3f & v2);
  void add(const Vector3f & v1);
  void add(const Vector3sh & v1);
  float normalize();
  void normalize1();
  void negate();
  void negate(const Vector3f & v);
  float dot(const Vector3f &  v) const;
  void xform_vec(const Vector3f & v, const Matrix4f & m);
  float length() const;
  void add_scaled(const Vector3f & v1, float s, const Vector3f & v2);
  void mul(const Vector3f & v1, const Vector3f & v2);
  void mul(const Vector3sh & v1, const Vector3f & v2);
  float sqr_distance(const Vector3f & v) const;
  float distance(const Vector3f & v) const;
  void xform_pt(const Vector3f & v, const Matrix4f & m);
  void div(float, const Vector3f & v);
  void round();
    
  bool less(const Vector3f &  v) const;
  bool almost_equal(const Vector3f & v, float tol) const;
  void combine(float a, const Vector3f & v1, float b, const Vector3f & v2);
  void full_xform_pt(const Vector3f & v, const Matrix4f & m);
  void clamp(const Vector3f & v,
             const Vector3f & my_min, const Vector3f & my_max);

  Vector3f & operator=(const Vector3f & v);
  Vector3f & operator=(float v);
  float & operator[](int i);
  float operator[](int i) const;
  bool operator==(const Vector3f & v) const;
  bool operator!=(const Vector3f & v) const;

private:
  float length_reciprocal1() const;
};

/*!
 */
inline float & Vector3f::operator[](int i)
{
  return m_vector[i];
}

/*!
 */
inline float Vector3f::operator[](int i) const
{
  return m_vector[i];
}

/*!
 */
inline void Vector3f::set(float a, float b, float c)
{
  m_vector[0] = a; 
  m_vector[1] = b; 
  m_vector[2] = c;
}

/*!
 */
inline void Vector3f::get(float * a, float * b, float * c)  const
{
  *a = m_vector[0]; 
  *b = m_vector[1]; 
  *c = m_vector[2];
}

/*!
 */
inline void Vector3f::set(int i, float val)
{
  m_vector[i] = val;
}

/*!
 */
inline float Vector3f::get(int i) const
{
  return m_vector[i];
}

/*!
 */
inline void Vector3f::set(const Vector3f & v)
{
  m_vector[0] = v.m_vector[0];
  m_vector[1] = v.m_vector[1];
  m_vector[2] = v.m_vector[2];
}

/*!
 */
inline void Vector3f::get(Vector3f & v) const
{
  v.m_vector[0] = m_vector[0];
  v.m_vector[1] = m_vector[1];
  v.m_vector[2] = m_vector[2];
}

/*! Constrcutor */
inline Vector3f::Vector3f()
{
  m_vector[0] = m_vector[1] = m_vector[2] = 0.0f;
}

/*! Constrcutor */
inline Vector3f::Vector3f(float a, float b, float c) { set(a, b, c); }

/*! Constrcutor */
inline Vector3f::Vector3f(const Vector3f & v) { set(v); }

/*! Constrcutor */
inline Vector3f::Vector3f(const std::string & text)
{
  std::istringstream tmp(text, std::istringstream::in);
  tmp >> m_vector[0];
  tmp >> m_vector[1];
  tmp >> m_vector[2];
}

/*! sets this vector to be v1 - v2 */
inline void Vector3f::sub(const Vector3f & v1, const Vector3f & v2)
{
  m_vector[0] = v1[0] - v2[0];
  m_vector[1] = v1[1] - v2[1];
  m_vector[2] = v1[2] - v2[2];
}

/*! sets this vector to be v1 - v2 */
inline void Vector3f::sub(const Vector3sh & v1, const Vector3sh & v2)
{
  m_vector[0] = (float) v1[0] - (float) v2[0];
  m_vector[1] = (float) v1[1] - (float) v2[1];
  m_vector[2] = (float) v1[2] - (float) v2[2];
}

/*! sets this vector to be this - v */
inline void Vector3f::sub(const Vector3f & v)
{
  m_vector[0] -= v[0];   
  m_vector[1] -= v[1];
  m_vector[2] -= v[2];
}

/*!
 */
inline bool Vector3f::equal(const Vector3f & v) const
{
  return(m_vector[0] == v[0] && m_vector[1] == v[1] && m_vector[2] == v[2]);
}

/*!
 */
inline Vector3f & Vector3f::operator=(const Vector3f & v)
{
  set(v);
  return *this;
}

/*!
 */
inline Vector3f & Vector3f::operator=(float v)
{
  m_vector[0] = v; 
  m_vector[1] = v; 
  m_vector[2] = v;
  return *this;
}

/*!
 */
inline bool Vector3f::operator==(const Vector3f & v) const
{
  return equal(v);
}

/*!
 */
inline bool Vector3f::operator!=(const Vector3f & v) const
{
  return !equal(v);
}

/*!
 */
inline void Vector3f::scale(float s, const Vector3f & v)
{
  m_vector[0] = s * v[0];
  m_vector[1] = s * v[1];
  m_vector[2] = s * v[2];
}

/*!
 */
inline void Vector3f::scale(float s)
{
  m_vector[0] *= s;
  m_vector[1] *= s;
  m_vector[2] *= s;
}

/*!
 */
inline void Vector3f::add(const Vector3f & v1, const Vector3f & v2)
{
  m_vector[0] = v1[0] + v2[0];
  m_vector[1] = v1[1] + v2[1];
  m_vector[2] = v1[2] + v2[2];
}

/*!
 */
inline void Vector3f::add(const Vector3f & v1)
{
  m_vector[0] += v1[0] ;
  m_vector[1] += v1[1] ;
  m_vector[2] += v1[2] ;
}

/*!
 */
inline void Vector3f::add(const Vector3sh & v1)
{
  m_vector[0] += v1[0] ;
  m_vector[1] += v1[1] ;
  m_vector[2] += v1[2] ;
}

inline void Vector3f::negate()
{
  m_vector[0] = -m_vector[0];
  m_vector[1] = -m_vector[1];
  m_vector[2] = -m_vector[2];
}

/*!
 */
inline void Vector3f::negate(const Vector3f & v)
{
  m_vector[0] = -v[0];
  m_vector[1] = -v[1];
  m_vector[2] = -v[2];
}

/*! Returns the dot product of this vector with v.
 */
inline float Vector3f::dot(const Vector3f &  v) const
{
  return(m_vector[0] * v[0] + m_vector[1] * v[1] + m_vector[2] * v[2]);
}

/*!
 */
inline void Vector3f::add_scaled(const Vector3f & v1, float s,
                                 const Vector3f & v2)
{
  m_vector[0] = v1[0] + s * v2[0];
  m_vector[1] = v1[1] + s * v2[1];
  m_vector[2] = v1[2] + s * v2[2];
}

/*!
 */
inline float Vector3f::length() const
{
  return squarerootf(m_vector[0] * m_vector[0] +
                     m_vector[1] * m_vector[1] +
                     m_vector[2] * m_vector[2]);
}

/*!
 */
inline float Vector3f::normalize()
{
  float my_length = length();
  if (my_length == 0.0f) return my_length;
  float length_reciprocal = 1.0f / my_length;
  m_vector[0] *= length_reciprocal;
  m_vector[1] *= length_reciprocal;
  m_vector[2] *= length_reciprocal;
  return my_length;
}

/*!
 */
inline void Vector3f::mul(const Vector3f & v1, const Vector3f & v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
  m_vector[2] = v1[2] * v2[2];
}

/*!
 */
inline void Vector3f::mul(const Vector3sh & v1, const Vector3f & v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
  m_vector[2] = v1[2] * v2[2];
}

/*!
 */
inline float Vector3f::sqr_distance(const Vector3f & v) const
{
  float d0, d1, d2;
  d0 = m_vector[0] - v.m_vector[0];
  d1 = m_vector[1] - v.m_vector[1];
  d2 = m_vector[2] - v.m_vector[2];
  return d0 * d0 + d1 * d1 + d2 * d2;
}

/*!
 */
inline float Vector3f::distance(const Vector3f & v) const
{
  float sqr_dist = sqr_distance(v);
  return sqrtf(sqr_dist);
}

/*!
 */
inline void Vector3f::div(float c, const Vector3f & v)
{
  m_vector[0] = c / v[0];
  m_vector[1] = c / v[1];
  m_vector[2] = c / v[2];
}

/*!
 */
inline void Vector3f::round()
{
  m_vector[0] = (float) ((int)m_vector[0]); 
  m_vector[1] = (float) ((int)m_vector[1]); 
  m_vector[2] = (float) ((int)m_vector[2]);
}

/*!
 */
inline float Vector3f::length_reciprocal1() const
{
  return Math::sqrt_reciprocalf(dot(*this));
}

/*!
 */
inline bool Vector3f::less(const Vector3f & v) const
{
  if (m_vector[0] != v[0])
    return(m_vector[0] < v[0] );
  if (m_vector[1] != v[1])
    return(m_vector[1] < v[1] );
  if (m_vector[2] != v[2])
    return(m_vector[2] < v[2] );
  return false;    
}

/*!
 */
inline bool Vector3f::almost_equal(const Vector3f & v, float tol) const
{
  float neg_tol = -tol;
  float diff = m_vector[0] - v[0];
  if ((diff < neg_tol) || (tol < diff)) return false;
  diff = m_vector[1] - v[1];
  if ((diff < neg_tol) || (tol < diff)) return false;
  diff = m_vector[2] - v[2];
  if ((diff < neg_tol) || (tol < diff)) return false;
  return true;
}

/*!
 */
inline void Vector3f::combine(float a, const Vector3f & v1, float b,
                              const Vector3f & v2)
{
  m_vector[0] = a * v1[0] + b * v2[0];
  m_vector[1] = a * v1[1] + b * v2[1];
  m_vector[2] = a * v1[2] + b * v2[2];
}

/*!
 */
inline float sqr_distance_pt3(Vector3f p1, Vector3f p2)
{
  return ((square(p1[0] - p2[0]) + square(p1[1] - p2[1]) + 
           square(p1[2] - p2[2])));
}

#if defined(SOLARIS_251)
Vector3f operator+(const Vector3f& _v1, const Vector3f& _v2);
Vector3f operator*(const float f, const Vector3f& v);
Vector3f operator*(const Vector3f& v, const float f);
bool operator>(const Vector3f& _v1, const Vector3f& _v2);
bool operator<(const Vector3f& _v1, const Vector3f& _v2);

inline bool operator<(const Vector3f& _v1, const Vector3f& _v2)
{
  return (_v1[0] < _v2[0] && _v1[1] < _v2[1] && _v1[2] < _v2[2]);
}

inline bool operator>(const Vector3f& _v1, const Vector3f& _v2)
{
  return (_v1[0] > _v2[0] && _v1[1] > _v2[1] && _v1[2] > _v2[2]);
}

inline Vector3f operator+(const Vector3f& _v1, const Vector3f& _v2)
{                                 
  Vector3f * c = new Vector3f;     
                                  
  c->set(_v1[0] + _v2[0], _v1[1] + _v2[1], _v1[2] + _v2[2]);
  return(*c);                   
}

inline Vector3f operator*(const Vector3f v, const float f)     
{                                 
  Vector3f * c = new Vector3f;     
                                  
  c->set(v[0] * f, v[1] + f);   
  return(*c);                   
}                                 
                                  
inline Vector3f operator*(const float f, const Vector3f v)
{
  Vector3f * c = new Vector3f;
 
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

#endif

/*! Exporter */
inline std::ostream & operator<<(std::ostream & os, const Vector3f & vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2];
  return os;
}  

/*! Importer */
inline std::istream & operator >>(std::istream & in, Vector3f & vec)
{
  Float x, y, z;
  in >> x >> y >> z;
  vec.set(x, y, z);
  return in;
}

SGAL_END_NAMESPACE

#endif
