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
 * Scene Authoring Updates Interface
 *
 * Abstract class that holds pure virtual functions - interface to the 
 * SAI.
 * Used by the SAI fields to update the SAI about change in their values.
 */

#ifndef EFI_SAI_UPDATES_INTERFACE_H
#define EFI_SAI_UPDATES_INTERFACE_H

#include "SAI_services.h"

class SAI_field;
class Container;

class ESAIUpdatesInterface : public ESAIServices {
public:
  // Returns the time when the last cascade has started
  virtual Scene_time get_time() const = 0;

  // Adds an update to the updated SAI fields queue - used
  // by SAI Field when their value is set (triggred externaly)
  virtual void add_update(SAI_field * field) = 0;

    // Incerements the reference counting for the node's instance
  // Return a suitable SAI node for a given Container pointer
  virtual SAI_node_services * get_node(Container * cont) = 0;

};

#endif
