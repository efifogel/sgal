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

/*!
 * Scene Authoring Interface
 *
 * The SAI is a generic interface to the scene graph.
 * The SAI mainly supports retrieve, manipulation and intereset 
 * registration of the scene graph fields and events.
 */

#ifndef EFI
#define EFI

#include "SAINode.h"
#include "SAIUpdatesInterface.h"
#include "CritSec.h"
#include "SAISynchronizer.h"

class Scene_graph;
class ESAINodeCreationEventListener;

typedef list<SAIFieldListType*> SAIFieldListsType;

class ESAI: public ESAIUpdatesInterface {
public:
  // Constructor
  ESAI(Scene_graph* sceneGraph);

  // Destructor
  virtual ~ESAI();

  // Return a suitable SAI node for a given node name
  // Incerements the reference counting for the node's instance
  virtual SAI_node_services* get_node(const String& name);

  // Return a suitable SAI node for a given Container pointer
  virtual SAI_node_services* get_node(Container* cont);

  // Decrements the reference counter of the node with the given name - 
  // if after decrementing it is zero - deletes the node's instance
  virtual void DisposeNode(const String& name);

  // Create a new node in the scene graph
  virtual Boolean CreateNode(const String& type, 
                          const String& name, 
                          const String& father = "");

  // Activated by the scene graph before the events cascade starts
  virtual void SignalCascadeStart();

  // Activated by the scene graph after the events cascade ends
  virtual void SignalCascadeEnd();

  // Returns the time when the last cascade has started
  virtual Scene_time get_time() const;

  // Registers interest in the scene graph general 
  // events - cascade start, cascade end ...
  virtual void RegisterGeneralEventsInterest(ESAIEventListener* eventListener);
   
  //  Registers interest in creation of new nodes in the scene graph.
  //  The given listener will be activated once for every node of the given type that
  //  was or will be created in the scene graph.
  virtual void RegisterNodeCreationInterest(const String& type,
                                            ESAINodeCreationEventListener* eventListener);

  // Adds an update to the updated SAI fields queue - used
  // by SAI Field when their value is set (triggred externaly)
  virtual void add_update(SAI_field* field);

  // Activated by external users of the SAI before starting to update any
  // fields in the scene graph.
  // This function starts a new buffer of updated fields which will be cascade
  // into the scene graph only after EndUpdate() will be called.
  // All set_value() on SAI fields should be called between a call to 
  // BeginUpdate() and a call to EndUpdate().
  virtual void BeginUpdate();

  // Activated by external users of the SAI after ending the update of fields
  // in the scene graph - only after EndUpdate is activated the new values
  // which were set to SAI fields will be cascade into the scene graph. 
  // All set_value() on SAI fields should be called between a call to 
  // BeginUpdate() and a call to EndUpdate().
  virtual void EndUpdate();

private:
  // Signals UPDATE_END event for all the event listeners in the list.
  void SignalEventListeners();

  // Activates CascadeIntoScene_graph for every SAI field in the 
  // m_updatedFieldsLists.
  void CascadeIntoSGUpdatedFields();

  Scene_graph* m_sceneGraph;

  ESAINodeMapType m_nodes;

  // A list of event listeners
  EventListenerListType m_eventListenerList;

  // Holds the time of the last cascade start
  Scene_time m_time;

  // The current updated fields list
  SAIFieldListType* m_currentUpdateFields;

  // List of already updated lists of updated fields
  SAIFieldListsType m_updatedFieldsLists;

  // TCriticalSection object for managing the event listeners list
  // as critical section
  TCriticalSection m_eventListenersCS;

  // TCriticalSection object for managing the updated fields lists
  // as critical section
  TCriticalSection m_fieldsUpdatesCS;

  // Used to synchronize SAI using threads activity with the scene graph activity
  ESAISynchronizer m_synchronizer;
};

typedef list<ESAI*> SAIListType;

#endif
