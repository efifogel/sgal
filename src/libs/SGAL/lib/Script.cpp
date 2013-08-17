// Copyright (c) 2013 Israel.
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
// $Id: $
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Script - implementation
 */

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Script.hpp"

// #include "JSWObjectInt.h"
// #include "SG_JSObject.h"
// #include "SGVec_JSObject.h"

SGAL_BEGIN_NAMESPACE

const std::string Script::s_tag = "Script";
Container_proto* Script::s_prototype(NULL);

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Node(proto)
  // m_field_infoIDCount(0),
  // m_JSWObject(NULL),
  // m_engineInitialized(false),
  // m_SAI(NULL),
  // m_SAINode(NULL)
{}

/*! Destructor */
Script::~Script()
{
  // if (m_JSWObject) m_JSWObject->Destroy();
}

/*! \brief initializes the container prototype. */
void Script::init_prototype()
{
  if (!s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());
}

/*! \brief deletes the container prototype. */
void Script::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Script::get_prototype()
{
  if (!s_prototype) Script::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Script::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;

  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "directOutput") {
      if (compare_to_true(value)) set_direct_output();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "mustEvaluate") {
      if (compare_to_true(value)) set_must_evaluate();
      elem->mark_delete(ai);
      continue;
    }
  }

  // Element::Field_attr_iter fi;
  // for (fi = elem->field_attrs_begin(); fi != elem->field_attrs_end(); ++fi) {
  //   const std::string& name = elem->get_name(fi);
  //   const std::string& value = elem->get_value(fi);
  //   const std::string& type = elem->get_type(fi);
  // }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! Get a list of atributes in this object. This method is called only
 * from the Builder side.
 *
 * @return a list of attributes
 */
Attribute_list Script::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  attrib.first = "url";
  attrib.second = m_url;
  attribs.push_back(attrib);

  return attribs;
}

void Script::add_to_scene(Scene_graph* sg)
{
  Node::add_to_scene(sg);

  m_SAI = sg->get_scriptsSAI();
  if (m_SAI == NULL) {
    assert(false);
    return;
  }

  m_SAINode = m_SAI->get_node(this);
  if (m_SAINode == NULL) {
    assert(false);
    return;
  }

  // Create a suitable script engine object
  m_JSWObject = (sg->GetJSWEngine())->
    CreateObject(new ESG_JSObject(m_SAI, m_SAINode,m_SAINode->get_name()));
}

/*! Return the number of children the object has .
 * @return number of childrent in the group.
 */
int Script::get_childCount() { return m_child_list.size(); }

/* Get a child according to its position in the child list.
 * @param index the index of the child.
 * @return a pointer to the child object.
 */
Node* Script::get_child(int index)
{
  if (index >= m_child_list.size()) return 0;
  NodeListIter iter = m_child_list.begin();
  for (Uint i = 0; i < index; ++i) ++iter;
  return *iter;
}

/*! Adds a field definition to the script
 * @param name (in) the name of the field
 * @param type (in) the type of the field
 */
void Script::add_field_def(const String& name, const String& type,
                           const String& value, Node* field)
{
  m_child_list.push_back(field);

  // Get the node's prototype - use the function - this way if s_prototype is not
  // allocated yet, init_prototype will be activated
  Container_proto *prototype = get_prototype();

  // Add the object fields to the prototype
  s_prototype->add_field_info(m_field_infoIDCount, name, type, value, 0,
                              (Execution_func_type)&Script::execute);
  ++m_field_infoIDCount;
}


/*! Adds a field definition to the script
 * @param name (in) the name of the field
 * @param type (in) the type of the field
 */
void Script::add_field_def(const String& name, const String& type,
                           Container* value, Node* field)
{
  m_child_list.push_back(field);

  // Get the node's prototype - use the function - this way if s_prototype is not
  // allocated yet, init_prototype will be activated
  Container_proto* prototype = get_prototype();

  // Add the object fields to the prototype
  s_prototype->add_field_info(m_field_infoIDCount, name, type, value, 0,
                              (Execution_func_type)&Script::execute);

  ++m_field_infoIDCount;
}
#endif

// Execution function - executes the suitable script function according to the
// event
void Script::execute(Field_info* field_info)
{
#if 0
  // if this is the first time the script is executed -
  // initialize the suitable script engine object
  if (!m_engineInitialized) {
    m_JSWObject->Init(get_name().c_str());
    bool compiled = m_JSWObject->CompileScript(m_url.c_str());
    if (!compiled)
      debug("JS Error - Object: %s, failed to compile script", m_name);
    m_engineInitialized = true;
  }

  SAI_fieldServices* field = m_SAINode->get_field(field_info->get_id());
  if (field == NULL) {
    assert(false);
    return;
  }
  Int arg_type = field->get_type();

  m_SAI->BeginUpdate();

  if (arg_type == SFBOOL) {
    SAI_fieldSpecificServicesTemplate<Boolean, SFBOOL>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Boolean, SFBOOL>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFFLOAT) {
    SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Float, SFFLOAT>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFTIME) {
    SAI_fieldSpecificServicesTemplate<Scene_time, SFTIME>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Scene_time, SFTIME>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFINT32)
  {
    SAI_fieldSpecificServicesTemplate<Int, SFINT32>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Int, SFINT32>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (int)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFSTRING) {
    SAI_fieldSpecificServicesTemplate<String, SFSTRING>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<String, SFSTRING>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFNODE)
  {
    SAI_fieldSpecificServicesTemplate<SAI_node_services*,SFNODE>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<SAI_node_services*, SFNODE>*>(field);
    if (spField == NULL) {
      assert(false);
      return;
    }
    SAI_node_services* nodeValue = spField->get_value();

    // If the SAI node has a client - it served a ESG_JSObject before -
    // use this client
    if (nodeValue->get_client()==NULL)
      // if the SAI node has no client - allocate a new object
      m_JSWObject->CallFunction(field_info->get_name(),
                                new ESG_JSObject(m_SAI, nodeValue,
                                                 field_info->get_name()),
                                (double)m_executionCoordinator->get_sceneTime());
    else {
      // Cast the client to ESG_JSObject
      ESG_JSObject* obj = dynamic_cast<ESG_JSObject*>(nodeValue->get_client());
      if (obj == NULL) {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), obj,
                                (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else if ((arg_type == SFVEC2F) || (arg_type == SFVEC3F) ||
           (arg_type == SFROTATION) || (arg_type == SFVEC4F))
  {
    // If the SAI field has a client - it served a ESGVec_JSObject before -
    // use this client
    if (field->get_client() == NULL) {
      // if the SAI node has no client - allocate a new object and return it
      SAI_node_services* SAINode = m_SAI->get_node(this);
      m_JSWObject->CallFunction(field_info->get_name(),
                                new ESGVec_JSObject(m_SAI,SAINode,field),
                                (double)m_executionCoordinator->get_sceneTime());
    }
    else {
      // Cast the client to ESG_JSObject
      ESGVec_JSObject* obj =
        dynamic_cast<ESGVec_JSObject*>(field->get_client());
      if (obj == NULL) {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), obj,
                                (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else assert(false);

  m_SAI->EndUpdate();
#endif
}

SGAL_END_NAMESPACE
