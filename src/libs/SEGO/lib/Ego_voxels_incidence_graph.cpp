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

#include "SEGO/Ego_voxels_incidence_graph.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple_comparison.hpp>

SGAL_BEGIN_NAMESPACE

std::pair<Ego_voxels_incidence_graph::out_edge_iterator,
          Ego_voxels_incidence_graph::out_edge_iterator>
Ego_voxels_incidence_graph::out_edges(const vertex_descriptor& u) const {
  
  boost::tuple<std::size_t, std::size_t, std::size_t>
    dim = m_voxels.size();
  
  Is_inside_voxels is_inside(dim);
  Out_edges_predicate filter(std::logical_and<bool>(),
                             Is_orthogonal(),
                             Not_self_edge(),
                             is_inside);
  
  neighborhood_iterator unfiltered_begin(u);
  neighborhood_iterator unfiltered_end;

  out_edge_iterator begin(filter, unfiltered_begin);
  out_edge_iterator end(filter, unfiltered_end);

  return std::make_pair(begin, end);
}

SGAL_END_NAMESPACE
