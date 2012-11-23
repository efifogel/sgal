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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * This node is the implementation of the VRML script node
 */

#ifndef SGAL_SCRIPT_HPP
#define SGAL_SCRIPT_HPP

#include "SGAL/basic.hpp"
#include "Node.h"
#include "JSWClientObject.h"
#include "SAINodeServices.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class Script : public Node {
public:
  /*! Constructor */
  Script(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Script();

  /*! Construct the prototype */
  static Script * prototype() { return new Script(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Script(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  virtual Attribute_list get_attributes();

  virtual Trav_directive Draw(Draw_action * draw_action) {return Trav_cont;}; 

  virtual void add_field_def(const std::string & name, const std::string & type,
                             const std::string & value, Node * field);

  virtual void add_field_def(const std::string & name, const std::string & type,
                             Container * value, Node * field);

  // Execution function - executes the suitable script function according to
  // the event
  virtual void execute(Field_info * field_info);

  int get_child_count();
  Node * get_child(int i);

private:
  /*! The node prototype */
  Container_proto * s_prototype;

  std::string m_url;

  NodeList m_child_list;

  Int m_field_infoIDCount;

  JSWObjectInt * m_JSWObject;

  Boolean m_engineInitialized;

  ESAI * m_SAI;

  SAI_node_services * m_SAINode;

public:
  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto * get_prototype(); 
};

SGAL_END_NAMESPACE

#endif
