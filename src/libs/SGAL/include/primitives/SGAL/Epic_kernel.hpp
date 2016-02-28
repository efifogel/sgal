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

#ifndef SCGAL_EPIC_KERNEL_HPP
#define SCGAL_EPIC_KERNEL_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/to_vector.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Exact_predicates_inexact_constructions_kernel Epic_kernel;

typedef Epic_kernel::RT                               Epic_RT;
typedef Epic_kernel::FT                               Epic_FT;

typedef Epic_kernel::Point_2                          Epic_point_2;
typedef Epic_kernel::Direction_2                      Epic_direction_2;

typedef Epic_kernel::Point_3                          Epic_point_3;
typedef Epic_kernel::Vector_3                         Epic_vector_3;
typedef Epic_kernel::Direction_3                      Epic_direction_3;
typedef Epic_kernel::Plane_3                          Epic_plane_3;
typedef Epic_kernel::Aff_transformation_3             Epic_aff_transformation_3;

/*! Convert a direction to Vector2f.
 */
template <>
inline Vector2f to_vector2f<Epic_direction_2>(const Epic_direction_2& dir)
{ return Vector2f(to_float(dir.dx()), to_float(dir.dy())); }

/*! Convert a direction to Vector3f.
 */
template <>
inline Vector3f to_vector3f<Epic_direction_3>(const Epic_direction_3& dir)
{
  return Vector3f(to_float(dir.dx()), to_float(dir.dy()), to_float(dir.dz()));
}

SGAL_END_NAMESPACE

#endif
