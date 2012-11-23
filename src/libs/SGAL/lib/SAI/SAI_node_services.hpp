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
 * Scene Authoring Node Services
 * 
 * Abstract class that holds pure virtual functions - external services to the
 * ESAINode class.
 */

#ifndef EFI_SAI_NODE_SEVICES_H
#define EFI_SAI_NODE_SEVICES_H

#include "Scene_graphDefs.h"
#include "FieldIDsList.h"

class SAI_fieldServices;
class ESAIClient;

class SCENE_GRAPH_CLASSDEF SAI_node_services {
public:
  virtual ~SAI_node_services(){};

  // Gets the node's name (the vrml DEF)
  virtual const String& get_name() const = 0;

  // Gets the node's type (the node's tag)
  virtual const String& get_type() const = 0;

  // Gets a field of the node according to the field name
  virtual SAI_fieldServices* get_field(const String& fieldName) = 0;

  // Gets a field of the node according to the field id
  virtual SAI_fieldServices* get_field(Int fieldID) = 0;

  // Puts in IDsList - a list of names and IDs of the node's fields
  virtual void get_fieldsList(FieldIDsList& IDsList) const = 0;

  // Sets client object to the SAI node
  virtual void SetClient(ESAIClient* client) = 0;

  // Gets client object of the SAI node
  virtual ESAIClient* get_client() const = 0;
};

#endif
