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
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

/*!
 */
std::string Vector3f::get_text() 
{
  char buf[64];
  sprintf(buf, "%g %g %g", m_vector[0], m_vector[1], m_vector[2]);
  std::string str(buf);
  return str;
}

/*!
 */
float Vector3f::get_max_comp()
{
  float max = (m_vector[0] > m_vector[1] ? m_vector[0] : m_vector[1]);
  max = (max > m_vector[2] ? max : m_vector[2]);
  return max;
}

/*! Normalize1
 * 1 iteration of precision
 */
void Vector3f::normalize1()
{
  float length_reciprocal = length_reciprocal1();
  m_vector[0] *= length_reciprocal;
  m_vector[1] *= length_reciprocal;
  m_vector[2] *= length_reciprocal;
}

/*! Sets this vector to be v, thought of as a row vector, times the 3X3
 * submatrix formed by the first 3 rows and first three columns of m.
 */
void Vector3f::xform_vec(const Vector3f & v, const Matrix4f & m)
{
  Vector3f t;
  t.set(v);
  m_vector[0] = t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0];
  m_vector[1] = t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1];
  m_vector[2] = t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2];
}

/*! Cross -  Sets this vector to be the cross product of v1 and v2.
 */
void Vector3f::cross(const Vector3f & v1, const Vector3f & v2)
{
    m_vector[0] = v1[1] * v2[2] - v1[2] * v2[1];
    m_vector[1] = v1[2] * v2[0] - v1[0] * v2[2];
    m_vector[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

/*! Sets this vector to be the first three components of (v,1) * m where v
 * is treated as a row vector.
 */
void Vector3f::xform_pt(const Vector3f & v, const Matrix4f & m)
{
  Vector3f t;
  t.set(v);
  m_vector[0] = t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0] + m[3][0];
  m_vector[1] = t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1] + m[3][1];
  m_vector[2] = t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2] + m[3][2];
}

/*!
 */
void Vector3f::clamp(const Vector3f & v,
                     const Vector3f & min, const Vector3f & max)
{
  m_vector[0] = (v[0] < min[0]) ? min[0] : (max[0] < v[0]) ? max[0] : v[0];
  m_vector[1] = (v[1] < min[1]) ? min[1] : (max[1] < v[1]) ? max[1] : v[1];
  m_vector[2] = (v[2] < min[2]) ? min[2] : (max[2] < v[2]) ? max[2] : v[2];
}

/*!  Sets this vector to be the inhomogeneous coordinates (that, divided by w) 
 * of (v,1) * m where v is treated as a row vector. 
 */
void Vector3f::full_xform_pt(const Vector3f & v, const Matrix4f & m)
{
  Vector3f t;
  t.set(v);
  float w = t[0]*m[0][3] + t[1]*m[1][3] + t[2]*m[2][3] + m[3][3];
  m_vector[0] = (t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0] + m[3][0]) / w;
  m_vector[1] = (t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1] + m[3][1]) / w;
  m_vector[2] = (t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2] + m[3][2]) / w;
}

SGAL_END_NAMESPACE
