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

#include "SGAL/basic.hpp"
#include "SGAL/Switch.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Stl_binary_formatter.hpp"
#include "SGAL/Obj_formatter.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Switch::s_tag = "Switch";
Container_proto* Switch::s_prototype(nullptr);

const Uint Switch::s_def_which_choice(static_cast<Uint>(-1));

REGISTER_TO_FACTORY(Switch, "Switch");

//! \brief Constructs.
Switch::Switch(Boolean proto) :
  Group(proto),
  m_which_choice(s_def_which_choice)
{}

//! \brief destructs.
Switch::~Switch() {}

//! \brief obtains the node to traverse.
Switch::Shared_container Switch::get_choice()
{
  return (m_which_choice < m_childs.size()) ?
    get_child(m_which_choice) : Shared_node();
}

//! \brief applies culling on the choosen node, ignores the rest.
void Switch::cull(Cull_context& cull_context)
{
  if (!is_visible())  return;

  auto node = boost::dynamic_pointer_cast<Node>(get_choice());
  if (node) node->cull(cull_context);
}

//! \brief traverses the selected child of the switch node.
Action::Trav_directive Switch::traverse(Action* action)
{
  if (!is_visible())  return Action::TRAV_CONT;
  auto node = boost::dynamic_pointer_cast<Node>(get_choice());
  if (!node) return Action::TRAV_CONT;
  return action->apply(node);
}

//! \brief applies drawing on the choosen node, ignores the rest.
Action::Trav_directive Switch::draw(Draw_action* draw_action)
{
  if (!is_visible())  return Action::TRAV_CONT;
  auto node = boost::dynamic_pointer_cast<Node>(get_choice());
  if (node) draw_action->apply(node);
  return Action::TRAV_CONT;
}

//! \brief applies selection on the choosen node, ignores the rest.
void Switch::isect(Isect_action* isect_action)
{
  if (!is_visible()) return;
  auto node = boost::dynamic_pointer_cast<Node>(get_choice());
  if (!node) return;

  if (m_touch_sensor && m_touch_sensor->is_enabled()) {
    if (m_num_selection_ids != 1) {
      m_scene_graph->free_selection_ids(m_start_selection_id,
                                        m_num_selection_ids);
      m_start_selection_id = 0;
      m_num_selection_ids = 0;
    }

    if (m_num_selection_ids == 0) {
      m_num_selection_ids = 1;
      m_start_selection_id =
        m_scene_graph->allocate_selection_ids(m_num_selection_ids);
    }
    m_touch_sensor->set_selection_ids(m_start_selection_id,
                                      m_num_selection_ids);
  }
  if (m_start_selection_id == 0) isect_action->apply(node);
  else {
    Uint save_id = isect_action->get_id();                // save the id
    isect_action->set_id(m_start_selection_id);
    isect_action->apply(node);
    isect_action->set_id(save_id);                        // restore the id
  }
}

/*! \brief cleans the sphere bound of the group based on all child objects.
 * Notice that the m_dirty_bounding_sphere flag must be set right before the
 * return statement and not earlier, cause the calls to clean_bounding_sphere()
 * and get_bounding_sphere() may reset it.
 */
void Switch::clean_bounding_sphere()
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

  auto node = boost::dynamic_pointer_cast<Node>(get_choice());
  if (node) {
    if (node->is_dirty_bounding_sphere()) node->clean_bounding_sphere();
    const auto& sb = node->get_bounding_sphere();
    if (sb.get_radius() != 0) {
      m_bounding_sphere.set_center(sb.get_center());
      m_bounding_sphere.set_radius(sb.get_radius());
    }
  }

  m_dirty_bounding_sphere = false;
}

//! \brief initializes the node prototype.
void Switch::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // Add the field-info records to the prototype:
  // whichChoice
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::bounding_sphere_changed);
  Uint_handle_function which_choice_func =
    static_cast<Uint_handle_function>(&Switch::which_choice_handle);
  s_prototype->add_field_info(new SF_uint(WHICH_CHOICE, "whichChoice",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          which_choice_func,
                                          s_def_which_choice,
                                          exec_func));
}

//! brief
void Switch::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! brief
Container_proto* Switch::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}


//! \brief sets the attributes of the object.
void Switch::set_attributes(Element* elem)
{
  Group::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "whichChoice") {
      m_which_choice = strtoul(value.c_str(), NULL, 10);
      elem->mark_delete(ai);
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! */
Attribute_list Switch::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  Vector3f vec;
  Rotation rot;

  attribs = Group::get_attributes();

  if (m_which_choise >= 0) {
    attrib.first = "whichChoice";
    attrib.second = String::fromInteger(m_which_choise);
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief exports this container.
void Switch::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Switch: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);
  auto* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  auto* obj_formatter = dynamic_cast<Obj_formatter*>(formatter);
  if (stl_formatter || obj_formatter) {
    Shared_node node = boost::dynamic_pointer_cast<Node>(get_choice());
    if (node) node->write(formatter);
    return;
  }
  auto* stl_binary_formatter = dynamic_cast<Stl_binary_formatter*>(formatter);
  if (stl_binary_formatter) {
    Shared_node node = boost::dynamic_pointer_cast<Node>(get_choice());
    if (node) stl_binary_formatter->write(node);
    return;
  }

  Group::write(formatter);
}

SGAL_END_NAMESPACE
