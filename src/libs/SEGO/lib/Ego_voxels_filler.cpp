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
#include "SEGO/Ego_voxels_filler_graph.hpp"

#include <boost/property_map/vector_property_map.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/connected_components.hpp>

#include <boost/unordered_set.hpp>

// #define EGO_VOXELIZER_FILLER_VERBOSE

SGAL_BEGIN_NAMESPACE

void Ego_voxels_filler::operator() (Ego_voxels* voxels) const {
  
  BOOST_CONCEPT_ASSERT((boost::GraphConcept<Ego_voxels_filler_graph>));
  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Ego_voxels_filler_graph>));
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Ego_voxels_filler_graph>));

  // Using connected components without EdgeListGraph
  // TODO: check if dfs is faster.
  
  typedef Ego_voxels_filler_graph::vertices_size_type
    vertices_size_type;
  typedef Ego_voxels_filler_graph::vertex_descriptor
    vertex_descriptor;
  typedef Ego_voxels_filler_graph::vertex_iterator
    vertex_iterator;


  Ego_voxels_filler_graph graph(*voxels);
  Ego_graph_vertex_index_map index_map(*voxels);

  boost::vector_property_map<vertices_size_type,
                             Ego_graph_vertex_index_map>
    component_map(index_map.max_index(), index_map);

#ifdef EGO_VOXELIZER_FILLER_VERBOSE
  std::cout << "connected components begin." << std::endl;
#endif

#ifdef EGO_VOXELIZER_FILLER_VERBOSE
  vertices_size_type num = 
    boost::connected_components(graph, component_map,
                                boost::vertex_index_map(index_map));
#else
    boost::connected_components(graph, component_map,
                                boost::vertex_index_map(index_map));
#endif

#ifdef EGO_VOXELIZER_FILLER_VERBOSE
  std::cout << "Number of connected components: " << num << std::endl;
#endif

  // get connected components of the boundary.
  boost::unordered_set<vertices_size_type> bound_connected_components;

  size_t x_max = voxels->voxels.size();
  size_t y_max = voxels->voxels[0].size();
  size_t z_max = voxels->voxels[0][0].size();

  // These are the boundary faces
  /// TODO: make boundary_vertices_iterator or something.
  size_t face[][3][2] = {{{0, 1}, {0, y_max}, {0, z_max}},
                         {{x_max-1, x_max}, {0, y_max}, {0, z_max}},
                         {{0, x_max}, {0, 1}, {0, z_max}},
                         {{0, x_max}, {y_max-1, y_max}, {0, z_max}},
                         {{0, x_max}, {0, y_max}, {0, 1}},
                         {{0, x_max}, {0, y_max}, {z_max-1, z_max}}};
  
  for (size_t f = 0; f < 6; ++f) {
    for (size_t i = face[f][0][0]; i < face[f][0][1]; ++i) {
      for (size_t j = face[f][1][0]; j < face[f][1][1]; ++j) {
        for (size_t k = face[f][2][0]; k < face[f][2][1]; ++k) {
          if (voxels->voxels[i][j][k] == false) {
            SGAL_assertion (i < voxels->voxels.size());
            SGAL_assertion (j < voxels->voxels[0].size());
            SGAL_assertion (k < voxels->voxels[0][0].size());

            bound_connected_components.
              insert(component_map[vertex_descriptor(i, j, k)]);
          }
        }
      }
    }
  }
  
  // Fill non-boundary components
  std::pair<vertex_iterator, vertex_iterator> verts =
    graph.vertices();

  size_t filled = 0;
  for (vertex_iterator it = verts.first; it != verts.second; ++it) {

    /// TODO: why does -> does not work here?
    size_t i = (*it).get<0>();
    size_t j = (*it).get<1>();
    size_t k = (*it).get<2>();

    SGAL_assertion (i < voxels->voxels.size());
    SGAL_assertion (j < voxels->voxels[0].size());
    SGAL_assertion (k < voxels->voxels[0][0].size());
    
    // If it is not filled and it is not a boundary
    if (voxels->voxels[i][j][k] == false) {
      if (bound_connected_components.find(component_map[*it]) ==
          bound_connected_components.end()) {        
        voxels->voxels[i][j][k] = true;
        ++filled;
      }
    }
  }

#ifdef EGO_VOXELIZER_FILLER_VERBOSE
  std::cout << "Filled " << filled << " voxels" << std::endl;
#endif
}
  
SGAL_END_NAMESPACE
