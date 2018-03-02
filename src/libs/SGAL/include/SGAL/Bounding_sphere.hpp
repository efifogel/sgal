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
 * A class representing a sphere bound of a node or a geometry container in the
 * scene graph.
 * A Bounding sphere in represented by its center point and its radius.
 *
 * When an animations is running or an asyncronious update occurs, the bounding
 * sphere has to be updated. For that purpose there is a dirty flag in
 * each class that contains a bounding sphere to indicate whether the bounding
 * sphere should be recalculated.
 */

#ifndef SGAL_BOUNDING_SPHERE_HPP
#define SGAL_BOUNDING_SPHERE_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Bounding_sphere;

class Bounding_sphere {
public:
  /*! Construct. */
  Bounding_sphere();

  /*! Copy construct. */
  Bounding_sphere(const Bounding_sphere& sb);

  /*! Destructor. */
  virtual ~Bounding_sphere();

  /*! Obtain the center.
   * \return The center.
   */
  const Vector3f& get_center() const;

  /*! Set the center.
   * \param[in] center the center.
   */
  void set_center(const Vector3f& center);

  /*! Set the center.
   * \param[in] x the x-coordinate of the center.
   * \param[in] y the y-coordinate of the center.
   * \param[in] z the z-coordinate of the center.
   */
  void set_center(Float x, Float y, Float z);

  /*! Obtain the radius.
   * \return The radius.
   */
  float get_radius() const;

  /*! Set the radius.
   * \param[in] radius the radius.
   */
  void set_radius(float radius);

  /*! Compare the sphere bound with another.
   * \param[in] other The other sphere bound.
   */
  Boolean operator==(const Bounding_sphere& other) const;

  /*! Determine whether the sphere contains another sphere.
   * \param[in] sphere the other sphere.
   * \return true iff it contains the sphere.
   */
  Boolean does_contain(const Bounding_sphere* sphere) const;

  /*! Determine whether the sphere contains a point.
   * \param[in] point the point.
   * \return true iff it contains the point.
   */
  Boolean does_contain(const Vector3f& point) const;

  /*! Sets the bounding sphere to contains a range of objects.
   */
  template <typename Iterator>
  void set_around(Iterator begin, Iterator end)
  {
    if (begin == end) {
      m_center.set(0, 0, 0);
      m_radius = 0.0f;
      return;
    }

    typedef typename std::iterator_traits<Iterator>::value_type Vt;
    // Dispatch the range to the appropriate implementation.
    return set_around(begin, end, Vt());
  }

  /*! Assignment operator */
  Bounding_sphere& operator=(const Bounding_sphere& s);

private:
  /*! Sets the bounding sphere to contains a range of points.
   */
  template <typename Iterator>
  void set_around(Iterator begin, Iterator end, const Bounding_sphere*)
  {
    const auto* first = *begin;
    m_center.set(first->get_center());
    m_radius = first->get_radius();

    for (++begin; begin != end; ++begin) {
      const auto* next = *begin;
      if (does_contain(next)) continue;
      if (next->does_contain(this)) {
        m_center.set(next->get_center());
        m_radius = next->get_radius();
        continue;
      }
      const Vector3f& c1 = get_center();
      const Vector3f& c2 = next->get_center();
      float r1 = get_radius();
      float r2 = next->get_radius();
      float d = c1.distance(c2);
      float R = (d + r1 + r2) * 0.5f;

      Vector3f v;
      v.sub(c2, c1);
      v.normalize();
      v.scale(R - r1);
      Vector3f center;
      center.add(c1, v);

      set_center(center);
      set_radius(R);
    }
  }

  /*! Sets the bounding sphere to contains a range of points.
   */
  template <typename Iterator>
  void set_around(Iterator begin, Iterator end, Vector3f)
  {
    // Find the bounding box; then, find the enclosing sphere that contains the
    // bounding box. This is acceptable for computing the frustum and the
    // navigation speed, but it is not good enough for collision detection.

    const Vector3f& vec = *begin;
    float minX = vec[0];
    float maxX = vec[0];
    float minY = vec[1];
    float maxY = vec[1];
    float minZ = vec[2];
    float maxZ = vec[2];

    for (++begin; begin != end; ++begin) {
      const Vector3f& vec = *begin;
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

  /*! The center point of the sphere. */
  Vector3f m_center;

  /*! The radius of the sphere. */
  float m_radius;
};

//! \brief obtains the center.
inline const Vector3f& Bounding_sphere::get_center() const { return m_center; }

//! \brief sets the center.
inline void Bounding_sphere::set_center(const Vector3f& center)
{ m_center = center; }

//! \brief sets the center.
inline void Bounding_sphere::set_center(Float x, Float y, Float z)
{ m_center.set(x, y, z); }

//! \brief obtains the radius.
inline float Bounding_sphere::get_radius() const { return m_radius; }

//! \brief sets the radius.
inline void Bounding_sphere::set_radius(float radius) { m_radius = radius; }

//! \brief compares the sphere bound with another.
inline Boolean Bounding_sphere::operator==(const Bounding_sphere& other) const
{
  return ((get_radius() == other.get_radius()) &&
          (get_center() == other.get_center()));
}

//! \brief assignment operator.
inline Bounding_sphere& Bounding_sphere::operator=(const Bounding_sphere& s)
{
  m_center = s.get_center();
  m_radius = s.get_radius();
  return *this;
}

SGAL_END_NAMESPACE

#endif
