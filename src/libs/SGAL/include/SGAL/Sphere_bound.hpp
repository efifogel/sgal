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
 * A class representing a sphere bound to a node/geometry in the scene graph.
 * A sphere bound in represented by its center point and its radius.
 *
 * When an animations is running or an asyncronious update occurs, the bounding
 * bounding sphere has to be updated. For that purpose there is a dirty flag in
 * each class that contains a bounding sphere to indicate whether the sphere
 * should be recalculated.
 *
 * @author: Tali Zvi
 * @date: 4/10/2000
 */

#ifndef SGAL_SPHERE_BOUND_HPP
#define SGAL_SPHERE_BOUND_HPP

#include <vector>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Sphere_bound;
typedef std::vector<Sphere_bound *>             Sphere_bound_vector;
typedef std::vector<const Sphere_bound *>       Sphere_bound_vector_const;

class Sphere_bound {
public:
  /*! Constructor. */
  Sphere_bound();

  /*! Copy constructor. */
  Sphere_bound(const Sphere_bound& sb);

  /*! Destructor. */
  virtual ~Sphere_bound();

  /*! Obtain the center.
   * \return The center.
   */
  const Vector3f& get_center() const;

  /*! Set the center.
   * \param center (in) the center.
   */
  void set_center(const Vector3f& center);

  /*! Set the center.
   * \param x (in) the x-coordinate of the center.
   * \param y (in) the y-coordinate of the center.
   * \param z (in) the z-coordinate of the center.
   */
  void set_center(Float x, Float y, Float z);

  /*! Obtain the radius.
   * \return The radius.
   */
  float get_radius() const;

  /*! Set the radius.
   * \param radius (in) the radius.
   */
  void set_radius(float radius);

  /*! Compare the sphere bound with another.
   * \param other The other sphere bound.
   */
  Boolean operator==(const Sphere_bound& other) const;

  Boolean does_contain(const Sphere_bound* sphere) const;

  Boolean does_contain(const Vector3f& point) const;

  void set_around(const Sphere_bound_vector_const& spheres);

  template <class Vector3f_iterator>
  void set_around(Vector3f_iterator begin, Vector3f_iterator end)
  {
    if (begin == end) {
      m_center.set(0, 0, 0);
      m_radius = 0.0f;
      return;
    }

    // to find the bounding sphere, we first find the bounding box
    // and then we find the enclosing sphere that contains the
    // bounding box. This is sufficiently good to find the right
    // frustum and the navigation speed, but it is not good enough
    // for collision detection.

    Vector3f vec = *begin;
    float minX = vec[0];
    float maxX = vec[0];
    float minY = vec[1];
    float maxY = vec[1];
    float minZ = vec[2];
    float maxZ = vec[2];

    for (begin++; begin != end; begin++) {
      Vector3f vec = *begin;
      if (vec[0] < minX) minX = vec[0];
      else if (vec[0] > maxX) maxX = vec[0];

      if (vec[1] < minY) minY = vec[1];
      else if (vec[1] > maxY) maxY = vec[1];

      if (vec[2] < minZ) minZ = vec[2];
      else if (vec[2] > maxZ) maxZ = vec[2];
    }

    m_center[0] = (minX + maxX)/2;
    m_center[1] = (minY + maxY)/2;
    m_center[2] = (minZ + maxZ)/2;
    Vector3f box_corner;
    box_corner.set(minX, minY, minZ);
    m_radius = m_center.distance(box_corner);
  }

  /*! Assignment operator */
  Sphere_bound& operator=(const Sphere_bound& s);

private:
  /*! The center point of the sphere. */
  Vector3f m_center;

  /*! The radius of the sphere. */
  float m_radius;
};

//! \brief obtains the center.
inline const Vector3f& Sphere_bound::get_center() const { return m_center; }

//! \brief sets the center.
inline void Sphere_bound::set_center(const Vector3f& center)
{ m_center = center; }

//! \brief sets the center.
inline void Sphere_bound::set_center(Float x, Float y, Float z)
{ m_center.set(x, y, z); }

//! \brief obtains the radius.
inline float Sphere_bound::get_radius() const { return m_radius; }

//! \brief sets the radius.
inline void Sphere_bound::set_radius(float radius) { m_radius = radius; }

//! \brief compares the sphere bound with another.
inline Boolean Sphere_bound::operator==(const Sphere_bound& other) const
{
  return ((get_radius() == other.get_radius()) &&
          (get_center() == other.get_center()));
}

/*! \brief assignment operator */
inline Sphere_bound& Sphere_bound::operator=(const Sphere_bound& s)
{
  m_center = s.get_center();
  m_radius = s.get_radius();
  return *this;
}

/*!
 */
inline std::ostream& operator<<(std::ostream& os, const Sphere_bound& sb)
{
  os << sb.get_center() << ", " << sb.get_radius();
  return os;
}

SGAL_END_NAMESPACE

#endif
