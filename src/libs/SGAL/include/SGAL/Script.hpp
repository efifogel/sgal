// Copyright (c) 2014 Israel.
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

#ifndef SGAL_SCRIPT_HPP
#define SGAL_SCRIPT_HPP

#include <vector>
#include <string>
#include <set>

#include <v8.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Script_base.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;
class Field;
class Field_info;
class Formatter;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Script Script.hpp
 * Script is a node used to program behaviour in a scene. Script nodes typically
 * 1. signify a change or user action;
 * 2. receive events from other nodes;
 * 3. contain a program module that performs some computation;
 * 4. effect change somewhere else in the scene by sending events.
 * Each Script node has associated programming language code, referenced by the
 * url field, that is executed to carry out the Script node's function.
 */
class SGAL_SGAL_DECL Script : public Script_base {
public:
  enum {
    FIRST = Script::LAST-1,
    LAST
  };

  /*! Constructor.
   * \param[in] proto determines whether to construct a prototype.
   */
  Script(Boolean proto = false);

  /*! Destructor. */
  virtual ~Script();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Script* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Export all fields of this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_fields(Formatter* formatter);

  /*! Add a field information record to the script node.
   * \param[in] type the type of the field.
   * \param[in] name the name of the field.
   * \param[in] value the initial value of the field.
   */
  void add_field_info(Field_rule rule, Field_type type, const String& name,
                      const std::string& value);

  /*! Execute the script function according to the event. */
  virtual void execute(const Field_info* field_info);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototypes. */
  static std::vector<Container_proto*> s_prototypes;

  /*! Intercept getting scalars.
   * \param[in] info
   */
  static void getter(v8::Local<v8::String> property,
              const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! Intercept setting scalars.
   * \param[in] property
   * \param[in] value
   * \param[in] info
   */
  static void setter(v8::Local<v8::String> property,
                     v8::Local<v8::Value> value,
                     const v8::PropertyCallbackInfo<void>& info);

  /*! Intercept getting arrays.
   * \param[in] property
   * \param[in] info
   */
  static void array_getter(v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! Intercept setting arrays.
   * \param[in] property
   * \param[in] value
   * \param[in] info
   */
  static void array_setter(v8::Local<v8::String> property,
                           v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void>& info);

  /*! Intercept getting an array element by index.
   * \param[in] property
   * \param[in] info
   */
  static void indexed_getter(uint32_t index,
                             const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! Intercept setting an array element by index.
   * \param[in] property
   * \param[in] value
   * \param[in] info
   */
  static void indexed_setter(uint32_t index,
                             v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! Clear the set of altered field-information records.
   */
  void clear_assigned();

  /*! Insert a field-information into the set of altered field-information
   * records.
   */
  void insert_assigned(const Field_info* field_info);

  /*! Traverse all fields that has been assigned during the execution of the
   * script, and cascade them.
   * We use the flag m_direct_output to determine whether the assignment is
   * applied immediately or deferred until the end of the execution of the
   * script.
   */
  void cascade_assigned();

  /*! The script protocol. */
  Protocol m_protocol;

  /*! The unique id of the script. */
  size_t m_id;

  /*! The v8 sandboxed execution context. */
  typedef v8::CopyablePersistentTraits<v8::Context>
    Copyable_persistent_context_traits;
  v8::Persistent<v8::Context, Copyable_persistent_context_traits> m_context;

  /*! The isolated instance of the V8 engine. */
  v8::Isolate* m_isolate;

  /*! A set of altered field-information records. */
  std::set<const Field_info*> m_assigned;

  /*! Add setter and getter callbacks to the engine for every field and every
   * output field.
   * \param[in] global the global object associated with the context.
   */
  void add_callbacks(v8::Local<v8::ObjectTemplate> global);

  /*! Obtain the first argument for the call.
   * \param[in] field_info the field information record of the field that
   *            stores the value of the first argument.
   * \return the argument.
   */
   v8::Handle<v8::Value> get_argument(const Field_info* field_info);

  /*! Bound the script.
   */
  void bound_script();

  /*! Convert a single Boolean field value to a v8 engine Boolean.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_boolean(const Field_info* field_info);

  /*! Convert a single float  field value to a v8 engine float.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_float(const Field_info* field_info);

  /*! Convert a single time field value to a v8 engine float.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_time(const Field_info* field_info);

  /*! Convert a single int32 field value to a v8 engine int32.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_int32(const Field_info* field_info);

  /*! Convert a single string field value to a v8 engine string.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_string(const Field_info* field_info);

  /*! Convert a single Vector2f field value to a v8 engine array of 2 floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_vector2f(const Field_info* field_info);

  /*! Convert a single Vector3f field value to a v8 engine array of 3 floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_vector3f(const Field_info* field_info);

  /*! Convert a single shared container field value to a v8 engine external.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_single_external(const Field_info* field_info);

  /*! Convert a multi Boolean field value to a v8 engine array of Booleans.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_boolean(const Field_info* field_info);

  /*! Convert a multi float field value to a v8 engine array of floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_float(const Field_info* field_info);

  /*! Convert a multi time field value to a v8 engine array of floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_time(const Field_info* field_info);

  /*! Convert a multi int32 field value to a v8 engine array of int32.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_int32(const Field_info* field_info);

  /*! Convert a multi string field value to a v8 engine array of strings.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_string(const Field_info* field_info);

  /*! Convert a multi Vector2f field value to a v8 engine array of arrays of
   * 2 floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_vector2f(const Field_info* field_info);

  /*! Convert a multi Vector3f field value to a v8 engine array of arrays of
   * 3 floats.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_vector3f(const Field_info* field_info);

  /*! Convert a multi shared-container field value to a v8 engine array of
   * exernals.
   * \param[in] field_info the field information record of the field that
   *            stores the value.
   */
  v8::Handle<v8::Value> get_multi_external(const Field_info* field_info);
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Script* Script::prototype() { return new Script(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Script::create() { return new Script(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Script::get_tag() const { return s_tag; }

//! \brief converts a single float field value to a v8 engine float.
inline v8::Handle<v8::Value>
Script::get_single_float(const Field_info* field_info)
{ return v8::Number::New(m_isolate, *(field_handle<Float>(field_info))); }

//! \brief converts a single time field value to a v8 engine float.
inline v8::Handle<v8::Value>
Script::get_single_time(const Field_info* field_info)
{ return v8::Number::New(m_isolate, *(field_handle<Scene_time>(field_info))); }

//! \brief converts a single int32 field value to a v8 engine int32.
inline v8::Handle<v8::Value>
Script::get_single_int32(const Field_info* field_info)
{ return v8::Int32::New(m_isolate, *(field_handle<Int32>(field_info))); }

//! \brief clears the set of altered field-information records.
inline void Script::clear_assigned() { m_assigned.clear(); }

/*! \brief inserts a field-information into the set of altered field-information
 * records.
 */
inline void Script::insert_assigned(const Field_info* field_info)
{ m_assigned.insert(field_info); }

SGAL_END_NAMESPACE

#endif
