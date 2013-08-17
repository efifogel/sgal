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

/**
 * @class: ERemoteKeys
 *
 * Purpose: keys actions.
 *
 * Description:  This is used to key actions on remote controller.
 *
 *  Inherits from Container
 */

#ifndef REMOTEKEYS_H
#define REMOTEKEYS_H

#include <string>

#include "SGAL/basic.hpp"
#include "Node.h"
#include "KeyAction.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SCENE_GRAPH_CLASSDEF Remote_keys : public Node {
public:
  typedef std::map<std::string, Key_action *> Key_action_map;

  /*! Constructor */
  Remote_keys(Boolean proto = false);

  /*! Destructor */
  virtual ~Remote_keys();

  /*! Construct the prototype */
  static Remote_keys* prototype() {  return new Remote_keys(true); }

  /*! Clone */
  virtual Container* clone() { return new Remote_keys(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  virtual Trav_directive Draw(Draw_action* draw_action) { return Trav_cont; }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  Key_action* get_keyAction(std::string str);

private:
  static Container_proto* s_prototype;

  Key_action_map m_keyMap;

private:
  Key_action* CreateKeyAction(std::string str);

};

SGAL_END_NAMESPACE

#endif
