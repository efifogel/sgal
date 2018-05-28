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

#ifndef SGAL_DXF_LEADER_ENTITY_HPP
#define SGAL_DXF_LEADER_ENTITY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_leader_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  String m_dimension_style_name; // Dimension style name
  int16_t m_use_arrowheads; // Arrowhead flag: 0 = Disabled; 1 = Enabled
  int16_t m_path_type;  // Leader path type: 0 = Straight line segments;
                        // 1 = Spline
  int16_t m_annotation_type; // Leader creation flag (default = 3):
                        // 0 = Created with text annotation
                        // 1 = Created with tolerance annotation
                        // 2 = Created with block reference annotation
                        // 3 = Created without any annotation
  int16_t m_hookline_direction; // Hookline direction flag:
                        // 0 = Hookline (or end of tangent for a splined leader)
                        //     is the opposite direction from the horizontal
                        //     vector
                        // 1 = Hookline (or end of tangent for a splined leader)
                        //     is the same direction as horizontal vector (see
                        //     code 75)
  int16_t m_hookline;   // Hookline flag:
                        // 0 = No hookline
                        // 1 = Has a hookline
  double m_text_annotation_height; // Text annotation height
  double m_text_annotation_width; // Text annotation width
  int16_t m_vertex_count; // Number of vertices in leader (ignored for OPEN)
  std::vector<std::array<double, 3> > m_vertices; // Vertex coordinates (one
                        // entry for each vertex)
  int16_t m_override_color; // Color to use if leader's DIMCLRD = BYBLOCK
  Uint m_associated_annotation_reference; // Hard reference to associated
                        // annotation (mtext, tolerance, or insert entity)
  double m_normal[3];   // Normal vector
  double m_right[3];    // "Horizontal" direction for leader
  double m_block_offset[3]; // Offset of last leader vertex from block reference
                        // insertion point
  double m_annotation_offset[3]; //  Offset of last leader vertex from
                        // annotation placement point
};

SGAL_END_NAMESPACE

#endif
