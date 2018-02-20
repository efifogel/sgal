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

#ifndef SGAL_HALFEDGEDS_HALFEDGE_WITH_ID_HPP
#define SGAL_HALFEDGEDS_HALFEDGE_WITH_ID_HPP

#include <CGAL/basic.h>
#include <CGAL/HalfedgeDS_halfedge_base.h>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class HalfedgeDS_halfedge_with_id HalfedgeDS_halfedge_with_id.hpp
 * A class template, an instance of which is a type of a halfedge of a polyhedron
 * extended with a general purpose id data member. It can be used as a halfedge
 * for a model of the PolyhedronItems_3 concept.
 * In fact, the halfedge type has three additional optional data members:
 * (i)   a reference to the previous halfedge,
 * (ii)  an incident vertex, and
 * (iii) an incident face.
 *
 * The Tag_true tags below are hard coded indications that the halfedge type
 * has the three aforementioned additional optional data members.
 *
 * In the future, if space becomes a concern, it can be saved (at the account
 * of performance); we can define a new halfedge type without some or all of the
 * optional data members, and instead introduce code that can cope with the
 * absence of these data members.
 */
template <typename Refs_, typename Id_>
class HalfedgeDS_halfedge_with_id :
  public CGAL::HalfedgeDS_halfedge_base<Refs_, CGAL::Tag_true,
                                        CGAL::Tag_true, CGAL::Tag_true>
{
public:
  typedef Refs_                                                   Refs;
  typedef Id_                                                     Id;
  typedef typename CGAL::HalfedgeDS_halfedge_base<Refs,
                                                  CGAL::Tag_true,
                                                  CGAL::Tag_true,
                                                  CGAL::Tag_true> Base;
  typedef Id                                                      size_type;

private:
  /*! A general purpose id.
   * Can be used as a halfedge descriptor.
   */
  size_type m_id;

public:
  /*! Construct from an optional halfedge id.
   */
  HalfedgeDS_halfedge_with_id(size_type id  = size_type(-1)) : m_id(id) {}

  /*! Set the general purpose halfedge id.
   */
  void set_id(size_type id) { m_id = id; }

  /*! Obtain the general purpose halfedge Boolean flag.
   */
  size_type get_id() const { return m_id; }

  /*! Obtain a non-const reference of the halfedge id.
   */
  size_type& id() { return m_id; }

  /*! Obtain a const reference of the halfedge id.
   */
  size_type const& id() const { return m_id; }
};

SGAL_END_NAMESPACE

#endif
