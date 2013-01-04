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

#ifndef SGAL_EGO_VOXELS_TILER_HPP
#define SGAL_EGO_VOXELS_TILER_HPP

#include "SGAL/config.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class Ego_voxels_tiler {
public:
  // TODO: Missing if rows are x or y.
  enum First_tile_placement {FIRST00, FIRST01, FIRST10, FIRST11};
  enum Strategy {GRID, NONGRID};

  Ego_voxels_tiler(First_tile_placement first_tile, Strategy strategy);
  
  void operator() (Ego_voxels* out_voxels);

private:
  void tile_layer(size_t layer, Ego_voxels* out_voxels);

  size_t first_brick_x_offset;
  size_t first_brick_y_offset;
  size_t offset_between_rows;
};

SGAL_END_NAMESPACE


#endif // SGAL_EGO_VOXELS_TILER_HPP

