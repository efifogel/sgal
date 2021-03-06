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

#ifndef SGAL_CONTAINER_HPP
#define SGAL_CONTAINER_HPP

/*! \file
 * Manages fields and node to node field connections.
 *
 * Maintains a list of fields that are connected to or connected from fields
 * in other containers.
 *
 * The following explains what should be done to connect two fields.
 * by example with the navigation sensor, which inherites from Container.
 * Assume that navSensor is of type Navigation_sensor* and pointing to the
 * navigation sensor instance in the scene graph.
 *
 * navSensor->add_field(Navigation_sensor::POSITION);
 * navSensor->add_field(Navigation_sensor::rotation);
 * Field* pos_field = nav_sensor->get_field(Navigation_sensor::POSITION);
 * Field* orient_field = nav_sensor->get_field(Navigation_sensor::rotation);
 *
 * Assume that in the same way transform_pos_field and transform_orient_field
 * are of type Field* and are pointing to the corresponding fields we want to
 * connect to.
 *
 * pos_field->connect(transform_pos_field);
 * orient_field->connect(transform_orient_field);
 *
 * that should connect the fields
 *
 * pos_field->Cascade();
 * orient_field->Cascade();
 */

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include <list>
#include <map>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Container_observer.hpp"
#include "SGAL/Field_value_applier.hpp"
#include "SGAL/Field_value_transformer.hpp"

SGAL_BEGIN_NAMESPACE

class Field;
class Field_info;
class Context;
class Container_proto;
class Element;
class Scene_graph;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Container : public Container_observer {
private:
  int m_base;

public:
  typedef std::pair<Container_observer*, const Field_info*>     Observer;
  typedef std::list<Observer>                                   Observer_list;
  typedef Observer_list::iterator                               Observer_iter;

  // Fields unique id enumeration
  enum {
    LAST
  };

  /*! Construct.
   */
  Container(Boolean proto = false);

  /*! Destructor. */
  virtual ~Container();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create() = 0;

  /*! Clone the container (virtual constructor) with deep copy.
   * \return a clone of this container.
   */
  virtual Container* clone();

  /*! Apply a unary operation on every field of the container.
   * \param op[in] the unary operation to apply.
   */
  virtual void apply(Field_value_applier& op);

  /*! Transform every field of this container using a unary operation into
   * a corresponding field in a target container.
   * \param target[in] the target container.
   * \param op[in] the unary operation to apply.
   */
  virtual void transform(Container* target, Field_value_transformer& op);

  /*! Initialize the node prototype.
   */
  virtual void init_prototype() {};

  /*! Delete the node prototype.
   */
  virtual void delete_prototype() {};

  /*! Obtain the node prototype.
   */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this container.
   */
  virtual void set_attributes(Element* element);

  /*! Obtain the attributes of this container.
   */
  virtual void get_attributes(Element* element);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* /* scene_graph */) {}

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export all fields of this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_fields(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Obtain the name of the container provided in the USE and DEF tags.
   */
  const std::string& get_name() const;

  /*! Set the name of the container provided in the USE and DEF tags.
   */
  void set_name(const std::string& name);

  /*! Obtain the tag (type) of the container
   */
  virtual const std::string& get_tag() const = 0;

  /*! Add a field of a given id. Obtains the appropriate field info, and use it
   * to add the new field. If a field with the given id already exists simply
   * return it. Otherwiese, create a new field first.
   * \param id (in) the id of the field
   * \return the resulting field or nullptr if the requested id is invalid
   */
  Field* add_field(Uint id);

  /*! Add a field of a given name. Obtains the suitable field info, and use it
   * to add the new field. If a field with the given id already exists simply
   * return it. Otherwiese, create a new field first.
   * \param name (in) the name of the field.
   * \return the resulting field or nullptr if the requested name is invalid.
   */
  Field* add_field(const std::string& name);

  /*! Add a field for a field info. If a field with the given field info
   * already exists, return it. Otherwiese, create a new field first.
   * \param field_info (in) the suitable field info for the field
   * \return the resulting field or nullptr if the requested name is invalid.
   */
  Field* add_field(const Field_info* field_info);

  /*! Obtain a field by id.
   * \param id (in) the requested field id.
   * \return The obtained field pointer.
   */
  Field* get_field(Uint id);

  /*! Obtain a field by name.
   * \param name (in) the requested field name.
   * \return The obtained field.
   */
  Field* get_field(const std::string& name);

  /*! Obtain a field info by id.
   * \param id (in) the id of the field.
   * \return The requested field info.
   */
  Field_info* get_field_info(Uint id);

  /*! Obtain a field info by name.
   *  Uses the node prototype to get the field info.
   * \param name (in) the name of the field.
   * \return The obtained field info.
   */
  Field_info* get_field_info(const std::string& name);

  /*! Obtain a field by a field info.
   * It loops over the fields and finds the one with the requested info.
   * \param (in) field_info the requested field info.
   * \return The obtained field.
   */
  Field* get_field(const Field_info* field_info);

  /*! Force re-rendering. */
  virtual void set_rendering_required(const Field_info* /* field_info */ = 0)
  {
    if (m_execution_coordinator)
      m_execution_coordinator->set_rendering_required();
  }

  virtual Boolean attach_context(Context* /* context */) { return false; }
  virtual Boolean detach_context(Context* /* context */ = 0) { return false; }

  /*! Process change of field.
   * \param field_info
   */
  virtual void field_changed(const Field_info* field_info);

  /* Sets the execution coordinator.
   * \param ec
   */
  void set_execution_coordinator(Execution_coordinator* ec)
  { m_execution_coordinator = ec; }

  /*! Obtain a source field with a given name.
   */
  Field* get_source_field(const std::string& src_field_name);

  /*! Obtain a destination field with a given name.
   */
  Field* get_destination_field(const std::string& dst_field_name);

  /*! Register an observer.
   */
  void register_observer(Observer& observer)
  { m_observers.push_back(observer); }

  /*! Unregister an observer.
   */
  void unregister_observer(Observer& observer)
  { m_observers.remove(observer); }

  /*! Processe change of content. */
  void process_content_changed();

  typedef std::map<const Field_info*, Field*>           Field_map;

  /*! Obtain the xxx. */
  const Field_map& get_fields() const { return m_fields; }

protected:
  /*! A pointer to the execution coordinator. */
  Execution_coordinator* m_execution_coordinator;

  /*! Deeply copy a given container. Containers that are fields of the source
   * container are cloned, and so are elements of array of containers that are
   * fields of the source container
   * \param other[in] the container to copy.
   */
  void copy(const Container* other);

private:
  //! The name of the container. Given as the value of the DEF tag.
  std::string m_name;

  //! A search structure to find Field_info's for fields.
  Field_map m_fields;

  //! A list of observers.
  Observer_list m_observers;

  //! \todo Critical_section m_field_cs;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the node prototype.
inline Container_proto* Container::get_prototype() { return nullptr; }

//! \brief sets the attributes of this container.
inline void Container::set_attributes(Element* /* elem */) {}

//! \brief obtains the attributes of this container.
inline void Container::get_attributes(Element* /* elem */) {}

//! \brief obtains the name of the container provided in the USE and DEF tags.
inline const std::string& Container::get_name() const { return m_name; }

//! \brief sets the name of the container provided in the USE and DEF tags.
inline void Container::set_name(const std::string& name) { m_name = name; }

SGAL_END_NAMESPACE

#endif
