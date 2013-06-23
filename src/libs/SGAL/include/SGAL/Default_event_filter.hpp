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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DEFAULT_EVENT_FILTER_HPP
#define SGAL_DEFAULT_EVENT_FILTER_HPP

/*! \file
 * This is a default event filter that is responsible to handle context related
 * events such as Draw, Size.
 * It is also responsible to catch mouse move events and call Isect as
 * necessary.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Event_filter.h"
#include "SGAL/Scene_graphDefs.h"
#include "SGAL/Debug.h"

SGAL_BEGIN_NAMESPACE

class Scene_graphInt;
class Execution_coordinator;

class SGAL_SGAL_DECL EDefaultEvent_filter : public EEvent_filter {
  DECLARE_DEBUG_CLASS(DBG_DEFAULTEVENTFILTER);
public:
  EDefaultEvent_filter() : EEvent_filter(), m_executionCoordinator(0) 
  {
    REGISTER_DEBUG_CLASS(DBG_DEFAULTEVENTFILTER, "EDefaultEvent_filter");
  };
  virtual ~EDefaultEvent_filter() {};

  void set_executionCoordinator(Execution_coordinator* ec)
  { m_executionCoordinator = ec; }

  void SetScene_graph(Scene_graphInt* sg) { m_sceneGraph = sg; }

  virtual bool Draw ( const EDrawData& );
  virtual bool Size ( const ESizeData& );
  virtual bool EraseBkgnd ( const EEraseBkgndData& );
  virtual bool PaletteChanged ( const EPaletteChangedData& );
  virtual bool SetCursor ( const ECursorData& );

private:

  /** a pointer to the executtion manage to redraw the scene */
  Execution_coordinator* m_executionCoordinator;

  /** a pointer to the scene graph interface. Used to call Isect
      on the scene graph */
  Scene_graphInt* m_sceneGraph;
};

SGAL_END_NAMESPACE

#endif
