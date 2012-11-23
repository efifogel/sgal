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

#ifndef SGAL_SWITCH_HPP
#define SGAL_SWITCH_HPP

/*! \file
 * A node in the scene graph that can choose for traversing one of child
 * objects.
 * 
 * This is a switch node in the scene graph. It inherits 
 * from Group and therefore can have multiple child objects.
 * It contains whichChoice field that choose which of child nodes will be
 * traversed.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Cull_context;
class Draw_action;
class Element;
class Container_proto;
class Isect_action;

class SGAL_CLASSDEF Switch : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    WHICH_CHOICE,
    LAST
  };

  /*! Constructor */
  Switch(Boolean proto = SGAL_FALSE);

  /*! Desstructor */
  virtual ~Switch();
  
  /*! Construct the prototype */
  static Switch * prototype() { return new Switch(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Switch(); }

  /*! Set the index of the child to traverse */
  void set_which_choice(Uint which_choice);

  /*! Obtain the index of the child to traverse */
  Uint  get_which_choice() const;

  /*! Return the node to traverse */
  virtual Node * get_choice();
  
  /*! Draw the node while traversing the scene graph */
  virtual Action::Trav_directive draw(Draw_action * action);

  virtual void cull(Cull_context & cull_context);

  virtual void isect(Isect_action * isect_action);

  /*! Calculate the sphere bound of the group */
  virtual bool calculate_sphere_bound();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto * get_prototype(); 

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that represents the container */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! Specifies the index of the child to traverse */
  Uint m_which_choice;
};

/*! Set the index of the child to traverse */
inline void Switch::set_which_choice(Uint index) { m_which_choice = index; }
  
/*! Obtain the index of the child to traverse */
inline Uint Switch::get_which_choice() const { return m_which_choice; }

SGAL_END_NAMESPACE

#endif
