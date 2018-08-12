// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GET_INDEX_LINES_INDICES_VISITOR_HPP
#define SGAL_GET_INDEX_LINES_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Get index lines indices visitor. */
class Get_index_lines_indices_visitor :
  public boost::static_visitor<Index_type>
{
private:
  size_t m_address;

public:
  Get_index_lines_indices_visitor(size_t address) : m_address(address) {}

  Index_type  operator()(const Line_indices& indices) const
  {
    auto res = std::div(m_address, 2);
    return indices[res.quot][res.rem];
  }

  Index_type operator()(const Polyline_indices& indices) const
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
    return indices[i][j];
  }

  Index_type operator()(const Flat_indices& indices) const
  { return indices[m_address]; }
};

SGAL_END_NAMESPACE

#endif
