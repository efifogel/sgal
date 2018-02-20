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

#ifndef SGAL_SET_INDEX_FACET_INDICES_VISITOR_HPP
#define SGAL_SET_INDEX_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Set index facet indices visitor. */
class Set_index_facet_indices_visitor : public boost::static_visitor<> {
private:
  size_t m_address;
  Index_type m_value;

public:
  Set_index_facet_indices_visitor(size_t address, Index_type value) :
    m_address(address), m_value(value) {}

  void operator()(Triangle_indices& indices) const
  {
    auto res = std::div(m_address, 3);
    indices[res.quot][res.rem] = m_value;
  }

  void operator()(Quad_indices& indices) const
  {
    auto res = std::div(m_address, 4);
    indices[res.quot][res.rem] = m_value;
  }

  void operator()(Polygon_indices& indices) const
  {
    auto address = m_address;
    size_t i(0);
    size_t j(0);
    for (auto it = indices.begin(); it != indices.end(); ++it) {
      if (address < it->size()) {
        j = address;
        break;
      }
      address -= it->size();
    }
    indices[i][j] = m_value;
  }

  void operator()(Flat_indices& indices) const
  { indices[m_address] = m_value; }
};

SGAL_END_NAMESPACE

#endif
