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

#include <limits>

// #define EGO_VOXELIZER_TILER_VERBOSE
#ifdef EGO_VOXELIZER_TILER_VERBOSE
#include <boost/tuple/tuple_io.hpp>
#endif

SGAL_BEGIN_NAMESPACE

// Tiling_rows are relevant only for non-grid.
Ego_voxels_tiler::Ego_voxels_tiler(std::size_t even_layer_x,
                                   std::size_t even_layer_y,
                                   std::size_t odd_layer_x,
                                   std::size_t odd_layer_y,
                                   std::size_t offset_between_rows,
                                   Tiling_rows rows,
                                   const Brick_types& available_types)
    : m_offset_between_rows(offset_between_rows),
      m_tiling_rows(rows), m_available_types(available_types) {

  m_layers_offsets[0][0] = even_layer_x;
  m_layers_offsets[0][1] = even_layer_y;
  m_layers_offsets[1][0] = odd_layer_x;
  m_layers_offsets[1][1] = odd_layer_y;
}

/** 
 * The current tiling covers all the voxels (might enlarge the
 * number of filled voxels.
 * It uses only 2x2 bricks and tries to tile them in a "strong" way.
 */
void Ego_voxels_tiler::operator() (Ego_voxels* out_voxels)
{
  size_t z;
  boost::tie(boost::tuples::ignore, boost::tuples::ignore, z) =
    out_voxels->size();
  
  for (size_t i = 0; i < z; ++i) {
    tile_layer(i, out_voxels);
  }    
}

void Ego_voxels_tiler::tile_layer(size_t layer, Ego_voxels* out_voxels) {
  
  // The first (main) brick we try to put in different locations.
  // The other bricks, which should only fill the holes - we don't need 
  // that.
  // We kind of assume that we have at least 1x1. This can be larger
  // in other schemes.

  // Case we have a (main) brick.
  if (m_available_types.size() > 1) {
    bool horizontal = (m_tiling_rows == XROWS);

    const Brick_type& brick = m_available_types.front();

    size_t first_brick_x = m_layers_offsets[layer % 2][0] % brick.get<0>();
    size_t first_brick_y = m_layers_offsets[layer % 2][1] % brick.get<1>();
    
    // It might be worth to move the skip outside...
    tile_layer(true, horizontal, layer, brick,
               first_brick_x, first_brick_y,
               m_offset_between_rows,
               out_voxels);
  }
  
  // Now we tile the rest (start from second, right?)
  Brick_types::iterator it = m_available_types.begin();
  if (m_available_types.size() > 1)
    ++it;
  
  for (; it != m_available_types.end(); ++it) {
    // horizontal does not matter if there is no offset...
    tile_layer(false, true, layer, *it, 0, 0, 0, out_voxels);
  }
}

/** 
 * 
 * 
 * @param skip Specifies whether to skip a brick if it does not fit.
 *        Otherwise, there is no point in different first location if
 *        the first location does not fit.
 * @param horizontal 
 * @param layer 
 * @param brick_type 
 * @param first_brick_x_placement 
 * @param first_brick_y_placement 
 * @param offset_between_rows 
 * @param out_voxels 
 */
void Ego_voxels_tiler::tile_layer(bool skip,
                                  bool horizontal,
                                  std::size_t layer,
                                  const Brick_type& brick_type,
                                  std::size_t first_brick_x_placement,
                                  std::size_t first_brick_y_placement,
                                  std::size_t offset_between_rows,
                                  Ego_voxels* out_voxels) {

  size_t maxx = out_voxels->size().get<0>();
  size_t maxy = out_voxels->size().get<1>();

  size_t jumpx = skip ? brick_type.get<0>() : 1;
  size_t jumpy = skip ? brick_type.get<1>(): 1;
  
  for (size_t i = first_brick_x_placement;
       i < maxx; i += jumpx) {
    for (size_t j = first_brick_y_placement;
         j < maxy; j += jumpy) {
      
      size_t row_index = (i - first_brick_x_placement) / jumpx;
      size_t column_index = (j - first_brick_y_placement) / jumpy;
      size_t row = i;
      size_t column = j;

      // Skip is according to the index...
      // if we are in an odd "row" we need to offset the column...
      if (horizontal) {
        if (row_index % 2 == 1)
          column += (offset_between_rows % brick_type.get<0>());
      } else {
        if (column_index % 2 == 1)
          row += (offset_between_rows % brick_type.get<1>());
      }

      // Should handle out-of-bounds
      tile_cell(layer, row, column, brick_type, out_voxels);
    }
  }
}


void Ego_voxels_tiler::tile_cell(size_t layer, size_t row, size_t column,
                                 const Brick_type& brick_type,
                                 Ego_voxels* out_voxels) {
  
  if (row >= out_voxels->size().get<0>())
    return;
  if (column >= out_voxels->size().get<1>())
    return;
  if (layer >= out_voxels->size().get<2>())
    return;

#ifdef EGO_VOXELIZER_TILER_VERBOSE
  std::cout << "Row: " << row << " Column: " << column << std::endl;
#endif

  // 1) All are legal and filled.
  // 2) All are not containing bricks.
  bool place = true;

  for (size_t i = row; i < row + brick_type.get<0>(); ++i) {
    for (size_t j = column; j < column + brick_type.get<1>(); ++j) {
      if (is_tiled(out_voxels, i, j, layer)) {
        place = false;
        break;
      }
    }
    
    // instead of goto.
    if (place == false)
      break;
  }
  
  if (place) {
#ifdef EGO_VOXELIZER_TILER_VERBOSE
    std::cout << "Placing: " << width << "x" << height << std::endl;
#endif
    
    out_voxels->place(boost::make_tuple(row, column, layer), brick_type);
  }
}

// If not legal - returns as if it is already tiled.
bool Ego_voxels_tiler::is_tiled(Ego_voxels* out_voxels, size_t x, size_t y, size_t z) {
  
  if (out_voxels->is_in_limits(x, y, z) == false)
    return true;

  // If this cell should not be filled.
  if (out_voxels->is_filled(x, y, z) == false)
    return true;

  // If this cell is already filled.
  if (out_voxels->is_placed(x, y, z))
    return true;

  return false;
}


SGAL_END_NAMESPACE
