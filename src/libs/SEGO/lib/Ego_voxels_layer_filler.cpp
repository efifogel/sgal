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

#include "SEGO/Ego_voxels_layer_filler.hpp"
#include "SEGO/Ego_voxels.hpp"
#include "SEGO/Ego_voxels_layer_filler_graph.hpp"
#include "SEGO/Ego_voxels_filler_template.hpp"

#include <boost/tuple/tuple_comparison.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/connected_components.hpp>

#include <boost/property_map/vector_property_map.hpp>
#include <boost/unordered_set.hpp>

// #define EGO_VOXELIZER_LAYER_FILLER_VERBOSE

SGAL_BEGIN_NAMESPACE

/*! The tag that identifies this container type */
std::string Ego_voxels_layer_filler::s_tag = "EgoVoxelsLayerFiller";

/* Constructor */
Ego_voxels_layer_filler::Ego_voxels_layer_filler(bool prototype)
    : Ego_voxels_filler_base(prototype) {}

/* Construct the prototype */
Ego_voxels_layer_filler* Ego_voxels_layer_filler::prototype() {
  return new Ego_voxels_layer_filler(true);
}

/*! Clone. */
Container* Ego_voxels_layer_filler::clone() {
  return new Ego_voxels_layer_filler();
}

/*! Obtain the tag (type) of the container. */
const std::string& Ego_voxels_layer_filler::get_tag() const {
  return s_tag;
}


void Ego_voxels_layer_filler::fill(Ego_voxels* voxels) const {
  Ego_voxels_filler_template<Ego_voxels_layer_filler_graph> filler(m_offset);
  filler(voxels);
}
  
SGAL_END_NAMESPACE
