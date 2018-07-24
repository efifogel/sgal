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

#ifndef SGAL_IS_CONVEX_HPP
#define SGAL_IS_CONVEX_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Orientation.hpp"

SGAL_BEGIN_NAMESPACE

template <typename InputIterator>
bool is_convex(InputIterator begin, InputIterator end)
{
  auto it = begin;
  if (it == end) return true;   // 0 vertices
  auto it1 = it++;
  if (it == end) return true;   // 1 vertex
  auto it2 = it++;
  if (it == end) return true;   // 2 vertices
  while (it != end)
    if (orientation(*it1++, *it2++, *it++) == Orientation::Right_turn)
      return false;
  it = begin;
  if (orientation(*it1++, *it2, *it++) == Orientation::Right_turn) return false;
  if (orientation(*it1, *begin, *it) == Orientation::Right_turn) return false;

  return true;
}

SGAL_END_NAMESPACE

#endif
