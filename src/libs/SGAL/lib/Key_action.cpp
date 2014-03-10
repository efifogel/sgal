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
#include "Key_action.h"

using namespace SGAL;

/*! Constructor */
Key_action::Key_action() {}

/*! Destructor */
Key_action::~Key_action() {}

KA_rotate::KA_rotate()
{
  m_vx = 0;
  m_vy = 0;
  m_vz = 0;
  m_angle = 0;
}

KA_rotate::KA_rotate(int vx,int vy, int vz, int angle)
{
  m_vx = vx;
  m_vy = vy;
  m_vz = vz;
  m_angle = angle;
}

KA_rotate::~KA_rotate()
{
}

void KA_rotate::Init(int vx,int vy, int vz, int angle)
{
  m_vx = vx;
  m_vy = vy;
  m_vz = vz;
  m_angle = angle;
}

//////////////////////////////////////
/// EKATranslate - class implementation
//////////////////////////////////////

EKATranslate::EKATranslate()
{
  m_dx = 0;
  m_dy = 0;
  m_dz = 0;
}

EKATranslate::EKATranslate(int dx,int dy, int dz)
{
  m_dx = dx;
  m_dy = dy;
  m_dz = dz;
}

EKATranslate::~EKATranslate()
{
}

void EKATranslate::Init(int dx,int dy, int dz)
{
  m_dx = dx;
  m_dy = dy;
  m_dz = dz;
}

//////////////////////////////////////
/// EKAField - class implementation
//////////////////////////////////////

EKAField::EKAField()
{
  m_nodeName = "";
  m_fieldName = "";
  m_value = "";
}

EKAField::EKAField(String nodeName,String fieldName,String value)
{
  m_nodeName = nodeName;
  m_fieldName = fieldName;
  m_value = value;
}

EKAField::~EKAField()
{
}

void EKAField::Init(String nodeName,String fieldName,String value)
{
  m_nodeName = nodeName;
  m_fieldName = fieldName;
  m_value = value;
}

//////////////////////////////////////
/// EKASwitchFields - class implementation
//////////////////////////////////////

EKASwitchFields::EKASwitchFields()
{
  m_nodeName[0] = "";
  m_fieldName[0] = "";
  m_value[0] = "";
  m_nodeName[1] = "";
  m_fieldName[1] = "";
  m_value[1] = "";
  m_current = 0;
}

EKASwitchFields::EKASwitchFields(String node1Name,String field1Name,String value1,String node2Name,String field2Name,String value2)
{
  m_nodeName[0] = node1Name;
  m_fieldName[0] = field1Name;
  m_value[0] = value1;
  m_nodeName[1] = node2Name;
  m_fieldName[1] = field2Name;
  m_value[1] = value2;
  m_current = 0;
}

void EKASwitchFields::Init(String node1Name,String field1Name,String value1,String node2Name,String field2Name,String value2)
{
  m_nodeName[0] = node1Name;
  m_fieldName[0] = field1Name;
  m_value[0] = value1;
  m_nodeName[1] = node2Name;
  m_fieldName[1] = field2Name;
  m_value[1] = value2;
  m_current = 0;
}

EKASwitchFields::~EKASwitchFields()
{
}
