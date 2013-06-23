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


#ifndef _EGO_VOXELS_VERTEX_LIST_GRAPH_HPP_
#define _EGO_VOXELS_VERTEX_LIST_GRAPH_HPP_

/**
 * @file   Ego_voxels_vertex_list_graph.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Thu Dec 13 21:57:52 2012
 * 
 * @brief  The class adapts the voxels sturcture to a boost graph (Graph and
 *         VertexListGraph only).
 *         Usually the class is used a base class for other voxels graphs
 *         adaptation which further extends vertex_list.
 *         The main classes are Ego_voxels_vertex_list_graph, and 
 *         Ego_graph_vertex_index_map.
 */

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

#include "SEGO/basic.hpp"
#include "SEGO/multi_iterator.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_SEGO_DECL Ego_voxels_vertex_list_graph {
public:
  Ego_voxels_vertex_list_graph(const Ego_voxels& voxels) : m_voxels(voxels) {}

  // Graph
  typedef boost::tuple<long, long, long>              vertex_descriptor;
  typedef std::pair<vertex_descriptor,
                    vertex_descriptor>                edge_descriptor;
  typedef boost::undirected_tag                       directed_category;
  typedef boost::disallow_parallel_edge_tag           edge_parallel_category;

  struct traversal_category : public boost::vertex_list_graph_tag
  {};

  static Ego_voxels_vertex_list_graph::vertex_descriptor
  null_vertex();

  // VertexListGraph
  typedef boost::counting_iterator<long>              counting_iterator;
  typedef multi_iterator<counting_iterator,
                         counting_iterator,
                         counting_iterator>           vertex_iterator;

  typedef vertex_iterator::difference_type            vertices_size_type;


  std::pair<vertex_iterator, vertex_iterator> vertices() const;    
  vertices_size_type num_vertices() const;
    
protected:
  const Ego_voxels &m_voxels;
};

// Global req. of graph traits.
// VertexListGraph
inline std::pair<Ego_voxels_vertex_list_graph::vertex_iterator,
                 Ego_voxels_vertex_list_graph::vertex_iterator>
vertices(const Ego_voxels_vertex_list_graph &graph) {
  return graph.vertices();
}

inline Ego_voxels_vertex_list_graph::vertices_size_type
num_vertices(const Ego_voxels_vertex_list_graph& g) {
  // TODO inside the function: return the size of the matrix instead.
  return g.num_vertices();
}

inline Ego_voxels_vertex_list_graph::vertex_descriptor
source(const Ego_voxels_vertex_list_graph::edge_descriptor e,
       const Ego_voxels_vertex_list_graph&) {
  return e.first;
}

inline Ego_voxels_vertex_list_graph::vertex_descriptor
target(const Ego_voxels_vertex_list_graph::edge_descriptor e,
       const Ego_voxels_vertex_list_graph&) {
  return e.second;
}

// IndexMap
struct SGAL_SEGO_DECL Ego_graph_vertex_index_map {
public:
  typedef Ego_voxels_vertex_list_graph::vertex_descriptor   vertex_descriptor;
  typedef std::size_t                                       value_type;
  typedef std::size_t                                       reference;
  typedef vertex_descriptor                                 key_type;
  typedef boost::readable_property_map_tag                  category;

  Ego_graph_vertex_index_map(const Ego_voxels& voxels) : m_voxels(voxels) {}
    
  std::size_t operator[] (const vertex_descriptor& v) const;    
  std::size_t max_index() const;

private:
  const Ego_voxels& m_voxels;
};

inline Ego_graph_vertex_index_map::reference
get(const Ego_graph_vertex_index_map& map,
    const Ego_graph_vertex_index_map::key_type &key) {
  return map[key];
}

SGAL_END_NAMESPACE

namespace boost {
  template <>
  struct graph_traits <SGAL::Ego_voxels_vertex_list_graph> {
    typedef SGAL::Ego_voxels_vertex_list_graph      Graph;

    typedef Graph::vertex_descriptor                vertex_descriptor;
    typedef Graph::directed_category                directed_category;
    typedef Graph::edge_parallel_category           edge_parallel_category;
    typedef Graph::traversal_category               traversal_category;
    typedef Graph::vertex_iterator                  vertex_iterator;
    typedef Graph::vertices_size_type               vertices_size_type;

    typedef Graph::edge_descriptor                  edge_descriptor;
  };
}

#endif // _EGO_VOXELS_VERTEX_LIST_GRAPH_HPP_
