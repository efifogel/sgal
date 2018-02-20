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
 * SAI node creation event and event listener
 *
 * The ESAINodeCreationEvent and ESAINodeCreationEventListener are used 
 * to register and listen to creation of nodes in the scene graph.
 * ESAINodeCreationEvent - used for retrieving information about the event.
 * ESAINodeCreationEventListener - Used to register interest in creation
 * of nodes of a certain type.  
 * SAI users should inherit from ESAINodeCreationEventListener and implement
 * the pure virtual function EventActivated(ESAINodeCreationEvent& event).
 * The EventActivated function will be activated once for each created node.
 * If node instances of the regitered type were created before registration 
 * of the listener - EventActivated will be called for each of them right 
 * after registration.
 */

#ifndef EFI
#define EFI

#include "Scene_graphDefs.h"

class SAI_fieldServices;

class SCENE_GRAPH_CLASSDEF ESAINodeCreationEvent {
private:
  String m_nodeName;
  Scene_time m_time;
  Int m_type;

public:
  // The type of the event
  enum EventType {
    NODE_CREATION = 0,
    NODE_DELETION = 1
  };

  // Constructor
  ESAINodeCreationEvent(Int type, Scene_time time, const String & nodeName):
    m_type(type), m_time(time), m_nodeName(nodeName) {}

  // Destructor
  virtual ~ESAINodeCreationEvent(){};
  
  // Get the SAI field instance of the changed field
  // If the event does not indicates a change in a field 
  // (UPDATE_START for example) returns ENULL
  const String & get_nodeName() const
  {
    return m_nodeName;
  }

  // Get the scene time in which the field has changed
  Scene_time get_time() const
  {
    return m_time;
  }

  // Get the type of the event
  Int get_type() const
  {
    return m_type;
  }
};

class ESAINodeCreationEventListener
{
public:
  virtual void EventActivated(const ESAINodeCreationEvent& event) = 0;
};

typedef list<ESAINodeCreationEventListener*> NodeCreationEventListernerListType;

#endif
