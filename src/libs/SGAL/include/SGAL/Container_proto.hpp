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

class SGAL_SGAL_DECL Container_proto {
private:
  typedef std::map<Uint,Field_info*>            Field_info_id_map;
  typedef Field_info_id_map::const_iterator     Field_info_id_const_iter;
  typedef Field_info_id_map::iterator           Field_info_id_iter;
  
  typedef std::map<std::string, Field_info*>    Field_info_name_map;
  typedef Field_info_name_map::const_iterator   Field_info_name_const_iter;

  /*! A search structure for field-infos by field-info ids */
  Field_info_id_map m_field_info_ids;

  /*! A search structure for field-infos by field-info names */
  Field_info_name_map m_field_info_names;

  /*! The ancestor container-prototype */
  Container_proto * m_ancestor;

  /*! The first id */
  Uint m_first_id;

public:
  /*! Constructor */
  Container_proto(Container_proto* ancestor = NULL);

  /*! Destructor - deletes each added field info */
  virtual ~Container_proto();

  /*! @defgroup field_info_additions Field_info addition
   * \brief adds() a field info to the container prototype
   *  @{
   */

  /*! The using class should allocate the added field info
   * The destructor will delete it when the container prototype is deleted
   * \todo this is not right!
   */
  void add_field_info(Field_info* field_info);

#if 0
  /*! The field info will be allocated according to the given params
   * The destructor will delete it when the container prototype is deleted
   */
  void add_field_info(Uint id, const std::string& name,
                      const std::string& type, const std::string& value,
                      Ulong offset, Execution_function execution = NULL);

  /*! The field info will be allocated according to the given params
   * The destructor will delete it when the container prototype is deleted
   */
  void add_field_info(Uint id,  const std::string& name,
                      const std::string& type, Container* value,
                      Ulong offset, Execution_function execution = NULL);
#endif
  
  /*! @} */ // end of field_info_additions
  
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
};

SGAL_END_NAMESPACE

#endif
