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

#include "SGAL/Proximity_sensor.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Boolean Proximity_sensor::s_def_enabled = true;

const std::string Proximity_sensor::s_tag = "ProximitySensor";
Container_proto* Proximity_sensor::s_prototype(NULL);

REGISTER_TO_FACTORY(Proximity_sensor, "Proximity_sensor");

/*! Constructor */
Proximity_sensor::Proximity_sensor(Boolean enabled, Boolean proto) :
  Node(proto),
  m_scene_graph(0),
  m_enabled(enabled)
{ if (!proto && m_enabled) register_events(); }

/*! Destructor */
Proximity_sensor::~Proximity_sensor() { if (m_enabled) unregister_events(); }

/*! \brief initializes the prototype. */
void Proximity_sensor::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  // enabled
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Proximity_sensor::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          enabled_func));

  // position
  Vector3f_handle_function position_func =
    static_cast<Vector3f_handle_function>(&Proximity_sensor::position_handle);
  s_prototype->add_field_info(new SF_vector3f(POSITION, "position",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              position_func));

  // orientation
  Rotation_handle_function orientation_func =
    static_cast<Rotation_handle_function>
    (&Proximity_sensor::orientation_handle);
  s_prototype->add_field_info(new SF_rotation(ORIENTATION, "orientation",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              orientation_func));
}

/*! \brief deletes the prototype. */
void Proximity_sensor::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Proximity_sensor::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! Enable or disable the sensor */
void Proximity_sensor::set_enabled(bool enabled)
{
  m_enabled = enabled;
  (enabled) ? register_events() : unregister_events();
}

/*! \brief sets the attributes of the object extracted from the input file */
void Proximity_sensor::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      set_enabled(compare_to_true(value));
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */
void Proximity_sensor::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

#if 0
/*! \brief obtains a list of atributes in this object. This method is called
 * only from the Builder side.
 *
 * \return a list of attributes
 */
Attribute_list Proximity_sensor::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  if (m_enabled != s_def_enabled) {
    attrib.first = "enabled";
    attrib.second = (m_enabled) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  return attribs;
};

/*! \brief adds a touch sensor object to the scene.
 * This includes adding it to the array of touch sensors in the
 * scene graph and setting a flag in the group indicating it has
 * a touch sensor.
 * @param sg (in) a reference to the scene graph
 * @param parent (in) a pointer to the parent object.
 */
void Proximity_sensor::add_to_scene(Scene_graph* sg)
{
  Node::add_to_scene(sg, parent);
  char id = sg->AddProximitySensor(this);
  Group* group = dynamic_cast<Group*>(parent);
  ASSERT(group);
  if (group) group->set_has_touch_sensor(id);
  m_scene_graph = sg;
}
#endif

/*! \brief registers the mouse and mostion events. */
void Proximity_sensor::register_events()
{
  Mouse_event::doregister(this);
  Motion_event::doregister(this);
  Passive_motion_event::doregister(this);
}

/*! \brief registers the mouse and mostion events */
void Proximity_sensor::unregister_events()
{
  Mouse_event::unregister(this);
  Motion_event::unregister(this);
  Passive_motion_event::unregister(this);
}

/*! \brief handles mouse events. */
void Proximity_sensor::handle(Mouse_event* /* event */) {}

/*! \brief handles motion events. */
void Proximity_sensor::handle(Motion_event* /* event */) {}

/*! \brief handles passive motion events.
 * Updates and cascades the m_is_over field if needed.
 */
void Proximity_sensor::handle(Passive_motion_event* /* event */)
{ if (!m_enabled) return; }

/*! prints an identification message. */
void Proximity_sensor::identify()
{ std::cout << "Agent: Proximity_sensor" << std::endl; }

SGAL_END_NAMESPACE
