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


#ifndef _EGO_VOXELS_FILLTER_GRAPH_HPP_
#define _EGO_VOXELS_FILLTER_GRAPH_HPP_

/**
 * @file   Ego_voxels_filler_graph.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Thu Dec 13 21:57:52 2012
 * 
 * @brief  The class adapts the voxels sturcture to a boost graph (Graph,
 *         VertexListGraph, and IncidenceGraph concept).
 *         We use the graph to fill the voxels from the inside.
 *         Vertices are defined to be the voxels themselves, and the edges
 *         are defined to be the neighboring voxels (no diagonals). An edge
 *         is defined between two voxels only if they are both filled/not-filled.
 *         The main classes are Ego_voxels_filler_graph, and Ego_graph_vertex_index_map.
 */

#include "SEGO/Ego_voxels_vertex_list_graph.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels_filler_graph : public Ego_voxels_vertex_list_graph {
public:
  Ego_voxels_filler_graph(const Ego_voxels& voxels)
      : Ego_voxels_vertex_list_graph(voxels) {}

  struct traversal_category
    : public Ego_voxels_vertex_list_graph::traversal_category,
      public boost::incidence_graph_tag
  {};

  // IncidenceGraph

  // We need some helper classes.

  // Get the orthogonal neighborhood of a vertex.
  class orthogonal_edges_no_filter : public boost::iterator_facade<
    orthogonal_edges_no_filter, edge_descriptor,
    boost::forward_traversal_tag, edge_descriptor>
  {

  public:
    orthogonal_edges_no_filter();
    orthogonal_edges_no_filter(const vertex_descriptor& c);

  protected:
    friend class boost::iterator_core_access;

    typedef boost::iterator_facade<
      orthogonal_edges_no_filter, edge_descriptor,
      boost::forward_traversal_tag, edge_descriptor>     Base;
    typedef Base                                         Facade;

    Facade::reference dereference() const;
    bool equal(const orthogonal_edges_no_filter& o) const;
    void increment();

  private:
    vertex_descriptor m_center;
    long diff[3];
  };

  class Is_inside_voxels {
  public:

    // TODO: Why do I need default constructor?
    Is_inside_voxels() {}
    Is_inside_voxels(const boost::tuple<std::size_t,
                                        std::size_t,
                                        std::size_t>& dim) : m_dim(dim) {}
    
    bool operator() (const vertex_descriptor& point);
    bool operator() (const edge_descriptor& edge);
    
  protected:
    boost::tuple<std::size_t, std::size_t, std::size_t> m_dim;
  };

  class Is_filling_identical {
  public:

    // TODO: Why do I need default constructor?
    Is_filling_identical() : m_voxels(NULL) {}
    
    Is_filling_identical(const Ego_voxels& voxels) : m_voxels(&voxels) {}
    
    bool operator() (const edge_descriptor& edge) const;

  protected:
    const Ego_voxels *m_voxels;
  };


  typedef boost::filter_iterator<Is_inside_voxels,
                                 orthogonal_edges_no_filter>  orthogonal_edges;
  typedef boost::filter_iterator<Is_filling_identical,
                                 orthogonal_edges>            out_edge_iterator;
  typedef out_edge_iterator::difference_type                  degree_size_type;

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(const vertex_descriptor& u) const;
};

inline std::pair<Ego_voxels_filler_graph::out_edge_iterator,
                 Ego_voxels_filler_graph::out_edge_iterator>
out_edges(const Ego_voxels_filler_graph::vertex_descriptor& v,
          const Ego_voxels_filler_graph &graph) {
  
  return graph.out_edges(v);
}

inline Ego_voxels_filler_graph::degree_size_type
out_degree(const Ego_voxels_filler_graph::vertex_descriptor& v,
           const Ego_voxels_filler_graph &graph) {
  // This can probably be more efficient.
  std::pair<Ego_voxels_filler_graph::out_edge_iterator,
            Ego_voxels_filler_graph::out_edge_iterator> edges = 
    out_edges(v, graph);
  
  return std::distance(edges.first, edges.second);
}

SGAL_END_NAMESPACE

namespace boost {
  template <>
  struct graph_traits <SGAL::Ego_voxels_filler_graph> {
    typedef SGAL::Ego_voxels_filler_graph           Graph;

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


#endif // _EGO_VOXELS_FILLTER_GRAPH_HPP_
