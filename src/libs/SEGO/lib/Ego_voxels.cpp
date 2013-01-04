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
  m_voxels.resize(length);
  for (long i = 0; i < length; ++i) {
    m_voxels[i].resize(width);
    for (long j = 0; j < width; ++j) {
      m_voxels[i][j].resize(height);
    }
  }
}

void Ego_voxels::fill(size_t x, size_t y, size_t z) {
  // We can also get boundary coordinates...

  if (x == m_voxels.size()) --x;
  if (y == m_voxels[0].size()) --y;
  if (z == m_voxels[0][0].size()) --z;

  SGAL_assertion(x < voxels.size());
  SGAL_assertion(y < voxels[0].size());
  SGAL_assertion(z < voxels[0][0].size());
  
  m_voxels[x][y][z].brick_location = boost::make_tuple(x, y, z);
}

bool Ego_voxels::is_filled(std::size_t x, std::size_t y, std::size_t z) const {
  return m_voxels[x][y][z].brick_location;
}

bool Ego_voxels::is_filled(const size_type& coord) const {
  return is_filled(coord.get<0>(), coord.get<1>(), coord.get<2>());
}
  
void Ego_voxels::print() const {

  for (std::size_t i = 0; i < m_voxels.size(); ++i) {
    for (std::size_t j = 0; j < m_voxels[0].size(); ++j) {
      for (std::size_t k = 0; k < m_voxels[0][0].size(); ++k) {
        if (is_filled(i, j, k))
          std::cout << "*";
        else
          std::cout << "-";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

Ego_voxels::Kernel::Point_3 Ego_voxels::origin() const {
  return m_origin;
}

void Ego_voxels::set_origin(const Kernel::Point_3& point) {
  m_origin = point;
}

Ego_voxels::size_type Ego_voxels::size() const {
  return size_type(m_voxels.size(),
                   m_voxels[0].size(),
                   m_voxels[0][0].size());
}

SGAL_END_NAMESPACE

