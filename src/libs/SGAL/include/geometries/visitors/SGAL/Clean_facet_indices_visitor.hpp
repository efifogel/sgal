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

#ifndef SGAL_CLEAN_FACET_INDICES_VISITOR_HPP
#define SGAL_CLEAN_FACET_INDICES_VISITOR_HPP

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Clean (compute) facet indices visitor */
class Clean_facet_indices_visitor : public boost::static_visitor<> {
public:
  std::vector<Int32>& m_indices;

  Clean_facet_indices_visitor(std::vector<Int32>& indices) :
    m_indices(indices)
  {}

  void operator()(const Triangle_indices& triangles)
  {
    m_indices.resize(4 * triangles.size());
    size_t k(0);
    for (size_t i = 0; i < triangles.size(); ++i) {
      m_indices[k++] = triangles[i][0];
      m_indices[k++] = triangles[i][1];
      m_indices[k++] = triangles[i][2];
      m_indices[k++] = -1;
    }
  }

  void operator()(const Quad_indices& quads)
  {
    m_indices.resize(5 * quads.size());
    size_t k(0);
    for (size_t i = 0; i < quads.size(); ++i) {
      m_indices[k++] = quads[i][0];
      m_indices[k++] = quads[i][1];
      m_indices[k++] = quads[i][2];
      m_indices[k++] = quads[i][3];
      m_indices[k++] = -1;
    }
  }

  void operator()(const Polygon_indices& polygons)
  {
    size_t size(0);
    for (size_t i = 0; i < polygons.size(); ++i) size += polygons[i].size() + 1;
    m_indices.resize(size);
    size_t k(0);
    for (size_t i = 0; i < polygons.size(); ++i) {
      for (size_t j = 0; j < polygons[i].size(); ++j)
        m_indices[k++] = polygons[i][j];
      m_indices[k++] = -1;
    }
  }

  void operator()(const Flat_indices& flats)
  { std::copy(flats.begin(), flats.end(), m_indices.begin()); }
};

SGAL_END_NAMESPACE

#endif
