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
 * SAI field of SFString type.
 * 
 * Implement get and set value in bytes for string - Sets and gets the string
 * as char*.
 */

#ifndef EFI
#define EFI

#include "SAIFieldTemplate.h"
#include "SAINode.h"

class ESAISFStringField : public SAI_fieldTemplate<String,SFSTRING> {
public:
  // Get length of the field's value in bytes
  virtual Int get_valueLength() const
  {
    EValueHolderTemplate<String> * valueHolder =
      dynamic_cast<EValueHolderTemplate<String>*>(m_valueHolder);
    if (valueHolder == NULL)
    {
      assert(false);
      return 0;
    }
    return (valueHolder->get_value()).length()+1;
  }

  // Implement get value in bytes for string
  virtual void get_value(char * value) const
  {
    EValueHolderTemplate<String> * valueHolder =
      dynamic_cast<EValueHolderTemplate<String>*>(m_valueHolder);
    if (valueHolder == NULL)
    {
      assert(false);
      return;
    }
    memcpy(value,
           ((valueHolder->get_value()).c_str()),
           (valueHolder->get_value()).length()+1);  
  }

  // Implement get value in bytes for string
  virtual void set_value(const char* value)
  {
    EValueHolderTemplate<String> * valueHolder =
      dynamic_cast<EValueHolderTemplate<String>*>(m_valueHolder);
    if (valueHolder ==NULL) {
      assert(false);
      return;
    }    
    valueHolder->set_value(String(value));

    // Add the field to the updated fields list in the SAI
    m_SAIUpdatesInterface->add_update(this);    
  }   
};

#endif
