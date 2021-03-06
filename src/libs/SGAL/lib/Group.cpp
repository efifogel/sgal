// Copyright (c) 2004 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Ram Shaked        <ramsha7@gmail.com>

#include <algorithm>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Group::s_prototype(nullptr);
const std::string Group::s_tag = "Group";

const Vector3f Group::s_def_bbox_center(0, 0, 0);
const Vector3f Group::s_def_bbox_size(-1, -1, -1);

REGISTER_TO_FACTORY(Group, "Group");

//! \brief constructor.
Group::Group(Boolean proto) :
  Node(proto),
  m_is_visible(true),
  m_num_lights(0),
  m_start_selection_id(0),
  m_num_selection_ids(0),
  m_scene_graph(nullptr),
  m_dirty_childs(false),
  m_dirty_other(false),
  m_bbox_center(s_def_bbox_center),
  m_bbox_size(s_def_bbox_size),
  m_locked_bounding_sphere_center(false),
  m_locked_bounding_sphere_radius(false)
{}

//! \brief cleans the children in case they are dirty.
void Group::clean_childs() { m_dirty_childs = false; }

//! \brief destructor.
Group::~Group()
{
  // Unregister observers
  Observer observer(this, get_field_info(BOUNDING_SPHERE));
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
    (*it)->unregister_observer(observer);

  // Clear children
  m_childs.clear();

  // Clear selection ids
  if (m_num_selection_ids > 0) {
    m_scene_graph->free_selection_ids(m_start_selection_id,
                                      m_num_selection_ids);
    m_start_selection_id = 0;
    m_num_selection_ids = 0;
  }
}

//! \brief obtains a child according to its position in the children array.
Group::Shared_container Group::get_child(Uint index)
{
  if (m_dirty_childs) clean_childs();
  if (index >= m_childs.size()) return Group::Shared_container();
  return m_childs[index];
}

//! Set the children array.
void Group::set_childs(Container_array& childs)
{
  //! \todo need to:
  // 1. For each light add_light()
  // 2. For each touch sensors add_touch_sensor()
  // 3. If there are other nodes then,
  // 3.1  for each register_observer()
  // 3.2  field_changed(get_field_info(BOUNDING_SPHERE));
  SGAL_error_msg("Not implemented yet!");
  m_dirty_other = true;
  m_childs = childs;
}

//! \brief obtains the (const) children array.
const Group::Container_array& Group::get_childs() const { return m_childs; }

//! \brief obtain the (non-const) children array.
Group::Container_array& Group::get_childs() { return m_childs; }

//! \brief adds a child to the sequence of children of the group.
void Group::add_child(Shared_container node)
{
  // Lights are inserted at the begining of the sequence, the engines, and
  // then all the rest.
  auto light = boost::dynamic_pointer_cast<Light>(node);
  if (light) {
    add_light(light);
    m_dirty_other = true;
    return;
  }
  auto touch_sensor = boost::dynamic_pointer_cast<Touch_sensor>(node);
  if (touch_sensor) {
    add_touch_sensor(touch_sensor);
    m_dirty_other = true;
    return;
  }

  m_childs.push_back(node);
  const auto* field_info = get_field_info(BOUNDING_SPHERE);
  Observer observer(this, field_info);
  node->register_observer(observer);
  field_changed(field_info);
  m_dirty_other = true;
}

//! \brief removes a given child from the sequence of children of the group.
void Group::remove_child(Shared_container node)
{
  remove_child(node, true);
}

void Group::remove_child(Shared_container node, bool invoke_field_changed)
{
  auto light = boost::dynamic_pointer_cast<Light>(node);
  if (light) {
    remove_light(light);
    return;
  }

  auto touch_sensor =
    boost::dynamic_pointer_cast<Touch_sensor>(node);
  if (touch_sensor) {
    remove_touch_sensor(touch_sensor);
    return;
  }

  const auto* field_info = get_field_info(BOUNDING_SPHERE);
  Observer observer(this, field_info);
  node->unregister_observer(observer);
  m_childs.erase(std::remove(m_childs.begin(), m_childs.end(), node),
                 m_childs.end());
  if (invoke_field_changed)
    field_changed(field_info);
}

void Group::clear_childs() {
  while (m_childs.size() > 1)
    remove_child(m_childs.back(), false); // don't invoke field-changed.
  remove_child(m_childs.back(), true);    // last remove -> invoke field-changed.
}

//! \brief traverses the children of the group.
Action::Trav_directive Group::traverse(Action* action)
{
  if (m_dirty_childs) clean_childs();
  if (!is_visible() || (action == nullptr)) return Action::TRAV_CONT;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (!node) continue;
    auto traversal_directive = action->apply(node);
    if (Action::TRAV_STOP == traversal_directive) return traversal_directive;
  }
  return Action::TRAV_CONT;
}

//! \brief draws the children of the group.
Action::Trav_directive Group::draw(Draw_action* draw_action)
{
  if (m_dirty_childs) clean_childs();
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
  if (m_dirty_childs) clean_childs();
  if (!is_visible()) return;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (node) node->cull(cull_context);
  }
}

//! \brief allocates the selection ids for this group.
void Group::allocate_selection_ids()
{
  if ((m_num_selection_ids != 0) && m_num_selection_ids != children_size()) {
    m_scene_graph->free_selection_ids(m_start_selection_id,
                                      m_num_selection_ids);
    m_start_selection_id = 0;
    m_num_selection_ids = 0;
  }

  if (m_num_selection_ids == 0) {
    m_num_selection_ids = children_size();
    m_start_selection_id =
      m_scene_graph->allocate_selection_ids(m_num_selection_ids);
  }
}

//! \brief draws the node for selection.
void Group::isect(Isect_action* isect_action)
{
  if (m_dirty_childs) clean_childs();
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
  SGAL_TRACE_CODE(Tracer::BOUNDING_SPHERE,
                  std::cout << "Group::clean " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  if (m_dirty_childs) clean_childs();

  if (!is_visible()) {
    m_bounding_sphere.set_radius(0);
    m_dirty_bounding_sphere = false;
    return;
  }

  std::vector<const Bounding_sphere*> spheres;
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = boost::dynamic_pointer_cast<Node>(*it);
    if (node) {
      const Bounding_sphere& sb = node->get_bounding_sphere();
      if (sb.get_radius() == 0) continue;
      spheres.push_back(&sb);
    }
  }

  if (m_locked_bounding_sphere_center || m_locked_bounding_sphere_radius) {
    Bounding_sphere bs;
    bs.set_around(spheres.begin(), spheres.end());
    if (! m_locked_bounding_sphere_center)
      m_bounding_sphere.set_center(bs.get_center());
    if (! m_locked_bounding_sphere_radius)
      m_bounding_sphere.set_radius(bs.get_radius());
  }
  else m_bounding_sphere.set_around(spheres.begin(), spheres.end());
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of the group.
void Group::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "visible") {
      if (!compare_to_true(value))
        set_invisible();
      elem->mark_delete(ai);
    }
    if (name == "bboxCenter") {
      m_bounding_sphere.set_center(value);
      m_locked_bounding_sphere_center = true;
      if (m_locked_bounding_sphere_radius) m_dirty_bounding_sphere = false;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxSize") {
      Vector3f vec(value);
      float radius = vec.min() * 0.5f;
      m_bounding_sphere.set_radius(radius);
      m_locked_bounding_sphere_radius = true;
      if (m_locked_bounding_sphere_center) m_dirty_bounding_sphere = false;
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "children") {
      Shared_container node = boost::dynamic_pointer_cast<Container>(cont);
      if (node) add_child(node);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "children") {
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++)
        add_child(*ci);
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief initializes the node prototype.
void Group::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  auto exec_func =
    static_cast<Execution_function>(&Node::bounding_sphere_changed);

  // visible
  auto is_visible_func =
    static_cast<Boolean_handle_function>(&Group::is_visible_handle);
  s_prototype->add_field_info(new SF_bool(IS_VISIBLE, "visible",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          is_visible_func, true, exec_func));

  // children
  auto childs_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Group::childs_handle);
  s_prototype->add_field_info(new MF_shared_container(CHILDREN, "children",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      childs_func, exec_func));
}

//! \brief deletes the node prototype.
void Group::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Group::get_prototype()
{
  if (!s_prototype) Group::init_prototype();
  return s_prototype;
}

//! \brief .
Boolean Group::attach_context(Context* context)
{
  if (m_dirty_childs) clean_childs();
  Boolean result = Node::attach_context(context);
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
    result &= (*it)->attach_context(context);
  return result;
}

//! \brief .
Boolean Group::detach_context(Context* context)
{
  if (m_dirty_childs) clean_childs();
  Boolean result = Node::detach_context(context);
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
    result &= (*it)->detach_context(context);
  return result;
}

//! \brief writes this container.
void Group::write(Formatter* formatter)
{
  if (m_dirty_childs) clean_childs();
  SGAL_TRACE_CODE(Tracer::EXPORT,
                  std::cout << "Group: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  if (! is_visible() && ! formatter->get_export_non_visible()) return;

  Container::write(formatter);
}

/*! \brief turns on the flag that indicates whether the shape should be
 * rendered.
 */
void Group::set_visible()
{
  if (!m_is_visible) {
    m_is_visible = true;
    m_dirty_bounding_sphere = true;
  }
}

/*! \brief turns off the flag that indicates whether the shape should be
 * rendered.
 */
void Group::set_invisible()
{
  if (m_is_visible) {
    m_is_visible = false;
    m_dirty_bounding_sphere = true;
  }
}

//! \brief sets the flag that indicates whether the shape should be rendered.
void Group::set_visible(Boolean flag)
{
  if (flag != m_is_visible) {
    m_is_visible = flag;
    m_dirty_bounding_sphere = true;
  }
}

//! \brief adds a light source to the group.
void Group::add_light(Shared_light light)
{
  // Insert the light at the front of the array.
  m_childs.insert(m_childs.begin(), light);
  ++m_num_lights;
}

//! \brief removes a light source from the group.
void Group::remove_light(Shared_light light)
{
  m_childs.erase(std::remove(m_childs.begin(), m_childs.end(), light),
                 m_childs.end());
  --m_num_lights;
}

//! \brief adds a touch sensor to the group.
void Group::add_touch_sensor(Shared_touch_sensor touch_sensor)
{
  if (m_touch_sensor) {
    std::cerr << "The Group already has a touch sensor!" << std::endl;
    return;
  }
  m_touch_sensor = touch_sensor;
  m_childs.push_back(touch_sensor);
}

//! \brief removes a touch sensor from the group.
void Group::remove_touch_sensor(Shared_touch_sensor touch_sensor)
{
  m_touch_sensor.reset();
  m_childs.erase(std::remove(m_childs.begin(), m_childs.end(), touch_sensor),
                 m_childs.end());
}

//! \brief processes change of field.
void Group::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case BOUNDING_SPHERE:
    SGAL_TRACE_CODE(Tracer::BOUNDING_SPHERE,
                    std::cout << "Group::dirty true " << "Tag: " << get_tag()
                    << ", name: " << get_name()
                    << std::endl;);
    if (! m_locked_bounding_sphere_center || ! m_locked_bounding_sphere_radius)
      m_dirty_bounding_sphere = true;
    break;

   default: break;
  }
  Node::field_changed(field_info);
}

SGAL_END_NAMESPACE
