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
  /*! Default Constructor. */
  Rotation();

  /*! Constructor.
   * \param x (in) the rotation axis x-coordinate.
   * \param y (in) the rotation axis y-coordinate.
   * \param z (in) the rotation axis z-coordinate.
   * \param angle (in) the rotation angle in radians.
   */
  Rotation(Float x, Float y, Float z, Float angle);

  /*! Constructor.
   * \param axis (in) the rotation axis.
   * \param angle (in) the rotation angle in radians.
   */
  Rotation(const Vector3f& axis, Float angle);

  /*! Copy Constructor.
   * \param other (in) the other rotation.
   */
  Rotation(const Rotation& other);

  /*! Constructor from string.
   * \param str (in) the rotation representation.
   */
  Rotation(const std::string& str);

  /*! Set the rotation angle.
   * \param angle (in) the rotation angle.
   */
  void set_angle(Float angle);

  /*! Obtain the (const) rotation angle.
   * \return the rotation angle.
   */
  Float get_angle() const;

  /*! Obtain the (non-const) rotation angle.
   * \return the rotation angle.
   */
  Float& get_angle();

  /*! Set the rotation axis.
   * \param x (in) the rotation axis x-coordinate.
   * \param y (in) the rotation axis y-coordinate.
   * \param z (in) the rotation axis z-coordinate.
   */
  void set_axis(Float x, Float y, Float z);

  /*! Set the rotation axis.
   * \param axis (in) the rotation axis.
   */
  void set_axis(const Vector3f& axis);

  /*! Obtain the (const) rotation axis.
   * \return the rotation axis.
   */
  const Vector3f& get_axis() const;

  /*! Obtain the (non-const) rotation axis.
   * \return the rotation axis.
   */
  Vector3f& get_axis();

  /*! Set the rotation.
   * \param x (in) the rotation axis x-coordinate.
   * \param y (in) the rotation axis y-coordinate.
   * \param z (in) the rotation axis z-coordinate.
   * \param angle (in) the rotation angle in radians.
   */
  void set(Float x, Float y, Float z, Float angle);

  /*! Set the rotation.
   * \param rotation (in) the new rotation.
   */
  void set(const Rotation& rotation);

  /*! Set the rotation.
   * \param axis (in) the rotation axis.
   * \param angle (in) the rotation angle in radians.
   */
  void set(const Vector3f& axis, Float angle);

  /*! Obtain the rotation.
   * \param x (out) the rotation axis x-coordinate.
   * \param y (out) the rotation axis y-coordinate.
   * \param z (out) the rotation axis z-coordinate.
   * \param angle (out) the rotation angle in radians.
   */
  void get(Float* x, Float* y, Float* z, Float* angle) const;

  std::string get_text();

  void mult(const Rotation& q1, const Rotation& q2);

  void invert(const Rotation& q1);

  void slerp(Float t, const Rotation& q1, const Rotation& q2);

  void make(const Vector3f& src, const Vector3f& dst);

  void rotate(const Vector3f& src, Vector3f& dst) const;

  /*! Obrain a (const) rotation component.
   * \param i (in) the component index.
   */
  Float operator[](size_t i) const;

  /*! Obrain a (const) rotation component.
   * \param i (in) the component index.
   */
  Float& operator[](size_t i);

  /*! Inequality operator.
   */
  Boolean operator!=(const Rotation& rotation) const;

  /*! Equality operator.
   */
  Boolean operator==(const Rotation& rotation) const;

private:
  /*! The axis around which the rotation takes place. */
  Vector3f m_axis;

  /*! The angle of rotation. */
  Float m_angle;

  void make_quaternion(Vector4f& q) const;
  void set_quaternion(const Vector4f& q);
};

//! \brief default Constructor.
inline Rotation::Rotation() : m_axis(0, 0, 1), m_angle(0) {}

//! \brief constructor.
inline Rotation::Rotation(Float x, Float y, Float z, Float angle) :
  m_axis(x, y, z), m_angle(angle)
{ m_axis.normalize(); }

//! \brief constructor---notice that axis is not normalized.
inline Rotation::Rotation(const Vector3f& axis, Float angle) :
  m_axis(axis), m_angle(angle)
{}

//! \brief copy Constructor.
inline Rotation::Rotation(const Rotation& r) { set(r); }

//! Constructor from string.
inline Rotation::Rotation(const std::string& text)
{
  std::istringstream tmp(text, std::istringstream::in);
  tmp >> m_axis[0] >> m_axis[1] >> m_axis[2] >> m_angle;
  m_axis.normalize();
}

//! \brief sets the rotation.
inline void Rotation::set(Float x, Float y, Float z, Float angle)
{
  set_axis(x, y, z);
  set_angle(angle);
}

//! \brief sets the rotation.
inline void Rotation::set(const Vector3f& axis, Float angle)
{
  set_axis(axis);
  set_angle(angle);
}

//! \brief obtain the rotation.
inline void Rotation::get(Float* x, Float* y, Float* z, Float* angle) const
{
  *x = m_axis[0];
  *y = m_axis[1];
  *z = m_axis[2];
  *angle = m_angle;
}

//! \brief set the rotation angle.
inline void Rotation::set_angle(Float angle) { m_angle = angle; }

//! \brief set the rotation axis.
inline void Rotation::set_axis(Float x, Float y, Float z)
{
  m_axis[0] = x;
  m_axis[1] = y;
  m_axis[2] = z;
  (void) m_axis.normalize();
}

//! \brief obtain the (const) rotation angle.
inline Float Rotation::get_angle() const { return m_angle; }

//! \brief obtain the (non-const) rotation angle.
inline Float& Rotation::get_angle() { return m_angle; }

//! \brief set the rotation axis.
inline void Rotation::set_axis(const Vector3f& axis)
{ set_axis(axis[0], axis[1], axis[2]); }

//! \brief obtains the (const) rotation axis.
inline const Vector3f& Rotation::get_axis() const { return m_axis; }

//! \brief obtain the (non-const) rotation axis.
inline Vector3f& Rotation::get_axis() { return m_axis; }

//! \brief set the rotation.
inline void Rotation::set(const Rotation& r)
{
  m_axis = r.m_axis;
  m_angle = r.m_angle;
}

//! \brief obtain a (const) rotation component.
inline Float Rotation::operator[](size_t i) const
{
  if (i == 3) return m_angle;
  return m_axis[i];
}

//! \brief obtain a (non-const) rotation component.
inline Float& Rotation::operator[](size_t i)
{
  if (i == 3) return m_angle;
  return m_axis[i];
}

//! \brief determines whether the rotation is not equal to another.
inline Boolean Rotation::operator!=(const Rotation& r) const
{ return ((m_axis != r.m_axis) || (m_angle != r.m_angle)); }

//! \brief determines whether the rotation is equal to another.
inline Boolean Rotation::operator==(const Rotation& r) const
{ return ((m_axis == r.m_axis) && (m_angle == r.m_angle)); }

//! \brief exports the rotation.
inline std::ostream& operator<<(std::ostream& os, const Rotation& rot)
{
  os << rot[0] << ", " << rot[1] << ", " << rot[2] << ", " << rot[3];
  return os;
}

//! \brief imports the rotation.
inline std::istream& operator >>(std::istream& in, Rotation& rot)
{
  Float x, y, z, angle;
  in >> x >> y >> z >> angle;
  rot.set(x, y, z, angle);
  return in;
}

SGAL_END_NAMESPACE

#endif
