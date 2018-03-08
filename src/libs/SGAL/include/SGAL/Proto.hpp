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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PROTO_HPP
#define SGAL_PROTO_HPP

#include <list>
#include <string>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_type.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_info_template.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Field_info;

class Proto : public Group {
public:
  typedef boost::variant<Boolean,
                         Float,
                         Scene_time,
                         Int32,
                         Vector2f,
                         Vector3f,
                         Rotation,
                         std::string,
                         Shared_container,
                         Boolean_array,
                         Float_array,
                         Scene_time_array,
                         Int32_array,
                         Vector2f_array,
                         Vector3f_array,
                         Rotation_array,
                         String_array,
                         Shared_container_array> Variant_field;
  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Proto(Boolean proto = false);

  /*! Construct the prototype container.
   * \return the prototype container.
   */
  static Proto* prototype();

  /*! Destruct. */
  virtual ~Proto();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Clone the container (virtual constructor) with deep copy.
   * \return a clone of this container.
   */
  virtual Container* clone();

  /// \name Container protoype handling
  //@{
  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   * \return the container prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  /*! Obtain a handle to a dynamically generated field.
   * \param field_info (in) the field information record.
   * \return a pointer to the field.
   */
  template <typename T>
  T* field_handle(Uint id)
  {
    std::list<Variant_field>::iterator it = m_fields.begin();
    std::advance(it, id - LAST);
    T& field = boost::get<T>(*it);
    return &field;
  }

  /*! Obtain a handle to a dynamically generated field.
   * \param id (in) the field information record.
   * \return a pointer to the field.
   */
  template <typename T>
  T* field_handle(const Field_info* field_info)
  {
    Uint id = field_info->get_id();
    return field_handle<T>(id);
  }
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the tag (type) of the container.
   */
  virtual const String& get_tag() const;

  /*! Set the tag (type) of the container.
   */
  void set_tag(const String& tag);

  /*! Set all field info records of the prototype.
   */
  void set_field_infos(Element* element);

protected:
  /*! Execute the script function according to the event. */
  void execute(const Field_info* field_info);

  /*! Set the container prototype.
   * \param[in] the container prototype.
   */
  void set_prototype(Container_proto* prototype);

  /*! Add a field to the prototype---a generic implementation.
   * \param id (in)
   * \param name (in)
   * \param rule (in)
   * \param initial_value (in)
   * \param exec_func (in)
   * \param prototype (in)
   */
  template <Field_type type, typename ValueType_>
  Field_info* create_fi(Uint id, const std::string& name, Field_rule rule,
                        const ValueType_& initial_value,
                        Execution_function exec_func)
  {
    typedef ValueType_                                  Value_type;
    typedef typename Handle_function<Value_type>::type  Handle_function;
    typedef Field_info_template<Value_type, type>       Field_info_type;

    auto field_func =
      static_cast<Handle_function>(&Proto::field_handle<Value_type>);
    return
      new Field_info_type(id, name, rule, field_func, initial_value, exec_func);
  }

  /*! Add a field information record to the prototype.
   * \param[in] type the type of the field.
   * \param[in] name the name of the field.
   * \param[in] value the initial value of the field.
   */
  void add_field_info(Field_rule rule, Field_type type,
                      const String& name, const String& value);

private:
  /*! The tag that identifies this container type. */
  std::string m_tag;

  //! The container prototype.
  Container_proto* m_prototype;

  /*! Fields. */
  std::list<Variant_field> m_fields;
};

//! \brief constructs the prototype container.
inline Proto* Proto::prototype() { return new Proto(true); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Proto::get_tag() const { return m_tag; }

//! \brief sets the tag (type) of the container.
inline void Proto::set_tag(const String& tag) { m_tag = tag; }

//! \brief sets the container prototype.
inline void Proto::set_prototype(Container_proto* prototype)
{ m_prototype = prototype; }

SGAL_END_NAMESPACE

#endif
