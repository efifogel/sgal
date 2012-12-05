// Copyright (c) 2012 Israel.
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
// $Source: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR2U_HPP
#define SGAL_VECTOR2U_HPP

/*! \file
 *
 */

#include <string>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Vector2sh.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Vector2u {
private:
  Uint m_vector[2];

public:
  Vector2u();
  Vector2u(Uint a, Uint b);
  Vector2u(const Vector2u& v);
  Vector2u(const std::string& text);

  void set(Uint a, Uint b);
  void get(Uint * a, Uint * b) const;
  Uint get(int i) const;
  void set(const Vector2u& v);
  void get(Vector2u& v) const;
  void add(const Vector2u& v1);
  void add(const Vector2sh& v1);
  void mul(const Vector2u& v1, const Vector2u& v2);
  void mul(const Vector2sh& v1, const Vector2u& v2);
  bool equal(const Vector2u&  v) const;
  void sub(const Vector2u& v);
    
  Uint dot(const Vector2u&  v) const;
  void add(const Vector2u& v1, const Vector2u& v2);
  void sub(const Vector2u& v1, const Vector2u& v2);

  Vector2u& operator=(const Vector2u& v);
  Vector2u& operator=(Uint v);
  Uint& operator[](Uint i);
  Uint operator[](Uint i) const;
  bool operator==(const Vector2u &v) const;
  bool operator!=(const Vector2u &v) const;
};

/*!
 */
inline void Vector2u::mul(const Vector2u& v1, const Vector2u& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
}

/*!
 */
inline void Vector2u::mul(const Vector2sh& v1, const Vector2u& v2)
{
  m_vector[0] = v1[0] * v2[0];
  m_vector[1] = v1[1] * v2[1];
}

/*!
 */
inline Vector2u::Vector2u(const std::string& text)
{
  std::istringstream tmp(text, std::istringstream::in);
  tmp >> m_vector[0];
  tmp >> m_vector[1];
}

#if defined(SOLARIS_251)

/*!
 */
Vector2u operator+(const Vector2u& _v1, const Vector2u& _v2);

/*!
 */
Vector2u operator*(const Uint f, const Vector2u& v);

/*!
 */
Vector2u operator*(const Vector2u& v, const Uint f);

/*!
 */
bool operator>(const Vector2u& _v1, const Vector2u& _v2);

/*!
 */
bool operator<(const Vector2u& _v1, const Vector2u& _v2);

/*!
 */
inline bool operator<(const Vector2u& _v1, const Vector2u& _v2)
{
  return (_v1[0] < _v2[0] && _v1[1] < _v2[1]);
}

/*!
 */
inline bool operator>(const Vector2u& _v1, const Vector2u& _v2)
{
  return (_v1[0] > _v2[0] && _v1[1] > _v2[1]);
}

/*!
 */
inline Vector2u operator+(const Vector2u& _v1, const Vector2u& _v2)
{                                 
  Vector2u *c = new Vector2u;     
  c->set(_v1[0] + _v2[0], _v1[1] + _v2[1]);
  return(*c);                   
}                                 
                                  
/*!
 */
inline Vector2u operator*(const Vector2u v, const Uint f)
{
  Vector2u *c = new Vector2u;     
  c->set(v[0] * f, v[1] + f);   
  return(*c);                   
}

/*!
 */
inline Vector2u operator*(const Uint f, const Vector2u v)
{
  Vector2u *c = new Vector2u;
  c->set(v[0] * f, v[1] + f);
  return(*c);
}

#endif

/*!
 */
inline std::ostream& operator<<(std::ostream& os, const Vector2u& vec)
{
  os << vec[0] << ", " << vec[1];
  return os;
}  

SGAL_END_NAMESPACE

#endif

