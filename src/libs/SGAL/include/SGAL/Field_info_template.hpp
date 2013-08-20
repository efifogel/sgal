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
#include "SGAL/Member_offset_type.hpp"
#include "SGAL/Value_holder.hpp"
#include "SGAL/Execution_function.hpp"
//! \todo #include "SAI_field_template.h"

SGAL_BEGIN_NAMESPACE

template <typename T>
struct Handle_function {
  typedef T* (Container::* type)(Field_info*);
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

  /*! The field offset */
  Member_offset_type m_offset;

  /*! The field initial value. */
  T m_initial_value;

  /*! Indicates whether to initialize the filed with the initial value. */
  bool m_use_initial_value;

public:
  /*! Constructor. */
  Field_info_template(Uint id, const std::string& name,
                      Handle handle,
                      Execution_function execution = NULL,
                      bool initially_blocked = false,
                      bool use_initial_value = false,
                      T initial_value = T()) :
    Field_info(id, name, execution, initially_blocked),
    m_handle(handle),
    m_offset(0),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Constructor. */
  Field_info_template(Uint id, const std::string& name,
                      Member_offset_type offset,
                      Execution_function execution = NULL,
                      bool initially_blocked = false,
                      bool use_initial_value = false,
                      T initial_value = T()) :
    Field_info(id, name, execution, initially_blocked),
    m_handle(NULL),
    m_offset(offset),
    m_initial_value(initial_value),
    m_use_initial_value(use_initial_value)
  {}

  /*! Destructor. */
  virtual ~Field_info_template() {}

  /*! Obtain the field-info type id. */
  virtual Uint get_type_id() const { return type_id; }

  /*! Obtain the field offset. */
  Member_offset_type get_offset() const { return m_offset; }

  /*! Create a Value_holder instance using the field info for the given
   * container.
   * \param container the container.
   */
  virtual Value_holder_base* create_value_holder(Container* container)
  {
    T* handle = (container->*m_handle)(this);
    Value_holder<T>* holder = new Value_holder<T>(handle);
    if (m_use_initial_value) holder->set_value(m_initial_value);
    return holder;
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
