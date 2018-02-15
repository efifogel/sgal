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

#ifndef SGAL_VECTOR3F_HPP
#define SGAL_VECTOR3F_HPP

/*! \file
 */

#include <iostream>
#include <sstream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector3sh.hpp"

SGAL_BEGIN_NAMESPACE

class Matrix4f;

class SGAL_SGAL_DECL Vector3f {
private:
  Float m_vector[3];

public:
  /*! Construct default. */
  Vector3f();

  /*! Construct from elements. */
  Vector3f(Float x, Float y, Float z);

  /*! Construct from string. */
  Vector3f (const std::string& str);

  /*! Construct copy. */
  Vector3f(const Vector3f& other);

  /*! Initialize the vector. */
  void init();

  void set(Float x, Float y, Float z);
  void get(Float& x, Float& y, Float& z)  const;
  void set(int i, Float f);
  Float get(int i) const;
  void set(const Vector3f& v);
  void get(Vector3f& v) const;
  std::string get_text();

  Float x() const;
  Float y() const;
  Float z() const;

  Float max();
  Float min();
  Boolean equal(const Vector3f&  v) const;
  void sub(const Vector3f& v1, const Vector3f& v2);
  void sub(const Vector3sh& v1, const Vector3sh& v2);
  void sub(const Vector3f& v);
  void cross(const Vector3f&  v1, const Vector3f&  v2);
  void scale(Float s, const Vector3f& v);
  void scale(Float s);
  void add(const Vector3f& v1, const Vector3f& v2);
  void add(const Vector3f& v1);
  void add(const Vector3sh& v1);
  Float normalize();
  void normalize1();
  void negate();
  void negate(const Vector3f& v);
  Float dot(const Vector3f&  v) const;
  void xform_vec(const Vector3f& v, const Matrix4f& m);
  Float length() const;
  Float length_reciprocal() const;
  void add_scaled(const Vector3f& v1, Float s, const Vector3f& v2);
  void add_scaled(Float s, const Vector3f& v2);
  void mul(const Vector3f& v1, const Vector3f& v2);
  void mul(const Vector3sh& v1, const Vector3f& v2);
  Float sqr_distance(const Vector3f& v) const;
  Float distance(const Vector3f& v) const;
  void xform_pt(const Vector3f& v, const Matrix4f& m);
  void div(Float, const Vector3f& v);
  void div(const Vector3f& v1, const Vector3f& v2);
  void round();

  Boolean less(const Vector3f& v) const;
  Boolean almost_equal(const Vector3f& v, Float tol) const;
  void combine(Float a, const Vector3f& v1, Float b, const Vector3f& v2);
  void full_xform_pt(const Vector3f& v, const Matrix4f& m);
  void clamp(const Vector3f& v, const Vector3f& my_min, const Vector3f& my_max);

  Vector3f& operator=(const Vector3f& v);
  Vector3f& operator=(Float v);
  Float& operator[](int i);
  Float operator[](int i) const;
  Boolean operator==(const Vector3f& v) const;
  Boolean operator!=(const Vector3f& v) const;

  /*! Compute the normal to a plane given by three points.
   * \param p1 (in) the first point.
   * \param p2 (in) the second point.
   * \param p3 (in) the third point.
   */
  void normal(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);

  /*! Determine whether three given points are collinear.
   * \param p1 (in) the first point.
   * \param p1 (in) the second point.
   * \param p1 (in) the third point.
   */
  static Boolean collinear(const Vector3f& p1, const Vector3f& p2,
                           const Vector3f& p3);
};

//! \brief obtains the first coordinate.
inline Float Vector3f::x() const { return m_vector[0]; }

//! \brief obtains the second coordinate.
inline Float Vector3f::y() const { return m_vector[1]; }

//! \brief obtains the third coordinate.
inline Float Vector3f::z() const { return m_vector[2]; }

//! \brief
inline Float& Vector3f::operator[](int i) { return m_vector[i]; }

//! \brief
inline Float Vector3f::operator[](int i) const { return m_vector[i]; }

//! \brief
inline void Vector3f::set(Float x, Float y, Float z)
{
  m_vector[0] = x;
  m_vector[1] = y;
  m_vector[2] = z;
}

//! \brief
inline void Vector3f::get(Float& x, Float& y, Float& z)  const
{
  x = m_vector[0];
  y = m_vector[1];
  z = m_vector[2];
}

//! \brief
inline void Vector3f::set(int i, Float val) { m_vector[i] = val; }

//! \brief
inline Float Vector3f::get(int i) const { return m_vector[i]; }

//! \brief
inline void Vector3f::set(const Vector3f& v)
{
  m_vector[0] = v.m_vector[0];
  m_vector[1] = v.m_vector[1];
  m_vector[2] = v.m_vector[2];
}

//! \brief
inline void Vector3f::get(Vector3f& v) const
{
  v.m_vector[0] = m_vector[0];
  v.m_vector[1] = m_vector[1];
  v.m_vector[2] = m_vector[2];
}

//! \brief sets this vector to be v1 - v2.
inline void Vector3f::init() { m_vector[0] = m_vector[1] = m_vector[2] = 0.0f; }

//! \brief default constrcutor.
inline Vector3f::Vector3f() { init(); }

//! \brief constrcutor.
inline Vector3f::Vector3f(Float a, Float b, Float c) { set(a, b, c); }

//! \brief copy constrcutor.
inline Vector3f::Vector3f(const Vector3f& other) { set(other); }

//! \brief sets this vector to be v1 - v2.
inline void Vector3f::sub(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] - v2[0];
  m_vector[1] = v1[1] - v2[1];
  m_vector[2] = v1[2] - v2[2];
}

//! \brief sets this vector to be v1 - v2.
inline void Vector3f::sub(const Vector3sh& v1, const Vector3sh& v2)
{
  m_vector[0] = (Float) v1[0] - (Float) v2[0];
  m_vector[1] = (Float) v1[1] - (Float) v2[1];
  m_vector[2] = (Float) v1[2] - (Float) v2[2];
}

//! \brief sets this vector to be this - v.
inline void Vector3f::sub(const Vector3f& v)
{
  m_vector[0] -= v[0];
  m_vector[1] -= v[1];
  m_vector[2] -= v[2];
}

//! \brief
inline Boolean Vector3f::equal(const Vector3f& v) const
{
  return((m_vector[0] == v[0]) && (m_vector[1] == v[1]) &&
         (m_vector[2] == v[2]));
}

//! \brief
inline Vector3f& Vector3f::operator=(const Vector3f& v)
{
  set(v);
  return *this;
}

//! \brief
inline Vector3f& Vector3f::operator=(Float v)
{
  m_vector[0] = v;
  m_vector[1] = v;
  m_vector[2] = v;
  return *this;
}

//! \brief
inline Boolean Vector3f::operator==(const Vector3f& v) const { return equal(v); }

//! \brief
inline Boolean Vector3f::operator!=(const Vector3f& v) const { return !equal(v); }

//! \brief
inline void Vector3f::scale(Float s, const Vector3f& v)
{
  m_vector[0] = s * v[0];
  m_vector[1] = s * v[1];
  m_vector[2] = s * v[2];
}

//! \brief
inline void Vector3f::scale(Float s)
{
  m_vector[0] *= s;
  m_vector[1] *= s;
  m_vector[2] *= s;
}

//! \brief
inline void Vector3f::add(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] + v2[0];
  m_vector[1] = v1[1] + v2[1];
  m_vector[2] = v1[2] + v2[2];
}

//! \brief
inline void Vector3f::add(const Vector3f& v1)
{
  m_vector[0] += v1[0] ;
  m_vector[1] += v1[1] ;
  m_vector[2] += v1[2] ;
}

//! \brief
inline void Vector3f::add(const Vector3sh& v1)
{
  m_vector[0] += v1[0] ;
  m_vector[1] += v1[1] ;
  m_vector[2] += v1[2] ;
}

//! \brief
inline void Vector3f::negate()
{
  m_vector[0] = -m_vector[0];
  m_vector[1] = -m_vector[1];
  m_vector[2] = -m_vector[2];
}

//! \brief
inline void Vector3f::negate(const Vector3f& v)
{
  m_vector[0] = -v[0];
  m_vector[1] = -v[1];
  m_vector[2] = -v[2];
}

/*! \brief obtains the dot product of this vector with v. */
inline Float Vector3f::dot(const Vector3f& v) const
{ return(m_vector[0] * v[0] + m_vector[1] * v[1] + m_vector[2] * v[2]); }

//! \brief
inline void Vector3f::add_scaled(const Vector3f& v1, Float s,
                                 const Vector3f& v2)
{
  m_vector[0] = v1[0] + s * v2[0];
  m_vector[1] = v1[1] + s * v2[1];
  m_vector[2] = v1[2] + s * v2[2];
}

//! \brief
inline void Vector3f::add_scaled(Float s, const Vector3f& v2)
{
  m_vector[0] = m_vector[0] + s * v2[0];
  m_vector[1] = m_vector[1] + s * v2[1];
  m_vector[2] = m_vector[2] + s * v2[2];
}

//! \brief
inline Float Vector3f::normalize()
{
  Float my_length = length();
  if (my_length == 0.0f) return my_length;
  Float length_reciprocal = 1.0f / my_length;
  m_vector[0] *= length_reciprocal;
  m_vector[1] *= length_reciprocal;
  m_vector[2] *= length_reciprocal;
  return my_length;
}

//! \brief
inline void Vector3f::mul(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
  m_vector[2] = v1[2] * v2[2];
}

//! \brief
inline void Vector3f::mul(const Vector3sh& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
  m_vector[2] = v1[2] * v2[2];
}

//! \brief
inline Float Vector3f::sqr_distance(const Vector3f& v) const
{
  Float d0, d1, d2;
  d0 = m_vector[0] - v.m_vector[0];
  d1 = m_vector[1] - v.m_vector[1];
  d2 = m_vector[2] - v.m_vector[2];
  return d0 * d0 + d1 * d1 + d2 * d2;
}

//! \brief
inline void Vector3f::div(Float c, const Vector3f& v)
{
  m_vector[0] = c / v[0];
  m_vector[1] = c / v[1];
  m_vector[2] = c / v[2];
}

//! \brief
inline void Vector3f::div(const Vector3f& v1, const Vector3f& v2)
{
  m_vector[0] = v1[0] / v2[0];
  m_vector[1] = v1[1] / v2[1];
  m_vector[2] = v1[2] / v2[2];
}

//! \brief
inline void Vector3f::round()
{
  m_vector[0] = (Float) ((int)m_vector[0]);
  m_vector[1] = (Float) ((int)m_vector[1]);
  m_vector[2] = (Float) ((int)m_vector[2]);
}

//! \brief
inline Boolean Vector3f::less(const Vector3f& v) const
{
  return ((m_vector[0] < v[0]) ? true :
          ((m_vector[0] > v[0]) ? false :
           ((m_vector[1] < v[1]) ? true :
            ((m_vector[1] > v[1]) ? false :
             ((m_vector[2] < v[2]) ? true : false)))));
}

//! \brief
inline Boolean Vector3f::almost_equal(const Vector3f& v, Float tol) const
{
  Float neg_tol = -tol;
  Float diff = m_vector[0] - v[0];
  if ((diff < neg_tol) || (tol < diff)) return false;
  diff = m_vector[1] - v[1];
  if ((diff < neg_tol) || (tol < diff)) return false;
  diff = m_vector[2] - v[2];
  if ((diff < neg_tol) || (tol < diff)) return false;
  return true;
}

//! \brief
inline void Vector3f::combine(Float a, const Vector3f& v1, Float b,
                              const Vector3f& v2)
{
  m_vector[0] = a * v1[0] + b * v2[0];
  m_vector[1] = a * v1[1] + b * v2[1];
  m_vector[2] = a * v1[2] + b * v2[2];
}

Boolean operator>(const Vector3f& v1, const Vector3f& v2);
Boolean operator<(const Vector3f& v1, const Vector3f& v2);

//! \brief
inline Boolean operator<(const Vector3f& v1, const Vector3f& v2)
{ return ((v1[0] < v2[0]) && (v1[1] < v2[1]) && (v1[2] < v2[2])); }

//! \brief
inline Boolean operator>(const Vector3f& v1, const Vector3f& v2)
{ return ((v1[0] > v2[0]) && (v1[1] > v2[1]) && (v1[2] > v2[2])); }

#if defined(SOLARIS_251)

Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
Vector3f operator*(const Float f, const Vector3f& v);
Vector3f operator*(const Vector3f& v, const Float f);

//! \brief
inline Vector3f operator+(const Vector3f& v1, const Vector3f& v2)
{
  Vector3f* c = new Vector3f;
  c->set(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
  return(*c);
}

//! \brief
inline Vector3f operator*(const Vector3f v, const Float f)
{
  Vector3f* c = new Vector3f;
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

//! \brief
inline Vector3f operator*(const Float f, const Vector3f v)
{
  Vector3f* c = new Vector3f;
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

#endif

//! \brief exporter.
inline std::ostream& operator<<(std::ostream& os, const Vector3f& vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2];
  return os;
}

//! \brief importer.
inline std::istream& operator>>(std::istream& in, Vector3f& vec)
{
  Float x, y, z;
  in >> x >> y >> z;
  vec.set(x, y, z);
  return in;
}

SGAL_END_NAMESPACE

#endif
