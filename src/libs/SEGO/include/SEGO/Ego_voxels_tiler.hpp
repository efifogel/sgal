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
#include <boost/tuple/tuple.hpp>
#include <vector>

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class Ego_voxels_tiler {
public:
  // TODO: Missing if rows are x or y.
  enum First_tile_placement {FIRST00, FIRST01, FIRST10, FIRST11};
  enum Strategy {GRID, NONGRID};
  enum Tiling_rows {XROWS, YROWS};

  Ego_voxels_tiler(First_tile_placement first_tile,
                   Strategy strategy,
                   Tiling_rows rows);
  
  void operator() (Ego_voxels* out_voxels);

private:
  void tile_layer(size_t layer, Ego_voxels* out_voxels);
  void tile_cell(size_t layer, size_t row, size_t column,
                 size_t width, size_t height, Ego_voxels* out_voxels);

  bool is_tiled(Ego_voxels* out_voxels, size_t x, size_t y, size_t z);

  size_t m_first_brick_x_offset;
  size_t m_first_brick_y_offset;
  size_t m_offset_between_rows;
  Tiling_rows m_tiling_rows;

  typedef boost::tuple<size_t, size_t, size_t>  Lego;
  typedef std::vector<Lego>                     Legos;
  Legos m_available_bricks;
};

SGAL_END_NAMESPACE


#endif // SGAL_EGO_VOXELS_TILER_HPP

