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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Types.hpp"
#include "SGAL/basic.hpp"
#include "SGAL/assertions.hpp"
#include "SGAL/Plane.hpp"
#include "SGAL/Line.hpp"
#include "SGAL/Math_defs.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief sets the plane with the content of another plane. */
void Plane::set(const Plane& plane)
{
  m_normal.set(plane.get_normal());
  m_offset = get_offset();
}

/*! \brief */
float Plane::contains(const Box_bound* box) const
{
  (void) box;
  SGAL_assertion(0);
  return 0;
}

/* \brief tests whether a given point is on the positive side of the plane */
float Plane::contains(const Vector3f& pt) const
{ return (pt.dot(m_normal) - m_offset); }

/*! \brief */
float Plane::contains(const Sphere_bound*) const
{
  SGAL_assertion(0);
  return 0;
}

/*! \brief */
Uint Plane::isect(const Seg*, Float*) const
{
  SGAL_assertion(0);
  return 0;
}

/*! \brief */
Uint Plane::isect(const Seg *, Float *, float *) const
{
  SGAL_assertion(0);
  return 0;
}

/*! \brief stores in this half space the result of transforming <pln> by the
 * ortho transform <m>.
 */
void Plane::ortho_xform(const Plane*, const Matrix4f&)
{
  SGAL_assertion(0);
}

/*! \brief constructs a plane from the three points <p1> <p2>, and <p3>,
 * assuming they form a counter-clockwise triangle viewed from the front.
 */
void Plane::make_pts(const Vector3f& p1, const Vector3f& p2,
                     const Vector3f& p3)
{
  Vector3f t1;
  Vector3f t2;
  t1.sub(p2,p1);
  t2.sub(p3,p2);
  m_normal.cross(t1,t2);
  m_normal.normalize();
  m_offset = p2.dot(m_normal);
}

/*! \brief stores the normal-point representation of the plane in norm and point.
 */
void Plane::make_norm_pt(const Vector3f& norm, const Vector3f& pos)
{
  m_normal.set(norm);
  m_normal.normalize();
  m_offset = pos.dot(m_normal);
}

/*! \brief stores in <dst> the point on this plane closest to <pt>. */
void Plane::closest_pt_on(const Vector3f& pt, Vector3f& dst) const
{
  float scale = -m_normal.dot(pt) - m_offset;
  dst.add_scaled(pt, scale, m_normal);
}

/*! \brief computes the intersection of the plane with a given line. */
bool Plane::intersect(const Line& line, Vector3f& point) const
{
  const Vector3f& ldirection = line.get_direction();
  float denominator = ldirection.dot(m_normal);
  if (abs_lt(denominator, SGAL_EPSILON)) return false;
  const Vector3f& lpoint = line.get_point();
  Vector3f p;
  p.scale(m_offset, m_normal);
  p.sub(lpoint);
  float d = p.dot(m_normal) / denominator;
  point.add_scaled(lpoint, d, ldirection);
  return true;
}

SGAL_END_NAMESPACE
