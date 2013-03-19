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

#include "SGAL/basic.hpp"

#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>

#include <vector>

SGAL_BEGIN_NAMESPACE


// Temp until we decide the true voxels.
// Each bool represents a voxel.
class Ego_voxels {
public:
  typedef boost::tuple<
    std::size_t, std::size_t, std::size_t>              size_type;

  void initialize_container(long length, long width, long height);
  size_type size() const;

  bool is_in_limits(size_t, size_t, size_t) const;

  void fill(std::size_t x, std::size_t y, std::size_t z);
  bool is_filled(std::size_t x, std::size_t y, std::size_t z) const;
  bool is_filled(const size_type& coord) const;

  void place(const size_type& coord, const size_type& size);
  bool is_placed(std::size_t x, std::size_t y, std::size_t z);
  boost::optional<size_type>
  get_brick(std::size_t x, std::size_t y, std::size_t z);

  void print() const;

private:

  // Each voxel is covered by one brick, but one brick can cover many
  // voxels. A voxel contains the position of its brick (where the
  // term "position of its brick" is defined to be the voxel with
  // the smallest coordinate that it covers.
  // This voxel also contains the size of the brick.
  
  class Voxel {
  public:
    bool filled;
    boost::optional<size_type> brick_location;
    boost::optional<size_type> brick_size;
  };

  typedef std::vector<std::vector<std::vector<Voxel> > > Container;
    
  Container       m_voxels;
};

SGAL_END_NAMESPACE
    
#endif // SGAL_EGO_VOXELS_HPP
