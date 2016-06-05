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

#ifndef SGAL_ELEMENT_HPP
#define SGAL_ELEMENT_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <list>
#include <string>
#include <tuple>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Field_info.hpp"

SGAL_BEGIN_NAMESPACE

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Element {
public:
  typedef boost::shared_ptr<Container>                    Shared_container;

  typedef std::pair<const std::string*, std::string*>     Str_attr;
  typedef std::list<Str_attr>                             Str_attr_list;
  typedef Str_attr_list::iterator                         Str_attr_iter;

  typedef std::list<std::string>                          Str_list;
  typedef Str_list::iterator                              Str_iter;
  typedef std::pair<const std::string*, Str_list*>        Multi_str_attr;
  typedef std::list<Multi_str_attr>                       Multi_str_attr_list;
  typedef Multi_str_attr_list::iterator                   Multi_str_attr_iter;

  typedef std::pair<const std::string*, Shared_container> Cont_attr;
  typedef std::list<Cont_attr>                            Cont_attr_list;
  typedef Cont_attr_list::iterator                        Cont_attr_iter;

  typedef std::list<Shared_container>                     Cont_list;
  typedef Cont_list::iterator                             Cont_iter;
  typedef std::pair<const std::string*, Cont_list*>       Multi_cont_attr;
  typedef std::list<Multi_cont_attr>                      Multi_cont_attr_list;
  typedef Multi_cont_attr_list::iterator                  Multi_cont_attr_iter;

  // <name, <rule, type, value> >
  typedef std::pair<const std::string*,
                    std::tuple<Field_info::Field_rule,
                               Field_info::Field_type,
                               std::string*> >            Field_attr;

  typedef std::list<Field_attr>                           Field_attr_list;
  typedef Field_attr_list::iterator                       Field_attr_iter;

  typedef std::pair<const std::string*,
                    std::tuple<Field_info::Field_rule,
                               Field_info::Field_type,
                               Str_list*> >               Field_multi_str_attr;

  typedef std::list<Field_multi_str_attr>                 Field_multi_str_attr_list;
  typedef Field_multi_str_attr_list::iterator             Field_multi_str_attr_iter;

public:
  /*! Obtain the name of an attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  template <typename Iterator>
  const std::string& get_name(Iterator ai) const;

  /*! Add a string attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Str_attr& attribute, bool back = true);

  /*! Add a string-list attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Multi_str_attr& attribute, bool back = true);

  /*! Add a container attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Cont_attr& attribute, bool back = true);

  /*! Add a container-list attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Multi_cont_attr& attribute, bool back = true);

  /*! Add a field attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Field_attr& attribute, bool back = true);

  /*! Add a multi-string field attribute.
   * \param attribute the attribute.
   * \param back indicates whether to add at the back of the attribute list.
   */
  void add_attribute(Field_multi_str_attr& attribute, bool back = true);

  /* Transfer attributes from a given element into this element.
   * \param element the given element.
   */
  void splice(Element& element);

  /*! Obtain the string-attribute container. */
  Str_attr_list& get_str_attributes();

  /*! Obtain the multi-string-attribute container. */
  Multi_str_attr_list& get_multi_str_attributes();

  /*! Obtain the container-attribute container. */
  Cont_attr_list& get_cont_attributes();

  /*! Obtain the multi-container-attribute container. */
  Multi_cont_attr_list& get_multi_cont_attributes();

  /*! Obtain the field-attribute container. */
  Field_attr_list& get_field_attributes();

  /*! Obtain the field-multi-string attribute container. */
  Field_multi_str_attr_list& get_field_multi_str_attributes();

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Str_attr_iter ai);

  /*! Obtain the begin iterator of the string-attribute container. */
  Str_attr_iter str_attrs_begin();

  /*! Obtain the past-the-end iterator of the string-attribute container. */
  Str_attr_iter str_attrs_end();

  /*! Obtain the value of a string-attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  const std::string& get_value(Str_attr_iter ai) const;

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Multi_str_attr_iter ai);

  /*! Obtain the begin iterator of the multi-string-attribute container. */
  Multi_str_attr_iter multi_str_attrs_begin();

  /*! Obtain the past-the-end iterator of the multi-string-attribute container.
   */
  Multi_str_attr_iter multi_str_attrs_end();

  /*! Obtain the value of a multi-string-attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  Str_list& get_value(Multi_str_attr_iter ai) const;

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Cont_attr_iter ai);

  /*! Obtain the begin iterator of the container-attribute container. */
  Cont_attr_iter cont_attrs_begin();

  /*! Obtain the past-the-end iterator of the container-attribute container. */
  Cont_attr_iter cont_attrs_end();

  /*! Obtain the value of a container-attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  Shared_container get_value(Cont_attr_iter ai) const;

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Multi_cont_attr_iter ai);

  /*! Obtain the begin iterator of the multi-container-attribute container. */
  Multi_cont_attr_iter multi_cont_attrs_begin();

  /*! Obtain the past-the-end iterator of the multi-container-attribute
   * container.
   */
  Multi_cont_attr_iter multi_cont_attrs_end();

  /*! Obtain the value of a multi-container-attribute pointed by a given
   * iterator.
   * \param ai the string-attribute iterator.
   */
  Cont_list& get_value(Multi_cont_attr_iter ai) const;

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Field_attr_iter ai);

  /*! Obtain the begin iterator of the field-attribute container. */
  Field_attr_iter field_attrs_begin();

  /*! Obtain the past-the-end iterator of the field-attribute container. */
  Field_attr_iter field_attrs_end();

  /*! Obtain the value of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  const std::string& get_value(Field_attr_iter ai) const;

  /*! Obtain the rule of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  Field_info::Field_rule get_rule(Field_attr_iter ai) const;

  /*! Obtain the type of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  Field_info::Field_type get_type(Field_attr_iter ai) const;

  /*! Delete the attribute pointed by a given iterator.
   * \param ai the string-attribute iterator.
   */
  void mark_delete(Field_multi_str_attr_iter ai);

  /*! Obtain the begin iterator of the field-attribute container. */
  Field_multi_str_attr_iter field_multi_str_attrs_begin();

  /*! Obtain the past-the-end iterator of the field-attribute container. */
  Field_multi_str_attr_iter field_multi_str_attrs_end();

  /*! Obtain the value of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  const Str_list& get_value(Field_multi_str_attr_iter ai) const;

  /*! Obtain the rule of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  Field_info::Field_rule get_rule(Field_multi_str_attr_iter ai) const;

  /*! Obtain the type of a field-attribute pointed by a given iterator.
   * \param ai the field-attribute iterator.
   */
  Field_info::Field_type get_type(Field_multi_str_attr_iter ai) const;

  /*! Delete all attributes. */
  void delete_marked();

protected:
  /*! The container of the string attributes. */
  Str_attr_list m_str_attrs;

  /*! The container of the multi-string attributes. */
  Multi_str_attr_list m_multi_str_attrs;

  /*! The container of the container attributes. */
  Cont_attr_list m_cont_attrs;

  /*! The container of the multi-container attributes. */
  Multi_cont_attr_list m_multi_cont_attrs;

  /*! The container of the field attributes. */
  Field_attr_list m_field_attrs;

  /*! The container of the field multi-string attributes. */
  Field_multi_str_attr_list m_field_multi_str_attrs;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the name of an attribute pointed by a given iterator.
template <typename Iterator>
inline const std::string& Element::get_name(Iterator ai) const
{ return *(ai->first); }

//! \brief obtains the string-attribute list.
inline Element::Str_attr_list& Element::get_str_attributes()
{ return m_str_attrs; }

//! \brief obtains the multi-string-attribute list.
inline Element::Multi_str_attr_list& Element::get_multi_str_attributes()
{ return m_multi_str_attrs; }

//! \brief obtains the container-attribute list.
inline Element::Cont_attr_list& Element::get_cont_attributes()
{ return m_cont_attrs; }

//! \brief obtains the multi-container-attribute list.
inline Element::Multi_cont_attr_list& Element::get_multi_cont_attributes()
{ return m_multi_cont_attrs; }

//! \brief obtains the field-attribute list.
inline Element::Field_attr_list& Element::get_field_attributes()
{ return m_field_attrs; }

//! \brief obtains the field-multi-string attribute container.
inline Element::Field_multi_str_attr_list&
Element::get_field_multi_str_attributes()
{ return m_field_multi_str_attrs; }

//! \brief obtains the begin iterator of the string-attribute container.
inline Element::Str_attr_iter Element::str_attrs_begin()
{ return m_str_attrs.begin(); }

//! \brief obtains the past-the-end iterator of the string-attribute container.
inline Element::Str_attr_iter Element::str_attrs_end()
{ return m_str_attrs.end(); }

//! \brief obtains the value of a string-attribute pointed by a given iterator.
inline const std::string& Element::get_value(Str_attr_iter ai) const
{ return *(ai->second); }

//! \brief obtains the begin iterator of the multi-string-attribute container.
inline Element::Multi_str_attr_iter Element::multi_str_attrs_begin()
{ return m_multi_str_attrs.begin(); }

/*! \brief obtains the past-the-end iterator of the multi-string-attribute
 * container.
 */
inline Element::Multi_str_attr_iter Element::multi_str_attrs_end()
{ return m_multi_str_attrs.end(); }

/*! \brief obtains the value of a multi-string-attribute pointed by a given
 * iterator.
 */
inline Element::Str_list& Element::get_value(Multi_str_attr_iter ai) const
{ return *(ai->second); }

//! \brief obtains the begin iterator of the container-attribute container.
inline Element::Cont_attr_iter Element::cont_attrs_begin()
{ return m_cont_attrs.begin(); }

/*! \brief obtains the past-the-end iterator of the container-attribute
 * container.
 */
inline Element::Cont_attr_iter Element::cont_attrs_end()
{ return m_cont_attrs.end(); }

/*! \brief obtains the value of a container-attribute pointed by a given
 * iterator.
 */
inline Element::Shared_container Element::get_value(Cont_attr_iter ai) const
{ return ai->second; }

/*! \brief obtains the begin iterator of the multi-container-attribute
 * container.
 */
inline Element::Multi_cont_attr_iter Element::multi_cont_attrs_begin()
{ return m_multi_cont_attrs.begin(); }

/*! \brief obtains the past-the-end iterator of the multi-container-attribute
 * container.
 */
inline Element::Multi_cont_attr_iter Element::multi_cont_attrs_end()
{ return m_multi_cont_attrs.end(); }

/*! \brief obtains the value of a multi-container-attribute pointed by a given
 * iterator.
 */
inline Element::Cont_list& Element::get_value(Multi_cont_attr_iter ai) const
{ return *(ai->second); }

//! \brief obtains the begin iterator of the field-attribute container.
inline Element::Field_attr_iter Element::field_attrs_begin()
{ return m_field_attrs.begin(); }

//! \brief obtains the past-the-end iterator of the field-attribute container.
inline Element::Field_attr_iter Element::field_attrs_end()
{ return m_field_attrs.end(); }

//! \brief obtains the value of a field-attribute pointed by a given iterator.
inline const std::string& Element::get_value(Field_attr_iter ai) const
{ return *(std::get<2>(ai->second)); }

//! \brief obtains the rule of a field-attribute pointed by a given iterator.
inline Field_info::Field_rule Element::get_rule(Field_attr_iter ai) const
{ return std::get<0>(ai->second); }

//! \brief obtains the type of a field-attribute pointed by a given iterator.
inline Field_info::Field_type Element::get_type(Field_attr_iter ai) const
{ return std::get<1>(ai->second); }

//! \brief obtain the begin iterator of the field-attribute container.
inline Element::Field_multi_str_attr_iter Element::field_multi_str_attrs_begin()
{ m_field_multi_str_attrs.begin(); }

//! \brief obtains the past-the-end iterator of the field-attribute container.
inline Element::Field_multi_str_attr_iter Element::field_multi_str_attrs_end()
{ m_field_multi_str_attrs.end(); }

//! \brief obtains the value of a field-attribute pointed by a given iterator.
inline const Element::Str_list& Element::get_value(Field_multi_str_attr_iter ai)
  const
{ return *(std::get<2>(ai->second)); }

//! \brief obtains the rule of a field-attribute pointed by a given iterator.
inline Field_info::Field_rule Element::get_rule(Field_multi_str_attr_iter ai)
  const
{ return std::get<0>(ai->second); }

//! \brief obtains the type of a field-attribute pointed by a given iterator.
inline Field_info::Field_type Element::get_type(Field_multi_str_attr_iter ai)
  const
{ return std::get<1>(ai->second); }

SGAL_END_NAMESPACE

#endif
