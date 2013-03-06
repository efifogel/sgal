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

// #define EGO_VOXELIZER_TILER_VERBOSE
#ifdef EGO_VOXELIZER_TILER_VERBOSE
#include <boost/tuple/tuple_io.hpp>
#endif

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

  // We currently use only 4x2, 2x2, 2x1, and 1x1 bricks.
  // Order of insertion determines preference.
  m_available_bricks.push_back(Lego(2, 4, 1));
  m_available_bricks.push_back(Lego(4, 2, 1));
  m_available_bricks.push_back(Lego(2, 2, 1));
  m_available_bricks.push_back(Lego(2, 1, 1));
  m_available_bricks.push_back(Lego(1, 2, 1));
  m_available_bricks.push_back(Lego(1, 1, 1));
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

  size_t xmax, ymax;
  bool horizontal = (m_tiling_rows == XROWS);
  if (horizontal)
    boost::tie(xmax, ymax, boost::tuples::ignore) = out_voxels->size();
  else
    boost::tie(ymax, xmax, boost::tuples::ignore) = out_voxels->size();
  
  size_t x = (layer + m_first_brick_x_offset) % 2;
  for (; x + 1 < xmax; x += 2) {

    size_t y = (layer + m_first_brick_y_offset + (x/2) * m_offset_between_rows) % 2;
    for (; y + 1 < ymax; y += 2) {

      size_t row = (horizontal) ? x : y;
      size_t column = (horizontal) ? y : x;
      size_t width = (horizontal) ? 2 : 4;
      size_t height = (horizontal) ? 4 : 2;

      this->tile_cell(layer, row, column, width, height, out_voxels);
    }
  }
}

void Ego_voxels_tiler::tile_cell(size_t layer, size_t row, size_t column,
                                 size_t width, size_t height,
                                 Ego_voxels* out_voxels) {
  
#ifdef EGO_VOXELIZER_TILER_VERBOSE
  std::cout << "Row: " << row << " Column: " << column << std::endl;
#endif

  for (size_t i = 0; i < 2; ++i) {
    for (size_t j = 0; j < 2; ++j) {

      for (Legos::iterator it = m_available_bricks.begin();
           it != m_available_bricks.end(); ++it) {

#ifdef EGO_VOXELIZER_TILER_VERBOSE
        std::cout << "Location: " << row + i << " " << column + j << std::endl;
        std::cout << "Checking brick " << *it << std::endl;
        std::cout << "Width " << width << " Height " << height << std::endl;
#endif
        // Check that the lego brick is in the proper size.
        if (it->get<0>() > width - i)
          continue;
        if (it->get<1>() > height - j)
          continue;

        // 1) All are legal and filled.
        // 2) All are not containing bricks.
        bool place = true;
        size_t place_x = row + i;
        size_t place_y = column + j;
        for (size_t n = place_x; n < place_x + it->get<0>(); ++n) {
          for (size_t m = place_y; m < place_y + it->get<1>(); ++m) {
#ifdef EGO_VOXELIZER_TILER_VERBOSE
            std::cout << n << " " << m << " is " << is_tiled(out_voxels, n, m, layer)
                      << std::endl;
#endif
            if (is_tiled(out_voxels, n, m, layer)) {
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
          std::cout << "Placing " << *it << std::endl;
          if (it->get<0>() == 4 || it->get<1>() == 4)
            std::cout << *it << std::endl;
#endif
          out_voxels->place(boost::make_tuple(place_x, place_y, layer), *it);
          break;
        }
      }
    }
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
