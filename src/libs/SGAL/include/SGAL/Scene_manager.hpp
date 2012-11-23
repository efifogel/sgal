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
// $Source$
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 *    
 */

#ifndef SGAL_SCENE_MANAGER_HPP
#define SGAL_SCENE_MANAGER_HPP

#include "SGAL/basic.hpp"
#include "Scene_graphInt.h"
#include "Scene_graph.h"
#include "SAI.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;

class SCENE_GRAPH_CLASSDEF ESceneManager : public Scene_graphInt {
public:
  ESceneManager();
  ESceneManager(Boolean createScene);
  virtual ~ESceneManager();

  virtual void CreateContext(Window_handle * winHandle);
  virtual void ReleaseContext();
  virtual void MakeCurrent();
  virtual XML_entity * CreateNode(String tag, Load_int ** loadInt = 0,
                                  EEvent_filter ** evenFilter = 0);
  virtual ESAIServices * CreateSAI();
  virtual void DisposeSAI(ESAIServices * SAI);
  virtual void SignalCascadeStart();
  virtual void SignalCascadeEnd();

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  virtual void SetRoot(XML_entity * root);
  virtual void SetNavigationRoot(XML_entity * nr);
  virtual void InitCreation();
  virtual void SetDone();
  virtual void Render(Draw_action * draw_action);
  virtual void Isect(UInt x, UInt y);
  virtual EEvent_filter * get_navigation_sensor();
  virtual Execution_coordinator * get_executionCoordinator();
  virtual void SetDefaultEvent_filter(EEvent_filter * ef);
  virtual Boolean ActiveNextCamera();
  virtual Boolean ActivePrevCamera();
  virtual Boolean IsLastCameraActivated();
  virtual Boolean IsFirstCameraActivated();
  virtual Boolean HasMoreThenOneCamera ();

  Scene_graph * get_scene_graph() { return m_scene; }
  void ESceneManager::SetScene_graph(Scene_graph *scene) { m_scene = scene; }

#ifdef ENB_MPEG_PROJECT
  Container * GetRemoteKeys();
#endif

private:
  /*! a pionter to the scene graph */
  Scene_graph * m_scene;

  /*! a list of scene authoring interfaces */
  SAIListType m_SAIs;

  ESceneManager(const ESceneManager &) {}

};

SGAL_END_NAMESPACE

#endif
