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
// $Id: $
// $Revision: 14220 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PLANE_HPP
#define SGAL_PLANE_HPP

/*! \file
 * An oriented plane.
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Box_bound;
class Sphere_bound;
class Seg;
class Line;

class SGAL_CLASSDEF Plane {
public:
  /*! Set the plane with the content of another plane. */
  void set(const Plane& plane);

  /*! Set the normal of the plane. */
  void set_normal(Float x, Float y, Float z);

  /*! Set the normal of the plane. */
  void set_normal(const Vector3f& n);

  /*! Obtain the normal of the plane. */
  void get_normal(Vector3f& n) const;

  /*! Obtain the normal of the plane. */
  const Vector3f& get_normal() const;
  
  /*! Set the offset of the plane. */
  void set_offset(float o);

  /*! Obtain the offset of the plane. */
  Float get_offset() const;

public:
  // Configuration

  // Constructs a plane from the three points <p1> <p2>, and <p3>,
  // assuming they form a counter-clockwise triangle viewed from the front.
  void make_pts(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);

  // Stores the normal-point representation of the plane in norm and point.
  void make_norm_pt(const Vector3f& norm, const Vector3f& pos);

public:
  // Containment/intersection

  // (See Bound::contains().)
  float contains(const Box_bound* box) const;

  float contains(const Sphere_bound* sph) const;

  float contains(const Vector3f& pt) const;

  /*! Compute the intersection of the plane with a given line.
   * \param line the line
   * \param point the intersection point if exists. 
   * \return true of there is an intersection and false otherwise.
   *        (line is parallel to the plane.)
   */
  bool intersect(const Line& line, Vector3f& point) const;
    
  // (See Bound::Isect().)
  Uint isect(const Seg* seg, Float* d) const;
  Uint isect(const Seg* seg, Float* d1, float* d2) const;

  // Stores in <dst> the point on this plane closest to <pt>.
  void closest_pt_on(const Vector3f& pt, Vector3f& dst) const;

public:
  // Transformation

  // Stores in this half space the result of transforming <pln> by the
  // ortho transform <m>.
  void ortho_xform(const Plane* plane, const Matrix4f& m);

public:
  Vector3f m_normal;            // Normal to the plane.

  float m_offset;               // Offset in the plane equation
                                // pt dot normal = offset
};

/*! \brief sets the normal of the plane. */
inline void Plane::set_normal(Float x, Float y, Float z) { m_normal.set(x,y,z); }

/*! \brief sets the normal of the plane. */
inline void Plane::set_normal(const Vector3f& n) { m_normal.set(n); }

/*! \brief obtains the normal of the plane. */
inline void Plane::get_normal(Vector3f& n) const { m_normal.get(n); }

/*! \brief obtains the normal of the plane. */
inline const Vector3f& Plane::get_normal() const { return m_normal; }
  
/*! \brief sets the offset of the plane. */
inline void Plane::set_offset(float o) { m_offset = o; }

/*! \brief obtains the offset of the plane. */
inline Float Plane::get_offset() const { return m_offset; }

SGAL_END_NAMESPACE

#endif
