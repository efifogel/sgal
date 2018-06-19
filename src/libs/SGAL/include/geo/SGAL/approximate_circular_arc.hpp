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

#ifndef SGAL_APPROXIMATE_CIRCULAR_ARC_HPP
#define SGAL_APPROXIMATE_CIRCULAR_ARC_HPP

#include <math.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/io_vector2f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Approximate a circular arc by a sequence of line segments represented as a
 * sequence of points.
 * The input circular arc is given by the endpoints of the arc and a bulge
 * factor. The resulting sequence of points do not include the endpoints.
 */
template <typename OutputIterator>
OutputIterator approximate_circular_arc(const Vector2f& v1, const Vector2f& v2,
                                        float bulge, float min_bulge,
                                        OutputIterator oi)
{
  auto bulge_abs = fabs(bulge);
  if (min_bulge >= bulge_abs) return oi;

  Vector2f d;
  d.sub(v2, v1);
  auto d2 = 0.5f * d.length();
  auto d1 = bulge * d2;
  auto radius = d2 * (1 + bulge * bulge) / (2.0f * bulge);
  // std::cout << "radius: " << radius << std::endl;
  return approximate_circular_arc(v1, v2, bulge, min_bulge, radius, oi);
}

/*! Approximate a circular arc by a sequence of line segments represented as a
 * sequence of points.
 * The input circular arc is given by the endpoints of the arc and a bulge
 * factor. The resulting sequence of points do not include the endpoints.
 */
template <typename OutputIterator>
OutputIterator approximate_circular_arc(const Vector2f& v1, const Vector2f& v2,
                                        float bulge, float min_bulge,
                                        float radius, OutputIterator oi)
{
  auto bulge_abs = fabs(bulge);
  if (min_bulge >= bulge_abs) return oi;
  // if (bulge_abs > 1) return oi;

  Vector2f d;
  d.sub(v2, v1);
  auto dist = d.length();
  // std::cout << "  dist: " << dist << std::endl;

  // Compute the point at the midle of the circular arc:
  Vector2f m;                 // mid point
  m.blend(0.5f, v1, v2);
  Vector2f v;                 // vector relative to new point
  auto c(0.5f * bulge);
  v.set(c * d[1], -c * d[0]); // compute orthogonal
  Vector2f p(m);
  p.add(v);

  // Compute the next bulge:
  auto d2_next =  0.25f * dist * squarerootf(bulge * bulge + 1);
  auto h_next = d2_next / bulge;
  auto d1_next = radius - h_next;
  auto bulge_next = d1_next / d2_next;
  if (bulge_abs < fabs(bulge_next)) return oi;
  bulge = bulge_next;

  // approximate the arc:
  oi = approximate_circular_arc(v1, p, bulge, min_bulge, radius, oi);
  *oi++ = p;
  oi = approximate_circular_arc(p, v2, bulge, min_bulge, radius, oi);

  return oi;
}

SGAL_END_NAMESPACE

#endif
