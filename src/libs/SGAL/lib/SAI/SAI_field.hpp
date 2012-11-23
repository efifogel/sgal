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
 * SAI field.
 *
 * This abstract class is used by SAI mechanisms as a non template
 * SAI field interface.
 * Actual SAI fields are always instantiations of the derived
 * SAIFieldTemplate (with type instantiation according to the field type).
 * The functions are implemented in SAIFieldTemplate.
 */

#ifndef EFI
#define EFI

#include "Field.h"
#include "SAIUpdatesInterface.h"
#include "SAIEvent.h"

class SAI_field : public Field {
protected:
  ESAIUpdatesInterface * m_SAIUpdatesInterface;
    
  // A list of event listeners
  EventListenerListType m_eventListenerList;

  // TCriticalSection object for managing the event listeners list
  // as critical section
  TCriticalSection m_eventListenersCS;

  Int m_typeId;
  String m_name;
  Int   m_id;
  Field* m_goalField;
  Boolean m_originatingCascade;

public:
  // Constructor
  SAI_field():
    Field(ENULL,ENULL), // there is no container for and field info for the SAI field
    m_goalField(ENULL),
    m_originatingCascade(false),
    m_name("")
  {
  };

  // Destructor
  virtual ~SAI_field();
  
  virtual void SetSAIUpdatesInterface(ESAIUpdatesInterface * SAIUpdatesInterface);

  // Connect the SAI field with the goal field in both directions
  virtual void Connect(Field * field);

  // Disconnect the SAI field from the goal field
  void Disconnect();

  // Add event listener to the SAI field
  void RegisterFieldInterest(ESAIEventListener* eventListener);

  // Pure virtual - should generate event cascade from this field into the scene graph
  virtual void CascadeIntoScene_graph();
};

typedef list<SAI_field*> SAIFieldListType;

#endif
