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
// $Id: Min_sphere.hpp 6624 2008-07-18 12:10:26Z efif $
// $Revision: 6624 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_MIN_SPHERE_HPP
#define SCGAL_MIN_SPHERE_HPP

#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "SGAL/basic.hpp"
#include "SCGAL/Approximate_kernel.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Min_sphere_of_spheres_d_traits_3<Approximate_kernel,
                                               Approximate_number_type>
                                                         Min_sphere_traits;
typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits> Min_sphere;
typedef Min_sphere_traits::Sphere                        Approximate_sphere_3;
 
typedef std::vector<Approximate_sphere_3>          Approximate_sphere_vector;
typedef Approximate_sphere_vector::const_iterator  Approximate_sphere_iter;

/* Convert an exact point into an approximate sphere */
template <typename T_Point>
inline Approximate_sphere_3 to_approximate_sphere(const T_Point & point)
{
  Approximate_point_3 approximate_point = to_approximate_point(point);
  return Approximate_sphere_3(approximate_point, 0.0);
}

SGAL_END_NAMESPACE

#endif
