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
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Services of specific type SAI field.
 *
 * Abstract class that holds only external services to the SAI_fieldTemplate
 * class.
 */

#ifndef EFI
#define EFI

#include "Scene_graphDefs.h"
#include "SAIFieldServices.h"

template<class T, int typeId>
class SCENE_GRAPH_CLASSDEF SAI_fieldSpecificServicesTemplate :
  public SAI_fieldServices
{
public:
  SAI_fieldSpecificServicesTemplate():SAI_fieldServices(){}

  // Value set and get functions
  virtual T get_value() const = 0;

  virtual void set_value(const T& value) = 0;
};

#endif
