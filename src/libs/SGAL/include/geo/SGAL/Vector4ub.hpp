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
// $Revision: 10789 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR4UB_HPP
#define SGAL_VECTOR4UB_HPP

/*! \file
 *
 */

#include <string>
#include <sstream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Vector4ub {
private:
  Ubyte m_vector[4];

public:
  // Constructors 
  Vector4ub();
  Vector4ub(Ubyte a, Ubyte b, Ubyte c, Ubyte d);
  Vector4ub(const Vector4ub& v);
  Vector4ub(const std::string& text);

public:
  // Set/get

  // Accessor functions
  void set(Ubyte a, Ubyte b, Ubyte c, Ubyte d);
  void get(Ubyte* a, Ubyte* b, Ubyte* c, Ubyte* d) const;
  void set(Int i, Ubyte val);

  Ubyte get(Int i) const;

  void set(const Vector4ub& v);
  void get(Vector4ub& v) const;

  // Math functions
  Boolean equal(const Vector4ub& v) const;

public:
  // Methods
  // Output function
  // void Write(EOutput* out);

public:
  // operators
  // Opertors
  Vector4ub& operator=(const Vector4ub& v);
  Vector4ub& operator=(Ubyte v);
  Ubyte& operator[](int i);
  Ubyte operator[](int i) const;
  Boolean operator==(const Vector4ub& v) const;
  Boolean operator!=(const Vector4ub& v) const;
};

inline Ubyte& Vector4ub::operator[](int i) { return m_vector[i]; }

inline Ubyte Vector4ub::operator[](int i) const { return m_vector[i]; }

inline void Vector4ub::set(Ubyte a, Ubyte b, Ubyte c, Ubyte d) 
{ m_vector[0] = a; m_vector[1] = b; m_vector[2] = c; m_vector[3] = d; }

inline void Vector4ub::get(Ubyte* a, Ubyte* b, Ubyte* c, Ubyte* d) const 
{ *a = m_vector[0]; *b = m_vector[1]; *c = m_vector[2]; *d = m_vector[3]; }

inline void Vector4ub::set(int i, Ubyte val)
{
  if ((i >= 0) && (i < 4)) m_vector[i ] = val;
  // else !!!!!!! Should WARN !!!!!!!
}

inline Ubyte Vector4ub::get(int i) const 
{
  if ((i >= 0) && (i < 4)) return m_vector[i];
    else // !!!!!!!! SHould WARN !!!!!!!!!!
  return 0;
  // else !!!!!!! Should WARN !!!!!!!
}

inline void Vector4ub::set(const Vector4ub& v) 
{
  m_vector[0] = v.m_vector[0]; m_vector[1] = v.m_vector[1]; 
  m_vector[2] = v.m_vector[2]; m_vector[3] = v.m_vector[3];
}

inline void Vector4ub::get(Vector4ub& v) const
{
  v.m_vector[0] = m_vector[0]; v.m_vector[1] = m_vector[1]; 
  v.m_vector[2] = m_vector[2]; v.m_vector[3] = m_vector[3];
}

inline Boolean Vector4ub::equal(const Vector4ub& v) const
{
  return ((m_vector[0] == v.m_vector[0]) && (m_vector[1] == v.m_vector[1]) &&
          (m_vector[2] == v.m_vector[2]) && (m_vector[3] == v.m_vector[3]));
}

inline Vector4ub& Vector4ub::operator=(const Vector4ub& v) 
{
  set(v);
  return *this;
}

inline Vector4ub& Vector4ub::operator=(Ubyte v) 
{
  m_vector[0] = v; m_vector[1] = v; m_vector[2] = v; m_vector[3] = v;
  return *this;
}

inline Boolean Vector4ub::operator==(const Vector4ub& v) const 
{ return equal(v); }

inline Boolean Vector4ub::operator!=(const Vector4ub& v) const 
{ return !equal(v); }

inline Vector4ub::Vector4ub() 
{
}

inline Vector4ub::Vector4ub(Ubyte a, Ubyte b, Ubyte c, Ubyte d) 
{ set(a, b, c, d); }

/*!
 */
inline Vector4ub::Vector4ub(const Vector4ub& v) { set(v); }

/*!
 */
inline Vector4ub::Vector4ub(const std::string& text)
{
  std::istringstream tmp(text, std::istringstream::in);
  unsigned int tmpi;
  tmp >> tmpi; m_vector[0] = tmpi;
  tmp >> tmpi; m_vector[1] = tmpi;
  tmp >> tmpi; m_vector[2] = tmpi;
  tmp >> tmpi; m_vector[3] = tmpi;
}

/*!
 */
inline std::ostream& operator<<(std::ostream& os, const Vector4ub & vec)
{
  os << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3];
  return os;
}  

SGAL_END_NAMESPACE

#endif
