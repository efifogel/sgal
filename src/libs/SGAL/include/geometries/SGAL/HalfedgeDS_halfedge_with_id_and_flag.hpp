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

#ifndef SGAL_HALFEDGEDS_HALFEDGE_WITH_ID_AND_FLAG_HPP
#define SGAL_HALFEDGEDS_HALFEDGE_WITH_ID_AND_FLAG_HPP

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/HalfedgeDS_halfedge_with_id.hpp"

SGAL_BEGIN_NAMESPACE

/*! \class HalfedgeDS_halfedge_with_id_and_flag HalfedgeDS_halfedge_with_id_and_flag.hpp
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
class HalfedgeDS_halfedge_with_id_and_flag :
  public HalfedgeDS_halfedge_with_id<Refs_, Id_>
{
public:
  typedef Refs_                                           Refs;
  typedef Id_                                             Id;
  typedef typename SGAL::HalfedgeDS_halfedge_with_id<Refs, Id>  Base;
  typedef typename Base::size_type                        size_type;

private:
  /*! A general purpose Boolean flag.
   * Can be used as a halfedge descriptor.
   */
  mutable Boolean m_flag;

public:
  /*! Construct default.
   */
  HalfedgeDS_halfedge_with_id_and_flag() : m_flag(false) {}

  /*! Construct from a halfedge id.
   */
  HalfedgeDS_halfedge_with_id_and_flag(size_type id) : Base(id), m_flag(false) {}

  /*! Construct from a Boolean flag.
   */
  HalfedgeDS_halfedge_with_id_and_flag(Boolean flag) : m_flag(flag) {}

  /*! Construct from a halfedge id and a Boolean flag.
   */
  HalfedgeDS_halfedge_with_id_and_flag(size_type id, Boolean flag) :
    Base(id), m_flag(flag)
  {}

  /*! Set the general purpose halfedge Boolean flag.
   */
  void set_flag(Boolean flag) const { m_flag = flag; }

  /*! Determine whether the general purpose halfedge Boolean flag is true.
   */
  Boolean is_flag() const { return m_flag; }

  /*! Set the flag that indicates whether the halfedge is creased.
   */
  void set_creased(Boolean flag) const { m_flag = flag; }

  /*! Determine whether the the halfedge is creased.
   */
  Boolean is_creased() const { return m_flag; }

};

SGAL_END_NAMESPACE

#endif
