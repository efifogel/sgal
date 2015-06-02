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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ISECT_ACTION_HPP
#define SGAL_ISECT_ACTION_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Contexted_action.hpp"
#include "SGAL/Color_map.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Node;

/*! \class Isect_action Isect_action.hpp
 * Isect_action is a class used while traversing the scene graph for selection.
 * It holds information about the current group color used to identify the
 * object.
 *
 * To implement the selection, we use the back frame buffer to store
 * information on the objects rendered. Each group that may perform selections
 * (e.g., has a touch sensor) obtains a continuous sequence of unique ids, one
 * id for each child, starting at base-id. If the group does need to apply
 * selections, its base-id is 0. When traversing the tree, instead of
 * rendering the objects with the material, we use the id as color and
 * render it to the frame buffer. When the traversal is complete, we read the
 * buffer and check the ids of the objects stored in pixels. While rendering,
 * lighting, texturing, blending operations, etc. must be disabled.
 */
class SGAL_SGAL_DECL Isect_action : public Contexted_action {
public:
  /*! Constructor */
  Isect_action();

  /*! Destructor */
  virtual ~Isect_action();

  /*! Apply the draw action on a given node. */
  virtual Trav_directive apply(Shared_node node);

  //virtual void begin(Node* /* node */) {};

  //virtual void end(Node* /* node */) {};

  /*! Set the context in the action and initializes the colormap used
   * for mapping unique ids to unique colors.
   */
  virtual void set_context(Context* context);

  /*! Obtain the current color id. */
  Uint get_id() const;

  /*! Set the current color id. */
  void set_id(Uint id);

  /*! Convert the pixel color into an index. */
  Uint get_index(const Uchar* color) const;

  /*! Convert the index into a color. */
  void get_color(Uint index, Uchar* color) const;

protected:
  /*! The current group color */
  Uint m_current_id;

  /*! A color map used to map unique colors to unique ids. This is used
   * in selection and picking.
   */
  Color_map m_color_map;
};

//! \brief obtains the current color id.
inline Uint Isect_action::get_id() const { return m_current_id; }

//! \brief sets the current color id.
inline void Isect_action::set_id(Uint id) { m_current_id = id; }

SGAL_END_NAMESPACE

#endif
