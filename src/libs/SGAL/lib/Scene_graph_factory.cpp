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

/************************************************************/
/*                                */
/*  Class : Scene_graphFactory                */
/*                                */
/*  Purpose :                        */
/*                              */
/*    Creates Scene_graph instance             */
/*                              */
/*  Description :                      */
/************************************************************/
#include "SGAL/basic.hpp"
#include "Scene_graphFactory.h"
#include "SceneManager.h"

using namespace SGAL;

/** actually returns a pointer to a  ESceneManager Obj */
Scene_graphInt * Scene_graphFactory::get_instance(EEventManagerInt *eventManager)
{
  return new ESceneManager();
}
