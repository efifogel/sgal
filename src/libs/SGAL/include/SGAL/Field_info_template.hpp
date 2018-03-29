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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_INFO_TEMPLATE_HPP
#define SGAL_FIELD_INFO_TEMPLATE_HPP

/*! \file
 * An instance of Field_info_template holds information about a field that
 * stores a value of a specific type. The type of the field value can be any
 * basic type, a complex type, or an array of the above.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_type.hpp"
#include "SGAL/Value_holder.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_info_applier.hpp"
#include "SGAL/Field_info_transformer.hpp"

SGAL_BEGIN_NAMESPACE

class Formatter;

template <bool b> struct Selector { static const bool value() { return b; } };

template <typename T> struct Is_scalar : Selector<true> {};
template <> struct Is_scalar<Vector2f> : Selector<false> {};
template <> struct Is_scalar<Vector3f> : Selector<false> {};
template <> struct Is_scalar<Rotation> : Selector<false> {};
template <> struct Is_scalar<Image> : Selector<false> {};
// Currently, a shared container is considered a scalar. This implies that
// getting or setting a specific field of a shared container is not allowed.
// Indeed, it is not supported yet.
//! \todo support getting or setting individual fields of a container.
// template <> struct Is_scalar<Shared_container> : Selector<false> {};
template <> struct Is_scalar<Boolean_array> : Selector<false> {};
template <> struct Is_scalar<Float_array> : Selector<false> {};
template <> struct Is_scalar<Int32_array> : Selector<false> {};
template <> struct Is_scalar<String_array> : Selector<false> {};
template <> struct Is_scalar<Vector2f_array> : Selector<false> {};
template <> struct Is_scalar<Vector3f_array> : Selector<false> {};
template <> struct Is_scalar<Rotation_array> : Selector<false> {};
template <> struct Is_scalar<Image_array> : Selector<false> {};
template <> struct Is_scalar<Shared_container_array> : Selector<false> {};

/*! \class Detacher Field_info_template.hpp
 * Detacher is a generic class that detaches a value of a field from the
 * container that contains the field before the field is overriden with a new
 * value. Essentially, there is nothing to for most type of field.
 * If the field is a container (shared container to be precise) itself, then
 * the container that contains the field might have been registered as an
 * observer. In this case, it must be unregistered just before it is overriden.
 * Observe that if the new field value (that is a container) should register
 * the containing container, it should be carried out by the dedicated
 * execution function stored in the field-info record.
 */
template <typename T>
struct Detacher {
  void operator()(T& value, Container* container, const Field_info* field_info)
  {}
};

template <>
struct Detacher<Shared_container> {
  void operator()(Shared_container& value,
                  Container* container, const Field_info* field_info)
  {
    if (value) {
      Container::Observer observer(container, field_info);
      value->unregister_observer(observer);
    }
  }
};

/*! \class Cloner Field_info_template.hpp
 */
template <typename T>
struct Cloner {
  void operator()(const T& source, T& target) { target = source; }
};

template <>
struct Cloner<Shared_container> {
  void operator()(Shared_container source, Shared_container& target)
  { target = Shared_container((source) ? source->clone() : nullptr); }
};

template <>
struct Cloner<Shared_container_array> {
  void operator()(const Shared_container_array& source,
                  Shared_container_array& target)
  {
    target.resize(source.size());
    std::transform(source.begin(), source.end(), target.begin(),
                   [](Shared_container cont)
                   { return Shared_container((cont) ? cont->clone() : nullptr); });
  }
};

/*! \class Mover Field_info_template.hpp
 */
template <typename T>
struct Mover {
  void operator()(const T& source, T& target) { target = source; }
};

template <>
struct Mover<Shared_container_array> {
  template <typename T>
  void operator()(const std::vector<T> source, std::vector<T>& target)
  { target = std::move(source); }
};

/*!
 */
template <typename T>
struct Handle_function {
  typedef T* (Container::* type)(const Field_info*);
};

/*! An instance of Field_info_template holds information about a field that
 * stores a value of a specific type. The type of the field value can be any
 * basic type, a complex type, or an array of the above.
 */
template <typename T, Field_type type_id>
class Field_info_template : public Field_info {
public:
  typedef typename Handle_function<T>::type     Handle;

private:
  //! The field handle function.
  Handle m_handle;

  //! The field initial value.
  T m_initial_value;

  //! Indicates whether to initialize the filed with the initial value.
  bool m_use_initial_value;

public:
  /*! Construct. */
  Field_info_template(Uint id, const std::string& name, Field_rule rule,
                      Handle handle, T initial_value,
                      Execution_function execution = nullptr,
                      bool initially_blocked = false,
                      bool use_initial_value = false) :
    Field_info(id, name, rule, execution, initially_blocked),
    m_handle(handle),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Temporary constructor.
   * \todo remove this constructor when no longer needed.
   */
  Field_info_template(Uint id, const std::string& name, Field_rule rule,
                      Handle handle, Execution_function execution = nullptr,
                      bool initially_blocked = false,
                      bool use_initial_value = false,
                      T initial_value = T()) :
    Field_info(id, name, rule, execution, initially_blocked),
    m_handle(handle),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Destruct. */
  virtual ~Field_info_template() {}

  /*! Obtain the initial value.
   * \return the initial value;
   */
  const T& get_initial_value() const { return m_initial_value; }

  /*! Obtain the field-info type id.
   */
  virtual Field_type get_type_id() const { return type_id; }

  /*! Clone the field, the (field) info of which is this object, of a source
   * container and store it in a target container.
   * \param[in] source the source container.
   * \param[in] target the target container.
   */
  virtual void clone(const Container* source, Container* target) const
  {
    const auto* source_handle =
      ((const_cast<Container*>(source))->*m_handle)(this);
    auto* target_handle = (target->*m_handle)(this);
    Cloner<T> cloner;
    cloner(*source_handle, *target_handle);
  }

  /*! Apply an operation on the field, the (field) info of which is this object,
   * of a given container.
   * \param[in] cont the container that holds the field.
   * \param[in] op the unary operator.
   */
  virtual void apply(const Container* cont, Field_value_applier& op) const
  {
    const auto* handle = ((const_cast<Container*>(cont))->*m_handle)(this);
    Field_info_applier<T> applier;
    applier(*handle, op);
  }

  /*! Transform the field, the (field) info of which is this object, of a source
   * container and store it in a target container.
   * \param[in] source the source container.
   * \param[in] target the target container.
   * \param[in] op the unary transform operator.
    */
  virtual void transform(const Container* source, Container* target,
                         Field_value_transformer& op) const
  {
    const auto* source_handle =
      ((const_cast<Container*>(source))->*m_handle)(this);
    auto* target_handle = (target->*m_handle)(this);
    Field_info_transformer<T> transformer;
    transformer(*source_handle, *target_handle, op);
  }

  /*! Move the value of the field, of a given container, associated with this
   * field-information record to the initial value of the field-information
   * record.
   * \param container the container that holds the field.
   */
  virtual void move_field_to_initial_value(Container* container)
  {
    auto* handle = ((const_cast<Container*>(container))->*m_handle)(this);
    Mover<T> mover;
    mover(*handle, m_initial_value);
  }

  /*! Create an object that holds a pointer to the value of an actual field
   * with this info.
   * \param container the container of the field.
   */
  virtual Value_holder_base* create_value_holder(Container* container) const
  {
    auto* handle = (container->*m_handle)(this);
    auto* holder = new Value_holder<T>(handle);
    if (m_use_initial_value) holder->set_value(m_initial_value);
    return holder;
  }

  /*! Detach the value of a field, the (field) info of which is this object,
   * from the container that contains the field before the field is overriden.
   * \param container (in) The container that contains the field.
   */
  virtual void detach(Container* container) const
  {
    auto* handle = (container->*m_handle)(this);     // Obtain the value
    Detacher<T> detacher;
    detacher(*handle, container, this);
  }

  /*! Determine whether the field is a scalar
   * \return \true if the field is a scalar and false otherwise.
   */
  virtual Boolean is_scalar() const { return Is_scalar<T>::value(); }

  /*! Export a field using a given formatter.
   * \param container (in) The container that contains the field.
   * \param formatter (in) The given formatter, e.g., VRML.
   */
  virtual void write(Container* container, Formatter* formatter,
                     Boolean declaration = false) const
  {
    const auto* handle = (container->*m_handle)(this);
    Field_info::write(formatter, *handle, m_initial_value, declaration);
  }
};

SGAL_END_NAMESPACE

#endif
