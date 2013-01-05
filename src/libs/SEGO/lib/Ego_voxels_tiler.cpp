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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

#include "SEGO/Ego_voxels_tiler.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple.hpp>

SGAL_BEGIN_NAMESPACE

// Tiling_rows are relevant only for non-grid.
Ego_voxels_tiler::Ego_voxels_tiler(First_tile_placement first_tile,
                                   Strategy strategy,
                                   Tiling_rows rows)
    : m_tiling_rows(rows) {
  switch (first_tile) {
  case FIRST00:
    m_first_brick_x_offset = 0;
    m_first_brick_y_offset = 0;
    break;
  case FIRST01:
    m_first_brick_x_offset = 0;
    m_first_brick_y_offset = 1;
    break;
  case FIRST10:
    m_first_brick_x_offset = 1;
    m_first_brick_y_offset = 0;
    break;
  case FIRST11:
    m_first_brick_x_offset = 1;
    m_first_brick_y_offset = 1;
    break;
  }

  switch (strategy) {
  case GRID:
    m_offset_between_rows = 0;
    break;
  case NONGRID:
    m_offset_between_rows = 1;
    break;
  }
}

/** 
 * The current tiling covers all the voxels (might enlarge the
 * number of filled voxels.
 * It uses only 2x2 bricks and tries to tile them in a "strong" way.
 */
void Ego_voxels_tiler::operator() (Ego_voxels* out_voxels) {

  size_t z;
  boost::tie(boost::tuples::ignore, boost::tuples::ignore, z) =
    out_voxels->size();
  
  for (size_t i = 0; i < z; ++i) {
    tile_layer(i, out_voxels);
  }
}

void Ego_voxels_tiler::tile_layer(size_t layer, Ego_voxels* out_voxels) {

  size_t xmax, ymax;
  if (m_tiling_rows == XROWS)
    boost::tie(xmax, ymax, boost::tuples::ignore) = out_voxels->size();
  else
    boost::tie(ymax, xmax, boost::tuples::ignore) = out_voxels->size();
  
  size_t x = (layer + m_first_brick_x_offset) % 2;
  for (; x + 1 < xmax; x += 2) {

    size_t y = (layer + m_first_brick_y_offset + (x/2) * m_offset_between_rows) % 2;
    for (; y + 1 < ymax; y += 2) {

      size_t row = (m_tiling_rows == XROWS) ? x : y;
      size_t column = (m_tiling_rows == XROWS) ? y : x;

      // now, if there is a voxel which is filled here, place a 2x2 brick.
      bool place = false;
      place = place || out_voxels->is_filled(row, column, layer);
      place = place || out_voxels->is_filled(row+1, column, layer);
      place = place || out_voxels->is_filled(row, column+1, layer);
      place = place || out_voxels->is_filled(row+1, column+1, layer);

      if (place)
        out_voxels->place(boost::make_tuple(row, column, layer),
                          boost::make_tuple(2, 2, 1));
    }
  }
}


SGAL_END_NAMESPACE
