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

#ifndef SGAL_CONTAINER_PROTO_HPP
#define SGAL_CONTAINER_PROTO_HPP

/*! \file
 * The prototype of a container type
 *
 * Every container type should create a Container_proto when it is
 * constructed. The constructed container type should use add_field_info()
 * for each container field.
 *
 * This type maintains 2 search data-structures to obtain a field-infos by its
 * id and by its name respectively.
 *
 * This type also maintains a pointer to an ancestor container type
 * Container_proto, in order to access the field infos of the ancestors in
 * the container types inheritance hierarchy.
 */

#include <string>
#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Container_proto {
public:
  typedef std::map<Uint,Field_info*>            Field_info_id_map;
  typedef Field_info_id_map::const_iterator     Field_info_id_const_iter;
  typedef Field_info_id_map::iterator           Field_info_id_iter;

  typedef std::map<std::string, Field_info*>    Field_info_name_map;
  typedef Field_info_name_map::const_iterator   Field_info_name_const_iter;

  class Id_const_iterator {
  public:
    typedef Field_info_id_const_iter            Iterator;

    typedef Iterator::iterator_category         iterator_category;
    typedef Iterator::value_type                value_type;
    typedef Iterator::difference_type           difference_type;
    typedef Iterator::pointer                   pointer;
    typedef Iterator::reference                 reference;

  public:
    /*! Constructor. */
    Id_const_iterator(const Container_proto* prototype) :
      m_prototype(prototype),
      m_it()
    {}

    /*! Constructor. */
    Id_const_iterator(const Container_proto* prototype, Iterator it) :
      m_prototype(prototype),
      m_it(it)
    {}

    /*! operator* */
    const value_type operator*() const { return *m_it; }

    // value_type operator*() { return *m_it; }

    /*! Operator pre ++ */
    Id_const_iterator& operator++()
    {
      ++m_it;
      if (m_it == m_prototype->field_info_ids_end()) {
        const Container_proto* ancestor = m_prototype->get_ancestor();
        if (ancestor) {
          m_prototype = ancestor;
          m_it = m_prototype->field_info_ids_begin();
        }
      }
      return *this;
    }

    /*! Operator == */
    bool operator==(const Id_const_iterator& other)
    { return (m_it == other.m_it); }

    /*! Operator != */
    bool operator!=(const Id_const_iterator& other)
    { return (m_it != other.m_it); }

    /*! Operator post ++ */
    Id_const_iterator operator++(int)
    {
      Id_const_iterator tmp = *this;
      ++*this;
      return tmp;
    }

  private:
    const Container_proto* m_prototype;
    Iterator m_it;
  };

  /*! Obtain the begin iterator of the extended range of field info ids.
   */
  Id_const_iterator ids_begin(const Container_proto* prototype) const;

  /*! Obtain the past-the-end iterator of the extended range of field info ids.
   */
  Id_const_iterator ids_end(const Container_proto* prototype) const;

  /*! Constructor */
  Container_proto(Container_proto* ancestor = nullptr);

  /*! Destructor - deletes each added field info */
  virtual ~Container_proto();

  /*! Add a field information record.
   * \param field_info (in) the field information record to be added.
   * The using class should allocate the added field info record.
   * The destructor will delete it when the container prototype is deleted.
   */
  void add_field_info(Field_info* field_info);

  /*! Obtain the field info by id
   * \param id the id of the field info
   * \return the requested field info
   */
  Field_info* get_field_info(Uint id) const;

  /*! Obtain the field info by name
   * \param name (in) the name of the field info
   * \return A pointer to the requested field info
   */
  Field_info * get_field_info(const std::string& name) const;

  /*! Obtain the size of the filed info ids container.
   * \return size of the filed info ids container.
   */
  Uint field_info_ids_size() const;

  /*! Obtain the begin iterator of the filed info ids container.
   * \return the begin iterator.
   */
  Field_info_id_const_iter field_info_ids_begin() const;

  /*! Obtain the past-the-end iterator of the filed info ids container.
   * \return the past-the-end iterator.
   */
  Field_info_id_const_iter field_info_ids_end() const;

  /*! Obtain the ancestor.
   * \return The ancestor
   */
  const Container_proto* get_ancestor() const;

private:
  /*! A search structure for field-infos by field-info ids */
  Field_info_id_map m_field_info_ids;

  /*! A search structure for field-infos by field-info names */
  Field_info_name_map m_field_info_names;

  /*! The ancestor container-prototype */
  Container_proto* m_ancestor;

  /*! The first id */
  Uint m_first_id;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the size of the filed info ids container.
inline Uint Container_proto::field_info_ids_size() const
{ return m_field_info_ids.size(); }

//! \brief obtains the begin iterator of the filed info ids container.
inline Container_proto::Field_info_id_const_iter
Container_proto::field_info_ids_begin() const
{ return m_field_info_ids.begin(); }

//! \brief obtains the past-the-end iterator of the filed info ids container.
inline Container_proto::Field_info_id_const_iter
Container_proto::field_info_ids_end() const
{ return m_field_info_ids.end(); }

//! \brief obtains the ancestor.
inline const Container_proto* Container_proto::get_ancestor() const
{ return m_ancestor; }

SGAL_END_NAMESPACE

#endif
