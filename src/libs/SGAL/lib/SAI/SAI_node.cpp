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
// Author(s): Efi Fogel         <efifogel@gmail.com>

/**
  ESAINode - implementation
*/


#include "Scene_graphPch.h"
#include "SAINode.h"
#include "Container.h"
#include "FieldTypesEnum.h"
#include "SAISFNodeField.h"
#include "SAISFStringField.h"
#include "SAIClient.h"
#include "SGAL/Trace.hpp"

//! \brief constructs.
SAI_node::SAI_node(Container* container,
                   ESAIUpdatesInterface* SAIUpdatesInterface):
    m_container(container),m_SAIUpdatesInterface(SAIUpdatesInterface),
    m_referenceCounter(0),m_client(ENULL){}

//! \brief destructs.
SAI_node::~SAI_node()
{
  TRACE_MSG(Tracer::DESTRUCTOR, "~SAI_node ...");
  SAI_fieldServicesListType::iterator fieldServicesIterator = m_fieldsServices.begin();
  while( fieldServicesIterator != m_fieldsServices.end())
  {
    SAI_fieldServices * fieldServices = (*fieldServicesIterator);
    delete fieldServices;
    fieldServicesIterator ++;
  }
  if (m_client) m_client->Destroy();
  TRACE_MSG(Tracer::DESTRUCTOR, " completed\n");
}


/**
 Purpose: Getting the name of the node
 Description:
      Gets the name from the pointed scene graph container

 @return the name string
*/
const String& SAI_node::get_name() const
{
  return m_container->get_name();
}


/**
 Purpose: Getting the type of the node
 Description:
      Returns the pointed container tag as the sai nodes type

 @return the tag string
*/
const String& SAI_node::get_type() const
{
  return m_container->get_tag();
}

/**
 Sets client object to the SAI node
*/
void SAI_node::SetClient(ESAIClient* client)
{
  m_client = client;
}

/**
 Gets client object of the SAI node
*/
ESAIClient* SAI_node::get_client() const
{
  return m_client;
}


/**
 Purpose: Getting a SAI field of the node
 Description:
  - Creates and adds a new sai field to the node.
  - Connect the sai field and the suitable original scene graph field
    in both ways - so every cascade on any of them - updates the other one
    as well.

 @param goalField (in) pointer to the suitable scene graph field
 @return the new sai field pointer
*/
SAI_fieldServices* SAI_node::get_field(Field* goalField)
{
  // Create a new SAI field
    Field_info* goalField_info = goalField->get_field_info();
    SAI_fieldServices* SAIFieldServ;
    if (goalField_info->get_typeId()==SFNODE)
        SAIFieldServ = new ESAISFNodeField;
    else
    if (goalField_info->get_typeId()==SFSTRING)
        SAIFieldServ = new ESAISFStringField;
    else
      SAIFieldServ = (goalField->get_field_info())->CreateSAIField();


    SAI_field* SAIField = dynamic_cast<SAI_field*>(SAIFieldServ);
    if (SAIField==ENULL)
    {
        assert(false);
        return ENULL;
    }

    // Set the SAI updates interface for the new created SAI field
    SAIField->SetSAIUpdatesInterface(m_SAIUpdatesInterface);

  // Add the new SAI field services to m_fieldsServices
    m_fieldsServices.push_back(SAIFieldServ);

  // Connect the SAI field to the goal field
  SAIField->Connect(goalField);

  // Return the new SAI field services
  return SAIFieldServ;
}




/**
 Purpose: Getting a SAI field of the node
 Description:
    Try to find a field with fieldID in m_fieldsServices - and return it if found.
    If not found - get the goal scene graph field and use get_field(Field* goalField).

 @param fieldID (in) the name of the requested field
 @return the new sai field pointer
*/
SAI_fieldServices* SAI_node::get_field(int fieldID)
{
    // Try to find a field with fieldID in m_fieldsServices
    SAI_fieldServicesListType::iterator fieldServicesIterator = m_fieldsServices.begin();
    while( fieldServicesIterator != m_fieldsServices.end())
    {
    SAI_fieldServices * fieldServices = (*fieldServicesIterator);
        if (fieldServices->get_iD()==fieldID)
            break;
    fieldServicesIterator ++;
  }

    // If found - return it
    if (fieldServicesIterator!=m_fieldsServices.end())
        return (*fieldServicesIterator);


  // Get the field with the given name from the pointed container
  Field* goalField = m_container->add_field(fieldID);

    // If not such field return ENULL
  if (goalField==ENULL)
    return ENULL;

  // use the get_field(Field*) private function to create the new SAI field
  return get_field(goalField);
}




/**
 Purpose: Getting a SAI field of the node
 Description:
    Try to find a field with fieldName in m_fieldsServices - and return it if found.
    If not found - get the goal scene graph field and use get_field(Field* goalField).

 @param fieldName (in) the name of the requested field
 @return the new sai field pointer
*/
SAI_fieldServices* SAI_node::get_field(const String& fieldName)
{
    // Try to find a field with fieldName in m_fieldsServices
    SAI_fieldServicesListType::iterator fieldServicesIterator = m_fieldsServices.begin();
    while( fieldServicesIterator != m_fieldsServices.end())
    {
    SAI_fieldServices * fieldServices = (*fieldServicesIterator);
        if (fieldServices->get_name()==fieldName)
            break;
    fieldServicesIterator ++;
  }

    // If found - return it
    if (fieldServicesIterator!=m_fieldsServices.end())
        return (*fieldServicesIterator);


  // Get the field with the given name from the pointed container
  Field* goalField = m_container->add_field(fieldName);

    // If not such field return ENULL
  if (goalField==ENULL)
    return ENULL;

  // use the get_field(Field*) private function to create the new SAI field
  return get_field(goalField);
}


/**
 Purpose: Getting a list of field Ids
 Description:
    Puts in IDsList a list of name and Id pairs for each field the node has

 @param IDsList (out) the list of the ID & Name pairs
*/
void SAI_node::get_fieldsList(FieldIDsList& IDsList) const
{
  Container_proto* nodeProto = m_container->get_prototype();
  if (nodeProto==ENULL)
  {
    assert(false);
    return;
  }
  nodeProto->get_iDsList(IDsList);
}
