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

#ifndef SGAL_HALFEDGEDS_FACE_WITH_ID_HPP
#define SGAL_HALFEDGEDS_FACE_WITH_ID_HPP

#include <CGAL/basic.h>
#include <CGAL/HalfedgeDS_face_base.h>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class HalfedgeDS_face_with_id HalfedgeDS_face_with_id.hpp
 * A class template, an instance of which is a type of a facet of a polyhedron
 * extended with a general purpose id data member. It can be used as a face for
 * a model of the PolyhedronItems_3 concept.
 * In fact, the face type has two additional optional data members:
 * (i)  a reference to an incident halfedge, and
 * (ii) a plane.
 *
 * The Tag_true tag below is a hard coded indication that the face type has a
 * data member that is reference to an incident halfedge, and the constructor
 * from a plane below supports the intialization of the plane data member.
 *
 * In the future, if space becomes a concern, it can be saved (at the account
 * of performance); we can define a new face type without some or all of the
 * optional data members, and instead introduce code that can cope with the
 * absence of these data members.
 */
template <typename Refs_, typename Plane_, typename Id_>
class HalfedgeDS_face_with_id :
  public CGAL::HalfedgeDS_face_base<Refs_, CGAL::Tag_true, Plane_>
{
public:
  typedef Refs_                                         Refs;
  typedef Plane_                                        Plane;
  typedef Id_                                           Id;
  typedef typename CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane>
                                                        Base;
  typedef Id                                            size_type;

private:
  /*! A general purpose id.
   * Can be used as a face descriptor.
   */
  size_type m_id;

public:
  /*! Construct default.
   */
  HalfedgeDS_face_with_id() : m_id ( size_type(-1) ) {}

  /*! Construct from a plane.
   */
  HalfedgeDS_face_with_id(Plane const& p) : Base(p), m_id(size_type(-1)) {}

  /*! Construct from a plane and a face id.
   */
  HalfedgeDS_face_with_id(Plane const& p, size_type id) : Base(p), m_id(id) {}

  /*! Set the general purpose face id.
   */
  void set_id(size_type id) { m_id = id; }

  /*! Obtain the general purpose halfedge Boolean flag.
   */
  size_type get_id() const { return m_id; }

  /*! Obtain a non-const reference of the face id.
   */
  size_type& id() { return m_id; }

  /*! Obtain a const reference of the face id.
   */
  size_type const& id() const { return m_id; }
};

SGAL_END_NAMESPACE

#endif
