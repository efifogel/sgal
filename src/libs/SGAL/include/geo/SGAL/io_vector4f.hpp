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

#ifndef SGAL_IO_VECTOR4F_HPP
#define SGAL_IO_VECTOR4F_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Export
 */

template <typename OutputStream>
inline OutputStream& operator<<(OutputStream& os, const Vector4f& vec)
{
  os << vec[0] << "," << vec[1] << "," << vec[2] << "," << vec[3];
  return os;
}

/*! Import
 */
template <typename InputStream>
inline InputStream& operator>>(InputStream& is, Vector4f& vec)
{
  Float x, y, z, w;
  is >> x;
  // This is used boost::lexical_cast among other.
  // boost::lexical_cast does not ignore whitespaces in the input (it unsets
  // the skipws flag of the input stream).
  if ((is.flags() & std::ios_base::skipws) == 0) {
    char whitespace;
    is >> whitespace;
  }
  is >> y;
  if ((is.flags() & std::ios_base::skipws) == 0) {
    char whitespace;
    is >> whitespace;
  }
  is >> z;
  if ((is.flags() & std::ios_base::skipws) == 0) {
    char whitespace;
    is >> whitespace;
  }
  is >> w;
  vec.set(x, y, z, w);
  return is;
}

SGAL_END_NAMESPACE

#endif
