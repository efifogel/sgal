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
#include "SGAL/Node.h"

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

  /*! Set the attributes of the object extracted from the input file.
   * \param elem
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  virtual Trav_directive Draw(Draw_action* draw_action) { return Trav_cont; }

  virtual void add_field_def(const std::string& name, const std::string& type,
                             const std::string& value, Node* field);

  virtual void add_field_def(const std::string& name, const std::string& type,
                             Container* value, Node * field);

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Execute the script function according to the event. */
  virtual void execute(Field_info* field_info);

  // int get_child_count();

  // Node* get_child(int i);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  std::string m_url;

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
inline Container* Script::clones() { return new Script(); }

/*! \brief sets the URL. */
inline void Script::set_url(const std::string& url) { m_image.set_url(url); }

/*! \brief obtains the URL. */
inline const std::string Script::get_url() const { return m_image.get_url(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Script::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
