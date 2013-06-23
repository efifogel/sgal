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

#include <vector>
#include <boost/tuple/tuple.hpp>

#include "SEGO/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_SEGO_DECL Ego_voxels_tiler {
public:
  typedef boost::tuple<size_t, size_t, size_t>          Brick_type;
  typedef std::vector<Brick_type>                       Brick_types;

  // TODO: Missing if rows are x or y.
  enum Tiling_rows{XROWS, YROWS};

  Ego_voxels_tiler(std::size_t even_layer_x,
                   std::size_t even_layer_y,
                   std::size_t odd_layer_x,
                   std::size_t odd_layer_y,
                   std::size_t offset_between_rows,
                   Tiling_rows rows,
                   const Brick_types &available_types);
  
  void operator()(Ego_voxels* out_voxels);

private:
  void tile_layer(size_t layer, Ego_voxels* out_voxels);
  void tile_layer(bool horizontal, bool skip, size_t layer,
                  const Brick_type& brick_type,
                  std::size_t first_brick_x_placement,
                  std::size_t first_brick_y_placement,
                  std::size_t offset_between_rows,
                  Ego_voxels* out_voxels);

  void tile_cell(size_t layer, size_t row, size_t column,
                 const Brick_type& brick_type, Ego_voxels* out_voxels);

  bool is_tiled(Ego_voxels* out_voxels, size_t x, size_t y, size_t z);

  // This represents the offset of the odd layers and the even layers
  // in case of the main brick. This actually what makes the tiling
  // stronger.
  // Should be adjusted not only for 2x2.
  size_t m_layers_offsets[2][2];
  size_t m_offset_between_rows;
  Tiling_rows m_tiling_rows;

  Brick_types m_available_types;
};

SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELS_TILER_HPP

