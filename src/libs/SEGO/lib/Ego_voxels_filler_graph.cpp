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

#include "SEGO/Ego_voxels_filler_graph.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple_comparison.hpp>

SGAL_BEGIN_NAMESPACE

std::pair<Ego_voxels_filler_graph::out_edge_iterator,
          Ego_voxels_filler_graph::out_edge_iterator>
Ego_voxels_filler_graph::out_edges(const vertex_descriptor& u) const {
  
  boost::tuple<std::size_t, std::size_t, std::size_t>
    dim = m_voxels.size();
  
  orthogonal_edges_no_filter unfiltered_begin(u);
  Is_inside_voxels pred1(dim);
  Is_unique_color pred2(m_voxels);
  
  orthogonal_edges env_begin(pred1, unfiltered_begin);
  orthogonal_edges env_end(pred1,
                           orthogonal_edges_no_filter());
  
  out_edge_iterator begin(pred2, env_begin);
  out_edge_iterator end(pred2, env_end);
  
  return std::make_pair(begin, end);
}

bool
Ego_voxels_filler_graph::Is_inside_voxels::operator() (const vertex_descriptor& point) {
  if (point.get<0>() < 0)
    return false;
  if (point.get<1>() < 0)
    return false;
  if (point.get<2>() < 0)
    return false;
  
  if (point.get<0>() >= long(m_dim.get<0>()))
    return false;
  if (point.get<1>() >= long(m_dim.get<1>()))
    return false;
  if (point.get<2>() >= long(m_dim.get<2>()))
    return false;
  
  return true;
}

bool Ego_voxels_filler_graph::Is_inside_voxels::operator() (const edge_descriptor& edge) {
  return (*this)(edge.first) && (*this)(edge.second);
}

Ego_voxels_filler_graph::orthogonal_edges_no_filter::orthogonal_edges_no_filter()
    : m_center(0, 0, 0) {
  diff[0] = 0;
  diff[1] = 0;
  diff[2] = 0;
}

Ego_voxels_filler_graph::orthogonal_edges_no_filter::
orthogonal_edges_no_filter(const vertex_descriptor& c)
    : m_center(c) {
  diff[0] = -1;
  diff[1] = 0;
  diff[2] = 0;
}

Ego_voxels_filler_graph::orthogonal_edges_no_filter::Facade::reference
Ego_voxels_filler_graph::orthogonal_edges_no_filter::dereference() const {
  return edge_descriptor (m_center, vertex_descriptor (m_center.get<0>() + diff[0],
                                                       m_center.get<1>() + diff[1],
                                                       m_center.get<2>() + diff[2]));
}

bool Ego_voxels_filler_graph::orthogonal_edges_no_filter::
equal(const orthogonal_edges_no_filter& o) const {
  // end iterators are all equal.
  if (diff[0] == 0 && diff[1] == 0 && diff[2] == 0 &&
      o.diff[0] == 0 && o.diff[1] == 0 && o.diff[2] == 0)
    return true;
  
  return (this->m_center == o.m_center) &&
    (this->diff[0] == o.diff[0]) &&
    (this->diff[1] == o.diff[1]) &&
    (this->diff[2] == o.diff[2]);
}

void Ego_voxels_filler_graph::orthogonal_edges_no_filter::increment() {
  for (long  i = 0; i < 3; ++i) {

    if (diff[i] == 0)
      continue;

    if (diff[i] == -1) {
      diff[i] = 1;
      break;
    }

    if (diff[i] == 1) {
      diff[i] = 0;
      if (i < 2)
        diff[i+1] = -1;
      break;
    }
  }
}

bool Ego_voxels_filler_graph::Is_unique_color::operator()
  (const edge_descriptor& edge) const {
  
  return (m_voxels->is_filled(edge.first) ==
          m_voxels->is_filled(edge.second));
}

SGAL_END_NAMESPACE
