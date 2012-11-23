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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Color_map.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Trace.hpp"

using namespace SGAL;

/*! Constructor */
Isect_action::Isect_action() :
  Action(), 
  m_current_id(0),
  m_color_map(0)
{}

/*! Destructor */
Isect_action::~Isect_action() 
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Isect_action ...");
  //! \todo DELETE_OBJECT(m_color_map);
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/*! Apply the draw action on a given node. For now this means calling
 * the draw method on the Node.
 * @param node a pointer to the node to draw
 */
Action::Trav_directive Isect_action::apply(Node * node)
{  
  node->isect(this); 
  return Action::TRAV_CONT;
}

/*! Set the context in the action. Also create the colormap used for assigning
 * a unique color for each id (0-n)
 */
void Isect_action::set_context(Context * context)
{
  Action::set_context(context);
  //! \todo DELETE_OBJECT (m_color_map);
  m_color_map = new Color_map(context);
}

/*!
 */
int Isect_action::get_index(Uint * rgb) const
{
  if (m_color_map) return m_color_map->get_index(rgb);
  return 0;
}

/*!
 */
void Isect_action::get_color(unsigned int index, Uint * rgb) const
{
  if (m_color_map) m_color_map->get_color(index, rgb);
}


