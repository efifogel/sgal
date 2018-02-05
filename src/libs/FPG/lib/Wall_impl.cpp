// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"

#include "FPG/Wall.hpp"

SGAL_BEGIN_NAMESPACE

//! Associate the node engine with a scene graph.
void Wall::add_to_scene(Scene_graph* scene_graph)
{ m_scene_graph = scene_graph; }

//! \brief cleans the geometry.
void Wall::clean_geometry()
{
}

SGAL_END_NAMESPACE
