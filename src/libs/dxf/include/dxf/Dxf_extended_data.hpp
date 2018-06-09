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

#ifndef SGAL_DXF_EXTENDED_DATA_HPP
#define SGAL_DXF_EXTENDED_DATA_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

struct Dxf_extended_data {
  String m_name;        // Strings in extended data can be up to 255 bytes long
                        // (with the 256th byte reserved for the null character).
  String m_layer_name;  // Name of a layer associated with the xdata.
  String m_entity_handle; //  Handle of an entity in the drawing database.
  double m_point[3];    // A point
  double m_real;        // A real value.
  int16_t m_integer;    // A 16-bit integer (signed or unsigned)
  int32_t m_long;       // A 32-bit signed (long) integer. If the value that
                        // appears in a 1071 group code is a short integer or
                        // real value, it is converted to a long integer; if it
                        // is invalid (for example, a string), it is converted
                        // to a long zero (0L).
  String m_binary_data; // Binary data organized into variable-length chunks,
                        // which can be handled in ObjectARX and Managed .NET
                        // with the ads_binary structure.
                        // The maximum length of each chunk is 127 bytes.
                        // Note: AutoLISP cannot directly handle binary chunks,
                        // so the same precautions that apply to long (1071)
                        // group codes apply to binary group codes as well.
  double m_worldspace_position[3]; // Unlike a simple 3D point, the WCS
                        // coordinates are moved, scaled, rotated, and mirrored
                        // along with the parent entity to which the extended
                        // data belongs. The WCS position is also stretched when
                        // the AutoCAD STRETCH command is applied to the parent
                        // entity and when this point lies within the select
                        // window.
  double m_worldspace_displacement[3]; // A 3D point that is scaled, rotated, or
                        // mirrored along with the parent, but not stretched or
                        // moved.
  double m_world_direction[3]; // A 3D point that is rotated or mirrored along
                        // with the parent, but not scaled, stretched, or moved.
                        // The WCS direction is a normalized displacement that
                        // always has a unit length.
  double m_distance;    // A real value that is scaled along with the parent
                        // entity.
  double m_scale_factor; // Also a real value that is scaled along with the
                        // parent.
};

SGAL_END_NAMESPACE

#endif
