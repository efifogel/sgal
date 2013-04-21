// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

#include "SEGO/Ego_voxels_layer_filler_graph.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple_comparison.hpp>

SGAL_BEGIN_NAMESPACE

std::pair<Ego_voxels_layer_filler_graph::out_edge_iterator,
          Ego_voxels_layer_filler_graph::out_edge_iterator>
Ego_voxels_layer_filler_graph::out_edges(const vertex_descriptor& u) const {
  
  boost::tuple<std::size_t, std::size_t, std::size_t>
    dim = m_voxels.size();
  
  Is_inside_voxels is_inside(dim);
  Is_filling_identical is_filling(m_voxels);
  Out_edges_predicate filter(std::logical_and<bool>(),
                             is_filling,
                             Is_orthogonal(),
                             Is_same_level(),
                             Not_self_edge(),
                             is_inside);
  
  neighborhood_iterator unfiltered_begin(u);
  neighborhood_iterator unfiltered_end;

  out_edge_iterator begin(filter, unfiltered_begin);
  out_edge_iterator end(filter, unfiltered_end);

  return std::make_pair(begin, end);
}

Ego_voxels_layer_filler_graph::neighborhood_iterator::neighborhood_iterator()
    : m_center(0, 0, 0), m_null(true) {
}

Ego_voxels_layer_filler_graph::neighborhood_iterator::
neighborhood_iterator(const vertex_descriptor& c)
    : m_center(c), m_null(false) {
  long x = c.get<0>();
  long y = c.get<1>();
  long z = c.get<2>();

  long x_b = std::max(x-1, 0L);
  long y_b = std::max(y-1, 0L);
  long z_b = std::max(z-1, 0L);

  m_begin = vertex_iterator(x_b, x+2, y_b, y+2, z_b, z+2);
  m_end = vertex_iterator(x+2, x+2, y+2, y+2, z+2, z+2);
}

Ego_voxels_layer_filler_graph::neighborhood_iterator::Facade::reference
Ego_voxels_layer_filler_graph::neighborhood_iterator::dereference() const {
  return edge_descriptor (m_center, *m_begin);
}

bool Ego_voxels_layer_filler_graph::neighborhood_iterator::
equal(const neighborhood_iterator& o) const {

  if (this->is_end() && o.is_end())
    return true;
  
  return (this->m_center == o.m_center) &&
    (this->m_begin == o.m_begin) &&
    (this->m_end == o.m_end);
}

void Ego_voxels_layer_filler_graph::neighborhood_iterator::increment() {
  ++m_begin;
}

bool Ego_voxels_layer_filler_graph::neighborhood_iterator::is_end() const {
  if (m_null)
    return true;

  return (m_begin == m_end);
}

SGAL_END_NAMESPACE
