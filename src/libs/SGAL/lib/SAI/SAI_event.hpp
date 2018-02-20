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
 * SAI event and event listener
 *
 * The ESAIEvent and ESAIEventListerner are used to register 
 * and listen to changes and events in the scene graph.
 * ESAIEvent - used for retrieving information about the event.
 * ESAIEventListener - Used to register interest in a certain field 
 * or event.
 * SAI users should inherit from ESAIEventListener and implement
 * the pure virtual function FieldChanged(ESAIEvent& event).
 */

#ifndef EFI
#define EFI

#include "Scene_graphDefs.h"

class SAI_fieldServices;

class SCENE_GRAPH_CLASSDEF ESAIEvent {
private:
  SAI_fieldServices* m_SAIField;
  Scene_time m_time;
  Int m_type;

public:
  // The type of the event
  enum EventType {
    FIELD_UPDATE = 0,
    UPDATE_START = 1,
    UPDATE_END = 2
  };

  // Constructor
  ESAIEvent(Int type, Scene_time time, SAI_fieldServices * SAIField = NULL):
    m_type(type), m_time(time), m_SAIField(SAIField){};

  // Destructor
  virtual ~ESAIEvent(){};

  // Get the SAI field instance of the changed field
  // If the event does not indicates a change in a field 
  // (UPDATE_START for example) returns ENULL
  SAI_fieldServices * get_sAIField() const
  {
    return m_SAIField;
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

class ESAIEventListener
{
public:
  virtual void EventActivated(const ESAIEvent & event) = 0;
};

typedef list<ESAIEventListener*> EventListenerListType;

#endif
