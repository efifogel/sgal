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
// $Source$
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_bound.hpp"

using namespace SGAL;

/*! Parameterless constructor */
Sphere_bound::Sphere_bound() : m_radius(0.0f) {}

/*! Copy constructor */
Sphere_bound::Sphere_bound(const Sphere_bound& s)
{
  m_center = s.get_center();
  m_radius = s.get_radius();
}

/*! Destructor */
Sphere_bound::~Sphere_bound() {}

/*! Checks whether the sphere contains another sphere.
 * @return true iff it contains the sphere.
 */
bool Sphere_bound::does_contain(const Sphere_bound* sphere) const
{
  float distance =
    m_center.distance(sphere->get_center()) + sphere->get_radius();
  return (distance < m_radius);
}

/*! Checks whether the sphere contains a point.
 * @return true iff it contains the point.
 */
bool Sphere_bound::does_contain(const Vector3f& point) const
{
  return (m_center.sqr_distance(point) < (m_radius* m_radius));
}

/*! Set the bounding sphere to contains all specified spheres.
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
void Sphere_bound::set_around(const Sphere_bound_vector_const& spheres)
{
  if (spheres.size() == 0) {
    m_center.set(0, 0, 0);
    m_radius = 0.0f;
    return;
  }

  if (spheres.size() == 1) {
    m_center.set(spheres[0]->get_center());
    m_radius = spheres[0]->get_radius();
    return;
  }

  Sphere_bound current(*spheres[0]);

  Vector3f c1, c2;
  float r1=0, r2=0;
  float d=0;      // the distance between the two centers
  float R = 0;    // the radius of the new sphere
  Vector3f v;    // the vector pointing from one center point to another
  Vector3f center;  // the center of the new sphere
  int num = spheres.size();
  for (int i = 1 ; i < num ; i++)
  {
    if (current.does_contain(spheres[i])) {
      continue;
    }
    if (spheres[i]->does_contain(&current)) {
      current = *spheres[i];
      continue;
    }
    c1 = current.get_center();
    c2 = spheres[i]->get_center();
    r1 = current.get_radius();
    r2 = spheres[i]->get_radius();
    d = c1.distance(c2);
    R = (d + r1 + r2)/2;

    v = c2;
    v.sub(c1);
    v.normalize();
    v.scale(R - r1);
    center = c1;
    center.add(v);

    current.set_center(center);
    current.set_radius(R);
  }

  m_radius = current.get_radius();
  m_center = current.get_center();
}
