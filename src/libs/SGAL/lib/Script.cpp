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

/**
 * Script - implementation
 */

#include "SGAL/basic.hpp"
#include "Script.h"
#include "JSWObjectInt.h"
#include "SG_JSObject.h"
#include "SGVec_JSObject.h"
#include "Element.h"
#include "Container_proto.h"

SGAL_BEGIN_NAMESPACE

std::string Script::s_tag = "Script";

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Node(proto),
  m_field_infoIDCount(0),
  m_JSWObject(NULL),
  m_engineInitialized(false),
  m_SAI(NULL),
  m_SAINode(NULL),
  s_prototype(NULL)
{}

/*! Destructor */
Script::~Script()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Script ...");
  if (m_JSWObject) m_JSWObject->Destroy();
  delete_prototype();
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}


/**
 Purpose: prototype initialization function.
*/
void Script::init_prototype()
{
  if (!s_prototype) return;
  s_prototype = new Container_proto();
}

/*! */
void Script::delete_prototype()
{  
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto * Script::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
} 

/** 
 * Return the number of children the object has .
 * @return number of childrent in the group.
 */
int Script::get_childCount()
{
  return m_child_list.size();
}

/** 
 * Get a child according to its position in the child list.
 * @param index the index of the child.
 * @return a pointer to the child object.
 */
Node* Script::get_child(int index)
{
  if ( index >= m_child_list.size() )
  {
    return 0;
  }
  
  NodeListIter iter = m_child_list.begin();
  for ( int i = 0 ; i < index ; i++ ) 
  {
    ++iter;
  }
  return *iter;
}

/**
 * Adds a field definition to the script
 * @param name (in) the name of the field
 * @param type (in) the type of the field
 */
void Script::add_fieldDef(const String& name, 
              const String& type, 
              const String& value,
              Node* field)
{
  m_child_list.push_back(field);

  // Get the node's prototype - use the function - this way if s_prototype is not
  // allocated yet, init_prototype will be activated
  Container_proto *prototype = get_prototype();

  // Add the object fields to the prototype
  s_prototype->add_field_info(m_field_infoIDCount, name, type, value, 0,
                (Execution_func_type)&Script::execute);    
  m_field_infoIDCount++;
}


/**
 * Adds a field definition to the script
 * @param name (in) the name of the field
 * @param type (in) the type of the field
 */
void Script::add_fieldDef(const String& name, 
              const String& type, 
              Container* value, 
              Node* field)
{
  m_child_list.push_back(field);

  // Get the node's prototype - use the function - this way if s_prototype is not
  // allocated yet, init_prototype will be activated
  Container_proto *prototype = get_prototype();

  // Add the object fields to the prototype
  s_prototype->add_field_info(m_field_infoIDCount, name, type, value, 0,
                (Execution_func_type)&Script::execute);    

  m_field_infoIDCount++;
}


/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Script::set_attributes(Element * elem) 
{ 
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string * name = elem->get_name(ai);
    const std::string * value = elem->get_value(ai);
    if (name == "url") {
      m_url = value;
      elem->mark_delete(ai);
      continue;
    } 
  }
  // Remove all the marked attributes:
  elem->delete_marked();
}

// Execution function - executes the suitable script function according to the 
// event
void Script::execute(Field_info* field_info)
{
  // if this is the first time the script is executed -
  // initialize the suitable script engine object
  if (!m_engineInitialized)
  {
    m_JSWObject->Init(get_name().c_str());
    bool compiled = m_JSWObject->CompileScript(m_url.c_str());
    if (!compiled)
      debug("JS Error - Object: %s, failed to compile script", 
          m_name);
    m_engineInitialized = true;
  }

  SAI_fieldServices* field = m_SAINode->get_field(field_info->get_id());
  if (field==NULL)
  {
    assert(false);
    return;
  }
  Int argType = field->get_type();

  m_SAI->BeginUpdate();

  if (argType==SFBOOL)
  {
    SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(), 
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (argType==SFFLOAT)
  {
    SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(), 
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (argType==SFTIME)
  {
    SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(), 
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (argType==SFINT32)
  {
    SAI_fieldSpecificServicesTemplate<Int,SFINT32>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Int,SFINT32>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(), 
           (int)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (argType==SFSTRING)
  {
    SAI_fieldSpecificServicesTemplate<String,SFSTRING>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<String,SFSTRING>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(), 
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (argType==SFNODE)
  {
    SAI_fieldSpecificServicesTemplate<SAI_node_services*,SFNODE>* spField = 
      dynamic_cast<SAI_fieldSpecificServicesTemplate<SAI_node_services*,SFNODE>*>(field);
    if (spField==NULL)
    {
      assert(false);
      return;
    }
    SAI_node_services* nodeValue = spField->get_value();

    // If the SAI node has a client - it served a ESG_JSObject before -
    // use this client
    if (nodeValue->get_client()==NULL)
      // if the SAI node has no client - allocate a new object
      m_JSWObject->CallFunction(field_info->get_name(), 
             new ESG_JSObject(m_SAI,nodeValue,field_info->get_name()),
             (double)m_executionCoordinator->get_sceneTime());
    else
    {
      // Cast the client to ESG_JSObject
      ESG_JSObject* obj = 
        dynamic_cast<ESG_JSObject*>(nodeValue->get_client());
      if (obj==NULL)
      {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), 
             obj,
             (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else if (argType==SFVEC2F || argType==SFVEC3F || argType==SFROTATION || argType==SFVEC4F)
  {
    // If the SAI field has a client - it served a ESGVec_JSObject before -
    // use this client
    if (field->get_client()==NULL)
      // if the SAI node has no client - allocate a new object and return it
    {
      SAI_node_services* SAINode = m_SAI->get_node(this);
      m_JSWObject->CallFunction(field_info->get_name(), 
             new ESGVec_JSObject(m_SAI,SAINode,field),
             (double)m_executionCoordinator->get_sceneTime());
    }
    else
    {
      // Cast the client to ESG_JSObject
      ESGVec_JSObject* obj = 
        dynamic_cast<ESGVec_JSObject*>(field->get_client());
      if (obj==NULL)
      {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), 
             obj,
             (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else
    assert(false);

  m_SAI->EndUpdate();
}



/**
 * Get a list of atributes in this object. This method is called only 
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

#if 0
void Script::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Node::AddToScene (sg, parent );

  m_SAI = sg->get_scriptsSAI();
  if (m_SAI==NULL)
  {
    assert(false);
    return;
  }

  m_SAINode = m_SAI->get_node(this);
  if (m_SAINode == NULL)
  {
    assert(false);
    return;
  }

  // Create a suitable script engine object
  m_JSWObject = (sg->GetJSWEngine())->
    CreateObject(new ESG_JSObject(m_SAI,m_SAINode,m_SAINode->get_name()));
}
#endif

SGAL_END_NAMESPACE
