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

#ifndef SGAL_EGO_VOXELIZER_HPP
#define SGAL_EGO_VOXELIZER_HPP

/**
 * @file   Ego_voxelizer.hpp
 * @author Ophir Setter <ophirset@ophir-desktop>
 * @date   Fri Nov 30 13:59:39 2012
 * 
 * @brief  The class converts Polyhedron into voxels.
 * 
 * 
 */


#include "SGAL/config.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

SGAL_BEGIN_NAMESPACE

namespace SEGO_internal{
  
  template <class Point>
  struct Compare_point_by_axis {
    long m_dim;
    
    Compare_point_by_axis(long dim) : m_dim(dim) {}
    
    bool operator() (const Point& a, const Point& b) const {
      return a[m_dim] < b[m_dim];
    }
  };

} // SEGO_internal

class SGAL_CLASSDEF Ego_voxelizer {
 public:
  
  typedef Exact_polyhedron_geo::Polyhedron              Polyhedron;
  typedef Polyhedron::Traits::Kernel                    Kernel;

  // Temp until we decide the true voxels.
  // Each bool represents a voxel.
  class Voxels {
  public:
    void initialize_container(long length, long width, long height);
    void mark(size_t x, size_t y, size_t z);
    void print() const;

    typedef std::vector<std::vector<std::vector<bool> > > Container;
    
    Kernel::Point_3 origin;
    Container       voxels;
  };
    
  Ego_voxelizer(const Kernel::FT& voxel_length, const Kernel::FT& voxel_width,
                const Kernel::FT& voxel_height);
  void operator() (const Polyhedron& polyhedron, Voxels* out_voxels) const;
  void operator() (const Geo_set& geo_set, Voxels* out_voxels) const;
  
 private:

  typedef Kernel::Iso_cuboid_3                          Iso_cuboid_3;
  typedef Kernel::Segment_3                             Segment_3;  
  typedef Kernel::Point_3                               Point_3;  
  typedef Kernel::Plane_3                               Plane_3;  
  typedef Kernel::Triangle_3                            Triangle_3;  
  typedef Kernel::Direction_3                           Direction_3;  
  typedef Kernel::Vector_3                              Vector_3;  
  
  typedef std::list<Segment_3>                          SlicingSegments;
  typedef std::list<Triangle_3>                         Triangles;

  void operator() (const Triangles& triangles, Voxels* out_voxels) const;

  Triangles create_triangles_from_polyhedron(const Polyhedron& polyhedron)
    const;
  Triangles create_triangles_from_geo_set(const Geo_set& polyhedron) const;
  
  void create_voxels_from_triangles(const Triangles& polyhedron,
                                    Voxels* out_voxels) const;
  void mark_triangle(const Triangle_3& triangle, Voxels* out_voxels) const;
  void mark_triangle_vertices(const Triangle_3& triangle,
                              Voxels* out_voxels) const;
  void mark_segment(const Segment_3& segment, Voxels* out_voxels) const;
  long most_steep_direction(const Segment_3& segment) const;
  void mark_point(const Point_3& point, Voxels* out_voxels) const;
  SlicingSegments create_slicing_segments(const Triangle_3& triangle,
                                          const Point_3& origin) const;
  void create_slicing_segments(long dim,
                               const Triangle_3& triangle,
                               const Point_3& origin,
                               SlicingSegments* out_slicing) const;
  void create_parallel_planes(const Point_3& pmin, const Point_3& pmax,
                              long dim, const Point_3& origin,
                              std::vector<Plane_3> *out_planes) const;
  Point_3 get_first_point_for_segment(const Point_3& psegment,
                                     const Point_3& porigin,
                                     long dim) const;
  void intersect_triangle_with_planes(const Triangle_3& triangle,
                                      const std::vector<Plane_3>& plane,
                                      SlicingSegments *out_segments) const;
  void intersect_segment_with_planes(const Segment_3& segment,
                                     const std::vector<Plane_3>&planes,
                                     std::vector<Point_3>* out_points) const;
  void fill_inside_of_polyhedron(Voxels* out_voxels) const;
  std::vector<long> get_voxels_coords(const Kernel::FT& ft) const;  

  Kernel::FT m_voxel_dimensions[3];
};


SGAL_END_NAMESPACE

#endif // SGAL_EGO_VOXELIZER_HPP


