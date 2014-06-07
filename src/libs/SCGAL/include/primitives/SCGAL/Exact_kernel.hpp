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

#ifndef SCGAL_EXACT_KERNEL_HPP
#define SCGAL_EXACT_KERNEL_HPP

#define SCGAL_CARTESIAN_KERNEL                  0
#define SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL      1
#define SCGAL_HOMOGENEOUS_KERNEL                2

#if SCGAL_KERNEL == SCGAL_CARTESIAN_KERNEL
#include <CGAL/Cartesian.h>

#elif SCGAL_KERNEL == SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL
#include <CGAL/Cartesian.h>
#include <CGAL/Lazy_kernel.h>

#elif SCGAL_KERNEL == SCGAL_HOMOGENEOUS_KERNEL
#include <CGAL/Homogeneous.h>

#else
#error No Kernel (SCGAL_KERNEL) specified!

#endif

#include "SCGAL/Exact_number_type.hpp"
#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

#if SCGAL_KERNEL == SCGAL_CARTESIAN_KERNEL
typedef CGAL::Cartesian<Exact_number_type>              Exact_kernel;

#elif SCGAL_KERNEL == SCGAL_LAZY_SIMPLE_CARTESIAN_KERNEL
typedef CGAL::Lazy_kernel<CGAL::Simple_cartesian<Exact_number_type> >
                                                        Exact_kernel;

#elif SCGAL_KERNEL == SCGAL_HOMOGENEOUS_KERNEL
typedef CGAL::Homogeneous<Exact_number_type>            Exact_kernel;

#else
#error Invalid kernel!

#endif

typedef Exact_kernel::RT                                Exact_RT;
typedef Exact_kernel::FT                                Exact_FT;

typedef Exact_kernel::Point_2                           Exact_point_2;
typedef Exact_kernel::Direction_2                       Exact_direction_2;

typedef Exact_kernel::Point_3                           Exact_point_3;
typedef Exact_kernel::Vector_3                          Exact_vector_3;
typedef Exact_kernel::Direction_3                       Exact_direction_3;
typedef Exact_kernel::Plane_3                           Exact_plane_3;

/* Convert a point into Vector3f */
template <typename Point>
inline Vector3f to_vector3f(const Point& point)
{
  return Vector3f(static_cast<float>(CGAL::to_double(point.x())),
                  static_cast<float>(CGAL::to_double(point.y())),
                  static_cast<float>(CGAL::to_double(point.z())));
}

/* Convert a direction into Vector3f */
template <>
inline Vector3f to_vector3f<Exact_direction_3>(const Exact_direction_3& dir)
{
  return Vector3f(static_cast<float>(CGAL::to_double(dir.dx())),
                  static_cast<float>(CGAL::to_double(dir.dy())),
                  static_cast<float>(CGAL::to_double(dir.dz())));
}

/* Convert a point into Vector2f */
template <typename Point>
inline Vector2f to_vector2f(const Point & point)
{
  return Vector2f(static_cast<float>(CGAL::to_double(point.x())),
                  static_cast<float>(CGAL::to_double(point.y())));
}

/* Convert a direction into Vector2f */
template <>
inline Vector2f to_vector2f<Exact_direction_2>(const Exact_direction_2& dir)
{
  return Vector2f(static_cast<float>(CGAL::to_double(dir.dx())),
                  static_cast<float>(CGAL::to_double(dir.dy())));
}

SGAL_END_NAMESPACE

#endif
