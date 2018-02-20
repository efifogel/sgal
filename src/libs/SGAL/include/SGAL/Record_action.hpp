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

/*!
 * Isect_action is a class used while traversing the scene graph for
 * frame recording.
 *
 * The frame recording is based on whether any given shape is set to be
 * a background or foreground object. If the object is a background object
 * it sets the color value to 0, otherwise it sets it to something other
 * than 0.
 *
 * Inherits from Isect_action
 */

#ifndef SGAL_RECORD_ACTION_HPP
#define SGAL_RECORD_ACTION_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Record_action : public Isect_action {
public:
  /*! Constructor */
  Record_action();

  /*! Destructor */
  virtual ~Record_action();

  virtual Action::Trav_directive pre_visit(Node* node);
  virtual Action::Trav_directive post_visit(Node* node);
};

SGAL_END_NAMESPACE

#endif
