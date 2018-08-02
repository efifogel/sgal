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

#ifndef SGAL_REMOVE_COLLINEAR_POINTS_HPP
#define SGAL_REMOVE_COLLINEAR_POINTS_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Orientation.hpp"

SGAL_BEGIN_NAMESPACE

/*! Simplify a simple polygon removing collinear points.
 * \param[in,out] points the sequence of ordered points.
 * \param closed[in] indicates whether the sequence of points represents a close
 *                   loop (a polygon).
 */
template <typename Points>
inline void remove_collinear_points(Points& points, bool closed = true)
{
  if (3 > points.size()) return;
  auto it1 = points.begin();
  auto it2 = std::next(it1);
  auto it3 = std::next(it2);
  while (it3 != points.end()) {

    if (Orientation::Collinear == orientation(*it1, *it2, *it3)) {
      points.erase(it2);
      // If it2 is erased. it2 and it3 become invalid and must be recomputed.
      it2 = std::next(it1);
      it3 = std::next(it2);
      continue;
    }

    // Advance all 3 iterators.
    it1 = it2;
    it2 = it3;
    ++it3;
  }

  if (! closed) return;

  // Test end conditions.
  it2 = std::prev(points.end());
  it1 = std::prev(it2);
  it3 = points.begin();
  if (Orientation::Collinear == orientation(*it1, *it2, *it3))
    points.erase(it2);

  it1 = std::prev(points.end());
  it2 = points.begin();
  it3 = std::next(it2);
  if (Orientation::Collinear == orientation(*it1, *it2, *it3))
    points.erase(it2);
}

SGAL_END_NAMESPACE

#endif
