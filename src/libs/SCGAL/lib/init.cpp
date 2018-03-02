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
#include "SCGAL/Exact_coord_minkowski.hpp"
#include "SCGAL/Transformer_with_epec_coords.hpp"
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

SGAL_BEGIN_NAMESPACE

// extern "C" void BOOST_EXTENSION_EXPORT_DECL scgal_init()
// void scgal_init()
SGAL_SCGAL_DECL void scgal_init()
{
  REGISTER_OBJECT(Boolean_operation);
  REGISTER_OBJECT(Exact_coord_minkowski);
  REGISTER_OBJECT(Transformer_with_epec_coords);
  REGISTER_OBJECT(Triangulation_geo);
#if defined(SGAL_USE_ENV)
  REGISTER_OBJECT(Lower_envelope_tri_geo);
  REGISTER_OBJECT(Lower_envelope_sphere_geo);
  REGISTER_OBJECT(Lower_envelope_plane_geo);
#endif
#if defined(SGAL_USE_CGM)
  REGISTER_OBJECT(Cubical_gaussian_map_geo);
#endif
#if defined(SGAL_USE_LTS)
  REGISTER_OBJECT(Lines_through_segments_geo);
#endif
#if defined(SGAL_USE_SGM)
  REGISTER_OBJECT(Spherical_gaussian_map_geo);
  REGISTER_OBJECT(Spherical_gaussian_map_colored_geo);
  REGISTER_OBJECT(Spherical_gaussian_map_marked_geo);
  REGISTER_OBJECT(Assembly);
  REGISTER_OBJECT(Assembly_part);
#endif
#if defined(SGAL_USE_AOT)
  REGISTER_OBJECT(Arrangement_on_torus_geo);
#endif
#if defined(SGAL_USE_AOS)
  REGISTER_OBJECT(Arrangement_on_sphere_geo);
  REGISTER_OBJECT(Arrangement_on_sphere_marked_geo);
  REGISTER_OBJECT(Arrangement_on_sphere_overlay_geo);
  REGISTER_OBJECT(Arrangement_on_sphere_sim_geo);
// Ignore in Debug mode, cause is_valid() on the sphere does not compile
#if defined(NDEBUG)
  REGISTER_OBJECT(Polygon_set_on_sphere_geo);
#endif
#endif
#if defined(SGAL_USE_POS)
  REGISTER_OBJECT(Arrangement_of_polygeodesics_on_sphere_geo);
#endif
#if defined(SGAL_USE_VOS)
  REGISTER_OBJECT(Voronoi_diagram_on_sphere_geo);
  REGISTER_OBJECT(Power_diagram_on_sphere_geo);
#endif
#if defined(SGAL_USE_NEF)
  REGISTER_OBJECT(Exact_nef_polyhedron);
#if defined(SGAL_USE_NGM)
  REGISTER_OBJECT(Nef_gaussian_map_geo);
#endif
#endif
#if defined(SGAL_USE_QUADRIC)
  REGISTER_OBJECT(Quadric_geo);
  REGISTER_OBJECT(Arrangement_on_quadric_geo);
#endif
#if defined(SGAL_USE_SSC)
  REGISTER_OBJECT(Smallest_stabbing_cube);
#endif
}

SGAL_END_NAMESPACE
