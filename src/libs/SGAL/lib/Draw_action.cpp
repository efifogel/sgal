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
// $Revision: 5848 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Draw_action::Draw_action(Configuration * config) :
  Isect_action(),
  m_pass_no(0),
  m_second_pass_required(false),
  m_current_lod(-1),    // Negative lod force choosing max level.
  m_snap_from_front(false),
  m_clear(Gfx::COLOR_CLEAR | Gfx::DEPTH_CLEAR),
  m_snap(true),
  m_apply_camera(true),
  m_configuration(config)
{
}

/*! Destructor */
Draw_action::~Draw_action() {}

/*! Apply the draw action on a given node. For now this means calling
 * the draw method on the Node.
 * @param node a pointer to the node to draw
 */
Action::Trav_directive Draw_action::apply(Node * node)
{  
  if (node) {
    // Ignore some nodes:
    if ((dynamic_cast<Transform*>(node) == 0) &&
        (dynamic_cast<Light*>(node) == 0))
    {
      node->draw(this);
    }
  }
  return Action::TRAV_CONT; 
}

/*! Returns true if a second pass is required to render the transparent
 * objects.
 * @return true if a 2nd pass is required.
 */
Boolean Draw_action::is_second_pass_required() 
{ 
  return m_second_pass_required; 
}

/*! Set the flag to indicate whether a second pass is required. This flag
 * should be set when a transparent object is found.
 */
void Draw_action::set_second_pass_required(Boolean flag) 
{ 
  m_second_pass_required = flag; 
}

/*! Increment the pass number.
 */
void Draw_action::inc_pass_no() 
{ 
  m_pass_no++;
  m_current_lod = -1;
}

/*! Returns the current pass being rendered, assuming 0 to be the first one.
 * @return the pass number.
 */
Int Draw_action::get_pass_no() 
{ 
  return m_pass_no; 
}

/*! Resets the pass number to 0.
 */
void Draw_action::reset_pass_no() 
{ 
  m_pass_no = 0; 
  m_second_pass_required = false; 

}

/*! Returns true if the current pass is the first one.
 * @return true if it is the first pass.
 */
Boolean Draw_action::is_first_pass()
{
  return (m_pass_no == 0);
}

SGAL_END_NAMESPACE
