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
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief */
std::string Vector4f::get_text() 
{
  char buf[64];
  sprintf(buf, "%g %g %g %g",
          m_vector[0], m_vector[1], m_vector[2], m_vector[3]);
  std::string str(buf);
  return str;
}

/*! \brief sets this vector to be v * m where v is treated as a row vector. */
void Vector4f::xform(const Vector4f& v, const Matrix4f& m)
{
  Vector4f t;
  t.set(v);
  m_vector[0] = t[0]*m[0][0] + t[1]*m[1][0] + t[2]*m[2][0] + t[3]*m[3][0];
  m_vector[1] = t[0]*m[0][1] + t[1]*m[1][1] + t[2]*m[2][1] + t[3]*m[3][1];
  m_vector[2] = t[0]*m[0][2] + t[1]*m[1][2] + t[2]*m[2][2] + t[3]*m[3][2];
  m_vector[3] = t[0]*m[0][3] + t[1]*m[1][3] + t[2]*m[2][3] + t[3]*m[3][3];
}

/*! \brief computes the length of the vector.
 * DO NOT MAKE INLINE! (Do not move to Vector4f.hp.) to avoid inclusion
 * of Math_depth.hpp in Vector4f.hpp. 
 */
float Vector4f::length() const
{ return squarerootf(dot(*this)); }

SGAL_END_NAMESPACE
