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

#ifndef SGAL_VALUE_HOLDER_HPP
#define SGAL_VALUE_HOLDER_HPP

/*! \file
 * Value_holder holds a field value.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
//! \todo #include "Critical_section.h"

SGAL_BEGIN_NAMESPACE

/*! Value_holder holds a single value. The value type can be any basic type,
 * a complex type, or an array of the above. It is an abstract class, which
 * is used as the base for a holder of a value of a specific type.
 */
class Value_holder {
public:
  /*! Destructor */
  virtual ~Value_holder(){}

  /*! Set the value to the value of another Value instance */
  virtual void set(Value_holder * other) = 0;

  /*! Virtual copy constructor */
  virtual Value_holder * clone() = 0;
};

/*! An instance of Value_holder_specifc holds a single value of a specific
 * type. The value type can be any basic type, a complex type, or an array
 * of the above.
 */
template<class T>
class Value_holder_specific : public Value_holder {
private:
  /*! A pointer to a value of a specific type */
  T * m_value;

  //! \todo
  // Critical_section m_cs;
  Boolean m_allocated_internally;

public:
  /*! Constructor */
  Value_holder_specific(T * value) :
    m_value(value),
    m_allocated_internally(false)
  {
    if (value == NULL) {
      m_value = new T;
      m_allocated_internally = true;
    }
  }

  /*! Destructor */
  virtual ~Value_holder_specific()
  { if (m_allocated_internally) delete m_value; }

  /*! Set the value of the value holder */
  void set_value(const T & value)
  {
    // lock m_CS to protect the value when copied
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock Auto_lock(&m_cs);
    *m_value = value;
  }

  /*! Obtain the value from the value holder */
  T & get_value()
  {
    // lock m_CS to protect the value when copied
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock Auto_lock(&m_CS);
    return *m_value;
  }

  /*! Set the value to the value of another Value instance */
  virtual void set(Value_holder * other)
  {
    Value_holder_specific<T> * specific_other = 
      dynamic_cast<Value_holder_specific<T>*>(other);
    SGAL_assertion(specific_other);

    // lock the both fields critical section
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock Auto_lock(&m_cs);
    //! \todo Auto_lock Auto_lock_other(&specific_other->m_cs);

    *m_value = *(specific_other->m_value);
  }

  /*! Create a new instance of Value_holder with the same held value */
  virtual Value_holder * clone() 
  {
    // Allocate memory for the new value and copy the current value to it
    T * new_value = new T;
    // lock m_CS to protect the value copy
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock auto_lock(&m_cs);
    *new_value = *m_value;

    // Allocate a new holder and construct it with the allocated value
    Value_holder_specific<T>* new_holder =
      new Value_holder_specific<T>(new_value);
    // Mark the holder as created by copy - to indicate the destructor to
    // m_value on destruction
    new_holder->m_allocated_internally = true;

    return new_holder;
  }
};

SGAL_END_NAMESPACE

#endif
