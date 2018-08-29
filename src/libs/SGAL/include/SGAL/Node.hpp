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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_NODE_HPP
#define SGAL_NODE_HPP

/*! \file
 * \brief A base class for nodes in the scene graph.
 *
 * A node is a base type for all objects that can be in the scene graph. This
 * type supports bounding sphere. For nodes in which the bounding sphere is
 * effective, the CalculateBoundingSphere method has to be implemented.
 * Otherwise the bounding sphere will be initialized to an invalid value
 * and will be ignored.
 *
 * Inherits from Container.
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Bounding_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Cull_context;
class Draw_action;
class Isect_action;
class Element;
class Container_proto;

class SGAL_SGAL_DECL Node : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    BOUNDING_SPHERE,
    LAST
  };

  enum Bound_mode {
    STATIC,
    DYNAMIC
  };

  /*! Constructor */
  Node(Boolean proto = false);

  /*! Destructor */
  virtual ~Node() {}

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Bounding_sphere* bounding_sphere_handle(const Field_info*)
  { return &m_bounding_sphere; }
  //@}

  /*! Draw the object represented by the node.
   */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Traverse the node.
   */
  virtual Action::Trav_directive traverse(Action* action);

  /*! Cull the node if invisible and prepare for rendering. */
  virtual void cull(Cull_context& cull_context);

  /*! Draw the node for selection.
   * \param isect_action
   */
  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the node.
   */
  virtual void clean_bounding_sphere();

  /*! Determines whether the bounding sphere of the node must be recomputed.
   * \return true iff the bounding sphere must be recomputed.
   */
  virtual Boolean is_dirty_bounding_sphere();

  /*! Obtain the sphere bound. */
  const Bounding_sphere& get_bounding_sphere();

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Set the flag that indicates that the sphere bound should be cleaned.*/
  void bounding_sphere_changed(const Field_info* field_info = nullptr);

protected:
  //! The sphere bound of the node.
  Bounding_sphere m_bounding_sphere;

  //! A flag that indicatres whether the bounding sphere is valid.
  Boolean m_dirty_bounding_sphere;

private:
  //! The container prototype.
  static Container_proto* s_prototype;
};

//! \brief draws the object.
inline Action::Trav_directive Node::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

//! \brief traverses the object.
inline Action::Trav_directive Node::traverse(Action* /* action */)
{ return Action::TRAV_CONT; }

//! \brief culls the node if invisible and prepare for rendering.
inline void Node::cull(Cull_context& /* cull_context */) {}

//! \brief draws the node for selection.
inline void Node::isect(Isect_action* /* isect_action */) {}

//! \brief cleans the bounding sphere.
inline void Node::clean_bounding_sphere() {}

/*! \brief determines whether the bounding sphere of the node must be
 * recomputed.
 */
inline Boolean Node::is_dirty_bounding_sphere() { return m_dirty_bounding_sphere; }

SGAL_END_NAMESPACE

#endif
