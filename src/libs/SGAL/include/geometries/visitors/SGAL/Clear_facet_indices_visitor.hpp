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

#ifndef SGAL_CLEAR_FACET_INDICES_VISITOR_HPP
#define SGAL_CLEAR_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clear facet indices visitor. */
class Clear_facet_indices_visitor : public boost::static_visitor<> {
public:
  // It is prohibited to specialize a function in non-namespace scope.
  // Instead, we we dispatch the appropriate function.

  template <typename T> struct Identity { typedef T type; };

  template <typename Indices>
  void operator()(Indices& indices) const
  { operator()(indices, Identity<Indices>()); }

private:
  template <typename Indices>
  void operator()(Indices& indices, Identity<Indices>) const { indices.clear(); }

  void operator()(Polygon_indices& indices, Identity<Polygon_indices>) const
  {
    for (auto it = indices.begin(); it != indices.end(); ++it) it->clear();
    indices.clear();
  }
};

SGAL_END_NAMESPACE

#endif
