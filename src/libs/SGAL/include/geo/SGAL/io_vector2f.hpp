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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IO_VECTOR2F_HPP
#define SGAL_IO_VECTOR2F_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Export
 */

template <typename OutputStream>
inline OutputStream& operator<<(OutputStream& os, const Vector2f& vec)
{
  os << vec[0] << ", " << vec[1];
  return os;
}

/*! Import
 */
template <typename InputStream>
inline InputStream& operator>>(InputStream& is, SGAL::Vector2f& vec)
{
  Float x, y;
  x << is;
  y << is;
  vec.set(x, y);
  return is;
}

SGAL_END_NAMESPACE

#endif
