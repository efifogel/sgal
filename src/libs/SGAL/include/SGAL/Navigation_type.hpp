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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_NAVIGATION_TYPE_HPP
#define SGAL_NAVIGATION_TYPE_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

//! The navigation types.
enum class Navigation_type {
  NONE,
  ANY,
  EXAMINE,
  FLY,
  WALK,
  TRANSFORM,
  NUM_TYPES
};

//! The navigation types literals.
extern const char* s_navigation_types[];

SGAL_END_NAMESPACE

#endif
