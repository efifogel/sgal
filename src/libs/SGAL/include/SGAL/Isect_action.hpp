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

#ifndef SGAL_ISECT_ACTION_HPP
#define SGAL_ISECT_ACTION_HPP

/*! \file
 * Isect_action is a class used while traversing the scene graph for selection.
 * It holds information about the current group colour used to identify the 
 * object.
 *
 * To implement the selection, we use the back frame buffer to store 
 * information on the objects rendered. Each group that has a touch
 * sensor gets a unique id in the creation time. If the group does not
 * have a touch sensor its id is 0. When traversing the tree, instead of 
 * rendering the objects with the material, we use the id as colour and 
 * write it to the frame buffer. When the traverse is complete, we read 
 * buffer and check the id of the object stored in the given pixel.
 * While rendering, we disable all light/texture/blending operations.
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Color_map;
class Context;
class Node;

class SGAL_CLASSDEF Isect_action : public Action {
public:
  /*! Constructor */
  Isect_action();

  /*! Destructor */
  virtual ~Isect_action();

  virtual Trav_directive apply(Node* node);
  virtual void begin(Node* /* node */){};
  virtual void end(Node* /* node */){};
  virtual void set_context(Context* context);

  /*! Obtain the current color id */
  Uint get_id() const { return m_current_id; }

  /*! Set the current color id */
  void set_id(Uint id) { m_current_id = id; }

  int get_index(unsigned int* rgb) const;
  void get_color(unsigned int index, unsigned int* rgb) const;

protected:
  /*! The current group's colour */
  unsigned int m_current_id;

  /*! A color map used to map an index (0-n) to a unique color. This is used
   * in picking.
   */
  Color_map * m_color_map;
};

SGAL_END_NAMESPACE

#endif
