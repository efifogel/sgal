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

#include "SEGO/Ego_voxelizer.hpp"

#include <CGAL/bounding_box.h>
#include <CGAL/squared_distance_3.h> 
#include <CGAL/Fraction_traits.h> 
#include <CGAL/Algebraic_structure_traits.h>
#include <CGAL/circulator.h>
#include <CGAL/intersections.h>

#include <boost/tuple/tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>

#include <functional>

#ifdef EGO_VOXELIZER_VERBOSE
#define VAR(x) #x << " " << x
#endif

SGAL_BEGIN_NAMESPACE

Ego_voxelizer::Ego_voxelizer(const Kernel::FT& voxel_length, const Kernel::FT& voxel_width,
                             const Kernel::FT& voxel_height) {
  m_voxel_dimensions[0] = voxel_length;
  m_voxel_dimensions[1] = voxel_width;
  m_voxel_dimensions[2] = voxel_height;
}

void Ego_voxelizer::operator() (const Polyhedron& polyhedron, Voxels* out_voxels) const {
  
  SGAL_assertion(out_voxels != NULL);
  
  // Actually, we might assum only convexity. But currently we have only this
  // assertion.
  SGAL_assertion(polyhedron.is_pure_triangle());

  create_voxels_from_polyhedron(polyhedron, out_voxels);

  for (Polyhedron::Facet_const_iterator it = polyhedron.facets_begin();
       it != polyhedron.facets_end(); ++it) {
    mark_facet(*it, out_voxels);
  }

  fill_inside_of_polyhedron(out_voxels);

#ifdef EGO_VOXELIZER_VERBOSE
  out_voxels->print();
#endif
}

void Ego_voxelizer::create_voxels_from_polyhedron(const Polyhedron& polyhedron,
                                                  Voxels* out_voxels) const {

  Iso_cuboid_3 bbox = CGAL::bounding_box(polyhedron.points_begin(),
                                         polyhedron.points_end());

  // make the length, width, and height integers that contain the 
  // mesh.
  // Not sure this is the best way to round up those puppies.
  
  Kernel::FT length = bbox.xmax() - bbox.xmin();
  Kernel::FT width = bbox.ymax() - bbox.ymin();
  Kernel::FT height = bbox.zmax() - bbox.zmin();
  
  long ilength = std::ceil(CGAL::to_double(length / m_voxel_dimensions[0]));
  long iwidth = std::ceil(CGAL::to_double(width / m_voxel_dimensions[1]));
  long iheight = std::ceil(CGAL::to_double(height / m_voxel_dimensions[2]));

  out_voxels->initialize_container(ilength, iwidth, iheight);

#ifdef EGO_VOXELIZER_VERBOSE
  std::cout << VAR(ilength) << std::endl;
  std::cout << VAR(iwidth) << std::endl;
  std::cout << VAR(iheight) << std::endl;
#endif

  out_voxels->origin = Kernel::Point_3(bbox.min_coord(0), bbox.min_coord(1),
                                       bbox.min_coord(2));
  

  // TODO: Do you want to uncomment this?
  // // We guess that putting the mesh exactly in the middle of the voxels
  // // bbox will produce better results right now.
  // Kernel::FT translate_x = (Kernel::FT(ilength) - length) / 2;
  // Kernel::FT translate_y = (Kernel::FT(iwidth) - width) / 2;
  // Kernel::FT translate_z = (Kernel::FT(iheight) - height) / 2;

  // Kernel::FT origin_x = bbox.min_coord(0) - translate_x;
  // Kernel::FT origin_y = bbox.min_coord(1) - translate_y;
  // Kernel::FT origin_z = bbox.min_coord(2) - translate_z;

  // out_voxels->origin = Kernel::Point_3(origin_x, origin_y, origin_z);
}

void Ego_voxelizer::mark_facet(const Facet& facet, Voxels* out_voxels) const {
  mark_facet_vertices(facet, out_voxels);

  SlicingSegments segments = create_slicing_segments(facet, out_voxels->origin);

  for (SlicingSegments::iterator it = segments.begin();
       it != segments.end(); ++it)
    mark_segment(*it, out_voxels);
}

void Ego_voxelizer::mark_facet_vertices(const Facet& facet,
                                        Voxels* out_voxels) const {
  Halfedge_around_facet_const_circulator cit = facet.facet_begin();
  Halfedge_around_facet_const_circulator endit = cit;
  do {
    mark_point(cit->vertex()->point(), out_voxels);
  } while (++cit != endit);
}

void Ego_voxelizer::mark_segment(const Segment_3& segment,
                                 Voxels* out_voxels) const {

  // This can be done with a walk. Currenly we do simple geometric 
  // stuff (that already exist). If you want optimizing, consider walk.
  // We use the most steep dimention to intersect the segment. If you
  // use the most steep direction it is enough.

  long dim = most_steep_direction(segment);

  std::vector<Plane_3> planes;
  create_parallel_planes(segment.min(), segment.max(), dim, out_voxels->origin, &planes);
  
  std::vector<Point_3> points;
  intersect_segment_with_planes(segment, planes, &points);
  
  BOOST_FOREACH(Point_3 p, points) {
    mark_point(p, out_voxels);
  }

  // It could be that one of those is not needed.
  mark_point(segment.source(), out_voxels);
  mark_point(segment.target(), out_voxels);
}

long Ego_voxelizer::most_steep_direction(const Segment_3& segment) const {
  Kernel::FT x = CGAL::abs(segment.to_vector().x());
  Kernel::FT y = CGAL::abs(segment.to_vector().y());
  Kernel::FT z = CGAL::abs(segment.to_vector().z());

  if (x >= y && x >= z)
    return 0;
  
  if (y >= z)
    return 1;

  return 2;
}

void
Ego_voxelizer::mark_point(const Point_3& point, Voxels* out_voxels) const {

  Point_3 in_voxel = CGAL::ORIGIN + (point - out_voxels->origin);

  Kernel::FT ft_x = in_voxel.x() / m_voxel_dimensions[0];
  Kernel::FT ft_y = in_voxel.y() / m_voxel_dimensions[1];
  Kernel::FT ft_z = in_voxel.z() / m_voxel_dimensions[2];

  // Point can be on a boundary (where we have to mark two squares).
  // The following functions can push_back upto two values.
  std::vector<long> x_coord = get_voxels_coords(ft_x);
  std::vector<long> y_coord = get_voxels_coords(ft_y);
  std::vector<long> z_coord = get_voxels_coords(ft_z);

  // We mark upto 8 voxels.
  for (std::vector<long>::iterator it = x_coord.begin();
       it != x_coord.end(); ++it)
    for (std::vector<long>::iterator jt = y_coord.begin();
         jt != y_coord.end(); ++jt)
      for (std::vector<long>::iterator kt = z_coord.begin();
           kt != z_coord.end(); ++kt)
        out_voxels->mark(*it, *jt, *kt);
}

Ego_voxelizer::SlicingSegments
Ego_voxelizer::create_slicing_segments(const Facet& facet,
                                       const Point_3& origin) const {
  
  // We assume here that the facet is convex.
  // We create here segments in all 3 direction, though I am quite sure
  // this can be reduced.
  SlicingSegments out_slicing;
  create_slicing_segments(0, facet, origin, &out_slicing);
  create_slicing_segments(1, facet, origin, &out_slicing);
  create_slicing_segments(2, facet, origin, &out_slicing);
  
  return out_slicing;
}

void Ego_voxelizer::create_slicing_segments(long dim, const Facet& facet,
                                            const Point_3& origin,
                                            SlicingSegments* out_slicing) const {
  // Assume triangles.
  // This can be done by a walk, but this is easier.

  Halfedge_around_facet_const_circulator cit = facet.facet_begin();
  Point_3 points[3];
  points[0] = cit->vertex()->point(); cit++;
  points[1] = cit->vertex()->point(); cit++;
  points[2] = cit->vertex()->point(); cit++;

  Point_3* pmax = std::max_element(points, points + 3,
                                   SEGO_internal::Compare_point_by_axis<Point_3>(dim));
  Point_3* pmin = std::min_element(points, points + 3,
                                   SEGO_internal::Compare_point_by_axis<Point_3>(dim));
  
  std::vector<Plane_3> planes;
  create_parallel_planes(*pmin, *pmax, dim, origin, &planes);
  
  Triangle_3 triangle(points[0], points[1], points[2]);
  intersect_triangle_with_planes(triangle, planes, out_slicing);
}

void Ego_voxelizer::create_parallel_planes(const Point_3& pmin,
                                           const Point_3& pmax,
                                           long dim, const Point_3& origin,
                                           std::vector<Plane_3> *out_planes) const {
  Kernel::FT dir_ft[3] = {0, 0, 0};
  dir_ft[dim] = 1;
  Direction_3 dir(dir_ft[0], dir_ft[1], dir_ft[2]);

  Kernel::FT ad_ft[3] = {0, 0, 0};
  ad_ft[dim] = m_voxel_dimensions[dim];
  Vector_3 advance(ad_ft[0], ad_ft[1], ad_ft[2]);

  Point_3 cur_point = get_first_point_for_segment(pmin, origin, dim);

  while (cur_point[dim] <= pmax[dim]) {
    out_planes->push_back(Plane_3(cur_point, dir));
    cur_point = cur_point + advance;
  }
}

Ego_voxelizer::Point_3 Ego_voxelizer::get_first_point_for_segment(const Point_3& psegment,
                                                                  const Point_3& porigin,
                                                                  long dim) const {

  typedef CGAL::Fraction_traits<Kernel::FT>            Fraction_traits;
  typedef Fraction_traits::Numerator_type              Numerator_type;
  typedef Fraction_traits::Denominator_type            Denominator_type;
  typedef CGAL::Algebraic_structure_traits<Numerator_type> 
    Algebraic_structure_traits;

  Kernel::FT begin_ft = (psegment - porigin)[dim] / m_voxel_dimensions[dim];

  Fraction_traits::Decompose decompose;
  Algebraic_structure_traits::Div_mod div;

  Numerator_type nume;
  Denominator_type denom;
  Algebraic_structure_traits::Type div_res, mod_res;
 
  decompose(begin_ft, nume, denom);
  div(nume, denom, div_res, mod_res);

  if (mod_res != 0)
    div_res = div_res + 1 * CGAL::sign(nume) * CGAL::sign(denom);

  SGAL_assertion(porigin[dim] + div_res * m_voxel_dimensions[dim] >= psegment[dim]);

  Kernel::FT res_ft[3] = {0, 0, 0};
  res_ft[dim] = porigin[dim] + div_res * m_voxel_dimensions[dim];
  Point_3 res(res_ft[0], res_ft[1], res_ft[2]);

  return res;
}


void Ego_voxelizer::intersect_triangle_with_planes(const Triangle_3& triangle,
                                                   const std::vector<Plane_3>& planes,
                                                   SlicingSegments *out_segments) const {

  BOOST_FOREACH(Plane_3 plane, planes) {
    CGAL::Object obj = CGAL::intersection(triangle, plane);
    
    const Segment_3* seg = CGAL::object_cast<Segment_3> (&obj);
    
    SGAL_assertion(obj.empty() == false);
    SGAL_assertion((CGAL::object_cast<Point_3> (&obj) != NULL) || 
                   (seg != NULL) || 
                   (CGAL::object_cast<Triangle_3> (&obj) != NULL));
    
    if (seg != NULL)
      out_segments->push_back(*seg);
  }
}

void
Ego_voxelizer::intersect_segment_with_planes(const Segment_3& segment,
                                             const std::vector<Plane_3>& planes,
                                             std::vector<Point_3>* out_points) const {

  BOOST_FOREACH(Plane_3 plane, planes) {
    CGAL::Object obj = CGAL::intersection(segment, plane);
    const Point_3* p = CGAL::object_cast<Point_3> (&obj);

    SGAL_assertion((CGAL::object_cast<Segment_3>(&obj) != NULL) || (p != NULL));
    
    if (p != NULL)
      out_points->push_back(*p);
  }
}

/** 
 * Currently does nothing, but generally should fill the inside of the
 * model with voxel.
 * 
 * @param out_voxels 
 */
void Ego_voxelizer::fill_inside_of_polyhedron(Voxels* out_voxels) const {
}

std::vector<long>
Ego_voxelizer::get_voxels_coords(const Kernel::FT& ft) const {

  typedef CGAL::Fraction_traits<Kernel::FT>            Fraction_traits;
  typedef Fraction_traits::Numerator_type              Numerator_type;
  typedef Fraction_traits::Denominator_type            Denominator_type;

  typedef CGAL::Algebraic_structure_traits<Numerator_type> 
    Algebraic_structure_traits;
  typedef Algebraic_structure_traits::Div_mod          Div_mod;

  std::vector<long> out_vec;
  
  Fraction_traits::Decompose decompose;
  Div_mod div;

  Numerator_type nume;
  Denominator_type denom;
  Algebraic_structure_traits::Type div_res, mod_res;
 
  // How do we convert Gmpz to long?
  decompose(ft, nume, denom);
  div(nume, denom, div_res, mod_res);
  out_vec.push_back(CGAL::to_double(div_res));
  if (mod_res == 0 && div_res > 0) {
    // We are on a border.
    out_vec.push_back(CGAL::to_double(div_res - 1));
  }

  return out_vec;
}

void
Ego_voxelizer::Voxels::initialize_container(long length,
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

void Ego_voxelizer::Voxels::mark(size_t x, size_t y, size_t z) {
  // We can also get boundary coordinates...

  if (x == voxels.size()) --x;
  if (y == voxels[0].size()) --y;
  if (z == voxels[0][0].size()) --z;

  SGAL_assertion(x < voxels.size());
  SGAL_assertion(y < voxels[0].size());
  SGAL_assertion(z < voxels[0][0].size());
  
  voxels[x][y][z] = true;    
}

void Ego_voxelizer::Voxels::print() const {

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
