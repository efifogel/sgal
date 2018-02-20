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
#include "Instance.h"
#include "Scene_graph.h"
#include "Element.h"

using namespace SGAL;

REGISTER_TO_FACTORY(Instance, "Instance");

Instance::Instance() : XML_entity()
{
  m_tag = "USE";
}

/*! Destructor */
Instance::~Instance() {}

void Instance::SetInstanceName(const String &name)
{
  m_instanceName = name;
}

String Instance::get_instanceName() const
{
  return m_instanceName;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Instance::set_attributes(Element * elem, Scene_graph * /* sg */)
{
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "name") {
      SetInstanceName(value);
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/**
Get the attributes of the box.

@return a list of attributes and their values
*/
Attribute_list Instance::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attrib.first = "name";
  attrib.second = get_instanceName();
  attribs.push_back(attrib);

  return attribs;
}

/**
Add the object to the scene.
First we find the object by its name in one of the pools in
the scene graph. Once found we call ther AddToScen method on this object.

@param sg a pointer to the scene graph
@param a pointer to the objects parent
*/
void Instance::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Container *node = sg->get_container(m_instanceName);
  if (node) {
    node->AddToScene(sg, parent);

        // If the parent is a touch sensor add the node as its routed node
        // (see documentation in TouchSensor.h)
      ETouchSensor *touchSensor = dynamic_cast<ETouchSensor *>(parent);
      if (touchSensor)
      {
             Container *routedNode = dynamic_cast<Container *>(node);
        touchSensor->SetRoutedNode(routedNode);
      }

#ifdef JAVA_SCRIPT
        // If the parent is a Field (of script or proto) set the instance as its value
        // (see documentation in TouchSensor.h)
      FieldDef *fieldDef = dynamic_cast<FieldDef *>(parent);
      if (fieldDef)
      {
        fieldDef->set_value(node);
      }
#endif
  }
}
