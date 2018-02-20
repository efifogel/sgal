// Copyright (c) 2004 Israel.
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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_MIN_SPHERE_HPP
#define SCGAL_MIN_SPHERE_HPP

#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Min_sphere_of_spheres_d_traits_3<Inexact_kernel,
                                               Inexact_number_type>
                                                         Min_sphere_traits;
typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits> Min_sphere;
typedef Min_sphere_traits::Sphere                        Inexact_sphere_3;

typedef std::vector<Inexact_sphere_3>          Inexact_sphere_vector;
typedef Inexact_sphere_vector::const_iterator  Inexact_sphere_iter;

/* Convert an exact point into an approximate point */
template <typename Point_>
inline Inexact_point_3 to_inexact_point(const Point_& point)
{
  return Inexact_point_3(static_cast<float>(CGAL::to_double(point.x())),
                         static_cast<float>(CGAL::to_double(point.y())),
                         static_cast<float>(CGAL::to_double(point.z())));
}

/*! Convert an exact point into an approximate sphere. */
template <typename T_Point>
inline Inexact_sphere_3 to_inexact_sphere(const T_Point& point)
{
  Inexact_point_3 inexact_point = to_inexact_point(point);
  return Inexact_sphere_3(inexact_point, 0.0f);
}

SGAL_END_NAMESPACE

#endif
