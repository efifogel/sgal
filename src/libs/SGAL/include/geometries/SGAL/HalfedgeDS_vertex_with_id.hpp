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

#ifndef SGAL_HALFEDGEDS_VERTEX_WITH_ID_HPP
#define SGAL_HALFEDGEDS_VERTEX_WITH_ID_HPP

#include <CGAL/basic.h>
#include <CGAL/HalfedgeDS_vertex_base.h>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class HalfedgeDS_vertex_with_id HalfedgeDS_vertex_with_id.hpp
 * A class template, an instance of which is a type of a vertex of a polyhedron
 * extended with a general purpose id data member. It can be used as a vertex
 * for a model of the PolyhedronItems_3 concept.
 * In fact, the vertex type has an aditional optional data member that is a
 * reference to an incident halfedge.
 *
 * The Tag_true tag below is a hard coded indication that the vertex type has a
 * data member that is reference to an incident halfedge.
 *
 * In the future, if space becomes a concern, it can be saved (at the account
 * of performance); we can define a new vertex type without the additional
 * optional data member, and instead introduce code that can cope with the
 * absence of this data member.
 */
template <typename Refs_, typename Point_, typename Id_>
class HalfedgeDS_vertex_with_id :
  public CGAL::HalfedgeDS_vertex_base<Refs_, CGAL::Tag_true, Point_>
{
public:
  typedef Refs_                                         Refs;
  typedef Id_                                           Id;
  typedef Point_                                        Point;
  typedef typename CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point>
                                                        Base;
  typedef Id                                            size_type;

private:
  /*! A general purpose id.
   * Can be used as a vertex descriptor.
   */
  size_type m_id;

public:
  /*! Construct default.
   */
  HalfedgeDS_vertex_with_id() : m_id ( size_type(-1) ) {}

  /*! Construct from a point.
   */
  HalfedgeDS_vertex_with_id(Point const& p) : Base(p), m_id(size_type(-1)) {}

  /*! Construct from a point and a vertex id.
   */
  HalfedgeDS_vertex_with_id(Point const& p, size_type id) : Base(p), m_id(id) {}

  /*! Set the general purpose vertex id.
   */
  void set_id(size_type id) { m_id = id; }

  /*! Obtain the general purpose halfedge Boolean flag.
   */
  size_type get_id() const { return m_id; }

  /*! Obtain a non-const reference of the vertex id.
   */
  size_type& id() { return m_id; }

  /*! Obtain a const reference of the vertex id.
   */
  size_type const& id() const { return m_id; }
};

SGAL_END_NAMESPACE

#endif
