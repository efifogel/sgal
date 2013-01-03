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

#include "SCGAL/Exact_polyhedron_geo.hpp"

#include <vector>

SGAL_BEGIN_NAMESPACE

// Temp until we decide the true voxels.
// Each bool represents a voxel.
class Ego_voxels {
public:
  typedef Exact_polyhedron_geo::Polyhedron              Polyhedron;
  typedef Polyhedron::Traits::Kernel                    Kernel;
  typedef boost::tuple<
    std::size_t, std::size_t, std::size_t>              size_type;

  void initialize_container(long length, long width, long height);
  void fill(std::size_t x, std::size_t y, std::size_t z);
  bool is_filled(std::size_t x, std::size_t y, std::size_t z) const;
  bool is_filled(const size_type& coord) const;
  void print() const;
  
  Kernel::Point_3 origin() const;
  void set_origin(const Kernel::Point_3& point);

  size_type size() const;

private:
  typedef std::vector<std::vector<std::vector<bool> > > Container;
    
  Kernel::Point_3 m_origin;
  Container       m_voxels;
};

SGAL_END_NAMESPACE
    
#endif // SGAL_EGO_VOXELS_HPP
