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
// $Id: $
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/**
    SAI_field - implementation
*/

#include "Scene_graphPch.h"
#pragma warning (disable: 4786) // this will disable the truncate to 255

#include "SAI_field.h"
#include "Trace.h"

/*! Destructor */
SAI_field::~SAI_field()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~SAI_field ...");
  if (m_goalField) m_goalField->Disconnect(this);
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/**
    Cascade the sai field into the scene graph
    This function uses the SAIField_info set_valueFor function which is using 
    the SAI field value as critical section
*/
void SAI_field::CascadeIntoScene_graph() 
{
    // Set the originating cascade flag - to indicate the current cascade is originating
    // from the SAI
    m_originatingCascade = true;

    // Use the base class's (Field) Cascade() - 
    // to cascade the update into the scene graph
    Field::Cascade();
}



  
/**
    Set the SAIUpdatesInterface pointer
*/
void SAI_field::SetSAIUpdatesInterface(ESAIUpdatesInterface* SAIUpdatesInterface) 
{
    m_SAIUpdatesInterface = SAIUpdatesInterface;
}


/**
    Connect the SAI field with the goal field in both directions
*/
void SAI_field::Connect(Field* field)
{
    // Connect to the goal field in both directions
    Field::Connect(field);
    field->Connect(this);
    m_goalField = field;

    // Copy the goal field name and type id
    m_name = (field->get_field_info())->get_name();
    m_typeId = (field->get_field_info())->get_typeId();
  m_id = (field->get_field_info())->get_id();

    // Create a copy of the goal field value holder
    m_valueHolder = (field->get_valueHolder())->CreateCopy();
}


/** 
    Disconnect the SAI field from the goal field
*/
void SAI_field::Disconnect()
{
    m_goalField->Disconnect(this);
}



/**
 Purpose: Add event listener to the SAI field
 Description: 
      Inserts the given event listener to the event listeners list
 @param  eventListener (in) the event listener to be registered
*/
void SAI_field::RegisterFieldInterest(ESAIEventListener* eventListener)
{
    // lock m_eventListenersCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_eventListenersCS);
  m_eventListenerList.insert(m_eventListenerList.begin(),eventListener);
}

