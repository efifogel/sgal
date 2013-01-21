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
// $Source$
// $Revision: 12384 $
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

SGAL_BEGIN_NAMESPACE

Container_proto* Group::s_prototype = 0;
std::string Group::s_tag = "sgalGroup";

REGISTER_TO_FACTORY(Group, "Group");

/*! Constructor */
Group::Group(Boolean proto) :
  Node(proto),
  m_is_visible(true),
  m_selection_id(0), 
  m_has_touch_sensor(false), 
  m_has_light(false)
{
}

/*! Copy constructor */
Group::Group(const Group& group) :
  Node(group),
  m_is_visible(group.m_is_visible),
  m_selection_id(group.m_selection_id), 
  m_has_touch_sensor(m_has_touch_sensor), 
  m_has_light(group.m_has_light)
{
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
    Node* node = *it;
    m_childs.push_back(node);
  }
}

/*! Destructor */
Group::~Group() { m_childs.clear(); }

/*! Return the number of children the object has.
 * \return number of children in the group.
 */
unsigned int Group::get_child_count() { return m_childs.size(); }

/*! Get a child according to its position in the child list.
 * \param index the index of the child.
 * \return a pointer to the child object.
 */
Node* Group::get_child(unsigned int index)
{
  if (index >= m_childs.size()) return 0;
  Node_iterator ni = m_childs.begin();
  for (unsigned int i = 0; i < index; ++i) ++ni;
  return *ni;
}

/*! Add a child to the list of children. 
 * In the process of adding child objects to a group, we sort the child objects
 * in such way that the lights always appear at the begining of the list and
 * all other child objects appear afterwards.
 * \param node (in) the child object
 */
void Group::add_child(Node* node)
{
  // if the child is a light, we need to add it at the beginning of the list
  // FIX - when engines are also implemented, we need to make sure they
  // apppera after the lights and before the other nodes.
  Light* light = dynamic_cast<Light*>(node);
  if (light) {
    m_childs.push_front(node);
    set_has_light();
    return;
  }
  Touch_sensor* touch_sensor = dynamic_cast<Touch_sensor*>(node);
  if (touch_sensor) {
    //! \todo it is more natural to keep the id only at the touch sensor itself
    Uint id = touch_sensor->get_first_selection_id();
    set_has_touch_sensor(id);
  }
  m_childs.push_back(node);
  m_dirty_sphere_bound = true;

  Observer observer(this, get_field_info(SPHERE_BOUND));
  node->register_observer(observer);
}

/*! Remove a given child 
 * \param node (in) the child object
 * \todo need to update has_lights and has_touch_sensor
 */
void Group::remove_child(Node* node)
{
  m_childs.remove(node);
  m_dirty_sphere_bound = true;

  Observer observer(this, get_field_info(SPHERE_BOUND));
  node->register_observer(observer);
}

/*! \brief draws the group; go over all of its children and draw each. */
Action::Trav_directive Group::draw(Draw_action* draw_action)
{
  if (!is_visible() || (draw_action == 0) || (draw_action->get_context() == 0))
    return Action::TRAV_CONT;
  if (m_has_light) draw_action->get_context()->push_lights();
  for (Node_iterator ni = m_childs.begin(); ni != m_childs.end(); ++ni)
    draw_action->apply(*ni);
  if (m_has_light) draw_action->get_context()->pop_lights();
  return Action::TRAV_CONT;
}

/*! \brief culls the node if invisible and prepare for rendering. */
void Group::cull(Cull_context& cull_context)
{
  if (!is_visible()) return;

  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) 
    (*it)->cull(cull_context);
}

/*! \brief prepare the node for selection. */
void Group::isect(Isect_action* isect_action) 
{
  if (!is_visible()) return;

  if (get_selection_id() != 0) isect_action->set_id(get_selection_id());
  for (Node_iterator ni = m_childs.begin(); ni != m_childs.end(); ++ni)
    isect_action->apply(*ni);
  isect_action->set_id(0);
}

/*! \brief cleans the bounding sphere of the group.
 * Computes the sphere that bounds all bounding spheres of all child nodes.
 */
Boolean Group::clean_sphere_bound()
{
  if (m_locked_sphere_bound) return false;
  m_dirty_sphere_bound = false;
  
  if (!is_visible()) {
    if (m_sphere_bound.get_radius() == 0) return false;
    m_sphere_bound.set_radius(0);
    return true;
  }

  Sphere_bound_vector_const spheres;
  Boolean bb_changed = false;
  for (Node_iterator ni = m_childs.begin(); ni != m_childs.end(); ++ni) {
    Boolean changed = false;
    if ((*ni)->is_dirty_sphere_bound())
      changed = (*ni)->clean_sphere_bound();
    const Sphere_bound& sb = (*ni)->get_sphere_bound();
    if (sb.get_radius() == 0) continue;
    spheres.push_back(&sb);
    bb_changed = bb_changed || changed;
  }

  // If the bb was changed in the child objects, or if the radius is 0
  // (which means that the object was invisible but not anymore)
  if (bb_changed || (m_sphere_bound.get_radius() == 0)) {
    m_sphere_bound.set_around(spheres);
    return true;
  }

  return false;
}

/* */
Boolean Group::does_have_touch_sensor() { return m_has_touch_sensor; }

/*! Sets a flag indicating that the group has a touch sensor
 * and sets the selection id. The selection id is used as a 
 * color to draw the object in selection mode.
 * @param id the selection id.
 */
void Group::set_has_touch_sensor(unsigned int id)
{
  m_has_touch_sensor = true;
  m_selection_id = id;
}

/*! \brief sets a flag indicating whether the group has a light source. */
void Group::set_has_light() { m_has_light = true; }

/*! \brief sets the attributes of the group. */
void Group::set_attributes(Element* elem) 
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "visible") {
      if (!compare_to_true(value))
        set_invisible();
      elem->mark_delete(ai);
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "children") {
      Node* node = dynamic_cast<Node*>(cont);
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
       mcai != elem->multi_cont_attrs_end(); mcai++)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "children") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Container* cont = *ci;
        Node* node = dynamic_cast<Node*>(cont);
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
/*! \brief gets the attributes of the object. */
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

/*! \brief initializes the node prototype. */
void Group::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance.
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::sphere_bound_changed);
  SF_bool* field_info = new SF_bool(ISVISIBLE, "visible",
                                    get_member_offset(&m_is_visible),
                                    exec_func);
  s_prototype->add_field_info(field_info);
}

/*! \brief deletes the node prototype */
void Group::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Group::get_prototype() 
{  
  if (!s_prototype) Group::init_prototype();
  return s_prototype;
}

/*! */
Boolean Group::attach_context(Context* context)
{
  Boolean result = Node::attach_context(context);
  for (Node_iterator ni = m_childs.begin(); ni != m_childs.end(); ++ni) {
    result &= (*ni)->attach_context(context);
  }
  return result;
}

/*! */
Boolean Group::detach_context(Context* context)
{
  Boolean result = Node::detach_context(context);
  for (Node_iterator ni = m_childs.begin(); ni != m_childs.end(); ++ni) {
    result &= (*ni)->detach_context(context);
  }
  return result;
}

/*! \brief writes this container. */
void Group::write(Formatter* formatter)
{
  formatter->container_begin(get_tag());
  Group::write_children(formatter);
  formatter->container_end();
}

/*! \brief writes the children. */
void Group::write_children(Formatter* formatter)
{
  formatter->multi_container_begin("children");
  for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
    Node* node = *it;
    formatter->write(node);
  }
  formatter->multi_container_end();
}

void Group::set_visible()
{
  if (!m_is_visible) {
    m_is_visible = true;
    m_dirty_sphere_bound = true;
  }
}

void Group::set_invisible()
{
  if (m_is_visible) {
    m_is_visible = false;
    m_dirty_sphere_bound = true;
  }
}

SGAL_END_NAMESPACE
