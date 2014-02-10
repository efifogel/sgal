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
#include "SGAL/Group.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Cull_context;
class Draw_action;
class Element;
class Container_proto;
class Isect_action;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Switch : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    WHICH_CHOICE,
    LAST
  };

  /*! Constructor */
  Switch(Boolean proto = false);

  /*! Desstructor */
  virtual ~Switch();

  /*! Construct the prototype */
  static Switch* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Uint* which_choice_handle(const Field_info*) { return &m_which_choice; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the node while traversing the scene graph.
   */
  virtual Action::Trav_directive draw(Draw_action* action);

  virtual void cull(Cull_context& cull_context);

  virtual void isect(Isect_action* isect_action);

  /*! Calculate the sphere bound of the group.
   */
  virtual bool clean_sphere_bound();

  /*! Write this container.
   * \param formatter (in) the formatter to use; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Set the index of the child to traverse.
   */
  void set_which_choice(Uint which_choice);

  /*! Obtain the index of the child to traverse.
   */
  Uint get_which_choice() const;

  /*! Return the node to traverse.
   */
  virtual Shared_node get_choice();

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that represents the container. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Specifies the index of the child to traverse. */
  Uint m_which_choice;

  // Default values
  static const Uint s_def_which_choice;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief construct the prototype.
inline Switch* Switch::prototype() { return new Switch(true); }

//! \brief clones.
inline Switch::Container* Switch::clone() { return new Switch(); }

//! \brief sets the index of the child to traverse.
inline void Switch::set_which_choice(Uint index)
{
  m_which_choice = index;
  m_dirty_sphere_bound = true;
}

//! \brief  obtains the index of the child to traverse.
inline Uint Switch::get_which_choice() const { return m_which_choice; }

//! \brief  obtains the tag (type) of the container.
inline const std::string& Switch::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
