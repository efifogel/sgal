// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"

#include "FPG/Counter_deck.hpp"

SGAL_BEGIN_NAMESPACE

//! Associate the node engine with a scene graph.
void Counter_deck::add_to_scene(Scene_graph* scene_graph)
{ m_scene_graph = scene_graph; }

//! \brief cleans the geometry.
void Counter_deck::clean_geometry()
{
}

SGAL_END_NAMESPACE
