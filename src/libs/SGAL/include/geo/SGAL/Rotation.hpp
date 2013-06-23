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
// $Id: $
// $Revision: 6096 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ROTATION_HPP
#define SGAL_ROTATION_HPP

/*! \file
 *
 */

#include <string>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Rotation {
public:
  /*! Default Constructor */
  Rotation();
  
  /*! Constructor */
  Rotation(float x, float y, float z, float radians);

  /*! Constructor */
  Rotation(const Vector3f& axis, float radians);

  /*! Copy Constructor */
  Rotation(const Rotation& r);

  /*! Constructor */
  Rotation(const std::string& text);

  void set_angle(float angle);
  float get_angle() const;
  void set_axis(float x, float y, float z);
  void set_axis(const Vector3f& axis);
  void get_axis(Vector3f& axis) const;
  Vector3f& get_axis() { return m_axis; }

  void set(float x, float y, float z, float radians);
  void get(float* x, float* y, float* z, float* radians) const;
  void set(const Rotation& r);
  void get(Rotation& r) const;
  void set(const Vector3f& axis, float radians);
  std::string get_text();

  void mult(const Rotation& q1, const Rotation& q2);
  void invert(const Rotation& q1);
  void slerp(float t, const Rotation& q1, const Rotation& q2);
  void make(const Vector3f& src, const Vector3f& dst);
  void rotate(const Vector3f& src, Vector3f& dst) const;
    
  float& operator[](int i);
  float operator[](int i) const;

  bool operator!=(const Rotation& r) const;
  int operator==(const Rotation& r) const;

private:
  /** the axis around which the rotation takes place */
  Vector3f m_axis;

  /** the angle of rotation */
  float m_angle;

  void make_quaternion(Vector4f& q) const;
  void set_quaternion(const Vector4f& q);
};

/*! Default Constructor */
inline Rotation::Rotation() : m_axis(0, 0, 1), m_angle(0) {}

/*! Constructor */
inline Rotation::Rotation(float x, float y, float z, float radians) :
  m_axis(x, y, z), m_angle(radians)
{ m_axis.normalize(); }

/*! Constructor -  notice that axis is not normalized */
inline Rotation::Rotation(const Vector3f& axis, float radians) :
  m_axis(axis), m_angle(radians)
{}

/*! Copy Constructor */
inline Rotation::Rotation(const Rotation& r) { set(r); }

/*! Constructor */
inline Rotation::Rotation(const std::string& text)
{
  std::istringstream tmp(text, std::istringstream::in);
  tmp >> m_axis[0] >> m_axis[1] >> m_axis[2] >> m_angle;
  m_axis.normalize();
}

/*! \brief */
inline void Rotation::set(float x, float y, float z, float radians)
{
  set_axis(x, y, z);
  set_angle(radians);
}

/*! \brief */
inline void Rotation::set(const Vector3f& axis, float radians)
{
  set_axis(axis);
  set_angle(radians);
}

/*! \brief */
inline void Rotation::get(float* x, float* y, float* z, float* radians) const
{
  *x = m_axis[0];
  *y = m_axis[1];
  *z = m_axis[2];
  *radians = m_angle;
}

/*! \brief */
inline void Rotation::set_angle(float angle) { m_angle = angle; }

/*! \brief */
inline void Rotation::set_axis(float x, float y, float z)
{
  m_axis[0] = x;
  m_axis[1] = y;
  m_axis[2] = z;
  (void) m_axis.normalize();
}

/*! \brief */
inline float Rotation::get_angle() const { return m_angle; }

/*! \brief */
inline void Rotation::set_axis(const Vector3f& axis)
{ set_axis(axis[0], axis[1], axis[2]); }

/*! \brief */
inline void Rotation::get_axis(Vector3f& axis) const { axis = m_axis; }

/*! \brief */
inline void Rotation::set(const Rotation& r)
{
  m_axis = r.m_axis;
  m_angle = r.m_angle;
}

/*! \brief */
inline void Rotation::get(Rotation& r) const
{
  r.m_axis = m_axis;
  r.m_angle = m_angle;
}

/*! \brief */
inline float& Rotation::operator[](int i)
{
  if (i == 3) return m_angle;
  return m_axis[i];
}

/*! \brief */
inline float Rotation::operator[](int i) const
{
  if (i == 3) return m_angle;
  return m_axis[i];
}

/*! \brief */
inline bool Rotation::operator!=(const Rotation& r) const
{ return m_axis != r.m_axis || m_angle != r.m_angle; }

/*! \brief */
inline int Rotation::operator==(const Rotation& r) const
{ return m_axis == r.m_axis && m_angle == r.m_angle; }

/*! Exporter */
inline std::ostream& operator<<(std::ostream& os, const Rotation& rot)
{
  os << rot[0] << ", " << rot[1] << ", " << rot[2] << ", " << rot[3];
  return os;
}

/*! Importer */
inline std::istream& operator >>(std::istream& in, Rotation& rot)
{
  float x, y, z, angle;
  in >> x >> y >> z >> angle;
  rot.set(x, y, z, angle);
  return in;
}

SGAL_END_NAMESPACE

#endif
