// Copyright (c) 2004,2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_VERTEX_ENTITY_HPP
#define SGAL_DXF_VERTEX_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_vertex_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  double m_location[3]; // Location point (in OCS when 2D, and WCS when 3D)
  double m_starting_width; // Starting width (optional; default is 0)
  double m_ending_width; // Ending width (optional; default is 0)
  double m_bulge;       // Bulge (optional; default is 0). The bulge is the
                        // tangent of one fourth the included angle for an arc
                        // segment, made negative if the arc goes clockwise from
                        // the start point to the endpoint. A bulge of 0
                        // indicates a straight segment, and a bulge of 1 is a
                        // semicircle
  int16_t m_flags;       // Vertex flags:
                        // 1 = Extra vertex created by curve-fitting
                        // 2 = Curve-fit tangent defined for this vertex. A
                        //     curve-fit tangent direction of 0 may be omitted
                        //     from DXF output but is significant if this bit is
                        //     set
                        // 4 = Not used
                        // 8 = Spline vertex created by spline-fitting
                        // 16 = Spline frame control point
                        // 32 = 3D polyline vertex
                        // 64 = 3D polygon mesh
                        // 128 = Polyface mesh vertex
  double m_curve_fit_tangent_direction; // Curve fit tangent direction
  int16_t m_polyface_mesh_vertex_index1; // Polyface mesh vertex index
                        // (optional; present only if nonzero)
  int16_t m_polyface_mesh_vertex_index2; // Polyface mesh vertex index
                        // (optional; present only if nonzero)
  int16_t m_polyface_mesh_vertex_index3; // Polyface mesh vertex index
                        // (optional; present only if nonzero)
  int16_t m_polyface_mesh_vertex_index4; // Polyface mesh vertex index
                        // (optional; present only if nonzero)
  int32_t m_identifier; // Vertex identifier
};

SGAL_END_NAMESPACE

#endif
