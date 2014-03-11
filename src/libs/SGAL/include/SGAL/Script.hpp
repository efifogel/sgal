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
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Field_info.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

/*! \class Script Script.hpp
 * Script is a node used to program behaviour in a scene. Script nodes typically
 * 1. signify a change or user action;
 * 2. receive events from other nodes;
 * 3. contain a program module that performs some computation;
 * 4. effect change somewhere else in the scene by sending events.
 * Each Script node has associated programming language code, referenced by the
 * url field, that is executed to carry out the Script node's function.
 */
class Script : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    URL,
    DIRECT_OUTPUT,
    MUST_EVALUATE,
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
  std::string* url_handle(const Field_info*) { return &m_url; }
  Boolean* direct_output_handle(const Field_info*) { return &m_direct_output; }
  Boolean* must_evaluate_handle(const Field_info*) { return &m_must_evaluate; }

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

  /*! Apply the script. */
  virtual Action::Trav_directive Draw(Draw_action* /* draw_action */)
  { return Action::TRAV_CONT; }

  /*! Add a field information record to the script node.
   * \param type (in) the type of the field.
   * \param name (in) the name of the field.
   * \param value (in) the initial value of the field.
   */
  void add_field_info(Field_rule rule, Field_type type,
                      const std::string& name, const std::string& value);

  // virtual void add_field(const std::string& name, const std::string& type,
  //                        Container* value, Node* field);

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Set the m_direct_output Boolean flag. */
  void set_direct_output();

  /*! Set the m_must_evaluate Boolean flag. */
  void set_must_evaluate();

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

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The script. */
  std::string m_url;

  /*! Specifies how to evaluate the script.
   * If TRUE, the script may also send events directly to any node to which it
   * has access, and may dynamically establish or break routes. If
   * m_direct_output is FALSE (the default), the script may only affect the rest
   * of the world via events sent through its eventOuts.
   */
  Boolean m_direct_output;

  /*! If the mustEvaluate field is FALSE, the browser may delay sending input
   * events to the script until its outputs are needed by the browser. If the
   * mustEvaluate field is TRUE, the browser shall send input events to the
   * script as soon as possible, regardless of whether the outputs are
   * needed. The mustEvaluate field shall be set to TRUE only if the Script node
   * has effects that are not known to the browser (such as sending information
   * across the network). Otherwise, poor performance may result.
   */
  Boolean m_must_evaluate;

  /*! Additional fields. */
  std::list<Variant_field> m_fields;

  /*! The script protocol. */
  Protocol m_protocol;

  /*! The scene graph */
  Scene_graph* m_scene_graph;

  // JSWObjectInt* m_JSWObject;
  // Boolean m_engineInitialized;
  // ESAI* m_SAI;
  // SAI_node_services * m_SAINode;
};

/*! \brief constructs the prototype */
inline Script* Script::prototype() { return new Script(true); }

/*! \brief clones */
inline Container* Script::clone() { return new Script(); }

/*! \brief sets the URL. */
inline void Script::set_url(const std::string& url) { m_url = url; }

/*! \brief obtains the URL. */
inline const std::string Script::get_url() const { return m_url; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Script::get_tag() const { return s_tag; }

/*! \brief sets the m_direct_output Boolean flag. */
inline void Script::set_direct_output() { m_direct_output = true; }

/*! \brief sets the m_must_evaluate Boolean flag. */
inline void Script::set_must_evaluate() { m_must_evaluate = true; }

SGAL_END_NAMESPACE

#endif
