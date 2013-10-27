// Copyright (c) 2013 Israel.
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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * This node is the implementation of the VRML script node
 */

#ifndef SGAL_SCRIPT_HPP
#define SGAL_SCRIPT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"

// #include "JSWClientObject.h"
// #include "SAINodeServices.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Field_info;

class Script : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    LAST
  };

  /*! Constructor */
  Script(Boolean proto = false);

  /*! Destructor */
  virtual ~Script();

  /*! Construct the prototype */
  static Script* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Apply the script. */
  virtual Action::Trav_directive Draw(Draw_action* /* draw_action */)
  { return Action::TRAV_CONT; }

  // virtual void add_field_def(const std::string& name, const std::string& type,
  //                            const std::string& value, Node* field);

  // virtual void add_field_def(const std::string& name, const std::string& type,
  //                            Container* value, Node* field);

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

  // int get_child_count();

  // Node* get_child(int i);

protected:
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

  // NodeList m_child_list;
  // Int m_field_infoIDCount;
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
