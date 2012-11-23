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

/*!
 * FieldDef - implementation
 */

#include "SGAL/basic.hpp"
#include "SGAL/Field_def.hpp"
#include "SGAL/Script.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto * Field_def::s_prototype = 0;
std::string Field_def::s_tag = "field";

REGISTER_TO_FACTORY(Field_def, "Field_def");

/*! Constructor */
Field_def::Field_def(Boolean proto) :
  Node(proto), m_containerValue(NULL), m_script(NULL)
{
}


/**
 Purpose: prototype initialization function.
*/
void Field_def::init_prototype()
{
  // The prototype shuold be allocated only once for all instances
  if (s_prototype!=NULL)
    return;

  // Allocate a prototype instance
  s_prototype = new Container_proto();

}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Field_def::set_attributes(Element * elem) 
{ 
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "name") {
      m_name = value;
      elem->mark_delete(ai);
    } else if (name == "type") {
      m_type = value;
      elem->mark_delete(ai);
    } else if (name == "value") {
      m_value = value;
      elem->mark_delete(ai);
    } 
  }
  // Remove all the marked attributes:
  elem->delete_marked();
}

/**
 * Get a list of atributes in this object. This method is called only 
 * from the Builder side. 
 *
 * @return a list of attributes 
 */
Attribute_list Field_def::get_attributes()
{ 
  Attribute_list attribs; 
  Attribue attrib;

  attribs = Node::get_attributes();

  attrib.first = "name";
  attrib.second = m_name;
  attribs.push_back(attrib);

  attrib.first = "type";
  attrib.second = m_type;
  attribs.push_back(attrib);

  if (m_type!="SFNode")
  {
    attrib.first = "value";
    attrib.second = m_value;
    attribs.push_back(attrib);
  }

  return attribs; 
};


void Field_def::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Node::AddToScene (sg, parent);

  m_script = dynamic_cast<EScript*>(parent);

  if (m_script!=NULL)
  {
    // if the field is not a node add it to the script with a value in string format
    if (!(m_type=="SFNode" || m_type=="Node"))
      m_script->add_fieldDef(m_name,m_type,m_value,this);

    // If set_value (for SFNodes) was already executed - 
    // add the field def to the script
    else if (m_containerValue!=NULL)
      m_script->add_fieldDef(m_name,m_type,m_containerValue,this);
  }
  else
    assert(false);
}


void Field_def::set_value(Container* value)
{
  m_containerValue = value;

  if (!(m_type=="SFNode" || m_type=="Node"))
  {
    assert(false);
    return;
  }
  // If AddToScene was already executed - add the field def to the script
  else if (m_script != NULL)
    // if it is a node field (SFNODE) add it with a Container* value
    m_script->add_fieldDef(m_name,m_type,m_containerValue,this);
}

SGAL_END_NAMESPACE
