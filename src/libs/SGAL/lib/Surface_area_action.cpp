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
#include "SGAL/Surface_area_action.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief applies the surface-area action to a given node.
Action::Trav_directive Surface_area_action::apply(Shared_node node)
{
  if (!node) return TRAV_CONT;

  auto shape = boost::dynamic_pointer_cast<Shape>(node);
  if (!shape) return node->traverse(this);

  const auto geometry = shape->get_geometry();
  const auto ifs = boost::dynamic_pointer_cast<Indexed_face_set>(geometry);
  if (!ifs) return TRAV_CONT;

  add(ifs->surface_area());

  return TRAV_CONT;
}

SGAL_END_NAMESPACE
