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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <algorithm>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Stl_formatter.hpp"

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
  m_bbox_center(s_def_bbox_center),
  m_bbox_size(s_def_bbox_size)
{}

//! \brief copy constructor.
Group::Group(const Group& other) :
  Node(other),
  m_is_visible(other.m_is_visible),
  m_touch_sensor(other.m_touch_sensor),
  m_num_lights(other.m_num_lights),
  m_start_selection_id(0),
  m_num_selection_ids(0),
  m_scene_graph(other.m_scene_graph)
{ m_childs = other.m_childs; }

//! \brief destructor.
Group::~Group()
{
  // Unregister observers
  Observer observer(this, get_field_info(SPHERE_BOUND));
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
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
Group::Shared_node Group::get_child(Uint index)
{
  if (index >= m_childs.size()) return Group::Shared_node();
  return m_childs[index];
}

//! \brief adds a child to the sequence of children of the group.
void Group::add_child(Shared_node node)
{
  // Lights are inserted at the begining of the sequence, the engines, and
  // then all the rest.
  Shared_light light = boost::dynamic_pointer_cast<Light>(node);
  if (light) {
    add_light(light);
    return;
  }
  Shared_touch_sensor touch_sensor =
    boost::dynamic_pointer_cast<Touch_sensor>(node);
  if (touch_sensor) {
    add_touch_sensor(touch_sensor);
    return;
  }

  m_childs.push_back(node);
  m_dirty_sphere_bound = true;
  Observer observer(this, get_field_info(SPHERE_BOUND));
  node->register_observer(observer);
}

//! \brief removes a given child from the sequence of children of the group.
void Group::remove_child(Shared_node node)
{
  Shared_light light = boost::dynamic_pointer_cast<Light>(node);
  if (light) {
    remove_light(light);
    return;
  }

  Shared_touch_sensor touch_sensor =
    boost::dynamic_pointer_cast<Touch_sensor>(node);
  if (touch_sensor) {
    remove_touch_sensor(touch_sensor);
    return;
  }

  Observer observer(this, get_field_info(SPHERE_BOUND));
  node->unregister_observer(observer);
  m_dirty_sphere_bound = true;
  m_childs.erase(std::remove(m_childs.begin(), m_childs.end(), node),
                 m_childs.end());
}

//! \brief draws the children of the group.
Action::Trav_directive Group::draw(Draw_action* draw_action)
{
  if (!is_visible() || (draw_action == 0) || (draw_action->get_context() == 0))
    return Action::TRAV_CONT;
  if (has_lights()) draw_action->get_context()->push_lights();
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    draw_action->apply(&*(*it));
  if (has_lights()) draw_action->get_context()->pop_lights();
  return Action::TRAV_CONT;
}

/*! \brief culls the node if invisible; otherwise culls the children of the
 * group.
 */
void Group::cull(Cull_context& cull_context)
{
  if (!is_visible()) return;
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    (*it)->cull(cull_context);
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
    for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
      isect_action->apply(&*(*it));
  }
  else {
    Uint save_id = isect_action->get_id();                // save the id
    Uint selection_id = m_start_selection_id;
    for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
      isect_action->set_id(selection_id++);
      isect_action->apply(&*(*it));
    }
    isect_action->set_id(save_id);                        // restore the id
  }
}

/*! \brief cleans the bounding sphere of the group.
 * Computes the sphere that bounds all bounding spheres of all child nodes.
 * Notice that the m_dirty_sphere_bound flag must be set right before the
 * return statement and not earlier, cause the calls to clean_sphere_bound()
 * and get_sphere_bound() may reset it.
 */
Boolean Group::clean_sphere_bound()
{
  if (m_locked_sphere_bound) {
    m_dirty_sphere_bound = false;
    return false;
  }

  bool res = false;

  if (!is_visible()) {
    if (m_sphere_bound.get_radius() != 0) {
      m_sphere_bound.set_radius(0);
      res = true;
    }
    m_dirty_sphere_bound = false;
    return res;
  }

  Sphere_bound_vector_const spheres;
  Boolean bb_changed = false;
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
    Boolean changed = false;
    if ((*it)->is_dirty_sphere_bound())
      changed = (*it)->clean_sphere_bound();
    const Sphere_bound& sb = (*it)->get_sphere_bound();
    if (sb.get_radius() == 0) continue;
    spheres.push_back(&sb);
    bb_changed = bb_changed || changed;
  }

  // If the bb was changed in the child objects, or if the radius is 0
  // (which means that the object was invisible but not anymore)
  if (bb_changed || (m_sphere_bound.get_radius() == 0)) {
    m_sphere_bound.set_around(spheres);
    res = true;
  }

  m_dirty_sphere_bound = false;
  return res;
}

//! \brief sets the attributes of the group.
void Group::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "visible") {
      if (!compare_to_true(value))
        set_invisible();
      elem->mark_delete(ai);
    }
    if (name == "bboxCenter") {
      m_sphere_bound.set_center(value);
      m_dirty_sphere_bound = false;
      m_locked_sphere_bound = true;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxSize") {
      Vector3f vec(value);
      float radius = vec.length();
      m_sphere_bound.set_radius(radius);
      m_dirty_sphere_bound = false;
      m_locked_sphere_bound = true;
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Shared_container cont = elem->get_value(cai);
    if (name == "children") {
      Shared_node node = boost::dynamic_pointer_cast<Node>(cont);
      if (node) add_child(node);
      elem->mark_delete(cai);
      continue;
    }
  }

  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "children") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Shared_container cont = *ci;
        Shared_node node = boost::dynamic_pointer_cast<Node>(cont);
        if (node) add_child(node);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
//! \brief gets the attributes of the object.
Attribute_list Group::get_attributes()
{
  Attribute_list attrs;
  attrs = Node::get_attributes();
  Attribue attrib;

  if (m_is_visible != true) {
    attrib.first = "visible";
    attrib.second = "FALSE";
    attrs.push_back(attrib);
  }

  return attrs;
}
#endif

//! \brief initializes the node prototype.
void Group::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  // visible
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::sphere_bound_changed);
  Boolean_handle_function is_visible_func =
    static_cast<Boolean_handle_function>(&Group::is_visible_handle);
  s_prototype->add_field_info(new SF_bool(IS_VISIBLE, "visible",
                                          RULE_EXPOSED_FIELD,
                                          is_visible_func, true, exec_func));

  // children
  Shared_container_array_handle_function childs_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Group::childs_handle);
  s_prototype->add_field_info(new MF_shared_container(CHILDREN, "children",
                                                      RULE_EXPOSED_FIELD,
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
  Boolean result = Node::attach_context(context);
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    result &= (*it)->attach_context(context);
  return result;
}

//! \brief .
Boolean Group::detach_context(Context* context)
{
  Boolean result = Node::detach_context(context);
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    result &= (*it)->detach_context(context);
  return result;
}

//! \brief writes this container.
void Group::write(Formatter* formatter)
{
  Stl_formatter* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  if (stl_formatter) {
    if (!is_visible()) return;
  }

  Container::write(formatter);
}

/*! \brief turns on the flag that indicates whether the shape should be
 * rendered.
 */
void Group::set_visible()
{
  if (!m_is_visible) {
    m_is_visible = true;
    m_dirty_sphere_bound = true;
  }
}

/*! \brief turns off the flag that indicates whether the shape should be
 * rendered.
 */
void Group::set_invisible()
{
  if (m_is_visible) {
    m_is_visible = false;
    m_dirty_sphere_bound = true;
  }
}

//! \brief sets the flag that indicates whether the shape should be rendered.
void Group::set_visible(Boolean flag)
{
  if (flag != m_is_visible) {
    m_is_visible = flag;
    m_dirty_sphere_bound = true;
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
   case SPHERE_BOUND:
    m_dirty_sphere_bound = true;
    break;
   default: break;
  }
  Node::field_changed(field_info);
}

SGAL_END_NAMESPACE
