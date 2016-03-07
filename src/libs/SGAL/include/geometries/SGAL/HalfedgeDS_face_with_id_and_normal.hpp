// Copyright (c) 2016 Israel.
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

#ifndef SGAL_HALFEDGEDS_FACE_WITH_ID_AND_NORMAL_HPP
#define SGAL_HALFEDGEDS_FACE_WITH_ID_AND_NORMAL_HPP

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/HalfedgeDS_face_with_id.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class HalfedgeDS_face_with_id_and_normal HalfedgeDS_face_with_id_and_normal.hpp
 * A class template, an instance of which is a type of a facet of a polyhedron
 * extended with a general purpose id and an inexact normal data members. It
 * can be used as a face for a model of the PolyhedronItems_3 concept.
 * In fact the face type has two additional data members:
 * (i) a reference to an incident halfedge, and
 * (ii) a plane.
 * see the base class HalfedgeDS_face_with_id for more information.
 */
template <typename Refs_, typename Plane_, typename Id_>
class HalfedgeDS_face_with_id_and_normal :
  public HalfedgeDS_face_with_id<Refs_, Plane_, Id_>
{
public:
  typedef Refs_                                                 Refs;
  typedef Plane_                                                Plane;
  typedef Id_                                                   Id;
  typedef typename SGAL::HalfedgeDS_face_with_id<Refs, Plane, Id>     Base;
  typedef typename Base::size_type                              size_type;

private:
  /*! The (inexact) normal of the plane containing the facet. */
  Vector3f m_normal;

public:
  /*! Construct default.
   */
  HalfedgeDS_face_with_id_and_normal() {}

  /*! Construct from a plane.
   */
  HalfedgeDS_face_with_id_and_normal(Plane const& p) : Base(p) {}

  /*! Construct from a plane and an id.
   */
  HalfedgeDS_face_with_id_and_normal(Plane const& p, size_type id) :
    Base(p, id)
  {}

  /*! Set the (inexact) normal of the plane containing the facet.
   */
  void set_normal(const Vector3f& normal) { m_normal = normal; }

  /*! Obtain the (inexact) normal of the plane containing the facet.
   */
  const Vector3f& get_normal() const { return m_normal; }
};

SGAL_END_NAMESPACE

#endif
