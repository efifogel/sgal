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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "Scene_manager.h"
#include "Scene_graph.h"
#include "SAI.h"
#include "Interpolator_int.h"
#include "Trace.h"

SGAL_BEGIN_NAMESPACE

Scene_manager::Scene_manager() 
{
  m_scene = new Scene_graph();
}

Scene_manager::Scene_manager(Boolean createScene) 
{
  // if this is called with FALSE, need to make sure SetScene_graph() is called as well
  if ( createScene ) {
    m_scene = new Scene_graph();
  } else {
    m_scene = 0;
  }
}

/*! Destructor */
Scene_manager::~Scene_manager() 
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Scene_manage ...");
  // Delete all items in m_SAIs
  SAIListType::iterator iter = m_SAIs.begin();
  while( iter != m_SAIs.end())
  {
    ESAI* SAI = (*iter);
    delete SAI;
    iter++;
  }

  delete m_scene;
  m_scene = 0;
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

void Scene_manager::CreateContext(Window_handle *winHandle)
{
  if ( m_scene ) {
    m_scene->CreateContext(winHandle);
  }
}

void Scene_manager::ReleaseContext()
{
  if ( m_scene ) {
    m_scene->ReleaseContext();
  }
}

void Scene_manager::MakeCurrent()
{
  if (m_scene!=NULL) 
  {
    Context* pContext = NULL;
    pContext = m_scene->get_context();
    if (pContext != NULL)
    {
      pContext->MakeCurrent();
    }
  }
}

XML_entity *Scene_manager::CreateNode(String tag, Load_int **loadInt, EEvent_filter **evenFilter)
{
  XML_entity_factory *factory = XML_entity_factory::get_instance();
  XML_entity *newObj = 0;
  if ( factory ) 
  {
    newObj = factory->CreateEntity(tag);
  }

  Load_int *tmp = dynamic_cast<Load_int *>(newObj);
  if ( tmp ) {
    *loadInt = tmp;
  }
  
  EEvent_filter *ef = dynamic_cast<EEvent_filter *>(newObj);
  if ( ef ) {
    *evenFilter = ef;
  }

  // Count compressed interpolator nodes
  Interpolator_int *pInt = dynamic_cast<Interpolator_int *>(newObj);
  if (pInt) {
    Execution_coordinator *pEc = get_executionCoordinator();
    if (pEc) {
      pEc->AddCompInterpolator(); 
      pInt->set_executeCoordinator(pEc);
    }
  }

  return newObj;
}

/**
    Creates a new "Scene Authoring Interface" instance
*/
ESAIServices *Scene_manager::CreateSAI()
{
    // Create a new SAI instance and insert it into m_SAIs
    ESAI* SAI = new ESAI(m_scene);
    m_SAIs.push_back(SAI);
  return SAI;
}

/**
    Deletes the SAI pointed by the given
*/
void Scene_manager::DisposeSAI(ESAIServices* SAI)
{
    // Remove the instance from the list and delete it
    ESAI* castedSAI = dynamic_cast<ESAI*>(SAI);
    m_SAIs.remove(castedSAI);
    delete castedSAI;
}


/**
    Signal cascade start for all scene authoring interfaces in m_SAIs
*/
void Scene_manager::SignalCascadeStart()
{
  // signal the execution coordinator the cascade start
  if(get_executionCoordinator())
    get_executionCoordinator()->CycleReset();

   if (m_scene)
    m_scene->SignalCascadeStart();

  SAIListType::iterator iter = m_SAIs.begin();
    while( iter != m_SAIs.end())
    {
    (*iter)->SignalCascadeStart();
    iter++;
  }
}

/**
    Signal cascade end for all scene authoring interfaces in m_SAIs
*/
void Scene_manager::SignalCascadeEnd()
{
  if (m_scene)
    m_scene->SignalCascadeEnd();
    
  SAIListType::iterator iter = m_SAIs.begin();
    while( iter != m_SAIs.end())
    {
    (*iter)->SignalCascadeEnd();
    iter++;
  }
}


void Scene_manager::AddToScene(XML_entity *parent, XML_entity *node)
{
  if ( m_scene ) {
    m_scene->AddToScene(parent, node);
  }
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Scene_manager::set_attributes(XML_entity *node, Attribute_list &attribs)
{
  if (m_scene) {
    m_scene->set_attributes(node, attribs);
  }
}

void Scene_manager::SetRoot(XML_entity *root)
{
  if ( m_scene ) {
    m_scene->SetRoot(root);
  }
}

void Scene_manager::SetNavigationRoot(XML_entity *nr)
{
  if ( m_scene ) {
    m_scene->SetNavigationRoot(nr);
  }
}

void Scene_manager::InitCreation()
{
  if ( m_scene ) {
    m_scene->InitCreation();
  }
}

void Scene_manager::SetDone()
{
  if ( m_scene ) {
    m_scene->SetDone();
  }
}

void Scene_manager::Render(Draw_action *draw_action)
{
  if ( m_scene ) {
    m_scene->Render(draw_action);
  }
}

void Scene_manager::Isect(UInt x, UInt y)
{
  if ( m_scene ) {
    m_scene->Isect(x, y);
  }
}

EEvent_filter *Scene_manager::get_navigation_sensor()
{
  if ( m_scene ) {
    return m_scene->get_navigation_sensor();
  }
  return 0;
}

Execution_coordinator *Scene_manager::get_executionCoordinator()
{
  if ( m_scene ) {
    return m_scene->get_executionCoordinator();
  }
  return 0;
}

void Scene_manager::SetDefaultEvent_filter(EEvent_filter *ef)
{
  if ( m_scene ) {
    m_scene->SetDefaultEvent_filter(ef);
  }
}

Boolean Scene_manager::ActiveNextCamera()
{
  if( m_scene ) {
    return m_scene->ActiveNextCamera(); 
  }
  return SGAL_FALSE;
}

Boolean Scene_manager::ActivePrevCamera()
{
  if( m_scene ) {
    return m_scene->ActivePrevCamera(); 
  }

  return SGAL_FALSE;
}

Boolean Scene_manager::HasMoreThenOneCamera ()
{
  if( m_scene && m_scene->get_camera_pool() ) {
    return m_scene->get_camera_pool()->get_count() > 1; 
  }
  return false;
}

Boolean Scene_manager::IsLastCameraActivated()
{
  if( m_scene && m_scene->get_camera_pool()) {
    return m_scene->get_camera_pool()->IsLastCameraActivated(); 
  }
  return false;
}

Boolean Scene_manager::IsFirstCameraActivated()
{
  if( m_scene && m_scene->get_camera_pool() ) {
    return m_scene->get_camera_pool()->IsFirstCameraActivated(); 
  }
  return false;
}

#ifdef ENB_MPEG_PROJECT
Container *Scene_manager::GetRemoteKeys()
{
  if( m_scene ) {
    return m_scene->get_container("KeyConfig"); 
  }

  return 0;
}
#endif //ENB_MPEG_PROJECT

SGAL_END_NAMESPACE
