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
 * A Scene Authoring Interface Node - an abstract external interface for any
 * scene graph node
 *
 * Supports generic function interfacing any scene graph node.
 */

#ifndef EFI
#define EFI

#include "SAIFieldTemplate.h"
#include "SAINodeServices.h"
#include "SAIUpdatesInterface.h"

typedef list<SAI_fieldServices*> SAI_fieldServicesListType;

class ESAINode: public SAI_node_services {
public:
  // Constructor
  ESAINode(Container * container, ESAIUpdatesInterface * SAIUpdatesInterface);

  // Destructor
  virtual ~ESAINode();

  // Gets the node's name (the vrml DEF)
  virtual const String& get_name() const;

  // Gets the node's type (the node's tag)
  virtual const String& get_type() const;

  // Gets a field of the node according to the field name
  virtual SAI_fieldServices* get_field(const String& fieldName);

  // Gets a field of the node according to the field id
  virtual SAI_fieldServices* get_field(Int fieldID);

  // Puts in IDsList - a list of names and IDs of the node's fields
  virtual void get_fieldsList(FieldIDsList& IDsList) const;

  // Sets client object to the SAI node
  virtual void SetClient(ESAIClient* client);

  // Gets client object of the SAI node
  virtual ESAIClient* get_client() const;

private:
  // Gets a field of the node according to the Field*
  SAI_fieldServices* get_field(Field* goalField);  

  Container* m_container;

  SAI_fieldServicesListType m_fieldsServices;

  ESAIUpdatesInterface* m_SAIUpdatesInterface;

  Int m_referenceCounter;

  ESAIClient* m_client;

public:

  // Returns the pointed container
  Container * get_container() const
  {
    return m_container;
  }

  // Incerement the reference counter
  void IncrementReference()
  {
    m_referenceCounter++;
  }

  // Decrement the reference counter - returns true in zero if the counter is <= zero 
  void DecrementReference(Boolean& zero)
  {
    m_referenceCounter--;
    zero = (m_referenceCounter<=0);
  }
};

typedef map<String,ESAINode*> ESAINodeMapType;

#endif
