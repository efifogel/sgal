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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VALUE_HOLDER_HPP
#define SGAL_VALUE_HOLDER_HPP

/*! \file
 * Holds a field value.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array_types.hpp"
//! \todo #include "Critical_section.h"

SGAL_BEGIN_NAMESPACE

typedef boost::shared_ptr<Container>                   Shared_container;

/*! Value_holder holds a single value. The value type can be any basic type,
 * a complex type, or an array of the above. It is an abstract class, which
 * is used as the base for a holder of a value of a specific type.
 */
class Value_holder_base {
public:
  /*! Destructor */
  virtual ~Value_holder_base() {}

  /*! Delegate the value to the value of another Value instance */
  virtual void delegate(Value_holder_base& other) = 0;

  /*! Virtual copy constructor */
  virtual Value_holder_base* clone() = 0;
};

template <class ValueType> class Value_holder;
class Value_holder_target {
public:
  // Single fields
  virtual void delegate_impl(Value_holder<Boolean>&) = 0;
  virtual void delegate_impl(Value_holder<Float>&) = 0;
  virtual void delegate_impl(Value_holder<Uint>&) = 0;
  virtual void delegate_impl(Value_holder<Int>&) = 0;
  virtual void delegate_impl(Value_holder<Scene_time>&) = 0;
  virtual void delegate_impl(Value_holder<Vector2f>&) = 0;
  virtual void delegate_impl(Value_holder<Vector3f>&) = 0;
  virtual void delegate_impl(Value_holder<Vector4f>&) = 0;
  virtual void delegate_impl(Value_holder<Rotation>&) = 0;
  virtual void delegate_impl(Value_holder<Sphere_bound>&) = 0;
  virtual void delegate_impl(Value_holder<std::string>&) = 0;
  virtual void delegate_impl(Value_holder<Shared_container>&) = 0;

  // Multi fields
  virtual void delegate_impl(Value_holder<Float_array>&) = 0;
  virtual void delegate_impl(Value_holder<Uint_array>&) = 0;
  virtual void delegate_impl(Value_holder<Int_array>&) = 0;
  virtual void delegate_impl(Value_holder<Scene_time_array>&) = 0;
  virtual void delegate_impl(Value_holder<Vector2f_array>&) = 0;
  virtual void delegate_impl(Value_holder<Vector3f_array>&) = 0;
  virtual void delegate_impl(Value_holder<Vector4f_array>&) = 0;
  virtual void delegate_impl(Value_holder<Rotation_array>&) = 0;
  virtual void delegate_impl(Value_holder<Sphere_bound_array>&) = 0;
  virtual void delegate_impl(Value_holder<String_array>&) = 0;
  virtual void delegate_impl(Value_holder<Shared_container_array>&) = 0;
};

class Value_middle :
  public Value_holder_target, public Value_holder_base {};

template <class T>
class Velue_delegator : virtual public Value_middle {
protected :
  void delegate(Value_holder_base& other)
  { static_cast<Value_middle&>(other).delegate_impl(static_cast<T&>(*this)); }
};

template <class ValueType1, class ValueType2>
class Delegate_dispatcher {
public:
  void operator()(ValueType1* value1, ValueType2* value2)
  { SGAL_error(); }
};

template <class ValueType>
class Delegate_dispatcher<ValueType, ValueType> {
public:
  void operator()(ValueType* value1, ValueType* value2)
  { *value2 = *value1; }
};

template <>
class Delegate_dispatcher<Int, Float> {
public:
  void operator()(Int* value1, Float* value2)
  { *value2 = *value1; }
};

template <>
class Delegate_dispatcher<Float, Int> {
public:
  void operator()(Float* value1, Int* value2)
  { *value2 = *value1; }
};

// This is only for debugging
// template <>
// class Delegate_dispatcher<Float, Float> {
// public:
//   void operator()(Float* value1, Float* value2)
//   {
//     std::cout << "value1: " << value1 << ", " << *value1 << std::endl;
//     std::cout << "value2: " << value2 << ", " << *value2 << std::endl;
//     *value1 = *value2; }
// };

/*! An instance of Value_holder holds a single value of a specific type. The
 * value type can be any basic type, a complex type, or an array of the above.
 */
template <typename ValueType>
class Value_holder : public Velue_delegator<Value_holder<ValueType> > {
public:
  typedef ValueType Value_type;

private:
  /*! A pointer to a value of a specific type. */
  Value_type* m_value;

  //! \todo
  // Critical_section m_cs;
  Boolean m_allocated_internally;

public:
  /*! Constructor */
  Value_holder(Value_type* value) :
    m_value(value),
    m_allocated_internally(false)
  {
    if (value == NULL) {
      m_value = new Value_type;
      m_allocated_internally = true;
    }
  }

  /*! Destructor */
  virtual ~Value_holder()
  {
    if (m_allocated_internally) {
      delete m_value;
      m_value = NULL;
    }
  }

  /*! Set the value of the value holder */
  void set_value(const Value_type& value)
  {
    // lock m_CS to protect the value when copied
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock Auto_lock(&m_cs);
    *m_value = value;
  }

  /*! Obtain the value from the value holder */
  Value_type* get_value()
  {
    // lock m_CS to protect the value when copied
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock Auto_lock(&m_CS);
    return m_value;
  }

  //! \delegator single fields specific implementations
  //@{
  // Boolean
  void delegate_impl(Value_holder<Boolean>& other)
  {
    Delegate_dispatcher<Value_type, Boolean> dd;
    dd(m_value, other.get_value());
  }

  // Float
  void delegate_impl(Value_holder<Float>& other)
  {
    Delegate_dispatcher<Value_type, Float> dd;
    dd(m_value, other.get_value());
  }

  // Uint
  void delegate_impl(Value_holder<Uint>& other)
  {
    Delegate_dispatcher<Value_type, Uint> dd;
    dd(m_value, other.get_value());
  }

  // Int
  void delegate_impl(Value_holder<Int>& other)
  {
    Delegate_dispatcher<Value_type, Int> dd;
    dd(m_value, other.get_value());
  }

  // Scene_time
  void delegate_impl(Value_holder<Scene_time>& other)
  {
    Delegate_dispatcher<Value_type, Scene_time> dd;
    dd(m_value, other.get_value());
  }

  // Vector2f
  void delegate_impl(Value_holder<Vector2f>& other)
  {
    Delegate_dispatcher<Value_type, Vector2f> dd;
    dd(m_value, other.get_value());
  }

  // Vector3f
  void delegate_impl(Value_holder<Vector3f>& other)
  {
    Delegate_dispatcher<Value_type, Vector3f> dd;
    dd(m_value, other.get_value());
  }

  // Vector4f
  void delegate_impl(Value_holder<Vector4f>& other)
  {
    Delegate_dispatcher<Value_type, Vector4f> dd;
    dd(m_value, other.get_value());
  }

  // Rotation
  void delegate_impl(Value_holder<Rotation>& other)
  {
    Delegate_dispatcher<Value_type, Rotation> dd;
    dd(m_value, other.get_value());
  }

  // Sphere_bound
  void delegate_impl(Value_holder<Sphere_bound>& other)
  {
    Delegate_dispatcher<Value_type, Sphere_bound> dd;
    dd(m_value, other.get_value());
  }

  // std::string
  void delegate_impl(Value_holder<std::string>& other)
  {
    Delegate_dispatcher<Value_type, std::string> dd;
    dd(m_value, other.get_value());
  }

  // Shared_container
  void delegate_impl(Value_holder<Shared_container>& other)
  {
    Delegate_dispatcher<Value_type, Shared_container> dd;
    dd(m_value, other.get_value());
  }
  //@}

  //! \delegator single fields specific implementations
  //@{
  // Float_array
  void delegate_impl(Value_holder<Float_array>& other)
  {
    Delegate_dispatcher<Value_type, Float_array> dd;
    dd(m_value, other.get_value());
  }

  // Uint_array
  void delegate_impl(Value_holder<Uint_array>& other)
  {
    Delegate_dispatcher<Value_type, Uint_array> dd;
    dd(m_value, other.get_value());
  }

  // Int_array
  void delegate_impl(Value_holder<Int_array>& other)
  {
    Delegate_dispatcher<Value_type, Int_array> dd;
    dd(m_value, other.get_value());
  }

  // Scene_time_array
  void delegate_impl(Value_holder<Scene_time_array>& other)
  {
    Delegate_dispatcher<Value_type, Scene_time_array> dd;
    dd(m_value, other.get_value());
  }

  // Vector2f_array
  void delegate_impl(Value_holder<Vector2f_array>& other)
  {
    Delegate_dispatcher<Value_type, Vector2f_array> dd;
    dd(m_value, other.get_value());
  }

  // Vector3f_array
  void delegate_impl(Value_holder<Vector3f_array>& other)
  {
    Delegate_dispatcher<Value_type, Vector3f_array> dd;
    dd(m_value, other.get_value());
  }

  // Vector4f_array
  void delegate_impl(Value_holder<Vector4f_array>& other)
  {
    Delegate_dispatcher<Value_type, Vector4f_array> dd;
    dd(m_value, other.get_value());
  }

  // Rotation_array
  void delegate_impl(Value_holder<Rotation_array>& other)
  {
    Delegate_dispatcher<Value_type, Rotation_array> dd;
    dd(m_value, other.get_value());
  }

  // Sphere_bound_array
  void delegate_impl(Value_holder<Sphere_bound_array>& other)
  {
    Delegate_dispatcher<Value_type, Sphere_bound_array> dd;
    dd(m_value, other.get_value());
  }

  // String_array
  void delegate_impl(Value_holder<String_array>& other)
  {
    Delegate_dispatcher<Value_type, String_array> dd;
    dd(m_value, other.get_value());
  }
  // Shared_container_array
  void delegate_impl(Value_holder<Shared_container_array>& other)
  {
    Delegate_dispatcher<Value_type, Shared_container_array> dd;
    dd(m_value, other.get_value());
  }
  //@}

  // /*! Delegate the value to the value of another Value object. */
  // virtual void delegate(Value_holder_base* other)
  // {
  //   Value_holder<T>* other = dynamic_cast<Value_holder<Value_type>*>(other);
  //   SGAL_assertion(other);

  //    // lock the both fields critical section
  //   // (will unlock automaticaly at end of function)
  //   //! \todo Auto_lock Auto_lock(&m_cs);
  //   //! \todo Auto_lock Auto_lock_other(&specific_other->m_cs);

  //   *m_value = *(other->m_value);
  // }

  /*! Create a new instance of Value_holder with the same held value. */
  virtual Value_holder_base* clone()
  {
    // Allocate memory for the new value and copy the current value to it
    Value_type* new_value = new Value_type;
    // lock m_CS to protect the value copy
    // (will unlock automaticaly at end of function)
    //! \todo Auto_lock auto_lock(&m_cs);
    *new_value = *m_value;

    // Allocate a new holder and construct it with the allocated value
    Value_holder<Value_type>* new_holder =
      new Value_holder<Value_type>(new_value);
    // Mark the holder as created by copy - to indicate the destructor to
    // m_value on destruction
    new_holder->m_allocated_internally = true;
    return new_holder;
  }
};

SGAL_END_NAMESPACE

#endif
