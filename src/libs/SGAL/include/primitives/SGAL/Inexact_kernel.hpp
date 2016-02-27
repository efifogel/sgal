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

#ifndef SCGAL_INEXACT_KERNEL_HPP
#define SCGAL_INEXACT_KERNEL_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/to_vector.hpp"

SGAL_BEGIN_NAMESPACE

typedef float                                   Inexact_number_type;
typedef CGAL::Cartesian<Inexact_number_type>    Inexact_kernel;

typedef Inexact_kernel::Point_2                 Inexact_point_2;
typedef Inexact_kernel::Direction_2             Inexact_direction_2;

typedef Inexact_kernel::RT                      Inexact_RT;
typedef Inexact_kernel::FT                      Inexact_FT;
typedef Inexact_kernel::Point_3                 Inexact_point_3;
typedef Inexact_kernel::Vector_3                Inexact_vector_3;
typedef Inexact_kernel::Direction_3             Inexact_direction_3;
typedef Inexact_kernel::Plane_3                 Inexact_plane_3;

typedef CGAL::Polygon_2<Inexact_kernel>         Inexact_polygon;

/*! Convert a point into Vector2f. */
template <>
inline Vector2f to_vector2f<Inexact_point_2>(const Inexact_point_2& point)
{ return Vector2f(point.x(), point.y()); }

/*! Convert a direction into Vector2f. */
template <>
inline Vector2f to_vector2f<Inexact_direction_2>(const Inexact_direction_2& dir)
{ return Vector2f(dir.dx(), dir.dy()); }

/*! Convert a point into Vector3f. */
template <>
inline Vector3f to_vector3f<Inexact_point_3>(const Inexact_point_3& point)
{ return Vector3f(point.x(), point.y(), point.z()); }

/*! Convert a direction into Vector3f. */
template <>
inline Vector3f to_vector3f<Inexact_direction_3>(const Inexact_direction_3& dir)
{ return Vector3f(dir.dx(), dir.dy(), dir.dz()); }

SGAL_END_NAMESPACE

#endif
