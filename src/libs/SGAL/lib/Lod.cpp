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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "Lod.h"
#include "Field_infos.h"
#include "Draw_action.h"
#include "Isect_action.h"
#include "Cull_context.h"
#include "Element.h"
#include "Container_proto.h"

SGAL_BEGIN_NAMESPACE

const std::string Lod::s_tag = "LOD";
Container_proto* Lod::s_prototype(NULL);

REGISTER_TO_FACTORY(Lod, "Lod");

/* Constructor */
Lod::Lod(Boolean proto) :  Switch(proto), m_choosen_node(0) {}

/* Destructor */
Lod::~Lod() {}

/*! \brief */
Node*  Lod::get_choosen_node() const
{
  Node_list::const_iterator it;
  for (it = m_child_list.begin(); it != m_child_list.end(); ++it) {
    if (*it == m_choosenNode) return *it;
  }
  return NULL;
}

/*! \brief culls the node. */
void Lod::Cull(Cull_context& cull_context)
{
  if (!IsVisible())  return;

  Vector3f camPos = cull_context.get_camera()->get_position();
  Matrix4f wtm;
  cull_context.get_currentMatrix(wtm);
  Vector3f pos;
  pos[0] = wtm[3][0] + m_center[0];
  pos[1] = wtm[3][1] + m_center[1];
  pos[2] = wtm[3][2] + m_center[2];

  pos.Sub(camPos);
  // Find distance from LOD center to camera source.
  float dist = pos.Length();

  int lod = 0;
  // Find node from range array.
  Uint i;
  int nodeIndex = -1;
  for (i = 0; i < m_range.size(); ++i) {
    if (dist < m_range[i]) {
      nodeIndex = i;
      lod = m_range.size()-nodeIndex;
      break;
    }
  }
  if (nodeIndex >= 0) {
    i = 0;
    if (!m_child_list.empty()) {
      m_choosenNode = m_child_list.front();
    }
    NodeList::const_iterator it;
    for (it = m_child_list.begin(); it != m_child_list.end(); ++it, ++i) {
      if (i == nodeIndex) {
        m_choosenNode = *it;
        break;
      }
    }
  } else {
    if (!m_child_list.empty()) {
      m_choosenNode = m_child_list.back();
    }
  }

  if (m_choosenNode) {
    int prevLod = cull_context.get_currentLod();
    cull_context.SetCurrentLod(lod);
    m_choosenNode->Cull(cull_context);
    cull_context.SetCurrentLod(prevLod);
  }
}

/*! \brief draw the node. */
Trav_directive Lod::Draw(Draw_action* draw_action)
{
  if (!IsVisible())  return Trav_cont;
  Node* node = get_choosenNode();
  if (node) draw_action->Apply(node);
  return Trav_cont;
}

/*! \brief traverses the choosen node for selections. */
void Lod::Isect(Isect_action* isect_action)
{
  if (!IsVisible())  return;

  isect_action->PreVisit(this);
  Node *node = get_choosenNode();
  if (node) isect_action->Apply(node);
  isect_action->PostVisit(this);
}

/*! \brief initializes the container prototype. */
void Lod::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // // Add the object fields to the prototype
  // s_prototype->add_field_info(new ESFInt(WHICHCHOOSE,
  //                                        m_whichChooseFieldName,
  //                                        which_choose_func,
  //                       (Execution_func_type)&Node::SetSphereBoundModified));
}

/*! \brief */
void Lod::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Lod::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this container. */
void Lod::set_attributes(Element * elem)
{
  Group::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "range") {
      value.getFloatArray(" ", m_range);
      elem->mark_delete(ai);
    }
    /*
    if (name == m_whichChooseFieldName)
    {
      m_whichChoose = value.toInteger();
    }
    */
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

Attribute_list Lod::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  Vector3f vec;
  Rotation rot;

  attribs = Group::get_attributes();

  if (!m_range.empty()) {
    String range;
    for (Uint i = 0; i < m_range.size(); i++) {
      range += String::fromFloat(m_range[i]) + " ";
    }
    range.trimRight();
    attrib.first = "range";
    attrib.second = range;
    attribs.push_back(attrib);
  }

  /*
  if (m_whichChoose >= 0)
  {
    attrib.first = m_whichChooseFieldName;
    attrib.second = String::fromInteger(m_whichChoose);
    attribs.push_back(attrib);
  }
  */

  return attribs;
}

SGAL_END_NAMESPACE
