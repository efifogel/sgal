// Copyright (c) 2007 Israel.
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
// $Id: Arrangement_extended_direction.hpp 6681 2008-07-27 12:01:54Z efif $
// $Revision: 6681 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_EXTENDED_DIRECTION_HPP
#define SGAL_ARRANGEMENT_EXTENDED_DIRECTION_HPP

#include <CGAL/basic.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

/* Convert a direction into Vector3f */
typedef CGAL::Arr_extended_direction_3<Exact_kernel>  Arr_extended_direction_3;

template <>
inline Vector3f
to_vector3f<Arr_extended_direction_3>(const Arr_extended_direction_3 & dir)
{
  const Exact_direction_3 & exact_dir = dir;
  return to_vector3f(exact_dir);
}

SGAL_END_NAMESPACE

#endif
