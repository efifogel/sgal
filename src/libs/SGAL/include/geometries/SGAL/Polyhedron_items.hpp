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

#ifndef SGAL_POLYHEDRON_ITEMS_HPP
#define SGAL_POLYHEDRON_ITEMS_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Refs>
struct Polyhedron_halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs> {
  Uint m_index;
  Boolean m_creased;
  Polyhedron_halfedge() {}
};

// An items type using my vertex and edge.
struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef Polyhedron_halfedge<Refs> Halfedge;
  };
};

SGAL_END_NAMESPACE

#endif
