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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOADER_HPP
#define SGAL_LOADER_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;

class SGAL_SGAL_DECL Loader {
public:
  /*! Constructor */
  Loader() {}
  
  /*! Destructor */
  int load(const char* filename, Scene_graph* sg);
};

SGAL_END_NAMESPACE

#endif
