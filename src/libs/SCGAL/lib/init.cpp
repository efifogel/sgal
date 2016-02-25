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

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4390 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <boost/extension/extension.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Boolean_operation.hpp"
#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_coord_minkowski.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"
#include "SCGAL/Transformer_with_exact_coords.hpp"
#include "SCGAL/Triangulation_geo.hpp"
#if defined(SGAL_USE_ENV)
#include "SCGAL/Lower_envelope_tri_geo.hpp"
#include "SCGAL/Lower_envelope_sphere_geo.hpp"
#include "SCGAL/Lower_envelope_plane_geo.hpp"
#endif
#if defined(SGAL_USE_CGM)
#include "SCGAL/Cubical_gaussian_map_geo.hpp"
#endif
#if defined(SGAL_USE_LTS)
#include "SCGAL/Lines_through_segments_geo.hpp"
#endif
#if defined(SGAL_USE_SGM)
#include "SCGAL/Spherical_gaussian_map_geo.hpp"
#include "SCGAL/Spherical_gaussian_map_colored_geo.hpp"
#include "SCGAL/Spherical_gaussian_map_marked_geo.hpp"
#include "SCGAL/Assembly.hpp"
#include "SCGAL/Assembly_part.hpp"
#endif
#if defined(SGAL_USE_AOT)
#include "SCGAL/Arrangement_on_torus_geo.hpp"
#endif
#if defined(SGAL_USE_AOS)
#include "SCGAL/Arrangement_on_sphere_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_marked_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_overlay_geo.hpp"
#include "SCGAL/Arrangement_on_sphere_sim_geo.hpp"
// Ignore in Debug mode, cause is_valid() on the sphere does not compile
#if defined(NDEBUG)
#include "SCGAL/Polygon_set_on_sphere_geo.hpp"
#endif
#endif
#if defined(SGAL_USE_POS)
#include "SCGAL/Arrangement_of_polygeodesics_on_sphere_geo.hpp"
#endif
#if defined(SGAL_USE_VOS)
#include "SCGAL/Voronoi_diagram_on_sphere_geo.hpp"
#include "SCGAL/Power_diagram_on_sphere_geo.hpp"
#endif
#if defined(SGAL_USE_NEF)
#include "SCGAL/Exact_nef_polyhedron.hpp"
#if defined(SGAL_USE_NGM)
#include "SCGAL/Nef_gaussian_map_geo.hpp"
#endif
#endif
#if defined(SGAL_USE_QUADRIC)
#include "SCGAL/Quadric_geo.hpp"
#include "SCGAL/Arrangement_on_quadric_geo.hpp"
#endif
#if defined(SGAL_USE_SSC)
#include "SCGAL/Smallest_stabbing_cube.hpp"
#endif
#include "SCGAL/Surface_border.hpp"

SGAL_BEGIN_NAMESPACE

// extern "C" void BOOST_EXTENSION_EXPORT_DECL scgal_init()
// void scgal_init()
SGAL_SCGAL_DECL void scgal_init()
{
  Container_factory::get_instance()->doregister(Boolean_operation::prototype());
  Container_factory::get_instance()->doregister(Polyhedron_geo::prototype());
  Container_factory::get_instance()->doregister(Exact_coord_minkowski::prototype());
  Container_factory::get_instance()->doregister(Exact_polyhedron_geo::prototype());
  Container_factory::get_instance()->doregister(Transformer_with_exact_coords::prototype());
  Container_factory::get_instance()->doregister(Triangulation_geo::prototype());
#if defined(SGAL_USE_ENV)
  Container_factory::get_instance()->doregister(Lower_envelope_tri_geo::prototype());
  Container_factory::get_instance()->doregister(Lower_envelope_sphere_geo::prototype());
  Container_factory::get_instance()->doregister(Lower_envelope_plane_geo::prototype());
#endif
#if defined(SGAL_USE_CGM)
  Container_factory::get_instance()->doregister(Cubical_gaussian_map_geo::prototype());
#endif
#if defined(SGAL_USE_LTS)
  Container_factory::get_instance()->doregister(Lines_through_segments_geo::prototype());
#endif
#if defined(SGAL_USE_SGM)
  Container_factory::get_instance()->doregister(Spherical_gaussian_map_geo::prototype());
  Container_factory::get_instance()->doregister(Spherical_gaussian_map_colored_geo::prototype());
  Container_factory::get_instance()->doregister(Spherical_gaussian_map_marked_geo::prototype());
  Container_factory::get_instance()->doregister(Assembly::prototype());
  Container_factory::get_instance()->doregister(Assembly_part::prototype());
#endif
#if defined(SGAL_USE_AOT)
  Container_factory::get_instance()->doregister(Arrangement_on_torus_geo::prototype());
#endif
#if defined(SGAL_USE_AOS)
  Container_factory::get_instance()->doregister(Arrangement_on_sphere_geo::prototype());
  Container_factory::get_instance()->doregister(Arrangement_on_sphere_marked_geo::prototype());
  Container_factory::get_instance()->doregister(Arrangement_on_sphere_overlay_geo::prototype());
  Container_factory::get_instance()->doregister(Arrangement_on_sphere_sim_geo::prototype());
// Ignore in Debug mode, cause is_valid() on the sphere does not compile
#if defined(NDEBUG)
  Container_factory::get_instance()->doregister(Polygon_set_on_sphere_geo::prototype());
#endif
#endif
#if defined(SGAL_USE_POS)
  Container_factory::get_instance()->doregister(Arrangement_of_polygeodesics_on_sphere_geo::prototype());
#endif
#if defined(SGAL_USE_VOS)
  Container_factory::get_instance()->doregister(Voronoi_diagram_on_sphere_geo::prototype());
  Container_factory::get_instance()->doregister(Power_diagram_on_sphere_geo::prototype());
#endif
#if defined(SGAL_USE_NEF)
  Container_factory::get_instance()->doregister(Exact_nef_polyhedron::prototype());
#if defined(SGAL_USE_NGM)
  Container_factory::get_instance()->doregister(Nef_gaussian_map_geo::prototype());
#endif
#endif
#if defined(SGAL_USE_QUADRIC)
  Container_factory::get_instance()->doregister(Quadric_geo::prototype());
  Container_factory::get_instance()->doregister(Arrangement_on_quadric_geo::prototype());
#endif
#if defined(SGAL_USE_SSC)
  Container_factory::get_instance()->doregister(Smallest_stabbing_cube::prototype());
#endif
  Container_factory::get_instance()->doregister(Surface_border::prototype());
}

SGAL_END_NAMESPACE
