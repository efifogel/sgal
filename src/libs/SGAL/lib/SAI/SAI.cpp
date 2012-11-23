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
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/**
 ESAI - implementation
*/

#include "Scene_graphPch.h"
#include "SAI.h"
#include "Scene_graph.h"
#include "Group.h"
#include "XML_entity_factory.h"
#include "SAI_nodeCreationEvent.h"
#include "Trace.h"

/*! Constructor */
SAI::SAI(Scene_graph* sceneGraph):
  m_sceneGraph(sceneGraph),
  m_currentUpdateFields(ENULL),
  m_time(Execution_coordinator::get_sceneTime())
{}

/*! Destructor */ 
SAI::~SAI()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~SAI ...");
  SAI_nodeMapType::iterator nodeIterator = m_nodes.begin();
  while (nodeIterator != m_nodes.end()) {
    SAI_node * node = (*nodeIterator).second;
    delete node;
    nodeIterator++;
  }
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/** 
  Returns a suitable SAI node for a given node name
  Searches for a node with the given name in m_nodes.
  If found returns it.
  If not - inserts the new node to m_nodes (list)
  @param name (in) the scene graph name of the requested node
*/
SAI_node_services* SAI::get_node(const String& name)
{
    SAI_node* node = ENULL;

    // Try to find a node with the given name
    SAI_nodeMapType::iterator nodeIterator = m_nodes.find(name);
    if (nodeIterator != m_nodes.end())
    {
        node = (*nodeIterator).second;
    }
    else
    {
        Container* container = m_sceneGraph->get_container(name);
        if (container==ENULL)
            return ENULL;

      // Allocate a new SAI node instance - initialize it with the suitable 
      // scene graph container pointer
      node = new SAI_node(container,this);

      // Add the new SAI node to m_nodes
        m_nodes.insert(SAI_nodeMapType::value_type(name,node));
    }

    node->IncrementReference();

  return node;
}


/** 
  Returns a suitable SAI node for a given container
  @param cont (in) a pointer to the suitable container
*/
SAI_node_services* SAI::get_node(ContainerPnt cont)
{
    if (cont==ENULL)
        return ENULL;

  SAI_node* node = ENULL;

    // Try to find a node with the given name
    SAI_nodeMapType::iterator nodeIterator = m_nodes.find(cont->get_name());
    if (nodeIterator != m_nodes.end())
    {
        node = (*nodeIterator).second;
    }
    else
    {
 
  // Allocate a new SAI node instance - initialize it with the suitable 
  // scene graph container pointer
  node = new SAI_node(cont,this);

  // Add the new SAI node to m_nodes with an empty name
    m_nodes.insert(SAI_nodeMapType::value_type(cont->get_name(),node));

    }

    node->IncrementReference();

  return node;  
  
}



/** 
  Decrements the reference counter of the node with the given name - 
  if after decrementing it is zero - deletes the node's instance
  @param name (in) the scene graph name of the requested node
*/
void SAI::DisposeNode(const String& name)
{
    // Try to find a node with the given name
    SAI_nodeMapType::iterator nodeIterator = m_nodes.find(name);

    // If the node is not found - return
    if (nodeIterator == m_nodes.end())
        return;

    SAI_node* node = (*nodeIterator).second;
    Boolean zero;

    node->DecrementReference(zero);

    // If the reference counter of the node is zero delete it
    if (zero)
    {
        m_nodes.erase(nodeIterator);
        delete node;
    }
}


/**
  Create a new node in the scene graph
  @param type (in) type of the node which is going to be created
  @param name (in) name for the node which is going to be created
  @param father (in) name of the node which should be the father of the new 
                     created node
*/
Boolean SAI::CreateNode(const String& type, const String& name, const String& father)
{
    // Perform this function being synchronized by the scene graph thread
    SAIAutoSynchronized AutoSynch(&m_synchronizer);

  XML_entity_factory *factory = XML_entity_factory::get_instance();
  XML_entity *node = 0;
  if ( factory ) {
    node = factory->CreateEntity(type);
  }

  if (node == ENULL)
    return false;

  node->SetName(name);

  // Set the remote flag for the new created node (since it is being created by
  // a SAI)
  node->SetRemote();

  // If a father node was given - get the father node
  // - else use the root node as ther father
  XML_entity *fatherNode;
  if (father == "")
    fatherNode = m_sceneGraph->GetRoot();
  else
    fatherNode = m_sceneGraph->get_container(father);

  // Add the new created node to the scene graph
  m_sceneGraph->AddToScene(fatherNode, node);  

  return true;
}



/**
  Activated by the scene graph before the events cascade starts:
  - Synchronize SAI's using threads activity.
  - Sets m_sceneTime to the current time.
  - Signals UPDATE_START event for all the event listeners in the list.
*/
void SAI::SignalCascadeStart()
{
    // Synchronize SAI's using threads activity (activity that should be synchronized
    // with the scene graph activity)
    m_synchronizer.SynchronizeActivity();

    // Remember the current time in m_time
    m_time = Execution_coordinator::get_sceneTime();

    // lock m_eventListenersCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_eventListenersCS);

    // If no event listeners exist in the list - return
  if ( m_eventListenerList.empty())
    return;

  EventListenerListType::iterator eventListenerIterator = 
    m_eventListenerList.begin();

  // Loop over the connected event listeners
  while( eventListenerIterator != m_eventListenerList.end())
  {
    (*eventListenerIterator)->EventActivated(
            SAIEvent(SAIEvent::UPDATE_START,m_time));
    eventListenerIterator++;
  }
}


/**
   Signals UPDATE_END event for all the event listeners in the list.
*/
void SAI::SignalEventListeners()
{
    // Remember the current time in sceneTime
    Scene_time sceneTime = Execution_coordinator::get_sceneTime();

    // lock m_eventListenersCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_eventListenersCS);

    // If no event listeners exist in the list - return
  if ( m_eventListenerList.empty())
    return;

  EventListenerListType::iterator eventListenerIterator = 
    m_eventListenerList.begin();

  // Loop over the connected event listeners
  while( eventListenerIterator != m_eventListenerList.end())
  {
    (*eventListenerIterator)->EventActivated(
            SAIEvent(SAIEvent::UPDATE_END,sceneTime));
    eventListenerIterator++;
  }
}


/**
  Activates CascadeIntoScene_graph for every SAI field in the 
  m_updatedFieldsLists.
  Removes each such field from m_updatedFieldsLists until it is empty.
*/
void SAI::CascadeIntoSGUpdatedFields()
{
    // lock m_fieldsUpdatesCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_fieldsUpdatesCS);
    
    // If no list of fields exists return
  if ( m_updatedFieldsLists.empty())
    return;

  // Loop over the updated fields lists
  SAIFieldListsType::iterator FieldsListsIterator = 
    m_updatedFieldsLists.begin();

  while (FieldsListsIterator != m_updatedFieldsLists.end())
  {
        SAIFieldListType* FieldsList = (*FieldsListsIterator);
      SAIFieldListType::iterator FieldIterator = 
        FieldsList->begin();
        
        // for each list - loop over the fields
        while (FieldIterator != FieldsList->end())
        {
            // Cascade into the scene graph each field
            (*FieldIterator)->CascadeIntoScene_graph();
            FieldIterator++;
        }
    FieldsListsIterator++;

        // After the cascade of the current list (pointed by FieldsListIterator)
        // Remove all items of the list 
        FieldsList->clear();
        // Delete the list
        delete FieldsList;
  }

    // Empty the list of fields lists
    m_updatedFieldsLists.clear();


}

/**
  Activated by the scene graph after the events cascade ends
  Calls SignalEventListeners and CascadeIntoUpdatedField
*/
void SAI::SignalCascadeEnd()
{
    SignalEventListeners();
    CascadeIntoSGUpdatedFields();
}


/**
   Returns the time when the last cascade has started
*/
Scene_time SAI::get_time() const
{
    return m_time;
}


/**
  Registers interest in the scene graph general 
  events - cascade start, cascade end ...
  Adds the given event listener to the event listeners list.
*/
void SAI::RegisterGeneralEventsInterest(SAIEventListener* eventListener)
{
    // lock m_eventListenersCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_eventListenersCS);

  m_eventListenerList.insert(m_eventListenerList.begin(),eventListener);
}



// Adds an update to the updated SAI fields queue - used
// by SAI Field when their value is set (triggred externaly)
void SAI::add_update(SAI_field* field)
{
    // lock m_fieldsUpdatesCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_fieldsUpdatesCS);

    if (m_currentUpdateFields!=ENULL)
        m_currentUpdateFields->insert(m_currentUpdateFields->end(),field);
    else
    {
        assert(false);
        return;
    }

}


/**
  Activated by external users of the SAI before starting to update any
  fields in the scene graph.
  This function starts a new buffer of updated fields which will be cascade
  into the scene graph only after EndUpdate() will be called.
  All set_value() on SAI fields should be called between a call to 
  BeginUpdate() and a call to EndUpdate().
  The function allocated a new list of updated fields.
*/
void SAI::BeginUpdate()
{
    // lock m_fieldsUpdatesCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_fieldsUpdatesCS);

    // if there is already an updated list - do nothing
    if (m_currentUpdateFields!=NULL)
    {
        assert(false);
        return;
    }

    m_currentUpdateFields = new SAIFieldListType; 
        
}


/**
  Activated by external users of the SAI after ending the update of fields
  in the scene graph - only after EndUpdate is activated the new values
  which were set to SAI fields will be cascade into the scene graph. 
  All set_value() on SAI fields should be called between a call to 
  BeginUpdate() and a call to EndUpdate().
  The function takes the list pointed by m_currentUpdateFields and
  adds it to m_updatedFieldsLists.
  m_currentUpdateFields is set to ENULL to indicate it does not exists
  (because the current update has just end)
*/
void SAI::EndUpdate()
{
    // Lock m_fieldsUpdatesCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_fieldsUpdatesCS);
   
    // Insert m_currentUpdateFields in the end of the list
    m_updatedFieldsLists.insert(m_updatedFieldsLists.end(),
                                    m_currentUpdateFields);

    // Set m_currentUpdateFields with ENULL
    m_currentUpdateFields = ENULL;   
}



/**
  Registers interest in creation of new nodes in the scene graph.
  The given listener will be activated once for every node of the given type that
  was or will be created in the scene graph.
*/
void SAI::RegisterNodeCreationInterest(const String& type,
                                        SAI_nodeCreationEventListener* eventListener)
{
    // Remember the current time
    Scene_time sceneTime = Execution_coordinator::get_sceneTime();

    ContainerList containers;
    m_sceneGraph->get_containers(type,containers);
        
    for ( ContainerListIter iter=containers.begin(); 
      iter!=containers.end();
          ++iter ) 
    {
    Container* c = *iter;
        eventListener->EventActivated(
            SAI_nodeCreationEvent(SAI_nodeCreationEvent::NODE_CREATION, sceneTime, c->get_name()));
     }
}

