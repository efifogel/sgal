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
// $SId: $
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief */
std::string Rotation::get_text()
{
  char buf[64];
  sprintf(buf, "%g %g %g %g", m_axis[0], m_axis[1], m_axis[2], m_angle);
  std::string str(buf);
  return str;
}

/*! \brief */
void Rotation::make_quaternion(Vector4f& q) const
{
  float angle = m_angle * 0.5f;
  float mySin = sinf(angle);
  float myCos = cosf(angle);
  q[0] = m_axis[0] * mySin;
  q[1] = m_axis[1] * mySin;
  q[2] = m_axis[2] * mySin;
  q[3] = myCos;
}

/*! \brief */
void Rotation::set_quaternion(const Vector4f& q)
{
  float angle = (q[3] > 1) ? 0 : arccosf(q[3]);
  m_angle = angle * 2.0f;
  float mySin = sinf(angle);

  if (abs(mySin) > SGAL_EPSILON) {
    m_axis[0] = q[0] / mySin;
    m_axis[1] = q[1] / mySin;
    m_axis[2] = q[2] / mySin;
  }
  /*
    else
    {
        m_axis[0] = 1;
        m_axis[1] = 0;
        m_axis[2] = 0;
    }
  */
}

/*! \brief this = q1 * q2. This is a quaternion multiplication. */
void Rotation::mult(const Rotation& r1, const Rotation& r2)
{
  Vector4f q1;
  Vector4f q2;
  r1.make_quaternion(q1);
  r2.make_quaternion(q2);

  // Multiply the quaternions:
  Vector4f q;
  q[0] = q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0] + q1[0]*q2[3];
  q[1] = q1[2]*q2[0] - q1[0]*q2[2] + q1[3]*q2[1] + q1[1]*q2[3];
  q[2] = q1[0]*q2[1] - q1[1]*q2[0] + q1[3]*q2[2] + q1[2]*q2[3];
  q[3] = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];

  set_quaternion(q);
}

/*! \brief sets this rotation to be the reverse of the given rotation. */
void Rotation::invert(const Rotation& /* q1 */)
{
  m_axis.negate();
}

/*! \brief Spherical interpolation for quaternions. */
void Rotation::slerp(Float t, const Rotation& r1, const Rotation& r2)
{
  Vector4f q1, q2, qr;
  r1.make_quaternion(q1);
  r2.make_quaternion(q2);

  float cos_half_theta = q1.dot(q2);
  if (abs(cos_half_theta) >= 1) qr = q1;
  else {
    // Calculate temporary values.
    double sin_half_theta = sqrtf(1.0 - cos_half_theta * cos_half_theta);
    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to q1 or q2
    if (abs(sin_half_theta) < SGAL_EPSILON) qr.combine(0.5, q1, 0.5, q2);
    else {
      float half_theta = acosf(cos_half_theta);
      float ratio1 = sinf((1 - t) * half_theta) / sin_half_theta;
      float ratio2 = sinf(t * half_theta) / sin_half_theta; 
      qr.combine(ratio1, q1, ratio2, q2);
    }
  }
  
  set_quaternion(qr);
}

/*! \brief sets this rotation to be the rotation that rotates a given
 * source unit vector to a given destination unit vector.
 */
void Rotation::make(const Vector3f& src, const Vector3f& dst)
{
  float dot_product = src.dot(dst) ;
  m_axis.cross(src,dst);
  float size = m_axis.normalize() ;
    
  if (size == 0.0f) {
    Vector3f tmp ;
    tmp.scale(-1.0,dst)  ;
    // assuming src and dst are equal or so close - so we can define zero
    // Rotation.
    m_axis[0] = 1 ;
    m_axis[1] = 0 ;
    m_axis[2] = 0 ;
    m_angle = 0.0f ;
  } else {
    m_angle = (float) atan2f(size, dot_product) ;
  }
}

/*! \brief rotates the given source vector using this rotation. */
void Rotation::rotate(const Vector3f& src, Vector3f& dst) const
{
  Matrix4f mat;
  mat.make_rot(m_axis[0], m_axis[1], m_axis[2], m_angle);
  dst.xform_vec(src, mat);
}

SGAL_END_NAMESPACE
