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

#ifndef SGAL_VECTOR2F_HPP
#define SGAL_VECTOR2F_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Vector2sh.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Vector2f {
private:
  Float m_vector[2];

public:
  Vector2f();
  Vector2f(Float a, Float b);
  Vector2f(const Vector2f& v);

  void set(Float a, Float b);
  void get(Float& a, Float& b) const;
  Float get(int i) const;
  void set(const Vector2f& v);
  void get(Vector2f& v) const;
  void add(const Vector2f& v1);
  void add(const Vector2sh& v1);
  void scale(Float s);
  void scale(Float s, const Vector2f& v);
  void mul(const Vector2f& v1, const Vector2f& v2);
  void mul(const Vector2sh& v1, const Vector2f& v2);
  bool equal(const Vector2f&  v) const;
  void sub(const Vector2f& v);
  void div(Float c, const Vector2f& v);
  void round();

  bool almost_equal(const Vector2f& v, Float tol) const;
  void negate(const Vector2f& v);
  Float dot(const Vector2f&  v) const;
  void add(const Vector2f& v1, const Vector2f& v2);
  void sub(const Vector2f& v1, const Vector2f& v2);
  void add_scaled(const Vector2f& v1, Float s, const Vector2f& v2);
  void combine(Float a, const Vector2f& v1, Float b, const Vector2f& v2);
  void blend(Float a, const Vector2f& v1, const Vector2f& v2);

  Float sqr_distance(const Vector2f& v) const;
  Float normalize();
  Float sqr_length() const;
  Float length() const;
  Float distance(const Vector2f& v) const;

  Vector2f& operator=(const Vector2f& v);
  Vector2f& operator=(Float v);
  Float& operator[](int i);
  Float operator[](int i) const;
  bool operator==(const Vector2f &v) const;
  bool operator!=(const Vector2f &v) const;
};

//! \brief constructs default.
inline Vector2f::Vector2f() { m_vector[0] = m_vector[1] = 0.0f; }

//! \brief constructs.
inline Vector2f::Vector2f(Float a, Float b) { set(a, b); }

//! \brief copy constructs.
inline Vector2f::Vector2f(const Vector2f& v) { set(v[0], v[1]); }

//! \brief obtains the (non-const) ith element.
inline Float& Vector2f::operator[](int i) { return m_vector[i]; }

//! \brief obtains the (const) ith element.
inline Float Vector2f::operator[](int i) const { return m_vector[i]; }

//! \brief multiplies two vectors.
inline void Vector2f::mul(const Vector2f& v1, const Vector2f& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
}

//! \brief multiplies two vectors.
inline void Vector2f::mul(const Vector2sh& v1, const Vector2f& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
}

//! \brief blends two vectors.
inline void Vector2f::blend(Float a, const Vector2f& v1, const Vector2f& v2)
{ combine(a, v1, 1.0f-a, v2); }

//! \brief computes the square of the length of the vector.
inline Float Vector2f::sqr_length() const { return dot(*this); }

#if defined(SOLARIS_251)

//!
Vector2f operator+(const Vector2f& _v1, const Vector2f& _v2);

//!
Vector2f operator*(const Float f, const Vector2f& v);

//!
Vector2f operator*(const Vector2f& v, const Float f);

//!
bool operator>(const Vector2f& v1, const Vector2f& v2);

//!
bool operator<(const Vector2f& v1, const Vector2f& v2);

//!
inline bool operator<(const Vector2f& v1, const Vector2f& v2)
{ return (v1[0] < v2[0] && v1[1] < v2[1]); }

//!
inline bool operator>(const Vector2f& v1, const Vector2f& v2)
{ return (v1[0] > v2[0] && v1[1] > v2[1]); }

//!
inline Vector2f operator+(const Vector2f& v1, const Vector2f& v2)
{
  Vector2f *c = new Vector2f;
  c->set(v1[0] + v2[0], v1[1] + v2[1]);
  return(*c);
}

//!
inline Vector2f operator*(const Vector2f v, const Float f)
{
  Vector2f* c = new Vector2f;
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

//!
inline Vector2f operator*(const Float f, const Vector2f v)
{
  Vector2f* c = new Vector2f;
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

#endif

SGAL_END_NAMESPACE

#endif
