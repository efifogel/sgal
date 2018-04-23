// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IO_MATRIX4F_HPP
#define SGAL_IO_MATRIX4F_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Export
 */
template <typename OutputStream>
inline OutputStream& operator<<(OutputStream& os, const Matrix4f& mat)
{
  os << mat[0][0] << ", " << mat[0][1] << ", "
     << mat[0][2] << ", " << mat[0][3] << std::endl
     << mat[1][0] << ", " << mat[1][1] << ", "
     << mat[1][2] << ", " << mat[1][3] << std::endl
     << mat[2][0] << ", " << mat[2][1] << ", "
     << mat[2][2] << ", " << mat[2][3] << std::endl
     << mat[3][0] << ", " << mat[3][1] << ", "
     << mat[3][2] << ", " << mat[3][3] << std::flush;
  return os;
}

/*! Import
 */
template <typename InputStream>
inline InputStream& operator>>(InputStream& is, Matrix4f& mat)
{
  Float a[16];
  for (size_t i = 0; i < 15; ++i)
    is >> a[i];
  // This is used boost::lexical_cast among other.
  // boost::lexical_cast does not ignore whitespaces in the input (it unsets
  // the skipws flag of the input stream).
  if ((is.flags() & std::ios_base::skipws) == 0) {
    char whitespace;
    is >> whitespace;
  }
  is >> a[15];;
  mat.set(a);
  return is;
}

SGAL_END_NAMESPACE

#endif
