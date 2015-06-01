// Copyright (c) 2015 Israel.
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
#include "SGAL/Volume_action.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief applies the draw action to a given node.
Action::Trav_directive Volume_action::apply(Node* node)
{
  if (!node) return TRAV_CONT;

  // auto* shape = dynamic_cast<Shape*>(node);
  // if (shape) {
  //   const auto geometry = shape->get_geometry();
  //   const auto ifs = dynamic_cast<Indexed_face_set*>(geometry);
  //   if (ifs) add(ifs->volume());
  // }

  return TRAV_CONT;
}

SGAL_END_NAMESPACE
