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

#include <list>
#include <string>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

#include <v8.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Script_base.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

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

  enum Protocol {
    PROTOCOL_INVALID = 0,
    PROTOCOL_CUSTOM_ECMASCRIPT, // "javascript: ..."
    PROTOCOL_ECMASCRIPT,        // "http://bar.com/foo.js"
    PROTOCOL_JAVA_BYTECODE      // "http://bar.com/foo.class"
  };

  typedef boost::shared_ptr<Container>                  Shared_container;

  typedef boost::variant<Boolean,
                         Float,
                         Scene_time,
                         Int,
                         Vector2f,
                         Vector3f,
                         Rotation,
                         std::string,
                         Shared_container,
                         std::vector<Boolean>,
                         std::vector<Float>,
                         std::vector<Int>,
                         std::vector<Vector2f>,
                         std::vector<Vector3f>,
                         std::vector<Rotation>,
                         std::vector<std::string>,
                         std::vector<Shared_container> > Variant_field;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Script(Boolean proto = false);

  /*! Destructor. */
  virtual ~Script();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Script* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

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

  /// \name field handlers
  //@{
  /*! Obtain a handle to a dynamically generated field.
   * \param field_info (in) the field information record.
   * \return a pointer to the field.
   */
  template <typename T>
  T* field_handle(const Field_info* field_info)
  {
    Uint id = field_info->get_id();
    std::list<Variant_field>::iterator it = m_fields.begin();
    std::advance(it, id - LAST);
    T& field = boost::get<T>(*it);
    return &field;
  }
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

  /*! Add a field information record to the script node.
   * \param type (in) the type of the field.
   * \param name (in) the name of the field.
   * \param value (in) the initial value of the field.
   */
  void add_field_info(Field_rule rule, Field_type type,
                      const std::string& name, const std::string& value);

  // virtual void add_field(const std::string& name, const std::string& type,
  //                        Container* value, Node* field);

  /*! Execute the script function according to the event. */
  virtual void execute(Field_info* field_info);

protected:
  /*! The callback to invoke when an input field is used by the engine.
   * \param name (in) the field name.
   */
  static void getter(v8::Local<v8::String> name,
                     const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! The callback to invoke when an output field is set by the engine.
   * \param name (in) the field name.
   */
  static void setter(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                     const v8::PropertyCallbackInfo<void>& info);

  static void
  named_property_setter(v8::Local<v8::String> property,
                        v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<v8::Value>& info);

  static void
  indexed_property_setter(uint32_t index,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<v8::Value>& info);

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Additional fields. */
  std::list<Variant_field> m_fields;

  /*! The script protocol. */
  Protocol m_protocol;

  /*! The scene graph */
  Scene_graph* m_scene_graph;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype */
inline Script* Script::prototype() { return new Script(true); }

/*! \brief clones */
inline Container* Script::clone() { return new Script(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Script::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
