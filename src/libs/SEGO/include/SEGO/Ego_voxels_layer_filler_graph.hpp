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


#ifndef SEGO_EGO_VOXELS_LAYER_FILLER_GRAPH_HPP
#define SEGO_EGO_VOXELS_LAYER_FILLER_GRAPH_HPP

/**
 * @file   Ego_voxels_layer_filler_graph.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Thu Dec 13 21:57:52 2012
 * 
 * @brief  The class adapts the voxels sturcture to a boost graph (Graph,
 *         VertexListGraph, and IncidenceGraph concept).
 *         We use the graph to layer the boundary of the initial voxels
 *         structure.
 *         Vertices are defined to be the voxels themselves, and the edges
 *         are defined to be the neighboring voxels (no diagonals) at the
 *         same level. An edge is defined between two voxels only if they 
 *         are both filled/not-filled.
 */

#include "SEGO/Ego_voxels_vertex_list_graph.hpp"
#include "SEGO/Ego_voxels_vertex_list_graph_utils.hpp"

#include "SEGO/chain.hpp"
#include <functional>

SGAL_BEGIN_NAMESPACE

class Ego_voxels_layer_filler_graph : public Ego_voxels_vertex_list_graph {
public:
  Ego_voxels_layer_filler_graph(const Ego_voxels& voxels)
      : Ego_voxels_vertex_list_graph(voxels) {}

  struct traversal_category
    : public Ego_voxels_vertex_list_graph::traversal_category,
      public boost::incidence_graph_tag
  {};

  // IncidenceGraph

  // We need some helper classes.
  
  // Get the neighborhood of a vertex.
  // TODO: make this iterator_adaptor
  class neighborhood_iterator : public boost::iterator_facade<
    neighborhood_iterator, edge_descriptor,
    boost::forward_traversal_tag, edge_descriptor>
  {

  public:
    neighborhood_iterator();
    neighborhood_iterator(const vertex_descriptor& c);

  protected:
    friend class boost::iterator_core_access;

    typedef boost::iterator_facade<
      neighborhood_iterator, edge_descriptor,
      boost::forward_traversal_tag, edge_descriptor>     Base;
    typedef Base                                         Facade;

    Facade::reference dereference() const;
    bool equal(const neighborhood_iterator& o) const;
    void increment();

  private:
    bool is_end() const;
    
    vertex_descriptor m_center;
    vertex_iterator m_begin;
    vertex_iterator m_end;
    bool m_null;
  };
  
  // Last functor is run first...
  typedef chain<std::logical_and<bool>,
                Is_filling_identical,
                Is_orthogonal,
                Is_same_level,
                Not_self_edge,
                Is_inside_voxels>                      Out_edges_predicate;
  typedef boost::filter_iterator<Out_edges_predicate,
                                 neighborhood_iterator>    out_edge_iterator;

  typedef out_edge_iterator::difference_type                  degree_size_type;

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(const vertex_descriptor& u) const;
};

inline std::pair<Ego_voxels_layer_filler_graph::out_edge_iterator,
                 Ego_voxels_layer_filler_graph::out_edge_iterator>
out_edges(const Ego_voxels_layer_filler_graph::vertex_descriptor& v,
          const Ego_voxels_layer_filler_graph &graph) {
  
  return graph.out_edges(v);
}

inline Ego_voxels_layer_filler_graph::degree_size_type
out_degree(const Ego_voxels_layer_filler_graph::vertex_descriptor& v,
           const Ego_voxels_layer_filler_graph& graph) {
  // This can probably be more efficient.
  std::pair<Ego_voxels_layer_filler_graph::out_edge_iterator,
            Ego_voxels_layer_filler_graph::out_edge_iterator> edges = 
    out_edges(v, graph);
  
  return std::distance(edges.first, edges.second);
}

SGAL_END_NAMESPACE

namespace boost {
  template <>
  struct graph_traits <SGAL::Ego_voxels_layer_filler_graph> {
    typedef SGAL::Ego_voxels_layer_filler_graph           Graph;

    typedef Graph::vertex_descriptor                vertex_descriptor;
    typedef Graph::directed_category                directed_category;
    typedef Graph::edge_parallel_category           edge_parallel_category;
    typedef Graph::traversal_category               traversal_category;
    typedef Graph::vertex_iterator                  vertex_iterator;
    typedef Graph::vertices_size_type               vertices_size_type;

    typedef Graph::out_edge_iterator                out_edge_iterator;
    typedef Graph::edge_descriptor                  edge_descriptor;
    typedef Graph::degree_size_type                 degree_size_type;
  };
}

#endif // SEGO_EGO_VOXELS_LAYER_FILLER_GRAPH_HPP
