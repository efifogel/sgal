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

#ifndef SGAL_EGO_VOXELS_HPP
#define SGAL_EGO_VOXELS_HPP

#include "SCGAL/Exact_polyhedron_geo.hpp"

#include <vector>

SGAL_BEGIN_NAMESPACE

// Temp until we decide the true voxels.
// Each bool represents a voxel.
class Ego_voxels {
public:
  void initialize_container(long length, long width, long height);
  void mark(std::size_t x, std::size_t y, std::size_t z);
  void print() const;

  typedef std::vector<std::vector<std::vector<bool> > > Container;
    
  typedef Exact_polyhedron_geo::Polyhedron              Polyhedron;
  typedef Polyhedron::Traits::Kernel                    Kernel;
  
  Kernel::Point_3 origin;
  Container       voxels;
};

SGAL_END_NAMESPACE
    
#endif // SGAL_EGO_VOXELS_HPP
