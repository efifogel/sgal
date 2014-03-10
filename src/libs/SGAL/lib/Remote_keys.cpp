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
#include "SGAL/RemoteKeys.h"
#include "SGAL/Element.h"
#include "SGAL/Container_proto.h"

SGAL_BEGIN_NAMESPACE

const std::string Remote_keys::s_tag = "RemoteKeys";
Container_proto * Remote_keys::s_prototype(NULL);

/*! Constructor */
Remote_keys::Remote_keys(Boolean proto) : Node(proto) {}

/*! Destructor */
Remote_keys::~Remote_keys() {}

/*! Initializes the node prototype */
void Remote_keys::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();
}

/*! */
void Remote_keys::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto * Remote_keys::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Remote_keys::set_attributes(Element * elem)
{
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "btn1") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["1"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn2") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["2"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn3") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["3"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn4") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["4"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn5") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["5"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn6") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["6"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn7") {
      elem->mark_delete(ai);
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["7"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn8") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["8"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn9") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["9"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btnStar") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["*"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btn0") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["0"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btnHash") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["#"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    } else if (name == "btnM") {
      EKeyAction *pTemp = CreateKeyAction(value);
      if (pTemp)
        m_keyMap["M"] = pTemp;
      else
        MessageBox(NULL,"Error::Invalid attribue",
                   "EnbayaPlyer=>Remote_keys",MB_OK);
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

Attribute_list Remote_keys::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
//  char buf[32];

  attribs = Node::get_attributes();


  return attribs;
}

void Remote_keys::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Node::AddToScene(sg, parent);
}

EKeyAction *Remote_keys::CreateKeyAction(String str)
{
  EKeyAction *res = NULL;
  StringVector vals = std::stringUtils::SplitStringByToken(str);

  if (vals.size() == 0)
    return res;

  if (vals[0] == "translate")
  {
    if (vals.size() != 4)
      return res;
    res = new EKATranslate(atoi(vals[1].c_str()),atoi(vals[2].c_str()),atoi(vals[3].c_str()));
  } else if (vals[0] == "rotate")
  {
    if (vals.size() != 5)
      return res;
    res = new EKARotate(atoi(vals[1].c_str()),atoi(vals[2].c_str()),atoi(vals[3].c_str()),atoi(vals[4].c_str()));
  } else if (vals[0] == "setField")
  {
    if (vals.size() != 4)
      return res;
    res = new EKAField(vals[1],vals[2],vals[3]);
  } else if (vals[0] == "reset")
  {
    res = new EKAReset();
  } else if (vals[0] == "switchFields")
  {
    if (vals.size() != 7)
      return res;
    res = new EKASwitchFields(vals[1],vals[2],vals[3],vals[4],vals[5],vals[6]);
  }
  return res;
}

EKeyAction *Remote_keys::get_keyAction(String str)
{
  EKeyAction *res = NULL;
  EKeyActionMap::iterator keyIter = m_keyMap.find(str);

  if (keyIter != m_keyMap.end())
  {
    res = (*keyIter).second;
  }

  return res;
}

SGAL_END_NAMESPACE
