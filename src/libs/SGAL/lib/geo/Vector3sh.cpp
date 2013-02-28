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
// Id: $
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Vector3sh.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief */
void Vector3f::negate(const Vector3f& v)
{
  m_vector[0] = -v[0];
  m_vector[1] = -v[1];
  m_vector[2] = -v[2];
}

/*! \brief */
void Vector3f::negate()
{
  m_vector[0] = -m_vector[0];
  m_vector[1] = -m_vector[1];
  m_vector[2] = -m_vector[2];
}

/*! \brief */
float Vector3f::dot(const Vector3f&  v) const
{ return(m_vector[0] * v[0] + m_vector[1] * v[1] + m_vector[2] * v[2]); }

/*! \brief */
void Vector3f::add(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] + v2[0];
  m_vector[1] = v1[1] + v2[1];
  m_vector[2] = v1[2] + v2[2];
}

/*! \brief */
void Vector3f::scale(float s, const Vector3f& v)
{
  m_vector[0] = s * v[0];
  m_vector[1] = s * v[1];
  m_vector[2] = s * v[2];
}

/*! \brief */
void Vector3f::add_scaled(const Vector3f& v1, float s, const Vector3f& v2)
{
  m_vector[0] = v1[0] + s * v2[0];
  m_vector[1] = v1[1] + s * v2[1];
  m_vector[2] = v1[2] + s * v2[2];
}

/*! \brief */
void Vector3f::combine(float a, const Vector3f& v1, float b,
                       const Vector3f& v2)
{
  m_vector[0] = a * v1[0] + b * v2[0];
  m_vector[1] = a * v1[1] + b * v2[1];
  m_vector[2] = a * v1[2] + b * v2[2];
}

/*! \brief */
float Vector3f::sqr_distance(const Vector3f& v) const
{
  float d0, d1, d2;
  d0 = m_vector[0] - v.m_vector[0];
  d1 = m_vector[1] - v.m_vector[1];
  d2 = m_vector[2] - v.m_vector[2];
  return d0 * d0 + d1 * d1 + d2 * d2;
}

/*! \brief */
float Vector3f::length() const { return EnbSqrtf(Dot(*this)); }

/*! \brief */
float Vector3f::distance(const Vector3f& v) const
{
  float sqrDist = SqrDistance(v);
  return sqrt(sqrDist);
}

/*! \brief */
Vector3f::vector3f(const std::string &text)
{
  String_vector vals = std::stringUtils::SplitStringByToken(text, " "); 
  if (vals.size() != 3) m_vector[0] = m_vector[1] = m_vector[2] = 0.0;
  m_vector[0] = atof(vals[0].c_str());
  m_vector[1] = atof(vals[1].c_str());
  m_vector[2] = atof(vals[2].c_str());
}

/*! \brief */
void Vector3f::mul(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
  m_vector[2] = v1[2] * v2[2];
}

/*! \brief */
void Vector3f::div(float c, const Vector3f& v)
{
  m_vector[0] = c / v[0];
  m_vector[1] = c / v[1];
  m_vector[2] = c / v[2];
}

/*! \brief */
float Vector3f::normalize()
{
  float length = length();
  if (length == 0.0f) return length;
  float length_reciprocal = 1.0f / length;
  m_vector[0] *= length_reciprocal;
  m_vector[1] *= length_reciprocal;
  m_vector[2] *= length_reciprocal;
  return length;
}

/*! \brief sets this vector to be v, thought of as a row vector, times the 3X3
 * submatrix formed by the first 3 rows and first three columns of m
 */
void Vector3f::xform_vec(const Vector3f& v, const Matrix4f& m)
{
  Vector3f t;
  t.set(v);
  m_vector[0] = t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0];
  m_vector[1] = t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1];
  m_vector[2] = t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2];
}

/*! \brief sets this vector to be the first three components of (v,1) * m
 * where v is treated as a row vector.
 */
void Vector3f::xform_pt(const Vector3f& v, const Matrix4f& m)
{
  Vector3f t;
  t.Set(v);
  m_vector[0] = t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0] + m[3][0];
  m_vector[1] = t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1] + m[3][1];
  m_vector[2] = t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2] + m[3][2];
}

/*! \brief Sets this vector to be the inhomogeneous coordinates (that, divided
 * by w) of (v,1) * m where v is treated as a row vector.
 */
void Vector3f::full_xform_pt(const Vector3f& v, const Matrix4f& m)
{
  Vector3f t;
  t.Set(v);
  float w = t[0]*m[0][3] + t[1]*m[1][3] + t[2]*m[2][3] + m[3][3];
  m_vector[0] = (t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0] + m[3][0]) / w;
  m_vector[1] = (t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1] + m[3][1]) / w;
  m_vector[2] = (t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2] + m[3][2]) / w;
}

/*! \brief sets this vector to be the cross product of v1 and v2. */
void Vector3f::cross(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[1] * v2[2] - v1[2] * v2[1];
  m_vector[1] = v1[2] * v2[0] - v1[0] * v2[2];
  m_vector[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

/*! \brief */
void Vector3f::clamp(const Vector3f& v,
                     const Vector3f& min, const Vector3f& max)
{
  m_vector[0] = (v[0] < min[0]) ? min[0] : (max[0] < v[0]) ? max[0] : v[0];
  m_vector[1] = (v[1] < min[1]) ? min[1] : (max[1] < v[1]) ? max[1] : v[1];
  m_vector[2] = (v[2] < min[2]) ? min[2] : (max[2] < v[2]) ? max[2] : v[2];
}

SGAL_END_NAMESPACE
