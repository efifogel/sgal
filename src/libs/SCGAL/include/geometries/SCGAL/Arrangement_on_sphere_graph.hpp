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
  \file   Arrangement_on_sphere_graph.h
  \brief  Contains the geometric definition of an arrangement whose faces
          contain a boost graph.
*/


#ifndef SGAL_ARRANGEMENT_ON_SPHERE_GRAPH_HPP
#define SGAL_ARRANGEMENT_ON_SPHERE_GRAPH_HPP

#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_tracing_traits_2.h>

#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Arrangement_graph_dcel.hpp"

SGAL_BEGIN_NAMESPACE

/*! 
  Internal namespace that there will not be any collision.
 */
namespace Arrangement_on_sphere_graph_internal
{
  typedef CGAL::Arr_geodesic_arc_on_sphere_traits_2<Exact_kernel>
                                                    Aos_geom_traits_base;
#if defined(CGAL_ARR_TRACING_TRAITS)
  typedef CGAL::Arr_tracing_traits_2<Aos_geom_traits_base>
                                                    Aos_geom_traits;
#else
  typedef Aos_geom_traits_base                      Aos_geom_traits;
#endif
  typedef CGAL::Arr_spherical_topology_traits_2
    <Aos_geom_traits, Arrangement_graph_dcel<Aos_geom_traits> >
                                                    Aos_topol_graph_traits;
}

// The definition of the arrangment
typedef CGAL::Arrangement_on_surface_2<
  Arrangement_on_sphere_graph_internal::Aos_geom_traits,
  Arrangement_on_sphere_graph_internal::Aos_topol_graph_traits>
                                                    Arrangement_on_sphere_graph;

SGAL_END_NAMESPACE


#endif  // SGAL_ARRANGEMENT_ON_SPHERE_GRAPH_HPP

