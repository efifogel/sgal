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

#include <boost/functional.hpp>

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
    //    multi_iterator() {}
    
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

  // Especially for vertex descriptors - should use vertex descrptors...
  class Is_inside_voxels {
  public:
    Is_inside_voxels(const boost::tuple<std::size_t,
                                        std::size_t,
                                        std::size_t>& dim)
        : m_dim(dim) {}

    bool operator() (const boost::tuple<long, long, long>& point) {
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
  protected:
    boost::tuple<std::size_t, std::size_t, std::size_t> m_dim;
  };
  
  template <class T1, class T2>
  struct Make_pair : public std::binary_function<T1, T2, std::pair<T1, T2> > {
    std::pair<T1, T2>
    operator() (const T1& t1, const T2& t2) const {
      return std::pair<T1, T2>(t1, t2);
    }
  };

  // Get the orthogonal neighborhood of a vertex.
  // This also need to use some typedef or something. (instead of tuple)
  class orthogonal_neighborhood_no_filter : public boost::iterator_facade<
    orthogonal_neighborhood_no_filter,
    boost::tuple<long, long, long>,
    boost::forward_traversal_tag,
    boost::tuple<long, long, long> > {

  public:
    orthogonal_neighborhood_no_filter()
        : m_center(0, 0, 0) {
      diff[0] = 0;
      diff[1] = 0;
      diff[2] = 0;
    }

    orthogonal_neighborhood_no_filter(const boost::tuple<long, long, long>& c)
        : m_center(c) {
      diff[0] = -1;
      diff[1] = 0;
      diff[2] = 0;
    }

  protected:
    friend class boost::iterator_core_access;
    
    typedef boost::iterator_facade<
      orthogonal_neighborhood_no_filter,
      boost::tuple<long, long, long>,
      boost::forward_traversal_tag,
      boost::tuple<long, long, long> >                  Base;

    typedef Base                                        Facade;

    typename Facade::reference
    dereference() const {
      return boost::tuple<long, long, long> (m_center.get<0>() + diff[0],
                                             m_center.get<1>() + diff[1],
                                             m_center.get<2>() + diff[2]);
    }

    bool equal(const orthogonal_neighborhood_no_filter& o) const {
      // end iterators are all equal.
      if (diff[0] == 0 && diff[1] == 0 && diff[2] == 0 &&
          o.diff[0] == 0 && o.diff[1] == 0 && o.diff[2] == 0)
        return true;
      
      return *this == o;
    }
    
    void increment() {
      for (long  i = 0; i < 3; ++i) {
        if (diff[i] == 0)
          continue;

        if (diff[i] == -1)
          diff[i] = 1;

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
    Ego_voxels_filler_graph(Ego_voxels& voxels) : m_voxels(voxels) {}

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
      orthogonal_neighborhood_no_filter>                orthogonal_neighborhood;
    
    typedef boost::binder1st<
      Make_pair<vertex_descriptor, vertex_descriptor> > Make_edge_from_source;
    typedef boost::transform_iterator<
      Make_edge_from_source, orthogonal_neighborhood>   out_edge_iterator;
    typedef out_edge_iterator::difference_type          degree_size_type;

    std::pair<out_edge_iterator, out_edge_iterator>
    out_edges(const vertex_descriptor& u) const {

      boost::tuple<std::size_t, std::size_t, std::size_t>
        dim(m_voxels.voxels.size(), m_voxels.voxels[0].size(),
            m_voxels.voxels[0][0].size());

      orthogonal_neighborhood_no_filter unfiltered_begin(u);
      Is_inside_voxels pred(dim);
      orthogonal_neighborhood env_begin(pred, unfiltered_begin);
      orthogonal_neighborhood env_end(pred,
                                      orthogonal_neighborhood_no_filter());
      
      Make_edge_from_source make_edges(Make_pair<vertex_descriptor,
                                                 vertex_descriptor>(), u);

      out_edge_iterator begin(env_begin, make_edges);
      out_edge_iterator end(env_end, make_edges);

      // e = *p.first;

      return std::make_pair(begin, end);
    }

  private:
    Ego_voxels &m_voxels;
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
}
  
SGAL_END_NAMESPACE
