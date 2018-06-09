// Copyright (c) 2018 Israel.
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

#ifndef SGAL_DXF_INSERT_ENTITY_HPP
#define SGAL_DXF_INSERT_ENTITY_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_entity.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_insert_entity : public Dxf_base_entity {
  typedef Dxf_base_entity                       Base;

  int16_t m_has_attributes; // Variable attributes-follow flag (optional;
                        // default = 0); if the value of attributes-follow flag
                        // is 1, a series of attribute entities is expected to
                        // follow the insert, terminated by a seqend entity
  String m_name;        // Block name
  double m_location[3]; //  Insertion point (in OCS)
  double m_x_scale_factor; // X scale factor (optional; default = 1)
  double m_y_scale_factor; // Y scale factor (optional; default = 1)
  double m_z_scale_factor; // Z scale factor (optional; default = 1)
  double m_rotation;    // Rotation angle (optional; default = 0)
  int16_t m_column_count; // Column count (optional; default = 1)
  int16_t m_row_count;  // Row count (optional; default = 1)
  double m_column_spacing; // Column spacing (optional; default = 0)
  double m_row_spacing; // Row spacing (optional; default = 0)
  double m_extrusion_direction[3]; // Extrusion direction (optional;
                        // default = 0, 0, 1)
};

SGAL_END_NAMESPACE

#endif
