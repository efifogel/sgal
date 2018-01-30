// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"

#include "FPG/Store.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief draws the children of the group.
Action::Trav_directive Group::draw(Draw_action* draw_action)
{
  if (!is_visible() || (draw_action == nullptr) ||
      (draw_action->get_context() == nullptr))
    return Action::TRAV_CONT;

  if (has_lights()) draw_action->get_context()->push_lights();
  Action::Trav_directive traversal_directive = Action::TRAV_CONT;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (!node) continue;
    traversal_directive = draw_action->apply(node);
    if (Action::TRAV_STOP == traversal_directive) break;
  }
  if (has_lights()) draw_action->get_context()->pop_lights();
  return traversal_directive;
}

/*! \brief culls the node if invisible; otherwise culls the children of the
 * group.
 */
void Group::cull(Cull_context& cull_context)
{
  if (!is_visible()) return;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (node) node->cull(cull_context);
  }
}

//! \brief draws the node for selection.
void Group::isect(Isect_action* isect_action)
{
  if (!is_visible()) return;

  // If the group has a touch sensor, reserve selections ids as many as
  // children.
  if (m_touch_sensor && m_touch_sensor->is_enabled()) {
    allocate_selection_ids();
    m_touch_sensor->set_selection_ids(m_start_selection_id,
                                      m_num_selection_ids);
  }

  // Apply the current Group selection ids only if selection ids have been
  // reserved for this Group. A start selection id that is equal to zero
  // indicates that no selection ids have been reserved.
  if (m_start_selection_id == 0) {
    for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
      auto node = boost::dynamic_pointer_cast<Node>(*it);
      if (node) isect_action->apply(node);
    }
  }
  else {
    Uint save_id = isect_action->get_id();                // save the id
    Uint selection_id = m_start_selection_id;
    for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
      auto node = boost::dynamic_pointer_cast<Node>(*it);
      if (node) {
        isect_action->set_id(selection_id);
        isect_action->apply(node);
      }
      ++selection_id;
    }
    isect_action->set_id(save_id);                        // restore the id
  }
}

/*! \brief cleans the bounding sphere of the group.
 * Computes the sphere that bounds all bounding spheres of all child nodes.
 * Notice that the m_dirty_bounding_sphere flag must be set right before the
 * return statement and not earlier, cause the calls to clean_bounding_sphere()
 * and get_bounding_sphere() may reset it.
 */
void Group::clean_bounding_sphere()
{
  if (m_locked_bounding_sphere) {
    m_dirty_bounding_sphere = false;
    return;
  }

  if (!is_visible()) {
    m_bounding_sphere.set_radius(0);
    m_dirty_bounding_sphere = false;
    return;
  }

  std::vector<const Bounding_sphere*> spheres;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (node) {
      if (node->is_dirty_bounding_sphere()) node->clean_bounding_sphere();
      const Bounding_sphere& sb = node->get_bounding_sphere();
      if (sb.get_radius() == 0) continue;
      spheres.push_back(&sb);
    }
  }

  m_bounding_sphere.set_around(spheres.begin(), spheres.end());
  m_dirty_bounding_sphere = false;
}

SGAL_END_NAMESPACE
