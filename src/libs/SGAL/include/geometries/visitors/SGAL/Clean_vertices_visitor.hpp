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

#ifndef SGAL_CLEAN_VERTICES_VISITOR_HPP
#define SGAL_CLEAN_VERTICES_VISITOR_HPP

#include <boost/variant.hpp>

#include <CGAL/basic.h>
#include <CGAL/convex_hull_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clean vertices visitor */
template <typename InputIterator>
class Clean_vertices_visitor : public boost::static_visitor<> {
public:
  typedef InputIterator         Input_iterator;

  /*! Construct.
   */
  Clean_vertices_visitor(Input_iterator begin, Input_iterator end) :
    m_begin(begin),
    m_end(end)
  {}

  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron) const
  {
    auto vit = polyhedron.vertices_begin();
    for (; vit != polyhedron.vertices_end(); ++vit) {
      // find the index
      Size index(0);
      for (auto cit = m_begin; cit != m_end; ++cit, ++index)
        if (vit->point() == *cit) break;
      vit->set_id(index);
    }
  }

  void operator()(Inexact_polyhedron& polyhedron) const { SGAL_error(); }
  void operator()(Epic_polyhedron& polyhedron) const { SGAL_error(); }

private:
  Input_iterator m_begin;
  Input_iterator m_end;
};

SGAL_END_NAMESPACE

#endif
