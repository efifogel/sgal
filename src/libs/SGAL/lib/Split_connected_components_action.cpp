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
#include "SGAL/Split_connected_components_action.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Connected_components_splitter.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs default.
Split_connected_components_action::Split_connected_components_action()
{
  m_splitter.reset(new Connected_components_splitter);
  SGAL_assertion(m_splitter);
}

//! \brief destructs.
Split_connected_components_action::~Split_connected_components_action()
{ m_splitter.reset(); }

//! \brief applies the action to a given node.
Action::Trav_directive Split_connected_components_action::apply(Shared_node node)
{
  if (!node) return TRAV_CONT;
  auto shape = boost::dynamic_pointer_cast<Shape>(node);
  if (shape) {
    m_shapes.push_back(shape);
    return TRAV_CONT;
  }

  auto group = boost::dynamic_pointer_cast<Group>(node);
  if (!group) return TRAV_CONT;

  node->traverse(this);

  for (auto shape : m_shapes) {
    const auto geometry = shape->get_geometry();
    const auto ifs = boost::dynamic_pointer_cast<Indexed_face_set>(geometry);
    if (!ifs) break;

    m_splitter->set_operand(ifs);
    m_splitter->execute();
    const auto& components = m_splitter->get_result();
    if (components.size() <= 1) break;
    group->remove_child(shape);
    for (auto geometry : components) {
      Shared_shape new_shape(new Shape);
      new_shape->set_appearance(shape->get_appearance());
      new_shape->set_geometry(geometry);
      group->add_child(new_shape);
    }
  }
  m_shapes.clear();

  return TRAV_CONT;
}

SGAL_END_NAMESPACE
