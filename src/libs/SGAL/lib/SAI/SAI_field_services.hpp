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
 * External services interface for SAI_field
 *
 * Abstract class that holds only external services to the SAI_field class.
 */

#ifndef EFI
#define EFI

#include "Scene_graphDefs.h"
#include "EnbTypes.h"
#include "SAIClient.h"

class ESAIEventListener;

class SCENE_GRAPH_CLASSDEF SAI_fieldServices {
private:
  ESAIClient * m_client;

public:
  SAI_fieldServices():m_client(ENULL){}

  virtual ~SAI_fieldServices()
  {
    if (m_client!=ENULL)
      m_client->Destroy();
  }

  // Get the field type
  virtual Int get_type() const = 0;

  // Get the field's name
  virtual const String & get_name() const = 0;

  // Get the field's id
  virtual Int get_iD() const = 0;

  // Register an event listener (adds it's pointer to the event listeners list)
  virtual void RegisterFieldInterest(ESAIEventListener * eventListener) = 0;

  // Get length of the field's value in bytes
  virtual Int get_valueLength() const = 0;

  // Get the field's value into the given memory place pointed by value
  virtual void get_value(char * value) const = 0;

  // Set the field's value taken from the memory place pointed by value
  virtual void set_value(const char * value) = 0;

  // Sets client object to the SAI field
  virtual void SetClient(ESAIClient* client)
  {
    m_client = client;
  }

  // Gets client object of the SAI field
  virtual ESAIClient * get_client() const
  {
    return m_client;
  }
};

#endif
