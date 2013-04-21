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
  
  Is_inside_voxels is_inside(dim);
  Is_filling_identical is_filling(m_voxels);
  
  Out_edges_predicate pred(std::logical_and<bool>(), 
                           is_filling,
                           Is_orthogonal(),
                           Not_self_edge(),
                           is_inside);

  out_edge_iterator begin(pred, neighborhood_iterator(u));
  out_edge_iterator end(pred, neighborhood_iterator());
  
  return std::make_pair(begin, end);
}

SGAL_END_NAMESPACE
