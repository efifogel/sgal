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
// Author(s)     : Ophir Setter           <ophir.setter@gmail.com>

#ifndef SGAL_EGO_VOXELS_FILLER_TEMPLATE_HPP
#define SGAL_EGO_VOXELS_FILLER_TEMPLATE_HPP

#include "SGAL/config.hpp"
#include "SGAL/SGAL_defs.hpp"

#include "SEGO/Ego_voxels_vertex_list_graph.hpp"
#include "SEGO/Ego_voxels_incidence_graph.hpp"

#include <boost/unordered_set.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/connected_components.hpp>

// #define EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
#ifdef EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
#include <boost/tuple/tuple_io.hpp>
#endif

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

template <class Graph>
class Ego_voxels_filler_template {
 public:
  typedef Ego_voxels_vertex_list_graph::vertices_size_type
    vertices_size_type;
  typedef Ego_voxels_vertex_list_graph::vertex_descriptor
    vertex_descriptor;
  typedef Ego_voxels_vertex_list_graph::edge_descriptor
    edge_descriptor;
  typedef Ego_voxels_vertex_list_graph::vertex_iterator
    vertex_iterator;

  typedef boost::vector_property_map<vertices_size_type,
                                     Ego_graph_vertex_index_map>
    Component_map;
  typedef boost::unordered_set<vertices_size_type> External_connected_components;

  
  Ego_voxels_filler_template(long offset);
  void operator() (Ego_voxels* voxels) const;

 private:
  Component_map compute_connected_components(const Ego_voxels* voxels) const;
  External_connected_components
    compute_external_connected_components(const Ego_voxels* voxels,
                                          const Component_map&) const;
  void offset(const External_connected_components& external_connected_components,
              Component_map* component_map, Ego_voxels* voxels) const;
  void fill(const External_connected_components& external_connected_components,
            const Component_map& component_map, Ego_voxels* voxels) const;

  long m_offset;
};

//////////////////////////////////////////////////////////////////////

template <class Graph>
Ego_voxels_filler_template<Graph>::Ego_voxels_filler_template(long offset)
    : m_offset(offset) {}

template <class Graph>
void Ego_voxels_filler_template<Graph>::operator() (Ego_voxels* voxels) const {

  Component_map component_map = compute_connected_components(voxels);

  // This is the set of connected components that are outside.
  External_connected_components external_connected_components = 
    compute_external_connected_components(voxels, component_map);

  if (m_offset > 0) {
    for (long i = 0; i < m_offset; ++i) {
      offset(external_connected_components, &component_map, voxels);
    }
  }

  // -1 means that we need to fully fill.
  if (m_offset == -1)
    fill(external_connected_components, component_map, voxels);
}

template <class Graph>
typename Ego_voxels_filler_template<Graph>::Component_map
Ego_voxels_filler_template<Graph>::compute_connected_components(const Ego_voxels* voxels) const {
  
  BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>));
  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Graph>));
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Graph>));

  Graph graph(*voxels);
  Ego_graph_vertex_index_map index_map(*voxels);

  Component_map component_map(index_map.max_index(), index_map);

#ifdef EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
  std::cout << "connected components begin." << std::endl;
#endif

#ifdef EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
  vertices_size_type num = 
    boost::connected_components(graph, component_map,
                                boost::vertex_index_map(index_map));
#else
    boost::connected_components(graph, component_map,
                                boost::vertex_index_map(index_map));
#endif

#ifdef EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
  std::cout << "Number of connected components: " << num << std::endl;

  size_t isize, jsize, ksize;
  boost::tie(isize, jsize, ksize) = voxels->size();
  if (isize < 80 && jsize < 80 && ksize < 80) {
    for (size_t k = 0; k < ksize; ++k) {
      for (size_t j = 0; j < jsize; ++j) {
        for (size_t i = 0; i < isize; ++i) {
          std::cout << component_map[boost::make_tuple(i, j, k)];
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }
#endif
  
  return component_map;
}

template <class Graph>
typename Ego_voxels_filler_template<Graph>::External_connected_components
Ego_voxels_filler_template<Graph>::compute_external_connected_components
(const Ego_voxels* voxels, const Component_map& component_map) const {
  size_t x_max, y_max, z_max;
  boost::tie(x_max, y_max, z_max) = voxels->size();

  External_connected_components external_connected_components;

  // We go over the frame of each level. External are the ones on the frame which are not filled.
  // We currently ignore some side case in case of regular filling (if the sillouette of the
  // voxels creates a non connected component on the top or bottom of the voxels.
  for (std::size_t i = 0; i < z_max; ++i) {
    for (std::size_t j = 0; j < y_max; ++j) {
      if (voxels->is_filled(0, j, i) == false)
        external_connected_components.insert(component_map[vertex_descriptor(0, j, i)]);

      if (voxels->is_filled(x_max - 1, j, i) == false)
        external_connected_components.insert(component_map[vertex_descriptor(x_max - 1, j, i)]);
    }

    for (std::size_t j = 0; j < x_max; ++j) {
      if (voxels->is_filled(j, 0, i) == false)
        external_connected_components.insert(component_map[vertex_descriptor(j, 0, i)]);

      if (voxels->is_filled(j, y_max - 1, i) == false)
        external_connected_components.insert(component_map[vertex_descriptor(j, y_max - 1, i)]);
    }
  }
  
#ifdef EGO_VOXELIZER_FILLER_TEMPLATE_VERBOSE
  std::cout << "External connected compnents:" << std::endl;
  for (External_connected_components::iterator it = external_connected_components.begin();
       it != external_connected_components.end(); ++it)
    std::cout << *it << std::endl;
  std::cout << std::endl;
#endif

  return external_connected_components;
}

template <class Graph>
void Ego_voxels_filler_template<Graph>::offset
(const External_connected_components& external_connected_components,
 Component_map* component_map, Ego_voxels* voxels) const {

  typedef boost::vector_property_map<vertex_descriptor,
                                     Ego_graph_vertex_index_map>
    Filling_map;

  typedef Ego_voxels_incidence_graph::out_edge_iterator out_edge_iterator;
  
  // For every cell we compute whether it becomes filled this time.
  Ego_voxels_incidence_graph graph(*voxels);
  
  Ego_graph_vertex_index_map index_map(*voxels);
  Filling_map filling_map(index_map.max_index(), index_map);
  
  std::pair<vertex_iterator, vertex_iterator> vertices_pair = vertices(graph);
  vertex_iterator vertices_begin = vertices_pair.first;
  vertex_iterator vertices_end = vertices_pair.second;

  for(; vertices_begin != vertices_end; ++vertices_begin) {
    // pointing to itself means that there is no filling.
    put(filling_map, *vertices_begin, *vertices_begin);
  }

  vertices_begin = vertices_pair.first; vertices_end = vertices_pair.second;
  for(; vertices_begin != vertices_end; ++vertices_begin) {
    // From every vertex we are filling one adjucent square, unless it is external or
    // already filled.
    if (voxels->is_filled(*vertices_begin) == false)
      continue;
    
    std::pair<out_edge_iterator, out_edge_iterator> edges_pair = out_edges(*vertices_begin, graph);
    out_edge_iterator edges_begin = edges_pair.first;
    out_edge_iterator edges_end = edges_pair.second;
    for (; edges_begin != edges_end; ++edges_begin) {
      
      vertex_descriptor target_vertex = target(*edges_begin, graph);
      if (voxels->is_filled(target_vertex) == true)
        continue;

      vertices_size_type connected_component = get(*component_map, target_vertex);    
      if (external_connected_components.find(connected_component) !=
          external_connected_components.end())
        continue;

      put(filling_map, target_vertex, *vertices_begin);
    }
  }

  // Now actually mark them.
  vertices_begin = vertices_pair.first; vertices_end = vertices_pair.second;
  for(; vertices_begin != vertices_end; ++vertices_begin) {
    vertex_descriptor target_v = *vertices_begin;
    vertex_descriptor source_v = get(filling_map, target_v);
    if (source_v == target_v)
      continue;

    voxels->fill(target_v);
    Component_map::value_type component = get(*component_map, source_v);
    put(*component_map, target_v, component);
  }
}

template <class Graph>
void Ego_voxels_filler_template<Graph>::fill
(const External_connected_components& external_connected_components,
 const Component_map& component_map, Ego_voxels* voxels) const {

  Ego_voxels_incidence_graph graph(*voxels);

  std::pair<vertex_iterator, vertex_iterator> vertices_pair = vertices(graph);
  vertex_iterator vertices_end = vertices_pair.second;
  for (vertex_iterator vertices_begin = vertices_pair.first;
       vertices_begin != vertices_end; ++vertices_begin) {
    
    if (external_connected_components.find(component_map[*vertices_begin]) ==
        external_connected_components.end())
      voxels->fill(*vertices_begin);
  }
}

SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELS_FILLER_TEMPLATE_HPP
