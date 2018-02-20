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
 * SAI field of SFNode type.
 */

#ifndef EFI
#define EFI

#include "SAIFieldTemplate.h"
#include "SAINode.h"

typedef SAI_node_services* SAI_node_servicesPnt;

class ESAISFNodeField : public SAI_fieldTemplate<SAI_node_servicesPnt,SFNODE>
{
public:
  // Dont support this function for SFNode
  virtual void get_value(char* value) const
  {
    return;
  }

  // Dont support this function for SFNode
  virtual void set_value(const char* value)
  {
    return;
  }

  // Value set and get functions
  virtual SAI_node_servicesPnt get_value() const
  {
    Value_holder_specific<Container*>* value_holder =
      dynamic_cast<Value_holder_specific<Container*>*>(m_value_holder);
    if (value_holder == NULL) {
      assert(false);
      return NULL;
    }
    Container* cont = value_holder->get_value();
    if (cont == NULL) {
      assert(false);
      return NULL;
    }
    SAI_node_servicesPnt SAINode = m_SAIUpdatesInterface->
      get_node(cont);
    return SAINode;
  }

  virtual void set_value(const SAI_node_servicesPnt& value)
  {
    Value_holder_specific<Container*>* value_holder =
      dynamic_cast<Value_holder_specific<Container*>*>(m_value_holder);
    if (value_holder == NULL) {
      assert(false);
      return;
    }
    ESAINode* SAINode = dynamic_cast<ESAINode*>(value);
    if (SAINode == NULL) {
      assert(false);
      return;
    }
    value_holder->set_value(SAINode->get_container());

    // Add the field to the updated fields list in the SAI
    m_SAIUpdatesInterface->add_update(this);
  }
};

#endif
