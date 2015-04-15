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
#include "SGAL/Route.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Route::s_tag = "ROUTE";
Container_proto* Route::s_prototype(nullptr);

REGISTER_TO_FACTORY(Route, "Route");

/*! Constructor */
Route::Route(Boolean proto) :
  Container(proto),
  m_src_field(0),
  m_dst_field(0)
{}

/*! Sets the connection */
void Route::set(Shared_container src_node, Field* src_field,
                Shared_container dst_node, Field* dst_field)
{
  // Disconnect old connections if exists:
  if (m_src_field && m_dst_field) m_src_field->disconnect(m_dst_field);

  // Connect new connections:
  m_src_node = src_node;
  m_src_field = src_field;
  m_dst_node = dst_node;
  m_dst_field = dst_field;

  if (!src_node || !dst_node || !src_field || !dst_field
#if !(defined _MSC_VER) || (_MSC_VER >= 1300)
      || (typeid(src_field->get_field_info()) !=
          typeid(dst_field->get_field_info()))
#endif
      )
  {
    SGAL_assertion(false);
    return;
  }

  // Connect the fields
  src_field->connect(dst_field);
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Route::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

//   typedef Element::Str_attr_iter          Str_attr_iter;

//   for (Str_attr_iter ai = elem->str_attrs_begin();
//        ai != elem->str_attrs_end(); ai++)
//   {
//     const std::string & name = elem->get_name(ai);
//     const std::string & value = elem->get_value(ai);
//     if (name == "fromNode") {
//       m_src_node = sg->get_container(value);
//       elem->mark_delete(ai);
//       continue;
//     }
//     if (name == "fromField") {
//       m_src_field = m_src_node->add_field(value);
//       elem->mark_delete(ai);
//       continue;
//     }
//     if (name == "toNode") {
//       m_dst_node = sg->get_container(value);
//       elem->mark_delete(ai);
//       continue;
//     }
//     if (name == "toField") {
//       m_dst_field = m_dst_node->add_field(value);
//       elem->mark_delete(ai);
//       continue;
//     }
//   }
//   // Remove all the deleted attributes:
//   elem->delete_marked();
}

//! \brief initializes the node prototype.
void Route::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();
}

//! \brief deletes the node prototype.
void Route::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Route::get_prototype()
{
  if (!s_prototype) Route::init_prototype();
  return s_prototype;
}

#if 0
/**
 * Get a list of atytributes in this object. This method is called only
 * from the Builder side.
 *
 * @return a list of attributes
 */
Attribute_list Route::get_attributes()
{
  Attribute_list attribs;
  attribs = Container::get_attributes();
  Attribue attrib;

  // Set attributes
  attrib.first = "fromNode";
  attrib.second = m_fromNode;
  attribs.push_back(attrib);

  attrib.first = "fromField";
  attrib.second = m_fromField;
  attribs.push_back(attrib);

  attrib.first = "toNode";
  attrib.second = m_toNode;
  attribs.push_back(attrib);

  attrib.first = "toField";
  attrib.second = m_toField;
  attribs.push_back(attrib);

  return attribs;
}

/**
 * Add a route object to the scene
 * @param sg (in) a reference to the scene graph
 * @param parentName (in) the name of the parent object.
*/
void Route::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Container::AddToScene(sg,parent);

  // Get the containers from the scene graph
  Container* fromNode = sg->get_container(m_fromNode);
  Container* toNode = sg->get_container(m_toNode);

  // Assert that the containers where found
  if (fromNode==NULL || toNode==NULL)
  {
    std::string msg;
    msg.format("%s|%s|%s", m_fromNode, m_toNode, "Route node connection error");
    m_executionCoordinator->SendErrorMessage(etNodeRoute, msg);
    assert(false);
    return;
  }

  // Get the fields from the containers
  Field* toField = toNode->add_field(m_toField);
  Field* fromField = fromNode->add_field(m_fromField);

  // Assert that the fields where found
  if (fromField==NULL || toField==NULL)
  {
    std::string msg;
    msg.format("%s|%s|%s", m_fromField, m_toField, "Route field connection error");
    m_executionCoordinator->SendErrorMessage(etFieldRoute, msg);
    assert(false);
    return;
  }


  // Assert that the fields can be connected (their field infos are of the
  // same type)
  if (typeid(fromField->get_field_info()) != typeid(toField->get_field_info()))
  {
    assert(false);
    return;
  }


  // Connect the fields
  fromField->Connect(toField);

}
#endif

SGAL_END_NAMESPACE
