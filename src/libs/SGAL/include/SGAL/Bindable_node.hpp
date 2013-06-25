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

#ifndef SGAL_BINDABLE_NODE_HPP
#define SGAL_BINDABLE_NODE_HPP

/*! \file
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Bindable_stack;
class Field_info;

class SGAL_SGAL_DECL Bindable_node : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    SET_BIND,
    IS_BOUND,
    LAST
  };

  /*! Constructor */
  Bindable_node(Boolean proto = false);

  /*! Destructor */
  virtual ~Bindable_node();

  /*! Clone */
  virtual Container* clone() { return new Bindable_node(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();
    
  /*! Obtain the Navigation_info at the top os the stack */
  Bindable_node* top_stack();
  
  /*! Erase the Navigation_info */
  void erase_stack(Bindable_node* bindable);
  
  /*! Pop the Navigation_info at the top of the stack */
  void pop_stack();

  /*! Pushe a Navigation_info onto the stack */
  void push_stack(Bindable_node* bindable);

  /*! Insert a Navigation_info at the back of the stack */
  void insert_stack(Bindable_node* bindable);

  /*! Bind this node (used during initialization) */
  void bind();
  
  /*! Set the flag that indicates whether this node is bound */
  void set_is_bound(bool is_bound);
  
  /*! Bind or unbinds this node */
  void set_bind(Field_info* field_info);

protected:
  /*! Set this field to bind the node */
  bool m_set_bind;
  
  /*! Indicates whether the node is bound */
  bool m_is_bound;
  
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Obtain the bindable stack */
  virtual Bindable_stack* get_stack() { SGAL_assertion(0); return NULL; }

  /*! Enable the bindable node */
  virtual void enable() { }

  /*! Disable the bindable node */
  virtual void disable() {}
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

SGAL_END_NAMESPACE

#endif
