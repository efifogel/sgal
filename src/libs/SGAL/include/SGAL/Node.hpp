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
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

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
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Sphere_bound.hpp"

SGAL_BEGIN_NAMESPACE

class Cull_context;
class Draw_action;
class Isect_action;
class Element;

class SGAL_CLASSDEF Node : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    SPHERE_BOUND,
    LAST
  };

  /*! Constructor */
  Node(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Node() {}

  enum Bound_mode {
    STATIC,
    DYNAMIC
  };

  /*! Implement this to draw the object. */
  virtual Action::Trav_directive draw(Draw_action* /* draw_action */)
  { return Action::TRAV_CONT; }
    
  /*! Implement this for culling drawable nodes. */
  virtual void cull(Cull_context& /* cull_context */) {}

  /*! Implement this to calculate intersaction with the object */
  virtual void isect(Isect_action* /* isect_action */) {}
  
  /*! Implement this to calculate the bounding sphere. */
  virtual Boolean calculate_sphere_bound() { return false; }

  /*! Obtain the sphere bound */
  const Sphere_bound& get_sphere_bound();

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();
  
  /*! */
  void set_sphere_bound_modified(Field_info* field_info = NULL);
  
protected:
  /*! The sphere bound of the node. */
  Sphere_bound m_sphere_bound;

  /*! A flag that indicatres whether the bounding sphere is valid */
  Boolean m_sphere_bound_dirty;

  /*! Indicates whether the bounding sphere is locked */
  Boolean m_sphere_bound_locked;
};

SGAL_END_NAMESPACE

#endif
