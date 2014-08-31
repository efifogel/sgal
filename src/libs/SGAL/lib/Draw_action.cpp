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
#include "SGAL/Draw_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructor.
Draw_action::Draw_action(Configuration* config) :
  Action(),
  m_pass_no(0),
  m_second_pass_required(false),
  m_current_lod(-1),    // Negative lod force choosing max level.
  m_snap_from_front(false),
  m_clear(Gfx::COLOR_CLEAR | Gfx::DEPTH_CLEAR),
  m_snap(true),
  m_apply_camera(true),
  m_configuration(config)
{}

//! \brief destructor.
Draw_action::~Draw_action() {}

//! \brief applies the draw action to a given node.
Action::Trav_directive Draw_action::apply(Node* node)
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

//! \brief determines whether a second pass is required.
Boolean Draw_action::is_second_pass_required()
{ return m_second_pass_required; }

//! \brief set the flag to indicate whether a second pass is required.
void Draw_action::set_second_pass_required(Boolean flag)
{ m_second_pass_required = flag; }

//! \brief increments the pass number.
void Draw_action::inc_pass_no()
{
  ++m_pass_no;
  m_current_lod = -1;
}

//! \brief obtains the number of the current pass being rendered.
Int Draw_action::get_pass_no() { return m_pass_no; }

//! brief resets the pass number to 0.
void Draw_action::reset_pass_no()
{
  m_pass_no = 0;
  m_second_pass_required = false;

}

//! Determine whether the current pass is the first one.
Boolean Draw_action::is_first_pass()
{ return (m_pass_no == 0); }

SGAL_END_NAMESPACE
