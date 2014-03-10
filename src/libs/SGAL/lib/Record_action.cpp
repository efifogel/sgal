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
#include "SGAL/Record_action.h"
#include "SGAL/Action.h"
#include "SGAL/Shape.h"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Record_action::Record_action() : Isect_action() {}

/*! Destructor */
Record_action::~Record_action() {}

/*!
 */
Action::Trav_directive Record_action::pre_visit(Node* node)
{
  Shape * shape = dynamic_cast<Shape *>(node);
  if (shape && !shape->is_background()) m_current_id = 1;
  return Action::TRAV_CONT;
}

/*!
 */
Action::Trav_directive Record_action::post_visit(Node* node)
{
  Shape * shape = dynamic_cast<Shape *>(node);
  if (shape) m_current_id = 0;
  return Action::TRAV_CONT;
}

SGAL_END_NAMESPACE
