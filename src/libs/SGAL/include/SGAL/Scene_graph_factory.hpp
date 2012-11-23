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

/*!
 * Creates Scene_graph instance
 */

#ifndef SGAL_SCENE_GRAPH_FACTORY_EFI
#define SGAL_SCENE_GRAPH_FACTORY_EFI

#include "SGAL/basic.hpp"
#include "Scene_graphDefs.h"
#include "Scene_graphInt.h"

SGAL_BEGIN_NAMESPACE

class EEventManagerInt;

class SCENE_GRAPH_CLASSDEF Scene_graphFactory {
public:
  Scene_graphFactory () { }
  virtual ~Scene_graphFactory () { }

  /*! returns a pointer to a Scene_graph Obj */
  Scene_graphInt *get_instance(EEventManagerInt *);
};

SGAL_END_NAMESPACE

#endif
