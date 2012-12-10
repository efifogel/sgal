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

#include "SEGO/Ego_voxels.hpp"

SGAL_BEGIN_NAMESPACE

void
Ego_voxels::initialize_container(long length,
                                 long width,
                                 long height) {
  voxels.resize(length);
  for (long i = 0; i < length; ++i) {
    voxels[i].resize(width);
    for (long j = 0; j < width; ++j) {
      voxels[i][j].resize(height);
      for (long k = 0; k < height; ++k)
        voxels[i][j][k] = false;
    }
  }
}

void Ego_voxels::mark(size_t x, size_t y, size_t z) {
  // We can also get boundary coordinates...

  if (x == voxels.size()) --x;
  if (y == voxels[0].size()) --y;
  if (z == voxels[0][0].size()) --z;

  SGAL_assertion(x < voxels.size());
  SGAL_assertion(y < voxels[0].size());
  SGAL_assertion(z < voxels[0][0].size());
  
  voxels[x][y][z] = true;    
}

void Ego_voxels::print() const {

  for (std::size_t i = 0; i < voxels.size(); ++i) {
    for (std::size_t j = 0; j < voxels[0].size(); ++j) {
      for (std::size_t k = 0; k < voxels[0][0].size(); ++k) {
        if (voxels[i][j][k] == true)
          std::cout << "*";
        else
          std::cout << "-";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

SGAL_END_NAMESPACE

