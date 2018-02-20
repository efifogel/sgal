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
 * Template of SAI field.
 *
 * SAI fields are instantiation of SAI_fieldTemplate.
 * The class implements the SAI field functionality:
 * - get_value and set_value functions that enable access to the field
 * value (according to the value type)
 * - Registration of interest in the field using ESAIEventListener
 * 
 * The class inherites from SAI_field abstract class - which is not a 
 * template and is used within the SAI mechanism to activate 
 * functions that are not dependent.
 * The class also inherites from SAI_fieldSpecificServicesTemplate which
 * is an abstract template and should be used by external SAI users who 
 * need to use type dependent interface (i.e. get_value and set_value).
 * SAI_fieldSpecificServicesTemplate inherites from abstract 
 * SAI_fieldServices which is not a template and is a non type dependent 
 * external interface to SAI field.
*/

#ifndef EFI
#define EFI

#include "SAIFieldSpecificServicesTemplate.h"
#include "SAIField.h"
#include "CritSec.h"

template<class T, int typeId>
class SAI_fieldTemplate : 
  public SAI_field,
  public SAI_fieldSpecificServicesTemplate<T,typeId>
{
private:
  T m_defaultValue; // to return in get value function when error is detected

public:
  SAI_fieldTemplate():SAI_fieldSpecificServicesTemplate<T,typeId>(),
    SAI_field()
  {
  }

  // Destructor
  virtual ~SAI_fieldTemplate(){};

  /*! Get the field type */
  Int get_type() const
  {
    return m_typeId;
  }

  /*! Get the field's name */
  const String& get_name() const
  {
    return m_name;
  }

  /*! Get the field's id */
  Int get_iD() const
  {
    return m_id;
  }

  // Get length of the field's value in bytes
  virtual Int get_valueLength() const
  {
    return sizeof(T);
  }

  // Get the field's value into the given memory place pointed by value
  virtual void get_value(char* value) const
  {
    T* dest = (T*)value;
    EValueHolderTemplate<T>* valueHolder =
      dynamic_cast<EValueHolderTemplate<T>*>(m_valueHolder);
    if (valueHolder == NULL) {
      assert(false);
      return;
    }
    (*dest) = valueHolder->get_value();
  }

  // Set the field's value taken from the memory place pointed by value
  virtual void set_value(const char* value)
  {
    T* source = (T*)value;
    EValueHolderTemplate<T>* valueHolder =
      dynamic_cast<EValueHolderTemplate<T>*>(m_valueHolder);
    if (valueHolder == NULL) {
      assert(false);
      return;
    }    
    valueHolder->set_value(*source);

    // Add the field to the updated fields list in the SAI
    m_SAIUpdatesInterface->add_update(this);
  }

  // Value set and get functions
  virtual T get_value() const
  {
    EValueHolderTemplate<T>* valueHolder =
      dynamic_cast<EValueHolderTemplate<T>*>(m_valueHolder);
    if (valueHolder == NULL) {
      assert(false);
      return m_defaultValue;
    }
    return valueHolder->get_value();
  }

  virtual void set_value(const T& value)
  {
    EValueHolderTemplate<T>* valueHolder =
      dynamic_cast<EValueHolderTemplate<T>*>(m_valueHolder);
    if (valueHolder == NULL) {
      assert(false);
      return;
    }
    valueHolder->set_value(value);

    // Add the field to the updated fields list in the SAI
    m_SAIUpdatesInterface->add_update(this);
  }

  // Register an event listener (adds it's pointer to the event listeners list)
  virtual void RegisterFieldInterest(ESAIEventListener* eventListener)
  {
    SAI_field::RegisterFieldInterest(eventListener);
  }

  /*! Handles event cascade arriving from within the scene graph
   * For all registered event listeners
   * - Activate the FieldChanged function
   */
  virtual void Cascade()
  {
    // If the current cascade was triggered by the SAI - the event listeners
    // should not be activated - so return
    if (m_originatingCascade) {
      m_originatingCascade = false;
      return;
    }

    // lock m_eventListenersCS (will unlock automaticaly at end of function)
    TAutoLock AutoLock(&m_eventListenersCS);

    // If no event listeners exist in the list - return
    if ( m_eventListenerList.empty())
      return;

    EventListenerListType::iterator eventListenerIterator = 
      m_eventListenerList.begin();

    // Loop over the connected fields
    while( eventListenerIterator != m_eventListenerList.end())
    {
      (*eventListenerIterator)->
        EventActivated(ESAIEvent(ESAIEvent::FIELD_UPDATE,
                                 Execution_coordinator::get_sceneTime(),
                                 this));
      eventListenerIterator++;
    }
  }
};

#endif
