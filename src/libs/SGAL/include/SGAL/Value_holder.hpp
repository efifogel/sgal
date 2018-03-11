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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VALUE_HOLDER_HPP
#define SGAL_VALUE_HOLDER_HPP

/*! \file This file contains the implementation of a double dispatch. The need
 * for a double dispatch arises from the desire to assign the value of a
 * destination field with the value of a source field. Both destination
 * and source values are held by corresponding Value_holder objects. In other
 * words, this file contains code that delegates the value of a source
 * Value_holder object to a destination Value_holder object.
 *
 * Virtual functions allow polymorphism on a single argument; however, sometimes
 * there is a need for multi-argument polymorphism. Double dispatch, commonly
 * used in C++ to implement multi-methods, does not lend easily extensible
 * code. Solutions based on function tables are difficult to implement and
 * prevent repeated derivation.

 * A viable alternative consists in storing pointers to functions into an
 * associative table, using the type_info of the classes as indexes as suggested
 * by Bjarne Stroustrup, or the name of the classes as discussed by Scott
 * Meyers. Meyers goes into the details of a table-driven implementation, but
 * the solution is rather complex and does not allow to further derive a class
 * from while still enjoying polymorphic behavior.
 *
 * Note: If you are interested in the continuous improvement of existing
 * solutions, made possible from language extensions, you may want to consider
 * the interplay of a currently unsupported feature, namely virtual member
 * template functions.Equipped with this feature the good-old double dispatch
 * becomes trivial to implement.
 *
 * A preprocessor-based solution has been implemented by Chatterton and Conway
 * to show the viability of multimethods in C++, but whenever is possible, we
 * prefer to stay within the language for real-world programming.
 *
 * The solution adopted has a deficiency, namely, extension is problematic and
 * the technique is mostly useful for cases where the base class cannot be
 * modified, for instance because it is in a shared library or the source code
 * is unavailable. Indeed, we do not want to change the base class.
 * If adding several derived classes is not expected, a dependency between base
 * and derived classes may be accepted in exchange for speed. What is normally
 * unacceptable, however, is to have a dependency between a base class used
 * across several projects and some project-specific derived classes.
 * In that case, a simple solution is to move the circular dependency inside
 * the project-specific code, making the base class reusable in different
 * contexts. That's the strategy behind the adopted technique.
 *
 * The underlying idea is simple: the class Base is expected to have a stable
 * interface, since it is reused in several projects. Therefore, it declares
 * only the pure virtual function copy(Value_holder_base&), that will be
 * subjected to multiple dispatch. Now you need a place for all the
 * project-specific versions of copy() that have been removed from
 * Value_holder_base. This role is played by another base class,
 * Value_holder_target, which defines a virtual function delegate(D&) for
 * each (project-specific) derived class D, e.g., Value_holder<Boolean>&. These
 * are the functions were the multiple polymorphism ultimately land on. Note
 * that only the implementors of derived classes need to know about
 * delegate() and users only need to know about copy().
 * Concrete classes, here Value_middle, derive from both Value_holder_base and
 * Value_holder_target, and implement Value_holder_base::copy() so that
 * the proper delegate() is called. This requires casting from a
 * Value_holder_base& (the parameter of copy()) to a Value_holder_target&
 * (where delegate is declared), a case known as cross-casting, as it
 * crosses the boundaries of a single inheritance hierarchy. Usually
 * cross-casting requires a dynamic_cast, but we can dispense with that by
 * providing a common derived class (Value_holder_middle). Since
 * Value_holder_middle derives from Value_holder_base and from
 * Value_holder_target, any reference to Value_holder_base (that we know is
 * really a reference to a derived class like A and B) can be converted into
 * a reference to Value_holder_target using two static_cast: from
 * Value_holder_base& to Value_holder_middle&, then from Value_holder_middle&
 * to Value_holder_target&. Note that static_cast is resolved at compile-time,
 * while dynamic_cast would incur a run-time performance penalty.
 * The final step is to avoid the manual implementation of copy() (where
 * the multiple dispatch takes place) in each concrete class. A general
 * implementation of copy() requires type information about either the
 * parameter (that we only know being a Value_holder_base&) or about the
 * receiver itself. Inside each derived class, we know the actual type of
 * *this, but how can we move the implementation of copy() outside the
 * derived classes without losing type information?
 * In C++, type-safe reuse is often accomplished through templates. In this
 * case, we need to derive a template class Value_copier from
 * Value_holder_middle (so to redefine Value_holder_base::copy()) and the
 * parameter of the template must provide the necessary type information.
 * Therefore, each concrete class is expected to derive from Value_copier
 * (to inherit the implementation of copy()) and also to provide its own
 * type to Value_copier as a parameter. This lead to a somewhat unusual,
 * "recursive" declaration for all the derived classes:
 *
 * template <typename ValueType>
 * class Value_holder : public Value_copier<Value_holder<ValueType> > {
 * public:
 *   // implements all the functions
 *   // delegate declared in Target
 * } ;
 *
 * The template class Value_copier plays two roles in this scheme: It
 * statically encodes the dynamic type of one parameter, and implements the
 * multiple dispatch using two static_cast, avoiding the replication of
 * boiler-plate code typical of double dispatch.
 * Consider now what happens when the following code is executed:
 *
 * Value_holder<Float> f;
 * Value_holder<Int32> i;
 * Value_holder_base& targetF = a;
 * Value_holder_base& sourceI = b;
 * targetF.copy(sourceI);
 *
 * First, Value_holder<Float>::copy(sourceI) is called. This function call is
 * dispatched to Value_copier<Value_holder<Float>>::copy(sourceI) through
 * normal polymorphism. Inside copy(), sourceI is cast to a reference to
 * Value_holder_middle, then
 * Value_holder_middle::delegate(Value_holder<Float>& target) is called. Since
 * delegate() is virtual, that means calling
 * Value_holder<Int32>::delegate(Value_holder<Float>& target), fully resolving
 * our call via multiple polymorphism.
 * To conclude, note the use of virtual inheritance from Value_holder_middle:
 * this is necessary to allow further derivation from a derived class. Without
 * virtual inheritance, a class C, as follows, would have two subobjects of
 * class Value_holder_base, and this does not correspond to the concept of
 * IS-A (a relationship where one class A is a subclass of another class B).
 *
 * class C : public Value_copier<C>, public Value_holder<Float>
 * {
 * public:
 *   // implements all the functions
 *   // delegate declared in Value_holder_target
 * };
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array_types.hpp"
//! \todo #include "Critical_section.h"

SGAL_BEGIN_NAMESPACE

typedef boost::shared_ptr<Container>                   Shared_container;

/*! Value_holder_base holds a single value. The value type can be any basic
 * type, a complex type, or an array of the above. It is an abstract class,
 * which is used as the base for a holder of a value of a specific type.
 */
class Value_holder_base {
public:
  /*! Destructor */
  virtual ~Value_holder_base() {}

  /*! Copy the value of a Value_holder source object to the value of this
   * Value_holder object.
   */
  virtual void copy(const Value_holder_base& source) = 0;
};

template <typename ValueType> class Value_holder;
class Value_holder_target {
public:
  // Single fields
  virtual void delegate(Value_holder<Boolean>&) const = 0;
  virtual void delegate(Value_holder<Float>&) const = 0;
  virtual void delegate(Value_holder<Uint>&) const = 0;
  virtual void delegate(Value_holder<Int32>&) const = 0;
  virtual void delegate(Value_holder<Scene_time>&) const = 0;
  virtual void delegate(Value_holder<Vector2f>&) const = 0;
  virtual void delegate(Value_holder<Vector3f>&) const = 0;
  virtual void delegate(Value_holder<Vector4f>&) const = 0;
  virtual void delegate(Value_holder<Rotation>&) const = 0;
  virtual void delegate(Value_holder<Bounding_sphere>&) const = 0;
  virtual void delegate(Value_holder<std::string>&) const = 0;
  virtual void delegate(Value_holder<Shared_container>&) const = 0;

  // Multi fields
  virtual void delegate(Value_holder<Boolean_array>&) const = 0;
  virtual void delegate(Value_holder<Float_array>&) const = 0;
  virtual void delegate(Value_holder<Uint_array>&) const = 0;
  virtual void delegate(Value_holder<Int32_array>&) const = 0;
  virtual void delegate(Value_holder<Scene_time_array>&) const = 0;
  virtual void delegate(Value_holder<Vector2f_array>&) const = 0;
  virtual void delegate(Value_holder<Vector3f_array>&) const = 0;
  virtual void delegate(Value_holder<Vector4f_array>&) const = 0;
  virtual void delegate(Value_holder<Rotation_array>&) const = 0;
  virtual void delegate(Value_holder<Bounding_sphere_array>&) const = 0;
  virtual void delegate(Value_holder<String_array>&) const = 0;
  virtual void delegate(Value_holder<Shared_container_array>&) const = 0;
};

class Value_middle :
  public Value_holder_target, public Value_holder_base {};

/*! Generic copier of the source value holder. */
template <typename T>
class Value_copier : virtual public Value_middle {
public:
  virtual void copy(const Value_holder_base& source)
  { static_cast<const Value_middle&>(source).delegate(static_cast<T&>(*this)); }
};

/*! Dispatcher of the delegation operation. */
template <typename ValueType1, typename ValueType2>
class Delegate_dispatcher {
public:
  void operator()(ValueType1* /* value1 */, ValueType2* /* value2 */)
  { SGAL_error(); }
};

template <typename ValueType>
class Delegate_dispatcher<ValueType, ValueType> {
public:
  void operator()(ValueType* value1, ValueType* value2) { *value2 = *value1; }
};

// Int32 <- Uint
template <>
class Delegate_dispatcher<Uint, Int32> {
public:
  void operator()(Uint* value1, Int32* value2) { *value2 = *value1; }
};

// Uint <- Int32
template <>
class Delegate_dispatcher<Int32, Uint> {
public:
  void operator()(Int32* value1, Uint* value2) { *value2 = *value1; }
};

// Int32 <- Float
template <>
class Delegate_dispatcher<Float, Int32> {
public:
  void operator()(Float* value1, Int32* value2)
  { *value2 = static_cast<Int32>(*value1); }
};

// Float <- Int32
template <>
class Delegate_dispatcher<Int32, Float> {
public:
  void operator()(Int32* value1, Float* value2)
  { *value2 = static_cast<Float>(*value1); }
};

// Int32 <- Boolean
template <>
class Delegate_dispatcher<Boolean, Int32> {
public:
  void operator()(Boolean* value1, Int32* value2)
  { *value2 = (*value1) ? 0 : 1; }
};

// Boolean <- Int32
template <>
class Delegate_dispatcher<Int32, Boolean> {
public:
  void operator()(Int32* value1, Boolean* value2)
  { *value2 = (*value1 != 0); }
};

// Uint <- Float
template <>
class Delegate_dispatcher<Float, Uint> {
public:
  void operator()(Float* value1, Uint* value2)
  { *value2 = static_cast<Uint>(*value1); }
};

// Float <- Uint
template <>
class Delegate_dispatcher<Uint, Float> {
public:
  void operator()(Uint* value1, Float* value2)
  { *value2 = static_cast<Float>(*value1); }
};

// Uint <- Boolean
template <>
class Delegate_dispatcher<Boolean, Uint> {
public:
  void operator()(Boolean* value1, Uint* value2)
  { *value2 = (*value1) ? 0 : 1; }
};

// Boolean <- Uint
template <>
class Delegate_dispatcher<Uint, Boolean> {
public:
  void operator()(Uint* value1, Boolean* value2) { *value2 = (*value1 != 0); }
};

// Float <- Boolean
template <>
class Delegate_dispatcher<Boolean, Float> {
public:
  void operator()(Boolean* value1, Float* value2)
  { *value2 = (*value1) ? 0.0f : 1.0f; }
};

// Boolean <- Float
template <>
class Delegate_dispatcher<Float, Boolean> {
public:
  void operator()(Float* value1, Boolean* value2)
  { *value2 = (*value1 != 0.0f); }
};

// Vector3f <- Vector2f
template <>
class Delegate_dispatcher<Vector2f, Vector3f> {
public:
  void operator()(Vector2f* value1, Vector3f* value2)
  {
    value2->set((*value1)[0], (*value1)[1], 0.0f);
  }
};

// Vector2f <- Vector3f
template <>
class Delegate_dispatcher<Vector3f, Vector2f> {
public:
  void operator()(Vector3f* value1, Vector2f* value2)
  {
    value2->set((*value1)[0], (*value1)[1]);
  }
};

// Boolean <- Shared container
template <>
class Delegate_dispatcher<Shared_container, Boolean> {
public:
  void operator()(Shared_container* value1, Boolean* value2)
  { *value2 = (*value1 != nullptr); }
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
class Value_holder : public Value_copier<Value_holder<ValueType> > {
public:
  typedef ValueType Value_type;

private:
  /*! A pointer to a value of a specific type. */
  Value_type* m_value;

  //! \todo
  // Critical_section m_cs;
  Boolean m_allocated_internally;

public:
  /*! Construct. */
  Value_holder(Value_type* value) :
    m_value(value),
    m_allocated_internally(false)
  {
    if (value == nullptr) {
      m_value = new Value_type;
      m_allocated_internally = true;
    }
  }

  /*! Destruct. */
  virtual ~Value_holder()
  {
    if (m_allocated_internally) {
      delete m_value;
      m_value = nullptr;
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
  void delegate(Value_holder<Boolean>& target) const
  {
    Delegate_dispatcher<Value_type, Boolean> dd;
    dd(m_value, target.get_value());
  }

  // Float
  void delegate(Value_holder<Float>& target) const
  {
    Delegate_dispatcher<Value_type, Float> dd;
    dd(m_value, target.get_value());
  }

  // Uint
  void delegate(Value_holder<Uint>& target) const
  {
    Delegate_dispatcher<Value_type, Uint> dd;
    dd(m_value, target.get_value());
  }

  // Int32
  void delegate(Value_holder<Int32>& target) const
  {
    Delegate_dispatcher<Value_type, Int32> dd;
    dd(m_value, target.get_value());
  }

  // Scene_time
  void delegate(Value_holder<Scene_time>& target) const
  {
    Delegate_dispatcher<Value_type, Scene_time> dd;
    dd(m_value, target.get_value());
  }

  // Vector2f
  void delegate(Value_holder<Vector2f>& target) const
  {
    Delegate_dispatcher<Value_type, Vector2f> dd;
    dd(m_value, target.get_value());
  }

  // Vector3f
  void delegate(Value_holder<Vector3f>& target) const
  {
    Delegate_dispatcher<Value_type, Vector3f> dd;
    dd(m_value, target.get_value());
  }

  // Vector4f
  void delegate(Value_holder<Vector4f>& target) const
  {
    Delegate_dispatcher<Value_type, Vector4f> dd;
    dd(m_value, target.get_value());
  }

  // Rotation
  void delegate(Value_holder<Rotation>& target) const
  {
    Delegate_dispatcher<Value_type, Rotation> dd;
    dd(m_value, target.get_value());
  }

  // Bounding_sphere
  void delegate(Value_holder<Bounding_sphere>& target) const
  {
    Delegate_dispatcher<Value_type, Bounding_sphere> dd;
    dd(m_value, target.get_value());
  }

  // std::string
  void delegate(Value_holder<std::string>& target) const
  {
    Delegate_dispatcher<Value_type, std::string> dd;
    dd(m_value, target.get_value());
  }

  // Shared_container
  void delegate(Value_holder<Shared_container>& target) const
  {
    Delegate_dispatcher<Value_type, Shared_container> dd;
    dd(m_value, target.get_value());
  }
  //@}

  //! \delegator multi fields specific implementations
  //@{
  // Boolean_array
  void delegate(Value_holder<Boolean_array>& target) const
  {
    Delegate_dispatcher<Value_type, Boolean_array> dd;
    dd(m_value, target.get_value());
  }

  // Float_array
  void delegate(Value_holder<Float_array>& target) const
  {
    Delegate_dispatcher<Value_type, Float_array> dd;
    dd(m_value, target.get_value());
  }

  // Uint_array
  void delegate(Value_holder<Uint_array>& target) const
  {
    Delegate_dispatcher<Value_type, Uint_array> dd;
    dd(m_value, target.get_value());
  }

  // Int32_array
  void delegate(Value_holder<Int32_array>& target) const
  {
    Delegate_dispatcher<Value_type, Int32_array> dd;
    dd(m_value, target.get_value());
  }

  // Scene_time_array
  void delegate(Value_holder<Scene_time_array>& target) const
  {
    Delegate_dispatcher<Value_type, Scene_time_array> dd;
    dd(m_value, target.get_value());
  }

  // Vector2f_array
  void delegate(Value_holder<Vector2f_array>& target) const
  {
    Delegate_dispatcher<Value_type, Vector2f_array> dd;
    dd(m_value, target.get_value());
  }

  // Vector3f_array
  void delegate(Value_holder<Vector3f_array>& target) const
  {
    Delegate_dispatcher<Value_type, Vector3f_array> dd;
    dd(m_value, target.get_value());
  }

  // Vector4f_array
  void delegate(Value_holder<Vector4f_array>& target) const
  {
    Delegate_dispatcher<Value_type, Vector4f_array> dd;
    dd(m_value, target.get_value());
  }

  // Rotation_array
  void delegate(Value_holder<Rotation_array>& target) const
  {
    Delegate_dispatcher<Value_type, Rotation_array> dd;
    dd(m_value, target.get_value());
  }

  // Bounding_sphere_array
  void delegate(Value_holder<Bounding_sphere_array>& target) const
  {
    Delegate_dispatcher<Value_type, Bounding_sphere_array> dd;
    dd(m_value, target.get_value());
  }

  // String_array
  void delegate(Value_holder<String_array>& target) const
  {
    Delegate_dispatcher<Value_type, String_array> dd;
    dd(m_value, target.get_value());
  }

  // Shared_container_array
  void delegate(Value_holder<Shared_container_array>& target) const
  {
    Delegate_dispatcher<Value_type, Shared_container_array> dd;
    dd(m_value, target.get_value());
  }
  //@}

  // //! \brief copies the value to the value of another Value object.
  // virtual void copy(Value_holder_base* source)
  // {
  //   SGAL_assertion(source);
  //   Value_holder<T>* other = dynamic_cast<Value_holder<Value_type>*>(source);

  //   // lock the critical section of both fields:
  //   // (will unlock automaticaly at end of function)
  //   //! \todo Auto_lock Auto_lock(&m_cs);
  //   //! \todo Auto_lock Auto_lock_other(&specific_other->m_cs);

  //   set_value(*(other->get_value()));
  // }
};

SGAL_END_NAMESPACE

#endif
