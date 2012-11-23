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
 * Scene Authoring Interface Services
 *
 * Abstract class that holds pure virtual functions - external services to the
 * ESAI class.
*/

#ifndef EFI_SAI_SEVICES_H
#define  EFI_SAI_SEVICES_H

#include "Scene_graphDefs.h"

class SAI_node_services;
class ESAIEventListener;
class ESAINodeCreationEventListener;

class SCENE_GRAPH_CLASSDEF ESAIServices {
public:
  virtual ~ESAIServices () {}

  // Return a suitable SAI node for a given node name
  // Incerements the reference counting for the node's instance
  virtual SAI_node_services * get_node(const String&  name) = 0;

  // Decrements the reference counter of the node with the given name - 
  // if after decrementing it is zero - deletes the node's instance
  virtual void DisposeNode(const String & name) = 0;

  // Create a new node in the scene graph
  virtual Boolean CreateNode(const String & type, 
                          const String & name, const String & father = "") = 0;

  // Returns the time when the last cascade has started
  virtual Scene_time get_time() const = 0;

  // Registers interest in the scene graph general 
  // events - cascade start, cascade end ...
  virtual void RegisterGeneralEventsInterest(ESAIEventListener * eventListener) = 0;

  //  Registers interest in creation of new nodes in the scene graph.
  //  The given listener will be activated once for every node of the given type that
  //  was or will be created in the scene graph.
  virtual void RegisterNodeCreationInterest(const String & type,
                                            ESAINodeCreationEventListener * eventListener) = 0;

  // Activated by external users of the SAI before starting to update any
  // fields in the scene graph.
  // This function starts a new buffer of updated fields which will be cascade
  // into the scene graph only after EndUpdate() will be called.
  // All set_value() on SAI fields should be called between a call to 
  // BeginUpdate() and a call to EndUpdate().
  virtual void BeginUpdate() = 0;

  // Activated by external users of the SAI after ending the update of fields
  // in the scene graph - only after EndUpdate is activated the new values
  // which were set to SAI fields will be cascade into the scene graph. 
  // All set_value() on SAI fields should be called between a call to 
  // BeginUpdate() and a call to EndUpdate().
  virtual void EndUpdate() = 0;
};

#endif
