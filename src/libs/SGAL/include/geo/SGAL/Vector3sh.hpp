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
// $Revision: 1326 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR3SH_HPP
#define SGAL_VECTOR3SH_HPP

/*! \file
 */

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Vector3sh {
private:
  short m_vector[3];

public:    // Constructors
  Vector3sh();
  Vector3sh(short a, short b, short c);
  Vector3sh(const Vector3sh & v);

public:  
  // Accessor functions
  void set(short a, short b, short c);
  void get(short * a, short * b, short * c)  const;
  void set(int i, short f);
  short get(int i) const;
  void set(const Vector3sh & v);
  void get(Vector3sh & v) const;
  
public:  
  // Math functions

  void add(const Vector3sh & v1, const Vector3sh & v2);
  void add(const Vector3sh & v);
  void sub(const Vector3sh & v1, const Vector3sh & v2);
  void sub(const Vector3sh & v);
  bool less(const Vector3sh & v) const;
  void scale(float s);
  bool equal(const Vector3sh & v) const;

  /* bool AlmostEqual(const Vector3f & v, float tol) const;
     void Negate(const Vector3f & v);
     void Negate();
     float Dot(const Vector3f &  v) const;
     void Mul(const Vector3f & v1, const Vector3f & v2);
     void Div(float, const Vector3f & v);
     void Scale(float s, const Vector3f & v);
    
     void AddScaled(const Vector3f & v1, float s, const Vector3f & v2);
     void Combine(float a, const Vector3f & v1, float b, const Vector3f & v2);
     float SqrDistance(const Vector3f & v) const;
     float Normalize();
     float Length() const;
     float Distance(const Vector3f & v) const;
     void Cross(const Vector3f &  v1, const Vector3f &  v2);
     void XformVec(const Vector3f & v, const Matrix4f & m);
     void XformPt(const Vector3f & v, const Matrix4f & m);
     void FullXformPt(const Vector3f & v, const Matrix4f & m);
  
     void Clamp(const Vector3f & v, const Vector3f & min, const Vector3f & max);
  */

public:
  // Opertors

  Vector3sh & operator=(const Vector3sh & v);
    
  short & operator[](int i);
  short operator[](int i) const;
  bool operator==(const Vector3sh & v) const;
  bool operator!=(const Vector3sh & v) const;
};

inline Vector3sh::Vector3sh()
{
  m_vector[0] = m_vector[1] = m_vector[2] = 0;
}

inline Vector3sh::Vector3sh(short a, short b, short c)
{
  set(a, b, c);
}

inline Vector3sh::Vector3sh(const Vector3sh & v)
{
  set(v);
}

inline short & Vector3sh::operator[](int i)
{
  return m_vector[i];
}

inline short Vector3sh::operator[](int i) const
{
  return m_vector[i];
}

inline void Vector3sh::set(short a, short b, short c)
{
  m_vector[0] = a; 
  m_vector[1] = b; 
  m_vector[2] = c;
}

inline void Vector3sh::get(short * a, short * b, short * c)  const
{
  *a = m_vector[0]; 
  *b = m_vector[1]; 
  *c = m_vector[2];
}

inline void Vector3sh::set(int i, short val)
{
  m_vector[i] = val;
}

inline short Vector3sh::get(int i) const
{
  return m_vector[i];
}

inline void Vector3sh::set(const Vector3sh & v)
{
  m_vector[0] = v.m_vector[0];
  m_vector[1] = v.m_vector[1];
  m_vector[2] = v.m_vector[2];
}

inline void Vector3sh::get(Vector3sh & v) const
{
  v.m_vector[0] = m_vector[0];
  v.m_vector[1] = m_vector[1];
  v.m_vector[2] = m_vector[2];
}

inline bool Vector3sh::less(const Vector3sh & v) const
{
  if (m_vector[0] != v[0])
    return(m_vector[0] < v[0] );
  if (m_vector[1] != v[1])
    return(m_vector[1] < v[1] );
  if (m_vector[2] != v[2])
    return(m_vector[2] < v[2] );
  return false;
    
}

inline void Vector3sh::add(const Vector3sh & v1, const Vector3sh & v2)
{
  m_vector[0] = v1[0] + v2[0];
  m_vector[1] = v1[1] + v2[1];
  m_vector[2] = v1[2] + v2[2];
}

inline void Vector3sh::add(const Vector3sh & v1)
{
  m_vector[0] += v1[0] ;
  m_vector[1] += v1[1] ;
  m_vector[2] += v1[2] ;
}

inline void Vector3sh::sub(const Vector3sh & v)
{
  m_vector[0] -= v[0];   
  m_vector[1] -= v[1];
  m_vector[2] -= v[2];
}

inline void Vector3sh::scale(float s)
{
  m_vector[0] = (short) (s * m_vector[0]);
  m_vector[1] = (short) (s * m_vector[1]);
  m_vector[2] = (short) (s * m_vector[2]);
}

inline Vector3sh & Vector3sh::operator=(const Vector3sh & v)
{
  set(v);
  return *this;
}

inline bool Vector3sh::operator==(const Vector3sh & v) const
{
  return equal(v);
}

inline bool Vector3sh::operator!=(const Vector3sh & v) const
{
  return !equal(v);
}

inline bool Vector3sh::equal(const Vector3sh & v) const
{
  return(m_vector[0] == v[0] && m_vector[1] == v[1] && m_vector[2] == v[2]);
}

inline void Vector3sh::sub(const Vector3sh & v1, const Vector3sh & v2)
{
  m_vector[0] = v1[0] - v2[0];
  m_vector[1] = v1[1] - v2[1];
  m_vector[2] = v1[2] - v2[2];
}

/*! Exporter */
inline std::ostream & operator<<(std::ostream & os, const Vector3sh & vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2];
  return os;
}  

SGAL_END_NAMESPACE

#endif
