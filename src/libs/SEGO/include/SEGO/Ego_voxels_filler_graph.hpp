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

#include "SEGO/multi_iterator.hpp"

#include <boost/tuple/tuple.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class Ego_voxels_filler_graph {
public:
  Ego_voxels_filler_graph(const Ego_voxels& voxels) : m_voxels(voxels) {}

  // Graph
  typedef boost::tuple<long, long, long>              vertex_descriptor;
  typedef std::pair<vertex_descriptor,
                    vertex_descriptor>                edge_descriptor;
  typedef boost::undirected_tag                       directed_category;
  typedef boost::disallow_parallel_edge_tag           edge_parallel_category;

  struct traversal_category : public boost::vertex_list_graph_tag,
                              public boost::incidence_graph_tag
  {};

  static Ego_voxels_filler_graph::vertex_descriptor
  null_vertex();

  // VertexListGraph
  typedef boost::counting_iterator<long>              counting_iterator;
  typedef multi_iterator<counting_iterator,
                         counting_iterator,
                         counting_iterator>           vertex_iterator;

  typedef vertex_iterator::difference_type            vertices_size_type;


  std::pair<vertex_iterator, vertex_iterator> vertices() const;    
  vertices_size_type num_vertices() const;
    
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

  class Is_unique_color {
  public:

    // TODO: Why do I need default constructor?
    Is_unique_color() : m_voxels(NULL) {}
    
    Is_unique_color(const Ego_voxels& voxels) : m_voxels(&voxels) {}
    
    bool operator() (const edge_descriptor& edge) const;

  protected:
    const Ego_voxels *m_voxels;
  };


  typedef boost::filter_iterator<Is_inside_voxels,
                                 orthogonal_edges_no_filter>  orthogonal_edges;
  typedef boost::filter_iterator<Is_unique_color,
                                 orthogonal_edges>            out_edge_iterator;
  typedef out_edge_iterator::difference_type                  degree_size_type;

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(const vertex_descriptor& u) const;

private:
  const Ego_voxels &m_voxels;
};

// Global req. of graph traits.
// VertexListGraph
inline std::pair<Ego_voxels_filler_graph::vertex_iterator,
                 Ego_voxels_filler_graph::vertex_iterator>
vertices(const Ego_voxels_filler_graph &graph) {
  return graph.vertices();
}

inline Ego_voxels_filler_graph::vertices_size_type
num_vertices(const Ego_voxels_filler_graph& g) {
  // TODO inside the function: return the size of the matrix instead.
  return g.num_vertices();
}

inline Ego_voxels_filler_graph::vertex_descriptor
source(const Ego_voxels_filler_graph::edge_descriptor e,
       const Ego_voxels_filler_graph&) {
  return e.first;
}

inline Ego_voxels_filler_graph::vertex_descriptor
target(const Ego_voxels_filler_graph::edge_descriptor e,
       const Ego_voxels_filler_graph&) {
  return e.second;
}

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

// IndexMap
struct Ego_graph_vertex_index_map {
public:
  typedef Ego_voxels_filler_graph::vertex_descriptor    vertex_descriptor;
  typedef std::size_t                                   value_type;
  typedef std::size_t                                   reference;
  typedef vertex_descriptor                             key_type;
  typedef boost::readable_property_map_tag              category;

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
