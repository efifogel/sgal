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
// $Id: Approximate_kernel.hpp 6624 2008-07-18 12:10:26Z efif $
// $Revision: 6624 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_APPROXIMATE_KERNEL_HPP
#define SCGAL_APPROXIMATE_KERNEL_HPP

#include <CGAL/Cartesian.h>

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

typedef float                                         Approximate_number_type;
typedef CGAL::Cartesian<Approximate_number_type>      Approximate_kernel;

typedef Approximate_kernel::RT                        Approximate_RT;
typedef Approximate_kernel::FT                        Approximate_FT;
typedef Approximate_kernel::Point_3                   Approximate_point_3;
typedef Approximate_kernel::Vector_3                  Approximate_vector_3;
typedef Approximate_kernel::Direction_3               Approximate_direction_3;
typedef Approximate_kernel::Plane_3                   Approximate_plane_3;

/* Convert an exact point into an approximate point */
template <typename T_Point>
inline Approximate_point_3 to_approximate_point(const T_Point & point)
{
  return Approximate_point_3(static_cast<float>(CGAL::to_double(point.x())),
                             static_cast<float>(CGAL::to_double(point.y())),
                             static_cast<float>(CGAL::to_double(point.z())));
}

SGAL_END_NAMESPACE

#endif
