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
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

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
