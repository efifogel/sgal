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

#ifndef SGAL_SIZE_FACET_INDICES_VISITOR_HPP
#define SGAL_SIZE_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Size facet indices visitor */
class Size_facet_indices_visitor : public boost::static_visitor<size_t> {
public:
  size_t operator()(const Triangle_indices& indices) const
  { return 3 * indices.size(); }

  size_t operator()(const Quad_indices& indices) const
  { return 4 * indices.size(); }

  size_t operator()(const Polygon_indices& indices) const
  {
    size_t size(0);
    for (auto& polygon: indices) size += polygon.size();
    return size;
  }

  size_t operator()(const Flat_indices& indices) const
  { return indices.size(); }
};

SGAL_END_NAMESPACE

#endif
