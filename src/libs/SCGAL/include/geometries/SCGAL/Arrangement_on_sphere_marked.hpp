// Copyright (c) 2009 Israel.
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
// $Id: 
// $Revision: 7205 $
//
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

/*!
  \file   Arrangement_on_sphere_marked.h
  \brief  Contains the geometric definition of an arrangement whose faces
          are marked.
*/


#ifndef SGAL_ARRANGEMENT_ON_SPHERE_MARKED_HPP
#define SGAL_ARRANGEMENT_ON_SPHERE_MARKED_HPP

#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_tracing_traits_2.h>

#include <CGAL/Gps_traits_2.h>

#include "SGAL/basic.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_mark_dcel.hpp"

SGAL_BEGIN_NAMESPACE

/*! 
  Internal namespace that there will not be any collision.
 */
namespace Arrangement_on_sphere_marked_internal {
  
//  We use Gps_face_base as the base of our face so we could use Boolean
//  set operations in the node Polygon_set_on_sphere_geo and still have
//  Polygon_set_on_sphere_geo inherit from the Arrangement_on_sphere_marked_geo.
//  This is a temporary solution that should be replaced with a better solution
//  that will allow decoupling of the DCEL from the Boolean set operations.
//  (like using BOOST property maps).
//  In the meantime, we use the Gps' "contain" flag instead of the "mark" flag.
  typedef CGAL::Gps_traits_2<
    CGAL::Arr_geodesic_arc_on_sphere_traits_2<Exact_kernel> >
                                                   Aos_geom_traits_base;

#if defined(CGAL_ARR_TRACING_TRAITS)
  typedef CGAL::Arr_tracing_traits_2<Aos_geom_traits_base>
                                                   Aos_geom_traits;
#else
  typedef Aos_geom_traits_base                     Aos_geom_traits;
#endif
  typedef CGAL::Arr_spherical_topology_traits_2
    <Aos_geom_traits, Arrangement_mark_dcel<Aos_geom_traits> >
                                                   Aos_topol_traits;
}

// The definition of the arrangment
typedef CGAL::Arrangement_on_surface_2<
  Arrangement_on_sphere_marked_internal::Aos_geom_traits, 
  Arrangement_on_sphere_marked_internal::Aos_topol_traits>
                                                   Arrangement_on_sphere_marked;

SGAL_END_NAMESPACE

#endif  // SGAL_ARRANGEMENT_ON_SPHERE_MARKED_HPP

