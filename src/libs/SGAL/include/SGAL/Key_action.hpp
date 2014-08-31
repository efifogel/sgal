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

#ifndef SGAL_KEY_ACTION_HPP
#define SGAL_KEY_ACTION_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Key_action {
public:
  Key_action();
  virtual ~Key_action();
};

class SGAL_SGAL_DECL EKARotate : public Key_action {
public:
  EKARotate();
  EKARotate(int vx,int vy, int vz, int angle);
  virtual ~EKARotate();

  void Init(int vx,int vy, int vz, int angle);

  inline int get_vX() {return m_vx;};
  inline int get_vY() {return m_vy;};
  inline int get_vZ() {return m_vz;};
  inline int get_angle() {return m_angle;};

private:
  int m_vx;
  int m_vy;
  int m_vz;
  int m_angle;
};

class SCENE_GRAPH_CLASSDEF EKATranslate : public Key_action {
public:
  EKATranslate();
  EKATranslate(int dx,int dy, int dz);
  virtual ~EKATranslate();

  void Init(int dx,int dy, int dz);

  inline int get_dX() {return m_dx;};
  inline int get_dY() {return m_dy;};
  inline int get_dZ() {return m_dz;};
private:
  int m_dx;
  int m_dy;
  int m_dz;
};

class SCENE_GRAPH_CLASSDEF EKAField : public Key_action {
public:
  EKAField();
  EKAField(String nodeName, String fieldName, String value);
  virtual ~EKAField();

  void Init(String nodeName, String fieldName, String value);

  inline String get_nodeName() {return m_nodeName;};
  inline String get_fieldName() {return m_fieldName;};
  inline String get_value() {return m_value;};

private:
  String m_nodeName;
  String m_fieldName;
  String m_value;
};

class SCENE_GRAPH_CLASSDEF EKASwitchFields : public Key_action
{
public:
  EKASwitchFields();
  EKASwitchFields(String node1Name,String field1Name,String value1,String node2Name,String field2Name,String value2);
  virtual ~EKASwitchFields();

  void Init(String node1Name,String field1Name,String value1,String node2Name,String field2Name,String value2);

  inline String get_nodeName() {return m_nodeName[m_current];};
  inline String get_fieldName() {return m_fieldName[m_current];};
  inline String get_value() {return m_value[m_current];};

  inline void SwitchFields() {m_current = (m_current+1)%2;};

private:
  String m_nodeName[2];
  String m_fieldName[2];
  String m_value[2];

  int m_current;
};

class SCENE_GRAPH_CLASSDEF EKAReset : public Key_action
{
public:
  EKAReset() {};
  virtual ~EKAReset() {};
};

SGAL_END_NAMESPACE

#endif
