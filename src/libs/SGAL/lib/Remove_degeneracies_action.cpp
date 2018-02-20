// Copyright (c) 2016 Israel.
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
#include "SGAL/Remove_degeneracies_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Remove_degeneracies_action::Remove_degeneracies_action() {}

//! \brief destructs.
Remove_degeneracies_action::~Remove_degeneracies_action() {}

//! \brief applies the action to a given node.
Action::Trav_directive Remove_degeneracies_action::apply(Shared_node node)
{
  typedef boost::shared_ptr<Container>          Shared_container;

  if (!node) return TRAV_CONT;

  auto group = boost::dynamic_pointer_cast<Group>(node);
  if (!group) return TRAV_CONT;

  group->remove_children_if
    ([](Shared_container node)
     {
       auto shape = boost::dynamic_pointer_cast<Shape>(node);
       if (!shape) return false;
       auto geometry = shape->get_geometry();
       auto ifs = boost::dynamic_pointer_cast<Indexed_face_set>(geometry);
       if (!ifs) return false;
       auto volume = ifs->volume();
       return (volume == 0);
     });

  node->traverse(this);
  return TRAV_CONT;
}

SGAL_END_NAMESPACE
