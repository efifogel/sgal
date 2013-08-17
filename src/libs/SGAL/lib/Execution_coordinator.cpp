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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "Execution_coordinator.h"

using namespace SGAL;

Execution_coordinator::Execution_coordinator():
    m_renderingRequired(false),
    m_draggingLocked(false),
    m_boundingSphereModified(true),
    m_currentCursorId(ctARROW),
    m_isCursorOn(true),
    m_isMouseOver(false),
    m_isLoadingDone(true),
    m_isAAInterrupt(false),
    m_activeKeySensor(NULL),
    m_scopeLocked (false),
    m_frameRate(30),
    m_minFrameRate(0),
    m_currentViewCalculationRequired(true),
    m_flowSensor(0),
    m_errorHandle(0),
    m_CompressedInterpolatorsCount(0),
    m_isUpdateTimeActive(true),
    m_SceneBoundingSphereRadius((Float)0.0),
    m_frameCounter(0)
{}


Scene_time Execution_coordinator::get_sceneTime()
{
    if ( EOperatingSystem::get_instance()==ENULL) {
        return 0;
    }
    return 10000.0 +
        ((Scene_time)EOperatingSystem::get_instance()->get_elapsedTime()) / 1000.0;
}

void Execution_coordinator::SetLoadingDone(Boolean flag)
{
    if (!m_isLoadingDone && flag) {
        // If loading done flag rised, scene must be rerendered.
        SetRenderingRequired();
    }
    m_isLoadingDone = flag;
}

void Execution_coordinator::SignalLastLevelRendered()
{
    if ( m_flowSensor ) {
        m_flowSensor->SetLoadingDone(true);
    }
}

void Execution_coordinator::SignalLevel0Rendered()
{
  if ( m_flowSensor ) {
    m_flowSensor->SetLevel0LoadingDone(true);
  }
}

void Execution_coordinator::SignalSnapshotDone()
{
    if ( m_flowSensor ) {
        m_flowSensor->SetSnapshotDone(true);
    }
}

void Execution_coordinator::UpdateCompInterpolator() 
{ 
    m_CompressedInterpolatorsCount--; 
    if (m_CompressedInterpolatorsCount <= 0)
    {
        m_isUpdateTimeActive = true;        
        if (m_flowSensor)
        {
            m_flowSensor->set_animationLoadingDone();
        }
    }
}

void Execution_coordinator::SetFlowSensor(EFlowSensor *flowSensor) 
{ 
    m_flowSensor = flowSensor; 
}

Boolean Execution_coordinator::IsAAInterrupt() 
{
    if ( m_isAAInterrupt ) {
        m_isAAInterrupt = false;
        return true;;
    }
    return m_isAAInterrupt;
}

void Execution_coordinator::SendErrorMessage(EErrorType type, String &error) 
{ 
    if ( m_errorHandle ) {
        m_errorHandle->SendErrorMessage(type, error); 
    }
}

void Execution_coordinator::AddPolygons(const int n) 
{
  m_flowSensor->AddNumPolygons(n);
}

void Execution_coordinator::AddMemory(const int n)
{
  m_flowSensor->AddGeometryMemory(n);
}
