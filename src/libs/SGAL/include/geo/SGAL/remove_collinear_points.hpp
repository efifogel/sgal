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

/*! Simplify a polyline removing duplicated and collinear points.
 * \param[in] begin the begin iterator of of input points.
 * \param[in] end the past-the-end iterator of of input points.
 * \param[out] the iterator of the output sequence.
 * \param closed[in] indicates whether the sequence of points represents a close
 *                   loop (a polygon).
 * \post If the input sequence is empty, then the output sequence is empty;
 *       else, if the input sequence consists of one point p duplicated zero or
 *         more times, then the output sequence consists of p;
 *       else, the output sequence consists of at least two (non-equal) points.
 */
template <typename InputIterator, typename OutputIterator>
void remove_collinear_points(InputIterator begin, InputIterator end,
                             bool closed, OutputIterator out)
{
  auto it2 = begin;
  if (it2 == end) return;

  auto it3 = std::next(it2);
  if (it3 == end) return;

  if (closed) {
    auto it1 = std::prev(end);
    while (Orientation::Collinear == orientation(*it1, *it2, *it3)) {
      if (it3 == it1) break;
      it2 = it3;
      ++it3;
    }
  }
  *out++ = *it2;
  auto it1 = it2;
  it2 = it3;
  ++it3;

  while (it3 != end) {
    if (Orientation::Collinear != orientation(*it1, *it2, *it3)) {
      *out++ = *it2;
      it1 = it2;
    }
    it2 = it3;
    ++it3;
  }

  if (*it1 != *it2) *out++ = *it2;
}

/*! Simplify a polyline removing duplicated and collinear points in-place.
 * \param[in,out] points the sequence of ordered points.
 * \param closed[in] indicates whether the sequence of points represents a close
 *                   loop (a polygon).
 * \post If the input sequence is empty, then the output sequence is empty;
 *       else, if the input sequence consists of one point p duplicated zero or
 *         more times, then the output sequence consists of p;
 *       else, the output sequence consists of at least two (non-equal) points.
 * The test for collineraity subsumes the test for point duplication, except for
 * at a special case when closed is false and the last two points of the
 * sequence are equal.
 */
template <typename Points>
inline void remove_collinear_points(Points& points, bool closed)
{
  if (3 > points.size()) return;
  auto it1 = points.begin();
  auto it2 = std::next(it1);
  auto it3 = std::next(it2);
  while (it3 != points.end()) {
    if (Orientation::Collinear == orientation(*it1, *it2, *it3)) {
      points.erase(it2);
      // If it2 is erased, it2 and it3 become invalid and must be recomputed.
      it2 = std::next(it1);
      it3 = std::next(it2);
      continue;
    }

    // Advance all 3 iterators.
    it1 = it2;
    it2 = it3;
    ++it3;
  }

  if (! closed) {
    if (*it1 == *it2) points.erase(it2);
    return;
  }

  // Test end conditions.
  it2 = std::prev(points.end());
  it1 = std::prev(it2);
  it3 = points.begin();
  if (it1 == it3) return;
  if (Orientation::Collinear == orientation(*it1, *it2, *it3))
    points.erase(it2);

  it1 = std::prev(points.end());
  it2 = points.begin();
  it3 = std::next(it2);
  if (it1 == it3) return;
  if (Orientation::Collinear == orientation(*it1, *it2, *it3))
    points.erase(it2);
}

/*! Simplify a polyline removing duplicated points and collinear points.
 * \param[in] begin the begin iterator of of input points.
 * \param[in] end the past-the-end iterator of of input points.
 * \param[out] the iterator of the output sequence.
 * \param closed[in] indicates whether the sequence of points represents a close
 *                   loop (a polygon).
 * \post If the input sequence is empty, then the output sequence is empty;
 *       else, if the input sequence consists of one point p duplicated zero or
 *         more times, then the output sequence consists of p;
 *       else, the output sequence consists of at least two (non-equal) points.
 */
template <typename PointInputIterator, typename BulgeInputIteratyor,
          typename PointOutputIterator, typename BulgeOutputIterator>
void remove_collinear_points(PointInputIterator points_begin,
                             PointInputIterator points_end,
                             BulgeInputIteratyor bulges_begin,
                             BulgeInputIteratyor bulges_end,
                             bool closed,
                             PointOutputIterator points_out,
                             BulgeOutputIterator bulges_out)
{
  auto it2 = points_begin;
  auto bit2 = bulges_begin;
  if (it2 == points_end) return;

  auto it3 = std::next(it2);
  if (it3 == points_end) return;

  if (closed) {
    auto it1 = std::prev(points_end);
    auto bit1 = std::prev(bulges_end);
    while (((*bit1 != 0.0) && (*bit2 != 0.0) &&
           (Orientation::Collinear == orientation(*it1, *it2, *it3))) ||
           (*it1 == *it2))
    {
      if (it3 == it1) break;
      it2 = it3;
      ++it3;
      ++bit2;
    }
  }
  *points_out++ = *it2;
  auto it1 = it2;
  it2 = it3;
  ++it3;
  *bulges_out++ = *bit2;
  auto bit1 = bit2;
  ++bit2;

  while (it3 != points_end) {
    if (((*bit1 == 0.0) || (*bit2 == 0.0) ||
         (Orientation::Collinear != orientation(*it1, *it2, *it3))) &&
        (*it1 != *it2))
      {
      *points_out++ = *it2;
      it1 = it2;
      *bulges_out++ = *bit2;
      bit1 = bit2;
    }
    it2 = it3;
    ++it3;
    ++bit2;
  }

  if (*it1 != *it2) {
    *points_out++ = *it2;
    *bulges_out++ = *bit2;
  }
}

SGAL_END_NAMESPACE

#endif
