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
// $Id: Arrangement_on_sphere_labeled.hpp 7789 2009-07-19 12:40:30Z efif $
// $Revision: 7789 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_ON_SPHERE_LABELED_HPP
#define SGAL_ARRANGEMENT_ON_SPHERE_LABELED_HPP

#include <CGAL/basic.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_tracing_traits_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_label_dcel.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Arr_geodesic_arc_on_sphere_traits_2<Exact_kernel>
                                                Aos_labeled_geom_traits_base;
#if defined(CGAL_ARR_TRACING_TRAITS)
typedef CGAL::Arr_tracing_traits_2<Aos_labeled_geom_traits_base>
                                                Aos_labeled_geom_traits;
#else
typedef Aos_labeled_geom_traits_base            Aos_labeled_geom_traits;
#endif
typedef Arrangement_label_dcel<Aos_labeled_geom_traits>
                                                Aos_labeled_dcel;
typedef CGAL::
Arr_spherical_topology_traits_2<Aos_labeled_geom_traits,
                                Aos_labeled_dcel>
                                                Aos_labeled_topol_traits;
typedef CGAL::Arrangement_on_surface_2<Aos_labeled_geom_traits,
                                       Aos_labeled_topol_traits>
                                                Arrangement_on_sphere_labeled;

SGAL_END_NAMESPACE

#endif
