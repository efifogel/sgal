// Copyright (c) 2014 Israel.
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

#ifndef SCGAL_ARRANGEMENT_ON_TORUS_HPP
#define SCGAL_ARRANGEMENT_ON_TORUS_HPP

#include <CGAL/basic.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arr_flat_torus_traits_2.h>
#include <CGAL/Arr_tracing_traits_2.h>
#include <CGAL/Arr_toroidal_topology_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Epec_kernel.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Arr_flat_torus_traits_2<Epec_kernel>      Aot_geom_traits_base;
#if defined(CGAL_ARR_TRACING_TRAITS)
typedef CGAL::Arr_tracing_traits_2<Aot_geom_traits_base>
                                                        Aot_geom_traits;
#else
typedef Aot_geom_traits_base                            Aot_geom_traits;
#endif
typedef CGAL::Arr_toroidal_topology_traits_2<Aot_geom_traits>
                                                        Aot_topol_traits;
typedef CGAL::Arrangement_on_surface_2<Aot_geom_traits, Aot_topol_traits>
                                                        Arrangement_on_torus;

SGAL_END_NAMESPACE

#endif
