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

#ifndef SGAL_FIELD_INFO_TEMPLATE_HPP
#define SGAL_FIELD_INFO_TEMPLATE_HPP

/*! \file
 * An instance of Field_info_template holds information about a field that
 * stores a value of a specific type. The type of the field value can be any
 * basic type, a complex type, or an array of the above.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Value_holder.hpp"
#include "SGAL/Execution_function.hpp"
//! \todo #include "SAI_field_template.h"

SGAL_BEGIN_NAMESPACE

class Formatter;

template <typename T>
struct Handle_function {
  typedef T* (Container::* type)(const Field_info*);
};

/*! An instance of Field_info_template holds information about a field that
 * stores a value of a specific type. The type of the field value can be any
 * basic type, a complex type, or an array of the above.
 */
template <typename T, Uint type_id>
class Field_info_template : public Field_info {
public:
  typedef typename Handle_function<T>::type     Handle;

private:
  /*! The field handle function. */
  Handle m_handle;

  /*! The field initial value. */
  T m_initial_value;

  /*! Indicates whether to initialize the filed with the initial value. */
  bool m_use_initial_value;

public:
  /*! Constructor. */
  Field_info_template(Uint id, const std::string& name,
                      Handle handle, T initial_value,
                      Execution_function execution = NULL,
                      bool initially_blocked = false,
                      bool use_initial_value = false) :
    Field_info(id, name, execution, initially_blocked),
    m_handle(handle),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Temporary constructor.
   * \todo remove this constructor when no longer needed.
   */
  Field_info_template(Uint id, const std::string& name,
                      Handle handle,
                      Execution_function execution = NULL,
                      bool initially_blocked = false,
                      bool use_initial_value = false,
                      T initial_value = T()) :
    Field_info(id, name, execution, initially_blocked),
    m_handle(handle),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Destructor. */
  virtual ~Field_info_template() {}

  /*! Obtain the field-info type id. */
  virtual Uint get_type_id() const { return type_id; }

  /*! Create an object that holds a pointer to the value of an actual field
   * with this info.
   * \param container the container of the field.
   */
  virtual Value_holder_base* create_value_holder(Container* container)
  {
    T* handle = (container->*m_handle)(this);
    Value_holder<T>* holder = new Value_holder<T>(handle);
    if (m_use_initial_value) holder->set_value(m_initial_value);
    return holder;
  }

  /*! Obtain the attribute of a field in a given container, where this field
   * info contains the information of the field.
   */
  virtual void write(Container* container, Formatter* formatter) const
  {
    const T* handle = (container->*m_handle)(this);
    Field_info::write(formatter, *handle);
  }

  /*! \todo
  // Creates an SAI_field of the same type id and name
  // as this field info.
  // The function also create a new SAI_field_info of the same type
  // and initializes the new SAI_field with it.
  virtual SAI_field_services * create_sai_field()
  {
    return new SAI_field_template<T,type_id>;
  }
  */
};

SGAL_END_NAMESPACE

#endif
