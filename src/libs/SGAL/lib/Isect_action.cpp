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

#include "SGAL/basic.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Color_map.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Isect_action::Isect_action() : m_current_id(0) {}

//! \brief destructor.
Isect_action::~Isect_action() {}

/*! \brief applies the draw action for selection on a given node.
 * \param node (in) a pointer to the node to draw in selection mode.
 */
Action::Trav_directive Isect_action::apply(Node* node)
{
  node->isect(this);
  return Action::TRAV_CONT;
}

/*! \brief sets the context in the action and initializes the colormap used
 * for mapping unique ids to unique colors.
 */
void Isect_action::set_context(Context* context)
{
  Action::set_context(context);
  m_color_map.init(context);
}

//! \brief converts the pixel color into an index.
Uint Isect_action::get_index(const Uchar* pixel) const
{ return m_color_map.get_index(pixel); }

//! \brief converts the index into a color.
void Isect_action::get_color(Uint index, Uchar* rgb) const
{ m_color_map.get_color(index, rgb); }

SGAL_END_NAMESPACE
