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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Switch.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Switch::s_tag = "Switch";
Container_proto * Switch::s_prototype = 0;

REGISTER_TO_FACTORY(Switch, "Switch");

/*! Constructor */
Switch::Switch(Boolean proto) :
  Group(proto),
  m_which_choice(static_cast<Uint>(-1))
{}

/*! Destructor */
Switch::~Switch() {}

/*! Return the node to traverse */
Node * Switch::get_choice()
{
  return (m_which_choice < m_childs.size()) ?
    get_child(m_which_choice) : NULL;
}

/*! Cull switch node.
 * If this node should be rendered, it added to Cull_context.
 * @param cull_context
 */
void Switch::cull(Cull_context & cull_context)
{
  if (!is_visible())  return;

  Node * node = get_choice();
  if (node) node->cull(cull_context);
}

/*! Draw choosen node.
 * @param draw_action
 */
Action::Trav_directive Switch::draw(Draw_action * draw_action)
{
  if (!is_visible())  return Action::TRAV_CONT;
  Node * node = get_choice();
  
  if (node) draw_action->apply(node);
  return Action::TRAV_CONT;
}

/*! Traverses the choosen node for selections.
 * @param isect_action
 */
void Switch::isect(Isect_action * isect_action) 
{
  if (!is_visible())  return;
  if (get_selection_id() != 0) isect_action->set_id(get_selection_id());
  
  Node * node = get_choice();
  if (node) isect_action->apply(node);
  isect_action->set_id(0);
}

/*! Calculate the sphere bound of the group based on all child objects.
 * \return true iff the bounding sphere has changed since last call.
 */
Boolean Switch::calculate_sphere_bound()
{
  if (!is_visible()) {
    if (m_sphere_bound.get_radius() == 0) return false;
    m_sphere_bound.set_radius(0);
    return true;
  }

  Node * node = get_choice();
  if (!node) return false;
  Sphere_bound_vector_const spheres;
  Boolean bb_changed = node->calculate_sphere_bound();
  const Sphere_bound & sb = node->get_sphere_bound();
  if (sb.get_radius() != 0) spheres.push_back(&sb);

  // If the bb was changed in the childobjects, or if the radius is 0
  // (which means that the object was invisible but not anymore)
  if (bb_changed || (m_sphere_bound.get_radius() == 0)) {
    m_sphere_bound.set_around(spheres);
    return true;
  }

  return false;
}

/*! Initializes the node prototype */
void Switch::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::set_sphere_bound_modified);
  s_prototype->add_field_info(new SF_int(WHICH_CHOICE, "whichChoice",
                                         get_member_offset(&m_which_choice),
                                         exec_func));
}

/*! */
void Switch::delete_prototype()
{
  delete s_prototype;
}

/*! */
Container_proto * Switch::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
}


/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Switch::set_attributes(Element * elem) 
{
  typedef Element::Str_attr_iter          Str_attr_iter;

  Group::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "whichChoice") {
      m_which_choice = strtoul(value.c_str(), NULL, 10);
      elem->mark_delete(ai);
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
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

SGAL_END_NAMESPACE
