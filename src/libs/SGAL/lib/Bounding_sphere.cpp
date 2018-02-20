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

/*!
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs default.
Bounding_sphere::Bounding_sphere() : m_radius(0.0f) {}

//! \brief constructs copt.
Bounding_sphere::Bounding_sphere(const Bounding_sphere& s)
{
  m_center = s.get_center();
  m_radius = s.get_radius();
}

//! \brief destructs.
Bounding_sphere::~Bounding_sphere() {}

//! \brief determines whether the sphere contains another sphere.
bool Bounding_sphere::does_contain(const Bounding_sphere* sphere) const
{
  float distance =
    m_center.distance(sphere->get_center()) + sphere->get_radius();
  return (distance < m_radius);
}

//! \brief determines whether the sphere contains a point.
bool Bounding_sphere::does_contain(const Vector3f& point) const
{ return (m_center.sqr_distance(point) < (m_radius* m_radius)); }

/*! \brief sets the bounding sphere to contains a range of spheres.
 * If only one sphere is given, it becomes the bounding sphere.
 * Otherwise we do the following:
 *
 * We set the bounding sphere to be the first sphere in the list.
 * Then we calculate the bounding sphere between this and the next
 * sphere. We get a new bounding sphere, referred to as current.
 * We keep iterating on the spheres, calculating the bounding sphere
 * of the current sphere and the next one in the list, until
 * we exhaust the list.
 *
 * To calculate a bounding sphere between any two spheres, we do:
 * # The radius of the bounding sphere is the R1+R2+d where R1
 * and R2 are the radii of the spheres and d is the distance between
 * the centeres of the the two spheres.
 * # We find the middle of the line connecting between the two centers
 * when the line is continued on both sides until it hits the sphere
 * border. This is the center of the sphere bound.
 *
 * @param spheres an array of sphere bound objects.
 */
// void Bounding_sphere::set_around(const Bounding_sphere_const_vector& spheres)
// {
// }

SGAL_END_NAMESPACE
