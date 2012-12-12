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

#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <boost/property_map/vector_property_map.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/connected_components.hpp>

SGAL_BEGIN_NAMESPACE

// We don't need other functions to compile this boost stuff.
namespace SEGO_internal {

  template <class It1, class It2, class It3>
  class multi_iterator : public boost::iterator_facade<
    multi_iterator<It1, It2, It3>,
    boost::tuple<typename std::iterator_traits<It1>::value_type,
                 typename std::iterator_traits<It2>::value_type,
                 typename std::iterator_traits<It3>::value_type>,
                   boost::forward_traversal_tag,
    boost::tuple<typename std::iterator_traits<It1>::reference,
                 typename std::iterator_traits<It2>::reference,
                 typename std::iterator_traits<It3>::reference> > {
  public:

    // TODO: Why do I need this?
    multi_iterator() {}
    
    multi_iterator(It1 it1_b, It1 it1_e,
                   It2 it2_b, It2 it2_e,
                   It3 it3_b, It3 it3_e)
        : m_it1(it1_b), m_it1end(it1_e), m_it2(it2_b),
          m_it2begin(it2_b), m_it2end(it2_e), m_it3(it3_b),
          m_it3begin(it2_b), m_it3end(it3_e) {}

  protected:
    friend class boost::iterator_core_access;

    typedef multi_iterator<It1, It2, It3>            This;
    typedef boost::iterator_facade<
      This,
      boost::tuple<typename std::iterator_traits<It1>::value_type,
                   typename std::iterator_traits<It2>::value_type,
                   typename std::iterator_traits<It3>::value_type>,
                     boost::forward_traversal_tag,
      boost::tuple<typename std::iterator_traits<It1>::reference,
                   typename std::iterator_traits<It2>::reference,
                   typename std::iterator_traits<It3>::reference> >
    Base;

    typedef Base                                        Facade;

    typename Facade::reference
    dereference() const {
      return typename Facade::reference(*m_it1, *m_it2, *m_it3);
    }

    bool equal(const This& o) const {
      return ((this->m_it1 == o.m_it1) &&
              (this->m_it2 == o.m_it2) &&
              (this->m_it3 == o.m_it3));
    }

    void increment() {
      ++m_it3;

      if (m_it3 == m_it3end && m_it2 != m_it2end) {
        m_it3 = m_it3begin;
        ++m_it2;
      }
      
      if (m_it2 == m_it2end && m_it1 != m_it1end) {
        m_it2 = m_it2begin;
        ++m_it1;
      }
    }

  private:
    It1 m_it1, m_it1end;
    It2 m_it2, m_it2begin, m_it2end;
    It3 m_it3, m_it3begin, m_it3end;
  };

  // Should somehow use vertex descriptors.
  class Is_inside_voxels {
  public:
    typedef boost::tuple<long, long, long>           Vertex;
    typedef std::pair<Vertex, Vertex>                Edge;

    // TODO: Why do I need default constructor?
    Is_inside_voxels() {}

    Is_inside_voxels(const boost::tuple<std::size_t,
                                        std::size_t,
                                        std::size_t>& dim)
        : m_dim(dim) {}

    bool operator() (const Vertex& point) {
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

    bool operator() (const Edge& edge) {
      return (*this)(edge.first) && (*this)(edge.second);
    }

  protected:
    boost::tuple<std::size_t, std::size_t, std::size_t> m_dim;
  };

  class Is_unique_color {
  public:
    typedef boost::tuple<long, long, long>             Vertex;
    typedef std::pair<Vertex, Vertex>                  Edge;

    // TODO: Why do I need default constructor?
    Is_unique_color() : m_voxels(NULL) {}

    Is_unique_color(const Ego_voxels& voxels) : m_voxels(&voxels) {}

    bool operator() (const Edge& edge) const {
      typedef Ego_voxels::Container::value_type::value_type::value_type
        Value;
      Value a = m_voxels->voxels[edge.first.get<0>()]
        [edge.first.get<1>()][edge.first.get<2>()];
      Value b = m_voxels->voxels[edge.second.get<0>()]
        [edge.second.get<1>()][edge.second.get<2>()];
      return (a == b);
    }

  protected:
    const Ego_voxels *m_voxels;
  };

  // Get the orthogonal neighborhood of a vertex.
  // This also need to use some typedef or something. (instead of tuple)
  class orthogonal_edges_no_filter : public boost::iterator_facade<
    orthogonal_edges_no_filter,
    std::pair<boost::tuple<long, long, long>, boost::tuple<long, long, long> >,
    boost::forward_traversal_tag,
    std::pair<boost::tuple<long, long, long>, boost::tuple<long, long, long> > >
  {

  public:
    typedef boost::tuple<long, long, long>              Vertex;

    orthogonal_edges_no_filter()
        : m_center(0, 0, 0) {
      diff[0] = 0;
      diff[1] = 0;
      diff[2] = 0;
    }

    orthogonal_edges_no_filter(const Vertex& c)
        : m_center(c) {
      diff[0] = -1;
      diff[1] = 0;
      diff[2] = 0;
    }

  protected:
    friend class boost::iterator_core_access;

    typedef std::pair<Vertex, Vertex>                   Edge;
   
    typedef boost::iterator_facade<
      orthogonal_edges_no_filter, Edge,
      boost::forward_traversal_tag, Edge>               Base;

    typedef Base                                        Facade;

    typename Facade::reference
    dereference() const {
      return Edge (m_center, Vertex (m_center.get<0>() + diff[0],
                                     m_center.get<1>() + diff[1],
                                     m_center.get<2>() + diff[2]));
    }

    bool equal(const orthogonal_edges_no_filter& o) const {
      // end iterators are all equal.
      if (diff[0] == 0 && diff[1] == 0 && diff[2] == 0 &&
          o.diff[0] == 0 && o.diff[1] == 0 && o.diff[2] == 0)
        return true;
      
      return (this->m_center == o.m_center) &&
        (this->diff[0] == o.diff[0]) &&
        (this->diff[1] == o.diff[1]) &&
        (this->diff[2] == o.diff[2]);
    }
    
    void increment() {
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
        }
      }
    }

  private:
    boost::tuple<long, long, long> m_center;
    long diff[3];
  };

  class Ego_voxels_filler_graph {
  public:
    Ego_voxels_filler_graph(const Ego_voxels& voxels) : m_voxels(voxels) {}

    // Graph
    typedef boost::tuple<long, long, long>              vertex_descriptor;
    typedef boost::tuple<vertex_descriptor,
                         vertex_descriptor>             edge_descriptor;
    typedef boost::undirected_tag                       directed_category;
    typedef boost::disallow_parallel_edge_tag           edge_parallel_category;

    struct traversal_category : public boost::vertex_list_graph_tag,
                                public boost::incidence_graph_tag
    {};

    static Ego_voxels_filler_graph::vertex_descriptor
    null_vertex() {
      return vertex_descriptor(-1, -1, -1);
    }


    // VertexListGraph
    typedef boost::counting_iterator<long>              counting_iterator;
    typedef multi_iterator<counting_iterator,
                           counting_iterator,
                           counting_iterator>           vertex_iterator;

    typedef vertex_iterator::difference_type            vertices_size_type;


    std::pair<vertex_iterator, vertex_iterator>
    vertices() const {
      size_t x = m_voxels.voxels.size();
      size_t y = m_voxels.voxels[0].size();
      size_t z = m_voxels.voxels[0][0].size();

      vertex_iterator begin(0, x, 0, y, 0, z);
      vertex_iterator end(x, x, y, y, z, z);
      
      return std::make_pair(begin, end);
    }
    
    vertices_size_type num_vertices() const {
      std::pair<vertex_iterator, vertex_iterator> vertices = 
        this->vertices();

      // TODO: return the size of the matrix instead.
      return std::distance(vertices.first, vertices.second);
    }
    
    // IncidenceGraph
    typedef boost::filter_iterator<
      Is_inside_voxels,
      orthogonal_edges_no_filter>                       orthogonal_edges;
    typedef boost::filter_iterator<
      Is_unique_color, orthogonal_edges>                out_edge_iterator;
    typedef out_edge_iterator::difference_type          degree_size_type;

    std::pair<out_edge_iterator, out_edge_iterator>
    out_edges(const vertex_descriptor& u) const {

      boost::tuple<std::size_t, std::size_t, std::size_t>
        dim(m_voxels.voxels.size(), m_voxels.voxels[0].size(),
            m_voxels.voxels[0][0].size());

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

  private:
    const Ego_voxels &m_voxels;
  };

  // Global req. of graph traits.
  // VertexListGraph
  std::pair<Ego_voxels_filler_graph::vertex_iterator,
            Ego_voxels_filler_graph::vertex_iterator>
  vertices(const Ego_voxels_filler_graph &graph) {
    return graph.vertices();
  }

  Ego_voxels_filler_graph::vertices_size_type
  num_vertices(const Ego_voxels_filler_graph& g) {
    // TODO inside the function: return the size of the matrix instead.
    return g.num_vertices();
  }

  Ego_voxels_filler_graph::vertex_descriptor
  source(const Ego_voxels_filler_graph::edge_descriptor e,
         const Ego_voxels_filler_graph&) {
    return e.get<0>();
  }

  Ego_voxels_filler_graph::vertex_descriptor
  target(const Ego_voxels_filler_graph::edge_descriptor e,
         const Ego_voxels_filler_graph&) {
    return e.get<1>();
  }

  std::pair<Ego_voxels_filler_graph::out_edge_iterator,
            Ego_voxels_filler_graph::out_edge_iterator>
  out_edges(const Ego_voxels_filler_graph::vertex_descriptor& v,
            const Ego_voxels_filler_graph &graph) {

    return graph.out_edges(v);
  }

  Ego_voxels_filler_graph::degree_size_type
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
    
    std::size_t operator[] (const vertex_descriptor& v) const {
      return (v.get<0>() * m_voxels.voxels.size()) + 
        (v.get<1>() * m_voxels.voxels[0].size()) + 
        (v.get<2>() * m_voxels.voxels[0][0].size());
    }
    
    std::size_t max_index() const {
      return m_voxels.voxels.size() * m_voxels.voxels[0].size() *
        m_voxels.voxels[0][0].size();
    }

  private:
    const Ego_voxels& m_voxels;
  };

  Ego_graph_vertex_index_map::reference
  get(const Ego_graph_vertex_index_map& map,
      const Ego_graph_vertex_index_map::key_type &key) {
    return map[key];
  }
}

SGAL_END_NAMESPACE

namespace boost {
  template <>
  struct graph_traits <SGAL::SEGO_internal::Ego_voxels_filler_graph> {
    typedef SGAL::SEGO_internal::Ego_voxels_filler_graph Graph;

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

SGAL_BEGIN_NAMESPACE

void Ego_voxels_filler::operator() (Ego_voxels* voxels) const {
  
  BOOST_CONCEPT_ASSERT((boost::GraphConcept<SEGO_internal::Ego_voxels_filler_graph>));
  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<SEGO_internal::Ego_voxels_filler_graph>));
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<SEGO_internal::Ego_voxels_filler_graph>));

  // Using connected components without EdgeListGraph
  // TODO: check if dfs is faster.
  
  SEGO_internal::Ego_voxels_filler_graph graph(*voxels);
  SEGO_internal::Ego_graph_vertex_index_map index_map(*voxels);

  boost::vector_property_map<
    SEGO_internal::Ego_voxels_filler_graph::vertices_size_type,
    SEGO_internal::Ego_graph_vertex_index_map>
    component_map(index_map.max_index(), index_map);

  std::cout << "connected components begin." << std::endl;
  int num = boost::connected_components(graph, component_map,
                                        boost::vertex_index_map(index_map));

  std::cout << "Number of connected components: " << num << std::endl;
    
}
  
SGAL_END_NAMESPACE
