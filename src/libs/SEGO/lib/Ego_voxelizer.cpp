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
#include "SEGO/Ego_voxels.hpp"

#include <CGAL/bounding_box.h>
#include <CGAL/squared_distance_3.h> 
#include <CGAL/Fraction_traits.h> 
#include <CGAL/Algebraic_structure_traits.h>
#include <CGAL/circulator.h>
#include <CGAL/intersections.h>

#include <boost/foreach.hpp>

#include <functional>

// #define EGO_VOXELIZER_VERBOSE
#ifdef EGO_VOXELIZER_VERBOSE
#define VAR(x) #x << " " << x
#endif

SGAL_BEGIN_NAMESPACE

/*! \brief Constructor */
Ego_voxelizer::Ego_voxelizer(const Kernel::FT& voxel_length,
                             const Kernel::FT& voxel_width,
                             const Kernel::FT& voxel_height)
{
  m_voxel_dimensions[0] = voxel_length;
  m_voxel_dimensions[1] = voxel_width;
  m_voxel_dimensions[2] = voxel_height;
}

/*! \brief voxelize */
Ego_voxelizer::Point_3
Ego_voxelizer::operator()(const Polyhedron& polyhedron, const Matrix4f& matrix,
                          Ego_voxels* out_voxels) const
{
  SGAL_assertion(polyhedron.is_pure_triangle());
  
  Triangles triangles = create_triangles_from_polyhedron(polyhedron, matrix);
  return operator() (triangles, out_voxels);
}

/*! \brief voxelize */
Ego_voxelizer::Point_3
Ego_voxelizer::operator()(const Exact_polyhedron& polyhedron,
                          const Matrix4f& matrix, Ego_voxels* out_voxels) const
{
  // Later we might assume only convexity.
  SGAL_assertion(polyhedron.is_pure_triangle());
  Triangles triangles = create_triangles_from_polyhedron(polyhedron, matrix);
  return operator() (triangles, out_voxels);
}

/*! \brief voxelize */
Ego_voxelizer::Point_3
Ego_voxelizer::operator()(Mesh_set& mesh_set, const Matrix4f& matrix,
                          Ego_voxels* out_voxels) const
{
  Triangles triangles = create_triangles_from_mesh_set(mesh_set, matrix);
  return operator() (triangles, out_voxels);
}

/*! \brief voxelize */
Ego_voxelizer::Point_3
Ego_voxelizer::operator()(const Triangles& triangles, Ego_voxels* out_voxels)
  const
{
  SGAL_assertion(out_voxels != NULL);

  Iso_cuboid_3 bbox;
  create_voxels_from_triangles(triangles, out_voxels, bbox);
  Point_3 origin(bbox.min_coord(0), bbox.min_coord(1), bbox.min_coord(2));
  Triangles::const_iterator it;
  for (it = triangles.begin(); it != triangles.end(); ++it)
    mark_triangle(*it, origin, out_voxels);

#ifdef EGO_VOXELIZER_VERBOSE
  out_voxels->print();
#endif

  Point_3 center((bbox.min_coord(0)+bbox.max_coord(0)) / 2,
                 (bbox.min_coord(1)+bbox.max_coord(1)) / 2,
                 (bbox.min_coord(2)+bbox.max_coord(2)) / 2);
  return center;
}

/*! \brief */
Ego_voxelizer::Triangles
Ego_voxelizer::create_triangles_from_polyhedron(const Polyhedron& polyhedron,
                                                const Matrix4f& matrix) const
{
  Triangles ret;

  for (Polyhedron::Facet_const_iterator it = polyhedron.facets_begin();
       it != polyhedron.facets_end(); ++it)
  {
    Polyhedron::Halfedge_around_facet_const_circulator cit = it->facet_begin();
    Point_3 points[3];
    Vector3f point, tmp;
    tmp.set(cit->vertex()->point().x(), cit->vertex()->point().y(),
            cit->vertex()->point().z());
    point.xform_pt(tmp, matrix);
    points[0] = Point_3(Kernel::FT(point[0]), Kernel::FT(point[1]),
                        Kernel::FT(point[2]));
    ++cit;
    tmp.set(cit->vertex()->point().x(), cit->vertex()->point().y(),
            cit->vertex()->point().z());
    point.xform_pt(tmp, matrix);
    points[1] = Point_3(Kernel::FT(point[0]), Kernel::FT(point[1]),
                        Kernel::FT(point[2]));
    ++cit;
    tmp.set(cit->vertex()->point().x(), cit->vertex()->point().y(),
            cit->vertex()->point().z());
    point.xform_pt(tmp, matrix);
    points[2] = Point_3(Kernel::FT(point[0]), Kernel::FT(point[1]),
                        Kernel::FT(point[2]));
    ++cit;
    Triangle_3 triangle(points[0], points[1], points[2]);
    ret.push_back(triangle);
  }

  return ret;
}

/*! \brief */
Ego_voxelizer::Triangles
Ego_voxelizer::
create_triangles_from_polyhedron(const Exact_polyhedron& polyhedron,
                                 const Matrix4f& matrix) const
{
  Triangles ret;

  // for (Exact_polyhedron::Facet_const_iterator it = polyhedron.facets_begin();
  //      it != polyhedron.facets_end(); ++it) {
    
  //   Exact_polyhedron::Halfedge_around_facet_const_circulator cit =
  //     it->facet_begin();
  //   Point_3 points[3];
  //   if (!matrix.is_identity()) {
  //     // EFEF: avoid construction of the kernel. Obtain it from somewhere. 
  //     Kernel kernel;
  //     Kernel::Construct_scaled_vector_3 ctr_scale = 
  //       kernel.construct_scaled_vector_3_object();
  //     Kernel::Construct_vector_3 ctr_vector = 
  //       kernel.construct_vector_3_object();
  //     Kernel::Construct_translated_point_3 ctr_point =
  //       kernel.construct_translated_point_3_object();

  //     Kernel::RT escale = Kernel::FT(scale);
  //     Kernel::Vector_3 vec = ctr_vector(cit->vertex()->point(), CGAL::ORIGIN);
  //     Kernel::Vector_3 scaled_vec = ctr_scale(vec, escale);

  //     vec = ctr_vector(cit->vertex()->point(), CGAL::ORIGIN);
  //     scaled_vec = ctr_scale(vec, escale);
  //     points[0] = ctr_point(CGAL::ORIGIN, scaled_vec);
  //     ++cit;
  //     vec = ctr_vector(cit->vertex()->point(), CGAL::ORIGIN);
  //     scaled_vec = ctr_scale(vec, escale);
  //     points[1] = ctr_point(CGAL::ORIGIN, scaled_vec);
  //     ++cit;
  //     vec = ctr_vector(cit->vertex()->point(), CGAL::ORIGIN);
  //     scaled_vec = ctr_scale(vec, escale);
  //     points[2] = ctr_point(CGAL::ORIGIN, scaled_vec);
  //     ++cit;
  //   }
  //   else {
  //     points[0] = cit->vertex()->point(); ++cit;
  //     points[1] = cit->vertex()->point(); ++cit;
  //     points[2] = cit->vertex()->point(); ++cit;
  //   }
    
  //   Triangle_3 triangle(points[0], points[1], points[2]);
  //   ret.push_back(triangle);
  // }

  return ret;
}

/*! \brief */
Ego_voxelizer::Triangles
Ego_voxelizer::create_triangles_from_mesh_set(Mesh_set& mesh_set,
                                              const Matrix4f& matrix) const
{
  Triangles res;
  
  const SGAL::Array<Uint>& coord_ind = mesh_set.get_coord_indices();
  const Coord_array& coords = *(mesh_set.get_coord_array());

  Point_3 triangle[3];
  long j = 0;
  for (SGAL::Array<Uint>::const_iterator it = coord_ind.begin();
       it != coord_ind.end(); ++it, ++j)
  {
    if (j == 3) j = 0;

    SGAL_assertion(*it < coords.size());

    Vector3f tmp(coords[*it][0], coords[*it][1], coords[*it][2]);
    Vector3f point;
    point.xform_pt(tmp, matrix);
    triangle[j] = Point_3(Kernel::FT(point[0]), Kernel::FT(point[1]),
                          Kernel::FT(point[2]));
    if (j == 2)
      res.push_back(Triangle_3(triangle[0], triangle[1], triangle[2]));
  }
  
  return res;
}

/*! \brief */
void Ego_voxelizer::create_voxels_from_triangles(const Triangles& triangles,
                                                 Ego_voxels* out_voxels,
                                                 Iso_cuboid_3& bbox) const
{
  std::vector<Point_3> vertices;
  Triangles::const_iterator it;
  for (it = triangles.begin(); it != triangles.end(); ++it) {
    vertices.push_back(it->vertex(0));
    vertices.push_back(it->vertex(1));
    vertices.push_back(it->vertex(2));
  }
  bbox = CGAL::bounding_box(vertices.begin(), vertices.end());

  // make the length, width, and height integers that contain the mesh.
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

/*! \brief */
void Ego_voxelizer::mark_triangle(const Triangle_3& triangle,
                                  const Point_3& origin,
                                  Ego_voxels* out_voxels) const
{
  mark_triangle_vertices(triangle, origin, out_voxels);
  SlicingSegments segments = create_slicing_segments(triangle, origin);
  for (SlicingSegments::iterator it = segments.begin();
       it != segments.end(); ++it)
    mark_segment(*it, origin, out_voxels);
}

/*! \brief */
void Ego_voxelizer::mark_triangle_vertices(const Triangle_3& triangle,
                                           const Point_3& origin,
                                           Ego_voxels* out_voxels) const
{
  mark_point(triangle[0], origin, out_voxels);
  mark_point(triangle[1], origin, out_voxels);
  mark_point(triangle[2], origin, out_voxels);
}

/*! \brief */
void Ego_voxelizer::mark_segment(const Segment_3& segment,
                                 const Point_3& origin,
                                 Ego_voxels* out_voxels) const
{
  // This can be done with a walk. Currenly we do simple geometric 
  // stuff (that already exist). If you want optimizing, consider walk.
  std::vector<Plane_3> planes;
  for (long dim = 0; dim < 3; ++dim) {
    SEGO_internal::Compare_point_by_axis<Point_3> comp(dim);
    Point_3 pmin, pmax;
    if (comp(segment.source(), segment.target())) {
      pmin = segment.source();
      pmax = segment.target();
    }
    else {
      pmin = segment.target();
      pmax = segment.source();
    }
    
    create_parallel_planes(pmin, pmax, dim, origin, &planes);
  }
  
  std::vector<Point_3> points;
  intersect_segment_with_planes(segment, planes, &points);
  
  BOOST_FOREACH(Point_3 p, points) {
    mark_point(p, origin, out_voxels);
  }
  
  // It could be that one of those is not needed.
  mark_point(segment.source(), origin, out_voxels);
  mark_point(segment.target(), origin, out_voxels);
}

/*! \brief */
void Ego_voxelizer::mark_point(const Point_3& point,
                               const Point_3& origin,
                               Ego_voxels* out_voxels) const
{
  Point_3 in_voxel = CGAL::ORIGIN + (point - origin);

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
        out_voxels->fill(*it, *jt, *kt);
}

/*! \brief */
Ego_voxelizer::SlicingSegments
Ego_voxelizer::create_slicing_segments(const Triangle_3& triangle,
                                       const Point_3& origin) const
{
  // We assume here that the facet is convex.
  // We create here segments in all 3 direction, though I am quite sure
  // this can be reduced.
  SlicingSegments out_slicing;
  create_slicing_segments(0, triangle, origin, &out_slicing);
  create_slicing_segments(1, triangle, origin, &out_slicing);
  create_slicing_segments(2, triangle, origin, &out_slicing);
  return out_slicing;
}

/*! \brief */
void Ego_voxelizer::create_slicing_segments(long dim,
                                            const Triangle_3& triangle,
                                            const Point_3& origin,
                                            SlicingSegments* out_slicing) const
{
  Point_3 points[3];
  points[0] = triangle[0];
  points[1] = triangle[1];
  points[2] = triangle[2];

  Point_3* pmax =
    std::max_element(points, points + 3,
                     SEGO_internal::Compare_point_by_axis<Point_3>(dim));
  Point_3* pmin =
    std::min_element(points, points + 3,
                     SEGO_internal::Compare_point_by_axis<Point_3>(dim));
  
  std::vector<Plane_3> planes;
  create_parallel_planes(*pmin, *pmax, dim, origin, &planes);
  intersect_triangle_with_planes(triangle, planes, out_slicing);
}

/*! \brief */
void Ego_voxelizer::create_parallel_planes(const Point_3& pmin,
                                           const Point_3& pmax,
                                           long dim, const Point_3& origin,
                                           std::vector<Plane_3> *out_planes)
  const
{
  SGAL_assertion(pmin[dim] <= pmax[dim]);
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

/*! \brief */
Ego_voxelizer::Point_3
Ego_voxelizer::get_first_point_for_segment(const Point_3& psegment,
                                           const Point_3& porigin,
                                           long dim) const
{
  typedef CGAL::Fraction_traits<Kernel::FT>        Fraction_traits;
  typedef Fraction_traits::Numerator_type          Numerator_type;
  typedef Fraction_traits::Denominator_type        Denominator_type;
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
  if (mod_res != 0) div_res = div_res + 1;
  SGAL_assertion(porigin[dim] + div_res * m_voxel_dimensions[dim] >=
                 psegment[dim]);
  Kernel::FT res_ft[3] = {0, 0, 0};
  res_ft[dim] = porigin[dim] + div_res * m_voxel_dimensions[dim];
  Point_3 res(res_ft[0], res_ft[1], res_ft[2]);
  return res;
}

/*! \brief */
void Ego_voxelizer::
intersect_triangle_with_planes(const Triangle_3& triangle,
                               const std::vector<Plane_3>& planes,
                               SlicingSegments* out_segments) const
{
  BOOST_FOREACH(Plane_3 plane, planes) {
    CGAL::Object obj = CGAL::intersection(triangle, plane);
    const Segment_3* seg = CGAL::object_cast<Segment_3> (&obj);
    SGAL_assertion(obj.empty() == false);
    SGAL_assertion((CGAL::object_cast<Point_3> (&obj) != NULL) || 
                   (seg != NULL) || 
                   (CGAL::object_cast<Triangle_3> (&obj) != NULL));
    if (seg != NULL) out_segments->push_back(*seg);
  }
}

/*! \brief */
void
Ego_voxelizer::intersect_segment_with_planes(const Segment_3& segment,
                                             const std::vector<Plane_3>& planes,
                                             std::vector<Point_3>* out_points)
  const
{
  BOOST_FOREACH(Plane_3 plane, planes) {
    CGAL::Object obj = CGAL::intersection(segment, plane);
    const Point_3* p = CGAL::object_cast<Point_3> (&obj);
    SGAL_assertion((CGAL::object_cast<Segment_3>(&obj) != NULL) || (p != NULL));
    if (p != NULL) out_points->push_back(*p);
  }
}

std::vector<long>
Ego_voxelizer::get_voxels_coords(const Kernel::FT& ft) const
{
  typedef CGAL::Fraction_traits<Kernel::FT>        Fraction_traits;
  typedef Fraction_traits::Numerator_type          Numerator_type;
  typedef Fraction_traits::Denominator_type        Denominator_type;

  typedef CGAL::Algebraic_structure_traits<Numerator_type> 
                                                   Algebraic_structure_traits;
  typedef Algebraic_structure_traits::Div_mod      Div_mod;

  std::vector<long> out_vec;
  Fraction_traits::Decompose decompose;
  Div_mod div;
  Numerator_type nume;
  Denominator_type denom;
  Algebraic_structure_traits::Type div_res, mod_res;
 
  // How do we convert Gmpz to long?
  decompose(ft, nume, denom);
  div(nume, denom, div_res, mod_res);
  long res = long(CGAL::to_double(div_res));
  SGAL_assertion(res == div_res);
  out_vec.push_back(res);
  if (mod_res == 0 && res > 0) out_vec.push_back(res - 1); // We are on a border
  return out_vec;
}

SGAL_END_NAMESPACE
