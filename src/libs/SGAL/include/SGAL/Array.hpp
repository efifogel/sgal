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

/*! \file
 * A generic data structure that supports the same operation as std::vector
 * plus the provision of a pointer to the data stored as a C array.
 */

#ifndef SGAL_ARRAY_HPP
#define SGAL_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

/*! A Sequence that supports random access to elements, constant time
 * insertion and removal of elements at the end, and linear time insertion
 * and removal of elements at the beginning or in the middle. The number of
 * elements in a Array may vary dynamically; memory management is automatic.
 * Array supports the same operation as the std::vector plus the provision
 * of a pointer to the data stored as a C array.
 */
template <class Attribute>
class Array {
public:
  /*! Constructor */
  Array() :
    m_size(0),
    m_capacity(0),
    m_vector(NULL)
  {}

  /*! Constructor */
  Array(Uint n) :
    m_size(0),
    m_capacity(0),
    m_vector(NULL)  
  {
    resize(n);
  }

  /*! Copy Constructor */
  Array(const Array & other) :
    m_size(0),
    m_capacity(0),
    m_vector(NULL)  
  {
    set(other);
  }

  /*! assignment operator */
  Array& operator=(const Array& other)
    {
      // using the copy constructor and placement new.
      if (this != &other)
      {
        set(other);
      }
      return *this;
    }
  
  
  /*! Destructor */
  virtual ~Array() { clear(); }

  /*! Obtain the size */
  Uint size() const { return m_size; }

  /*! Resize the capacity */
  void resize(Uint n) {
    if (n > m_capacity) {
      if (m_vector) delete [] m_vector;
      m_vector = new Attribute[n];
#ifdef _DEBUG
      memset(m_vector, 0, n * sizeof(Attribute));
#endif
      m_capacity = n;
    }
    m_size = n;
  }
  
  /*! Clear the structure, delete the vector is necessary */
  void clear() {
    if (m_vector) {
      delete [] m_vector;
      m_vector = NULL;
      m_capacity = 0;
    }
    m_size = 0;
  }

  /*! The iterator to the first element */
  Attribute * begin() { return m_vector; }
  const Attribute * begin() const { return m_vector; }

  /*! The iterator to the past-the-end element */
  Attribute * end() { return begin() + m_size; }
  const Attribute * end() const { return begin() + m_size; }
  
  /*! Obtain the vector */
  Attribute * get_vector() { return m_vector; }
  const Attribute * get_vector() const { return m_vector; }

  /*! Indexing operator */
  Attribute & operator[](Uint n) { return *(begin() + n); }
  const Attribute & operator[](Uint n) const { return *(begin() + n); }
  
  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

private:
  
  /*! uses by copy-constructor and assignment operator to change the contents
   * of the array
   */
  void set (const Array& other)
    {
      resize(other.size());
      
      // std::copy already uses MEMMOVE (ranges can overlap) whenever possible.
      std::copy(other.m_vector, other.m_vector + m_size, m_vector);
    }

  /*! The number of coordinate */
  Uint m_size;

  /*! The capacity of the vector */
  Uint m_capacity;
  
  /*! The coordinates */
  Attribute * m_vector;
};

SGAL_END_NAMESPACE

#endif
