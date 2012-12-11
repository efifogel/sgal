// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter           <ophir.setter@gmail.com>

#ifndef SGAL_EGO_VOXELS_FILLER_HPP
#define SGAL_EGO_VOXELS_FILLER_HPP

#include "SGAL/config.hpp"
#include "SGAL/SGAL_defs.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_CLASSDEF Ego_voxels_filler {
 public:
  void operator() (Ego_voxels* voxels) const;
};

SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELS_FILLER_HPP
