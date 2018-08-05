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

#ifndef SGAL_ORIENTATIO_HPP
#define SGAL_ORIENTATIO_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Enumerations of orientation of three consecutive points.
 */
enum class Orientation {
  Right_turn = -1,
  Collinear = 0,
  Left_turn = 1,

  Clockwise = -1,
  Counterclockwise = 1
};

/*! Determine the orientation of three consecutive points.
 * \param[in] a the first point.
 * \param[in] b the second point.
 * \param[in] c the third point.
 * \return the orientation of three consecutive points.
 */
template <typename Vector>
inline Orientation orientation(const Vector& a, const Vector& b, const Vector& c)
{
  auto v0 = b[0] - a[0];
  auto v1 = b[1] - a[1];
  auto u0 = c[0] - b[0];
  auto u1 = c[1] - b[1];
  auto x = v0 * u1 - v1 * u0;
  return (x < 0) ? Orientation::Right_turn :
    ((x == 0) ? Orientation::Collinear : Orientation::Left_turn);
}

/*! Obtain the orientation of a simple polygon.
 * \param[in] begin the being iterator of the vertices of the polygon.
 * \param[in] end the past-the-end iterator of the vertices of the polygon.
 * \return the orientation.
 */
template <typename InputIterator>
inline Orientation orientation(InputIterator begin, InputIterator end)
{
  auto it = begin;
  auto left = it;
  for (++it; it < end; ++it) {
    if (((*it)[0] < (*left)[0]) ||
        (((*it)[0] == (*left)[0]) && ((*it)[1] < (*left)[1])))
      left = it;
  }
  Orientation orient;
  if (left == begin)
    return orientation(*(std::prev(end)), *left, *(std::next(left)));
  else if (left == std::prev(end))
    return orientation(*(std::prev(left)), *left, *begin);
  else return orientation(*(std::prev(left)), *left, *(std::next(left)));
}

SGAL_END_NAMESPACE

#endif
