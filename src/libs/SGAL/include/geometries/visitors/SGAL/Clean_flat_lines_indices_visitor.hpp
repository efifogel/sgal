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

#ifndef SGAL_CLEAN_FLAT_LINES_INDICES_VISITOR_HPP
#define SGAL_CLEAN_FLAT_LINES_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clean (compute) lines indices visitor */
class Clean_flat_lines_indices_visitor : public boost::static_visitor<> {
public:
  std::vector<Int32>& m_indices;

  Clean_flat_lines_indices_visitor(std::vector<Int32>& indices) :
    m_indices(indices)
  {}

  void operator()(const Line_indices& lines)
  {
    m_indices.resize(4 * lines.size());
    size_t k(0);
    for (size_t i = 0; i < lines.size(); ++i) {
      m_indices[k++] = lines[i][0];
      m_indices[k++] = lines[i][1];
      m_indices[k++] = -1;
    }
  }

  void operator()(const Polyline_indices& polylines)
  {
    size_t size(0);
    for (size_t i = 0; i < polylines.size(); ++i)
      size += polylines[i].size() + 1;
    m_indices.resize(size);
    size_t k(0);
    for (size_t i = 0; i < polylines.size(); ++i) {
      for (size_t j = 0; j < polylines[i].size(); ++j)
        m_indices[k++] = polylines[i][j];
      m_indices[k++] = -1;
    }
  }

  void operator()(const Flat_indices& flats)
  { std::copy(flats.begin(), flats.end(), m_indices.begin()); }
};

SGAL_END_NAMESPACE

#endif
