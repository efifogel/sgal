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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FIELD_DEF
#define SGAL_FIELD_DEF

/*! \file
 * Definition of a VRML field - used in Scripts and Protos
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Script;

class Field_def : public Node {
public:
  // Constructor
  Field_def(Boolean proto = false);

  // Destructor
  virtual ~Field_def(){};

  /* Construct the prototype */
  static Field_def* prototype() { return new Field_def(true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  // protoype handling
  virtual void init_prototype();
  virtual void delete_prototype()
  {
    delete s_prototype;
    s_prototype = NULL;
  }

  virtual Container_proto* get_prototype()
  {
    if (s_prototype == NULL) Field_def::init_prototype();
    return s_prototype;
  }

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  virtual Attribute_list get_attributes();

  virtual Action::Trav_directive Draw(Draw_action* draw_action)
  { return Action::TRAV_CONT; }

  // Set value for fields of type SFNode
  void set_value(Container* value);

  Container* get_container_value() const;

private:
  static Container_proto* s_prototype;

  std::string m_name;
  std::string m_type;
  std::string m_value;

  Container* m_containerValue;
  Script* m_script;
};

inline Container* Field_def::get_containerValue() const
{ return m_containerValue; }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Field_def::create()
{ return new Field_def(); }

SGAL_END_NAMESPACE

#endif
