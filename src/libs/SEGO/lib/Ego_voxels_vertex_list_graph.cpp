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

#include "SEGO/Ego_voxels_vertex_list_graph.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple_comparison.hpp>

SGAL_BEGIN_NAMESPACE

Ego_voxels_vertex_list_graph::vertex_descriptor
Ego_voxels_vertex_list_graph::null_vertex() {
  return vertex_descriptor(-1, -1, -1);
}

std::pair<Ego_voxels_vertex_list_graph::vertex_iterator,
          Ego_voxels_vertex_list_graph::vertex_iterator>
Ego_voxels_vertex_list_graph::vertices() const {
  size_t x, y, z;
  boost::tie(x, y, z) = m_voxels.size();
  
  vertex_iterator begin(0, x, 0, y, 0, z);
  vertex_iterator end(x, x, y, y, z, z);
  
  return std::make_pair(begin, end);
}

Ego_voxels_vertex_list_graph::vertices_size_type
Ego_voxels_vertex_list_graph::num_vertices() const {
  // std::pair<vertex_iterator, vertex_iterator> vertices = this->vertices();
  
  size_t x, y, z;
  boost::tie(x, y, z) = m_voxels.size();
  vertices_size_type ret = x * y * z;
  // SGAL_assertion(ret == std::distance(vertices.first, vertices.second));
  return ret;
}

std::size_t Ego_graph_vertex_index_map::operator[] (const vertex_descriptor& v) const {
  Ego_voxels::size_type size = m_voxels.size();
  size_t face_length = size.get<0>();
  size_t face_width = size.get<1>();
  size_t face_size = face_length * face_width;
  
  return (v.get<0>() + (v.get<1>() * face_length) + 
          (v.get<2>() * face_size));
}

std::size_t Ego_graph_vertex_index_map::max_index() const {
  Ego_voxels::size_type size = m_voxels.size();

  return size.get<0>() * size.get<1>() * size.get<2>();
}

SGAL_END_NAMESPACE
