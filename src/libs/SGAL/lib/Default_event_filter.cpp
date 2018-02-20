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
#include "Scene_graphPch.h"
#include "DefaultEvent_filter.h"
#include "Scene_graphInt.h"
#include "CursorData.h"
#include "DrawData.h"
#include "ExecutionCoordinator.h"

using namespace SGAL;

INIT_DEBUG_CLASS(EDefaultEvent_filter, DBG_DEFAULTEVENTFILTER)

bool EDefaultEvent_filter::Draw( const EDrawData & data)
{
  //DBG(get_debug()->DebugEmit(DBG_DEFAULTEVENTFILTER, "HDC = %h", data.get_dC()));

  if ( m_executionCoordinator )
    m_executionCoordinator->SetRenderingRequired();
  return true;
}

bool EDefaultEvent_filter::Size( const ESizeData & )
{
  if ( m_executionCoordinator )
    m_executionCoordinator->SetRenderingRequired();
  return true;
}

bool EDefaultEvent_filter::EraseBkgnd( const EEraseBkgndData & )
{
  if ( m_executionCoordinator )
    m_executionCoordinator->SetRenderingRequired();
  return true;
}

bool EDefaultEvent_filter::PaletteChanged( const EPaletteChangedData & )
{
  if ( m_executionCoordinator )
    m_executionCoordinator->SetRenderingRequired();
  return true;
}

bool EDefaultEvent_filter::SetCursor  ( const ECursorData &cursor )
{
  //m_sceneGraph->Isect(cursor.GetX(), cursor.GetY());
  return true;
}

