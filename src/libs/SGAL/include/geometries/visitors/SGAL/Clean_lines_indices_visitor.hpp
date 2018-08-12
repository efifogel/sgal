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

#ifndef SGAL_CLEAN_LINES_INDICES_VISITOR_HPP
#define SGAL_CLEAN_LINES_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clean indices visitor */
class Clean_lines_indices_visitor : public boost::static_visitor<> {
public:
  const std::vector<Int32>& m_indices;
  Size m_num_primitives;

  Clean_lines_indices_visitor(const std::vector<Int32>& indices,
                              Size num_primitives) :
    m_indices(indices),
    m_num_primitives(num_primitives)
  {}

  void operator()(Line_indices& lines)
  {
    lines.resize(m_num_primitives);
    auto it = m_indices.begin();
    for (size_t j = 0; j < m_num_primitives; ++j) {
      lines[j][0] = *it++;
      lines[j][1] = *it++;
      ++it;               // consume the -1 end-of-line-strip indicator
    }
  }

  void operator()(Polyline_indices& polylines)
  {
    polylines.resize(m_num_primitives);
    auto it = m_indices.begin();
    for (size_t j = 0; j < m_num_primitives; ++j) {
      auto it_start = it;
      size_t count(0);
      while (*it++ != -1) ++count;
      if (count != 0) {
        polylines[j].resize(count);
        it = it_start;
        size_t i(0);
        while (*it != -1) polylines[j][i++] = *it++;
        ++it;
      }
    }
  }

  void operator()(Flat_indices& flats)
  {
    flats.resize(m_num_primitives);
    std::copy(m_indices.begin(), m_indices.end(), flats.begin()); }
};

SGAL_END_NAMESPACE

#endif
