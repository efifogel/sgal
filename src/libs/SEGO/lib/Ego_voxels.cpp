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

  for (long i = 0; i < length; ++i) {
    for (long j = 0; j < width; ++j) {
      for (long k = 0; k < height; ++k) {
        m_voxels[i][j][k].filled = false;
      }
    }
  }
}

Ego_voxels::size_type Ego_voxels::size() const {
  return size_type(m_voxels.size(),
                   m_voxels[0].size(),
                   m_voxels[0][0].size());
}

bool Ego_voxels::is_in_limits(size_t x, size_t y, size_t z) const {
  if (x >= m_voxels.size())
    return false;
  
  if (y >= m_voxels[x].size())
    return false;

  if (z >= m_voxels[x][y].size()) 
    return false;
  
  return true;
}

void Ego_voxels::fill(size_t x, size_t y, size_t z) {
  // We can also get boundary coordinates...

  if (x == m_voxels.size()) --x;
  if (y == m_voxels[0].size()) --y;
  if (z == m_voxels[0][0].size()) --z;

  SGAL_assertion(x < m_voxels.size());
  SGAL_assertion(y < m_voxels[0].size());
  SGAL_assertion(z < m_voxels[0][0].size());
  
  m_voxels[x][y][z].filled = true;
}

bool Ego_voxels::is_filled(std::size_t x, std::size_t y, std::size_t z) const {
  SGAL_assertion(is_in_limits(x, y, z) == true);
  
  return m_voxels[x][y][z].filled;
}

bool Ego_voxels::is_filled(const size_type& coord) const {
  return is_filled(coord.get<0>(), coord.get<1>(), coord.get<2>());
}
  
void Ego_voxels::place(const size_type& coord, const size_type& size) {

  size_t x, y, z;
  boost::tie(x, y, z) = coord;

  for (size_t i = 0; i < size.get<0>(); ++i) {
    for (size_t j = 0; j < size.get<1>(); ++j) {
      for (size_t k = 0; k < size.get<2>(); ++k) {
        SGAL_assertion(is_in_limits(x+i, y+j, z+k) == true);
        SGAL_assertion(is_filled(x+i, y+j, z+k) == true);
        m_voxels[x+i][y+j][z+k].brick_location = coord;
      }
    }
  }
  
  // first brick to contain the brick:
  m_voxels[x][y][z].brick_size = size;
}

void Ego_voxels::clear_placing() {
  for (size_t i = 0; i < m_voxels.size(); ++i)
    for (size_t j = 0; j < m_voxels[0].size(); ++j)
      for (size_t k = 0; k < m_voxels[0][0].size(); ++k) {
        m_voxels[i][j][k].brick_location.reset();
        m_voxels[i][j][k].brick_size.reset();
      }
}

// A brick was placed in this place if there is a brick in the coords location.
bool Ego_voxels::is_placed(std::size_t x, std::size_t y, std::size_t z) {
  SGAL_assertion(is_filled(x, y, z) == true);

  return m_voxels[x][y][z].brick_location;
}


boost::optional<Ego_voxels::size_type>
Ego_voxels::get_brick(std::size_t x, std::size_t y, std::size_t z) {
  return m_voxels[x][y][z].brick_size;
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

SGAL_END_NAMESPACE
